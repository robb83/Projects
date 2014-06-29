// gcc -Wall -O2 -o x-hellosdl x-hellosdl.c -lSDL
#include <SDL/SDL.h>
#include <stdio.h>

#if ANDROID_NOVO9
#define SCREEN_WIDTH 2048
#define SCREEN_HEIGHT 1536
#else
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#endif
#define PIXEL_FORMAT 32

int main( int argc, char* args[] )
{
    //The images
    SDL_Surface* screen = NULL;

    //Start SDL
    if (SDL_Init( SDL_INIT_TIMER ) == -1)
    {
       printf("SDL error: %s!\n", SDL_GetError());
       return 1;
    }

    printf("SDL_Init = OK");

    //screen = SDL_GetVideoSurface();

    //Set up screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, PIXEL_FORMAT, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);

    if (!screen)
    {
        printf("Video mode error: %s!\n", SDL_GetError());
        return 1;
    }

    printf("SDL video mode setted\n");

    //Fill the screen white
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

    printf("SDL filled\n");

    //Update Screen
    SDL_Flip( screen );

    printf("SDL Flip\n");

    //Pause
    SDL_Delay( 2000 );

    //Quit SDL
    SDL_Quit();

    return 0;
}
