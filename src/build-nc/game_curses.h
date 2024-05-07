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

#ifndef GAMECURSES_H
#define GAMECURSES_H

#ifdef _WIN32
#include <ncursesw/panel.h>
#include <ncursesw/menu.h>
#else
#include <panel.h>
#include <menu.h>
#endif

#define EXPECTED_NCURSES_VERSION_MAJOR 6 /**< Defines min expected major ncurses version.*/
#define EXPECTED_NCURSES_VERSION_MINOR 4 /**< Defines min expected minor ncurses version.*/
#define EXPECTED_NCURSES_VERSION_PATCH 20230520	/**< Defines min expected patch ncurses version.*/

#define HLPD_DEFAULT_SCREEN_ROWS 24
#define HLPD_DEFAULT_SCREEN_COLS 80
#define HLPD_MIN_SCREEN_ROWS (HLPD_DEFAULT_SCREEN_ROWS+4) /**< Defines minimum y size for game screen.*/
#define HLPD_MIN_SCREEN_COLS (HLPD_DEFAULT_SCREEN_COLS) /**< Defines minimum x size for game screen.*/

#include <locale.h>

#include "../utils/turn_op.h"
#include "../core/game_animations.h"

int display_colorpairs(void);
void print_encoded_char(WINDOW * w, int y, int x, char c);
int get_saveslot_index(void);
void displayLore(char **lore_strings, int lore_counter);

void printBattleStats(WINDOW * wins[3], Fighter * f, Enemy * e);

void init_wins(WINDOW ** wins, Enemy * e, Fighter * f);
void win_show(WINDOW * win, Enemy * e, Fighter * f, int isEnemy, int border);
void print_in_panel(WINDOW * win, int starty, int startx, int width, Enemy * e,
                    Fighter * f, int isEnemy);

void printBossBattleStats(WINDOW * wins[3], Fighter * f, Boss * b);

void init_boss_wins(WINDOW ** wins, Boss * b, Fighter * f);
void boss_win_show(WINDOW * win, Boss * b, Fighter * f, int isBoss, int border);
void boss_print_in_panel(WINDOW * win, int starty, int startx, int width,
                         Boss * b, Fighter * f, int isBoss);

void setEquipslotSprite(Equipslot * s);
void setEnemySprite(Enemy * e);
void setBossSprite(Boss * b);
void setFighterSprite(Fighter * f);
void setChestSprite(Chest * c);

void updateSelectedConsumableW(WINDOW * w, MENU * my_menu, Fighter * f);

void updateSelectedArtifactW(WINDOW * w, MENU * my_menu, Fighter * f);

void updateSelectedEquipW(WINDOW * w, MENU * my_menu, Equip * selected,
                          Fighter * f);

void updateEquipslotsWin(WINDOW * w, Fighter * f);

void updateSelectedSpecialW(WINDOW * w, MENU * my_menu, Fighter * f);

void updateShopDisplayW_Equip(WINDOW * w, Equip * e);

void updateShopDisplayW_Consumable(WINDOW * w, Consumable * c);

void debug_printFoeParty(FoeParty * fp);
void display_printFoeParty(FoeParty * fp);
void printLoadout(Fighter * f);
void displayEquipbagMenu(Fighter * f);

void handleConsumables(Fighter * f, Enemy * e, Boss * b, int isBoss);
void handleArtifacts(Fighter * f);
void handleEquips(Fighter * f, Path * p);
void fight_Special(specialMove move, WINDOW * w, Fighter * f, Enemy * e,
                   Boss * b, Path * p, int roomIndex, int enemyIndex,
                   int isBoss);
void handleSpecials(Fighter * f, Enemy * e, Boss * b, Path * p, int roomIndex,
                    int enemyIndex, int isBoss);
void handleStats(Fighter * f);
void handleTutorial(void);
int handleRogueMenu(Gamestate * gmst, Path * p, Fighter * player, Room * room,
                    loadInfo * load_info, Koliseo * kls, Koliseo_Temp * t_kls);
int handleGameOptions(GameOptions * game_options);
void draw_buildinfo(WINDOW* win);
#endif
