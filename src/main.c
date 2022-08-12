#include "external/flecs/flecs.h"
#include "external./klib/kstring.h"
#include <libtcod.h>
#include <SDL.h>
#include "stb_ds.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "components.h"
#include "damage_system.h"
#include "inventory_system.h"
#include "io.h"
#include "map.h"
#include "map_indexing_system.h"
#include "melee_combat_system.h"
#include "monster_ai_system.h"
#include "player.h"
#include "potion_use_system.h"
#include "spawner.h"
#include "util.h"
#include "visibility_system.h"

int main(int argc, char* argv[]) {
    // Give the world a seed
    srand(time(NULL));

    // Setup libtcod
    TCOD_Console* console = TCOD_console_new(80, 50);
    TCOD_Tileset* tileset = TCOD_tileset_load("tileset.png", 16, 16, 256, TCOD_CHARMAP_CP437);

    TCOD_ContextParams params = {
        .tcod_version = TCOD_COMPILEDVERSION,
        .console = console,
        .window_title = "Libtcod Project",
        .sdl_window_flags = SDL_WINDOW_RESIZABLE,
        .tileset = tileset,
        .vsync = true,
        .argc = argc,
        .argv = argv
    };

    TCOD_Context* context;
    TCOD_context_new(&params, &context);

    initialize_state();

    Map map = new_map_rooms_and_corridors();

    // Setup ecs
    ecs_world_t* world = ecs_init();

    // Create the custom pipeline
    ECS_TAG(world, PlayerTurn);
    ECS_TAG(world, MonsterTurn);
    ECS_TAG(world, PostTurn);
    ECS_TAG(world, ResolveStep);
    ECS_TAG(world, CleanUp);
    ECS_TAG(world, BeforeDraw);
    ECS_TAG(world, DrawMap);
    ECS_TAG(world, DrawUI);
    ECS_TAG(world, DrawEntities);
    ECS_TAG(world, AfterDraw);
    ECS_TAG(world, TakeInput);

    ECS_PIPELINE(world, Pipeline, PlayerTurn, MonsterTurn, PostTurn, ResolveStep, CleanUp, BeforeDraw, DrawMap, DrawUI, DrawEntities, AfterDraw, TakeInput);
    ECS_PIPELINE(world, PausedPipeline, BeforeDraw, DrawMap, DrawUI, DrawEntities, AfterDraw, TakeInput);
    ecs_set_pipeline(world, Pipeline);

    // Add the components to the world
    ECS_COMPONENT_DEFINE(world, Position);
    ECS_COMPONENT_DEFINE(world, Renderable);
    ECS_COMPONENT_DEFINE(world, Viewshed);
    ECS_COMPONENT_DEFINE(world, Name);
    ECS_COMPONENT_DEFINE(world, CombatStats);
    ECS_COMPONENT_DEFINE(world, MeleeAttacker);
    ECS_COMPONENT_DEFINE(world, Potion);
    ECS_COMPONENT_DEFINE(world, InBackpack);
    ECS_COMPONENT_DEFINE(world, ItemPickup);
    ECS_COMPONENT_DEFINE(world, DrinkPotion);
    ECS_COMPONENT_DEFINE(world, DropItem);
    ECS_TAG_DEFINE(world, Player);
    ECS_TAG_DEFINE(world, Monster);
    ECS_TAG_DEFINE(world, BlocksTile);
    ECS_TAG_DEFINE(world, Item);

    // Add the systems to the world
    ECS_SYSTEM(world, input, TakeInput, 0);
    ECS_SYSTEM(world, begin_draw, BeforeDraw, 0);
    ECS_SYSTEM(world, draw_console, DrawEntities, Position, Renderable);
    ECS_SYSTEM(world, draw_map, DrawMap, 0);
    ECS_SYSTEM(world, draw_ui, DrawUI, 0);
    ECS_SYSTEM(world, end_draw, AfterDraw, 0);
    ECS_SYSTEM(world, player_input, PlayerTurn, Position, Viewshed, CombatStats, MeleeAttacker, ItemPickup, Player);
    ECS_SYSTEM(world, potion_use_system, AfterDraw, DrinkPotion, Name, CombatStats);
    ECS_SYSTEM(world, item_drop_system, AfterDraw, DropItem, Position);
    ECS_SYSTEM(world, visiblity_system, PostTurn, Viewshed, Position);
    ECS_SYSTEM(world, monster_ai, MonsterTurn, Position, Viewshed, Name, MeleeAttacker, Monster);
    ECS_SYSTEM(world, map_indexing_system, PostTurn, Position);
    ECS_SYSTEM(world, melee_combat_system, PostTurn, MeleeAttacker, Name, CombatStats);
    ECS_SYSTEM(world, item_collection_system, PostTurn, ItemPickup, Position, Name);
    ECS_SYSTEM(world, damage_system, ResolveStep, CombatStats);
    ECS_SYSTEM(world, delete_the_dead, CleanUp, CombatStats, Name);

    // Create player entity
    Point player_position = center(&map.rooms[0]);
    ecs_entity_t player_entity = player(world, player_position.x, player_position.y);

    // Create enemies - Skip first room
    for (int i = 1; i < arrlen(map.rooms); i++)
    {
        spawn_room(world, &map.rooms[i]);
    }
    
    ecs_set_target_fps(world, 60.0f);

    // Set the context needed for rendering
    GameLog game_log;
    game_log.entries = NULL;
    arrpush(game_log.entries, "Welcome to this C roguelike.");
    Resources world_resources = { console, context, &map, &player_position, &game_log };
    ecs_set_context(world, &world_resources);

    // Main game loop
    while (ecs_progress(world, 0)) {
        if (game_state == TakingTurns) {
            ecs_set_pipeline(world, Pipeline);
        }
        else if (game_state == WaitingOnInput) {
            ecs_set_pipeline(world, PausedPipeline);
        }
        else if (game_state == InventorySelection) {
            ecs_set_pipeline(world, PausedPipeline);
        }
        else if (game_state == DropSelection) {
            ecs_set_pipeline(world, PausedPipeline);
        }
    }

    // Clean up the dynamic arrays in Viewshed components
    ecs_iter_t it = ecs_term_iter(world, &(ecs_term_t){ ecs_id(Viewshed) });
    while (ecs_term_next(&it)) {
        Viewshed* v = ecs_term(&it, Viewshed, 1);

        for (int i = 0; i < it.count; i++) {
            arrfree(v[i].visible_tiles);
        }
    }

    // Clean up the data used in the game
    arrfree(map.visible_tiles);
    arrfree(map.tiles);
    arrfree(map.rooms);
    TCOD_console_delete(console);
    TCOD_context_delete(context);
    ecs_fini(world);

    return 0;
}
