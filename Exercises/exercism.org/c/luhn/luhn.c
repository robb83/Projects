#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "luhn.h"

bool luhn(const char *num)
{
    if (!num) return false;

    char ch; int sum = 0, digit, digits = 0; bool flip = false;
    for (size_t p = strlen(num); p > 0; --p) {
        ch = num[p - 1];
        if (ch == ' ') continue;
        if (!isdigit(ch)) return false;
        digit = ch - '0';
        if (flip) {
            digit += digit;
            if (digit > 9) digit -= 9;
        }
        sum += digit;
        flip = !flip;
        ++digits;
    }

    return (digits > 1) && (sum % 10 == 0);
}