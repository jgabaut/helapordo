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

#ifndef SPECIALS_H
#define SPECIALS_H
#include "game_core.h"

#ifdef _WIN32
#include <ncursesw/panel.h>
#include <ncursesw/menu.h>
#else
#include <panel.h>
#include <menu.h>
#endif

void knightSpecial_Slash(WINDOW * w, Fighter * f, Enemy * e, Boss * b, Path * p,
                         int roomIndex, int enemyIndex, int isBoss);
void knightSpecial_Cover(WINDOW * w, Fighter * f, Enemy * e, Boss * b, Path * p,
                         int roomIndex, int enemyIndex, int isBoss);
void knightSpecial_Armordrop(WINDOW * w, Fighter * f, Enemy * e, Boss * b,
                             Path * p, int roomIndex, int enemyIndex,
                             int isBoss);
void knightSpecial_Berserk(WINDOW * w, Fighter * f, Enemy * e, Boss * b,
                           Path * p, int roomIndex, int enemyIndex, int isBoss);

void archerSpecial_Headshot(WINDOW * w, Fighter * f, Enemy * e, Boss * b,
                            Path * p, int roomIndex, int enemyIndex,
                            int isBoss);
void archerSpecial_Quivercheck(WINDOW * w, Fighter * f, Enemy * e, Boss * b,
                               Path * p, int roomIndex, int enemyIndex,
                               int isBoss);
void archerSpecial_Poisonshot(WINDOW * w, Fighter * f, Enemy * e, Boss * b,
                              Path * p, int roomIndex, int enemyIndex,
                              int isBoss);
void archerSpecial_Fireshot(WINDOW * w, Fighter * f, Enemy * e, Boss * b,
                            Path * p, int roomIndex, int enemyIndex,
                            int isBoss);

void mageSpecial_Fatewarp(WINDOW * w, Fighter * f, Enemy * e, Boss * b,
                          Path * p, int roomIndex, int enemyIndex, int isBoss);
void mageSpecial_Powerup(WINDOW * w, Fighter * f, Enemy * e, Boss * b, Path * p,
                         int roomIndex, int enemyIndex, int isBoss);
void mageSpecial_Spellstrike(WINDOW * w, Fighter * f, Enemy * e, Boss * b,
                             Path * p, int roomIndex, int enemyIndex,
                             int isBoss);
void mageSpecial_Flamering(WINDOW * w, Fighter * f, Enemy * e, Boss * b,
                           Path * p, int roomIndex, int enemyIndex, int isBoss);

void assassinSpecial_Grimdagger(WINDOW * w, Fighter * f, Enemy * e, Boss * b,
                                Path * p, int roomIndex, int enemyIndex,
                                int isBoss);
void assassinSpecial_Leechknife(WINDOW * w, Fighter * f, Enemy * e, Boss * b,
                                Path * p, int roomIndex, int enemyIndex,
                                int isBoss);
void assassinSpecial_Disguise(WINDOW * w, Fighter * f, Enemy * e, Boss * b,
                              Path * p, int roomIndex, int enemyIndex,
                              int isBoss);
void assassinSpecial_Venomblade(WINDOW * w, Fighter * f, Enemy * e, Boss * b,
                                Path * p, int roomIndex, int enemyIndex,
                                int isBoss);

#endif
