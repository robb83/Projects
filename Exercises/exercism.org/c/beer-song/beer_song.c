#pragma warning(disable:4996)

#include "beer_song.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* create(char* source, size_t size)
{
    char* buffer = (char*)malloc((size + 1 ) * sizeof(char));
    strcpy(buffer, source);
    return buffer;
}

void recite(uint8_t start_bottles, uint8_t take_down, char **song)
{
    static char default_single[] = "Take it down and pass it around, no more bottles of beer on the wall.";
    static char default_no_more[] = "No more bottles of beer on the wall, no more bottles of beer.";
    static char default_gotostore[] = "Go to the store and buy some more, 99 bottles of beer on the wall.";

    for (int i = start_bottles, position = 0; i > start_bottles - take_down; --i)
    {
        if (position > 0) 
        {
            song[position++] = create("", 0);
        }

        if (i == 0) 
        {
            song[position++] = create(default_no_more, sizeof(default_no_more));
            song[position++] = create(default_gotostore, sizeof(default_gotostore));
        }
        else 
        {
            char* buffer1 = (char*)malloc(68 * sizeof(char));
            sprintf(buffer1, "%d bottle%s of beer on the wall, %d bottle%s of beer.", i, ( i > 1 ? "s" : ""), i, ( i > 1 ? "s" : ""));
            song[position++] = buffer1;

            if (i == 1) 
            {
                song[position++] = create(default_single, sizeof(default_single));
            }
            else
            {
                char* buffer2 = (char*)malloc(74 * sizeof(char));
                sprintf(buffer2, "Take one down and pass it around, %d bottle%s of beer on the wall.", i - 1, ( i - 1 > 1 ? "s" : ""));
                song[position++] = buffer2;
            }
        }
    }
}