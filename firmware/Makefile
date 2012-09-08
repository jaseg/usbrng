
all: objects


objects: srsly/*.c main.c
	avr-gcc -Wall -fshort-enums -fno-inline-small-functions -fpack-struct -Wall -fno-strict-aliasing -funsigned-char -funsigned-bitfields -ffunction-sections -mmcu=atmega16u2 -DFDEV_SETUP_STREAM -DF_USB=16000000 -DF_CPU=16000000 -std=gnu99 -Os -o main.elf -Wl,--gc-sections,--relax $^
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

clean:
	rm main.elf main.hex
