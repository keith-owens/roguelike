#include <SDL.h>
#include <libtcod.h>
#include "external/flecs/flecs.h"

int main(int argc, char* argv[]) {
    TCOD_Console* console = TCOD_console_new(80, 25);

    TCOD_ContextParams params = {
        .tcod_version = TCOD_COMPILEDVERSION,
        .console = console,
        .window_title = "Libtcod Project",
        .sdl_window_flags = SDL_WINDOW_RESIZABLE,
        .vsync = true,
        .argc = argc,
        .argv = argv
    };

    TCOD_Context* context;
    TCOD_context_new(&params, &context);

    while (1) {
        TCOD_console_clear(console);
        TCOD_console_printf(console, 0, 0, "Hello World");
        TCOD_context_present(context, console, NULL);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                return 0;
            }
        }
    }
}