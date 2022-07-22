#include "external/flecs/flecs.h"
#include <libtcod.h>
#include <SDL.h>
#include "stb_ds.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "components.h"
#include "io.h"
#include "map.h"
#include "monster_ai_system.h"
#include "player.h"
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

    Map map = new_map_rooms_and_corridors();

    // Setup ecs
    ecs_world_t* world = ecs_init();

    // Create the custom pipeline
    ECS_TAG(world, OnUpdate);
    ECS_TAG(world, BeforeDraw);
    ECS_TAG(world, DrawMap);
    ECS_TAG(world, DrawEntities);
    ECS_TAG(world, AfterDraw);
    ECS_TAG(world, TakeInput);

    ECS_PIPELINE(world, Pipeline, OnUpdate, BeforeDraw, DrawMap, DrawEntities, AfterDraw, TakeInput);
    ECS_PIPELINE(world, PausedPipeline, BeforeDraw, DrawMap, DrawEntities, AfterDraw, TakeInput);
    ecs_set_pipeline(world, Pipeline);

    // Add the components to the world
    ECS_COMPONENT_DEFINE(world, Position);
    ECS_COMPONENT_DEFINE(world, Renderable);
    ECS_COMPONENT_DEFINE(world, Viewshed);
    ECS_COMPONENT_DEFINE(world, Name);
    ECS_TAG_DEFINE(world, Player);
    ECS_TAG_DEFINE(world, Monster);

    // Add the systems to the world
    ECS_SYSTEM(world, input, TakeInput, 0);
    ECS_SYSTEM(world, begin_draw, BeforeDraw, 0);
    ECS_SYSTEM(world, draw_console, DrawEntities, Position, Renderable);
    ECS_SYSTEM(world, draw_map, DrawMap, 0);
    ECS_SYSTEM(world, end_draw, AfterDraw, 0);
    ECS_SYSTEM(world, player_input, OnUpdate, Position, Viewshed, Player);
    ECS_SYSTEM(world, visiblity_system, OnUpdate, Viewshed, Position);
    ECS_SYSTEM(world, monster_ai, OnUpdate, Position, Viewshed, Name, Monster);

    // Create player entity
    ecs_entity_t e = ecs_new_id(world);

    Point player_position = center(&map.rooms[0]);
    ecs_set(world, e, Position, { .x=player_position.x, .y=player_position.y });
    ecs_set(world, e, Renderable, { .glyph="@", .fg=TCOD_yellow, .bg=TCOD_black });
    ecs_set(world, e, Viewshed, { .visible_tiles=NULL, .range=8 });
    ecs_add_id(world, e, Player);

    // Create enemies - Skip first room
    for (int i = 1; i < arrlen(map.rooms); i++)
    {
        Point center_of_room = center(&map.rooms[i]);
        int x = center_of_room.x;
        int y = center_of_room.y;

        // Random chance for goblin or orc
        char* glyph;
        char* name;
        int roll = random_in_range(1, 2);
        if (roll == 1) {
            glyph = "g";
            name = "Goblin";
        }
        else {
            glyph = "o";
            name = "Orc";
        }

        ecs_entity_t enemy = ecs_new_id(world);
        ecs_set(world, enemy, Position, { .x=x, .y=y });
        ecs_set(world, enemy, Renderable, { .glyph=glyph, .fg=TCOD_red, .bg=TCOD_black });
        ecs_set(world, enemy, Viewshed, { .visible_tiles=NULL, .range=8 });
        ecs_set(world, enemy, Name, { .name=name });
        ecs_add_id(world, enemy, Monster);
    }
    
    ecs_set_target_fps(world, 60.0f);

    // Set the context needed for rendering
    Resources world_resources = { console, context, &map, &player_position };
    ecs_set_context(world, &world_resources);

    // Main game loop
    while (ecs_progress(world, 0)) {
        if (paused) {
            ecs_set_pipeline(world, Pipeline);
        }
        else {
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
