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

#ifndef GAME_INIT_H_
#define GAME_INIT_H_

#include "sprites.h"
#include "specials.h"
#include "game_rng.h"
#include "game_fight.h"

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
void setEquipPrices(int size, int *equipPrices, Equip * equips[]);
void setConsumablePrices(int size, int *consumablePrices, Consumable ** consumables);
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

effect_fun getStatusCounterFun(fighterStatus status);

effect_e_fun getStatusCounterEnemyFun(fighterStatus status);

effect_b_fun getStatusCounterBossFun(fighterStatus status);

effect_fp_fun get_StatusCounter_FoeParty_Fun(fighterStatus status);

boost_fun getStatBoostCounterFun(Stat s);
boost_e_fun getStatBoostCounterEnemyFun(Stat s);
boost_b_fun getStatBoostCounterBossFun(Stat s);
boost_fp_fun get_StatBoostCounter_FoeParty_Fun(Stat s);

void set_turnboost_atk(Fighter * f, int boost);
void set_turnboost_def(Fighter * f, int boost);
void set_turnboost_vel(Fighter * f, int boost);
void set_turnboost_enr(Fighter * f, int boost);

void set_enemy_turnboost_atk(Enemy * e, int boost);
void set_enemy_turnboost_def(Enemy * e, int boost);
void set_enemy_turnboost_vel(Enemy * e, int boost);
void set_enemy_turnboost_enr(Enemy * e, int boost);

void set_boss_turnboost_atk(Boss * b, int boost);
void set_boss_turnboost_def(Boss * b, int boost);
void set_boss_turnboost_vel(Boss * b, int boost);
void set_boss_turnboost_enr(Boss * b, int boost);

void set_foeparty_turnboost_atk(FoeParty * fp, int boost);
void set_foeparty_turnboost_def(FoeParty * fp, int boost);
void set_foeparty_turnboost_vel(FoeParty * fp, int boost);
void set_foeparty_turnboost_enr(FoeParty * fp, int boost);

void resetFighterStatus(Fighter * f);
void resetEnemyStatus(Enemy * e);
void resetBossStatus(Boss * b);

int costFromSpecial(fighterClass class, specialMove move);
int costFromSkill(skillType class);

#endif // GAME_INIT_H_
