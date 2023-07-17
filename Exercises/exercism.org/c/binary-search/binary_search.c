#include "binary_search.h"

const int *binary_search(int value, const int *arr, size_t length)
{
    if (length == 0)
    {
        return NULL;
    }

    int min = 0;
    int max = length;
    int current = (max - min) / 2;

    while ( max - min >= 0)
    {
        int test = value - arr[current];

        if (test > 0)
        {
            min = current + 1;
            current = min + ((max - min) / 2);
        }
        else if (test < 0)
        {
            max = current - 1;
            current = min + ((max - min) / 2);
        } 
        else
        {
            return &arr[current];
        }
    }

    return NULL;
}