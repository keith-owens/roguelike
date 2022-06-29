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
    src/external/stb/stb_ds.c
    src/main.c
)

target_link_libraries(roguelike-tutorial
SDL2::SDL2 libtcod SDL2::SDL2main
)