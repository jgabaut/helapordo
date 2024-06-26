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

#ifndef GAME_ROOMS_H_
#define GAME_ROOMS_H_
#include "artifacts.h"
#include "turn_op.h"

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

#ifdef HELAPORDO_CURSES_BUILD
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
                       Koliseo * kls, Koliseo_Temp * t_kls, RingaBuf* rb_notifications);
int handleRoom_Boss(Gamestate * gamestate, Room * room, int index, Path * p,
                    Fighter * player, loadInfo * load_info,
                    char boss_sprites[BOSSCLASSESMAX +
                                      1][MAXFRAMES][MAXROWS][MAXCOLS],
                    char fighter_sprites[CLASSESMAX +
                            1][MAXFRAMES][MAXROWS][MAXCOLS],
                    Koliseo * kls, Koliseo_Temp * t_kls, RingaBuf* rb_notifications);
int handleRoom_Shop(Room * room, int roomsDone, Path * path, Fighter * f,
                    Koliseo * kls, Koliseo_Temp * t_kls);
int handleRoom_Treasure(Room * room, int roomsDone, Path * path, Fighter * f,
                        Koliseo * kls, Koliseo_Temp * t_kls);
int handleRoom_Roadfork(Room * room, int *roadFork_value, int roomsDone,
                        Path * path, Fighter * f);

void open_chest(WINDOW * w, Chest * c, Fighter * f, Koliseo * kls,
                Koliseo_Temp * t_kls);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
void open_chest(Rectangle * notification_area, Chest * c, Fighter * f, Koliseo * kls,
                Koliseo_Temp * t_kls);
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

#endif // GAME_ROOMS_H_
