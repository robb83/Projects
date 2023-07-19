#include "binary.h"
#include <math.h>
#include <string.h>

int convert(const char *input)
{
    int result = 0;
    char current;

    while( (current = *input) != 0 )
    {
        if (current != '0' && current != '1')
        {
            return INVALID;   
        }

        result = (result << 1) + (*input - '0');
        ++input;
    }

    return result;
}