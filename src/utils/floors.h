// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2022-2026 jgabaut

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
#include "../core/game_log.h"
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
void debug_print_floor_visible_layout(Floor * floor, FILE * fp);

extern char roomclass_chars[ROOM_CLASS_MAX+1];
extern int roomclass_colors[ROOM_CLASS_MAX+1];
int room_color(Floor* floor, int cell_x, int cell_y);
char room_char(Floor* floor, int cell_x, int cell_y);

bool blocks_vision(const Floor *floor, int x, int y);
void cast_light(Floor *floor, int cx, int cy, int row, float start_slope, float end_slope, int radius, int xx, int xy, int yx, int yy);
void floor_calculate_fov(Floor *floor, int player_x, int player_y, int radius);

#ifdef HELAPORDO_CURSES_BUILD
#include "../build-nc/game_curses.h"

void display_roomclass_layout(Floor * floor, WINDOW * win);
void display_floor_layout(Floor * floor, WINDOW * win);
void display_explored_layout(Floor * floor, WINDOW * win);

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
void display_floor_layout_with_player(Floor * floor, Rectangle * win, int current_x, int current_y, float pixelSize);
void display_explored_layout(Floor *floor, Rectangle *win, float pixelSize);
void display_explored_layout_with_player(Floor *floor, Rectangle *win, int current_x, int current_y, float pixelSize);
void draw_floor_view(Floor * floor, int current_x, int current_y, float pixelSize, Rectangle * win);
void step_floor(Floor * floor, int *current_x,
                int *current_y, int control);
#endif // HELAPORDO_RAYLIB_BUILD

#endif // HELAPORDO_CURSES_BUILD

#endif // FLOORS_H
