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
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) //We need C11
#define HELAPORDO_H

#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#ifdef HELAPORDO_CURSES_BUILD
#ifdef _WIN32
#include <ncursesw/panel.h>
#include <ncursesw/menu.h>
#include "floor_tester.h"
#else
#include <panel.h>
#include <menu.h>
#include <sys/utsname.h>
#endif // _WIN32
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined"
#else
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

#include <locale.h>
#include <sys/stat.h>
#include "game_core.h"
#include "game_utils.h"
#include "rooms.h"
#include "specials.h"
#include "artifacts.h"

#ifdef HELAPORDO_CURSES_BUILD
#include "game_curses.h"
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined"
#else
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

#include "sprites.h"
#include "floors.h"
#include "anvil__helapordo.h"
#include "game_lore.h"
#include "game_lore_alt.h"

/*! \mainpage Helapordo index page
 *
 * \section intro_sec Intro
 *
 * Helapordo is roguelike terminal game, using ncurses.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Check it out on [github](https://github.com/jgabaut/helapordo).
 */

/**
 * Call function associated with the passed turnOption_OP.
 * @param op The turnOption_OP to execute.
 * @param args Pointer to turnOP_args object.
 * @param kls The Koliseo used for allocations.
 */
OP_res turnOP(turnOption_OP op, turnOP_args * args, Koliseo * kls,
              Koliseo_Temp * t_kls);

#ifdef HELAPORDO_CURSES_BUILD
fightResult do_Skill(Fighter * player, Enemy * e, skillType picked_skill, WINDOW * notify_win, Koliseo * kls);
fightResult do_Skill_boss(Fighter * player, Boss * b, skillType picked_skill, Path * path, WINDOW * notify_win, Koliseo * kls);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
fightResult do_Skill(Fighter * player, Enemy * e, skillType picked_skill, Rectangle * notification_area, Koliseo * kls);
fightResult do_Skill_boss(Fighter * player, Boss * b, skillType picked_skill, Path * path, Rectangle * notification_area, Koliseo * kls);
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

/*
void register_counter_callback(int index, callback_void_t ptr, Fighter*);
*/

void resetFighterStatus(Fighter * f);

void resetEnemyStatus(Enemy * e);

void resetBossStatus(Boss * b);

effect_fun getStatusCounterFun(fighterStatus status);

effect_e_fun getStatusCounterEnemyFun(fighterStatus status);

effect_b_fun getStatusCounterBossFun(fighterStatus status);

effect_fp_fun get_StatusCounter_FoeParty_Fun(fighterStatus status);

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

boost_fun getStatBoostCounterFun(Stat s);
boost_e_fun getStatBoostCounterEnemyFun(Stat s);
boost_b_fun getStatBoostCounterBossFun(Stat s);
boost_fp_fun get_StatBoostCounter_FoeParty_Fun(Stat s);

void initPerks(Fighter * f, Koliseo * kls);
void applyEquipPerks(Equip * e, Fighter * f);
void removeEquipPerks(Equip * e, Fighter * f);
void printActivePerks(Fighter * f);

void initCounters(Fighter * f, Koliseo * kls);

void initECounters(Enemy * e, Koliseo_Temp * t_kls);

void initBCounters(Boss * b, Koliseo_Temp * t_kls);

void initFoePartyCounters(FoeParty * fp, Koliseo_Temp * t_kls);

void printCounters(Turncounter * counters[]);

void updateCounters(Turncounter * counters[], int isEnemy, Fighter * f,
                    Enemy * e);
void updateCounters_Boss(Turncounter * counters[], int isBoss, Fighter * f,
                         Boss * b);
void setCounter(Turncounter * c, int turns);

void setSpecials(Fighter * f, Koliseo * kls);

void setSkills(Fighter *f, Koliseo *kls);
void setEnemySkills(Enemy *e, Koliseo_Temp *t_kls);
void setBossSkills(Boss *b, Koliseo_Temp *t_kls);

void resetPermboosts(Fighter * f);
void applyPermboosts(Fighter * f);

void resetArtifactsState(Fighter * f);
void applyArtifacts(Fighter * f, Enemy * e, Boss * b, int isBoss);

void initEquipSlots(Fighter * f, Koliseo * kls);

void initConsumableBag(Fighter * f, Koliseo * kls);
void initArtifactsBag(Fighter * f, Koliseo * kls);

void initWincon(Wincon * w, Path * p, winconClass class);

void initPlayerStats(Fighter * player, Path * path, Koliseo * kls);

void initEnemyStats(Enemy * e, Koliseo_Temp * t_kls);

int getBossBoost(int lvl, bossClass bclass);

void statResetBoss(Boss * b, int force);

void initBossStats(Boss * b, Koliseo_Temp * t_kls);
void prepareBoss(Boss * b, Koliseo_Temp * t_kls);

void initFoePartyStats(FoeParty * fp, Koliseo_Temp * t_kls);
void prepareFoeParty(FoeParty * fp, int total_foes, int roomindex,
                     Koliseo_Temp * t_kls);

int getEnemyBoost(int lvl, enemyClass eclass);

void statResetEnemy(Enemy * e, int force);

void prepareRoomEnemy(Enemy * e, int roomindex, int enemiesInRoom,
                      int enemyindex, Koliseo_Temp * t_kls);

void setEquipPrices(int size, int *equipPrices, Equip * equips[]);
void setConsumablePrices(int size, int *consumablePrices,
                         Consumable ** consumables);
void initShop(Shop * s, int indexWeight, Fighter * player,
              Koliseo_Temp * t_kls);
void initChest(Chest * c, Fighter * f, Koliseo_Temp * t_kls);
void prepareChest(Chest * c, Fighter * f, Koliseo_Temp * t_kls);

void initTreasure(Treasure * t, Fighter * f, Koliseo_Temp * t_kls);
void prepareTreasure(Treasure * t, Fighter * f, Koliseo_Temp * t_kls);

void prepareRoadfork(Roadfork * r);

void printStats(Fighter * f);

void printEStats(Enemy * e);

int getEnemyXpGain(Enemy * e);

int getBossXpGain(Boss * b);

void printConsumablesStats(Consumable * c);

void printArtifactStats(Artifact * a);

void printQualityColor(quality q);

void printEquipStats(Equip * e);

void printSpawnMessage(Enemy * e, int roomIndex, int enemyIndex);

void getParams(int argc, char **argv, Fighter * player, Path * path, int optTot,
               Koliseo * kls);

turnOption getTurnChoice(char *ch);

foeTurnOption enemyTurnPick(Enemy * e, Fighter * f);

foeTurnOption bossTurnPick(Boss * b, Fighter * f);

int getBoost(int lvl, int luck);

void unlockSpecial(Fighter * f);

void onLevelUp(Fighter * player);

void checkremainder(Fighter * player, int xp);

void giveXp(Fighter * player, Enemy * e);
void giveXp_Boss(Fighter * player, Boss * b);

void statReset(Fighter * player, int force);

int dropConsumable(Fighter * player);
int dropArtifact(Fighter * player);

#ifdef HELAPORDO_CURSES_BUILD
void dropEquip(Fighter * player, int beast, WINDOW * notify_win, Koliseo * kls);

int defer_fight_enemy(Fighter * player, Enemy * e, foeTurnOption_OP foe_op,
                      WINDOW * notify_win, Koliseo * kls);
int defer_skill_enemy(Fighter *player, Enemy *e, skillType picked_skill, foeTurnOption_OP foe_op,
                      WINDOW * notify_win, Koliseo * kls);

int fight(Fighter * player, Enemy * e, WINDOW * notify_win, Koliseo * kls);

int enemy_attack(Enemy * e, Fighter * target, WINDOW * notify_win,
                 Koliseo * kls);

int defer_fight_boss(Fighter * player, Boss * b, Path * p,
                     foeTurnOption_OP foe_op, WINDOW * notify_win,
                     Koliseo * kls);

int defer_skill_boss(Fighter *player, Boss *b, skillType picked_skill, Path *p, foeTurnOption_OP foe_op,
                     WINDOW *notify_win, Koliseo *kls);

int boss_fight(Fighter * player, Boss * b, Path * p, WINDOW * notify_win,
               Koliseo * kls);

int boss_attack(Boss * b, Fighter * target, Path * p, WINDOW * notify_win,
                Koliseo * kls);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
void dropEquip(Fighter * player, int beast, Rectangle * notification_area, Koliseo * kls);

int defer_fight_enemy(Fighter * player, Enemy * e, foeTurnOption_OP foe_op,
                      Rectangle * notification_area, Koliseo * kls);
int defer_skill_enemy(Fighter *player, Enemy *e, skillType picked_skill, foeTurnOption_OP foe_op,
                      Rectangle * notification_area, Koliseo * kls);

int fight(Fighter * player, Enemy * e, Rectangle * notification_area, Koliseo * kls);

int enemy_attack(Enemy * e, Fighter * target, Rectangle * notification_area,
                 Koliseo * kls);

int defer_fight_boss(Fighter * player, Boss * b, Path * p,
                     foeTurnOption_OP foe_op, Rectangle * notification_area,
                     Koliseo * kls);

int defer_skill_boss(Fighter *player, Boss *b, skillType picked_skill, Path *p, foeTurnOption_OP foe_op,
                     Rectangle * notification_area, Koliseo *kls);

int boss_fight(Fighter * player, Boss * b, Path * p, Rectangle * notification_area,
               Koliseo * kls);

int boss_attack(Boss * b, Fighter * target, Path * p, Rectangle * notification_area,
                Koliseo * kls);
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

void useConsumable(Fighter * f, Enemy * e, Boss * b, char *string, int isBoss);

int getConsumableQty(Fighter * f, int n);

void emptyConsumables(Fighter * player);

void emptyArtifacts(Fighter * player);

void emptyEquips(Fighter * player);

OP_res handleSave_Enemies(FILE * file, Fighter * f, Path * p, Enemy * e,
                          int enemyIndex, int roomTotalEnemies, int roomIndex);
OP_res handleSave_Home(FILE * file, Fighter * f, Path * p, int roomIndex);

OP_res handleLoadgame_Enemies(FILE * file, Fighter * f, Path * p, Enemy * e,
                              int *enemyIndex, int *roomTotalEnemies,
                              int *roomIndex, int *total_foes,
                              int *done_loading, Koliseo * kls);
OP_res handleLoadgame_Home(FILE * file, Fighter * f, Path * p, int *roomIndex,
                           int *done_loading, Koliseo * kls);
saveType read_saveType(FILE * file);

void death(Fighter * player, loadInfo * load_info);

void e_death(Enemy * e);

void b_death(Boss * b);

int retry(void);

void debug_generic(Gamestate * gmst, Fighter * player, Path * p, int roomIndex,
                   Koliseo * kls, Koliseo_Temp * t_kls);
void debug_enemies_room(Gamestate * gmst, Room * room, Fighter * player,
                        Enemy * e, Path * p, int roomIndex, int currentEnemyNum,
                        Koliseo * kls, Koliseo_Temp * t_kls);

void quit(Fighter * p, Room * room, loadInfo * load_info, Koliseo_Temp * t_kls);

void sell_all_equips(Fighter * f, Koliseo_Temp * t_kls);

#ifdef HELAPORDO_CURSES_BUILD
void open_chest(WINDOW * w, Chest * c, Fighter * f, Koliseo * kls,
                Koliseo_Temp * t_kls);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
void open_chest(Rectangle * notification_area, Chest * c, Fighter * f, Koliseo * kls,
                Koliseo_Temp * t_kls);
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

Path *randomise_path(int seed, Koliseo * kls, const char *path_to_savefile);

void gameloop(int argc, char **argv);

#ifdef _WIN32
void gameloop_Win(int argc, char **argv);
#endif

#else
#error "This code requires C11.\n    _Alignof\n"
#endif
#endif //HELAPORDO_H
