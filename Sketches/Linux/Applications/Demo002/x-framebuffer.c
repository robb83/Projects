#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/types.h>

typedef struct {
	struct timeval time;
	__u16 type;
	__u16 code;
	__s32 value;
} ievent;

typedef struct {
	int x;
	int y;
	int w;
	int h;
	int weight;
} point;

// cat /proc/bus/input/devices
#if ANDROID
#define FB_DEVICE "/dev/graphics/fb0"
#else
#define FB_DEVICE "/dev/fb0"
#endif
#define TOUCH_DEVICE "/dev/input/event2"

static char *fbp = NULL;
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static long int fbfd = 0, touchd = 0L;
#define RECTANGLES 5
point rectangle_positions[] =
{
	{200,200, 100, 100, 0},
	{200,200, 100, 100, 0},
	{200,200, 100, 100, 0},
	{200,200, 100, 100, 0},
	{200,200, 100, 100, 0}
};

void clear()
{
	int row;
	int width = vinfo.xres;
	int height = vinfo.yres;

	for(row=0; row<height; row++)
	{
		long int location = vinfo.xoffset * (vinfo.bits_per_pixel / 8) + (row + vinfo.yoffset) * finfo.line_length;
		memset(fbp + location, 0, finfo.line_length);
	};
}

void plot(int x, int y)
{
	long int location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;

			*(fbp + location) = 0;100; //100;        // Some blue
			*(fbp + location + 1) = 255;//15+(x-100)/2;     // A little green
			*(fbp + location + 2) = 0;//200-(y-100)/5;    // A lot of red
			*(fbp + location + 3) = 0;      // No transparency
}

void rectangle(int x1, int y1, int x2, int y2)
{
	int t,x,y, color;
	if (x2 < x1)
	{
		t = x2;
		x2 = x1;
		x1 = t;
	}

	if (y2 < y1)
	{
		t = y2;
		y2 = y1;
		y1 = t;
	}

	int* location = (int*)(fbp + ((x1 + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y1 + vinfo.yoffset) * finfo.line_length));
	int step = finfo.line_length / sizeof(int) - (x2 - x1);
	color = 0;
	color |= (0xff << 8);

    for (y = y1; y < y2; y++)
    {
        for (x = x1; x < x2; x++)
	{
		*location = color;
		location++;
        }
	location += step;
    }
}

void update()
{
    vinfo.activate |= FB_ACTIVATE_NOW | FB_ACTIVATE_FORCE;
    if(0 > ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo))
    {
        printf("Failed to refresh\n");
        exit(-1);
    }
}

int main()
{
    long int screensize = 0;

    // Open the file for reading and writing
    fbfd = open(FB_DEVICE, O_RDWR);  //use "/dev/fb0" in linux
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	if (vinfo.bits_per_pixel != 32) {
		printf("Not supported!");
		exit(3);
	}

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ( fbp == NULL) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    touchd = open(TOUCH_DEVICE, O_RDWR);
    if (touchd == -1) return -1;

    char buffer[16];
    ievent *ie;
	point *pp;
	int x = 0, y = 0, id = 0, e = 0, weight = 0;
    while(1)
    {
       int readed = read(touchd, buffer, 16);
       if(readed == 16)
       {
		ie = (ievent*)buffer;

		if (ie->type == 0)
		{
			e = id % RECTANGLES;
			rectangle_positions[e].x = x;
			rectangle_positions[e].y = y;
			rectangle_positions[e].weight = weight;

			if (rectangle_positions[e].x < 150
				&& rectangle_positions[e].y < 150)
			{
				goto finished;
			}

			clear();

			int wh = 50;
			int hh = 50;

			for(e=0;e < RECTANGLES;e++)
			{
				pp = &rectangle_positions[e];

				if (pp->weight > 10)
				{
					rectangle(
						pp->x - wh,
						pp->y - hh,
						pp->x + wh,
						pp->y + hh);
				}
			}
		}
		else
		{
			if (ie->code == 0x35)
			{
				x = ie->value;
				if (x < 50) x = 50;
				if (x > vinfo.xres - 50) x = vinfo.xres - 50;
			} else if (ie->code == 0x36)
			{
				y = ie->value;
                                if (y < 50) y = 50;
                                if (y > vinfo.yres - 50) y = vinfo.yres - 50;
			} else if (ie->code == 0x39)
			{
				id = ie->value;
			} else if (ie->code == 0x30)
			{
				weight = ie->value;
			}
		}
       }
    }

finished:
    munmap(fbp, screensize);
    close(fbfd);

    return 0;
}
