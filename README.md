
Purpose
=======
usbrng is a low-cost easy-to-build usb stick generating a truly random bit stream. With the included Linux kernel module you can use usbrng to seed your kernel's random number generator.

Prerequisites
=============
To build the kernel module you need the linux kernel headers, normally you should be able to find them in a package called "linux-headers" or alike.
To build the firmware you need the avr-libc and avr-gcc/binutils-avr. To upload the firmware to the device you need avrdude.
The board layout and schematics were done using gEDA.

Building
========
This repository uses git submodules, so in order to build it you first have update them:
```
~/projects/usbrng
<3 git submodule init
~/projects/usbrng
<3 git submodule update
```
After that, just type ```make``` in the top-level directory or one of the subdirectories and the thing should build.

To flash the firmware do ```make flash```.

Todo
====
 * We still need a nice name for the project. "usbrng" somehow sounds crappy.

