# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

CWD = $(realpath $(PWD))
PICO_SDK_VERSION = 2.1.1
RM = rm -rf
MKBUILD = test -d build || mkdir build

include wifi_creds.env
export


.PHONY: build

all build: build/rfserver.uf2

build/rfserver.uf2: $(wildcard *.c) $(wildcard *.h) CMakeLists.txt Makefile
	$(MKBUILD)

	./toolchain.sh && cd build && PICO_SDK_FETCH_FROM_GIT_PATH="./pico-sdk" PICO_SDK_FETCH_FROM_GIT="ON" PICO_SDK_FETCH_FROM_GIT_TAG=$(PICO_SDK_VERSION) PICO_TOOLCHAIN_PATH="$(CWD)/toolchain" cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DPICO_BOARD="pico_w" "-DWIFI_SSID=${WIFI_SSID}" -DWIFI_PASS="${WIFI_PASS}" -G "Unix Makefiles" .. && make

flash: build/rfserver.uf2
	picotool load $< -f

clean:
	rm -rf build .cache 


