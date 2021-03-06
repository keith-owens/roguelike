#ifndef PLAYER_H
#define PLAYER_H

#include "components.h"
#include "external/flecs/flecs.h"
#include "io.h"
#include "map.h"
#include <SDL.h>

void player_input(ecs_iter_t* it);
void try_move_player(int delta_x, int delta_y, Position* position, TileType* tiles);

#endif