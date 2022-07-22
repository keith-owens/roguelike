#include "map.h"

int xy_index(int x, int y) {
    return (y * 80) + x;
}

Map new_map_rooms_and_corridors(void) {
    int map_width = 80;
    int map_height = 50;

    TileType* tiles = NULL;
    arrsetlen(tiles, map_width * map_height);
    for (int i = 0; i < map_width * map_height; i++) {
        tiles[i] = Wall;
    }
    Rect* rooms = NULL;
    
    int max_rooms = 30;
    int min_size = 6;
    int max_size = 10;    

    for (int i = 0; i < max_rooms; i++) {
        int w = random_in_range(min_size, max_size);
        int h = random_in_range(min_size, max_size);
        int x = random_in_range(1, (map_width - w - 1));
        int y = random_in_range(1, (map_height - h - 1));

        Rect new_room = new_rect(x, y, w, h);
        bool ok = true;
        for (int a = 0; a < arrlen(rooms); a++) {
            if (intersect(&new_room, &rooms[a]))
                ok = false;
        }
        if (ok) {
            apply_room_to_map(&new_room, tiles);
            if (arrlen(rooms) != 0) {
                Point new_point = center(&new_room);
                Point prev_point = center(&rooms[arrlen(rooms) - 1]);
                if (random_in_range(0, 1) == 1) {
                    apply_horizontal_tunnel(tiles, prev_point.x, new_point.x, prev_point.y);
                    apply_vertical_tunnel(tiles, prev_point.y, new_point.y, new_point.x);
                } else {
                    apply_vertical_tunnel(tiles, prev_point.y, new_point.y, prev_point.x);
                    apply_horizontal_tunnel(tiles, prev_point.x, new_point.x, new_point.y);
                }
            }
            arrpush(rooms, new_room);
        }
    }

    bool* revealed = NULL;
    arrsetlen(revealed, map_width * map_height);
    for (int j = 0; j < arrlen(revealed); j++) {
        revealed[j] = false;
    }

    bool* visible = NULL;
    arrsetlen(visible, map_width * map_height);
    for (int k = 0; k < arrlen(visible); k++) {
        visible[k] = false;
    }

    return (Map){ tiles, rooms, map_width, map_height, revealed, visible };
}

void apply_room_to_map(Rect* room, TileType* tiles) {
    for (int y = room->y1; y < room->y2; y++) {
        for (int x = room->x1; x < room->x2; x++) {
            tiles[xy_index(x, y)] = Floor;
        }
    }
}

void apply_horizontal_tunnel(TileType* tiles, int x1, int x2, int y) {
    for (int x = MIN(x1, x2); x < MAX(x1, x2) + 1; x++) {
        int index = xy_index(x, y);
        if (index > 0 && index < 80*50) {
            tiles[index] = Floor;
        }
    }
}

void apply_vertical_tunnel(TileType* tiles, int y1, int y2, int x) {
    for (int y = MIN(y1, y2); y < MAX(y1, y2) + 1; y++) {
        int index = xy_index(x, y);
        if (index > 0 && index < 80*50) {
            tiles[index] = Floor;
        }
    }
}
