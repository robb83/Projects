#include "space_age.h"

float age(planet_t planet, int64_t seconds)
{
    static float periods[] = { 
        0.2408467, 0.61519726, 1.0, 1.8808158, 
        11.862615, 29.447498, 84.016846, 164.79132 
    };

    if (planet < 0 || planet > 7) {
        return -1;
    }

    return seconds / (31557600 * periods[planet]);
}