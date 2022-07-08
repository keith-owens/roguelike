#include "io.h"

InputEvents input_events;

void input(ecs_iter_t* it) {
    input_events.move_up = false;
    input_events.move_down = false;
    input_events.move_left = false;
    input_events.move_right = false;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                ecs_quit(it->world);
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    ecs_quit(it->world);
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_KP_8 || event.key.keysym.sym == SDLK_k)
                    input_events.move_up = true;
                if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_KP_2 || event.key.keysym.sym == SDLK_j)
                    input_events.move_down = true;
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_KP_4 || event.key.keysym.sym == SDLK_h)
                    input_events.move_left = true;
                if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_KP_6 || event.key.keysym.sym == SDLK_l)
                    input_events.move_right = true;
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

void end_draw(ecs_iter_t* it) {
    Resources* r = ecs_get_context(it->world);
    TCOD_context_present(r->context, r->console, NULL);
}
