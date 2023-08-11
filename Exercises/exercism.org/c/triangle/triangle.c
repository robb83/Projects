#include "triangle.h"

bool is_triangle(triangle_t t)
{
    return (t.c > 0 && t.b > 0 && t.a > 0) && (t.a + t.b >= t.c) && (t.b + t.c >= t.a) && (t.a + t.c >= t.b);
}

bool is_equilateral(triangle_t t)
{
    return is_triangle(t) && t.a == t.b && t.a == t.c;
}

bool is_isosceles(triangle_t t)
{
    int same = 0;
    if (t.a == t.b) same++;
    if (t.a == t.c) same++;
    if (t.b == t.c) same++;
    return  is_triangle(t) && same > 0;
}

bool is_scalene(triangle_t t)
{
    return  is_triangle(t) && t.a != t.b && t.a != t.c && t.b != t.c;
}