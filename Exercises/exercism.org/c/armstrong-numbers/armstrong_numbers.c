#include "armstrong_numbers.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

bool is_armstrong_number(int candidate)
{
    int digits[10];
    int digit_length = 0;
    int sum = 0;
    int temp = candidate;
    
    for (int i = 10; i && temp; i--)
    {
        digits[digit_length++] = temp % 10;
        temp = temp / 10;
    }
    
    for (int i = 0; i < digit_length; ++i)
    {
        sum += pow(digits[i], digit_length);
    }

    return sum == candidate;
}