#include "clock.h"
#include <string.h>
#include <stdio.h>

clock_t clock_create(int hour, int minute)
{
    int t = (((hour * 60 + minute) % 1440) + 1440) % 1440;
    hour = t / 60;
    minute = t % 60;

    clock_t result;
    sprintf((char*)&result.text, "%02d:%02d", hour, minute);
    return result;
}

clock_t clock_add(clock_t clock, int minute_add)
{
    int h, m, t;
    sscanf_s(clock.text, "%02d:%02d", &h, &m);

    t = ((((h * 60) + m + minute_add) % 1440) + 1440) % 1440;
    h = t / 60;
    m = t % 60;

    return clock_create(h, m);
}

clock_t clock_subtract(clock_t clock, int minute_subtract)
{
    int h, m, t;
    sscanf_s(clock.text, "%02d:%02d", &h, &m);

    t = ((((h * 60) + m - minute_subtract) % 1440) + 1440) % 1440;
    h = t / 60;
    m = t % 60;

    return clock_create(h, m);
}

bool clock_is_equal(clock_t a, clock_t b)
{
    return strcmp(a.text, b.text) == 0;
}