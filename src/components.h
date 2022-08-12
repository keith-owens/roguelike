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

typedef struct Potion {
    int heal_amount;
} Potion;

typedef struct InBackpack {
    ecs_entity_t owner;
} InBackpack;

typedef struct ItemPickup {
    bool wants_to_pickup;
    ecs_entity_t item;
} ItemPickup;

typedef struct DrinkPotion {
    bool wants_to_drink_potion;
    ecs_entity_t potion;
} DrinkPotion;

typedef struct DropItem {
    bool wants_to_drop_item;
    ecs_entity_t item;
} DropItem;

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Renderable);
extern ECS_COMPONENT_DECLARE(Viewshed);
extern ECS_COMPONENT_DECLARE(Name);
extern ECS_COMPONENT_DECLARE(CombatStats);
extern ECS_COMPONENT_DECLARE(MeleeAttacker);
extern ECS_COMPONENT_DECLARE(Potion);
extern ECS_COMPONENT_DECLARE(InBackpack);
extern ECS_COMPONENT_DECLARE(ItemPickup);
extern ECS_COMPONENT_DECLARE(DrinkPotion);
extern ECS_COMPONENT_DECLARE(DropItem);

// Tags
ecs_entity_t Player;
ecs_entity_t Monster;
ecs_entity_t BlocksTile;
ecs_entity_t Item;

extern ECS_TAG_DECLARE(Player);
extern ECS_TAG_DECLARE(Monster);
extern ECS_TAG_DECLARE(BlocksTile);
extern ECS_TAG_DECLARE(Item);

#endif