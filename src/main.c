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
#include "helapordo.h"
#include "floor_tester.h"

int G_PRELOAD_ANIMATIONS_ON = 0;
int G_DEBUG_ON = 0;
int G_LOG_ON = 0;
int G_EXPERIMENTAL_ON = 0;
int G_FASTQUIT_ON = 0;
int G_GODMODE_ON = 0;
int G_DEBUG_ROOMTYPE = ENEMIES;
int G_DEBUG_ROOMTYPE_ON = 0;
char *G_DEBUG_ROOMTYPE_ARG = NULL;
int G_DEBUG_ENEMYTYPE = Zombie;
int G_DEBUG_ENEMYTYPE_ON = 0;
char *G_DEBUG_ENEMYTYPE_ARG = NULL;

int G_DOTUTORIAL_ON = 0;

int GS_AUTOSAVE_ON = 1;
Gamemode GAMEMODE = Rogue;
Koliseo *default_kls = NULL;
Koliseo *temporary_kls = NULL;

int main(int argc, char **argv)
{
    //Randomise seed
    srand(time(NULL));

    setlocale(LC_ALL, "");
    // Randomise path
    //      Branches?
    //
    //test_floors();
#ifndef _WIN32
    gameloop(argc, argv);
#else
    gameloop_Win(argc, argv);
#endif
    //      Monsters
    //      Leveling
    //      Abilities?
    //      Items?
    //      Turns

    // Settings
    //
    // Saves
    //
    // Victory / Death

    return 1;
}
