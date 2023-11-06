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

#include <string.h>
#include <locale.h>

#include "game_core.h"
#include "game_utils.h"

#include "palette.h"

#include "./animations/knight_tapis.h"
#include "./animations/mage_spark.h"
#include "./animations/archer_drop.h"
#include "./animations/assassin_poof.h"

#include "./animations/mummy_shuffle.h"
#include "./animations/ghost_spell.h"
#include "./animations/boar_scream.h"
#include "./animations/troll_club.h"
#include "./animations/goblin_shoot.h"
#include "./animations/zombie_walk.h"
#include "./animations/imp_fireball.h"
#include "./animations/werewolf_transform.h"

#include "./animations/crawlingdude_crawl.h"
#include "./animations/srwarthog_square.h"
#include "./animations/headlessninja_throw.h"
#include "./animations/bluetroll_wonder.h"

#include "./animations/enter_door.h"
#include "./animations/alt_chest_opening.h"

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

void print_label(WINDOW * win, int starty, int startx, int width, char *string,
		 chtype color);

void printStatusText(WINDOW * notify_win, fighterStatus status, char *subject);

void applyStatus(WINDOW * notify_win, Fighter * f);

void applyEStatus(WINDOW * notify_win, Enemy * e);

void applyBStatus(WINDOW * notify_win, Boss * b);

void display_notification(WINDOW * w, char *text, int time);

void setConsumableSprite(Consumable * c);
void setArtifactSprite(Artifact * a);
void setEquipSprite(Equip * e);
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
#endif
