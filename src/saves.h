#ifndef SAVES_H_
#define SAVES_H_

#include "game_utils.h"
#include "equips.h"
#include "specials.h"
#include "game_init.h"

#ifdef HELAPORDO_CURSES_BUILD
#include "game_curses.h"
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
#include "game_rl.h"
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

OP_res handleLoadgame_Home(FILE * file, Fighter * f, Path * p, int *roomIndex,
                           int *done_loading, Koliseo * kls);
OP_res handleLoadgame_Enemies(FILE * file, Fighter * f, Path * p, Enemy * e,
                              int *enemyIndex, int *roomTotalEnemies,
                              int *roomIndex, int *total_foes,
                              int *done_loading, Koliseo * kls);
OP_res handleSave_Enemies(FILE * file, Fighter * f, Path * p, Enemy * e,
                          int enemyIndex, int roomTotalEnemies, int roomIndex);
OP_res handleSave_Home(FILE * file, Fighter * f, Path * p, int roomIndex);
saveType read_saveType(FILE * file);
#endif // SAVES_H_
