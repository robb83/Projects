#include "acronym.h"
#include <stdlib.h>
#include <ctype.h>

#define MAX_ACRONYM 20

char *abbreviate(const char *phrase)
{
    if (phrase && *phrase != 0)
    {
        char c;
        int cursor = 0;
        char* result = (char*)malloc(sizeof(char) * MAX_ACRONYM);

        while ( (c = *phrase) != 0 && cursor + 1 < MAX_ACRONYM)
        {
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') )
            {
                result[cursor++] = toupper(c);
                while ( (c = *phrase) != 0 && (!isspace(c) && c != '-' && c != '_')) ++phrase;
            }
            else{
                ++phrase;
            }
        }

        result[cursor] = 0;
        return result;
    }

    return NULL;
}