#ifndef GAME_INIT_H_
#define GAME_INIT_H_

#include "game_utils.h"
#include "specials.h"

#ifdef HELAPORDO_CURSES_BUILD
#include "game_curses.h"
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
#include "game_rl.h"
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

void initWincon(Wincon * w, Path * p, winconClass class);

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
