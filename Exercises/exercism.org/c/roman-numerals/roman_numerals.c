#include "roman_numerals.h"
#include <string.h>
#include <stdlib.h>

int append_string(char* buffer, int pos, char* parts) {
    int e = pos + strlen(parts);
    int i = 0;
    while ( pos < e) {
        buffer[pos++] = parts[i++];
    }

    return pos;
}

char *to_roman_numeral(unsigned int number) {
    if (number >= 4000) {
        return NULL;
    }

    char* result = (char*)malloc(sizeof(char) * 16);
    int pos = 0;

    int c = number / 1000;
    switch(c) {
        case 1: pos = append_string(result, pos, "M"); break;
        case 2: pos = append_string(result, pos, "MM"); break;
        case 3: pos = append_string(result, pos, "MMM"); break;
    }

    c = (number % 1000) / 100;
    switch(c) {
        case 1: pos = append_string(result, pos, "C"); break;
        case 2: pos = append_string(result, pos, "CC"); break;
        case 3: pos = append_string(result, pos, "CCC"); break;
        case 4: pos = append_string(result, pos, "CD"); break;
        case 5: pos = append_string(result, pos, "D"); break;
        case 6: pos = append_string(result, pos, "DC"); break;
        case 7: pos = append_string(result, pos, "DCC"); break;
        case 8: pos = append_string(result, pos, "DCCC"); break;
        case 9: pos = append_string(result, pos, "CM"); break;
    }

    c = (number % 100) / 10;
    switch(c) {
        case 1: pos = append_string(result, pos, "X"); break;
        case 2: pos = append_string(result, pos, "XX"); break;
        case 3: pos = append_string(result, pos, "XXX"); break;
        case 4: pos = append_string(result, pos, "XL"); break;
        case 5: pos = append_string(result, pos, "L"); break;
        case 6: pos = append_string(result, pos, "LX"); break;
        case 7: pos = append_string(result, pos, "LXX"); break;
        case 8: pos = append_string(result, pos, "LXXX"); break;
        case 9: pos = append_string(result, pos, "XC"); break;
    }

    c = number % 10;
    switch(c) {
        case 1: pos = append_string(result, pos, "I"); break;
        case 2: pos = append_string(result, pos, "II"); break;
        case 3: pos = append_string(result, pos, "III"); break;
        case 4: pos = append_string(result, pos, "IV"); break;
        case 5: pos = append_string(result, pos, "V"); break;
        case 6: pos = append_string(result, pos, "VI"); break;
        case 7: pos = append_string(result, pos, "VII"); break;
        case 8: pos = append_string(result, pos, "VIII"); break;
        case 9: pos = append_string(result, pos, "IX"); break;
    }

    result[pos] = '\0';
    return result;
}