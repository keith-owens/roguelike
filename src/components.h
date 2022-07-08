#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "external/flecs/flecs.h"
#include <libtcod.h>

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Renderable {
    char* glyph;
    TCOD_ColorRGB fg;
    TCOD_ColorRGB bg;
} Renderable;

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Renderable);

// Tags
ecs_entity_t Player;

#endif