
all: module firmware


module:
	make -C kernel


firmware:
	make -C firmware

flash:
	make -C firmware flash

clean:
	make -C kernel clean
	make -C firmware clean
