// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2023  jgabaut

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
#include "artifacts.h"

//Artifact functions
/**
 * Takes a Fighter, Enemy and Boss pointers. Uses the Enemy pointer if isBoss is 0;
 * If the active value inside the Artifact at THKSKULL index of artifactsBag array is false, fighter's permboost_def value increases by 4.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @param b The boss involved in the artifact action.
 * @param isBoss Integer describing if effect applies to enemy or boss.
 */
void artifact_thinkingskull(Fighter *f, Enemy *e, Boss *b, int isBoss)
{
    if (!(f->artifactsBag[THKSKULL]->active)) {	//Gives permanent +4 DEF to f
        f->permboost_def += 4;
        f->artifactsBag[THKSKULL]->active = 1;
    }
}

/**
 * Takes a Fighter pointer and an Enemy pointer.
 * If the active value inside the Artifact at TWINKIE index of artifactsBag array is false, and the class value of enemy pointer is Zombie, enemy's xp value is doubled and then increased by its level value.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @param b The boss involved in the artifact action.
 * @param isBoss Integer describing if effect applies to enemy or boss.
 */
void artifact_twinkie(Fighter *f, Enemy *e, Boss *b, int isBoss)
{
    if (!(f->artifactsBag[TWINKIE]->active) && !isBoss) {	//Zombies give double exp
        if (e->class == Zombie) {
            e->xp = e->level + (e->xp * 2);
        }
        f->artifactsBag[TWINKIE]->active = 1;
    }
}

/**
 * Takes a Fighter pointer and an Enemy pointer.
 * If the active value inside the Artifact at WRISTBAND index of artifactsBag array is false, fighter's permboost_enr value increases by 2.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @param b The boss involved in the artifact action.
 * @param isBoss Integer describing if effect applies to enemy or boss.
 */
void artifact_wristband(Fighter *f, Enemy *e, Boss *b, int isBoss)
{
    if (!(f->artifactsBag[WRISTBAND]->active)) {	//Gives permanent +2 ENR to f
        f->permboost_enr += 2;
        f->artifactsBag[WRISTBAND]->active = 1;
    }
}

/**
 * Takes a Fighter pointer and an Enemy pointer.
 * If the active value inside the Artifact at BOARTAIL index of artifactsBag array is false, and the class value of enemy pointer is Boar, enemy's vel value is multiplied by 2/3.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @param b The boss involved in the artifact action.
 * @param isBoss Integer describing if effect applies to enemy or boss.
 */
void artifact_boartail(Fighter *f, Enemy *e, Boss *b, int isBoss)
{
    if (!(f->artifactsBag[BOARTAIL]->active) && !isBoss) {	//Boars get slowed down
        if (e->class == Boar) {
            e->vel /= 1.5;
        }
        f->artifactsBag[BOARTAIL]->active = 1;
    }
}

/**
 * Takes a Fighter pointer and an Enemy pointer.
 * If the active value inside the Artifact at CHAOSORB index of artifactsBag array is false, all the and the level values for the baseenemystats array increase by 2.
 * If the artifact is already active, the same action as before occurs but the increase is by 3 and only if we hit 0 on a 20 roll.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @param b The boss involved in the artifact action.
 * @param isBoss Integer describing if effect applies to enemy or boss.
 */
void artifact_chaosorb(Fighter *f, Enemy *e, Boss *b, int isBoss)
{
    //TODO:
    //Notification window for procs outside of a fight room?
    if (!(f->artifactsBag[CHAOSORB]->active) && f->artifactsBag[CHAOSORB]->innerValue == 0) {	//Every monster levels up twice
        for (int i = 0; i < (ENEMYCLASSESMAX + 1); i++) {
            EnemyBaseStats *base = &baseenemystats[i];
            base->level += 2;
        }
        //red();
        //printf("\t\tYou feel an ominous power spike.\n");
        //white();
        f->artifactsBag[CHAOSORB]->active = 1;
        f->artifactsBag[CHAOSORB]->innerValue += 1;
    } else if (!(f->artifactsBag[CHAOSORB]->active) && (f->artifactsBag[CHAOSORB]->innerValue > 0) && (rand() % 19 == 0)) {	//Every monster levels up 3 times if we roll a 20
        for (int i = 0; i < ENEMYCLASSESMAX + 1; i++) {
            EnemyBaseStats *base = &baseenemystats[i];
            base->level += 3;
        }
        //red();
        //printf("\t\tYou hear screams from behind the enemy.\n");
        //white();
    }
}

/**
 * Takes a Fighter pointer and an Enemy pointer.
 * If the active value inside the Artifact at POWERSYPHON index of artifactsBag array is false, fighter energy value increases by 1.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @param b The boss involved in the artifact action.
 * @param isBoss Integer describing if effect applies to enemy or boss.
 */
void artifact_powersyphon(Fighter *f, Enemy *e, Boss *b, int isBoss)
{
    if (!(f->artifactsBag[POWERSYPHON]->active) && (f->energy < f->totalenergy)) {	//You gain back 1 energy if you're not full already
        f->energy += 1;
        f->artifactsBag[POWERSYPHON]->active = 1;
    }
}

/**
 * Takes a Fighter pointer and an Enemy pointer.
 * If the active value inside the Artifact at GIANTFOSSILE index of artifactsBag array is false, and the class value of enemy pointer is Troll, the enemy hp value is either decreased by 10 if it's at least 11 or set to 1 otherwise.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @param b The boss involved in the artifact action.
 * @param isBoss Integer describing if effect applies to enemy or boss.
 */
void artifact_giantfossile(Fighter *f, Enemy *e, Boss *b, int isBoss)
{
    if (!(f->artifactsBag[GIANTFOSSILE]->active) && !isBoss) {	//Trolls lose some hp
        if (e->class == Troll) {
            if (e->hp > 10) {	//This actual value will mean little since artifacts are called before fighting the enemy
                e->hp -= 10;
            } else {
                e->hp = 1;
            }
        }
        f->artifactsBag[GIANTFOSSILE]->active = 1;
    }
}
