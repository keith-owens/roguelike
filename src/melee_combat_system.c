#include "melee_combat_system.h"

void melee_combat_system(ecs_iter_t* it) {
    MeleeAttacker* m = ecs_term(it, MeleeAttacker, 1);
    Name* n = ecs_term(it, Name, 2);
    CombatStats* c = ecs_term(it, CombatStats, 3);

    Resources* r = ecs_get_context(it->world);

    for (int i = 0; i < it->count; i++) {
        if (m[i].wants_to_melee && c[i].hp > 0) {
            CombatStats* target_stats = ecs_get_id(it->world, m[i].target, ecs_id(CombatStats));
            if (target_stats->hp > 0) {
                Name* target_name = ecs_get_id(it->world, m[i].target, ecs_id(Name));
                int damage = MAX(0, c[i].power - target_stats->defense);

                if (damage == 0) {
                    kstring_t a = { 0, 0, NULL };
                    kputs(n[i].name, &a);
                    kputs(" is unable to hurt ", &a);
                    kputs(target_name->name, &a);
                    arrpush(r->game_log->entries, a.s);
                } else {
                    kstring_t a = { 0, 0, NULL };
                    kputs(n[i].name, &a);
                    kputs(" hits ", &a);
                    kputs(target_name->name, &a);
                    kputs(", for ", &a);
                    kputuw(damage, &a);
                    kputs(" hp.", &a);
                    arrpush(r->game_log->entries, a.s);

                    target_stats->damage_taken += damage;
                    m[i].wants_to_melee = false;
                }
            }
        }
    }
}
