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

#ifndef SAVES_H_
#define SAVES_H_

#include "equips.h"
#include "game_init.h"

#ifdef HELAPORDO_CURSES_BUILD
#include "game_curses.h"
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
#include "game_rl.h"
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

OP_res handleLoadgame_Home(FILE * file, Fighter * f, Path * p, int *roomIndex,
                           int *done_loading, Koliseo * kls);
OP_res handleLoadgame_Enemies(FILE * file, Fighter * f, Path * p, Enemy * e,
                              int *enemyIndex, int *roomTotalEnemies,
                              int *roomIndex, int *total_foes,
                              int *done_loading, Koliseo * kls);
OP_res handleSave_Enemies(FILE * file, Fighter * f, Path * p, Enemy * e,
                          int enemyIndex, int roomTotalEnemies, int roomIndex);
OP_res handleSave_Home(FILE * file, Fighter * f, Path * p, int roomIndex);
saveType read_saveType(FILE * file);
#endif // SAVES_H_
