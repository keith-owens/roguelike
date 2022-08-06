#include "player.h"

void player_input(ecs_iter_t* it) {
    Position* p = ecs_term(it, Position, 1);
    Viewshed* v = ecs_term(it, Viewshed, 2);
    CombatStats* c = ecs_term(it, CombatStats, 3);
    MeleeAttacker* m = ecs_term(it, MeleeAttacker, 4);
    Resources* r = ecs_get_context(it->world);

    for (int i = 0; i < it->count; i++) {
        if (input_events.move_up) {
            try_move_player(0, -1, p, c, m, r->map, it->world, it->entities[i]);
            v[i].dirty = true;
        }
        if (input_events.move_down) {
            try_move_player(0, 1, p, c, m, r->map, it->world, it->entities[i]);
            v[i].dirty = true;
        }
        if (input_events.move_left) {
            try_move_player(-1, 0 , p, c, m, r->map, it->world, it->entities[i]);
            v[i].dirty = true;
        }
        if (input_events.move_right) {
            try_move_player(1, 0 , p, c, m, r->map, it->world, it->entities[i]);
            v[i].dirty = true;
        }

        if (input_events.move_up_right) {
            try_move_player(1, -1, p, c, m, r->map, it->world, it->entities[i]);
            v[i].dirty = true;
        }
        if (input_events.move_up_left) {
            try_move_player(-1, -1, p, c, m, r->map, it->world, it->entities[i]);
            v[i].dirty = true;
        }
        if (input_events.move_down_right) {
            try_move_player(1, 1 , p, c, m, r->map, it->world, it->entities[i]);
            v[i].dirty = true;
        }
        if (input_events.move_down_left) {
            try_move_player(-1, 1 , p, c, m, r->map, it->world, it->entities[i]);
            v[i].dirty = true;
        }

        if (it->count == 1) {
            r->player_position->x = p[i].x;
            r->player_position->y = p[i].y;
        }
    }
}

void try_move_player(int delta_x, int delta_y, Position* position, CombatStats* combat_stats, MeleeAttacker* melee_attacker, Map* map, ecs_world_t* world, ecs_entity_t player) {
    int destination_index = xy_index(position->x + delta_x, position->y + delta_y);

    ecs_filter_t f;
    ecs_filter_init(world, &f, &(ecs_filter_desc_t) {
        .terms = {
            { ecs_id(Position) },
            { ecs_id(CombatStats) }
        }
     });
    ecs_iter_t it = ecs_filter_iter(world, &f);
    while (ecs_filter_next(&it)) {
        Position* p = ecs_term(&it, Position, 1);
        CombatStats* c = ecs_term(&it, CombatStats, 2);

        for (int i = 0; i < it.count; i++) {
            if (p[i].x == position->x + delta_x && p[i].y == position->y + delta_y) {
                printf("From Hell's Heart, I stab thee!");
                melee_attacker->wants_to_melee = true;
                melee_attacker->target = it.entities[i];
                return;
            }
        }
    }
    ecs_filter_fini(&f);

    if (!map->blocked[destination_index]) {
        position->x = MIN(79, MAX(0, position->x + delta_x));
        position->y = MIN(49, MAX(0, position->y + delta_y));
    }
}
