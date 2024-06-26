#ifndef GAME_DEBUG_H
#define GAME_DEBUG_H

#include "floors.h"
//TODO This inclusion could be in a better place
#include "../bsp/bsp.h"

#ifdef _WIN32
#else
#include <sys/utsname.h>
#endif // _WIN32

#ifdef HELAPORDO_CURSES_BUILD
void debug_generic(Gamestate * gmst, Fighter * player, Path * p, int roomIndex,
                   Koliseo * kls, Koliseo_Temp * t_kls);
void debug_enemies_room(Gamestate * gmst, Room * room, Fighter * player,
                        Enemy * e, Path * p, int roomIndex, int currentEnemyNum,
                        Koliseo * kls, Koliseo_Temp * t_kls);
#endif // HELAPORDO_CURSES_BUILD
#endif // GAME_DEBUG_H
