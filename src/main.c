#include <SDL.h>
#include <libtcod.h>
#include "external/flecs/flecs.h"
#include "stb_ds.h"

typedef enum { Wall, Floor } TileType;

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Renderable {
    char* glyph;
    TCOD_ColorRGB fg;
    TCOD_ColorRGB bg;
} Renderable;

typedef struct Resources {
    TCOD_Console* console;
    TCOD_Context* context;
    TileType* map;
} Resources;

void input(ecs_iter_t* it);
void begin_draw(ecs_iter_t* it);
void draw_console(ecs_iter_t* it);
void draw_map(ecs_iter_t* it);
void end_draw(ecs_iter_t* it);
void player_input(ecs_iter_t* it);
void try_move_player(int delta_x, int delta_y, Position* position, TileType* map);
int xy_index(int x, int y);
TileType* new_map(void);

int main(int argc, char* argv[]) {
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

    TileType* map = new_map();
    Resources world_resources = { console, context, map };

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
    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Renderable);
    ECS_TAG(world, LeftMover);
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
    ecs_set(world, e, Position, { .x=40, .y=25});
    ecs_set(world, e, Renderable, { .glyph="@", .fg=TCOD_yellow, .bg=TCOD_black});
    ecs_add_id(world, e, Player);
    
    ecs_set_target_fps(world, 60.0f);

    // Main game loop
    while (ecs_progress(world, 0)) {}

    arrfree(map);
    TCOD_console_delete(console);
    TCOD_context_delete(context);
    ecs_fini(world);

    return 0;
}

void input(ecs_iter_t* it) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                ecs_quit(it->world);
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    ecs_quit(it->world);
                break;
        }
    }
}

void begin_draw(ecs_iter_t* it) {
    Resources* r = ecs_get_context(it->world);
    TCOD_console_clear(r->console);
}

void draw_console(ecs_iter_t* it) {
    Position* p = ecs_column(it, Position, 1);
    Renderable* r = ecs_column(it, Renderable, 2);

    Resources* resources = ecs_get_context(it->world);

    for (int i = 0; i < it->count; i++)
    {
        TCOD_console_set_default_foreground(resources->console, r[i].fg);
        TCOD_console_set_default_background(resources->console, r[i].bg);
        TCOD_console_printf(resources->console, p[i].x, p[i].y, r[i].glyph);
    }
}

void draw_map(ecs_iter_t* it) {
    Resources* r = ecs_get_context(it->world);

    int y = 0;
    int x = 0;

    for (int i = 0; i < 80*50; i++)
    {
        if (r->map[i] == Floor) {
            TCOD_console_set_default_foreground(r->console, TCOD_gray);
            TCOD_console_set_default_background(r->console, TCOD_black);
            TCOD_console_printf(r->console, x, y, ".");
        }
        if (r->map[i] == Wall) {
            TCOD_console_set_default_foreground(r->console, TCOD_yellow);
            TCOD_console_set_default_background(r->console, TCOD_black);
            TCOD_console_printf(r->console, x, y, "#");
        }

        x += 1;
        if (x > 79) {
            x = 0;
            y += 1;
        }
    }
    
}

void end_draw(ecs_iter_t* it) {
    Resources* r = ecs_get_context(it->world);
    TCOD_context_present(r->context, r->console, NULL);
}

void player_input(ecs_iter_t* it) {
    Position* p = ecs_term(it, Position, 1);
    Resources* r = ecs_get_context(it->world);

    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    for (int i = 0; i < it->count; i++)
    {
        if (keystate[SDL_SCANCODE_UP]) {
            try_move_player(0, -1, p, r->map);
        }
        if (keystate[SDL_SCANCODE_DOWN]) {
            try_move_player(0, 1, p, r->map);
        }
        if (keystate[SDL_SCANCODE_LEFT]) {
            try_move_player(-1, 0 , p, r->map);
        }
        if (keystate[SDL_SCANCODE_RIGHT]) {
            try_move_player(1, 0 , p, r->map);
        }
    }
}

void try_move_player(int delta_x, int delta_y, Position* position, TileType* map) {
    int destination_index = xy_index(position->x + delta_x, position->y + delta_y);
    if (map[destination_index] != Wall) {
        position->x = MIN(79, MAX(0, position->x + delta_x));
        position->y = MIN(49, MAX(0, position->y + delta_y));
    }
}

int xy_index(int x, int y) {
    return (y * 80) + x;
}

TileType* new_map(void) {
    TileType* map = NULL;
    arrsetlen(map, 80*50);
    for (int i = 0; i < 80*50; i++)
    {
        map[i] = Floor;
    }

    for (int x = 0; x < 80; x++)
    {
        map[xy_index(x, 0)] = Wall;
        map[xy_index(x, 49)] = Wall;
    }

    for (int y = 0; y < 50; y++)
    {
        map[xy_index(0, y)] = Wall;
        map[xy_index(79, y)] = Wall;
    }
    
    for (int j = 0; j < 400; j++)
    {
        int x = (rand() % 79) + 1;
        int y = (rand() % 49) + 1;

        int idx = xy_index(x, y);
        if (idx != xy_index(40, 25)) {
            map[idx] = Wall;
        }
    }
    
    return map;
}
