#ifndef MAP_H
#define MAP_H

#include "external/flecs/flecs.h"
#include "external/stb/stb_ds.h"
#include <libtcod.h>
#include "io.h"
#include "rect.h"
#include "util.h"

typedef enum { Wall, Floor } TileType;

typedef struct MapInfo {
    TileType* map;
    Rect* rooms;
} MapInfo;

int xy_index(int x, int y);
TileType* new_map_test(void);
MapInfo new_map_rooms_and_corridors(void);
void apply_room_to_map(Rect* room, TileType* map);
void apply_horizontal_tunnel(TileType* map, int x1, int x2, int y);
void apply_vertical_tunnel(TileType* map, int y1, int y2, int x);
void draw_map(ecs_iter_t* it);

#endif