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

#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#ifndef _WIN32
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L // Needed for getopt
#endif // _POSIX_C_SOURCE
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
#include <time.h>
#include <inttypes.h>
#include <signal.h>
#include "../core/game_core.h"
#include "../core/game_log.h"
#include "../core/game_strings.h"
#include "../core/game_rng.h"
#include "../core/game_fight.h"
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

saveType saveTypeFrom_string(char *s);

void scanName(Fighter * player);

void printClasses(void);
int scanClass(void);
void pickClass(Fighter * player);

void printWincons(void);
int scanWincon(void);
void pickWincon(Wincon * w);

void pickName(Fighter * player);

void copy_fighter(Fighter * source, Fighter * dest);

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
                              foeTurnOption_OP foe_op, skillType picked_skill, RingaBuf* rb_notifications);
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

int checkremainder(Fighter * player, int xp);
int giveXp(Fighter * player, Enemy * e);
int giveXp_Boss(Fighter * player, Boss * b);
int getEnemyXpGain(Enemy * e);
int getBossXpGain(Boss * b);
int onLevelUp(Fighter * player);
void sell_all_equips(Fighter * f);

turnOption getTurnChoice(char *ch);
foeTurnOption enemyTurnPick(Enemy * e, Fighter * f);
foeTurnOption bossTurnPick(Boss * b, Fighter * f);
void quit(Fighter * p, Room * room, loadInfo * load_info, Koliseo_Temp * t_kls);
void useConsumable(Fighter * f, Enemy * e, Boss * b, char *string, int isBoss);

#define hlpd_d_keyval(key) ((((key) >= 0) && ((key) <= HLPD_KEYCLASS_MAX)) ? (hlpd_default_keybinds[(key)].val) : -1 )
void hlpd_use_forced_flags(const char* whoami);
int display_colorpairs(void);
int hlpd_getopt(size_t argc, char** argv, const char* whoami);
#endif
