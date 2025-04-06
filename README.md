# rfserver

Raspberry Pi Pico W firmware that can control LED lights over 433mhz. If light's actual remote is used, it keeps track of that state.

RF stuff heavily inspired by / ported from [rc-switch](https://github.com/sui77/rc-switch). Many thanks for their implementation--it would have taken me ages to figure this out otherwise.

This was designed to work for the remotes described in [this fcc report](https://fcc.report/FCC-ID/2AUSN-LXZK1021/4490455.pdf). In particular, I have this one, which controls a fish tank light:
<img width="142" alt="image" src="https://github.com/user-attachments/assets/7c01e1c1-cc67-414c-a56c-d56dde1a09e1" />

Exposes two endpoints for controlling/reading the state (hoping to be rid of this soon in favor of MQTT):
- POST /command/exec with a body of `COMMAND_{the command}`
- GET /state

## Building / Flashing
- The Makefile is configured to download the ARM GNU toolchain automatically if not present, and it also configures CMake to clone the pico sdk if needed. You may want to install these on your system globally instead and adjust the Makefile accordingly. Also note that `toolchain.sh` is configured to download the toolchain for Apple Silicon.
- I haven't tried this on anything other than MacOS with Apple Silicon, YMMV on other platforms.
- With the Pico W in bootloader mode, you can run `make flash`. Requires [picotool](https://github.com/raspberrypi/picotool) to be on the path.

## Components used

To actually decode the protocol, I followed [this tutorial](https://www.codrey.com/electronic-circuits/433mhz-rf-sniffer/) to the tee.

The components used on the "final product" are just [these from amazon](https://www.amazon.com/dp/B0BLTSSMXH). Note that the sniffer I built used [these slightly cheaper, much worse ones](https://www.amazon.com/dp/B01DKC2EY4).

### A warning

*The Pico W has a logic level of 3.3v--it is NOT safe to have a 5v input.*

This may become a problem for me at some point, as I did not realize this until *after* I had already soldered everything together. It's working for now, but might kill that GPIO (or worse) at some point in the future.

The transmitter runs off of 5V, and the receiver runs off of 3.3v.

## TODO
- Use MQTT instead of HTTP, integrate with HomeAssistant
- FreeRTOS
- OTA
- Maybe use an ESP32 instead and expose as a Matter device
