CC=gcc
CFLAGS=-Wall -L/usr/lib/x86_64-linux-gnu -lfreetype -lz -I/usr/include/freetype2 -lm
ARCH=x86

all:
	$(CC) $(CFLAGS) -o demo-$(ARCH) file.c gfx.c input.c main.c

clean:
	rm demo-$(ARCH)

