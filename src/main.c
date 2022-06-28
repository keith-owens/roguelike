#include <SDL.h>
#include <libtcod.h>
#include "external/flecs/flecs.h"

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Renderable {
    char* glyph;
    TCOD_ColorRGB fg;
    TCOD_ColorRGB bg;
} Renderable;

typedef struct ConsoleInfo {
    TCOD_Console* console;
    TCOD_Context* context;
} ConsoleInfo;

void input(ecs_iter_t* it);
void begin_draw(ecs_iter_t* it);
void draw_console(ecs_iter_t* it);
void end_draw(ecs_iter_t* it);
void move_left(ecs_iter_t* it);
void player_input(ecs_iter_t* it);
void try_move_player(int delta_x, int delta_y, Position* position);

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

    ConsoleInfo console_info = { console, context };

    // Setup ecs
    ecs_world_t* world = ecs_init();

    // Create the custom pipeline
    ECS_TAG(world, TakeInput);
    ECS_TAG(world, OnUpdate);
    ECS_TAG(world, BeforeDraw);
    ECS_TAG(world, Draw);
    ECS_TAG(world, AfterDraw);

    ECS_PIPELINE(world, Pipeline, TakeInput, OnUpdate, BeforeDraw, Draw, AfterDraw);
    ecs_set_pipeline(world, Pipeline);

    // Add the components to the world
    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Renderable);
    ECS_TAG(world, LeftMover);
    ECS_TAG(world, Player);

    // Add the systems to the world
    ECS_SYSTEM(world, input, TakeInput, 0);
    ECS_SYSTEM(world, begin_draw, BeforeDraw, 0);
    ECS_SYSTEM(world, draw_console, Draw, Position, Renderable);
    ECS_SYSTEM(world, end_draw, AfterDraw, 0);
    ECS_SYSTEM(world, move_left, OnUpdate, Position, LeftMover);
    ECS_SYSTEM(world, player_input, OnUpdate, Position, Player);

    // Set the context needed for rendering
    ecs_set_context(world, &console_info);

    // Create entities
    ecs_entity_t e = ecs_new_id(world);
    ecs_set(world, e, Position, { .x=40, .y=25});
    ecs_set(world, e, Renderable, { .glyph="@", .fg=TCOD_yellow, .bg=TCOD_black});
    ecs_add_id(world, e, Player);

    for (int i = 0; i < 10; i++)
    {
        ecs_entity_t new_entity = ecs_new_id(world);
        ecs_set(world, new_entity, Position, { .x=i*7, .y=20 });
        ecs_set(world, new_entity, Renderable, { .glyph="☺", .fg=TCOD_red, .bg=TCOD_black });
        ecs_add_id(world, new_entity, LeftMover);
    }
    
    ecs_set_target_fps(world, 60.0f);

    // Main game loop
    while (ecs_progress(world, 0)) {}

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
        }
    }
}

void begin_draw(ecs_iter_t* it) {
    ConsoleInfo* ci = ecs_get_context(it->world);
    TCOD_console_clear(ci->console);
}

void draw_console(ecs_iter_t* it) {
    Position* p = ecs_column(it, Position, 1);
    Renderable* r = ecs_column(it, Renderable, 2);

    ConsoleInfo* c = ecs_get_context(it->world);

    for (int i = 0; i < it->count; i++)
    {
        TCOD_console_set_default_foreground(c->console, r[i].fg);
        TCOD_console_set_default_background(c->console, r[i].bg);
        TCOD_console_printf(c->console, p[i].x, p[i].y, r[i].glyph);
    }
}

void end_draw(ecs_iter_t* it) {
    ConsoleInfo* ci = ecs_get_context(it->world);
    TCOD_context_present(ci->context, ci->console, NULL);
}

void move_left(ecs_iter_t* it) {
    Position *p = ecs_term(it, Position, 1);

    for (int i = 0; i < it->count; i++)
    {
        p[i].x -= 1;
        if (p[i].x < 0) {
            p[i].x = 79;
        }
    }
}

void player_input(ecs_iter_t* it) {
    Position* p = ecs_term(it, Position, 1);

    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    for (int i = 0; i < it->count; i++)
    {
        if (keystate[SDL_SCANCODE_UP]) {
            try_move_player(0, -1, p);
        }
        if (keystate[SDL_SCANCODE_DOWN]) {
            try_move_player(0, 1, p);
        }
        if (keystate[SDL_SCANCODE_LEFT]) {
            try_move_player(-1, 0 , p);
        }
        if (keystate[SDL_SCANCODE_RIGHT]) {
            try_move_player(1, 0 , p);
        }
    }
}

void try_move_player(int delta_x, int delta_y, Position* position) {
    position->x = MIN(79, MAX(0, position->x + delta_x));
    position->y = MIN(49, MAX(0, position->y + delta_y));
}
