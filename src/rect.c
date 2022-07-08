#include "rect.h"

Rect new_rect(int x, int y, int width, int height) {
    return (Rect){ .x1=x, .y1=y, .x2=x+width, .y2=y+height };
}

bool intersect(Rect* r1, Rect* r2) {
    return r1->x1 <= r2->x2 && r1->x2 >= r2->x1 && r1->y1 <= r2->y2 && r1->y2 >= r2->y1;
}

Point center(Rect* rect) {
    return (Point){ (rect->x1 + rect->x2)/2, (rect->y1 + rect->y2)/2 };
}
