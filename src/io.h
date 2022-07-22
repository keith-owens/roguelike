#ifndef IO_H
#define IO_H

#include "external/flecs/flecs.h"
#include <libtcod.h>
#include <SDL.h>
#include <stdbool.h>

#include "components.h"
#include "map.h"

typedef struct Resources {
    TCOD_Console* console;
    TCOD_Context* context;
    Map* map;
    Point* player_position;
} Resources;

typedef struct InputEvents {
    bool move_up;
    bool move_down;
    bool move_left;
    bool move_right;
} InputEvents;

extern InputEvents input_events;
extern bool paused;

void input(ecs_iter_t* it);
void begin_draw(ecs_iter_t* it);
void draw_console(ecs_iter_t* it);
void draw_map(ecs_iter_t* it);
void end_draw(ecs_iter_t* it);

#endif