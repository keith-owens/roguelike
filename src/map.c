#include "map.h"

int xy_index(int x, int y) {
    return (y * 80) + x;
}

TileType* new_map_test(void) {
    TileType* map = NULL;
    arrsetlen(map, 80*50);
    for (int i = 0; i < 80*50; i++) {
        map[i] = Floor;
    }

    for (int x = 0; x < 80; x++) {
        map[xy_index(x, 0)] = Wall;
        map[xy_index(x, 49)] = Wall;
    }

    for (int y = 0; y < 50; y++) {
        map[xy_index(0, y)] = Wall;
        map[xy_index(79, y)] = Wall;
    }

    for (int j = 0; j < 400; j++) {
        int x = random_in_range(1, 79);
        int y = random_in_range(1, 49);

        int idx = xy_index(x, y);
        if (idx != xy_index(40, 25)) {
            map[idx] = Wall;
        }
    }
    
    return map;
}

MapInfo new_map_rooms_and_corridors(void) {
    TileType* map = NULL;
    arrsetlen(map, 80*50);
    for (int i = 0; i < 80*50; i++) {
        map[i] = Wall;
    }
    Rect* rooms = NULL;
    
    int max_rooms = 30;
    int min_size = 6;
    int max_size = 10;    

    for (int i = 0; i < max_rooms; i++) {
        int w = random_in_range(min_size, max_size);
        int h = random_in_range(min_size, max_size);
        int x = random_in_range(1, (80 - w - 1));
        int y = random_in_range(1, (50 - h - 1));

        Rect new_room = new_rect(x, y, w, h);
        bool ok = true;
        for (int a = 0; a < arrlen(rooms); a++) {
            if (intersect(&new_room, &rooms[a]))
                ok = false;
        }
        if (ok) {
            apply_room_to_map(&new_room, map);
            if (arrlen(rooms) != 0) {
                Point new_point = center(&new_room);
                Point prev_point = center(&rooms[arrlen(rooms) - 1]);
                if (random_in_range(0, 1) == 1) {
                    apply_horizontal_tunnel(map, prev_point.x, new_point.x, prev_point.y);
                    apply_vertical_tunnel(map, prev_point.y, new_point.y, new_point.x);
                } else {
                    apply_vertical_tunnel(map, prev_point.y, new_point.y, prev_point.x);
                    apply_horizontal_tunnel(map, prev_point.x, new_point.x, new_point.y);
                }
            }
            arrpush(rooms, new_room);
        }
    }

    return (MapInfo){ map, rooms };
}

void apply_room_to_map(Rect* room, TileType* map) {
    for (int y = room->y1; y < room->y2; y++) {
        for (int x = room->x1; x < room->x2; x++) {
            map[xy_index(x, y)] = Floor;
        }
    }
}

void apply_horizontal_tunnel(TileType* map, int x1, int x2, int y) {
    for (int x = MIN(x1, x2); x < MAX(x1, x2) + 1; x++) {
        int index = xy_index(x, y);
        if (index > 0 && index < 80*50) {
            map[index] = Floor;
        }
    }
}

void apply_vertical_tunnel(TileType* map, int y1, int y2, int x) {
    for (int y = MIN(y1, y2); y < MAX(y1, y2) + 1; y++) {
        int index = xy_index(x, y);
        if (index > 0 && index < 80*50) {
            map[index] = Floor;
        }
    }
}

void draw_map(ecs_iter_t* it) {
    Resources* r = ecs_get_context(it->world);

    int x = 0;
    int y = 0;

    for (int i = 0; i < 80*50; i++) {
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
