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

#ifndef GAMECURSES_RL_H
#define GAMECURSES_RL_H

#include "../utils/game_utils.h"
#include "../core/sprites.h"
#include "../core/game_animations.h"

#define EXPECTED_RAYLIB_VERSION_MAJOR 4 /**< Defines min expected major ncurses version.*/
#define EXPECTED_RAYLIB_VERSION_MINOR 5 /**< Defines min expected minor ncurses version.*/
#define EXPECTED_RAYLIB_VERSION_PATCH 0	/**< Defines min expected patch ncurses version.*/

void handleTutorial(S4C_Color* palette);
void setEnemySprite(Enemy * e);
void setEquipSprite(Equip * e);
void setEquipslotSprite(Equipslot * s);
void setConsumableSprite(Consumable * c);
void setBossSprite(Boss * b);
void setFighterSprite(Fighter * f);
void setChestSprite(Chest * c);
#endif // GAMECURSES_RL_H
