#include "darts.h"
#include <math.h>
#include <stdint.h>

uint8_t score(coordinate_t coord)
{
    float r = sqrt((coord.x * coord.x) + (coord.y * coord.y));

    if (r > 10.0F) 
    {
        return 0;
    } 
    else if ( r > 5.0F) 
    {
        return 1;
    } 
    else if ( r > 1 ) 
    {
        return 5;
    } 
    else 
    {
        return 10;
    }
}