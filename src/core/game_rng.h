// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2022-2026 jgabaut

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

#ifndef GAME_RNG_H
#define GAME_RNG_H
#include <stdbool.h>
#include "game_core.h"
#include "game_log.h"

int hlpd_rand_docount(bool count);
int hlpd_rand(void);
unsigned long hlpd_hash(unsigned char *str);
void gen_random_seed(char buffer[PATH_SEED_BUFSIZE+1]);
bool check_seed(char buffer[PATH_SEED_BUFSIZE]);
#endif // GAME_RNG_H
