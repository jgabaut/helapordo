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

#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L // Needed for getopt
#endif // _WIN32

#ifdef HELAPORDO_CURSES_BUILD
#include "../core/sprites.h"
#ifdef _WIN32
#include <ncursesw/panel.h>
#include <ncursesw/menu.h>
#else
#include <menu.h>
#include <panel.h>
#endif // _WIN32
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined."
#else

#endif // HELAPORDO_RAYLIB_BUILD

#endif // HELAPORDO_CURSES_BUILD

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#ifdef _WIN32 // From https://stackoverflow.com/a/230068
#include <io.h>
#define F_OK 0
#define access _access
#endif

#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <inttypes.h>
#include <signal.h>
#include "../core/game_core.h"
#ifdef HELAPORDO_CURSES_BUILD
#endif // HELAPORDO_CURSES_BUILD
#ifdef ANVIL_BUILD
#include "../anvil__helapordo.h"
#endif // ANVIL_BUILD

#include "../release_data/release_data.h"

void* s4c_gui_malloc(size_t size);
void* s4c_gui_calloc(size_t count, size_t size);
void hlpd_sigint_handler(int signum);
void initWincon(Wincon * w, Path * p, winconClass class);
void printGlobVars(void);

#ifdef _WIN32
void printWin_EnvVars(void);
void log_Win_EnvVars(void);
#endif

void red(void);
void lightRed(void);
void strongWhite(void);
void white(void);
void green(void);
void lightGreen(void);
void yellow(void);
void lightYellow(void);
void blue(void);
void lightBlue(void);
void purple(void);
void lightPurple(void);
void cyan(void);
void lightCyan(void);

void init_game_color_pairs(void);
void resolve_staticPath(char static_path[500]);

void dbg_print_floor_layout(Floor * floor);
void dbg_print_explored_layout(Floor * floor);
void dbg_print_roomclass_layout(Floor * floor);
void dbg_Gamestate(Gamestate * gmst);
void dbg_GameScreen(GameScreen * scr);
void dbg_GameOptions(GameOptions * options);
void dbg_Fighter(Fighter * fighter);
void dbg_countStats(countStats * stats);
void dbg_Wincon(Wincon * wc);
void dbg_Path(Path * path);
void dbg_Saveslot(Saveslot * saveslot);

void update_Gamestate(Gamestate * gmst, int current_fighters,
                      roomClass current_roomtype, int current_room_index,
                      int current_enemy_index, Floor * current_floor, Room* current_room, GameOptions* game_options);

void update_Equipslots(Fighter* f);

void loadLore(char **lore_strings, int loreKind);

void setRoomType(Path * path, int *roadFork_value, roomClass * room_type,
                 int roomsDone);
void freeRoom(Room * room);

void printTitle(void);

void printVersion(void);
void printFormattedVersion(const char *progname);
void hlpd_dbg_features(void);

void usage(const char *progname);
void log_tag(char *filename, char *header, const char *format, ...);
void log_OP(turnOption_OP op);

saveType saveTypeFrom_string(char *s);
const char *stringFrom_HLPD_DirectionalKeys_Schema(int dks);
char *stringFrom_HLPD_KeyClass(HLPD_KeyClass k);
char *stringFrom_HLP_Region_Type(HLP_Region_Type t);
char *stringFrom_OP_res(OP_res r);
char *stringFrom_saveType(saveType s);
char *stringFrom_fightResult(fightResult fr);
char *stringFromTurnOP(turnOption_OP t);
char *stringFromFoeTurnOP(foeTurnOption_OP ft);
char *stringFromStatus(fighterStatus s);
char *stringFromClass(fighterClass f);
char *stringFromEClass(enemyClass e);
char *stringFromBossClass(bossClass b);
char *stringFromConsumables(int c);
char *stringFromEquips(int e);
char *stringFromEquipzones(int z);
char *stringFromQuality(int q);
char *stringFromArtifacts(int a);
char *stringFromFloorclass(floorClass fc);

char *descStringFromSpecial(fighterClass class, specialMove move);
char *nameStringFromSpecial(fighterClass class, specialMove move);
int costFromSpecial(fighterClass class, specialMove move);

char *descStringFromSkill(skillType class);
char *nameStringFromSkill(skillType class);
int costFromSkill(skillType class);

char *nameStringFromPerk(int p);
char *descStringFromPerk(int p);

char *descStringFromTreasure(int t);
char *descStringFromChest(int c);

char *stringFromWinconClass(winconClass w);

char *stringFromFoePartyClass(foePartyClass fp);
char *stringFromRoom(roomClass r);
char *stringFromGamemode(Gamemode g);

void scanName(Fighter * player);

void printClasses(void);
int scanClass(void);
void pickClass(Fighter * player);

void printWincons(void);
int scanWincon(void);
void pickWincon(Wincon * w);

void pickName(Fighter * player);

void copy_fighter(Fighter * source, Fighter * dest);

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

void setEquipPrices(int size, int *equipPrices, Equip * equips[]);
void setConsumablePrices(int size, int *consumablePrices,
                         Consumable ** consumables);

void statReset(Fighter * player, int force);
void statResetBoss(Boss * b, int force);
void statResetEnemy(Enemy * e, int force);
int getBossBoost(int lvl, bossClass bclass);
int getEnemyBoost(int lvl, enemyClass eclass);

void resetPermboosts(Fighter * f);
void applyPermboosts(Fighter * f);

void resetArtifactsState(Fighter * f);

void printSpawnMessage(Enemy * e, int roomIndex, int enemyIndex);
int dropConsumable(Fighter * player);
int dropArtifact(Fighter * player);

int getConsumableQty(Fighter * f, int n);
void emptyConsumables(Fighter * player);
void emptyArtifacts(Fighter * player);
void emptyEquips(Fighter * player);
Path *randomise_path(char* seed, Koliseo * kls, const char *path_to_savefile);
void printStats(Fighter * f);
void printEStats(Enemy * e);
void printConsumablesStats(Consumable * c);
void printArtifactStats(Artifact * a);
void enqueue_notification(char *text, int time, int color, RingaBuf* rb_notifications);
void applyStatus(Fighter * f, RingaBuf* rb_notifications);
void applyEStatus(Enemy * e, RingaBuf* rb_notifications);
void applyBStatus(Boss * b, RingaBuf* rb_notifications);
void printStatusText(fighterStatus status, char *subject, int color, RingaBuf* rb_notifications);
void dropEquip(Fighter * player, int beast, Koliseo * kls, RingaBuf* rb_notifications);

#ifdef HELAPORDO_CURSES_BUILD
void test_game_color_pairs(WINDOW * win, int colors_per_row);
void init_Gamestate(Gamestate * gmst, clock_t start_time, countStats * stats, Wincon * wincon,
                    Path * path, Fighter * player, Gamemode gamemode, GameScreen* screen, GameOptions* options, bool is_seeded);
turnOP_args *init_turnOP_args(Gamestate * gmst, Fighter * actor, Path * path,
                              Room * room, loadInfo * load_info, Enemy * enemy,
                              Boss * boss, FILE * save_file,
                              WINDOW * notify_win, Koliseo_Temp * t_kls,
                              foeTurnOption_OP foe_op, skillType picked_skill, RingaBuf* rb_notifications);
void print_label(WINDOW * win, int starty, int startx, int width, char *string,
                 chtype color);
void setEquipSprite(Equip * e);
void setConsumableSprite(Consumable * c);
void setArtifactSprite(Artifact * a);
void printEquipStats(Equip * e);
void printQualityColor(quality q);
void unlockSpecial(Fighter * f);
void printCounters(Turncounter * counters[]);
void printActivePerks(Fighter * f);
int retry(char* seed);
void getParams(int argc, char **argv, Fighter * player, Path * path, int optTot,
               Koliseo * kls);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
void test_game_color_pairs(Rectangle * win, int colors_per_row);
void init_Gamestate(Gamestate * gmst, clock_t start_time, countStats * stats, Wincon * wincon,
                    Path * path, Fighter * player, Gamemode gamemode);
turnOP_args *init_turnOP_args(Gamestate * gmst, Fighter * actor, Path * path,
                              Room * room, loadInfo * load_info, Enemy * enemy,
                              Boss * boss, FILE * save_file,
                              Rectangle * notification_area, Koliseo_Temp * t_kls,
                              foeTurnOption_OP foe_op, skillType picked_skill);
void unlockSpecial(Fighter * f);
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

void death(Fighter * player, loadInfo * load_info);
void e_death(Enemy * e);
void b_death(Boss * b);

void updateCounters(Turncounter * counters[], int isEnemy, Fighter * f,
                    Enemy * e);
void updateCounters_Boss(Turncounter * counters[], int isBoss, Fighter * f,
                         Boss * b);

void checkremainder(Fighter * player, int xp);
void giveXp(Fighter * player, Enemy * e);
void giveXp_Boss(Fighter * player, Boss * b);
int getEnemyXpGain(Enemy * e);
int getBossXpGain(Boss * b);
void onLevelUp(Fighter * player);
int getBoost(int lvl, int luck);
void sell_all_equips(Fighter * f, Koliseo_Temp * t_kls);

turnOption getTurnChoice(char *ch);
foeTurnOption enemyTurnPick(Enemy * e, Fighter * f);
foeTurnOption bossTurnPick(Boss * b, Fighter * f);
void quit(Fighter * p, Room * room, loadInfo * load_info, Koliseo_Temp * t_kls);
void setCounter(Turncounter * c, int turns);
void useConsumable(Fighter * f, Enemy * e, Boss * b, char *string, int isBoss);

int hlpd_rand_docount(bool count);
int hlpd_rand(void);
unsigned long hlpd_hash(unsigned char *str);
void gen_random_seed(char buffer[PATH_SEED_BUFSIZE+1]);
bool check_seed(char buffer[PATH_SEED_BUFSIZE]);

#define hlpd_d_keyval(key) ((((key) >= 0) && ((key) <= HLPD_KEYCLASS_MAX)) ? (hlpd_default_keybinds[(key)].val) : -1 )
void hlpd_reset_logfile(void);
void hlpd_use_forced_flags(const char* whoami);
int display_colorpairs(void);
int hlpd_getopt(size_t argc, char** argv, const char* whoami);
#endif
