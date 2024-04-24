// jgabaut @ github.com/jgabaut
//
// Directly inspired -basically stolen- from bsp-dungeon-generator by Matúš Ďurica.
//
// Reference repo: https://github.com/mtsdurica/bsp-dungeon-generator
// Original license: MIT.
// You can find a copy of the original license in the same directory as this header.

#ifndef BSP_H_
#define BSP_H_

#include "../utils/floors.h"

typedef struct BSP_Wall {
    int start_x;
    int start_y;
    int end_x;
    int end_y;
} BSP_Wall;

enum BSP_Wall_Index {
    WALL_TOP,
    WALL_RIGHT,
    WALL_BOTTOM,
    WALL_LEFT,
};

#define BSP_WALL_INDEX_MAX WALL_LEFT

struct BSP_Room;

typedef struct BSP_Room {
    BSP_Wall walls[BSP_WALL_INDEX_MAX+1];
    struct BSP_Room* child_left; // Left/top
    struct BSP_Room* child_right; // Right/bottom
    int center_x;
    int center_y;
} BSP_Room;

#define BSP_ROOM_SIZE 4

void floor_bsp_gen(Floor* floor, int x, int y);

#endif
