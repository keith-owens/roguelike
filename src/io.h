#ifndef IO_H
#define IO_H

#include "external/flecs/flecs.h"
#include <libtcod.h>
#include <SDL.h>
#include <stdbool.h>

#include "components.h"
#include "game_log.h"
#include "map.h"
#include "util.h"

typedef struct Resources {
    TCOD_Console* console;
    TCOD_Context* context;
    Map* map;
    Point* player_position;
    GameLog* game_log;
} Resources;

typedef struct InputEvents {
    bool move_up;
    bool move_down;
    bool move_left;
    bool move_right;
    bool move_up_right;
    bool move_up_left;
    bool move_down_right;
    bool move_down_left;
} InputEvents;

extern InputEvents input_events;
extern bool paused;

void input(ecs_iter_t* it);
void begin_draw(ecs_iter_t* it);
void draw_console(ecs_iter_t* it);
void draw_map(ecs_iter_t* it);
void draw_ui(ecs_iter_t* it);
void draw_tooltips(ecs_world_t* world, Resources* resources);
void end_draw(ecs_iter_t* it);

#endif