#include "monster_ai_system.h"

void monster_ai(ecs_iter_t* it) {
    Position* p = ecs_term(it, Position, 1);
    Viewshed* v = ecs_term(it, Viewshed, 2);
    Name* n = ecs_term(it, Name, 3);
    MeleeAttacker* m = ecs_term(it, MeleeAttacker, 4);
    Resources* r = ecs_get_context(it->world);

    for (int i = 0; i < it->count; i++) {
        bool is_player_visible = false;
        for (int j = 0; j < arrlen(v[i].visible_tiles); j++) {
            if (v[i].visible_tiles[j].x == r->player_position->x && v[i].visible_tiles[j].y == r->player_position->y) {
                is_player_visible = true;
            }
        }
        if (is_player_visible) {
            TCOD_map_t cost_map = TCOD_map_new(r->map->width, r->map->height);
            for (int x = 0; x < r->map->width; ++x) {
                for (int y = 0; y < r->map->height; ++y) {
                    bool is_transparent = r->map->tiles[xy_index(x, y)] == Floor;
                    bool is_walkable = !r->map->tiles[xy_index(x, y)] == Wall;
                    TCOD_map_set_properties(cost_map, x, y,
                                            is_transparent,
                                            is_walkable);
                }
            }

            TCOD_path_t path = TCOD_path_new_using_map(cost_map, 1.0f);
            bool path_exists = TCOD_path_compute(path, p[i].x, p[i].y, r->player_position->x, r->player_position->y);

            if (path_exists && TCOD_path_size(path) < 1.5) {
                m[i].wants_to_melee = true;

                ecs_entity_t player;
                ecs_iter_t iter = ecs_term_iter(it->world, &(ecs_term_t){ Player });
                while (ecs_term_next(&iter)) {
                    player = iter.entities[0];
                }
                m[i].target = player;
                continue;
            }

            if (path_exists && !TCOD_path_is_empty(path))
            {
                int new_x;
                int new_y;
                if (TCOD_path_walk(path, &new_x, &new_y, true)) {
                    if (!r->map->blocked[xy_index(new_x, new_y)]) {
                        p[i].x = new_x;
                        p[i].y = new_y;
                        v[i].dirty = true;
                        r->map->blocked[xy_index(new_x, new_y)] = true;
                    }
                }
            }

            TCOD_path_delete(path);
            TCOD_map_delete(cost_map);
        }
    }
}
