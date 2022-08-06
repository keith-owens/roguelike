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
    bool dirty;
} Viewshed;

typedef struct Name {
    char* name;
    int value;
} Name;

typedef struct CombatStats {
    int max_hp;
    int hp;
    int defense;
    int power;
    int damage_taken;
} CombatStats;

typedef struct MeleeAttacker {
    bool wants_to_melee;
    ecs_entity_t target;
} MeleeAttacker;

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Renderable);
extern ECS_COMPONENT_DECLARE(Viewshed);
extern ECS_COMPONENT_DECLARE(Name);
extern ECS_COMPONENT_DECLARE(CombatStats);
extern ECS_COMPONENT_DECLARE(MeleeAttacker);

// Tags
ecs_entity_t Player;
ecs_entity_t Monster;
ecs_entity_t BlocksTile;

extern ECS_TAG_DECLARE(Player);
extern ECS_TAG_DECLARE(Monster);
extern ECS_TAG_DECLARE(BlocksTile);

#endif