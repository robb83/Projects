#ifndef RESISTOR_COLOR_TRIO_H
#define RESISTOR_COLOR_TRIO_H

#include <stdint.h>

#define KILOOHMS 1000
#define OHMS 0

typedef struct {
    uint16_t value;
    uint16_t unit;
} resistor_value_t;

typedef enum resistor_band {
    BLACK = 0,
    BROWN,
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    VIOLET,
    GREY,
    WHITE
} resistor_band_t;

resistor_value_t color_code(resistor_band_t[]);

#endif
