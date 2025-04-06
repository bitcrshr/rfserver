#ifndef _RF_H
#define _RF_H

#include <stdbool.h>
#include <stdint.h>

#include "command.h"

#define MAX_CHANGES 67
#define SEPARATION_LIMIT 11000
#define RECEIVE_TOLERANCE_PERCENT 60
#define IRQ_EVENT_MASK GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL

void send_cmd(enum Command cmd);

struct rf_logic_pulse {
  uint8_t high_count;
  uint8_t low_count;
};

struct rf_cfg {
  bool rx_enabled;
  bool rx_ever_enabled;

  unsigned int rx_pin;
  unsigned int tx_pin;
  unsigned int pulse_len_us;
  unsigned int tx_repetitions;

  struct rf_logic_pulse one;
  struct rf_logic_pulse zero;
  struct rf_logic_pulse sync;

  enum Command state;
};


struct rf_cfg* init_default_rf_cfg(unsigned int rx_pin, unsigned int tx_pin);

void enable_rx(void);
void disable_rx(void);

enum Pulse {
  ONE,
  ZERO,
  SYNC,
};

#endif
