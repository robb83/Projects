#include "resistor_color_trio.h"
#include <math.h>
#include <stdio.h>

resistor_value_t color_code(resistor_band_t bands[])
{
    uint32_t value = (bands[0] * 10 + bands[1]) * pow(10, bands[2]);

    resistor_value_t result;
    if (value >= KILOOHMS) {
        result.value = value / KILOOHMS;
        result.unit = KILOOHMS;
    } else {
        result.value = value;
        result.unit = OHMS;
    }

    return result;
}