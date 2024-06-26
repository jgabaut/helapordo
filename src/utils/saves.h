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

#include "../core/equips.h"
#include "game_init.h"

#ifdef HELAPORDO_CURSES_BUILD
#include "../build-nc/game_curses.h"
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
#include "../build-rl/game_rl.h"
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
bool set_Saveslot_name(Saveslot * sv);
#endif // SAVES_H_
