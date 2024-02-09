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
#ifdef HELAPORDO_CURSES_BUILD
#include "helapordo.h"
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
#include "helapordo_raylib.h"
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
#include "specials.h"

#ifdef HELAPORDO_CURSES_BUILD
//Functions for special moves
/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * The enemy pointer hp value is reduced by half of the fighter atk value, then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void knightSpecial_Slash(WINDOW *w, Fighter *f, Enemy *e, Boss *b, Path *p,
                         int roomIndex, int enemyIndex, int isBoss)
{
    char victim[30];
    if (isBoss) {
        strcpy(victim, stringFromBossClass(b->class));
    } else {
        strcpy(victim, stringFromEClass(e->class));
    }
    box(w, 0, 0);
    int y = 2;
    int x = 2;

    mvwprintw(w, y, x, "%s strikes %s.", f->name, victim);

    if (isBoss) {
        e->hp -= f->atk * 0.5;	//Dmg the boss
    } else {
        e->hp -= f->atk * 0.5;	//Dmg the enemy
    }
    f->energy -= costFromSpecial(f->class, KSlash);	//Reduce fighter energy
    y += 1;
    wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    mvwprintw(w, y, x, "%s was deeply hurt.", victim);
    wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * The fighter pointer def value is increased by 10, then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void knightSpecial_Cover(WINDOW *w, Fighter *f, Enemy *e, Boss *b, Path *p,
                         int roomIndex, int enemyIndex, int isBoss)
{
    box(w, 0, 0);
    int y = 2;
    int x = 2;

    mvwprintw(w, y, x, "%s uses his shield to cover.", f->name);

    f->def += 10;		//Raise fighter def
    f->energy -= costFromSpecial(f->class, KCover);	//Reduce fighter energy

    y += 1;

    wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    mvwprintw(w, y, x, "+10 DEF");
    wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * If the fighter pointer def value is at least 11; the def, vel and atk values are respectfully decreased by 10, increased by 10, increased by 2. Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void knightSpecial_Armordrop(WINDOW *w, Fighter *f, Enemy *e, Boss *b, Path *p,
                             int roomIndex, int enemyIndex, int isBoss)
{
    box(w, 0, 0);

    int y = 2;
    int x = 2;

    if (f->def >= 11) {
        mvwprintw(w, y, x, "%s drops his armor to strike faster.", f->name);
        f->def -= 10;
        f->vel += 10;
        f->atk += 2;

        f->energy -= costFromSpecial(f->class, KArmordrop);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "+10 VEL    +2 ATK");
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        y += 1;
        wattron(w, COLOR_PAIR(S4C_RED));
        mvwprintw(w, y, x, "-10 DEF");
        wattroff(w, COLOR_PAIR(S4C_RED));
    } else {
        wattron(w, COLOR_PAIR(S4C_RED));
        mvwprintw(w, y, x, "%s is too exposed to drop his armor.", f->name);
        wattroff(w, COLOR_PAIR(S4C_RED));
    }
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * If the fighter pointer hp value is at least 3 and the enemy hp value is at least 6; they are respectfully lowered by 2 and 5, and the atk value of the fighter is increased by 9. Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void knightSpecial_Berserk(WINDOW *w, Fighter *f, Enemy *e, Boss *b, Path *p,
                           int roomIndex, int enemyIndex, int isBoss)
{
    char victim[30];
    if (isBoss) {
        strcpy(victim, stringFromBossClass(b->class));
    } else {
        strcpy(victim, stringFromEClass(e->class));
    }
    box(w, 0, 0);
    int y = 2;
    int x = 2;
    if (isBoss) {		//Boss
        if (f->hp > 2 && b->hp > 5) {	//Check if both boss and you can lose at least 2 hp...
            mvwprintw(w, y, x, "%s is getting mad at %s!", f->name, victim);
            f->atk += 9;
            b->hp -= 5;		//Deal damage
            f->hp -= 2;		//Deal damage
            f->energy -= costFromSpecial(f->class, KBerserk);	//Reduce fighter energy
            y += 1;
            wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
            mvwprintw(w, y, x, "%s was stunned and hurt!", victim);
            wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        } else if (b->hp <= 2) {
            wattron(w, COLOR_PAIR(S4C_RED));
            mvwprintw(w, y, x, "%s is weak and not impressionable.", victim);
            wattroff(w, COLOR_PAIR(S4C_RED));
        } else if (f->hp <= 2) {
            wattron(w, COLOR_PAIR(S4C_RED));
            mvwprintw(w, y, x, "%s is too weak.", f->name);
            wattroff(w, COLOR_PAIR(S4C_RED));
        }

    } else {			//Enemy
        if (f->hp > 2 && e->hp > 5) {	//Check if both enemy and you can lose at least 2 hp...
            mvwprintw(w, y, x, "%s is getting mad at %s!", f->name, victim);
            f->atk += 9;
            e->hp -= 5;		//Deal damage
            f->hp -= 2;		//Deal damage
            f->energy -= costFromSpecial(f->class, KBerserk);	//Reduce fighter energy
            y += 1;
            wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
            mvwprintw(w, y, x, "%s was stunned and hurt!", victim);
            wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        } else if (e->hp <= 2) {
            wattron(w, COLOR_PAIR(S4C_RED));
            mvwprintw(w, y, x, "%s is weak and not impressionable.", victim);
            wattroff(w, COLOR_PAIR(S4C_RED));
        } else if (f->hp <= 2) {
            wattron(w, COLOR_PAIR(S4C_RED));
            mvwprintw(w, y, x, "%s is too weak.", f->name);
            wattroff(w, COLOR_PAIR(S4C_RED));
        }
    }
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * If the enemy pointer hp value is at least 2 it gets halved, then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void archerSpecial_Headshot(WINDOW *w, Fighter *f, Enemy *e, Boss *b, Path *p,
                            int roomIndex, int enemyIndex, int isBoss)
{
    char victim[30];
    if (isBoss) {
        strcpy(victim, stringFromBossClass(b->class));
    } else {
        strcpy(victim, stringFromEClass(e->class));
    }
    box(w, 0, 0);
    int y = 2;
    int x = 2;

    if (isBoss && b->hp > 1) {	//Check if boss has some hp
        mvwprintw(w, y, x, "%s aims an arrow at %s's head!", f->name, victim);

        b->hp /= 1.75;		//Deal damage
        f->energy -= costFromSpecial(f->class, AHeadshot);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s got headshot!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));

    } else if (e->hp > 1) {
        mvwprintw(w, y, x, "%s aims an arrow at %s's head!", f->name, victim);

        e->hp /= 2;		//Deal damage
        f->energy -= costFromSpecial(f->class, AHeadshot);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s got headshot!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    } else {
        wattron(w, COLOR_PAIR(S4C_RED));
        mvwprintw(w, y, x, "%s is weak already.", victim);
        wattroff(w, COLOR_PAIR(S4C_RED));
    }
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * If the fighter pointer atk value is at least 11, it increases by 8, then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void archerSpecial_Quivercheck(WINDOW *w, Fighter *f, Enemy *e, Boss *b,
                               Path *p, int roomIndex, int enemyIndex,
                               int isBoss)
{
    box(w, 0, 0);
    int y = 2;
    int x = 2;
    mvwprintw(w, y, x, "%s checks its quiver for more arrows.", f->name);
    if (f->atk > 10) {		//Check if atk is high enough
        f->atk += 8;		//Gain atk
        f->energy -= costFromSpecial(f->class, AQuivercheck);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s found stronger arrows inside itself.", f->name);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));

    } else {
        y += 1;
        wattron(w, COLOR_PAIR(S4C_RED));
        mvwprintw(w, y, x, "%s can't help but feel weak.", f->name);
        wattroff(w, COLOR_PAIR(S4C_RED));
    }
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * If the enemy pointer hp value is at least 2, its status value is set to Poison. The count value of enemy's Turncounter related to Poison status gets set to 4. If the enemy hp value is at least 4, it decreases by 3.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void archerSpecial_Poisonshot(WINDOW *w, Fighter *f, Enemy *e, Boss *b, Path *p,
                              int roomIndex, int enemyIndex, int isBoss)
{
    char victim[30];
    if (isBoss) {
        strcpy(victim, stringFromBossClass(b->class));
    } else {
        strcpy(victim, stringFromEClass(e->class));
    }
    box(w, 0, 0);
    int y = 2;
    int x = 2;

    if (isBoss && b->hp > 1) {	//Check if boss has some hp
        if (b->hp >= 4) {
            b->hp -= 3;
        }

        mvwprintw(w, y, x, "%s aims a poisoned arrow at %s!", f->name, victim);

        b->status = Poison;	//Set status to Poison. May need change to manage multiple statuses active at once
        setCounter((Turncounter *) b->counters[POISON], 4);	//Give 4 turns of Poison status
        f->energy -= costFromSpecial(f->class, APoisonshot);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s was hit and poisoned!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));

    } else if (e->hp > 1) {	//Check if enemy has some hp
        if (e->hp >= 4) {
            e->hp -= 3;
        }

        mvwprintw(w, y, x, "%s aims a poisoned arrow at %s!", f->name, victim);

        e->status = Poison;	//Set status to Poison. May need change to manage multiple statuses active at once
        setCounter((Turncounter *) e->counters[POISON], 4);	//Give 4 turns of Poison status
        f->energy -= costFromSpecial(f->class, APoisonshot);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s was hit and poisoned!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));

    } else {
        y += 1;
        wattron(w, COLOR_PAIR(S4C_RED));
        mvwprintw(w, y, x, "%s is weak already.", victim);
        wattroff(w, COLOR_PAIR(S4C_RED));
    }
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * If the enemy pointer hp value is at least 2, its status value is set to Burned. The count value of enemy's Turncounter related to Burned status gets set to 3. If the enemy hp value is at least 5, it decreases by 4.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void archerSpecial_Fireshot(WINDOW *w, Fighter *f, Enemy *e, Boss *b, Path *p,
                            int roomIndex, int enemyIndex, int isBoss)
{
    char victim[30];
    if (isBoss) {
        strcpy(victim, stringFromBossClass(b->class));
    } else {
        strcpy(victim, stringFromEClass(e->class));
    }
    box(w, 0, 0);
    int y = 2;
    int x = 2;

    if (isBoss && b->hp > 1) {	//Check if boss has some hp
        if (b->hp >= 5) {
            b->hp -= 4;
        }

        mvwprintw(w, y, x, "%s lights an arrow on fire and shoots at %s's!",
                  f->name, victim);

        b->status = Burned;	//Set status to Burned. May need change to manage multiple statuses active at once
        setCounter((Turncounter *) b->counters[BURNED], 3);	//Give 3 turns of Burned status
        f->energy -= costFromSpecial(f->class, AFireshot);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s was hit and set on fire!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    } else if (e->hp > 1) {	//Check if enemy has some hp
        if (e->hp >= 5) {
            e->hp -= 4;
        }

        mvwprintw(w, y, x, "%s lights an arrow on fire and shoots at %s's!",
                  f->name, victim);

        e->status = Burned;	//Set status to Burned. May need change to manage multiple statuses active at once
        setCounter((Turncounter *) e->counters[BURNED], 3);	//Give 3 turns of Burned status
        f->energy -= costFromSpecial(f->class, AFireshot);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s was hit and set on fire!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    } else {
        wattron(w, COLOR_PAIR(S4C_RED));
        mvwprintw(w, y, x, "%s's is weak already.", victim);
        wattroff(w, COLOR_PAIR(S4C_RED));
    }
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * If the fighter pointer luck value is less than MAXPLAYERLUCK - 6, it increases by 5.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void mageSpecial_Fatewarp(WINDOW *w, Fighter *f, Enemy *e, Boss *b, Path *p,
                          int roomIndex, int enemyIndex, int isBoss)
{
    box(w, 0, 0);
    int y = 2;
    int x = 2;
    if (f->luck < MAXPLAYERLUCK - 6) {
        mvwprintw(w, y, x, "%s warps fate to fulfill its destiny.", f->name);
        f->luck += 5;

        f->energy -= costFromSpecial(f->class, MFatewarp);	//Reduce fighter energy
        y += 2;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "+5 LUCK");
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    } else {
        wattron(w, COLOR_PAIR(S4C_RED));
        mvwprintw(w, y, x, "%s's fate is bright already.", f->name);
        wattroff(w, COLOR_PAIR(S4C_RED));
    }
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * The fighter pointer atk value increases by 5.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void mageSpecial_Powerup(WINDOW *w, Fighter *f, Enemy *e, Boss *b, Path *p,
                         int roomIndex, int enemyIndex, int isBoss)
{
    box(w, 0, 0);
    int y = 2;
    int x = 2;
    mvwprintw(w, y, x, "%s channels its energy.", f->name);
    f->atk += 5;

    f->energy -= costFromSpecial(f->class, MPowerup);	//Reduce fighter energy
    y += 1;
    wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    mvwprintw(w, y, x, "+5 ATK");
    wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * If the enemy pointer hp value is at least 2, it either decreases by 10 if it's greater than 10 or get set to 1.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void mageSpecial_Spellstrike(WINDOW *w, Fighter *f, Enemy *e, Boss *b, Path *p,
                             int roomIndex, int enemyIndex, int isBoss)
{
    char victim[30];
    if (isBoss) {
        strcpy(victim, stringFromBossClass(b->class));
    } else {
        strcpy(victim, stringFromEClass(e->class));
    }
    box(w, 0, 0);
    int y = 2;
    int x = 2;
    if (isBoss && b->hp > 1) {	//Check if boss has some hp
        if (b->hp >= 11) {
            b->hp -= 10;	//Deal damage
        } else if (b->hp > 1) {
            b->hp = 1;		//Set enemy hp to 1
        }

        mvwprintw(w, y, x, "%s casts a deadly spell on %s!", f->name, victim);

        f->energy -= costFromSpecial(f->class, MSpellstrike);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s feels the spell!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));

    } else if (e->hp > 1) {	//Check if enemy has some hp
        if (e->hp >= 11) {
            e->hp -= 10;	//Deal damage
        } else if (e->hp > 1) {
            e->hp = 1;		//Set enemy hp to 1
        }

        mvwprintw(w, y, x, "%s casts a deadly spell on %s!", f->name, victim);

        f->energy -= costFromSpecial(f->class, MSpellstrike);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s feels the spell!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    } else {
        wattron(w, COLOR_PAIR(S4C_RED));
        mvwprintw(w, y, x, "%s's is weak already.", victim);
        wattroff(w, COLOR_PAIR(S4C_RED));
    }
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * If the enemy pointer hp value is at least 2, its status value is set to Burned. The count value of enemy's Turncounter related to Burned status gets set to 3. If the enemy hp value is at least 5, it decreases by 4.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void mageSpecial_Flamering(WINDOW *w, Fighter *f, Enemy *e, Boss *b, Path *p,
                           int roomIndex, int enemyIndex, int isBoss)
{
    char victim[30];
    if (isBoss) {
        strcpy(victim, stringFromBossClass(b->class));
    } else {
        strcpy(victim, stringFromEClass(e->class));
    }
    box(w, 0, 0);
    int y = 2;
    int x = 2;

    if (isBoss && b->hp > 1) {	//Check if boss has some hp
        if (b->hp >= 5) {
            b->hp -= 4;
        }

        mvwprintw(w, y, x, "%s summons a ring of fire around %s!", f->name,
                  victim);

        b->status = Burned;	//Set status to Burned. May need change to manage multiple statuses active at once
        setCounter((Turncounter *) b->counters[BURNED], 3);	//Give 3 turns of Burned status
        f->energy -= costFromSpecial(f->class, MFlamering);	//Reduce fighter energy
        y += 2;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s was set on fire!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));

    } else if (e->hp > 1) {	//Check if enemy has some hp
        if (e->hp >= 5) {
            e->hp -= 4;
        }

        mvwprintw(w, y, x, "%s summons a ring of fire around %s!", f->name,
                  victim);

        e->status = Burned;	//Set status to Burned. May need change to manage multiple statuses active at once
        setCounter((Turncounter *) e->counters[BURNED], 3);	//Give 3 turns of Burned status
        f->energy -= costFromSpecial(f->class, MFlamering);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s was set on fire!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    } else {
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s's is weak already.", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    }
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * If the enemy pointer hp value is at least 2 and the def value is at least 5, the def value is set to 0.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void assassinSpecial_Grimdagger(WINDOW *w, Fighter *f, Enemy *e, Boss *b,
                                Path *p, int roomIndex, int enemyIndex,
                                int isBoss)
{
    char victim[30];
    if (isBoss) {
        strcpy(victim, stringFromBossClass(b->class));
    } else {
        strcpy(victim, stringFromEClass(e->class));
    }
    box(w, 0, 0);
    int y = 2;
    int x = 2;

    if (isBoss && b->hp > 1 && b->def >= 5) {	//Check if boss has some hp and defense
        mvwprintw(w, y, x, "%s throws its dagger at %s's heart.", f->name,
                  victim);
        b->def = 0;		//Set boss def

        f->energy -= costFromSpecial(f->class, XGrimdagger);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s is defenseless!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    } else if (e->hp > 1 && e->def >= 5) {	//Check if enemy has some hp and defense

        mvwprintw(w, y, x, "%s throws its dagger at %s's heart.", f->name,
                  victim);
        e->def = 0;		//Set enemy def

        f->energy -= costFromSpecial(f->class, XGrimdagger);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s is defenseless!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    } else {
        wattron(w, COLOR_PAIR(S4C_RED));
        mvwprintw(w, y, x, "%s's is weak already.", victim);
        wattroff(w, COLOR_PAIR(S4C_RED));
    }
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * If the enemy pointer hp value is at least 6 and the fighter pointer hp value is equal or lower than its totalhp value, the enemy hp decreases by 5 and the fighter hp increases by the same amount.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void assassinSpecial_Leechknife(WINDOW *w, Fighter *f, Enemy *e, Boss *b,
                                Path *p, int roomIndex, int enemyIndex,
                                int isBoss)
{
    char victim[30];
    if (isBoss) {
        strcpy(victim, stringFromBossClass(b->class));
    } else {
        strcpy(victim, stringFromEClass(e->class));
    }
    box(w, 0, 0);
    int y = 2;
    int x = 2;
    if (isBoss && b->hp >= 6) {	//Check if boss has some hp
        if (f->hp <= f->totalhp - 6) {	//Check if fighter has some space in his healthbar

            mvwprintw(w, y, x, "%s strikes %s with its vampire knife.", f->name,
                      victim);
            f->hp += 5;
            b->hp -= 5;

            f->energy -= costFromSpecial(f->class, XLeechknife);	//Reduce fighter energy
            y += 1;
            wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
            mvwprintw(w, y, x, "%s's health was sapped!", victim);
            wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        } else {
            wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
            mvwprintw(w, y, x, "%s is feeling good enough.", f->name);
            wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        }

    } else if (e->hp >= 6) {	//Check if enemy has some hp
        if (f->hp <= f->totalhp - 6) {	//Check if fighter has some space in his healthbar

            mvwprintw(w, y, x, "%s strikes %s with its vampire knife.", f->name,
                      victim);
            f->hp += 5;
            e->hp -= 5;

            f->energy -= costFromSpecial(f->class, XLeechknife);	//Reduce fighter energy
            y += 1;
            wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
            mvwprintw(w, y, x, "%s's health was sapped!", victim);
            wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        } else {
            wattron(w, COLOR_PAIR(S4C_BRIGHT_YELLOW));
            mvwprintw(w, y, x, "%s is feeling good enough.", f->name);
            wattroff(w, COLOR_PAIR(S4C_BRIGHT_YELLOW));
        }
    } else {
        wattron(w, COLOR_PAIR(S4C_BRIGHT_YELLOW));
        mvwprintw(w, y, x, "%s's is too weak.", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_YELLOW));
    }
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * If the fighter pointer def value is at least 8 and the vel value is at least 3, the def value increases by 8 and the vel value decreases by 2.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void assassinSpecial_Disguise(WINDOW *w, Fighter *f, Enemy *e, Boss *b, Path *p,
                              int roomIndex, int enemyIndex, int isBoss)
{
    box(w, 0, 0);
    int y = 2;
    int x = 2;
    if (f->def >= 8 && f->vel >= 3) {
        mvwprintw(w, y, x, "%s disguises himself as a %s.", f->name,
                  stringFromEClass(e->class));
        f->def += 8;
        f->vel -= 2;
        f->energy -= costFromSpecial(f->class, XDisguise);	//Reduce fighter energy

        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "+8 DEF");
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        y += 1;
        wattron(w, COLOR_PAIR(S4C_RED));
        mvwprintw(w, y, x, "-2 VEL");
        wattroff(w, COLOR_PAIR(S4C_RED));
    } else {
        wattron(w, COLOR_PAIR(S4C_RED));
        mvwprintw(w, y, x, "%s is too weak to disguise.", f->name);
        wattroff(w, COLOR_PAIR(S4C_RED));
    }
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * A WINDOW pointer is passed to print the messages onto.
 * If the enemy pointer hp value is at least 2, its status value is set to Poison. The count value of enemy's Turncounter related to Poison status gets set to 4. If the enemy hp value is at least 4, it decreases by 3.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param w The WINDOW to print to.
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param b The Boss receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Integer describing is the effect applies to Enemy or Boss.
 */
void assassinSpecial_Venomblade(WINDOW *w, Fighter *f, Enemy *e, Boss *b,
                                Path *p, int roomIndex, int enemyIndex,
                                int isBoss)
{
    char victim[30];
    if (isBoss) {
        strcpy(victim, stringFromBossClass(b->class));
    } else {
        strcpy(victim, stringFromEClass(e->class));
    }
    box(w, 0, 0);
    int y = 2;
    int x = 2;

    if (isBoss && b->hp > 1) {	//Check if boss has some hp
        if (b->hp >= 4) {
            b->hp -= 3;
        }

        mvwprintw(w, y, x, "%s throws venomous daggers at %s!", f->name,
                  victim);

        b->status = Poison;	//Set status to Poison. May need change to manage multiple statuses active at once
        setCounter((Turncounter *) b->counters[POISON], 4);	//Give 4 turns of Poison status
        f->energy -= costFromSpecial(f->class, XVenomblade);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s was hit and poisoned!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    } else if (e->hp > 1) {	//Check if enemy has some hp
        if (e->hp >= 4) {
            e->hp -= 3;
        }

        mvwprintw(w, y, x, "%s throws venomous daggers at %s!", f->name,
                  victim);

        e->status = Poison;	//Set status to Poison. May need change to manage multiple statuses active at once
        setCounter((Turncounter *) e->counters[POISON], 4);	//Give 4 turns of Poison status
        f->energy -= costFromSpecial(f->class, XVenomblade);	//Reduce fighter energy
        y += 1;
        wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
        mvwprintw(w, y, x, "%s was hit and poisoned!", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
    } else {
        wattron(w, COLOR_PAIR(S4C_BRIGHT_YELLOW));
        mvwprintw(w, y, x, "%s's is weak already.", victim);
        wattroff(w, COLOR_PAIR(S4C_BRIGHT_YELLOW));
    }
}

/**
 * Takes a Fighter pointer and prepares its specialSlot fields by allocating SPECIALSMAX slots.
 * Special slots are initialised.
 * The specific move assigned is determined using the ordering of both fighterClass values and SPECIALSMAX.
 * @see Fighter
 * @see Specialslot
 * @see SPECIALSMAX
 * @see REGISTER_CALLBACK()
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param kls The Koliseo used for allocations.
 * @param f The Fighter pointer whose special slots will be initialised.
 */
void setSpecials(Fighter *f, Koliseo *kls)
{
    char movename[80];
    char movedesc[80];
    for (int i = 0; i <= SPECIALSMAX; i++) {
        kls_log(kls, "DEBUG", "Prepping Specialslot (%i)", i);
        Specialslot *s =
            (Specialslot *) KLS_PUSH_TYPED(kls, Specialslot, HR_Specialslot,
                                           "Specialslot", "Specialslot");
        s->enabled = 0;
        s->move = i + (f->class * (SPECIALSMAX + 1));	// Assign the i-th move offsetting by classNum * specialsMax
        s->cost = costFromSpecial(f->class, i);
        strcpy(movename, nameStringFromSpecial(f->class, i));
        strcpy(movedesc, descStringFromSpecial(f->class, i));
        //printf("DEBUG\n%i\t%s\n",(i+ (f->class * (SPECIALSMAX + 1))),stringFromSpecial(f->class,i));
        strcpy(s->name, movename);
        strcpy(s->desc, movedesc);
        f->specials[i] = s;
    };
}
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else

void knightSpecial_Slash(Rectangle * win, Fighter * f, Enemy * e, Boss * b, Path * p,
                         int roomIndex, int enemyIndex, int isBoss)
{

}

void knightSpecial_Cover(Rectangle * win, Fighter * f, Enemy * e, Boss * b, Path * p,
                         int roomIndex, int enemyIndex, int isBoss)
{

}

void knightSpecial_Armordrop(Rectangle * win, Fighter * f, Enemy * e, Boss * b,
                             Path * p, int roomIndex, int enemyIndex,
                             int isBoss)
{

}

void knightSpecial_Berserk(Rectangle * win, Fighter * f, Enemy * e, Boss * b,
                           Path * p, int roomIndex, int enemyIndex, int isBoss)
{

}
void archerSpecial_Headshot(Rectangle * win, Fighter * f, Enemy * e, Boss * b,
                            Path * p, int roomIndex, int enemyIndex,
                            int isBoss)
{

}
void archerSpecial_Quivercheck(Rectangle * win, Fighter * f, Enemy * e, Boss * b,
                               Path * p, int roomIndex, int enemyIndex,
                               int isBoss)
{

}
void archerSpecial_Poisonshot(Rectangle * win, Fighter * f, Enemy * e, Boss * b,
                              Path * p, int roomIndex, int enemyIndex,
                              int isBoss)
{

}
void archerSpecial_Fireshot(Rectangle * win, Fighter * f, Enemy * e, Boss * b,
                            Path * p, int roomIndex, int enemyIndex,
                            int isBoss)
{

}
void mageSpecial_Fatewarp(Rectangle * win, Fighter * f, Enemy * e, Boss * b,
                          Path * p, int roomIndex, int enemyIndex, int isBoss)
{

}
void mageSpecial_Powerup(Rectangle * win, Fighter * f, Enemy * e, Boss * b, Path * p,
                         int roomIndex, int enemyIndex, int isBoss)
{

}
void mageSpecial_Spellstrike(Rectangle * win, Fighter * f, Enemy * e, Boss * b,
                             Path * p, int roomIndex, int enemyIndex,
                             int isBoss)
{

}
void mageSpecial_Flamering(Rectangle * win, Fighter * f, Enemy * e, Boss * b,
                           Path * p, int roomIndex, int enemyIndex, int isBoss)
{

}
void assassinSpecial_Grimdagger(Rectangle * win, Fighter * f, Enemy * e, Boss * b,
                                Path * p, int roomIndex, int enemyIndex,
                                int isBoss)
{

}
void assassinSpecial_Leechknife(Rectangle * win, Fighter * f, Enemy * e, Boss * b,
                                Path * p, int roomIndex, int enemyIndex,
                                int isBoss)
{

}
void assassinSpecial_Disguise(Rectangle * win, Fighter * f, Enemy * e, Boss * b,
                              Path * p, int roomIndex, int enemyIndex,
                              int isBoss)
{

}
void assassinSpecial_Venomblade(Rectangle * win, Fighter * f, Enemy * e, Boss * b,
                                Path * p, int roomIndex, int enemyIndex,
                                int isBoss)
{

}
void setSpecials(Fighter *f, Koliseo *kls)
{

}
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
