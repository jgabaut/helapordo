// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only

#ifndef GAME_ROOMS_H
#define GAME_ROOMS_H
#include "helapordo.h"

void initRoom(Room * r, Fighter * f, int index, roomClass type, int enemyTotal,
	      loadInfo * load_info, Koliseo_Temp * t_kls);
void initRoom_Home(Room * r, int roomIndex, Fighter * f, loadInfo * load_info,
		   Koliseo_Temp * t_kls);
void initRoom_Enemies(Room * r, int roomIndex, int enemyTotal,
		      loadInfo * load_info, Koliseo_Temp * t_kls);
void initRoom_Shop(Room * r, int roomIndex, Fighter * f, Koliseo_Temp * t_kls);
void initRoom_Boss(Room * r, int roomIndex, Fighter * f, Koliseo_Temp * t_kls);
void initRoom_Treasure(Room * r, int roomIndex, Fighter * f,
		       Koliseo_Temp * t_kls);
void initRoom_Roadfork(Room * r, int roomIndex, Fighter * f,
		       Koliseo_Temp * t_kls);

int handleRoom_Home(Gamestate * gamestate, Room * room, int index, Path * p,
		    Fighter * player, loadInfo * load_info,
		    char fighter_sprites[CLASSESMAX +
					 1][MAXFRAMES][MAXROWS][MAXCOLS],
		    Koliseo * kls, Koliseo_Temp * t_kls);
int handleRoom_Enemies(Gamestate * gamestate, Room * room, int index, Path * p,
		       Fighter * player, loadInfo * load_info,
		       char enemy_sprites[ENEMYCLASSESMAX +
					  1][MAXFRAMES][MAXROWS][MAXCOLS],
		       char fighter_sprites[CLASSESMAX +
					    1][MAXFRAMES][MAXROWS][MAXCOLS],
		       Koliseo * kls, Koliseo_Temp * t_kls);
int handleRoom_Boss(Gamestate * gamestate, Room * room, int index, Path * p,
		    Fighter * player, loadInfo * load_info,
		    char boss_sprites[BOSSCLASSESMAX +
				      1][MAXFRAMES][MAXROWS][MAXCOLS],
		    char fighter_sprites[CLASSESMAX +
					 1][MAXFRAMES][MAXROWS][MAXCOLS],
		    Koliseo * kls, Koliseo_Temp * t_kls);
int handleRoom_Shop(Room * room, int roomsDone, Path * path, Fighter * f,
		    Koliseo * kls, Koliseo_Temp * t_kls);
int handleRoom_Treasure(Room * room, int roomsDone, Path * path, Fighter * f,
			Koliseo * kls, Koliseo_Temp * t_kls);
int handleRoom_Roadfork(Room * room, int *roadFork_value, int roomsDone,
			Path * path, Fighter * f);

#endif
