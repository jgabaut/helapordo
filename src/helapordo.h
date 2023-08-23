#ifndef HELAPORDO_H
#define HELAPORDO_H
#include <sys/utsname.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <panel.h>
#include <menu.h>
#include <locale.h>
#include <sys/stat.h>
#include "../sprites4curses/s4c-animate/animate.h"
#define DEMOFRAMES 30 /**< Defines the number of sprites in the demo.*/
#define DEMOROWS 18 /**< Defines the maximum number of rows per sprite.*/
#define DEMOCOLS 18 /**< Defines the maximum number of colums per sprite.*/
#define DEMOFRAMETIME 67 /**< Defines for how many millisecs a sprite should stay on screen in the demo.*/
#include "../koliseo/src/koliseo.h"
#include "game_core.h"
#include "game_utils.h"
#include "rooms.h"
#include "specials.h"
#include "artifacts.h"
#include "game_curses.h"
#include "sprites.h"
#include "floors.h"
#include "anvil__helapordo.h"

/**
 * Call function associated with the passed turnOption_OP.
 * @param op The turnOption_OP to execute.
 * @param args Pointer to turnOP_args object.
 * @param kls The Koliseo used for allocations.
 */
OP_res turnOP(turnOption_OP op, turnOP_args* args, Koliseo* kls, Koliseo_Temp* t_kls);

void register_counter_callback(int index, callback_void_t ptr, Fighter*);

void resetFighterStatus(Fighter* f);

void resetEnemyStatus(Enemy* e);

void resetBossStatus(Boss* b);

effect_fun getStatusCounterFun(fighterStatus status);

effect_e_fun getStatusCounterEnemyFun(fighterStatus status);

effect_b_fun getStatusCounterBossFun(fighterStatus status);

effect_fp_fun get_StatusCounter_FoeParty_Fun(fighterStatus status);

void set_turnboost_atk(Fighter* f,int boost);
void set_turnboost_def(Fighter* f,int boost);
void set_turnboost_vel(Fighter* f,int boost);
void set_turnboost_enr(Fighter* f,int boost);

void set_enemy_turnboost_atk(Enemy* e,int boost);
void set_enemy_turnboost_def(Enemy* e,int boost);
void set_enemy_turnboost_vel(Enemy* e,int boost);
void set_enemy_turnboost_enr(Enemy* e,int boost);

void set_boss_turnboost_atk(Boss* b,int boost);
void set_boss_turnboost_def(Boss* b,int boost);
void set_boss_turnboost_vel(Boss* b,int boost);
void set_boss_turnboost_enr(Boss* b,int boost);

void set_foeparty_turnboost_atk(FoeParty* fp,int boost);
void set_foeparty_turnboost_def(FoeParty* fp,int boost);
void set_foeparty_turnboost_vel(FoeParty* fp,int boost);
void set_foeparty_turnboost_enr(FoeParty* fp,int boost);

boost_fun getStatBoostCounterFun(Stat s);
boost_e_fun getStatBoostCounterEnemyFun(Stat s);
boost_b_fun getStatBoostCounterBossFun(Stat s);
boost_fp_fun get_StatBoostCounter_FoeParty_Fun(Stat s);

void initPerks(Fighter* f, Koliseo* kls);
void applyEquipPerks(Equip* e, Fighter* f);
void removeEquipPerks(Equip* e, Fighter* f);
void printActivePerks(Fighter* f);

void initCounters(Fighter* f, Koliseo* kls);

void initECounters(Enemy* e, Koliseo_Temp* t_kls);

void initBCounters(Boss* b, Koliseo_Temp* t_kls);

void initFoePartyCounters(FoeParty* fp, Koliseo_Temp* t_kls);

void printCounters(Turncounter* counters[]);

void updateCounters(Turncounter* counters[], int isEnemy, Fighter* f, Enemy* e);
void updateCounters_Boss(Turncounter* counters[], int isBoss, Fighter* f, Boss* b);
void setCounter(Turncounter* c,int turns);

void setSpecials(Fighter* f, Koliseo* kls);

void resetPermboosts(Fighter* f);
void applyPermboosts(Fighter* f);

void resetArtifactsState(Fighter* f);
void applyArtifacts(Fighter* f, Enemy* e, Boss* b, int isBoss);

void initEquipSlots(Fighter* f, Koliseo* kls);

void initConsumableBag(Fighter* f, Koliseo* kls);
void initArtifactsBag(Fighter* f, Koliseo* kls);

void initWincon(Wincon* w, Path* p, winconClass class);

void initPlayerStats(Fighter* player, Path* path, Koliseo* kls);

void initEnemyStats(Enemy* e, Koliseo_Temp* t_kls);

int getBossBoost(int lvl, bossClass bclass);

void statResetBoss(Boss* b, int force);

void initBossStats(Boss* b, Koliseo_Temp* t_kls);
void prepareBoss(Boss* b, Koliseo_Temp* t_kls);

void initFoePartyStats(FoeParty* fp, Koliseo_Temp* t_kls);
void prepareFoeParty(FoeParty* fp, int total_foes, int roomindex, Koliseo_Temp* t_kls);

int getEnemyBoost(int lvl, enemyClass eclass);

void statResetEnemy(Enemy* e, int force);

void prepareRoomEnemy(Enemy* e, int roomindex, int enemiesInRoom, int enemyindex, Koliseo_Temp* t_kls);

void setEquipPrices(int size, int* equipPrices, Equip* equips[]);
void setConsumablePrices(int size, int* consumablePrices, Consumable** consumables);
void initShop(Shop* s, int indexWeight, Fighter* player);
void initChest(Chest* c, Fighter* f);
void prepareChest(Chest* c, Fighter* f);

void initTreasure(Treasure* t, Fighter* f);
void prepareTreasure(Treasure* t, Fighter* f);

void prepareRoadfork(Roadfork* r);

void printStats(Fighter* f);

void printEStats(Enemy* e);

int getEnemyXpGain(Enemy* e);

int getBossXpGain(Boss* b);

void printConsumablesStats(Consumable* c);

void printArtifactStats(Artifact* a);

void printQualityColor(quality q);

void printEquipStats(Equip* e);

void printSpawnMessage(Enemy* e, int roomIndex, int enemyIndex);

void getParams(int argc, char** argv, Fighter* player, Path* path, int optTot, Koliseo* kls);

turnOption getTurnChoice(char* ch);

int getBoost(int lvl, int luck);

void unlockSpecial(Fighter* f);

void onLevelUp(Fighter* player);

void checkremainder(Fighter* player, int xp);

void giveXp(Fighter* player, Enemy* e);
void giveXp_Boss(Fighter* player, Boss* b);

void statReset(Fighter* player, int force);

int dropConsumable(Fighter* player);

void dropEquip(Fighter* player, int beast, WINDOW* notify_win, Koliseo* kls);

int dropArtifact(Fighter* player);

int fight(Fighter* player, Enemy* e, WINDOW* notify_win, Koliseo* kls);

int boss_fight(Fighter* player, Boss* b, Path* p, WINDOW* notify_win, Koliseo* kls);

void useConsumable(Fighter* f, Enemy* e, Boss* b, char* string, int isBoss);

int getConsumableQty(Fighter* f, int n);

void emptyConsumables(Fighter* player);

void emptyArtifacts(Fighter* player);

void emptyEquips(Fighter* player);

OP_res handleSave_Enemies(FILE* file, Fighter* f, Path*p, Enemy* e, int enemyIndex, int roomTotalEnemies, int roomIndex);
OP_res handleSave_Home(FILE* file, Fighter* f, Path*p, int roomIndex);

OP_res handleLoadgame_Enemies(FILE* file, Fighter* f, Path* p, Enemy* e, int* enemyIndex, int* roomTotalEnemies, int* roomIndex, int* total_foes, int* done_loading, Koliseo* kls);
OP_res handleLoadgame_Home(FILE* file, Fighter* f, Path* p, int* roomIndex, int* done_loading, Koliseo* kls);
saveType read_saveType(FILE* file);

void death(Fighter* player, loadInfo* load_info);

void e_death(Enemy* e);

void b_death(Boss* b);

int retry(void);

void debug_generic(Fighter* player, Path* p, int roomIndex, Koliseo* kls);
void debug_enemies_room(Room* room, Fighter* player, Enemy* e, Path* p, int roomIndex,int currentEnemyNum);

void quit(Fighter* p, Room* room, loadInfo* load_info, Koliseo_Temp* t_kls);

void sell_all_equips(Fighter* f);

void open_chest(WINDOW* w, Chest * c, Fighter* f);

Path* randomise_path(int seed, Koliseo* kls);

void gameloop(int argc, char** argv);

#endif
