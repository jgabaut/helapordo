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
#ifndef GAME_ANIMATIONS_H_
#define GAME_ANIMATIONS_H_
#include "../animations/knight_tapis.h"
#include "../animations/mage_spark.h"
#include "../animations/archer_drop.h"
#include "../animations/assassin_poof.h"

#include "../animations/mummy_shuffle.h"
#include "../animations/ghost_spell.h"
#include "../animations/boar_scream.h"
#include "../animations/troll_club.h"
#include "../animations/goblin_shoot.h"
#include "../animations/zombie_walk.h"
#include "../animations/imp_fireball.h"
#include "../animations/werewolf_transform.h"

#include "../animations/crawlingdude_crawl.h"
#include "../animations/srwarthog_square.h"
#include "../animations/headlessninja_throw.h"
#include "../animations/bluetroll_wonder.h"

#include "../animations/enter_door.h"
#include "../animations/alt_chest_opening.h"

#include "../utils/game_utils.h"

void hlpd_load_fighter_animations(char fighter_sprites[CLASSESMAX+1][MAXFRAMES][MAXROWS][MAXCOLS]);
void hlpd_load_enemy_animations(char enemy_sprites[ENEMYCLASSESMAX+1][MAXFRAMES][MAXROWS][MAXCOLS]);
void hlpd_load_boss_animations(char boss_sprites[BOSSCLASSESMAX + 1][MAXFRAMES][MAXROWS][MAXCOLS]);
void hlpd_load_animations(char fighter_sprites[CLASSESMAX+1][MAXFRAMES][MAXROWS][MAXCOLS], char enemy_sprites[ENEMYCLASSESMAX+1][MAXFRAMES][MAXROWS][MAXCOLS], char boss_sprites[BOSSCLASSESMAX + 1][MAXFRAMES][MAXROWS][MAXCOLS]);
#endif // GAME_ANIMATIONS_H_
