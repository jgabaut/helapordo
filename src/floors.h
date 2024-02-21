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

#ifndef FLOORS_H
#define FLOORS_H
#include <math.h>
#include "game_utils.h"

void init_floor_layout(Floor * floor);
float calc_distance(int x1, int y1, int x2, int y2);
void init_floor_rooms(Floor * floor);
void floor_random_walk(Floor * floor, int x, int y, int steps,
                       int do_layout_clean);
void floor_set_room_types(Floor * floor);
void load_floor_explored(Floor * floor);
void debug_print_roomclass_layout(Floor * floor, FILE * fp);
void debug_print_floor_layout(Floor * floor, FILE * fp);

#ifdef HELAPORDO_CURSES_BUILD
#include "game_curses.h"

void display_floor_layout(Floor * floor, WINDOW * win);
void display_explored_layout(Floor * floor, WINDOW * win);
void display_roomclass_layout(Floor * floor, WINDOW * win);

void draw_cell(Floor * floor, int cell_x, int cell_y, WINDOW * win,
               int drawcorner_x, int drawcorner_y, int x_size, int y_size,
               int recurse);
void draw_floor_view(Floor * floor, int current_x, int current_y, WINDOW * win);

void move_update(Gamestate * gamestate, Floor * floor, int *current_x,
                 int *current_y, WINDOW * win, Path * path, Fighter * player,
                 Room * room, loadInfo * load_info, Koliseo * kls,
                 Koliseo_Temp * t_kls);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined."
#else
void display_roomclass_layout(Floor *floor, Rectangle *win, float pixelSize);
void display_floor_layout(Floor * floor, Rectangle * win, float pixelSize);
void display_explored_layout(Floor *floor, Rectangle *win, float pixelSize);
#endif // HELAPORDO_RAYLIB_BUILD

#endif // HELAPORDO_CURSES_BUILD

#endif // FLOORS_H
