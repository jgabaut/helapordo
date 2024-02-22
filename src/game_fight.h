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

#ifndef GAME_FIGHT_H_
#define GAME_FIGHT_H_

#include "game_utils.h"

#ifdef HELAPORDO_CURSES_BUILD
int defer_fight_enemy(Fighter * player, Enemy * e, foeTurnOption_OP foe_op,
                      WINDOW * notify_win, Koliseo * kls);
int defer_skill_enemy(Fighter *player, Enemy *e, skillType picked_skill, foeTurnOption_OP foe_op,
                      WINDOW * notify_win, Koliseo * kls);

int fight(Fighter * player, Enemy * e, WINDOW * notify_win, Koliseo * kls);

int enemy_attack(Enemy * e, Fighter * target, WINDOW * notify_win,
                 Koliseo * kls);

int defer_fight_boss(Fighter * player, Boss * b, Path * p,
                     foeTurnOption_OP foe_op, WINDOW * notify_win,
                     Koliseo * kls);

int defer_skill_boss(Fighter *player, Boss *b, skillType picked_skill, Path *p, foeTurnOption_OP foe_op,
                     WINDOW *notify_win, Koliseo *kls);

int boss_fight(Fighter * player, Boss * b, Path * p, WINDOW * notify_win,
               Koliseo * kls);

int boss_attack(Boss * b, Fighter * target, Path * p, WINDOW * notify_win,
                Koliseo * kls);
fightResult do_Skill(Fighter * player, Enemy * e, skillType picked_skill, WINDOW * notify_win, Koliseo * kls);
fightResult do_Skill_boss(Fighter * player, Boss * b, skillType picked_skill, Path * path, WINDOW * notify_win, Koliseo * kls);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
int defer_fight_enemy(Fighter * player, Enemy * e, foeTurnOption_OP foe_op,
                      Rectangle * notification_area, Koliseo * kls);
int defer_skill_enemy(Fighter *player, Enemy *e, skillType picked_skill, foeTurnOption_OP foe_op,
                      Rectangle * notification_area, Koliseo * kls);

int fight(Fighter * player, Enemy * e, Rectangle * notification_area, Koliseo * kls);

int enemy_attack(Enemy * e, Fighter * target, Rectangle * notification_area,
                 Koliseo * kls);

int defer_fight_boss(Fighter * player, Boss * b, Path * p,
                     foeTurnOption_OP foe_op, Rectangle * notification_area,
                     Koliseo * kls);

int defer_skill_boss(Fighter *player, Boss *b, skillType picked_skill, Path *p, foeTurnOption_OP foe_op,
                     Rectangle * notification_area, Koliseo *kls);

int boss_fight(Fighter * player, Boss * b, Path * p, Rectangle * notification_area,
               Koliseo * kls);

int boss_attack(Boss * b, Fighter * target, Path * p, Rectangle * notification_area,
                Koliseo * kls);
fightResult do_Skill(Fighter * player, Enemy * e, skillType picked_skill, Rectangle * notification_area, Koliseo * kls);
fightResult do_Skill_boss(Fighter * player, Boss * b, skillType picked_skill, Path * path, Rectangle * notification_area, Koliseo * kls);
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

#endif // GAME_FIGHT_H_
