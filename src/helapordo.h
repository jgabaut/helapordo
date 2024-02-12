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

#ifndef HELAPORDO_H
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) //We need C11
#define HELAPORDO_H

#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif

#include "rooms.h"
#include "anvil__helapordo.h"
#include "game_lore.h"

/*! \mainpage Helapordo index page
 *
 * \section intro_sec Intro
 *
 * Helapordo is roguelike terminal game, using ncurses.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Check it out on [github](https://github.com/jgabaut/helapordo).
 */

/*
void register_counter_callback(int index, callback_void_t ptr, Fighter*);
*/

void printStats(Fighter * f);

void printEStats(Enemy * e);

void printConsumablesStats(Consumable * c);

void printArtifactStats(Artifact * a);

void getParams(int argc, char **argv, Fighter * player, Path * path, int optTot,
               Koliseo * kls);

int getConsumableQty(Fighter * f, int n);

void emptyConsumables(Fighter * player);

void emptyArtifacts(Fighter * player);

void emptyEquips(Fighter * player);

int retry(void);

#ifdef HELAPORDO_CURSES_BUILD
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

Path *randomise_path(int seed, Koliseo * kls, const char *path_to_savefile);

void gameloop(int argc, char **argv);

#ifdef _WIN32
void gameloop_Win(int argc, char **argv);
#endif

#else
#error "This code requires C11.\n    _Alignof\n"
#endif
#endif //HELAPORDO_H
