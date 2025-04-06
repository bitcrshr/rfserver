#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_PICOSDK
#define MG_OTA MG_OTA_PICOSDK

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_PICO_W 1
#define MG_IO_SIZE 512

#define MG_ENABLE_LOG 1

extern void main_setcreds(char **ssid, char **pass);
#define MG_SET_WIFI_CREDS(ssid, pass) main_setcreds(ssid, pass)
