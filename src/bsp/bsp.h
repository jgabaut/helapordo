//
// Directly inspired -basically stolen- from bsp-dungeon-generator by Matúš Ďurica.
//
// Reference repo: https://github.com/mtsdurica/bsp-dungeon-generator
// Original license: MIT.
// The author of the original code gave permission to re-license it to ship helapordo with just one license.
//
// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2022-2024 jgabaut

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef BSP_H_
#define BSP_H_

#include "../utils/floors.h"

typedef struct BSP_Wall {
    int start_x;
    int start_y;
    int end_x;
    int end_y;
} BSP_Wall;

#define BSP_Wall_Fmt "st_x: %i, st_y: %i, e_x: %i, e_y: %i"

#define BSP_Wall_Arg(wall) (wall).start_x, (wall).start_y, (wall).end_x, (wall).end_y

enum BSP_Wall_Index {
    WALL_TOP,
    WALL_RIGHT,
    WALL_BOTTOM,
    WALL_LEFT,
};

#define BSP_WALL_INDEX_MAX WALL_LEFT

extern const char* BSP_Wall_Index_strings[BSP_WALL_INDEX_MAX+1];

const char* stringFromBSP_Wall_Index(int i);

struct BSP_Room;

typedef struct BSP_Room {
    BSP_Wall walls[BSP_WALL_INDEX_MAX+1];
    struct BSP_Room* child_left; // Left/top
    struct BSP_Room* child_right; // Right/bottom
    int center_x;
    int center_y;
} BSP_Room;

#define BSP_ROOM_SIZE 4

BSP_Room* floor_bsp_gen(Floor* floor, Koliseo_Temp* t_kls, int base_x, int base_y);

void dbg_BSP_Room(BSP_Room* bsp_room);

#endif
