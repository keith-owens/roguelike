#include "io.h"

InputEvents input_events;
bool paused;

void input(ecs_iter_t* it) {
    input_events.move_up = false;
    input_events.move_down = false;
    input_events.move_left = false;
    input_events.move_right = false;
    input_events.move_up_right = false;
    input_events.move_up_left = false;
    input_events.move_down_right = false;
    input_events.move_down_left = false;

    paused = false;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                ecs_quit(it->world);
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    ecs_quit(it->world);
                if (event.key.keysym.sym == SDLK_UP ||
                    event.key.keysym.sym == SDLK_KP_8 ||
                    event.key.keysym.sym == SDLK_k)
                    input_events.move_up = true;
                if (event.key.keysym.sym == SDLK_DOWN ||
                    event.key.keysym.sym == SDLK_KP_2 ||
                    event.key.keysym.sym == SDLK_j)
                    input_events.move_down = true;
                if (event.key.keysym.sym == SDLK_LEFT ||
                    event.key.keysym.sym == SDLK_KP_4 ||
                    event.key.keysym.sym == SDLK_h)
                    input_events.move_left = true;
                if (event.key.keysym.sym == SDLK_RIGHT ||
                    event.key.keysym.sym == SDLK_KP_6 ||
                    event.key.keysym.sym == SDLK_l)
                    input_events.move_right = true;
                
                // Diagonals
                if (event.key.keysym.sym == SDLK_KP_9 ||
                    event.key.keysym.sym == SDLK_y)
                    input_events.move_up_right = true;
                if (event.key.keysym.sym == SDLK_KP_7 ||
                    event.key.keysym.sym == SDLK_u)
                    input_events.move_up_left = true;
                if (event.key.keysym.sym == SDLK_KP_3 ||
                    event.key.keysym.sym == SDLK_n)
                    input_events.move_down_right = true;
                if (event.key.keysym.sym == SDLK_KP_1 ||
                    event.key.keysym.sym == SDLK_b)
                    input_events.move_down_left = true;
                break;
        }
    }

    if (input_events.move_up || input_events.move_down || input_events.move_left || input_events.move_right ||
        input_events.move_up_right || input_events.move_up_left || input_events.move_down_right || input_events.move_down_left) {
        paused = true;
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
        if (arrlen(resources->map->visible_tiles) > 0 && resources->map->visible_tiles[xy_index(p[i].x, p[i].y)]) {
            TCOD_console_set_default_foreground(resources->console, r[i].fg);
            TCOD_console_set_default_background(resources->console, r[i].bg);
            TCOD_console_printf(resources->console, p[i].x, p[i].y, r[i].glyph);
        }
    }
}

void draw_map(ecs_iter_t* it) {
    Resources* r = ecs_get_context(it->world);

    int x = 0;
    int y = 0;

    int mouse_x;
    int mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    mouse_x = (int)((float)mouse_x / 16.0f);
    mouse_y = (int)((float)mouse_y / 16.0f);
    
    for (int i = 0; i < r->map->width * r->map->height; i++) {
        bool point_found = false;
        for (int p = 0; p < arrlen(r->map->visible_tiles); p++) {
            if (r->map->visible_tiles[xy_index(x, y)]) {
                point_found = true;
            }
        }
        
        if (point_found) {
            if (r->map->tiles[i] == Floor) {
                TCOD_console_set_default_foreground(r->console, TCOD_cyan);
                TCOD_console_set_default_background(r->console, TCOD_black);
                if (x == mouse_x && y == mouse_y) {
                    TCOD_console_set_default_foreground(r->console, TCOD_magenta);
                }
                TCOD_console_printf(r->console, x, y, ".");
            }
            if (r->map->tiles[i] == Wall) {
                TCOD_console_set_default_foreground(r->console, TCOD_green);
                TCOD_console_set_default_background(r->console, TCOD_black);
                if (x == mouse_x && y == mouse_y) {
                    TCOD_console_set_default_foreground(r->console, TCOD_magenta);
                }
                TCOD_console_printf(r->console, x, y, "#");
            }
        }
        else if (r->map->revealed_tiles[i]) {
            if (r->map->tiles[i] == Floor) {
                TCOD_console_set_default_foreground(r->console, TCOD_gray);
                TCOD_console_set_default_background(r->console, TCOD_black);
                TCOD_console_printf(r->console, x, y, ".");
            }
            if (r->map->tiles[i] == Wall) {
                TCOD_console_set_default_foreground(r->console, TCOD_gray);
                TCOD_console_set_default_background(r->console, TCOD_black);
                TCOD_console_printf(r->console, x, y, "#");
            }
        }

        x += 1;
        if (x > 79) {
            x = 0;
            y += 1;
        }
    }
}

void draw_ui(ecs_iter_t* it) {
    Resources* r = ecs_get_context(it->world);

    CombatStats* player_stats;

    ecs_filter_t f;
    ecs_filter_init(it->world, &f, &(ecs_filter_desc_t) {
        .terms = {
            { ecs_id(CombatStats) },
            { Player }
        }
     });
    ecs_iter_t iter = ecs_filter_iter(it->world, &f);
    while (ecs_filter_next(&iter)) {
        CombatStats* c = ecs_term(&iter, CombatStats, 1);

        for (int i = 0; i < iter.count; i++) {
            player_stats = c;
        }
    }
    ecs_filter_fini(&f);

    // Health bar
    TCOD_console_draw_rect_rgb(r->console, 0, 43, 20, 1, 0, NULL, &TCOD_dark_red, TCOD_BKGND_SET);
    float health_proportion = (((float)player_stats->hp/(float)player_stats->max_hp)*20.0f);
    TCOD_console_draw_rect_rgb(r->console, 0, 43, (int)health_proportion, 1, 0, NULL, &TCOD_dark_green, TCOD_BKGND_SET);

    TCOD_console_set_default_foreground(r->console, TCOD_white);
    TCOD_console_printf_rect(r->console, 0, 43, 20, 1, "HP: %d/%d", player_stats->hp, player_stats->max_hp);

    int y = 43;
    for (int i = arrlen(r->game_log->entries) - 1; i >= 0; i--) {
        if (y < 50) {
            TCOD_console_printf(r->console, 21, y, r->game_log->entries[i]);
        }
        y += 1;
    }

    draw_tooltips(it->world, r);
}

void draw_tooltips(ecs_world_t* world, Resources* resources) {
    int mouse_x;
    int mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    mouse_x = (int)((float)mouse_x / 16.0f);
    mouse_y = (int)((float)mouse_y / 16.0f);

    if (mouse_x >= resources->map->width || mouse_y >= resources->map->height) {
        return;
    }

    char** tooltip = NULL;

    ecs_filter_t f;
    ecs_filter_init(world, &f, &(ecs_filter_desc_t) {
        .terms = {
            { ecs_id(Position) },
            { ecs_id(Name) }
        }
     });
    ecs_iter_t iter = ecs_filter_iter(world, &f);
    while (ecs_filter_next(&iter)) {
        Position* p = ecs_term(&iter, Position, 1);
        Name* n = ecs_term(&iter, Name, 2);

        for (int i = 0; i < iter.count; i++) {
            int idx = xy_index(p[i].x, p[i].y);
            if (p[i].x == mouse_x && p[i].y == mouse_y && resources->map->visible_tiles[idx]) {
                arrpush(tooltip, n[i].name);
            }
        }

        if (arrlen(tooltip) > 0) {
            for (int j = 0; j < arrlen(tooltip); j++)
            {
                char* something = tooltip[j];
                TCOD_console_printf(resources->console, 0, 44, tooltip[j]);
            }
        }
    }
    ecs_filter_fini(&f);
}

void end_draw(ecs_iter_t* it) {
    Resources* r = ecs_get_context(it->world);
    TCOD_context_present(r->context, r->console, NULL);
}
