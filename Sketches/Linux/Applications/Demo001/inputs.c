#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <dirent.h>
#include <string.h>
//#include <stropts.h>
#include <poll.h>

// input_id
#define BITS_PER_LONG        (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1) 
#define OFF(x) ((x)%BITS_PER_LONG) 
#define BIT(x) (1UL<<OFF(x)) 
#define LONG(x) ((x)/BITS_PER_LONG) 
#define test_bit(bit, array) ((array[LONG(bit)] >> OFF(bit)) & 1)

struct buff {
	int length;
	char* buffer;
};

static struct buff cells;

void search_input(char *path)
{
	DIR *dir;
	char buffer[256];
	struct dirent *ent;
	if ((dir = opendir (path)) != NULL)
	{
		while ((ent = readdir (dir)) != NULL)
		{
			if (ent->d_type == DT_CHR)
			{
				printf("Name: %s, type: %d.\n", ent->d_name, ent->d_type);
				snprintf(buffer, sizeof(buffer)-1, "%s/%s", path, ent->d_name);
				print_input_info(buffer);
			}
		}
		closedir (dir);
	}
}

int print_input_info(char* filename)
{
	int fd, yalv;
	char evtype_b[256];
	char name[256]= "Unknown";
	int have_abs = 0;
	struct input_absinfo absinfo;

	if ((fd = open(filename, O_RDONLY)) < 0) 
	{
		return -1;
	}

	if(ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0)
	{
		return -1;
	}

	printf("Device: %s, name: %s.\n", filename, name);

	memset(evtype_b, 0, sizeof(evtype_b));
	if (ioctl(fd, EVIOCGBIT(0, EV_MAX), evtype_b) < 0) {
		perror("evdev ioctl");
		return -1;
	}

	printf("Supported event types:\n");

	for (yalv = 0; yalv < EV_MAX; yalv++) {
		if (test_bit(yalv, evtype_b)) {
			/* the bit is set in the event types list */
			printf("  Event type 0x%02x ", yalv);
			switch ( yalv)
				{
				case EV_SYN :
					printf(" (Synch Events)\n");
					break;
				case EV_KEY :
					printf(" (Keys or Buttons)\n");
					break;
				case EV_REL :
					printf(" (Relative Axes)\n");
					break;
				case EV_ABS :
					have_abs = 1;
					printf(" (Absolute Axes)\n");
					break;
				case EV_MSC :
					printf(" (Miscellaneous)\n");
					break;
				case EV_LED :
					printf(" (LEDs)\n");
					break;
				case EV_SND :
					printf(" (Sounds)\n");
					break;
				case EV_REP :
					printf(" (Repeat)\n");
					break;
				case EV_FF :
				case EV_FF_STATUS:
					printf(" (Force Feedback)\n");
					break;
				case EV_PWR:
					printf(" (Power Management)\n");
					break;
				default:
					printf(" (Unknown: 0x%04hx)\n",
				 yalv);
				}
		}
	}
	if (have_abs)
	{
		ioctl( fd, EVIOCGABS(ABS_X), &absinfo );
		printf("X: minimum: %d, maximum: %d\n", absinfo.minimum, absinfo.maximum);
		ioctl( fd, EVIOCGABS(ABS_Y), &absinfo );
		printf("Y: minimum: %d, maximum: %d\n", absinfo.minimum, absinfo.maximum);
	}
	close(fd);

	return 0;
}

int input_open(struct pollfd* array, int i, char* filename)
{
	int fd;

        if ((fd = open(filename, O_RDONLY)) < 0)
        {
                return -1;
        }

	array[i].fd = fd;
	array[i].events = POLLRDNORM;

	return 0;
}

int input_close(struct pollfd* pfd)
{
	if (pfd)
	{
		close(pfd->fd);
		pfd->fd = 0;

		return 0;
	}

	return -1;
}

int input_search(struct pollfd* fds, int max, char *path)
{
        DIR *dir;
	int current = 0;
        char buffer[256];
        struct dirent *ent;
        if ((dir = opendir (path)) != NULL)
        {
                while ((ent = readdir (dir)) != NULL)
                {
                        if (ent->d_type == DT_CHR)
                        {
				snprintf(buffer, sizeof(buffer)-1, "%s/%s", path, ent->d_name);
				if (input_open(fds, current, buffer) == 0)
				{
					++current;
				}
                        }
                }
                closedir (dir);
        }
	return current;
}


int main(int argc, char* argv[])
{
	search_input("/dev/input");
	return 0;
	
	struct pollfd fds[20];
	int input_index = 0;
	
	if (read_content(&cells, "cells.raw"))

	input_index = input_search(fds, 20, "/dev/input");

	int ret, i, readed;
	struct input_event input_data;
	while(1)
	{
		ret = poll(fds, input_index, 1000);
		for(i=0;i<input_index;i++)
		{
			if (fds[i].revents & POLLRDNORM)
			{
				readed = read(fds[i].fd, &input_data, sizeof(struct input_event));
				if (readed > 0)
				{
					printf("Type: %d, code: %d, value: %d\n", input_data.type, input_data.code, input_data.value);
				}
			}
		}
	}

	input_close(&fds[0]);
	input_close(&fds[1]);

	return 0;
}
