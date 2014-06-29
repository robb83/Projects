#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include "gfx.h"
#include "file.h"

#ifdef ANDROID
#define FB_DEVICE "/dev/graphics/fb0"
#else
#define FB_DEVICE "/dev/fb0"
#endif

static char *fbp = NULL;
static long int fbfd = 0;
static int screensize = 0;
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;

// text
static fontdata mainfont;
static Buffer mainfont_data;

int gfx_init(void)
{
	if (fbp != NULL) 
	{
		return 1;
	}
	
	fbfd = open(FB_DEVICE, O_RDWR);
	if (fbfd == -1) 
	{
		perror("Error: cannot open framebuffer device");
		return 0;
	}
        
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) 
	{
	    perror("Error reading fixed information");
	    gfx_quit();
	    return 0;
	}

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) 
	{
		perror("Error reading variable information");
		gfx_quit();
		return 0;
	}
		
	if (vinfo.bits_per_pixel != 32) 
	{
		perror("Not supported bits per pixel!");
		gfx_quit();
		return 0;
	}
    
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ( fbp == NULL) 
	{
		perror("Error: failed to map framebuffer device to memory");
		gfx_quit();
		return 0;
	}
	
	printf("gfx: %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
	
	return 1;
}

int gfx_clear(void)
{
	if (fbp == NULL) return 0;

	int row;
	int height = vinfo.yres;

	for(row=0; row<height; row++)
	{
		long int location = vinfo.xoffset * (vinfo.bits_per_pixel / 8) + (row + vinfo.yoffset) * finfo.line_length;
		memset(fbp + location, 0x00, finfo.line_length);
	};
	
	return 1;
}

int gfx_draw(rect* rectangle, int color)
{
	if (fbp == NULL) return 0;

	int x,y,
		x1 = rectangle->x,
		y1 = rectangle->y,
		x2 = rectangle->x + rectangle->w,
		y2 = rectangle->y + rectangle->h;

	int* location = (int*)(fbp + ((x1 + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y1 + vinfo.yoffset) * finfo.line_length));
	
	int step = finfo.line_length / sizeof(int) - (x2 - x1);
	
	for (y = y1; y < y2; y++)
	{
		for (x = x1; x < x2; x++)
		{
			*location = color;
			location++;
		}
		location += step;
	}
	
	return 1;
}

int gfx_quit()
{
	if (fbp != NULL) 
	{
		munmap(fbp, screensize);
	}
	
	if (fbfd > 0)
	{
		close(fbfd);
	}
	
	return 0;
}

void gfx_display(display *disp)
{
	if (fbp == NULL)
	{
		disp->width = 0;
		disp->height = 0;
		disp->bitsPerPixel = 0;
	} else
	{
		disp->width = vinfo.xres;
		disp->height = vinfo.yres;
		disp->bitsPerPixel = vinfo.bits_per_pixel;	
	}
}

int gfx_text_draw(int x1, int y1, char* text)
{
	int i=0, a, b;
	char ch;
		
	int step = finfo.line_length / sizeof(int) - mainfont.cell_width;
	int fontstep = mainfont.image_width - mainfont.cell_width;
	
	while((ch = text[i++])>0)
	{
		int* location = (int*)(fbp + ((x1 + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y1 + vinfo.yoffset) * finfo.line_length));
		int* font = ((int*)mainfont_data.content) + ((ch % mainfont.columns) * mainfont.cell_width ) + (((ch / mainfont.columns) * mainfont.cell_height) * mainfont.image_width);
		
		for(a = 0; a < mainfont.cell_height; a++)
		{
			for (b = 0; b < mainfont.cell_width; b++)
			{
				*location = *font;
				location++;
				font++;
			}
			location += step;
			font += fontstep;
		}
		
		x1 += mainfont.cell_width + 1;
	}
	
	return 0;
}

int gfx_text_init(fontdata *f)
{
	read_content(&mainfont_data, f->name);
	mainfont = *f;
	
	return 1;
}

int gfx_blit(int x, int y, Surface* surface)
{
	int r,c;
	
	if (surface == NULL || surface->pixels == NULL) return 0;

	int x1 = (x < 0 ? 0 : x);
	if (x1 >= vinfo.xres)
		x1 = vinfo.xres - 1;
	
	int y1 = (y < 0 ? 0 : y);
	if (y1 >= vinfo.yres)
		y1 = vinfo.yres - 1;
		
	int w1 = x1 + surface->width;
	if (w1 >= vinfo.xres)
	{
		w1 = vinfo.xres - x;
	} else 
	{
		w1 = w1 - x;
	}
	
	int h1 = y1 + surface->height;
	if (h1 >= vinfo.yres)
	{
		h1 = vinfo.yres - y1;
	} else 
	{
		h1 = h1 - y1;
	}
	
	int* surface_location = surface->pixels;
	int surface_step = surface->width - w1;
	int* display_location = (int*)(fbp + ((x1 + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y1 + vinfo.yoffset) * finfo.line_length));
	int display_step = (finfo.line_length / sizeof(int)) - w1;
	
	for(r = 0; r < h1; r++)
	{
		for(c = 0; c < w1; c++)
		{
			*display_location = *surface_location;
			
			++surface_location;
			++display_location;
		}
		
		surface_location += surface_step;
		display_location += display_step;
	}
	
	return 1;
}