// gcc -Wall -std=c99 -O2 -o input input.c
/*
 * http://www.linuxjournal.com/article/6429
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <fcntl.h>
#include <signal.h>

#define DEVICE_FILE "/dev/input/event4"

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
	int i;
	ssize_t readed;
	char buffer[256];
	struct input_event event;
	struct input_id id;
	long int eventtypes = 0;
	char *deviceFile;
	
	signal(SIGINT, signal_handler);
	
	if (argc > 1) {
		deviceFile = argv[1];
	} else {
		deviceFile = DEVICE_FILE;
	}

	device = open(deviceFile, O_RDONLY);
	if (device < 0) {
		printf("Can't open device: %s \n", deviceFile);
		perror("\tMessage: ");
		return -1;
	}

	if (ioctl(device, EVIOCGID, &id) < 0) {
		perror("Invalid ioctl call: ");
		return -1;
	}

	printf("type: %d, vendor: %d, product: %d, version: %d\n", id.bustype, id.vendor, id.product, id.version);

	if (ioctl(device, EVIOCGNAME(sizeof(buffer)), buffer) < 0) {
		perror("Invalid ioctl call: ");
		return -1;
	}

	printf("Device name: %s\n", buffer);

	if (ioctl(device, EVIOCGPHYS(sizeof(buffer)), buffer) < 0) {
		perror("Invalid ioctl call: ");
		return -1;
	}

	printf("Physical location: %s\n", buffer);

	if (ioctl(device, EVIOCGUNIQ(sizeof(buffer)), buffer) < 0) {
		perror("Invalid ioctl call: ");
		return -1;
	}

	printf("Unique identifier: %s\n", buffer);

	if (ioctl(device, EVIOCGPROP(sizeof(buffer)), buffer) < 0) {
		perror("Invalid ioctl call: ");
		return -1;
	}

	printf("Properties: %s\n", buffer);

	if (ioctl(device, EVIOCGBIT(0, EV_MAX), &eventtypes) < 0) {
		perror("Invalid ioctl call: ");
		return -1;
	}

	printf("Event types:\n");

	for(i=0;i<sizeof(eventtypes);i++) {
		switch (eventtypes & i) {
			case EV_SYN:
				printf("\tEV_SYN\n");
				break;
			case EV_KEY:
				printf("\tEV_KEY\n");
				break;
			case EV_REL:
				printf("\tEV_REL\n");
				break;
			case EV_ABS:
				printf("\tEV_ABS\n");
				break;
			case EV_MSC:
				printf("\tEV_MSC\n");
				break;
			case EV_SW:
				printf("\tEV_SW\n");
				break;
			case EV_LED:
				printf("\tEV_LED\n");
				break;
			case EV_SND:
				printf("\tEV_SND\n");
				break;
			case EV_REP:
				printf("\tEV_REP\n");
				break;
			case EV_FF:
				printf("\tEV_FF\n");
				break;
			case EV_PWR:
				printf("\tEV_PWR\n");
				break;
			case EV_FF_STATUS:
				printf("\tEV_FF_STATUS\n");
				break;
			default:
				printf("Unknow event type");
				break;
			}
	}

	while(!terminate) {
		readed = read(device, &event, sizeof(struct input_event));
		if (readed > 0) {
			if (event.type == 0) {
				printf("SYNC_REPORT\n");
			} else {
				printf("type: %d, code: %d\n", event.type, event.code);
			}
		}
	}
	
	cleanup();
	
	return 0;
}
