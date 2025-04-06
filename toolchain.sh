#!/bin/bash

ARM_GNU_TOOLCHAIN_DOWNLOAD_URL="https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-darwin-arm64-arm-none-eabi.tar.xz"

test -d toolchain && exit 0

test -f toolchain.tar.xz || (wget $ARM_GNU_TOOLCHAIN_DOWNLOAD_URL -O toolchain.tar.xz || rm toolchain.tar.xz)

mkdir toolchain

tar -xJf toolchain.tar.xz -C toolchain --strip-components 1 && rm toolchain.tar.xz

