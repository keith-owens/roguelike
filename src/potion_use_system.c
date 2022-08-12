#include "potion_use_system.h"

void potion_use_system(ecs_iter_t* it) {
    DrinkPotion* dp = ecs_term(it, DrinkPotion, 1);
    Name* n = ecs_term(it, Name, 2);
    CombatStats* cs = ecs_term(it, CombatStats, 3);

    Resources* r = ecs_get_context(it->world);
    
    for (int i = 0; i < it->count; i++) {
        if (dp[i].wants_to_drink_potion) {
            if (ecs_has_id(it->world, dp[i].potion, ecs_id(Potion))) {
                Potion* potion = ecs_get_id(it->world, dp[i].potion, ecs_id(Potion));
                Name* potion_name = ecs_get_id(it->world, dp[i].potion, ecs_id(Name));
                cs[i].hp = MIN(cs[i].max_hp, cs[i].hp + potion->heal_amount);
                if (ecs_has_id(it->world, it->entities[i], Player)) {
                    kstring_t a = { 0, 0, NULL };
                    kputs("You drink the ", &a);
                    kputs(potion_name->name, &a);
                    kputs(", healing ", &a);
                    kputuw(potion->heal_amount, &a);
                    kputs(" hp.", &a);
                    arrpush(r->game_log->entries, a.s);
                }

                ecs_delete(it->world, dp[i].potion);
                dp[i].potion = NULL;
            }

            dp[i].wants_to_drink_potion = false;
        }
    }
}
