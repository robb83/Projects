#include "difference_of_squares.h"
#include <math.h>

unsigned int sum_of_squares(unsigned int number)
{
    int acc = 0;
    for (unsigned int i = 1; i <= number; ++i)
    {
        acc += pow(i, 2);
    }
    return acc;
}

unsigned int square_of_sum(unsigned int number)
{
    int acc = 0;
    for (unsigned int i = 1; i <= number; ++i)
    {
        acc += i;
    }
    return pow(acc, 2);
}

unsigned int difference_of_squares(unsigned int number)
{
    return square_of_sum(number) - sum_of_squares(number);
}