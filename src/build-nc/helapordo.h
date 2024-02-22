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

#ifndef HELAPORDO_H
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) //We need C11
#define HELAPORDO_H

#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif // _WIN32

#include "../utils/rooms.h"
#include "../anvil__helapordo.h"
#include "../core/game_lore.h"

/*
void register_counter_callback(int index, callback_void_t ptr, Fighter*);
*/

void gameloop(int argc, char **argv);


#else
#error "This code requires C11.\n    _Alignof\n"
#endif
#endif //HELAPORDO_H
