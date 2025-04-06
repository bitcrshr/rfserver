#include <pico/stdio.h>
#include "http.h"
#include "rf.h"
#include <pico/stdlib.h>
#include <pico/malloc.h>

#ifndef WIFI_SSID
#error 1
#endif

#ifndef WIFI_PASS
#error 1
#endif

void main_setcreds(char **ssid, char **pass) {
  *ssid = WIFI_SSID;
  *pass = WIFI_PASS;
}


int main() {
  stdio_init_all();

  struct rf_cfg *cfg = init_default_rf_cfg(0, 17);
  enable_rx();

  listen("http://localhost:8000", cfg);

  return 0;
}
