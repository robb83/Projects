// gcc -Wall -std=c99 -O2 -o mouse mouse.c

/*
 * http://www.computer-engineering.org/ps2mouse/
 */
#include <stdio.h>
#include <unistd.h>
#include <linux/input.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

volatile int terminate = 0;
volatile int device = -1;

void cleanup() {
	int d = device;
	if (d > -1) {
		device = -1;
		close(d);
	}
	exit(0);
}

void signal_handler(int signum) {	
	terminate = 1;
	cleanup();
}

int main(int argc, char** argv) {
	int readed;
	char buffer[256];
	char deviceFile[] = "/dev/input/mice";
		
	signal(SIGINT, signal_handler);
		
	device = open(deviceFile, O_RDONLY);
	if (device < 0) {
		printf("Can't open device: %s \n", deviceFile);
		perror("\tMessage: ");
		return -1;
	}
	
	while(!terminate) {
		readed = read(device, buffer, sizeof(buffer));
		if (readed > 0) {
			int xoffset = buffer[1];
			int yoffset = buffer[2];
			int btn1 = (buffer[0] & 0x04) >> 2;
			int btn2 = (buffer[0] & 0x02) >> 1;
			int btn3 = (buffer[0] & 0x01);
			
			printf("xoffset: %d, yoffset: %d, btn1: %d, btn2: %d, btn3: %d (%d)\n", xoffset, yoffset, btn1, btn2, btn3, readed);
		}
	}
	
	cleanup();
	
	return 0;
}
