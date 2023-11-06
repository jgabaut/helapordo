#ifndef FLOORS_H
#define FLOORS_H

#include <stdlib.h>
#include <math.h>
#include "game_utils.h"
#include "game_curses.h"

void init_floor_layout(Floor * floor);
float calc_distance(int x1, int y1, int x2, int y2);
void init_floor_rooms(Floor * floor);
void load_floor_explored(Floor * floor);
void debug_print_roomclass_layout(Floor * floor, FILE * fp);
void display_floor_layout(Floor * floor, WINDOW * win);
void debug_print_floor_layout(Floor * floor, FILE * fp);
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

void floor_random_walk(Floor * floor, int x, int y, int steps,
		       int do_layout_clean);
void floor_set_room_types(Floor * floor);

#endif
