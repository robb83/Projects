#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "phone_number.h"

char *phone_number_clean(const char *input) 
{
    const size_t phone_length = 10;
    char* temp = (char*)malloc((phone_length + 1) * sizeof(char));
    int digits = 10;

    if (input) {
        char t;
        for (int l = strlen(input); l > 0; --l) {
            t = input[l - 1];
            if (isdigit(t))
            {
                if ((digits == 0 && t != '1') || ((digits == 1 || digits == 4) && t < '2') ) {
                    digits = phone_length;
                    break;
                } 
                else if (digits > 0)
                {
                    temp[digits - 1] = t;
                    --digits;
                }
            }
        }
    }

    if (digits > 0)
    {
        memset(temp, '0', phone_length);
    } 
    
    temp[phone_length] = '\0';

    return temp;
}