#ifndef GAMECURSES_H
#define GAMECURSES_H
#include <panel.h>
#include <menu.h>
#include <string.h>
#include <locale.h>

#include "game_core.h"
#include "game_utils.h"

int display_colorpairs(void);
void print_encoded_char(WINDOW* w, int y, int x, char c);
void displayLore(char** lore_strings, int lore_counter);

void printBattleStats(WINDOW* wins[3], Fighter* f, Enemy* e);

void init_wins(WINDOW **wins, Enemy* e, Fighter* f);
void win_show(WINDOW *win, Enemy* e, Fighter* f, int isEnemy, int border);
void print_in_panel(WINDOW *win, int starty, int startx, int width, Enemy* e, Fighter* f, int isEnemy);

void printBossBattleStats(WINDOW* wins[3], Fighter* f, Boss* b);

void init_boss_wins(WINDOW **wins, Boss* b, Fighter* f);
void boss_win_show(WINDOW *win, Boss* b, Fighter* f, int isBoss, int border);
void boss_print_in_panel(WINDOW *win, int starty, int startx, int width, Boss* b, Fighter* f, int isBoss);

void print_label(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

void printStatusText(WINDOW *notify_win, fighterStatus status, char* subject);

void applyStatus(WINDOW* notify_win, Fighter* f);

void applyEStatus(WINDOW *notify_win, Enemy* e);

void applyBStatus(WINDOW *notify_win, Boss* b);

void display_notification(WINDOW* w, char* text, int time);

void setConsumableSprite(Consumable* c);
void setArtifactSprite(Artifact* a);
void setEquipSprite(Equip* e);
void setEquipslotSprite(Equipslot* s);
void setEnemySprite(Enemy* e);
void setBossSprite(Boss* b);
void setFighterSprite(Fighter* f);
void setChestSprite(Chest* c);

void updateSelectedConsumableW(WINDOW* w, MENU* my_menu, Fighter* f);

void updateSelectedArtifactW(WINDOW* w, MENU* my_menu, Fighter* f);

void updateSelectedEquipW(WINDOW* w, MENU* my_menu, Equip* selected,Fighter* f);

void updateEquipslotsWin(WINDOW* w, Fighter* f);

void updateSelectedSpecialW(WINDOW* w, MENU* my_menu, Fighter* f);

void updateShopDisplayW_Equip(WINDOW* w, Equip* e);

void updateShopDisplayW_Consumable(WINDOW* w, Consumable* c);

void debug_printFoeParty(FoeParty* fp);
void display_printFoeParty(FoeParty* fp);
void printLoadout(Fighter* f);
void displayEquipbagMenu(Fighter* f);

void handleConsumables(Fighter* f, Enemy* e, Boss* b, int isBoss);
void handleArtifacts(Fighter* f);
void handleEquips(Fighter* f, Path* p);
void fight_Special(specialMove move, WINDOW* w, Fighter* f, Enemy* e, Boss* b, Path* p, int roomIndex, int enemyIndex, int isBoss);
void handleSpecials(Fighter* f, Enemy* e, Boss* b, Path* p, int roomIndex, int enemyIndex, int isBoss);
void handleStats(Fighter* f);
void handleTutorial(void);
#endif
