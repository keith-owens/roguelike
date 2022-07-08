#include "external/flecs/flecs.h"
#include <libtcod.h>
#include <SDL.h>
#include "stb_ds.h"
#include <stdlib.h>
#include <time.h>

#include "components.h"
#include "io.h"
#include "map.h"
#include "player.h"

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

    MapInfo map_info = new_map_rooms_and_corridors();
    Resources world_resources = { console, context, map_info.map };

    // Setup ecs
    ecs_world_t* world = ecs_init();

    // Create the custom pipeline
    ECS_TAG(world, TakeInput);
    ECS_TAG(world, OnUpdate);
    ECS_TAG(world, BeforeDraw);
    ECS_TAG(world, DrawMap);
    ECS_TAG(world, DrawEntities);
    ECS_TAG(world, AfterDraw);

    ECS_PIPELINE(world, Pipeline, TakeInput, OnUpdate, BeforeDraw, DrawMap, DrawEntities, AfterDraw);
    ecs_set_pipeline(world, Pipeline);

    // Set the context needed for rendering
    ecs_set_context(world, &world_resources);

    // Add the components to the world
    ECS_COMPONENT_DEFINE(world, Position);
    ECS_COMPONENT_DEFINE(world, Renderable);
    ECS_TAG(world, Player);

    // Add the systems to the world
    ECS_SYSTEM(world, input, TakeInput, 0);
    ECS_SYSTEM(world, begin_draw, BeforeDraw, 0);
    ECS_SYSTEM(world, draw_console, DrawEntities, Position, Renderable);
    ECS_SYSTEM(world, draw_map, DrawMap, 0);
    ECS_SYSTEM(world, end_draw, AfterDraw, 0);
    ECS_SYSTEM(world, player_input, OnUpdate, Position, Player);

    // Create entities
    ecs_entity_t e = ecs_new_id(world);

    Point player_position = center(&map_info.rooms[0]);
    ecs_set(world, e, Position, { .x=player_position.x, .y=player_position.y});
    ecs_set(world, e, Renderable, { .glyph="@", .fg=TCOD_yellow, .bg=TCOD_black});
    ecs_add_id(world, e, Player);
    
    ecs_set_target_fps(world, 60.0f);

    // Main game loop
    while (ecs_progress(world, 0)) {}

    arrfree(map_info.map);
    arrfree(map_info.rooms);
    TCOD_console_delete(console);
    TCOD_context_delete(context);
    ecs_fini(world);

    return 0;
}
