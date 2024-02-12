#ifndef GAME_DEBUG_H
#define GAME_DEBUG_H

#include "game_utils.h"
#include "floors.h"

#ifdef _WIN32
#else
#include <sys/utsname.h>
#endif // _WIN32

void debug_generic(Gamestate * gmst, Fighter * player, Path * p, int roomIndex,
                   Koliseo * kls, Koliseo_Temp * t_kls);
void debug_enemies_room(Gamestate * gmst, Room * room, Fighter * player,
                        Enemy * e, Path * p, int roomIndex, int currentEnemyNum,
                        Koliseo * kls, Koliseo_Temp * t_kls);
#endif // GAME_DEBUG_H
