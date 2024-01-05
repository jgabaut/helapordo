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

#ifdef HELAPORDO_CURSES_BUILD
#ifdef _WIN32
#include <ncursesw/panel.h>
#else
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
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "game_core.h"

void printGlobVars(void);

#ifdef _WIN32
void printWin_EnvVars(void);
void log_Win_EnvVars(void);
#endif

bool set_Saveslot_name(FILE * file, Saveslot * sv);

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
#ifdef HELAPORDO_CURSES_BUILD
void test_game_color_pairs(WINDOW * win, int colors_per_row);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
void test_game_color_pairs(Rectangle * win, int colors_per_row);
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
void resolve_staticPath(char static_path[500]);

void dbg_print_floor_layout(Floor * floor);
void dbg_print_explored_layout(Floor * floor);
void dbg_print_roomclass_layout(Floor * floor);
void dbg_Gamestate(Gamestate * gmst);
void dbg_Fighter(Fighter * fighter);
void dbg_countStats(countStats * stats);
void dbg_Wincon(Wincon * wc);
void dbg_Path(Path * path);
void dbg_Saveslot(Saveslot * saveslot);
void init_Gamestate(Gamestate * gmst, clock_t start_time, countStats * stats, Wincon * wincon,
                    Path * path, Fighter * player, Gamemode gamemode);
void update_Gamestate(Gamestate * gmst, int current_fighters,
                      roomClass current_roomtype, int current_room_index,
                      int current_enemy_index, Floor * current_floor);

void loadLore(char **lore_strings, int loreKind);

void setRoomType(Path * path, int *roadFork_value, roomClass * room_type,
                 int roomsDone);
void freeRoom(Room * room);

void printTitle(void);

void printVersion(void);
void printFormattedVersion(char *progName);
void hlpd_dbg_features(void);

void usage(char *progname);
void log_tag(char *filename, char *header, const char *format, ...);
void log_OP(turnOption_OP op);

#ifdef HELAPORDO_CURSES_BUILD
turnOP_args *init_turnOP_args(Gamestate * gmst, Fighter * actor, Path * path,
                              Room * room, loadInfo * load_info, Enemy * enemy,
                              Boss * boss, FILE * save_file,
                              WINDOW * notify_win, Koliseo_Temp * t_kls,
                              foeTurnOption_OP foe_op, skillType picked_skill);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
turnOP_args *init_turnOP_args(Gamestate * gmst, Fighter * actor, Path * path,
                              Room * room, loadInfo * load_info, Enemy * enemy,
                              Boss * boss, FILE * save_file,
                              Rectangle * notification_area, Koliseo_Temp * t_kls,
                              foeTurnOption_OP foe_op, skillType picked_skill);
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

saveType saveTypeFrom_string(char *s);
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

#endif
