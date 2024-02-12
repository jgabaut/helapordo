#include <stdio.h>
#include <string.h>
#define HELAPORDO_RAYLIB_BUILD
#include "../../src/game_core.h"
#include <raylib.h>

void fail(char* msg, int ex, int val) {
  printf("[Fail]    %s, expected (%i), was %i\n",msg,ex,val);
}

int main(void) {
 int check = -1;
 if ( ! (( check = strcmp(VERSION, "1.4.3-dev") ) == 0)) {
   fail("VERSION",0,check);
 };
 if ( ! (HLP_MAX_INDEX == 31) ) {
   fail("HLP_MAX_INDEX",31,HLP_MAX_INDEX);
 }
 if ( ! (MAX_SAVESLOTS == 3) ) {
   fail("MAX_SAVESLOTS",3,MAX_SAVESLOTS);
 }
 if ( ! (FOETURNOP_MAX == 3) ) {
   fail("FOETURNOP_MAX",3,FOETURNOP_MAX);
 };
 if ( ! (TURNOP_MAX == 16) ) {
   fail("TURNOP_MAX",16,TURNOP_MAX);
 };
 if ( ! (OP_RES_MAX == 10) ) {
   fail("OP_RES_MAX",10,OP_RES_MAX);
 };
 if ( ! (FIGHT_RES_TOT == 7) ) {
   fail("FIGHT_RES_TOT",7,FIGHT_RES_TOT);
 };
 if ( ! (MAXPLAYERLUCK == 30) ) {
   fail("MAXPLAYERLUCK",30,MAXPLAYERLUCK);
 };
 if ( ! (MAXLENGTH == 100 )) {
   fail("MAXLENGTH",100,MAXLENGTH);
 };
 if ( ! (MAXLUCK == 50) ) {
   fail("MAXLUCK",50,MAXLUCK);
 };
 if ( ! (GAMEMODE_MAX == 2 )) {
   fail("GAMEMODE_MAX",2,GAMEMODE_MAX);
 };
 if ( ! (LORES_MAX == 2) ) {
   fail("LORES_MAX",2,LORES_MAX);
 };
 if ( ! (MAX_ROGUE_FLOORS == 17) ) {
   fail("MAX_ROGUE_FLOORS",17,MAX_ROGUE_FLOORS);
 };
 if ( ! (CONSUMABLESMAX == 5) ) {
   fail("CONSUMABLESMAX",5,CONSUMABLESMAX);
 };
 if ( ! (ARTIFACTSMAX == 6) ) {
   fail("ARTIFACTSMAX",6,ARTIFACTSMAX);
 };
 if ( ! (STATUSMAX == 5) ) {
   fail("STATUSMAX",5,ARTIFACTSMAX);
 };
 if ( ! (CLASSESMAX == 3) ) {
   fail("CLASSESMAX",3,CLASSESMAX);
 };
 if ( ! (STATMAX == 3) ) {
   fail("STATMAX",3,STATMAX);
 };
 if ( ! (ENEMYCLASSESMAX == 7) ) {
   fail("ENEMYCLASSESMAX",7,ENEMYCLASSESMAX);
 };
 if ( ! (BOSSCLASSESMAX == 4) ) {
   fail("BOSSCLASSESMAX",4,BOSSCLASSESMAX);
 };
 if ( ! (SPECIALSMAX == 3) ) { //num of specials per class (0 inclusive)
   fail("SPECIALSMAX",3,SPECIALSMAX);
 };
 if ( ! (SKILLSTOTAL == 3) ) {
   fail("SKILLSTOTAL",3,SKILLSTOTAL);
 };
 if ( ! (FIGHTER_SKILL_SLOTS == 3) ) {
   fail("FIGHTER_SKILL_SLOTS",3,FIGHTER_SKILL_SLOTS);
 };
 if ( ! (ENEMY_SKILL_SLOTS == 3) ) {
   fail("ENEMY_SKILL_SLOTS",3,ENEMY_SKILL_SLOTS);
 };
 if ( ! (BOSS_SKILL_SLOTS == 3) ) {
   fail("BOSS_SKILL_SLOTS",3,BOSS_SKILL_SLOTS);
 };
 if ( ! (COUNTERSMAX == 9) ) {
   fail("COUNTERSMAX",9,COUNTERSMAX);
 };
 if ( ! (EQUIPZONES == 2) ) {
   fail("EQUIPZONES",2,EQUIPZONES);
 };
 if ( ! (EQUIPSBAGSIZE == 20) ) {
   fail("EQUIPSBAGSIZE",20,EQUIPSBAGSIZE);
 };
 if ( ! (PERKSMAX == 8) ) {
   fail("PERKSMAX",8,PERKSMAX);
 };
 if ( ! (EQUIPPERKSMAX == 2) ) {
   fail("EQUIPPERKSMAX",2,EQUIPPERKSMAX);
 };
char a[8];
 if ( ! ( ARRAY_SIZE(a) == ( sizeof(a) / sizeof(a[0]) ) ) ) {
   fail("ARRAY_SIZE",ARRAY_SIZE(a),(sizeof(a) / sizeof(a[0])) );
 };
 if ( ! (WINCON_CLASS_MAX == 2) ) {
   fail("WINCON_CLASS_MAX",2,WINCON_CLASS_MAX);
 };
 if ( ! (EQUIPSMAX == 5) ) {
   fail("EQUIPSMAX",5,EQUIPSMAX);
 };
 if ( ! (QUALITIESMAX == 2) ) {
   fail("QUALITIESMAX",2,QUALITIESMAX);
 };
 if ( ! (ITEM_SHOP_MAX == 20) ) {
   fail("ITEM_SHOP_MAX",20,ITEM_SHOP_MAX);
 }
 if ( ! (EQUIP_SHOP_MAX == 2) ) {
   fail("EQUIP_SHOP_MAX",2,EQUIP_SHOP_MAX);
 }
 if ( ! (CONSUMABLE_SHOP_MAX == 3) ) {
   fail("CONSUMABLE_SHOP_MAX",3,CONSUMABLE_SHOP_MAX);
 }
 if ( ! (SINGLE_CONSUMABLE_SHOP_MAX == 3) ) {
   fail("SINGLE_CONSUMABLE_SHOP_MAX",3,SINGLE_CONSUMABLE_SHOP_MAX);
 }
 if ( ! (CHEST_CLASS_MAX == 1) ) {
   fail("CHEST_CLASS_MAX",1,CHEST_CLASS_MAX);
 }
 if ( ! (CHEST_CONSUMABLES_MAX == 5) ) {
   fail("CHEST_CONSUMABLES_MAX",5,CHEST_CONSUMABLES_MAX);
 }
 if ( ! (CHEST_EQUIPS_MAX == 5) ) {
   fail("CHEST_EQUIPS_MAX",5,CHEST_EQUIPS_MAX);
 }
 if ( ! (TREASURE_CLASS_MAX == 3) ) {
   fail("TREASURE_CLASS_MAX",3,TREASURE_CLASS_MAX);
 }
 if ( ! (ROADFORK_OPTIONS_MAX == 4) ) {
   fail("ROADFORK_OPTIONS_MAX",4,ROADFORK_OPTIONS_MAX);
 }
 if ( ! (FLOOR_CLASS_MAX == 1) ) {
   fail("FLOOR_CLASS_MAX",1,FLOOR_CLASS_MAX);
 }
 if ( ! (ROOM_CLASS_MAX == 7) ) {
   fail("ROOM_CLASS_MAX",7,ROOM_CLASS_MAX);
 }
 if ( ! (ROOM_ENEMIES_MAX == 4) ) {
   fail("ROOM_ENEMIES_MAX",4,ROOM_ENEMIES_MAX);
 }
 if ( ! (FIGHT == 100) ) {
   fail("FIGHT",100,FIGHT);
 }
 if ( ! (SPECIAL == 101) ) {
   fail("SPECIAL",101,SPECIAL);
 }
 if ( ! (QUIT == 102) ) {
   fail("QUIT",102,QUIT);
 }
 if ( ! (CONSUMABLE == 103) ) {
   fail("CONSUMABLE",103,CONSUMABLE);
 }
 if ( ! (ARTIFACTS == 104) ) {
   fail("ARTIFACTS",104,ARTIFACTS);
 }
 if ( ! (EQUIPS == 105) ) {
   fail("EQUIPS",105,EQUIPS);
 }
 if ( ! (PERKS == 106) ) {
   fail("PERKS",106,PERKS);
 }
 if ( ! (STATS == 107) ) {
   fail("STATS",107,STATS);
 }
 if ( ! (SAVE == 108) ) {
   fail("SAVE",108,SAVE);
 }
 if ( ! (DEBUG == 1337) ) {
   fail("DEBUG",1337,DEBUG);
 }
 if ( ! (SHOPROOM == 4) ) {
   fail("SHOPROOM",4,SHOPROOM);
 }
 if ( ! (BOSSROOM == 5) ) {
   fail("BOSSROOM",5,BOSSROOM);
 }
 if ( ! (BEASTROOM == 3) ) {
   fail("BEASTROOM",3,BEASTROOM);
 }
 if ( ! (BSTFACTOR == 1.5) ) {
   fail("BSTFACTOR",3,BSTFACTOR);
 }
 if ( ! (ENEMYLVLRATIO == 2) ) {
   fail("ENEMYLVLRATIO",2,ENEMYLVLRATIO);
 }
 if ( ! (SPECIALLVLRATIO == 2) ) {
   fail("SPECIALLVLRATIO",2,SPECIALLVLRATIO);
 }
 if ( ! (EQUIPLVLBOOSTRATIO == 3) ) {
   fail("EQUIPLVLBOOSTRATIO",3,EQUIPLVLBOOSTRATIO);
 }
 if ( ! (FIGHTRES_KILL_DONE == 1) ) {
   fail("FIGHTRES_KILL_DONE",1,FIGHTRES_KILL_DONE);
 }
 if ( ! (FIGHTRES_DMG_DEALT == 2) ) {
   fail("FIGHTRES_DMG_DEALT",2,FIGHTRES_DMG_DEALT);
 }
 if ( ! (FIGHTRES_DMG_TAKEN == 3) ) {
   fail("FIGHTRES_DMG_TAKEN",3,FIGHTRES_DMG_TAKEN);
 }
 if ( ! (FIGHTRES_DEATH == 4) ) {
   fail("FIGHTRES_DEATH",4,FIGHTRES_DEATH);
 };
 if ( ! (FIGHTRES_NO_DMG == 5) ) {
   fail("FIGHTRES_NO_DMG",5,FIGHTRES_NO_DMG);
 };
 if ( ! (OP_OK == 0) ) {
   fail("OP_OK",0,OP_OK);
 };
 if ( ! (INVALID_OP == 1) ) {
   fail("INVALID_OP",1,INVALID_OP);
 };
 if ( ! (NO_OP == 2) ) {
   fail("NO_OP",2,NO_OP);
 };
 if ( ! (OP_CANNOT_OPEN_SAVEFILE == 3) ) {
   fail("OP_CANNOT_OPEN_SAVEFILE",3,OP_CANNOT_OPEN_SAVEFILE);
 };
 if ( ! (OP_RES_KILL_DONE == 4) ) {
   fail("OP_RES_KILL_DONE",4,OP_RES_KILL_DONE);
 };
 if ( ! (OP_RES_DMG_DEALT == 5) ) {
   fail("OP_RES_DMG_DEALT",5,OP_RES_DMG_DEALT);
 };
 if ( ! (OP_RES_DMG_TAKEN == 6) ) {
   fail("OP_RES_DMG_TAKEN",6,OP_RES_DMG_TAKEN);
 };
 if ( ! (OP_RES_DEATH == 7) ) {
   fail("OP_RES_DEATH",7,OP_RES_DEATH);
 };
 if ( ! (OP_RES_NO_DMG == 8) ) {
   fail("OP_RES_NO_DMG",8,OP_RES_NO_DMG);
 };
 return 0;
}
