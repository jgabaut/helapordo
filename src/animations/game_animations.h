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
#ifndef GAME_ANIMATIONS_H_
#define GAME_ANIMATIONS_H_
#include "knight_tapis.h"
#include "mage_spark.h"
#include "archer_drop.h"
#include "assassin_poof.h"

#include "mummy_shuffle.h"
#include "ghost_spell.h"
#include "boar_scream.h"
#include "troll_club.h"
#include "goblin_shoot.h"
#include "zombie_walk.h"
#include "imp_fireball.h"
#include "werewolf_transform.h"

#include "crawlingdude_crawl.h"
#include "srwarthog_square.h"
#include "headlessninja_throw.h"
#include "bluetroll_wonder.h"

#include "enter_door.h"
#include "alt_chest_opening.h"

#include "../core/game_log.h"
#include "../core/game_strings.h"

void hlpd_load_fighter_animations(char fighter_sprites[CLASSESMAX+1][S4C_MAXFRAMES][S4C_MAXROWS][S4C_MAXCOLS]);
void hlpd_load_enemy_animations(char enemy_sprites[ENEMYCLASSESMAX+1][S4C_MAXFRAMES][S4C_MAXROWS][S4C_MAXCOLS]);
void hlpd_load_boss_animations(char boss_sprites[BOSSCLASSESMAX + 1][S4C_MAXFRAMES][S4C_MAXROWS][S4C_MAXCOLS]);
void hlpd_load_animations(char fighter_sprites[CLASSESMAX+1][S4C_MAXFRAMES][S4C_MAXROWS][S4C_MAXCOLS], char enemy_sprites[ENEMYCLASSESMAX+1][S4C_MAXFRAMES][S4C_MAXROWS][S4C_MAXCOLS], char boss_sprites[BOSSCLASSESMAX + 1][S4C_MAXFRAMES][S4C_MAXROWS][S4C_MAXCOLS]);
#endif // GAME_ANIMATIONS_H_
