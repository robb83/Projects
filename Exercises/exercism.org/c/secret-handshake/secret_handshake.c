#include <stdlib.h>
#include <string.h>
#include "secret_handshake.h"

#define MAXCOMMAND 4

const char **commands(size_t number)
{
    char *commands[] = { "wink", "double blink", "close your eyes", "jump" };

    int cursor = 0;
    const char** result = (const char**)malloc(sizeof(const char*) * MAXCOMMAND);
    memset(result, 0, sizeof(char*) * MAXCOMMAND);

    if (number & 0x10) {
        for (int i = 3; i >= 0; --i)
        {
            if (number & 0x08) {
                result[cursor++] = commands[i];
            }

            number <<= 1;
        }
    } else {
        for (int i = 0; i < 4; ++i) 
        {
            if (number & 0x01) {
                result[cursor++] = commands[i];
            }

            number >>= 1;
        }
    }

    return result;
}