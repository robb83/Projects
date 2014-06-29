#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <poll.h>
#include <linux/input.h>
#include <string.h>
#include <unistd.h>
#include "input.h"

#define DEVICE_INPUT_DIR "/dev/input"
#define DEVICE_INPUT_MAX 20
#define DEVICE_INPUT_BUFFER 10

static int fds_index = 0;
static struct pollfd fds[DEVICE_INPUT_MAX];
static struct input_event input_event_data;
static event events[DEVICE_INPUT_MAX];
static void (*input_handler)(event*) = NULL;

int __input_open(struct pollfd* pfd, event* event, char* filename)
{
	int fd, i;
	struct input_absinfo absinfo;
	char name[256] = "Unknown";
	char evtype_b[256];
	
    if ((fd = open(filename, O_RDONLY)) < 0)
    {
            return -1;
    }
	
	if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0)
	{
		close(fd);
		return -1;
	}
	
	memset(evtype_b, 0, sizeof(evtype_b));
	if (ioctl(fd, EVIOCGBIT(0, EV_MAX), evtype_b) < 0) 
	{
		close(fd);
		return -1;
	}
	
	for (i = 0; i < EV_MAX; i++) {
		if (test_bit(i, evtype_b)) 
		{
			if ( i == EV_ABS)
			{
				ioctl( fd, EVIOCGABS(ABS_X), &absinfo );
				event->abs_x_min = absinfo.minimum;
				event->abs_x_max = absinfo.maximum;
				
				ioctl( fd, EVIOCGABS(ABS_Y), &absinfo );
				
				event->abs_y_min = absinfo.minimum;
				event->abs_y_max = absinfo.maximum;
				
				//ioctl( fd, EVIOCGABS(ABS_Z), &absinfo );
			}
		}
	}
	
 	printf("Opened: %s, fd: %d, name: %s\n", filename, fd, name);

	pfd->fd = fd;
	pfd->events = POLLRDNORM;	

	return 0;
}

int __input_close(struct pollfd* pfd)
{
	if (pfd)
	{
		if (pfd->fd)
		{
			close(pfd->fd);
			pfd->fd = 0;
		}
		pfd->events = 0;

		return 0;
	}

	return -1;
}

int __input_search(struct pollfd* fds, int max, char *path)
{
    DIR *dir;
	int current = 0;
    char buffer[256];
    struct dirent *ent;
	
	if (max <= 0) return current;
	
        if ((dir = opendir (path)) != NULL)
        {
                while ((ent = readdir (dir)) != NULL)
                {			
					if (ent->d_type == DT_CHR)
                    {
						snprintf(buffer, sizeof(buffer)-1, "%s/%s", path, ent->d_name);
				
						if (__input_open(&fds[current], &events[current], buffer) == 0)
						{
							++current;
					
							if (current >= max) 
							{
								break;
							}
						}
                    }
                }
                closedir (dir);
        }
		
	return current;
}

int input_poll(void)
{
	int ret, i, readed;
	
	if (fds_index == 0) return 0;
	
	ret = poll(fds, fds_index, 1000);
	
	for(i=0;i<fds_index;i++)
	{
		if (fds[i].revents & POLLRDNORM)
		{
			readed = read(fds[i].fd, &input_event_data, sizeof(struct input_event));
			
			if(readed > 0)
			{
#ifdef ANDROID
 				printf("Type: %d, code: %d, value: %d\n", input_event_data.type, input_event_data.code, input_event_data.value);
#endif
				if (input_event_data.type == EV_SYN)
				{
					// raise event
// 					if (events[i].type & EVENT_KEY > 0)
// 					{
// 						printf("KEY: %d\n", events[i].key);
// 					} 
// 					
// 					if (events[i].type & EVENT_ABS > 0)
// 					{
// 						printf("ABS: X: %d, Y: %d, Z: %d\n", events[i].abs_x, events[i].abs_y, events[i].abs_z);
// 					}
// 					
// 					if (events[i].type & EVENT_REL > 0)
// 					{
// 						printf("REL: X: %d, Y: %d, Z: %d\n", events[i].x, events[i].y, events[i].z);
// 					}
					
					if (input_handler != NULL)
					{
						input_handler(&events[i]);
					}
					
					events[i].type = 0;
				} 
				else if (input_event_data.type == EV_KEY)
				{
					// type == EV_KEY, Code = BTN/KEY, Value = 1/0
					//printf("Type: %d, code: %d, value: %d\n", input_event_data.type, input_event_data.code, input_event_data.value);
					events[i].type |= EVENT_KEY;
					events[i].key = input_event_data.code;
					events[i].key_value = input_event_data.value;
				} else if (input_event_data.type == EV_ABS)
				{
					if (input_event_data.code == ABS_X || input_event_data.code == ABS_MT_POSITION_X)
					{
						events[i].abs_x = input_event_data.value;
					} else if (input_event_data.code == ABS_Y || input_event_data.code == ABS_MT_POSITION_Y)
					{
						events[i].abs_y = input_event_data.value;
					} else if (input_event_data.code == ABS_Z || input_event_data.code == ABS_WHEEL)
					{
						events[i].abs_z = input_event_data.value;
					}
					
					events[i].type |= EVENT_ABS;
				} else if (input_event_data.type == EV_REL)
				{
					if (input_event_data.code == REL_X)
					{
						events[i].x = input_event_data.value;
					} else if (input_event_data.code == REL_Y)
					{
						events[i].y = input_event_data.value;
					} else if (input_event_data.code == REL_Z || input_event_data.code == REL_WHEEL)
					{
						events[i].z = input_event_data.value;
					}
										
					events[i].type |= EVENT_REL;
				}
			}
		}
	}

	return ret;
}

int input_init(void (*handler)(event*))
{
	input_handler = handler;
	fds_index = __input_search(fds, DEVICE_INPUT_MAX, DEVICE_INPUT_DIR);
	return fds_index;
}

int input_quit(void)
{
	int i;
	for(i=0;i<fds_index;i++)
	{
		__input_close(&fds[i]);
	}

	return 0;
}