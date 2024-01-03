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

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#ifdef _WIN32
#else
#endif
#include <locale.h>
#include <sys/stat.h>
#include "game_core.h"
#include "game_utils.h"
#include "sprites.h"
#include "anvil__helapordo.h"
#include "game_lore.h"
#include "game_lore_alt.h"

void gameloop_rl(int argc, char** argv);
#endif
