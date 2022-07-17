#include "visibility_system.h"

void visiblity_system(ecs_iter_t* it) {
    Viewshed* v = ecs_term(it, Viewshed, 1);
    Position* p = ecs_term(it, Position, 2);
    Resources* r = ecs_get_context(it->world);

    for (int i = 0; i < it->count; i++) {
        if (!v->dirty) continue; // if fov is not dirty it does not need to be updated

        if (arrlen(v[i].visible_tiles) > 0) {
            arrfree(v[i].visible_tiles);
            v[i].visible_tiles = NULL;
        }

        TCOD_Map* fov_map = TCOD_map_new(r->map->width, r->map->height);
        for (int x = 0; x < r->map->width; ++x) {
            for (int y = 0; y < r->map->height; ++y) {
                bool is_transparent = r->map->tiles[xy_index(x, y)] == Floor;
                TCOD_map_set_properties(fov_map, x, y,
                                        is_transparent,
                                        r->map->tiles[xy_index(x, y)] == Wall);
            }
        }

        TCOD_map_compute_fov(fov_map, p[i].x, p[i].y, v[i].range, true, FOV_BASIC);
        
        for (int x = 0; x < r->map->width; ++x) {
            for (int y = 0; y < r->map->height; ++y) {
                if (TCOD_map_is_in_fov(fov_map, x, y)) {
                    Point point = {x, y};
                    arrpush(v[i].visible_tiles, point);
                    if (ecs_has_id(it->world, it->entities[i], Player)) {
                        r->map->revealed_tiles[xy_index(x, y)] = true;
                    }
                }
            }
        }

        v->dirty = false;
    }
}
