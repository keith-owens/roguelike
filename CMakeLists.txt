cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_C_STANDARD 11)
project(roguelike-tutorial VERSION 0.1.0)

find_package(SDL2 REQUIRED)
INCLUDE_DIRECTORIES(
    C:/src/vcpkg/installed/x86-windows/include
)

LINK_DIRECTORIES(
    C:/src/vcpkg/installed/x86-windows/lib
)

add_executable(roguelike-tutorial
    src/external/flecs/flecs.c
    src/external/klib/kstring.c
    src/external/stb/stb_ds.c
    src/components.c
    src/damage_system.c
    src/inventory_system.c
    src/io.c
    src/main.c
    src/map.c
    src/map_indexing_system.c
    src/melee_combat_system.c
    src/monster_ai_system.c
    src/player.c
    src/potion_use_system.c
    src/rect.c
    src/spawner.c
    src/util.c
    src/visibility_system.c
)

target_link_libraries(roguelike-tutorial
SDL2::SDL2 libtcod SDL2::SDL2main
)