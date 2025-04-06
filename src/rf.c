#include "rf.h"
#include "command.h"
#include <hardware/gpio.h>
#include <hardware/irq.h>
#include <hardware/timer.h>
#include <pico/sem.h>
#include <pico/time.h>
#include <stdlib.h>
#include <stdio.h>

static struct rf_cfg default_cfg = {
    .pulse_len_us = 400,
    .tx_repetitions = 4,
    .state = INVALID,
    .one =
        {
            .high_count = 3,
            .low_count = 1,
        },
    .zero =
        {
            .high_count = 1,
            .low_count = 3,
        },
    .sync =
        {
            .high_count = 1,
            .low_count = 30,
        },
};

static struct rf_cfg *cfg;

static unsigned int timings[MAX_CHANGES];

static void send(struct rf_cfg *cfg, enum Pulse pulse) {
  struct rf_logic_pulse *target_pulse;

  switch (pulse) {
  case ONE:
    target_pulse = &cfg->one;
    break;
  case ZERO:
    target_pulse = &cfg->zero;
    break;
  case SYNC:
    target_pulse = &cfg->sync;
    break;
  }

  const unsigned long long high_delay =
      cfg->pulse_len_us * target_pulse->high_count;
  const unsigned long long low_delay =
      cfg->pulse_len_us * target_pulse->low_count;

  gpio_put(cfg->tx_pin, 1);
  busy_wait_us(high_delay);
  gpio_put(cfg->tx_pin, 0);
  busy_wait_us(low_delay);
}

static inline unsigned int diff(int A, int B) { return abs(A-B); }

static void process_interrupt(unsigned int change_count) {
  unsigned long code = 0;
  const unsigned int sync_len_pulses = cfg->sync.low_count;
  const unsigned int delay = timings[0] / sync_len_pulses;
  const unsigned int delay_tolerance = delay * RECEIVE_TOLERANCE_PERCENT / 100;

  for (unsigned int i = 1; i < change_count - 1; i += 2) {
    code <<= 1;

    if (diff(timings[i], delay * cfg->zero.high_count) < delay_tolerance && diff(timings[i + 1], delay * cfg->zero.low_count) < delay_tolerance) {
      // zero
    } else if (diff(timings[i], delay * cfg->one.high_count) < delay_tolerance && diff(timings[i + 1], delay * cfg->one.low_count) < delay_tolerance) {
      // one      
      code |= 1;
    } else {
      return;
    }
  }

  if (change_count > 7) {
    const enum Command cmd = cmd_from_int(code);
    if (cmd == INVALID) {
      printf("invalid command code: %lu\n", code);
      return;
    }

    const char* cmd_str = cmd_to_str(cmd);
    printf("received command %s (%lu)\n", cmd_str, code);

    cfg->state = cmd;
  }
}

static void interrupt_handler(uint gpio, uint32_t event_type) {
  static unsigned int change_count = 0;
  static unsigned long long last_time = 0;
  static unsigned int repeat_count = 0;

  const unsigned long long time = get_absolute_time();
  const unsigned int duration = time - last_time;

  if (duration > SEPARATION_LIMIT) {
    if ((repeat_count == 0) || (diff(duration, timings[0]) < 200)) {
      repeat_count++;

      if (repeat_count == 2) {
       process_interrupt(change_count);       

       repeat_count = 0;
      }
    }

    change_count = 0;
  }

  if (change_count >= MAX_CHANGES) {
    change_count = 0;
    repeat_count = 0;
  }

  timings[change_count++] = duration;
  last_time = time;
}

struct rf_cfg* init_default_rf_cfg(unsigned int rx_pin, unsigned int tx_pin) {
  cfg = &default_cfg;
  cfg->rx_pin = rx_pin;
  cfg->tx_pin = tx_pin;

  gpio_init(cfg->rx_pin);
  gpio_set_pulls(cfg->rx_pin, false, true);

  gpio_init(cfg->tx_pin);
  gpio_set_dir(cfg->tx_pin, GPIO_OUT);
  gpio_set_drive_strength(cfg->tx_pin, GPIO_DRIVE_STRENGTH_12MA);

  return cfg;
}

void enable_rx(void) {
  if (cfg->rx_ever_enabled) {
    gpio_set_irq_enabled(cfg->rx_pin, IRQ_EVENT_MASK, true);
  } else {
    gpio_set_irq_enabled_with_callback(cfg->rx_pin, IRQ_EVENT_MASK, true,
                                     interrupt_handler);
    cfg->rx_ever_enabled = true;
  }

  cfg->rx_enabled = true;
}

void disable_rx(void) {
  gpio_set_irq_enabled(cfg->rx_pin, IRQ_EVENT_MASK, false);
  cfg->rx_enabled = false;
}

void send_cmd(enum Command cmd) {
  bool should_reenable_rx = false;
  if (cfg->rx_enabled) {
    printf("disabling rx\n");
    disable_rx();
    printf("rx disabled\n");
    should_reenable_rx = true;
  }

  for (int r = 0; r < cfg->tx_repetitions; r++) {
    for (int i = 23; i >= 0; i--) {
      if ((cmd >> i) & 1) {
        printf("about to send 1\n");
        send(cfg, ONE);
        printf("sent 1\n");
      } else {
        printf("about to send 0\n");
        send(cfg, ZERO);
        printf("sent 0\n");
      }
    }

    printf("about to send sync\n");
    send(cfg, SYNC);
    printf("sent sync\n");
  }

  if (should_reenable_rx) {
    printf("re-enabling rx\n");
    enable_rx();
    printf("rx re-enabled\n");
  }

  cfg->state = cmd;
}
