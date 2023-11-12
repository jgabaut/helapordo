// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2023  jgabaut

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

#ifndef SPRITES_H
#define SPRITES_H

#include "game_core.h"

extern char bosses_sprites[BOSSCLASSESMAX + 1][9][15];
extern char consumables_sprites[CONSUMABLESMAX + 1][9][14];
extern char equips_sprites[EQUIPSMAX + 1][9][15];
extern char equipzones_sprites[EQUIPZONES + 1][9][15];
extern char artifacts_sprites[ARTIFACTSMAX + 1][9][15];
extern char enemies_sprites[ENEMYCLASSESMAX + 1][9][15];

#define MISC_SPRITES_MAX 1
extern char misc_sprites[MISC_SPRITES_MAX + 1][9][15];

#endif
