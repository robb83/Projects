#include "isogram.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool is_isogram(const char phrase[])
{
    if (phrase)
    {
        int len = strlen(phrase);
        
        for (int i = 0; i < len; ++i)
        {
            if (phrase[i] == ' ' || phrase[i] == '-')
            {
                continue;
            }
        
            for (int j = i + 1; j < len; ++j)
            {
                if (tolower(phrase[j]) == tolower(phrase[i]))
                {
                    return false;
                }
            }
        }
    
        return true;
    }
    return false;
}