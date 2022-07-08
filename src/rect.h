#ifndef RECT_H
#define RECT_H

#include <stdbool.h>

#include "util.h"

typedef struct Rect {
    int x1;
    int x2;
    int y1;
    int y2;
} Rect;

Rect new_rect(int x, int y, int width, int height);
bool intersect(Rect* r1, Rect* r2);
Point center(Rect* rect);

#endif