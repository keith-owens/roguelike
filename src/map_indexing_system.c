#include "map_indexing_system.h"

void map_indexing_system(ecs_iter_t* it) {
    Position* p = ecs_term(it, Position, 1);
    Resources* r = ecs_get_context(it->world);

    populate_blocked(r->map);
    for (int i = 0; i < it->count; i++)
    {
        int x = p[i].x;
        int y = p[i].y;
        int idx = xy_index(p[i].x, p[i].y);

        if (ecs_has_id(it->world, it->entities[i], BlocksTile))
            r->map->blocked[idx] = true;
    }
}
