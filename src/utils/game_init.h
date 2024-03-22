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

#ifndef GAME_INIT_H_
#define GAME_INIT_H_

#include "game_utils.h"
#include "specials.h"

#ifdef HELAPORDO_CURSES_BUILD
#include "../build-nc/game_curses.h"
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
#include "../build-rl/game_rl.h"
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

void initPerks(Fighter * f, Koliseo * kls);
void initConsumableBag(Fighter * f, Koliseo * kls);

void initCounters(Fighter * f, Koliseo * kls);
void initECounters(Enemy * e, Koliseo_Temp * t_kls);
void initBCounters(Boss * b, Koliseo_Temp * t_kls);
void initFoePartyCounters(FoeParty * fp, Koliseo_Temp * t_kls);

void initBossStats(Boss * b, Koliseo_Temp * t_kls);
void prepareBoss(Boss * b, Koliseo_Temp * t_kls);

void initFoePartyStats(FoeParty * fp, Koliseo_Temp * t_kls);
void prepareFoeParty(FoeParty * fp, int total_foes, int roomindex,
                     Koliseo_Temp * t_kls);

void initEquipSlots(Fighter * f, Koliseo * kls);

void initArtifactsBag(Fighter * f, Koliseo * kls);

void initEquipsBag(Fighter * f, Koliseo * kls);

void initPlayerStats(Fighter * player, Path * path, Koliseo * kls);

void initEnemyStats(Enemy * e, Koliseo_Temp * t_kls);
void setSkills(Fighter *f, Koliseo *kls);
void initChest(Chest * c, Fighter * f, Koliseo_Temp * t_kls);
void initShop(Shop * s, int indexWeight, Fighter * player,
              Koliseo_Temp * t_kls);
void initTreasure(Treasure * t, Fighter * f, Koliseo_Temp * t_kls);
void prepareChest(Chest * c, Fighter * f, Koliseo_Temp * t_kls);
void prepareRoomEnemy(Enemy * e, int roomindex, int enemiesInRoom,
                      int enemyindex, Koliseo_Temp * t_kls);

void prepareTreasure(Treasure * t, Fighter * f, Koliseo_Temp * t_kls);

void prepareRoadfork(Roadfork * r);
void setEnemySkills(Enemy *e, Koliseo_Temp *t_kls);
void setBossSkills(Boss *b, Koliseo_Temp *t_kls);
#endif // GAME_INIT_H_
