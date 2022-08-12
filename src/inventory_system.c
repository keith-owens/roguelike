#include "inventory_system.h"

void item_collection_system(ecs_iter_t* it) {
    ItemPickup* ip = ecs_term(it, ItemPickup, 1);
    Position* p = ecs_term(it, Position, 2);
    Name* n = ecs_term(it, Name, 3);

    Resources* r = ecs_get_context(it->world);

    for (int i = 0; i < it->count; i++) {
        if (ip[i].wants_to_pickup) {
            ecs_remove(it->world, ip[i].item, Position);
            ecs_set(it->world, ip[i].item, InBackpack, { .owner=it->entities[i] });

            if (ecs_has_id(it->world, it->entities[i], Player)) {
                kstring_t a = { 0, 0, NULL };
                kputs("You pick up the ", &a);
                Name* name = ecs_get_id(it->world, ip->item, ecs_id(Name));
                kputs(name->name, &a);
                kputs(".", &a);

                arrpush(r->game_log->entries, a.s);
            }
            ip[i].wants_to_pickup = false;
        }
    }
}

void item_drop_system(ecs_iter_t* it) {
    DropItem* di = ecs_term(it, DropItem, 1);
    Position* p = ecs_term(it, Position, 2);

    Resources* r = ecs_get_context(it->world);

    for (int i = 0; i < it->count; i++) {
        if (di[i].wants_to_drop_item) {
            ecs_set(it->world, di[i].item, Position, { .x=p[i].x, .y=p[i].y });
            ecs_remove(it->world, di[i].item, InBackpack);


        Name* name = ecs_get_id(it->world, di[i].item, ecs_id(Name));
            kstring_t a = { 0, 0, NULL };
            kputs("You drop the ", &a);
            kputs(name->name, &a);
            arrpush(r->game_log->entries, a.s);

            di[i].wants_to_drop_item = false;
        }
    }
}
