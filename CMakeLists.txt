cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(rfserver LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()
add_executable(rfserver src/main.c src/rf.c src/http.c src/command.c lib/mongoose/mongoose.c)
# add_executable(rfserver main.c rf.c mongoose/mongoose.c)
target_include_directories(rfserver PUBLIC lib include)

target_compile_definitions(rfserver PUBLIC WIFI_PASS=$ENV{WIFI_PASS} WIFI_SSID=$ENV{WIFI_SSID} )

target_link_libraries(rfserver
  pico_stdlib
  hardware_spi
  pico_rand
  pico_cyw43_driver
  pico_cyw43_arch_none
  hardware_flash
  pico_bootrom
)

pico_add_extra_outputs(rfserver)     # create map/bin/hex file etc.

pico_enable_stdio_usb(rfserver 1)    # Route stdio

set(PICO_USE_NEWLIB_NANO 1)

set_property(TARGET rfserver APPEND_STRING PROPERTY LINK_FLAGS "-Wl,--print-memory-usage")



# # Prevent extreme optimization from breaking OTA
# target_compile_options(rfserver PRIVATE -fno-rtti -fno-exceptions -Os)
