#!/bin/sh
SOURCE=$1
MMCU=attiny2313
AVRDUDE_PARAMETERS=-p t2313 -c stk500v1 -P /dev/ttyACM0 -b 19200

if [ ! -f "$SOURCE.c" ]; then
	echo "Usage: source without extension"
	exit 1
fi

# compile
avr-gcc -mmcu=$MMCU -Wall -Os -o $SOURCE.elf $SOURCE.c

[ $? -ne 0 ] && echo "Compiler failed" && exit 1

# make firmware (extract .data and .text section)
avr-objcopy -j .text -j .data -O ihex $SOURCE.elf $SOURCE.hex

[ $? -ne 0 ] && echo "Packing failed" && exit 1

# upload
echo "Press reset on arduino"
avrdude -v -v -U flash:w:$SOURCE.hex $AVRDUDE_PARAMETERS

[ $? -ne 0 ] && echo "Uploader failed" && exit 1
