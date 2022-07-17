#include "player.h"

void player_input(ecs_iter_t* it) {
    Position* p = ecs_term(it, Position, 1);
    Viewshed* v = ecs_term(it, Viewshed, 2);
    Resources* r = ecs_get_context(it->world);

    for (int i = 0; i < it->count; i++) {
        if (input_events.move_up) {
            try_move_player(0, -1, p, r->map->tiles);
            v->dirty = true;
        }
        if (input_events.move_down) {
            try_move_player(0, 1, p, r->map->tiles);
            v->dirty = true;
        }
        if (input_events.move_left) {
            try_move_player(-1, 0 , p, r->map->tiles);
            v->dirty = true;
        }
        if (input_events.move_right) {
            try_move_player(1, 0 , p, r->map->tiles);
            v->dirty = true;
        }
    }
}

void try_move_player(int delta_x, int delta_y, Position* position, TileType* tiles) {
    int destination_index = xy_index(position->x + delta_x, position->y + delta_y);
    if (tiles[destination_index] != Wall) {
        position->x = MIN(79, MAX(0, position->x + delta_x));
        position->y = MIN(49, MAX(0, position->y + delta_y));
    }
}
