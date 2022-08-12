#ifndef SPAWNER_H
#define SPAWNER_H

#include "external/flecs/flecs.h"
#include "external/stb/stb_ds.h"
#include "components.h"
#include "map.h"
#include "rect.h"

#define MAX_MONSTERS 4
#define MAX_ITEMS 2

ecs_entity_t player(ecs_world_t* world, int player_x, int player_y);
void random_monster(ecs_world_t* world, int x, int y);
void orc(ecs_world_t* world, int x, int y);
void goblin(ecs_world_t* world, int x, int y);
void monster(ecs_world_t* world, int x, int y, char* glyph, char* name);
void health_potion(ecs_world_t* world, int x, int y);
void spawn_room(ecs_world_t* world, Rect* room);

#endif