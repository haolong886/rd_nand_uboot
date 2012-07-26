#!/bin/bash

make CROSS_COMPILE=arm-none-linux-gnueabi- mrproper
make CROSS_COMPILE=arm-none-linux-gnueabi- clean
make CROSS_COMPILE=arm-none-linux-gnueabi- omap3_beagle_config
make CROSS_COMPILE=arm-none-linux-gnueabi-
#cp u-boot.bin /media/usr_fs/tmp/
