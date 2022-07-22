#include "monster_ai_system.h"

void monster_ai(ecs_iter_t* it) {
    Position* p = ecs_term(it, Position, 1);
    Viewshed* v = ecs_term(it, Viewshed, 2);
    Name* n = ecs_term(it, Name, 3);
    Resources* r = ecs_get_context(it->world);

    for (int i = 0; i < it->count; i++) {
        bool is_player_visible = false;
        for (int j = 0; j < arrlen(v[i].visible_tiles); j++) {
            if (v[i].visible_tiles[j].x == r->player_position->x && v[i].visible_tiles[j].y == r->player_position->y) {
                is_player_visible = true;
            }
        }
        if (is_player_visible) {
            printf("%s shouts insult.", n->name);
        }
    }
}
