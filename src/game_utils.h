#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#include <stdio.h>
#include <termios.h>
#include <panel.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "game_core.h"

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
void screenTime(float t);
void resolve_staticPath(char static_path[500]);

void dbg_Gamestate(Gamestate* gmst);
void init_Gamestate(Gamestate* gmst, countStats* stats, Wincon* wincon, Path* path, Fighter* player, Gamemode gamemode );
void update_Gamestate(Gamestate* gmst, int current_fighters, roomClass current_roomtype, int current_room_index, int current_enemy_index);

void loadLore(char** lore_strings, int loreKind);

void setRoomType(Path* path, int* roadFork_value, roomClass* room_type, int roomsDone);
void freeRoom(Room* room);

void printTitle(void);

void printVersion(void);
void printFormattedVersion(char* progName);

void usage(char* progname);
void log_tag(char* filename, char* header, char* msg);
void log_OP(turnOption_OP op);

turnOP_args* init_turnOP_args(Fighter* actor, Path* path, Room* room, loadInfo* load_info, Enemy* enemy, Boss* boss, FILE* save_file, WINDOW* notify_win, Koliseo_Temp* t_kls);

saveType saveTypeFrom_string(char* s);
char* stringFrom_HLP_Region_Type(HLP_Region_Type t);
char* stringFrom_OP_res(OP_res r);
char* stringFrom_saveType(saveType s);
char* stringFrom_fightResult(fightResult fr);
char* stringFromTurnOP(turnOption_OP t);
char* stringFromStatus(fighterStatus s);
char* stringFromClass(fighterClass f);
char* stringFromEClass(enemyClass e);
char* stringFromBossClass(bossClass b);
char* stringFromConsumables(int c);
char* stringFromEquips(int e);
char* stringFromEquipzones(int z);
char* stringFromQuality(int q);
char* stringFromArtifacts(int a);

char* descStringFromSpecial(fighterClass class, specialMove move);
char* nameStringFromSpecial(fighterClass class, specialMove move);
int costFromSpecial(fighterClass class, specialMove move);

char* nameStringFromPerk(int p);
char* descStringFromPerk(int p);

char* descStringFromTreasure(int t);
char* descStringFromChest(int c);

char* stringFromWinconClass(winconClass w);

char* stringFromFoePartyClass(foePartyClass fp);
char* stringFromRoom(roomClass r);
char* stringFromGamemode(Gamemode g);

void scanName(Fighter* player);

void printClasses(void);
int scanClass(void);
void pickClass(Fighter* player);

void printWincons(void);
int scanWincon(void);
void pickWincon(Wincon* w);

void pickName(Fighter* player);

void copy_fighter(Fighter* source, Fighter* dest);

#endif
