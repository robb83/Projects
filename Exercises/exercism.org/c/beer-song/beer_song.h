#ifndef BEER_SONG_H
#define BEER_SONG_H

#include <stdint.h>
#include <stddef.h>

void recite(uint8_t start_bottles, uint8_t take_down, char **song);
char* create(char* source, size_t size);

#endif
