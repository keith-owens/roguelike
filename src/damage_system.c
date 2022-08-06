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

    for (int i = 0; i < it->count; i++) {
        if (c[i].hp < 1) {
            if (ecs_has_id(it->world, it->entities[i], Player))
            {
                printf("You are dead");
            } else {
                ecs_delete(it->world, it->entities[i]);
            }
        }
    }
}
