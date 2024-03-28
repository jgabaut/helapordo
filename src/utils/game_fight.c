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
#include "game_fight.h"

#ifdef HELAPORDO_CURSES_BUILD
/**
 * Takes a Fighter and a Enemy pointers and compares their stats to determine who gets damaged and returns the fightStatus value.
 * Prints notifications to the passed WINDOW pointer.
 * On enemy death, there's a chance to call dropConsumable, dropEquip or dropArtifact (guaranteed for beast enemies).
 * @see Fighter
 * @see Enemy
 * @see statReset()
 * @see statResetEnemy()
 * @see stringFromEClass()
 * @see dropConsumable()
 * @see dropEquip()
 * @see dropArtifact()
 * @param player The Fighter pointer at hand.
 * @param e The Enemy pointer at hand.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 * @see display_notification()
 */
int fight(Fighter *player, Enemy *e, WINDOW *notify_win, Koliseo *kls)
{

    fightResult res = FIGHTRES_NO_DMG;
    char msg[200];
    //Stat comparisons
    //

    int atkdelta = (player->atk + player->equipboost_atk) - e->atk - (hlpd_rand() % 3) - 1;	//Skewed with defender
    int defdelta = (player->def + player->equipboost_def) - e->def + (hlpd_rand() % 2) + 1;	//Skewed with attacker
    int veldelta =
        (player->vel + player->equipboost_vel) - e->vel + (hlpd_rand() % 3) + 1;

    int atkOnPlayer =
        e->atk - (player->def + player->equipboost_def + (player->vel / 6));
    int atkOnEnemy =
        (player->atk + player->equipboost_atk) - (e->def + (e->vel / 6));

    if (G_GODMODE_ON == 1) {
        log_tag("debug_log.txt", "[DEBUG]", "fight():  G_GODMODE_ON == 1");
        atkdelta = 100;
        defdelta = 100;
        veldelta = 100;
        atkOnPlayer = 1;
        atkOnEnemy = 100;
    }

    int damageDealt = -1;
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkdelta %i", atkdelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "defdelta %i", defdelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "veldelta %i", veldelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkOnEnemy %i", atkOnEnemy);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkOnPlayer %i", atkOnPlayer);

    if (veldelta >= 0) {

        if (atkOnEnemy > 3) {
            damageDealt = atkOnEnemy;
            e->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result A WIN (faster, great atk).");
        } else if (atkOnEnemy >= 0) {
            damageDealt = abs(atkOnEnemy - atkdelta);
            e->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result B WIN (faster, ok atk).");
        } else {
            if (atkOnEnemy > -3) {
                damageDealt =
                    fabsf(atkOnPlayer -
                          0.75F * (player->vel + player->equipboost_vel));
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result C1 LOST (faster, atk > -3).");
            } else {
                damageDealt = abs(atkOnPlayer - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result C2 LOST (faster, atk < -3).");
            }
            player->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
        }
    } else {
        atkdelta = -atkdelta;
        if (atkOnPlayer > 3) {
            damageDealt = atkOnPlayer;
            player->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result D LOST (slower, great enemy atk).");
        } else if (atkOnPlayer >= 0) {
            damageDealt = abs(atkOnPlayer - atkdelta);
            player->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result E LOST (slower, ok enemy atk).");
        } else {
            if (atkOnPlayer > -3) {
                damageDealt = fabsf(atkOnEnemy - 0.75F * e->vel);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result F1 WIN (slower, enemy atk > -3).");
            } else {
                damageDealt = abs(atkOnEnemy - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result F2 WIN (slower, enemy atk < -3).");
            }
            e->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
        }
    }
    log_tag("debug_log.txt", "[FIGHT]", "damageCalc %i", damageDealt);

    int yourhit = (res == FIGHTRES_DMG_DEALT) ? 1 : 0;
    char victim[25];

    if (!yourhit) {

        e->vel--;
        e->atk--;
        e->def -= 2;

        //Check if someone earned a stat reset after the fight
        statReset(player, 0);
        statResetEnemy(e, 0);

        strcpy(victim, player->name);
    } else {

        player->vel--;
        player->atk--;
        player->def -= 2;

        //Account for vampirism perk
        int vampire_perks = player->perks[VAMPIRISM]->innerValue;
        if (vampire_perks > 0) {
            int recovery = floor(damageDealt * (0.1 * vampire_perks));
            player->hp += recovery;
            log_tag("debug_log.txt", "[PERKS]", "Vampirism proc for +%i HP.",
                    recovery);
            if (player->hp >= player->totalhp) {
                player->hp = player->totalhp;
            };
        }
        //Account for burn on touch perk
        int hotbody_perks = player->perks[HOT_BODY]->innerValue;
        if (hotbody_perks > 0) {
            int burnchance = 11 - hotbody_perks;
            if (hlpd_rand() % burnchance == 0) {
                //TODO
                //Handle multiple statuses
                e->status = Burned;	//Set status to Burned. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) e->counters[Burned], 2);	//Give 2 turns of Burned status
                log_tag("debug_log.txt", "[PERKS]",
                        "Hotbody proc on 1/%i chance.", burnchance);
            }
        }
        //Account for poison on touch perk. Order of checks with hot_body perk may cause issues?
        int biohazard_perks = player->perks[BIOHAZARD]->innerValue;
        if (biohazard_perks > 0) {
            int poisonchance = 11 - biohazard_perks;
            if (hlpd_rand() % poisonchance == 0) {
                e->status = Poison;	//Set status to Poison. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) e->counters[POISON], 2);	//Give 2 turns of Poison status
                log_tag("debug_log.txt", "[PERKS]",
                        "Biohazard proc on 1/%i chance.", poisonchance);
            }
        }
        //Check if someone earned a stat reset after the fight
        statResetEnemy(e, 0);
        statReset(player, 0);

        strcpy(victim, stringFromEClass(e->class));
    }

    int color = -1;
    if (yourhit) {
        color = S4C_WHITE;
    } else {
        color = S4C_RED;
    }

    sprintf(msg, "%s was hit.    (%i DMG)", victim,
            damageDealt > 0 ? damageDealt : 1);
    wattron(notify_win, COLOR_PAIR(color));
    display_notification(notify_win, msg, 500);
    wattroff(notify_win, COLOR_PAIR(color));

    //Rolls
    //
    //Critical hit roll

    //Account for critboost_chance perks
    int critboost_value = 1.5 * player->perks[CRITBOOST_CHANCE]->innerValue;
    int critMax = round(10.0 - floor(player->luck / 5) - (critboost_value));

    int critRes = (hlpd_rand() % critMax);

    if (res == FIGHTRES_DMG_DEALT && (critRes <= 0)) {

        //Account for critboost_dmg perks
        int dmgboost_perks = player->perks[CRITBOOST_DMG]->innerValue;
        damageDealt *= (0.30 + (0.12 * dmgboost_perks));
        e->hp -= (damageDealt > 0 ? damageDealt : 1);
        log_tag("debug_log.txt", "[FIGHT]",
                "Critical hit for %i dmg, proc on 1/%i chance.", damageDealt,
                critMax);
        log_tag("debug_log.txt", "[PERKS]", "Critical hit, critboost was %i.",
                critboost_value);

        sprintf(msg, "A critical hit!    (%i DMG)",
                damageDealt > 0 ? damageDealt : 1);
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        //Update stats
        player->stats->criticalhits++;
    }
    //Check for deaths -> exit condition from loop
    //
    //
    //
    if (e->hp <= 0) {
        res = FIGHTRES_KILL_DONE;

        //Account for runic circle perk
        int runic_perks = player->perks[RUNIC_MAGNET]->innerValue;
        if (runic_perks > 0) {
            int recovery = round(0.51 * runic_perks);
            player->energy += recovery;
            log_tag("debug_log.txt", "[PERKS]",
                    "Runicmagnet proc for %i energy.", recovery);
        }
        if (e->beast) {
            color = S4C_MAGENTA;
        } else {
            color = S4C_RED;
        }
        wattron(notify_win, COLOR_PAIR(color));
        sprintf(msg, "%s fainted.", stringFromEClass(e->class));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(color));

        log_tag("debug_log.txt", "[FIGHT]", "Killed  %s.",
                stringFromEClass(e->class));

        //Update stats
        player->stats->enemieskilled++;
    } else {
        //Apply status effects to enemy
        if (e->status != Normal) {
            applyEStatus(notify_win, e);
            log_tag("debug_log.txt", "[STATUS]", "Applied  %s to %s.",
                    stringFromStatus(e->status), stringFromEClass(e->class));
        }
    }

    if (player->hp <= 0) {
        res = FIGHTRES_DEATH;
    } else {
        //Apply status effects to player
        if (player->status != Normal) {
            applyStatus(notify_win, player);
        }
    }

    //Consumable drop, guaranteed on killing a beast
    if (res == FIGHTRES_KILL_DONE
        && (e->beast || ((hlpd_rand() % 9) - (player->luck / 10) <= 0))) {
        int drop = dropConsumable(player);
        sprintf(msg, "You found a %s!", stringFromConsumables(drop));
        wattron(notify_win, COLOR_PAIR(S4C_CYAN));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_CYAN));
        log_tag("debug_log.txt", "[DROPS]", "Found Consumable:    %s.",
                stringFromConsumables(drop));
    }

    //Artifact drop (if we don't have all of them), guaranteed on killing a beast
    if ((player->stats->artifactsfound != ARTIFACTSMAX + 1)
        && res == FIGHTRES_KILL_DONE && (e->beast
                                         ||
                                         ((hlpd_rand() % ENEMY_ARTIFACTDROP_CHANCE) -
                                          (player->luck / 10) <= 0))) {
        int artifact_drop = dropArtifact(player);
        sprintf(msg, "You found a %s!", stringFromArtifacts(artifact_drop));
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        log_tag("debug_log.txt", "[DROPS]", "Found Artifact:    %s.",
                stringFromArtifacts(artifact_drop));
        if (!e->beast)
            log_tag("debug_log.txt", "[.1%% CHANCE]",
                    "\nNORMAL ENEMY DROPPED ARTIFACT! 0.1%% chance??\n");
    }

    //Equip drop, guaranteed on killing a beast
    if (res == FIGHTRES_KILL_DONE
        && (e->beast || ((hlpd_rand() % 15) - (player->luck / 10) <= 0))) {
        dropEquip(player, e->beast, notify_win, kls);
    }
    return res;
}

/**
 * Takes an Enemy and a Fighter pointer and compares their stats to determine who gets damaged and returns the fightStatus value.
 * Prints notifications to the passed WINDOW pointer.
 * On enemy death, there's a chance to call dropConsumable, dropEquip or dropArtifact (guaranteed for beast enemies).
 * NOTE:  that the return values are always from the POV of the Fighter: FIGHTRES_DMG_DEALT means the Enemy was damaged!
 * @see defer_fight_enemy()
 * @see Fighter
 * @see Enemy
 * @see statReset()
 * @see statResetEnemy()
 * @see stringFromEClass()
 * @see dropConsumable()
 * @see dropEquip()
 * @see dropArtifact()
 * @param player The Fighter pointer at hand.
 * @param e The Enemy pointer at hand.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 * @see display_notification()
 */
int enemy_attack(Enemy *e, Fighter *target, WINDOW *notify_win, Koliseo *kls)
{
    //Implementation similar to fight(), as a base idea
    //Should return fightResult values, while keeping the perspective on the Fighter, as in:
    //
    //      FIGHTRES_DEATH means the Fighter died
    //      FIGHTRES_KILL_DONE means the Enemy died
    //      FIGHTRES_DMG_DEALT means the Fighter inflicted damage
    //      FIGHRES_DMG_TAKEN means the Fighter received damage

    fightResult res = FIGHTRES_NO_DMG;
    char msg[200];
    //Stat comparisons
    //

    int atkdelta = (e->atk + e->turnboost_atk) - (target->atk + target->equipboost_atk + target->turnboost_atk - (hlpd_rand() % 3)) - 1;	//Skewed with defender
    int defdelta = (e->def + e->turnboost_def) - (target->def + target->equipboost_def + target->turnboost_def) + (hlpd_rand() % 2) + 1;	//Skewed with attacker
    int veldelta =
        (e->vel + e->turnboost_vel) - (target->vel + target->equipboost_vel +
                                       target->turnboost_vel) + (hlpd_rand() % 3) +
        1;

    int atkOnPlayer =
        (e->atk + e->turnboost_atk) - (target->def + target->equipboost_def +
                                       target->turnboost_def +
                                       (target->vel / 6));
    int atkOnEnemy =
        (target->atk + target->equipboost_atk + target->turnboost_atk) -
        (e->def + e->turnboost_def + (e->vel / 6));

    if (G_GODMODE_ON == 1) {
        log_tag("debug_log.txt", "[DEBUG]", "[%s]:  G_GODMODE_ON == 1",
                __func__);
        atkdelta = -100;
        defdelta = -100;
        veldelta = -100;
        atkOnPlayer = 1;
        atkOnEnemy = 100;
    }

    int damageDealt = -1;
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkdelta %i", atkdelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "defdelta %i", defdelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "veldelta %i", veldelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkOnEnemy %i", atkOnEnemy);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkOnPlayer %i", atkOnPlayer);

    if (veldelta >= 0) {	//Enemy has a non-negative veldelta
        if (atkOnPlayer > 3) {
            damageDealt = atkOnPlayer;
            target->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "[%s]:  Fight result D LOST (slower, great enemy atk).",
                    __func__);
        } else if (atkOnPlayer >= 0) {
            damageDealt = abs(atkOnPlayer - atkdelta);
            target->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "[%s]:  Fight result E LOST (slower, ok enemy atk).",
                    __func__);
        } else {
            if (atkOnPlayer > -3) {
                damageDealt = fabsf(atkOnEnemy - 0.75F * e->vel);
                log_tag("debug_log.txt", "[FIGHT]",
                        "[%s]:  Fight result F1 WIN (slower, enemy atk > -3).",
                        __func__);
            } else {
                damageDealt = abs(atkOnEnemy - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "[%s]:  Fight result F2 WIN (slower, enemy atk < -3).",
                        __func__);
            }
            e->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
        }
    } else {			//Enemy veldelta is not strictly positive
        atkdelta = -atkdelta;
        if (atkOnEnemy > 3) {
            damageDealt = atkOnEnemy;
            e->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "[%s]:  Fight result A WIN (faster, great atk).", __func__);
        } else if (atkOnEnemy >= 0) {
            damageDealt = abs(atkOnEnemy - atkdelta);
            e->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "[%s]:  Fight result B WIN (faster, ok atk).", __func__);
        } else {
            if (atkOnEnemy > -3) {
                damageDealt =
                    fabsf(atkOnPlayer -
                          0.75F * (target->vel + target->equipboost_vel));
                log_tag("debug_log.txt", "[FIGHT]",
                        "[%s]:  Fight result C1 LOST (faster, atk > -3).",
                        __func__);
            } else {
                damageDealt = abs(atkOnPlayer - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "[%s]:  Fight result C2 LOST (faster, atk < -3).",
                        __func__);
            }
            target->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
        }
    }
    log_tag("debug_log.txt", "[FIGHT]", "[%s]:  damageCalc %i", __func__,
            damageDealt);

    int playerhit = (res == FIGHTRES_DMG_DEALT) ? 1 : 0;
    char victim[25];

    if (!playerhit) {

        e->vel--;
        e->atk--;
        e->def -= 2;

        //Check if someone earned a stat reset after the fight
        statReset(target, 0);
        statResetEnemy(e, 0);

        strcpy(victim, target->name);
    } else {

        target->vel--;
        target->atk--;
        target->def -= 2;

        //Account for vampirism perk
        int vampire_perks = target->perks[VAMPIRISM]->innerValue;
        if (vampire_perks > 0) {
            int recovery = floor(damageDealt * (0.1 * vampire_perks));
            target->hp += recovery;
            log_tag("debug_log.txt", "[PERKS]", "Vampirism proc for +%i HP.",
                    recovery);
            if (target->hp >= target->totalhp) {
                target->hp = target->totalhp;
            };
        }
        //Account for burn on touch perk
        int hotbody_perks = target->perks[HOT_BODY]->innerValue;
        if (hotbody_perks > 0) {
            int burnchance = 11 - hotbody_perks;
            if (hlpd_rand() % burnchance == 0) {
                //TODO
                //Handle multiple statuses
                e->status = Burned;	//Set status to Burned. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) e->counters[Burned], 2);	//Give 2 turns of Burned status
                log_tag("debug_log.txt", "[PERKS]",
                        "Hotbody proc on 1/%i chance.", burnchance);
            }
        }
        //Account for poison on touch perk. Order of checks with hot_body perk may cause issues?
        int biohazard_perks = target->perks[BIOHAZARD]->innerValue;
        if (biohazard_perks > 0) {
            int poisonchance = 11 - biohazard_perks;
            if (hlpd_rand() % poisonchance == 0) {
                e->status = Poison;	//Set status to Poison. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) e->counters[POISON], 2);	//Give 2 turns of Poison status
                log_tag("debug_log.txt", "[PERKS]",
                        "Biohazard proc on 1/%i chance.", poisonchance);
            }
        }
        //Check if someone earned a stat reset after the fight
        statResetEnemy(e, 0);
        statReset(target, 0);

        strcpy(victim, stringFromEClass(e->class));
    }

    int color = -1;
    if (playerhit) {
        color = S4C_WHITE;
    } else {
        color = S4C_RED;
    }

    sprintf(msg, "%s was hit.    (%i DMG)", victim,
            damageDealt > 0 ? damageDealt : 1);
    wattron(notify_win, COLOR_PAIR(color));
    display_notification(notify_win, msg, 500);
    wattroff(notify_win, COLOR_PAIR(color));

    //Rolls
    //
    //Critical hit roll

    //Account for critboost_chance perks
    int critboost_value = 1.5 * target->perks[CRITBOOST_CHANCE]->innerValue;
    int critMax = round(10.0 - floor(target->luck / 5) - (critboost_value));

    int critRes = (hlpd_rand() % critMax);

    if (res == FIGHTRES_DMG_DEALT && (critRes <= 0)) {

        //Account for critboost_dmg perks
        int dmgboost_perks = target->perks[CRITBOOST_DMG]->innerValue;
        damageDealt *= (0.30 + (0.12 * dmgboost_perks));
        e->hp -= (damageDealt > 0 ? damageDealt : 1);
        log_tag("debug_log.txt", "[FIGHT]",
                "Critical hit for %i dmg, proc on 1/%i chance.", damageDealt,
                critMax);
        log_tag("debug_log.txt", "[PERKS]", "Critical hit, critboost was %i.",
                critboost_value);

        sprintf(msg, "A critical hit!    (%i DMG)",
                damageDealt > 0 ? damageDealt : 1);
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        //Update stats
        target->stats->criticalhits++;
    }
    //Check for deaths -> exit condition from loop
    //
    //
    //
    if (e->hp <= 0) {
        res = FIGHTRES_KILL_DONE;

        //Account for runic circle perk
        int runic_perks = target->perks[RUNIC_MAGNET]->innerValue;
        if (runic_perks > 0) {
            int recovery = round(0.51 * runic_perks);
            target->energy += recovery;
            log_tag("debug_log.txt", "[PERKS]",
                    "Runicmagnet proc for %i energy.", recovery);
        }
        if (e->beast) {
            color = S4C_MAGENTA;
        } else {
            color = S4C_RED;
        }
        wattron(notify_win, COLOR_PAIR(color));
        sprintf(msg, "%s fainted.", stringFromEClass(e->class));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(color));

        log_tag("debug_log.txt", "[FIGHT]", "Killed  %s.",
                stringFromEClass(e->class));

        //Update stats
        target->stats->enemieskilled++;
    } else {
        //Apply status effects to enemy
        if (e->status != Normal) {
            applyEStatus(notify_win, e);
            log_tag("debug_log.txt", "[STATUS]", "Applied  %s to %s.",
                    stringFromStatus(e->status), stringFromEClass(e->class));
        }
    }

    if (target->hp <= 0) {
        log_tag("debug_log.txt", "[DEBUG]", "[%s]:  Target died. Hp: (%i)",
                __func__, target->hp);
        res = FIGHTRES_DEATH;
    } else {
        //Apply status effects to target
        if (target->status != Normal) {
            applyStatus(notify_win, target);
        }
    }

    //Consumable drop, guaranteed on killing a beast
    if (res == FIGHTRES_KILL_DONE
        && (e->beast || ((hlpd_rand() % 9) - (target->luck / 10) <= 0))) {
        int drop = dropConsumable(target);
        sprintf(msg, "You found a %s!", stringFromConsumables(drop));
        wattron(notify_win, COLOR_PAIR(S4C_CYAN));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_CYAN));
        log_tag("debug_log.txt", "[DROPS]", "Found Consumable:    %s.",
                stringFromConsumables(drop));
    }

    //Artifact drop (if we don't have all of them), guaranteed on killing a beast
    if ((target->stats->artifactsfound != ARTIFACTSMAX + 1)
        && res == FIGHTRES_KILL_DONE && (e->beast
                                         ||
                                         ((hlpd_rand() % ENEMY_ARTIFACTDROP_CHANCE) -
                                          (target->luck / 10) <= 0))) {
        int artifact_drop = dropArtifact(target);
        sprintf(msg, "You found a %s!", stringFromArtifacts(artifact_drop));
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        log_tag("debug_log.txt", "[DROPS]", "Found Artifact:    %s.",
                stringFromArtifacts(artifact_drop));
        if (!e->beast)
            log_tag("debug_log.txt", "[.1%% CHANCE]",
                    "\nNORMAL ENEMY DROPPED ARTIFACT! 0.1%% chance??\n");
    }

    //Equip drop, guaranteed on killing a beast
    if (res == FIGHTRES_KILL_DONE
        && (e->beast || ((hlpd_rand() % 15) - (target->luck / 10) <= 0))) {
        dropEquip(target, e->beast, notify_win, kls);
    }
    return res;
}

/**
 * Takes a Fighter and a Enemy pointers and calls fight().
 * @see Fighter
 * @see Enemy
 * @see fight()
 * @param player The Fighter pointer at hand.
 * @param e The Enemy pointer at hand.
 * @param foe_op The foeTurnOption_OP for the foe.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 */
int defer_fight_enemy(Fighter *player, Enemy *e, foeTurnOption_OP foe_op,
                      WINDOW *notify_win, Koliseo *kls)
{
    char msg[200];
    //FIXME
    //Is it okay to return just one result, when having 2 interactions that could go differently?
    //
    //Use FIGHTRES_CLASH as needed, to indicate both sides were damaged at some point.
    fightResult res = FIGHTRES_NO_DMG;

    int player_goes_first = (player->vel >= e->vel ? 1 : 0);

    int first_act_res = FIGHTRES_NO_DMG;

    if (player_goes_first) {

        res = fight(player, e, notify_win, kls);

        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);
        first_act_res = res;

        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            switch (foe_op) {
            case FOE_OP_INVALID: {
                log_tag("debug_log.txt", "[ERROR]",
                        "foe_op was FOE_OP_INVALID in [%s]: [%i]", __func__,
                        foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            case FOE_OP_IDLE: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } was idle.", __func__,
                        stringFromEClass(e->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is loafing around.",
                        stringFromEClass(e->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));
            }
            break;
            case FOE_OP_FIGHT: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } wants to fight.", __func__,
                        stringFromEClass(e->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is angry!", stringFromEClass(e->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));

                res = enemy_attack(e, player, notify_win, kls);
            }
            break;
            case FOE_OP_SPECIAL: {
                log_tag("debug_log.txt", "[TODO]",
                        "[%s()]:  Foe { %s } wants to use a special.",
                        __func__, stringFromEClass(e->class));
                //TODO
                //Implement enemy special function
                //res = enemy_attack_special(e,player,notify_win,kls);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected foeTurnOption_OP in [%s()]: [%i]",
                        __func__, foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            }			// End foe_op switch

            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);
            if (res == FIGHTRES_DEATH || res == FIGHTRES_KILL_DONE) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was not a clash...", __func__);
            } else if ((res == FIGHTRES_DMG_TAKEN
                        && first_act_res == FIGHTRES_DMG_DEALT)
                       || (res == FIGHTRES_DMG_DEALT
                           && first_act_res == FIGHTRES_DMG_TAKEN)) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was a clash!", __func__);
                res = FIGHTRES_CLASH;
            }

            return res;
        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    } else {
        //Foe acts first
        switch (foe_op) {
        case FOE_OP_INVALID: {
            log_tag("debug_log.txt", "[ERROR]",
                    "foe_op was FOE_OP_INVALID in [%s]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        case FOE_OP_IDLE: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } was idle.", __func__,
                    stringFromEClass(e->class));
            wattron(notify_win, COLOR_PAIR(S4C_GREY));
            sprintf(msg, "%s is loafing around.",
                    stringFromEClass(e->class));
            display_notification(notify_win, msg, 500);
            wattroff(notify_win, COLOR_PAIR(S4C_GREY));
        }
        break;
        case FOE_OP_FIGHT: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } wants to fight.", __func__,
                    stringFromEClass(e->class));
            res = enemy_attack(e, player, notify_win, kls);
        }
        break;
        case FOE_OP_SPECIAL: {
            log_tag("debug_log.txt", "[TODO]",
                    "[%s()]:  Foe { %s } wants to use a special.", __func__,
                    stringFromEClass(e->class));
            //TODO
            //Implement enemy special function
            //res = enemy_attack_special(e,player,notify_win,kls);
        }
        break;
        default: {
            log_tag("debug_log.txt", "[ERROR]",
                    "Unexpected foeTurnOption_OP in [%s()]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        }			// End foe_op switch

        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);
        first_act_res = res;

        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            res = fight(player, e, notify_win, kls);

            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);
            if (res == FIGHTRES_DEATH || res == FIGHTRES_KILL_DONE) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was not a clash...", __func__);
            } else if ((res == FIGHTRES_DMG_TAKEN
                        && first_act_res == FIGHTRES_DMG_DEALT)
                       || (res == FIGHTRES_DMG_DEALT
                           && first_act_res == FIGHTRES_DMG_TAKEN)) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was a clash!", __func__);
                res = FIGHTRES_CLASH;
            }

            return res;
        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    }
    return res;
}

/**
 * Takes a Fighter and a Enemy pointers and calls do_Skill().
 * @see Fighter
 * @see Enemy
 * @see do_Skill()
 * @param player The Fighter pointer at hand.
 * @param e The Enemy pointer at hand.
 * @param picked_skill The picked skill by Fighter.
 * @param foe_op The foeTurnOption_OP for the foe.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 */
int defer_skill_enemy(Fighter *player, Enemy *e, skillType picked_skill, foeTurnOption_OP foe_op,
                      WINDOW *notify_win, Koliseo *kls)
{
    char msg[200];
    //FIXME
    //Is it okay to return just one result, when having 2 interactions that could go differently?
    //
    //Use FIGHTRES_CLASH as needed, to indicate both sides were damaged at some point.
    fightResult res = FIGHTRES_NO_DMG;

    int player_goes_first = (player->vel >= e->vel ? 1 : 0);

    int first_act_res = FIGHTRES_NO_DMG;

    if (player_goes_first) {

        res = do_Skill(player, e, picked_skill, notify_win, kls);

        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);
        first_act_res = res;

        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            switch (foe_op) {
            case FOE_OP_INVALID: {
                log_tag("debug_log.txt", "[ERROR]",
                        "foe_op was FOE_OP_INVALID in [%s]: [%i]", __func__,
                        foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            case FOE_OP_IDLE: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } was idle.", __func__,
                        stringFromEClass(e->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is loafing around.",
                        stringFromEClass(e->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));
            }
            break;
            case FOE_OP_FIGHT: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } wants to fight.", __func__,
                        stringFromEClass(e->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is angry!", stringFromEClass(e->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));

                res = enemy_attack(e, player, notify_win, kls);
            }
            break;
            case FOE_OP_SPECIAL: {
                log_tag("debug_log.txt", "[TODO]",
                        "[%s()]:  Foe { %s } wants to use a special.",
                        __func__, stringFromEClass(e->class));
                //TODO
                //Implement enemy special function
                //res = enemy_attack_special(e,player,notify_win,kls);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected foeTurnOption_OP in [%s()]: [%i]",
                        __func__, foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            }			// End foe_op switch

            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);
            if (res == FIGHTRES_DEATH || res == FIGHTRES_KILL_DONE) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was not a clash...", __func__);
            } else if ((res == FIGHTRES_DMG_TAKEN
                        && first_act_res == FIGHTRES_DMG_DEALT)
                       || (res == FIGHTRES_DMG_DEALT
                           && first_act_res == FIGHTRES_DMG_TAKEN)) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was a clash!", __func__);
                res = FIGHTRES_CLASH;
            }

            return res;
        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    } else {
        //Foe acts first
        switch (foe_op) {
        case FOE_OP_INVALID: {
            log_tag("debug_log.txt", "[ERROR]",
                    "foe_op was FOE_OP_INVALID in [%s]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        case FOE_OP_IDLE: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } was idle.", __func__,
                    stringFromEClass(e->class));
            wattron(notify_win, COLOR_PAIR(S4C_GREY));
            sprintf(msg, "%s is loafing around.",
                    stringFromEClass(e->class));
            display_notification(notify_win, msg, 500);
            wattroff(notify_win, COLOR_PAIR(S4C_GREY));
        }
        break;
        case FOE_OP_FIGHT: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } wants to fight.", __func__,
                    stringFromEClass(e->class));
            res = enemy_attack(e, player, notify_win, kls);
        }
        break;
        case FOE_OP_SPECIAL: {
            log_tag("debug_log.txt", "[TODO]",
                    "[%s()]:  Foe { %s } wants to use a special.", __func__,
                    stringFromEClass(e->class));
            //TODO
            //Implement enemy special function
            //res = enemy_attack_special(e,player,notify_win,kls);
        }
        break;
        default: {
            log_tag("debug_log.txt", "[ERROR]",
                    "Unexpected foeTurnOption_OP in [%s()]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        }			// End foe_op switch

        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);
        first_act_res = res;

        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            res = do_Skill(player, e, picked_skill, notify_win, kls);

            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);
            if (res == FIGHTRES_DEATH || res == FIGHTRES_KILL_DONE) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was not a clash...", __func__);
            } else if ((res == FIGHTRES_DMG_TAKEN
                        && first_act_res == FIGHTRES_DMG_DEALT)
                       || (res == FIGHTRES_DMG_DEALT
                           && first_act_res == FIGHTRES_DMG_TAKEN)) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was a clash!", __func__);
                res = FIGHTRES_CLASH;
            }

            return res;
        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    }
    return res;
}

/**
 * Takes a Fighter, a Boss and a Path pointers and compares fighters stats to determine who gets damaged and returns the fightStatus value.
 * Prints notifications to the passed WINDOW pointer.
 * On boss death, we call dropConsumable, dropEquip and dropArtifact.
 * @see Fighter
 * @see Boss
 * @see statReset()
 * @see statResetBoss()
 * @see stringFromBossClass()
 * @see dropConsumable()
 * @see dropEquip()
 * @see dropArtifact()
 * @param player The Fighter pointer at hand.
 * @param b The Enemy pointer at hand.
 * @param p The Path pointer for the game.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 * @see display_notification()
 */
int boss_fight(Fighter *player, Boss *b, Path *p, WINDOW *notify_win,
               Koliseo *kls)
{

    fightResult res = FIGHTRES_NO_DMG;
    //Stat comparisons
    //
    char msg[200];

    int atkdelta = (player->atk + player->equipboost_atk) - b->atk - (hlpd_rand() % 3) - 1;	//Skewed with defender
    int defdelta = (player->def + player->equipboost_def) - b->def + (hlpd_rand() % 2) + 1;	//Skewed with attacker
    int veldelta =
        (player->vel + player->equipboost_vel) - b->vel + (hlpd_rand() % 3) + 1;

    int atkOnPlayer =
        b->atk - (player->def + player->equipboost_def + (player->vel / 6));
    int atkOnEnemy =
        (player->atk + player->equipboost_atk) - (b->def + (b->vel / 6));
    if (G_GODMODE_ON == 1) {
        log_tag("debug_log.txt", "[DEBUG]", "boss_fight():  G_GODMODE_ON == 1");
        atkdelta = 100;
        defdelta = 100;
        veldelta = 100;
        atkOnPlayer = 1;
        atkOnEnemy = 100;
    }
    sprintf(msg, "atkdelta %i", atkdelta);
    log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);
    sprintf(msg, "defdelta %i", defdelta);
    log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);
    sprintf(msg, "veldelta %i", veldelta);
    log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);
    sprintf(msg, "atkOnEnemy %i", atkOnEnemy);
    log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);
    sprintf(msg, "atkOnPlayer %i\n", atkOnPlayer);
    log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);

    int damageDealt = -1;

    if (veldelta >= 0) {

        if (atkOnEnemy > 3) {
            damageDealt = atkOnEnemy;
            b->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result A WIN (faster, great atk).\n");
        } else if (atkOnEnemy >= 0) {
            damageDealt = abs(atkOnEnemy - atkdelta);
            b->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result B WIN (faster, ok atk).\n");
        } else {
            if (atkOnEnemy < -3) {
                damageDealt =
                    fabsf(atkOnPlayer -
                          0.75F * (player->vel + player->equipboost_vel));
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result C1 LOST (faster, atk < -3).\n");
            } else {
                damageDealt = abs(atkOnPlayer - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result C2 LOST (faster, atk > -3).\n");
            }
            player->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
        }
    } else {
        atkdelta = -atkdelta;
        if (atkOnPlayer > 3) {
            damageDealt = atkOnPlayer;
            player->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result D LOST (slower, great enemy atk.\n");
        } else if (atkOnPlayer >= 0) {
            damageDealt = abs(atkOnPlayer - atkdelta);
            player->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result E LOST (slower, ok enemy atk.\n");
        } else {
            if (atkOnPlayer < -3) {
                damageDealt = fabsf(atkOnEnemy - 0.75F * b->vel);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result F1 WIN (slower, enemy atk < -3.\n");
            } else {
                damageDealt = abs(atkOnEnemy - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result F2 WIN (slower, enemy atk > -3.\n");
            }
            b->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
        }
    }

    log_tag("debug_log.txt", "[FIGHT]", "damageCalc %i", damageDealt);

    int yourhit = (res == FIGHTRES_DMG_DEALT) ? 1 : 0;
    char victim[25];

    if (!yourhit) {

        b->vel--;
        b->atk--;
        b->def -= 2;

        //Check if someone earned a stat reset after the fight
        statReset(player, 0);
        statResetBoss(b, 0);

        strcpy(victim, player->name);
    } else {

        player->vel--;
        player->atk--;
        player->def -= 2;

        //Account for vampirism perk
        int vampire_perks = player->perks[VAMPIRISM]->innerValue;
        if (vampire_perks > 0) {
            int recovery = floor(damageDealt * (0.1 * vampire_perks));
            player->hp += recovery;
            sprintf(msg, "Vampirism proc for +%i HP.\n", recovery);
            log_tag("debug_log.txt", "[PERKS]", msg);
            if (player->hp >= player->totalhp) {
                player->hp = player->totalhp;
            };
        }
        //Account for burn on touch perk
        int hotbody_perks = player->perks[HOT_BODY]->innerValue;
        if (hotbody_perks > 0) {
            int burnchance = 11 - hotbody_perks;
            if (hlpd_rand() % burnchance == 0) {
                b->status = Burned;	//Set status to Burned. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) b->counters[Burned], 2);	//Give 2 turns of Burned status
                sprintf(msg, "Hotbody proc on 1/%i chance.\n", burnchance);
                log_tag("debug_log.txt", "[PERKS]", msg);
            }
        }
        //Account for poison on touch perk. Order of checks with hot_body perk may cause issues?
        int biohazard_perks = player->perks[BIOHAZARD]->innerValue;
        if (biohazard_perks > 0) {
            int poisonchance = 11 - biohazard_perks;
            if (hlpd_rand() % poisonchance == 0) {
                b->status = Poison;	//Set status to Poison. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) b->counters[POISON], 2);	//Give 2 turns of Poison status
                sprintf(msg, "Biohazard proc on 1/%i chance.\n", poisonchance);
                log_tag("debug_log.txt", "[PERKS]", msg);
            }
        }
        //Check if someone earned a stat reset after the fight
        statResetBoss(b, 0);
        statReset(player, 0);

        strcpy(victim, stringFromBossClass(b->class));
    }

    int color = -1;
    if (yourhit) {
        color = S4C_WHITE;
    } else {
        color = S4C_RED;
    }

    wattron(notify_win, COLOR_PAIR(color));
    sprintf(msg, "%s was hit.    (%i DMG)", victim,
            damageDealt > 0 ? damageDealt : 1);
    display_notification(notify_win, msg, 500);
    wattroff(notify_win, COLOR_PAIR(color));

    //Rolls
    //
    //Critical hit roll

    //Account for critboost_chance perks
    int critMax =
        round(10.0 - floor(player->luck / 5) -
              (1.5 * player->perks[CRITBOOST_CHANCE]->innerValue));

    int critRes = (hlpd_rand() % critMax);

    if (res == FIGHTRES_DMG_DEALT && (critRes <= 0)) {

        //Account for critboost_dmg perks
        int dmgboost_perks = player->perks[CRITBOOST_DMG]->innerValue;
        damageDealt *= (0.30 + (0.12 * dmgboost_perks));
        b->hp -= damageDealt > 0 ? damageDealt : 1;
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        sprintf(msg, "A critical hit!    (%i DMG)",
                damageDealt > 0 ? damageDealt : 1);
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        sprintf(msg, "Critical hit for %i dmg, proc on 1/%i chance.\n",
                damageDealt, critMax);
        log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);

        //Update stats
        player->stats->criticalhits++;
    }
    //Check for deaths -> exit condition from loop
    //
    //
    //
    if (b->hp <= 0) {
        res = FIGHTRES_KILL_DONE;

        //Account for runic circle perk
        int runic_perks = player->perks[RUNIC_MAGNET]->innerValue;
        if (runic_perks > 0) {
            int recovery = round(0.51 * runic_perks);
            player->energy += recovery;
            sprintf(msg, "Runicmagnet proc for %i energy.\n", recovery);
            log_tag("debug_log.txt", "[PERKS]", msg);
        }
        if (b->beast) {
            color = S4C_MAGENTA;
        } else {
            color = S4C_RED;
        }
        wattron(notify_win, COLOR_PAIR(color));
        sprintf(msg, "%s fainted.", stringFromBossClass(b->class));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(color));
        sprintf(msg, "Killed  %s.", stringFromBossClass(b->class));
        log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);

        //Update stats
        player->stats->bosseskilled++;

        //Check if we killed this boss before
        if (player->stats->killed_bosses[b->class] == 0) {
            //Update stats
            player->stats->unique_bosseskilled++;
            player->stats->killed_bosses[b->class] += 1;
            //Check if we have to update the wincon value
            if (p->win_condition->class == ALL_BOSSES) {
                p->win_condition->current_val++;
            }
            sprintf(msg, "Killed new boss  %s.\n",
                    stringFromBossClass(b->class));
            log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);
        }
    } else {
        //Apply status effects to boss
        if (b->status != Normal) {
            applyBStatus(notify_win, b);
            sprintf(msg, "Applied  %s to %s.", stringFromStatus(b->status),
                    stringFromBossClass(b->class));
            log_tag("debug_log.txt", "[STATUS]", msg);
        }
    }

    if (player->hp <= 0) {
        res = FIGHTRES_DEATH;
    } else {
        //Apply status effects to player
        if (player->status != Normal) {
            applyStatus(notify_win, player);
        }
    }

    //Consumable drop, guaranteed on killing a beast
    if (res == FIGHTRES_KILL_DONE) {
        int drop = dropConsumable(player);
        wattron(notify_win, COLOR_PAIR(S4C_CYAN));
        sprintf(msg, "You found a %s!", stringFromConsumables(drop));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_CYAN));
        sprintf(msg, "Found Consumable:    %s.", stringFromConsumables(drop));
        log_tag("debug_log.txt", "[DROPS]", msg);
    }

    //Artifact drop (if we don't have all of them)
    if (res == FIGHTRES_KILL_DONE
        && (player->stats->artifactsfound != ARTIFACTSMAX + 1)) {
        int artifact_drop = dropArtifact(player);
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        sprintf(msg, "You found a %s!", stringFromArtifacts(artifact_drop));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        sprintf(msg, "Found Artifact:    %s.",
                stringFromArtifacts(artifact_drop));
        log_tag("debug_log.txt", "[DROPS]", msg);
    }
    //Equip drop
    if (res == FIGHTRES_KILL_DONE) {
        //We give 1 to obtain the better equip generation used for beasts
        dropEquip(player, 1, notify_win, kls);
    }

    return res;
}

int boss_attack(Boss *b, Fighter *target, Path *p, WINDOW *notify_win,
                Koliseo *kls)
{

    //TODO
    //Implementation similar to boss_fight(), as a base idea
    //Should return fightResult values, while keeping the perspective on the Fighter, as in:
    //
    //      FIGHTRES_DEATH means the Fighter died
    //      FIGHTRES_KILL_DONE means the Boss died
    //      FIGHTRES_DMG_DEALT means the Fighter inflicted damage
    //      FIGHRES_DMG_TAKEN means the Fighter received damage
    fightResult res = FIGHTRES_NO_DMG;
    //Stat comparisons
    //
    char msg[200];

    int atkdelta = (b->atk + b->turnboost_atk) - (target->atk + target->equipboost_atk + target->turnboost_atk - (hlpd_rand() % 3)) - 1;	//Skewed with defender
    int defdelta = (b->def + b->turnboost_def) - (target->def + target->equipboost_def + target->turnboost_def) + (hlpd_rand() % 2) + 1;	//Skewed with attacker
    int veldelta =
        (b->vel + b->turnboost_vel) - (target->vel + target->equipboost_vel +
                                       target->turnboost_vel) + (hlpd_rand() % 3) +
        1;

    int atkOnPlayer =
        (b->atk + b->turnboost_atk) - (target->def + target->equipboost_def +
                                       target->turnboost_def +
                                       (target->vel / 6));
    int atkOnEnemy =
        (target->atk + target->equipboost_atk + target->turnboost_atk) -
        (b->def + b->turnboost_def + (b->vel / 6));
    if (G_GODMODE_ON == 1) {
        log_tag("debug_log.txt", "[DEBUG]", "[%s]:  G_GODMODE_ON == 1",
                __func__);
        atkdelta = -100;
        defdelta = -100;
        veldelta = -100;
        atkOnPlayer = 1;
        atkOnEnemy = 100;
    }
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkdelta %i", atkdelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "defdelta %i", defdelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "veldelta %i", veldelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkOnEnemy %i", atkOnEnemy);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkOnPlayer %i", atkOnPlayer);

    int damageDealt = -1;

    if (veldelta >= 0) {	//Boss was faster
        if (atkOnPlayer > 3) {
            damageDealt = atkOnPlayer;
            target->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result D LOST (slower, great enemy atk.");
        } else if (atkOnPlayer >= 0) {
            damageDealt = abs(atkOnPlayer - atkdelta);
            target->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result E LOST (slower, ok enemy atk.");
        } else {
            if (atkOnPlayer < -3) {
                damageDealt = fabsf(atkOnEnemy - 0.75F * b->vel);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result F1 WIN (slower, enemy atk < -3.");
            } else {
                damageDealt = abs(atkOnEnemy - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result F2 WIN (slower, enemy atk > -3.");
            }
            b->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
        }
    } else {			//Target was faster
        atkdelta = -atkdelta;
        if (atkOnEnemy > 3) {
            damageDealt = atkOnEnemy;
            b->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result A WIN (faster, great atk).");
        } else if (atkOnEnemy >= 0) {
            damageDealt = abs(atkOnEnemy - atkdelta);
            b->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result B WIN (faster, ok atk).");
        } else {
            if (atkOnEnemy < -3) {
                damageDealt =
                    fabsf(atkOnPlayer -
                          0.75F * (target->vel + target->equipboost_vel));
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result C1 LOST (faster, atk < -3).");
            } else {
                damageDealt = abs(atkOnPlayer - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result C2 LOST (faster, atk > -3).");
            }
            target->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
        }
    }

    log_tag("debug_log.txt", "[FIGHT]", "damageCalc %i", damageDealt);

    int yourhit = (res == FIGHTRES_DMG_DEALT) ? 1 : 0;
    char victim[25];

    if (!yourhit) {

        b->vel--;
        b->atk--;
        b->def -= 2;

        //Check if someone earned a stat reset after the fight
        statReset(target, 0);
        statResetBoss(b, 0);

        strcpy(victim, target->name);
    } else {

        target->vel--;
        target->atk--;
        target->def -= 2;

        //Account for vampirism perk
        int vampire_perks = target->perks[VAMPIRISM]->innerValue;
        if (vampire_perks > 0) {
            int recovery = floor(damageDealt * (0.1 * vampire_perks));
            target->hp += recovery;
            log_tag("debug_log.txt", "[PERKS]", "Vampirism proc for +%i HP.",
                    recovery);
            if (target->hp >= target->totalhp) {
                target->hp = target->totalhp;
            };
        }
        //Account for burn on touch perk
        int hotbody_perks = target->perks[HOT_BODY]->innerValue;
        if (hotbody_perks > 0) {
            int burnchance = 11 - hotbody_perks;
            if (hlpd_rand() % burnchance == 0) {
                b->status = Burned;	//Set status to Burned. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) b->counters[Burned], 2);	//Give 2 turns of Burned status
                log_tag("debug_log.txt", "[PERKS]",
                        "Hotbody proc on 1/%i chance.", burnchance);
            }
        }
        //Account for poison on touch perk. Order of checks with hot_body perk may cause issues?
        int biohazard_perks = target->perks[BIOHAZARD]->innerValue;
        if (biohazard_perks > 0) {
            int poisonchance = 11 - biohazard_perks;
            if (hlpd_rand() % poisonchance == 0) {
                b->status = Poison;	//Set status to Poison. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) b->counters[POISON], 2);	//Give 2 turns of Poison status
                log_tag("debug_log.txt", "[PERKS]",
                        "Biohazard proc on 1/%i chance.", poisonchance);
            }
        }
        //Check if someone earned a stat reset after the fight
        statResetBoss(b, 0);
        statReset(target, 0);

        strcpy(victim, stringFromBossClass(b->class));
    }

    int color = -1;
    if (yourhit) {
        color = S4C_WHITE;
    } else {
        color = S4C_RED;
    }

    wattron(notify_win, COLOR_PAIR(color));
    sprintf(msg, "%s was hit.    (%i DMG)", victim,
            damageDealt > 0 ? damageDealt : 1);
    display_notification(notify_win, msg, 500);
    wattroff(notify_win, COLOR_PAIR(color));

    //Rolls
    //
    //Critical hit roll

    //Account for critboost_chance perks
    int critMax =
        round(10.0 - floor(target->luck / 5) -
              (1.5 * target->perks[CRITBOOST_CHANCE]->innerValue));

    int critRes = (hlpd_rand() % critMax);

    if (res == FIGHTRES_DMG_DEALT && (critRes <= 0)) {

        //Account for critboost_dmg perks
        int dmgboost_perks = target->perks[CRITBOOST_DMG]->innerValue;
        damageDealt *= (0.30 + (0.12 * dmgboost_perks));
        b->hp -= damageDealt > 0 ? damageDealt : 1;
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        sprintf(msg, "A critical hit!    (%i DMG)",
                damageDealt > 0 ? damageDealt : 1);
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        log_tag("debug_log.txt", "[FIGHT-BOSS]",
                "Critical hit for %i dmg, proc on 1/%i chance.", damageDealt,
                critMax);

        //Update stats
        target->stats->criticalhits++;
    }
    //Check for deaths -> exit condition from loop
    //
    //
    //
    if (b->hp <= 0) {
        res = FIGHTRES_KILL_DONE;

        //Account for runic circle perk
        int runic_perks = target->perks[RUNIC_MAGNET]->innerValue;
        if (runic_perks > 0) {
            int recovery = round(0.51 * runic_perks);
            target->energy += recovery;
            log_tag("debug_log.txt", "[PERKS]",
                    "Runicmagnet proc for %i energy.", recovery);
        }
        if (b->beast) {
            color = S4C_MAGENTA;
        } else {
            color = S4C_RED;
        }
        wattron(notify_win, COLOR_PAIR(color));
        sprintf(msg, "%s fainted.", stringFromBossClass(b->class));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(color));
        log_tag("debug_log.txt", "[FIGHT-BOSS]", "Killed  %s.",
                stringFromBossClass(b->class));

        //Update stats
        target->stats->bosseskilled++;

        //Check if we killed this boss before
        if (target->stats->killed_bosses[b->class] == 0) {
            //Update stats
            target->stats->unique_bosseskilled++;
            target->stats->killed_bosses[b->class] += 1;
            //Check if we have to update the wincon value
            if (p->win_condition->class == ALL_BOSSES) {
                p->win_condition->current_val++;
            }
            log_tag("debug_log.txt", "[FIGHT-BOSS]", "Killed new boss  %s.",
                    stringFromBossClass(b->class));
        }
    } else {
        //Apply status effects to boss
        if (b->status != Normal) {
            applyBStatus(notify_win, b);
            log_tag("debug_log.txt", "[STATUS]", "Applied  %s to %s.",
                    stringFromStatus(b->status), stringFromBossClass(b->class));
        }
    }

    if (target->hp <= 0) {
        res = FIGHTRES_DEATH;
    } else {
        //Apply status effects to target
        if (target->status != Normal) {
            applyStatus(notify_win, target);
        }
    }

    //Consumable drop, guaranteed on killing a beast
    if (res == FIGHTRES_KILL_DONE) {
        int drop = dropConsumable(target);
        wattron(notify_win, COLOR_PAIR(S4C_CYAN));
        sprintf(msg, "You found a %s!", stringFromConsumables(drop));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_CYAN));
        log_tag("debug_log.txt", "[DROPS]", "Found Consumable:    %s.",
                stringFromConsumables(drop));
    }

    //Artifact drop (if we don't have all of them)
    if (res == FIGHTRES_KILL_DONE
        && (target->stats->artifactsfound != ARTIFACTSMAX + 1)) {
        int artifact_drop = dropArtifact(target);
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        sprintf(msg, "You found a %s!", stringFromArtifacts(artifact_drop));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        log_tag("debug_log.txt", "[DROPS]", "Found Artifact:    %s.",
                stringFromArtifacts(artifact_drop));
    }
    //Equip drop
    if (res == FIGHTRES_KILL_DONE) {
        //We give 1 to obtain the better equip generation used for beasts
        dropEquip(target, 1, notify_win, kls);
    }

    return res;
}

/**
 * Takes a Fighter and a Boss pointers and calls boss_fight().
 * @see Fighter
 * @see Boss
 * @see boss_fight()
 * @param player The Fighter pointer at hand.
 * @param b The Boss pointer at hand.
 * @param foe_op The foeTurnOption_OP for the foe.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 */
int defer_fight_boss(Fighter *player, Boss *b, Path *p, foeTurnOption_OP foe_op,
                     WINDOW *notify_win, Koliseo *kls)
{
    char msg[200];
    //FIXME
    //Is it okay to return just one result, when having 2 interactions that could go differently?
    //
    //Use FIGHTRES_CLASH as needed, to indicate both sides were damaged at some point.
    fightResult res = FIGHTRES_NO_DMG;

    int player_goes_first = (player->vel >= b->vel ? 1 : 0);

    if (player_goes_first) {
        res = boss_fight(player, b, p, notify_win, kls);
        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);
        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            switch (foe_op) {
            case FOE_OP_INVALID: {
                log_tag("debug_log.txt", "[ERROR]",
                        "foe_op was FOE_OP_INVALID in [%s]: [%i]", __func__,
                        foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            case FOE_OP_IDLE: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } was idle.", __func__,
                        stringFromBossClass(b->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is loafing around.",
                        stringFromBossClass(b->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));
            }
            break;
            case FOE_OP_FIGHT: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } wants to fight.", __func__,
                        stringFromBossClass(b->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is angry!", stringFromBossClass(b->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));
                res = boss_attack(b, player, p, notify_win, kls);
            }
            break;
            case FOE_OP_SPECIAL: {
                log_tag("debug_log.txt", "[TODO]",
                        "[%s()]:  Foe { %s } wants to use a special.",
                        __func__, stringFromBossClass(b->class));
                //TODO
                //Implement boss special function
                //res = boss_attack_special(b,player,p,notify_win,kls);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected foeTurnOption_OP in [%s()]: [%i]",
                        __func__, foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            }			// End foe_op switch

            //TODO
            //Check second turn act res?
            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);

            return res;
        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    } else {
        //Foe acts first
        switch (foe_op) {
        case FOE_OP_INVALID: {
            log_tag("debug_log.txt", "[ERROR]",
                    "foe_op was FOE_OP_INVALID in [%s()]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        case FOE_OP_IDLE: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } was idle.", __func__,
                    stringFromBossClass(b->class));
            wattron(notify_win, COLOR_PAIR(S4C_GREY));
            sprintf(msg, "%s is loafing around.",
                    stringFromBossClass(b->class));
            display_notification(notify_win, msg, 500);
            wattroff(notify_win, COLOR_PAIR(S4C_GREY));
        }
        break;
        case FOE_OP_FIGHT: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } wants to fight.", __func__,
                    stringFromBossClass(b->class));
            wattron(notify_win, COLOR_PAIR(S4C_GREY));
            sprintf(msg, "%s is angry!", stringFromBossClass(b->class));
            display_notification(notify_win, msg, 500);
            wattroff(notify_win, COLOR_PAIR(S4C_GREY));
            res = boss_attack(b, player, p, notify_win, kls);
        }
        break;
        case FOE_OP_SPECIAL: {
            log_tag("debug_log.txt", "[TODO]",
                    "[%s()]:  Foe { %s } wants to use a special.", __func__,
                    stringFromBossClass(b->class));
            //TODO
            //Implement boss special function
            //res = boss_attack_special(b,player,p,notify_win,kls);
        }
        break;
        default: {
            log_tag("debug_log.txt", "[ERROR]",
                    "Unexpected foeTurnOption_OP in [%s()]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        }			// End foe_op switch

        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);

        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            res = boss_fight(player, b, p, notify_win, kls);

            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);
            //TODO
            //Check second turn act res?
            return res;

        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    }

    return res;
}

/**
 * Takes a Fighter and a Boss pointers and calls do_Skill_boss().
 * @see Fighter
 * @see Boss
 * @see do_Skill_boss()
 * @param player The Fighter pointer at hand.
 * @param b The Boss pointer at hand.
 * @param picked_skill The skill picked by Fighter.
 * @param foe_op The foeTurnOption_OP for the foe.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 */
int defer_skill_boss(Fighter *player, Boss *b, skillType picked_skill, Path *p, foeTurnOption_OP foe_op,
                     WINDOW *notify_win, Koliseo *kls)
{
    char msg[200];
    //FIXME
    //Is it okay to return just one result, when having 2 interactions that could go differently?
    //
    //Use FIGHTRES_CLASH as needed, to indicate both sides were damaged at some point.
    fightResult res = FIGHTRES_NO_DMG;

    int player_goes_first = (player->vel >= b->vel ? 1 : 0);

    if (player_goes_first) {
        res = do_Skill_boss(player, b, picked_skill, p, notify_win, kls);
        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);
        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            switch (foe_op) {
            case FOE_OP_INVALID: {
                log_tag("debug_log.txt", "[ERROR]",
                        "foe_op was FOE_OP_INVALID in [%s]: [%i]", __func__,
                        foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            case FOE_OP_IDLE: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } was idle.", __func__,
                        stringFromBossClass(b->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is loafing around.",
                        stringFromBossClass(b->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));
            }
            break;
            case FOE_OP_FIGHT: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } wants to fight.", __func__,
                        stringFromBossClass(b->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is angry!", stringFromBossClass(b->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));
                res = boss_attack(b, player, p, notify_win, kls);
            }
            break;
            case FOE_OP_SPECIAL: {
                log_tag("debug_log.txt", "[TODO]",
                        "[%s()]:  Foe { %s } wants to use a special.",
                        __func__, stringFromBossClass(b->class));
                //TODO
                //Implement boss special function
                //res = boss_attack_special(b,player,p,notify_win,kls);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected foeTurnOption_OP in [%s()]: [%i]",
                        __func__, foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            }			// End foe_op switch

            //TODO
            //Check second turn act res?
            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);

            return res;
        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    } else {
        //Foe acts first
        switch (foe_op) {
        case FOE_OP_INVALID: {
            log_tag("debug_log.txt", "[ERROR]",
                    "foe_op was FOE_OP_INVALID in [%s()]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        case FOE_OP_IDLE: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } was idle.", __func__,
                    stringFromBossClass(b->class));
            wattron(notify_win, COLOR_PAIR(S4C_GREY));
            sprintf(msg, "%s is loafing around.",
                    stringFromBossClass(b->class));
            display_notification(notify_win, msg, 500);
            wattroff(notify_win, COLOR_PAIR(S4C_GREY));
        }
        break;
        case FOE_OP_FIGHT: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } wants to fight.", __func__,
                    stringFromBossClass(b->class));
            wattron(notify_win, COLOR_PAIR(S4C_GREY));
            sprintf(msg, "%s is angry!", stringFromBossClass(b->class));
            display_notification(notify_win, msg, 500);
            wattroff(notify_win, COLOR_PAIR(S4C_GREY));
            res = boss_attack(b, player, p, notify_win, kls);
        }
        break;
        case FOE_OP_SPECIAL: {
            log_tag("debug_log.txt", "[TODO]",
                    "[%s()]:  Foe { %s } wants to use a special.", __func__,
                    stringFromBossClass(b->class));
            //TODO
            //Implement boss special function
            //res = boss_attack_special(b,player,p,notify_win,kls);
        }
        break;
        default: {
            log_tag("debug_log.txt", "[ERROR]",
                    "Unexpected foeTurnOption_OP in [%s()]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        }			// End foe_op switch

        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);

        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            res = do_Skill_boss(player, b, picked_skill, p, notify_win, kls);

            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);
            //TODO
            //Check second turn act res?
            return res;

        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    }

    return res;
}

fightResult do_Skill(Fighter * player, Enemy * e, skillType picked_skill, WINDOW * notify_win, Koliseo * kls)
{

    return FIGHTRES_NO_DMG;
}

fightResult do_Skill_boss(Fighter * player, Boss * b, skillType picked_skill, Path * path, WINDOW * notify_win, Koliseo * kls)
{

    return FIGHTRES_NO_DMG;
}

#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
int defer_fight_enemy(Fighter * player, Enemy * e, foeTurnOption_OP foe_op,
                      Rectangle * notification_area, Koliseo * kls)
{
    return -1;
}

int defer_skill_enemy(Fighter *player, Enemy *e, skillType picked_skill, foeTurnOption_OP foe_op,
                      Rectangle * notification_area, Koliseo * kls)
{
    return -1;
}

int fight(Fighter * player, Enemy * e, Rectangle * notification_area, Koliseo * kls)
{
    return -1;
}

int enemy_attack(Enemy * e, Fighter * target, Rectangle * notification_area,
                 Koliseo * kls)
{
    return -1;
}

int defer_fight_boss(Fighter * player, Boss * b, Path * p,
                     foeTurnOption_OP foe_op, Rectangle * notification_area,
                     Koliseo * kls)
{
    return -1;
}

int defer_skill_boss(Fighter *player, Boss *b, skillType picked_skill, Path *p, foeTurnOption_OP foe_op,
                     Rectangle * notification_area, Koliseo *kls)
{
    return -1;
}

int boss_fight(Fighter * player, Boss * b, Path * p, Rectangle * notification_area,
               Koliseo * kls)
{
    return -1;
}

int boss_attack(Boss * b, Fighter * target, Path * p, Rectangle * notification_area,
                Koliseo * kls)
{
    return -1;
}
fightResult do_Skill(Fighter * player, Enemy * e, skillType picked_skill, Rectangle * notification_area, Koliseo * kls)
{
    return FIGHTRES_NO_DMG;
}
fightResult do_Skill_boss(Fighter * player, Boss * b, skillType picked_skill, Path * path, Rectangle * notification_area, Koliseo * kls)
{
    return FIGHTRES_NO_DMG;
}
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
