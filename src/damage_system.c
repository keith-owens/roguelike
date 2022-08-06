#include "damage_system.h"

void damage_system(ecs_iter_t* it) {
    CombatStats* c = ecs_term(it, CombatStats, 1);

    for (int i = 0; i < it->count; i++) {
        if (c[i].damage_taken > 0) {
            c[i].hp -= c[i].damage_taken;
            c[i].damage_taken = 0;
        }
    }
}

void delete_the_dead(ecs_iter_t* it) {
    CombatStats* c = ecs_term(it, CombatStats, 1);
    Name* n = ecs_term(it, Name, 2);
    Resources* r = ecs_get_context(it->world);


    for (int i = 0; i < it->count; i++) {
        if (c[i].hp < 1) {
            if (ecs_has_id(it->world, it->entities[i], Player))
            {
                arrpush(r->game_log->entries, "You are dead.");
            } else {
                kstring_t a = { 0, 0, NULL };
                kputs(n[i].name, &a);
                kputs(" is dead.", &a);
                arrpush(r->game_log->entries, a.s);
                ecs_delete(it->world, it->entities[i]);
            }
        }
    }
}
