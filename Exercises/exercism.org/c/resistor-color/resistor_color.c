#include "resistor_color.h"

resistor_band_t array_of_colors[] = { BLACK, BROWN, RED,    ORANGE, YELLOW, GREEN, BLUE,  VIOLET, GREY,   WHITE };

 const resistor_band_t * colors()
 {
    return &array_of_colors[0];
 }

 int color_code(resistor_band_t color)
 {
    return (int)color;
 }