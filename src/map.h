#ifndef MAP_H
#define MAP_H

#include "components.h"
#include "external/flecs/flecs.h"
#include "external/stb/stb_ds.h"
#include <libtcod.h>
#include "rect.h"
#include "util.h"

typedef enum { Wall, Floor } TileType;

typedef struct Map {
    TileType* tiles;
    Rect* rooms;
    int width;
    int height;
    bool* revealed_tiles;
    bool* visible_tiles;
} Map;

int xy_index(int x, int y);
TileType* new_map_test(void);
Map new_map_rooms_and_corridors(void);
void apply_room_to_map(Rect* room, TileType* tiles);
void apply_horizontal_tunnel(TileType* tiles, int x1, int x2, int y);
void apply_vertical_tunnel(TileType* tiles, int y1, int y2, int x);

#endif