#include "spawner.h"

ecs_entity_t player(ecs_world_t* world, int player_x, int player_y) {
    ecs_entity_t e = ecs_new_id(world);

    ecs_set(world, e, Position, { .x=player_x, .y=player_y });
    ecs_set(world, e, Renderable, { .glyph="@", .fg=TCOD_yellow, .bg=TCOD_black });
    ecs_set(world, e, Viewshed, { .visible_tiles=NULL, .range=8, .dirty=true});
    ecs_set(world, e, CombatStats, { .max_hp=300, .hp=300, .defense=2, .power=5, .damage_taken=0 });
    ecs_set(world, e, MeleeAttacker, { .wants_to_melee=false, .target=NULL });
    ecs_set(world, e, Name, { .name="Player" });
    ecs_set(world, e, ItemPickup, { .wants_to_pickup=false, .item=NULL });
    ecs_set(world, e, DrinkPotion, { .wants_to_drink_potion=false, .potion=NULL });
    ecs_set(world, e, DropItem, { .wants_to_drop_item=false, .item=NULL });
    ecs_add_id(world, e, Player);

    return e;
}

void random_monster(ecs_world_t* world, int x, int y) {
    int roll = random_in_range(1, 2);

    if (roll == 1) {
        orc(world, x, y);
    } else {
        goblin(world, x, y);
    }
}

void orc(ecs_world_t* world, int x, int y) {
    monster(world, x, y, "o", "Orc");
}

void goblin(ecs_world_t* world, int x, int y) {
    monster(world, x, y, "g", "Goblin");
}

void monster(ecs_world_t* world, int x, int y, char* glyph, char* name) {
    ecs_entity_t enemy = ecs_new_id(world);
    ecs_set(world, enemy, Position, { .x=x, .y=y });
    ecs_set(world, enemy, Renderable, { .glyph=glyph, .fg=TCOD_red, .bg=TCOD_black });
    ecs_set(world, enemy, Viewshed, { .visible_tiles=NULL, .range=8, .dirty=true });
    ecs_set(world, enemy, Name, { .name=name });
    ecs_set(world, enemy, CombatStats, { .max_hp=16, .hp=16, .defense=1, .power=4, .damage_taken=0 });
    ecs_set(world, enemy, MeleeAttacker, { .wants_to_melee=false, .target=NULL });
    ecs_add_id(world, enemy, Monster);
    ecs_add_id(world, enemy, BlocksTile);
}

void health_potion(ecs_world_t* world, int x, int y) {
    ecs_entity_t potion = ecs_new_id(world);
    ecs_set(world, potion, Position, { .x=x, .y=y });
    ecs_set(world, potion, Renderable, { .glyph="i", .fg=TCOD_magenta, .bg=TCOD_black });
    ecs_set(world, potion, Name, { .name="Health Potion" });
    ecs_add_id(world, potion, Item);
    ecs_set(world, potion, Potion, { .heal_amount=8 });
}

void spawn_room(ecs_world_t* world, Rect* room) {
    int* monster_spawn_points = NULL;
    int* item_spawn_points = NULL;

    int num_monsters = random_in_range(1, MAX_MONSTERS);
    int num_items = random_in_range(1, MAX_ITEMS);

    for (int i = 0; i < num_monsters; i++) {
        bool added = false;
        while (!added) {
            int x = (room->x1 + random_in_range(1, abs(room->x2 - room->x1)) - 1);
            int y = (room->y1 + random_in_range(1, abs(room->y2 - room->y1)) - 1);
            int idx = (y * MAP_WIDTH) + x;
            bool contains_point = false;
            for (int j = 0; j < arrlen(monster_spawn_points); j++) {
                if (monster_spawn_points[j] == idx)
                    contains_point = true;
            }
            
            if (!contains_point) {
                arrpush(monster_spawn_points, idx);
                added = true;
            }
        }
    }

    for (int a = 0; a < num_items; a++) {
        bool added = false;
        while (!added) {
            int x = (room->x1 + random_in_range(1, abs(room->x2 - room->x1)) - 1);
            int y = (room->y1 + random_in_range(1, abs(room->y2 - room->y1)) - 1);
            int idx = (y * MAP_WIDTH) + x;
            bool contains_point = false;
            for (int j = 0; j < arrlen(item_spawn_points); j++) {
                if (item_spawn_points[j] == idx)
                    contains_point = true;
            }
            
            if (!contains_point) {
                arrpush(item_spawn_points, idx);
                added = true;
            }
        }
    }
    

    // Spawn the monsters
    for (int k = 0; k < arrlen(monster_spawn_points); k++) {
        int x = monster_spawn_points[k] % MAP_WIDTH;
        int y = monster_spawn_points[k] / MAP_WIDTH;
        random_monster(world, x, y);
    }

    // Spawn the potions
    for (int b = 0; b < arrlen(item_spawn_points); b++) {
        int x = item_spawn_points[b] % MAP_WIDTH;
        int y = item_spawn_points[b] / MAP_WIDTH;
        health_potion(world, x, y);
    }
}
