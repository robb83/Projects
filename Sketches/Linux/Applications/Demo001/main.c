// gcc -Wall -O2 -o framebuffer file.c gfx.c input.c main.c -lfreetype -lz -lm -I/usr/include/freetype2
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ft2build.h>
#include <math.h>
#include FT_FREETYPE_H
#include "input.h"
#include "gfx.h"
#include "file.h"

static display disp;
int green = 0xff00ff00; // argb, green

int colors[8] = {
	0xff00ff00,
	0xff0000ff,
	0xffff0000,
	0xff00ffff,
	0xffffff00,
	0xffffffff,
	0xff0ffff0,
	0xfff0ff0f
};

void safe_gfx_draw(rect* rectangle, int color)
{
	if (rectangle->x + rectangle->w > disp.width)
	{
		rectangle->x = disp.width - rectangle->w;
	}

	if (rectangle->y + rectangle->h > disp.height)
	{
		rectangle->y = disp.height - rectangle->h;
	}
	
	if (rectangle->y < 0)
	{
		rectangle->y = 0;
	}
	
	if (rectangle->x < 0)
	{
		rectangle->x = 0;
	}
	
	
	gfx_draw(rectangle, colors[0]);
}

void input_event(event *event)
{
//	static rect one = {10,10,10,10};
	static rect two = {100,100,50,50};

// 	gfx_draw(&one, colors[event->type]);
// 					
// 	one.x += 15;
//
// 	if (one.x + one.w > disp.width)
// 	{
// 		one.x = 0;
// 		one.y += 15;
// 	}
//
// 	if (one.y + one.h > disp.height)
// 	{
// 		one.x = 0;
// 		one.y = 0;
// 	
// 		gfx_clear();
// 	}
	
	if (event->type == EVENT_KEY)
	{
		if (event->key_value == 1 || event->key_value == 2)
		{
			if (event->key == 103 || event->key == 17)
			{
				two.y--;
			} 
			else if (event->key == 108 || event->key == 31)
			{
				two.y++;
			} 
			else if (event->key == 105 || event->key == 30)
			{
				two.x--;
			} 
			else if (event->key == 106 || event->key == 32)
			{
				two.x++;
			} 
			else 
			{
				return;
			}
			
			gfx_clear();
			safe_gfx_draw(&two, colors[0]);
		}		
	} 
	else if (event->type == EVENT_REL)
	{	
		two.x += event->x;
		two.y += event->y;
	
		gfx_clear();
		safe_gfx_draw(&two, colors[0]);
		
		printf("x\n");
	}	 
	else if (event->type == EVENT_ABS)
	{	
		if (event->abs_x_max > 0)
		{
			float a = (float)(event->abs_x - event->abs_x_min) / (float)(event->abs_x_max - event->abs_x_min);
			float b = (float)(event->abs_y - event->abs_y_min) / (float)(event->abs_y_max - event->abs_y_min);
		
			two.x = (int)(disp.width * a);
			two.y = (int)(disp.height * b);
		} 
		else {
			two.x = event->abs_x;
			two.y = event->abs_y;
		}
		
		gfx_clear();
		safe_gfx_draw(&two, colors[0]);
		
		printf("y\n");
	}
}

int main(int argc, char* argv[])
{
	int n;
  FT_Library    library;
  FT_Face       face;

  FT_GlyphSlot  slot;
  FT_Matrix     matrix;                 /* transformation matrix */
  FT_Vector     pen;                    /* untransformed origin  */
  FT_Error      error;
double angle         = ( 25.0 / 360 ) * 3.14159 * 2;
  error = FT_Init_FreeType( &library );              /* initialize library */
  /* error handling omitted */

  // http://openfontlibrary.org/en/font/geo-1
  error = FT_New_Face( library, "geo_1.ttf", 0, &face );/* create face object */
  /* error handling omitted */

  /* use 50pt at 100dpi */
  error = FT_Set_Char_Size( face, 50 * 64, 0, 100, 0 );                /* set character size */
  /* error handling omitted */

  slot = face->glyph;
  /* set up matrix */
  matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */
  pen.x = 10;
  pen.y = 400;
  
	fontdata raw = {"cells.raw", 16, 16, 16, 0, 0, 256 };
	Buffer buffer;
	//ioctl(fd, KDSETMODE, KD_GRAPHICS);

	read_content(&buffer, raw.name);
	
	if (input_init(input_event) > 0)
	{
		if (gfx_init())
		{
			if (gfx_text_init(&raw))
			{
				gfx_display(&disp);
				gfx_clear();
				
				Surface surface = { 256, 256, (int*)buffer.content };
				printf("Result: %d\n", gfx_blit(10, 10, &surface));
			
				char* text          = "Hello World!";
				int num_chars     = strlen( text );
			  for ( n = 0; n < num_chars; n++ )
			  {
				/* set transformation */
				FT_Set_Transform( face, &matrix, &pen );

				/* load glyph image into the slot (erase previous one) */
				error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
				if ( error )
				{
				  continue;                 /* ignore errors */
				  }

				  Surface sfc = { slot->bitmap.width, slot->bitmap.rows, (int*)slot->bitmap.buffer };
				  printf("sfc: %d, %d\n", sfc.width, sfc.height);
				  gfx_blit(slot->bitmap_left, 800 - slot->bitmap_top, &sfc);
				/* now, draw to our target surface (convert position) */
				draw_bitmap( &slot->bitmap,
							 slot->bitmap_left,
							 target_height - slot->bitmap_top );

				/* increment pen position */
				pen.x += slot->advance.x;
				pen.y += slot->advance.y;
			  }
  
				while(1)
				{
					input_poll();
				}
			}
			gfx_quit();
		}
		
		input_quit();
	}
	
	FT_Done_Face    ( face );
	FT_Done_FreeType( library );
  
	return 0;
}
