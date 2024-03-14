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

#ifndef TURNOP_H_
#define TURNOP_H_

#include "game_debug.h"
#include "game_fight.h"
#include "saves.h"
#include "bin_saves.h"
#include "../ser/ser_core.h"

/**
 * Call function associated with the passed turnOption_OP.
 * @param op The turnOption_OP to execute.
 * @param args Pointer to turnOP_args object.
 * @param kls The Koliseo used for allocations.
 */
OP_res turnOP(turnOption_OP op, turnOP_args * args, Koliseo * kls,
              Koliseo_Temp * t_kls);
#endif // TURNOP_H_
