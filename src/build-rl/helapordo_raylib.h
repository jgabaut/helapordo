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
#define HELAPORDO_H

#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif

#include "game_rl.h"
#ifdef ANVIL_BUILD
#include "../anvil__helapordo.h"
#endif // ANVIL_BUILD
#include "../core/game_lore.h"
void gameloop_rl(int argc, char** argv);
#endif
