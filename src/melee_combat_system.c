#include "melee_combat_system.h"

void melee_combat_system(ecs_iter_t* it) {
    MeleeAttacker* m = ecs_term(it, MeleeAttacker, 1);
    Name* n = ecs_term(it, Name, 2);
    CombatStats* c = ecs_term(it, CombatStats, 3);

    for (int i = 0; i < it->count; i++) {
        if (m[i].wants_to_melee && c[i].hp > 0) {
            CombatStats* target_stats = ecs_get_id(it->world, m[i].target, ecs_id(CombatStats));
            if (target_stats->hp > 0) {
                Name* target_name = ecs_get_id(it->world, m[i].target, ecs_id(Name));
                int damage = MAX(0, c[i].power - target_stats->defense);

                if (damage == 0) {
                    printf("%s%d is unable to hurt %s%d", n[i].name, n[i].value, target_name->name, target_name->value);
                } else {
                    printf("%s%d hits %s%d, for %d hp.", n[i].name, n[i].value, target_name->name, target_name->value, damage);
                    target_stats->damage_taken += damage;
                    m[i].wants_to_melee = false;
                }
            }
        }
    }
}
