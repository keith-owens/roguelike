#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

typedef struct Point {
    int x;
    int y;
} Point;

int random_in_range(int min, int max);

#endif