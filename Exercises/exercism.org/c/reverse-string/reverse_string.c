#include "reverse_string.h"
#include <string.h>
#include <stdlib.h>

char *reverse(const char *value)
{
    if (value)
    {
        int length = strlen(value);
        char* result = (char*)malloc(sizeof(char) * (length + 1));
        result[length] = '\0';

        for (int i = 0; i < length; ++i)
        {
            result[i] = value[length - 1 - i];
        }

        return result;
    }

    return NULL;
}