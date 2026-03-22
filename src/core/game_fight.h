// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2022-2026 jgabaut

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

#include "../core/game_log.h"
#include "../core/game_rng.h"
#include "../notifications/game_notifications.h"
//#include "game_utils.h"

void setCounter(Turncounter * c, int turns);
int getBoost(int lvl, int luck);
int getBossBoost(int lvl, bossClass bclass);
int getEnemyBoost(int lvl, enemyClass eclass);
void statReset(Fighter * player, int force);
void statResetBoss(Boss * b, int force);
void statResetEnemy(Enemy * e, int force);

void printStatusText(fighterStatus status, char *subject, int color, RingaBuf* rb_notifications);
void applyStatus(Fighter * f, RingaBuf* rb_notifications);
void applyEStatus(Enemy * e, RingaBuf* rb_notifications);
void applyBStatus(Boss * b, RingaBuf* rb_notifications);

int defer_fight_enemy(Fighter * player, Enemy * e, foeTurnOption_OP foe_op,
                      Koliseo * kls, RingaBuf* rb_notifications);
int defer_skill_enemy(Fighter *player, Enemy *e, skillType picked_skill, foeTurnOption_OP foe_op,
                      Koliseo * kls, RingaBuf* rb_notifications);

int fight(Fighter * player, Enemy * e, Koliseo * kls, RingaBuf* rb_notifications);

int enemy_attack(Enemy * e, Fighter * target,
                 Koliseo * kls, RingaBuf* rb_notifications);

int defer_fight_boss(Fighter * player, Boss * b, Path * p,
                     foeTurnOption_OP foe_op,
                     Koliseo * kls, RingaBuf* rb_notifications);

int defer_skill_boss(Fighter *player, Boss *b, skillType picked_skill, Path *p, foeTurnOption_OP foe_op,
                     Koliseo *kls, RingaBuf* rb_notifications);

int boss_fight(Fighter * player, Boss * b, Path * p,
               Koliseo * kls, RingaBuf* rb_notifications);

int boss_attack(Boss * b, Fighter * target, Path * p,
                Koliseo * kls, RingaBuf* rb_notifications);
foeTurnOption enemyTurnPick(Enemy * e, Fighter * f);
foeTurnOption bossTurnPick(Boss * b, Fighter * f);
fightResult do_Skill(Fighter * player, Enemy * e, skillType picked_skill, Koliseo * kls);
fightResult do_Skill_boss(Fighter * player, Boss * b, skillType picked_skill, Path * path, Koliseo * kls);

#endif // GAME_FIGHT_H_
