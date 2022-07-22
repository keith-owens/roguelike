#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "external/flecs/flecs.h"
#include <libtcod.h>
#include "util.h"

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Renderable {
    char* glyph;
    TCOD_ColorRGB fg;
    TCOD_ColorRGB bg;
} Renderable;

typedef struct Viewshed {
    Point* visible_tiles;
    int range;
} Viewshed;

typedef struct Name {
    char* name;
} Name;

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Renderable);
extern ECS_COMPONENT_DECLARE(Viewshed);
extern ECS_COMPONENT_DECLARE(Name);

// Tags
ecs_entity_t Player;
ecs_entity_t Monster;

extern ECS_TAG_DECLARE(Player);
extern ECS_TAG_DECLARE(Monster);

#endif