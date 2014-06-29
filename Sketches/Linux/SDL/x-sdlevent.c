// gcc -Wall -O2 -o x-sdlevent x-sdlevent.c -lSDL -lSDL_ttf
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

#if ANDROID_NOVO9
#define SCREEN_WIDTH 2048
#define SCREEN_HEIGHT 1536
#else
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#endif
#define PIXEL_FORMAT 32
#define BOX_OFFSET 15
#define MSG_BOX_WIDTH 640
#define MSG_BOX_HEIGHT 480

static SDL_Surface* screen = NULL;
static SDL_Surface* textbox1 = NULL;
static SDL_Surface* boxes = NULL;
static TTF_Font *font = NULL;
static SDL_Color fontColor = {255, 255, 255};
static int fontSize = 25;
static char sprintBuffer[80];
static char *eventTypes[] = 
{
       "SDL_NOEVENT",
       "SDL_ACTIVEEVENT",
       "SDL_KEYDOWN",
       "SDL_KEYUP",
       "SDL_MOUSEMOTION",
       "SDL_MOUSEBUTTONDOWN",
       "SDL_MOUSEBUTTONUP",
       "SDL_JOYAXISMOTION",
       "SDL_JOYBALLMOTION",
       "SDL_JOYHATMOTION",
       "SDL_JOYBUTTONDOWN",
       "SDL_JOYBUTTONUP",
       "SDL_QUIT",
       "SDL_SYSWMEVENT",
       "SDL_EVENT_RESERVEDA",
       "SDL_EVENT_RESERVEDB",
       "SDL_VIDEORESIZE",
       "SDL_VIDEOEXPOSE",
       "SDL_EVENT_RESERVED2",
       "SDL_EVENT_RESERVED3",
       "SDL_EVENT_RESERVED4",
       "SDL_EVENT_RESERVED5",
       "SDL_EVENT_RESERVED6",
       "SDL_EVENT_RESERVED7",
       "SDL_USEREVENT",
       "event: 25",
       "event: 26",
       "event: 27",
       "event: 28",
       "event: 29",
       "event: 30",
       "event: 31",
       "event: 32"       
};

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
	//Holds offsets
	SDL_Rect offset;

	//Get offsets
	offset.x = x;
	offset.y = y;

	//Blit
	SDL_BlitSurface( source, clip, destination, &offset );
}

void draw_screen()
{
	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );
	apply_surface(0, 0, boxes, screen, NULL);
	apply_surface(screen->w - textbox1->w, screen->h - textbox1->h, textbox1, screen, NULL);
	SDL_Flip( screen );
}

void move_surface_up(SDL_Surface* surface, int row)
{
	if( SDL_MUSTLOCK( surface ) )
	{
		SDL_LockSurface( surface );
	}
    
	int height = surface->h - row, h;
	Uint32 *top = ((Uint32 *)surface->pixels);
	Uint32 *first = ((Uint32 *)surface->pixels) + (surface->w * row);
	
	for(h=0;h<height;h++)
	{
		memmove(top, first, surface->w * 4);
		first += surface->w;
		top += surface->w;
	}
	
	height = surface->h;
	for(;h<height;h++)
	{
		memset(top, 0x00, surface->w * 4);
		top += surface->w;
	}
	
	if( SDL_MUSTLOCK( surface ) )
	{
		SDL_UnlockSurface( surface );
	}
}

void draw_text(char* message)
{
	static SDL_Surface* label = NULL;
	label = TTF_RenderText_Solid(font, message, fontColor);
	
	move_surface_up(textbox1, fontSize);
	
	apply_surface(0, MSG_BOX_HEIGHT - label->h, label, textbox1, NULL);
	
	if (label)
	{
		SDL_FreeSurface( label );
	}
	
	draw_screen();
}

void draw_event(int type)
{
	static SDL_Rect box = { 0, 0, 10, 10 };
	static Uint32 boxColor = 0xFF00FF00;
	static Uint32 boxErrorColor = 0xFFFF0000;
	
	SDL_FillRect( boxes, &box, (type < 0 ? boxErrorColor : boxColor) );
	
	box.x += BOX_OFFSET;

	if (box.x + box.w > boxes->clip_rect.w)
	{
		box.y += BOX_OFFSET;
		box.x = 0;
	}

	if (box.y + box.h > boxes->clip_rect.h)
	{
		SDL_FillRect( boxes, &boxes->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );

		box.y = 0;
		box.x = 0;
	}
	
	draw_screen();
}

void print_directory(char *path)
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (path)) != NULL) 
	{
		snprintf(sprintBuffer, 79, "Open: %s", path);
		draw_text(sprintBuffer);
		
		while ((ent = readdir (dir)) != NULL) {
			draw_text(ent->d_name);
			printf ("%s\n", ent->d_name);
		}
		closedir (dir);
	} else {
		snprintf(sprintBuffer, 79, "Failed to open: %s", path);
		draw_text(sprintBuffer);
	}
}

int main(int argc, char *argv[]) { //	sleep(10);
	int quit = 0;	
	SDL_Event event;
	SDL_Joystick *joystick = NULL;

	// sdl initialization
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == -1)
	{
		printf("SDL Initialization failed: %s!\n", SDL_GetError());
		return 1;
	}

	printf("SDL initialized\n");

	if (TTF_Init() == -1)
        {
                printf("TTF Initialization failed: %s!\n", SDL_GetError());
                return 1;
        }

	// http://openfontlibrary.org/en/font/geo-1
	font = TTF_OpenFont("geo_1.ttf", 16);
	if (!font)
	{
		printf("OpenFont failed: %s\n", SDL_GetError());
		return 1;
	}
	
	fontSize = TTF_FontHeight(font);

	printf("TTF Initialized and opened!\n");

	// video initialization
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, PIXEL_FORMAT, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
	if (!screen)
	{
		printf("SDL_SetVideoMode failed: %s!\n", SDL_GetError());
		return 1;
	}
	
	textbox1 = SDL_CreateRGBSurface( SDL_SWSURFACE, MSG_BOX_WIDTH, MSG_BOX_HEIGHT, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask );
	boxes = SDL_CreateRGBSurface( SDL_SWSURFACE, screen->w, screen->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask );
	
	printf("SDL Video initialized\n");

	// clean screen
	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );
	draw_event(0);
	printf("SDL Screen cleard!\n");

	// joystick initialization
	int joys = SDL_NumJoysticks();
	if (joys <= 0)
	{
		printf("No joystick were found!\n");
		draw_text("No joystick were found!\n");
		draw_event(-1);
	} else {
		snprintf(sprintBuffer, 79, "Joysticks: %d", joys);
		draw_text(sprintBuffer);
		
		joystick = SDL_JoystickOpen(joys-1);
	}
	
	print_directory("/dev/input");

	// loop
	quit = 1;
	while(quit)
	{
		while(SDL_PollEvent(&event))
		{
			printf("event.type = %d\n", event.type);
			draw_event(event.type);
			
			if (event.type == SDL_QUIT)
			{
				quit = 0;
				break;
			} else
			{
				if (event.type >= 0 && event.type <= 32)
				{
					draw_text(eventTypes[event.type]);
				} else 
				{
					snprintf(sprintBuffer, 79, "Unknow: %d", event.type);
					draw_text(sprintBuffer);
				}				
			}
		}
	}

	// release joystick
	if (joystick)
	{
		SDL_JoystickClose(joystick);
	}

	// release TTF
	if (font)
	{
		TTF_CloseFont( font );
	}
	TTF_Quit();

	// release SDL
	if (textbox1)
	{
		SDL_FreeSurface( textbox1 );
	}
	
	if (boxes)
	{
		SDL_FreeSurface( boxes );
	}
	
	SDL_Quit();

	return 0;
}
