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
#include "game_init.h"

/**
 * Takes a Fighter pointer and prepares its perks field by allocating all the Perk accounted in perkClass.
 * Perks are initialised using the ordering of perkClass.
 * @see Fighter
 * @see Perk
 * @see perkClass
 * @see PERKSMAX
 * @param f The Fighter pointer whose perks field will be initialised.
 * @param kls The Koliseo used for allocation.
 */
void initPerks(Fighter *f, Koliseo *kls)
{
    f->perksCount = 0;
    //Ordering of i corresponds to perksClass enum
    int total = (PERKSMAX + 1);
    for (int i = 0; i < total; i++) {
        kls_log(kls, "DEBUG", "Prepping Perk (%i)", i);
        Perk *p =
            (Perk *) KLS_PUSH_TYPED(kls, Perk, HR_Perk, "Perk", "Perk");
        p->class = i;
        char *name = nameStringFromPerk(i);
        char *desc = descStringFromPerk(i);

        //p->name = name; //(char*)malloc(sizeof(name));
        strcpy(p->name, name);

        //p->desc = (char*)malloc(sizeof(desc));
        strcpy(p->desc, desc);

        p->innerValue = 0;

        f->perks[i] = p;
    }
}

/**
 * Takes a Fighter pointer and prepares its consumablesBag field by allocating a Consumable for each consumableClass.
 * @see Fighter
 * @see Consumable
 * @see consumableClass
 * @see CONSUMABLESMAX
 * @param f The Fighter pointer whose consumablesBag field will be initialised.
 * @param kls The Koliseo to do allocations.
 */
void initConsumableBag(Fighter *f, Koliseo *kls)
{

    for (int i = 0; i < CONSUMABLESMAX + 1; i++) {
        kls_log(kls, "DEBUG", "Prepping Consumable (%i)", i);
        Consumable *c =
            (Consumable *) KLS_PUSH_TYPED(kls, Consumable, HR_Consumable,
                                          "Consumable", "Consumable");
        c->class = i;

        Consumable *base = &consumablesBase[i];

        strcpy(c->name, base->name);
        strcpy(c->desc, base->desc);
        for (int j = 0; j < 8; j++) {
            strcpy(c->sprite[j], base->sprite[j]);
        }
        c->qty = 0;

        f->consumablesBag[i] = (Consumable *)c;
    }

}

/**
 * Takes a Fighter pointer and prepares its counters field by allocating all the Turncounter accounted in counterIndexes.
 * Counters are initialised using the ordering of counterIndexes, primarily for distinguinsing what kind of function pointer will be registered in the counter.
 * For a given Counter, only the correct pointer field is assigned and the others are left untouched.
 * @see Fighter
 * @see Turncounter
 * @see Countertype
 * @see COUNTERSMAX
 * @see counterIndexes
 * @see getStatBoostCounterFun()
 * @see getStatusCounterFun()
 * @param f The Fighter pointer whose counters field will be initialised.
 * @param kls The Koliseo used for allocation.
 */
void initCounters(Fighter *f, Koliseo *kls)
{
    //Ordering of i corresponds to counterIndexes enum
    int total = (COUNTERSMAX + 1);
    for (int i = 0; i < total; i++) {
        log_tag("debug_log.txt", "[DEBUG]", "Prepping Turncounter (%i)", i);
        kls_log(kls, "DEBUG", "Prepping Turncounter (%i)", i);
        Turncounter *c =
            (Turncounter *) KLS_PUSH_TYPED(kls, Turncounter, HR_Turncounter,
                                           "Turncounter", "Turncounter");

        //First, prepare counters for statuses
        if (i < STATUSMAX + 1) {
            c->desc =
                (char *)KLS_PUSH_ARR_TYPED(kls, char *, sizeof(stringFromStatus(i)),
                                           HR_Turncounter_desc, "Turncounter desc",
                                           "Turncounter desc");
            strcpy(c->desc, stringFromStatus(i));
            log_tag("debug_log.txt", "[DEBUG]",
                    "Allocated size %lu for status counter: (%s)",
                    sizeof(stringFromStatus(i)), c->desc);
            kls_log(kls, "DEBUG", "Allocated size %lu for status counter: (%s)",
                    sizeof(stringFromStatus(i)), c->desc);

            c->effect_fun = getStatusCounterFun(i);
            //sprintf(msg,"Status function pointer is: (%i)", (int) *(c->effect_fun));
            //log_tag("debug_log.txt","[DEBUG]",msg);
            c->type = CNT_STATUS;

            //Register function for the specific status counter
            //
            //REGISTER_COUNTER_CALLBACK(i,resetFighterStatus);
            //
            //Why does uncommenting this cause problems to special moves callback?
            //More specific, ONLY MAGE MOVES
            //debug
            //printf("%s",stringFromStatus(j));
        } else {		//Then, counters for boosts to (all?) stats

            switch (i) {
            case TURNBOOST_ATK: {
                c->desc =
                    (char *)KLS_PUSH_ARR_TYPED(kls, char *, sizeof("ATK boost"),
                                               HR_Turncounter_desc,
                                               "Turncounter desc",
                                               "Turncounter desc");
                strcpy(c->desc, "ATK boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("ATK boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("ATK boost"), c->desc);

                c->boost_fun = getStatBoostCounterFun(ATK);
                c->type = CNT_ATKBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            case TURNBOOST_DEF: {
                c->desc =
                    (char *)KLS_PUSH_ARR_TYPED(kls, char *, sizeof("DEF boost"),
                                               HR_Turncounter_desc,
                                               "Turncounter desc",
                                               "Turncounter desc");
                strcpy(c->desc, "DEF boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("DEF boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("DEF boost"), c->desc);

                c->boost_fun = getStatBoostCounterFun(DEF);
                c->type = CNT_DEFBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            case TURNBOOST_VEL: {
                c->desc =
                    (char *)KLS_PUSH_ARR_TYPED(kls, char *, sizeof("VEL boost"),
                                               HR_Turncounter_desc,
                                               "Turncounter desc",
                                               "Turncounter desc");
                strcpy(c->desc, "VEL boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("VEL boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("VEL boost"), c->desc);

                c->boost_fun = getStatBoostCounterFun(VEL);
                c->type = CNT_VELBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            case TURNBOOST_ENR: {
                c->desc =
                    (char *)KLS_PUSH_ARR_TYPED(kls, char *, sizeof("ENR boost"),
                                               HR_Turncounter_desc,
                                               "Turncounter desc",
                                               "Turncounter desc");
                strcpy(c->desc, "ENR boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("ENR boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("ENR boost"), c->desc);

                c->boost_fun = getStatBoostCounterFun(ENR);
                c->type = CNT_ENRBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            default: {
                printf
                ("\n\nERROR in initCounters(): unexpected i: %i value \n\n",
                 i);
                exit(EXIT_FAILURE);
            }
            }
        }			//End else
        c->count = 0;
        c->innerValue = 0;
        f->counters[i] = (struct Turncounter *)c;
    };				//End for all counters
}

/**
 * Takes a Enemy pointer and prepares its counters field by allocating all the Turncounter accounted in counterIndexes.
 * Counters are initialised using the ordering of counterIndexes, primarily for distinguinsing what kind of function pointer will be registered in the counter.
 * For a given Counter, only the correct pointer field is assigned and the others are left untouched.
 * @see Enemy
 * @see Turncounter
 * @see Countertype
 * @see COUNTERSMAX
 * @see counterIndexes
 * @see getStatBoostCounterEnemyFun()
 * @see getStatusCounterEnemyFun()
 * @param e The Enemy pointer whose counters field will be initialised.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initECounters(Enemy *e, Koliseo_Temp *t_kls)
{
    //Ordering of i corresponds to counterIndexes enum
    int total = (COUNTERSMAX + 1);
    Koliseo *kls = t_kls->kls;
    for (int i = 0; i < total; i++) {
        log_tag("debug_log.txt", "[DEBUG]", "Prepping enemy Turncounter (%i)",
                i);
        kls_log(kls, "DEBUG", "Prepping enemy Turncounter (%i)", i);
        Turncounter *c =
            (Turncounter *) KLS_PUSH_T_TYPED(t_kls, Turncounter,
                                             HR_Turncounter, "Turncounter",
                                             "Turncounter");

        //First, prepare counters for statuses
        if (i < STATUSMAX + 1) {
            c->desc =
                (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                             sizeof(stringFromStatus(i)),
                                             HR_Turncounter_desc,
                                             "Turncounter desc",
                                             "Turncounter desc");
            strcpy(c->desc, stringFromStatus(i));
            log_tag("debug_log.txt", "[DEBUG]",
                    "Allocated size %lu for enemy status counter: (%s)",
                    sizeof(stringFromStatus(i)), c->desc);
            kls_log(kls, "DEBUG",
                    "Allocated size %lu for enemy status counter: (%s)",
                    sizeof(stringFromStatus(i)), c->desc);

            c->effect_e_fun = getStatusCounterEnemyFun(i);
            //sprintf(msg,"[DEBUG]    Enemy status function pointer is: (%i)", *(c->effect_e_fun));
            //log_tag("debug_log.txt","[DEBUG]",msg);
            c->type = CNT_STATUS;

        } else {		//Then, counters for boosts to (all?) stats

            switch (i) {
            case TURNBOOST_ATK: {
                c->desc =
                    (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                                 sizeof("ATK boost"),
                                                 HR_Turncounter_desc,
                                                 "Turncounter desc",
                                                 "Turncounter desc");
                strcpy(c->desc, "ATK boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("ATK boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("ATK boost"), c->desc);

                c->boost_e_fun = getStatBoostCounterEnemyFun(ATK);
                c->type = CNT_ATKBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            case TURNBOOST_DEF: {
                c->desc =
                    (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                                 sizeof("DEF boost"),
                                                 HR_Turncounter_desc,
                                                 "Turncounter desc",
                                                 "Turncounter desc");
                strcpy(c->desc, "DEF boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("DEF boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("DEF boost"), c->desc);

                c->boost_e_fun = getStatBoostCounterEnemyFun(DEF);
                c->type = CNT_DEFBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            case TURNBOOST_VEL: {
                c->desc =
                    (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                                 sizeof("VEL boost"),
                                                 HR_Turncounter_desc,
                                                 "Turncounter desc",
                                                 "Turncounter desc");
                strcpy(c->desc, "VEL boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("VEL boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("VEL boost"), c->desc);

                c->boost_e_fun = getStatBoostCounterEnemyFun(VEL);
                c->type = CNT_VELBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            case TURNBOOST_ENR: {
                c->desc =
                    (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                                 sizeof("ENR boost"),
                                                 HR_Turncounter_desc,
                                                 "Turncounter desc",
                                                 "Turncounter desc");
                strcpy(c->desc, "ENR boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("ENR boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("ENR boost"), c->desc);

                c->boost_e_fun = getStatBoostCounterEnemyFun(ENR);
                c->type = CNT_ENRBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "ERROR INITALISING TURNCOUNTER in initECounters()\n");
                exit(EXIT_FAILURE);
            }
            }
        }			//End else
        c->count = 0;
        c->innerValue = 0;
        e->counters[i] = (struct Turncounter *)c;
    };
}

/**
 * Takes a Boss pointer and prepares its counters field by allocating all the Turncounter accounted in counterIndexes.
 * Counters are initialised using the ordering of counterIndexes, primarily for distinguinsing what kind of function pointer will be registered in the counter.
 * For a given Counter, only the correct pointer field is assigned and the others are left untouched.
 * @see Boss
 * @see Turncounter
 * @see Countertype
 * @see COUNTERSMAX
 * @see counterIndexes
 * @see getStatBoostCounterBossFun()
 * @see getStatusCounterBossFun()
 * @param b The Boss pointer whose counters field will be initialised.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initBCounters(Boss *b, Koliseo_Temp *t_kls)
{
    //Ordering of i corresponds to counterIndexes enum
    int total = (COUNTERSMAX + 1);
    Koliseo *kls = t_kls->kls;
    for (int i = 0; i < total; i++) {
        log_tag("debug_log.txt", "[DEBUG]", "Prepping boss counter %i", i);
        kls_log(kls, "DEBUG", "Prepping boss counter %i", i);
        Turncounter *c =
            (Turncounter *) KLS_PUSH_T_TYPED(t_kls, Turncounter,
                                             HR_Turncounter, "Turncounter",
                                             "Turncounter");

        //First, prepare counters for statuses
        if (i < STATUSMAX + 1) {
            c->desc =
                (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                             sizeof(stringFromStatus(i)),
                                             HR_Turncounter_desc,
                                             "Turncounter desc",
                                             "Turncounter desc");
            strcpy(c->desc, stringFromStatus(i));
            log_tag("debug_log.txt", "[DEBUG]",
                    "Allocated size %lu for boss status counter: (%s)",
                    sizeof(stringFromStatus(i)), c->desc);
            kls_log(kls, "DEBUG",
                    "Allocated size %lu for boss status counter: (%s)",
                    sizeof(stringFromStatus(i)), c->desc);

            c->effect_b_fun = getStatusCounterBossFun(i);
            //sprintf(msg,"Boss status function pointer is: (%i)", *(c->effect_b_fun));
            //log_tag("debug_log.txt","[DEBUG]",msg);
            c->type = CNT_STATUS;

        } else {		//Then, counters for boosts to (all?) stats

            switch (i) {
            case TURNBOOST_ATK: {
                c->desc =
                    (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                                 sizeof("ATK boost"),
                                                 HR_Turncounter_desc,
                                                 "Turncounter desc",
                                                 "Turncounter desc");
                strcpy(c->desc, "ATK boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for boss stat counter: (%s)",
                        sizeof("ATK boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for boss stat counter: (%s)",
                        sizeof("ATK boost"), c->desc);

                c->boost_b_fun = getStatBoostCounterBossFun(ATK);
                c->type = CNT_ATKBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_b_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            case TURNBOOST_DEF: {
                c->desc =
                    (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                                 sizeof("DEF boost"),
                                                 HR_Turncounter_desc,
                                                 "Turncounter desc",
                                                 "Turncounter desc");
                strcpy(c->desc, "DEF boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for boss stat counter: (%s)",
                        sizeof("DEF boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for boss stat counter: (%s)",
                        sizeof("DEF boost"), c->desc);

                c->boost_b_fun = getStatBoostCounterBossFun(DEF);
                c->type = CNT_DEFBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_b_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            case TURNBOOST_VEL: {
                c->desc =
                    (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                                 sizeof("VEL boost"),
                                                 HR_Turncounter_desc,
                                                 "Turncounter desc",
                                                 "Turncounter desc");
                strcpy(c->desc, "VEL boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for boss stat counter: (%s)",
                        sizeof("VEL boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for boss stat counter: (%s)",
                        sizeof("VEL boost"), c->desc);

                c->boost_b_fun = getStatBoostCounterBossFun(VEL);
                c->type = CNT_VELBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_b_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            case TURNBOOST_ENR: {
                c->desc =
                    (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                                 sizeof("ENR boost"),
                                                 HR_Turncounter_desc,
                                                 "Turncounter desc",
                                                 "Turncounter desc");
                strcpy(c->desc, "ENR boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for boss stat counter: (%s)",
                        sizeof("ENR boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for boss stat counter: (%s)",
                        sizeof("ENR boost"), c->desc);

                c->boost_b_fun = getStatBoostCounterBossFun(ENR);
                c->type = CNT_ENRBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_b_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "ERROR INITALISING TURNCOUNTER in initBCounters()\n");
                exit(EXIT_FAILURE);
            }
            }
        }			//End else
        c->count = 0;
        c->innerValue = 0;
        b->counters[i] = (struct Turncounter *)c;
    };
}

/**
 * Takes a FoeParty pointer and prepares its counters field by allocating all the Turncounter accounted in counterIndexes.
 * Counters are initialised using the ordering of counterIndexes, primarily for distinguinsing what kind of function pointer will be registered in the counter.
 * For a given Counter, only the correct pointer field is assigned and the others are left untouched.
 * @see FoeParty
 * @see Turncounter
 * @see Countertype
 * @see COUNTERSMAX
 * @see counterIndexes
 * @see get_StatBoostCounter_FoeParty_Fun()
 * @see get_StatusCounter_FoeParty_Fun()
 * @param fp The FoeParty pointer whose counters field will be initialised.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initFoePartyCounters(FoeParty *fp, Koliseo_Temp *t_kls)
{
    //Ordering of i corresponds to counterIndexes enum
    int total = (COUNTERSMAX + 1);
    Koliseo *kls = t_kls->kls;
    for (int i = 0; i < total; i++) {
        log_tag("debug_log.txt", "[DEBUG]", "Prepping foeparty counter %i", i);
        kls_log(kls, "DEBUG", "Prepping foeparty counter %i", i);
        Turncounter *c =
            (Turncounter *) KLS_PUSH_T_TYPED(t_kls, Turncounter,
                                             HR_Turncounter, "Turncounter",
                                             "Turncounter");

        //First, prepare counters for statuses
        if (i < STATUSMAX + 1) {
            c->desc =
                (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                             sizeof(stringFromStatus(i)),
                                             HR_Turncounter_desc,
                                             "Turncounter desc",
                                             "Turncounter desc");
            strcpy(c->desc, stringFromStatus(i));
            log_tag("debug_log.txt", "[DEBUG]",
                    "Allocated size %lu for foeparty status counter: (%s)",
                    sizeof(stringFromStatus(i)), c->desc);
            kls_log(kls, "DEBUG",
                    "Allocated size %lu for foeparty status counter: (%s)",
                    sizeof(stringFromStatus(i)), c->desc);

            //c->effect_fp_fun = get_StatusCounter_FoeParty_Fun(i);
            //sprintf(msg,"Foeparty status function pointer is: (%i)", *(c->effect_b_fun));
            //log_tag("debug_log.txt","[DEBUG]",msg);
            c->type = CNT_STATUS;

        } else {		//Then, counters for boosts to (all?) stats

            switch (i) {
            case TURNBOOST_ATK: {
                c->desc =
                    (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                                 sizeof("ATK boost"),
                                                 HR_Turncounter_desc,
                                                 "Turncounter desc",
                                                 "Turncounter desc");
                strcpy(c->desc, "ATK boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("ATK boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("ATK boost"), c->desc);

                c->boost_fp_fun = get_StatBoostCounter_FoeParty_Fun(ATK);
                c->type = CNT_ATKBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fp_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            case TURNBOOST_DEF: {
                c->desc =
                    (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                                 sizeof("DEF boost"),
                                                 HR_Turncounter_desc,
                                                 "Turncounter desc",
                                                 "Turncounter desc");
                strcpy(c->desc, "DEF boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("DEF boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("DEF boost"), c->desc);

                c->boost_fp_fun = get_StatBoostCounter_FoeParty_Fun(DEF);
                c->type = CNT_DEFBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fp_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            case TURNBOOST_VEL: {
                c->desc =
                    (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                                 sizeof("VEL boost"),
                                                 HR_Turncounter_desc,
                                                 "Turncounter desc",
                                                 "Turncounter desc");
                strcpy(c->desc, "VEL boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("VEL boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("VEL boost"), c->desc);

                c->boost_fp_fun = get_StatBoostCounter_FoeParty_Fun(VEL);
                c->type = CNT_VELBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fp_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            case TURNBOOST_ENR: {
                c->desc =
                    (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *,
                                                 sizeof("ENR boost"),
                                                 HR_Turncounter_desc,
                                                 "Turncounter desc",
                                                 "Turncounter desc");
                strcpy(c->desc, "ENR boost");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("ENR boost"), c->desc);
                kls_log(kls, "DEBUG",
                        "Allocated size %lu for status counter: (%s)",
                        sizeof("ENR boost"), c->desc);

                c->boost_fp_fun = get_StatBoostCounter_FoeParty_Fun(ENR);
                c->type = CNT_ENRBOOST;
                //sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fp_fun));
                //log_tag("debug_log.txt","[DEBUG]",msg);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Invalid counter in initFoePartyCounters");
                exit(EXIT_FAILURE);
            }
            }
        }			//End else
        c->count = 0;
        c->innerValue = 0;
        fp->counters[i] = (struct Turncounter *)c;
    };
}

/**
 * Takes a Fighter pointer and prepares its equipslots field by allocating an Equipslot for each Equipzone.
 * @see Fighter
 * @see Equipslot
 * @see Equipzone
 * @see counterIndexes
 * @see EQUIPZONES
 * @param f The Fighter pointer whose equipslots field will be initialised.
 * @param kls The Koliseo used for allocations.
 */
void initEquipSlots(Fighter *f, Koliseo *kls)
{
    for (int i = 0; i <= EQUIPZONES; i++) {
        kls_log(kls, "DEBUG", "Prepping Equipslot (%i)", i);
        Equipslot *s =
            (Equipslot *) KLS_PUSH_TYPED(kls, Equipslot, HR_Equipslot,
                                         "Equipslot", "Equipslot");
        s->active = 0;
        s->type = i;
        setEquipslotSprite(s);
        f->equipslots[i] = (struct Equipslot *)s;
    };
}

/**
 * Takes a Fighter pointer and prepares its artifactsBag field by allocating a Artifact for each artifactClass.
 * @see Fighter
 * @see Artifact
 * @see artifactClass
 * @see ARTIFACTSMAX
 * @param kls The Koliseo used for allocations.
 * @param f The Fighter pointer whose artifactsBag field will be initialised.
 */
void initArtifactsBag(Fighter *f, Koliseo *kls)
{
    for (int i = 0; i < ARTIFACTSMAX + 1; i++) {
        kls_log(kls, "DEBUG", "Prepping Artifact (%i)", i);
        Artifact *a =
            (Artifact *) KLS_PUSH_TYPED(kls, Artifact, HR_Artifact,
                                        "Artifact", "Artifact");
        a->class = i;

        Artifact *base = &artifactsBase[i];

        strcpy(a->name, base->name);
        strcpy(a->desc, base->desc);
        for (int j = 0; j < 8; j++) {
            strcpy(a->sprite[j], base->sprite[j]);
        }
        a->qty = 0;

        f->artifactsBag[i] = (struct Artifact *)a;
    }
}

/**
 * Takes a Fighter pointer and prepares its equipsBag field by allocating an Equip for each array slot.
 * @see Fighter
 * @see Equip
 * @see ARTIFACTSMAX
 * @param kls The Koliseo used for allocations.
 * @param f The Fighter pointer whose equipsBag field will be initialised.
 */
void initEquipsBag(Fighter * f, Koliseo * kls)
{
    for (int i = 0; i < EQUIPSBAGSIZE; i++) {
        kls_log(kls, "DEBUG", "Allocating space for Equip (%i)", i);
        Equip *e =
            (Equip *) KLS_PUSH_TYPED(kls, Equip, HR_Equip,
                                     "Equip", "Fighter bag equip");
        e->class = -1; // Setting an invalid class
        for (int pix = 0; pix < (EQUIPPERKSMAX); pix++) {
            e->perks[pix] = (Perk *) KLS_PUSH_TYPED(kls, Perk, HR_Perk, "Perk", "Fighter bag perk");
        }
        f->equipsBag[i] = e;
    }
}

/**
 * Takes one Fighter and one Path pointers and initialises the fighter fields.
 * Luck value is set as path luck value modulo MAXPLAYERLUCK.
 * The BaseStats pointer for the fighter's figtherClass is loaded.
 * The stats field is initalised with all inner values at 0.
 * setSpecials(), initCounters() are called to init the specialslots and counters fields.
 * initEquipSlots() is called to init the equipslots field, and the relative int field are set to 0.
 * initArtifactsFun() is called to init all the Artifact effect_fun field.
 * All the tempboost_STAT and permboost_STAT fields are set to 0.
 * All the fields common to BaseStats are initialised with the base value.
 * The status field is set to Normal.
 * @see Fighter
 * @see fighterClass
 * @see Path
 * @see BaseStats
 * @see countStats
 * @see MAXPLAYERLUCK
 * @see setSpecials()
 * @see initCounters()
 * @see initEquipSlots()
 * @see initArtifactsFun()
 * @see fighterStatus
 * @param player The Fighter whose fields will be initialised.
 * @param path The Path pointer of the current game.
 * @param kls The Koliseo used for allocation.
 */
void initPlayerStats(Fighter *player, Path *path, Koliseo *kls)
{

    //player luck : MAXPLAYERLUCK = path luck : MAXLUCK

    player->luck = (path->luck * MAXPLAYERLUCK) / MAXLUCK;

    BaseStats *base = &basestats[player->class];

    kls_log(kls, "DEBUG", "Prepping countStats");
    countStats *s =
        (countStats *) KLS_PUSH_TYPED(kls, countStats, HR_countStats,
                                      "countStats", "countStats");

    s->enemieskilled = 0;
    s->criticalhits = 0;
    s->consumablesfound = 0;
    s->equipsfound = 0;
    s->artifactsfound = 0;
    s->roomscompleted = 0;
    s->floorscompleted = 0;
    s->specialsunlocked = 0;
    s->coinsfound = 0;
    s->bosseskilled = 0;
    s->unique_bosseskilled = 0;
    for (int i = 0; i < BOSSCLASSESMAX + 1; i++) {
        s->killed_bosses[i] = 0;
    }
    s->keysfound = 0;

    setSpecials(player, kls);
    setSkills(player, kls);
    initCounters(player, kls);
    initPerks(player, kls);

    initConsumableBag(player, kls);
    initArtifactsBag(player, kls);
    initEquipsBag(player, kls);

    initEquipSlots(player, kls);
    player->equipsBagOccupiedSlots = 0;	//Keeps track of how many slots are occupied.
    player->earliestBagSlot = 0;	//To alwasy use the array efficiently (???) I sense linked lists may be better

    player->permboost_atk = 0;
    player->permboost_def = 0;
    player->permboost_vel = 0;
    player->permboost_enr = 0;

    player->equipboost_atk = 0;
    player->equipboost_def = 0;
    player->equipboost_vel = 0;
    player->equipboost_enr = 0;

    player->balance = 0;
    player->keys_balance = 0;

    player->stats = s;
    player->hp = base->hp;
    player->totalhp = base->totalhp;
    player->atk = base->atk;
    player->def = base->def;
    player->vel = base->vel;
    player->level = base->level;
    player->totalxp = base->totalxp;
    player->currentlevelxp = base->currentlevelxp;
    player->totallevelxp = base->totallevelxp;
    player->status = Normal;
    player->totalenergy = base->totalenergy;
    player->energy = player->totalenergy;
    player->totalstamina = base->totalstamina;
    player->stamina = player->totalstamina;

    setFighterSprite(player);

    player->floor_x = -1;
    player->floor_y = -1;
}

/**
 * Takes one Enemy pointer and initialises the enemy fields.
 * The EnemyBaseStats pointer for the enemy's enemyClass is loaded.
 * If the beast field at the pointer is already set before this call, atk def vel and hp of the enemy will receive a multiplicative boost equal to BSTFACTOR . Also, Xp value will be multiplied by 3.
 * Notably, the level field is not set at all by this function and needs to be set by the caller.
 * All the fields common to EnemyBaseStats are initialised with the base value and eventual beast boost.
 * initECounters() is called to init the counters field.
 * The status field is set to Normal.
 * @see Enemy
 * @see enemyClass
 * @see EnemyBaseStats
 * @see initECounters()
 * @see fighterStatus
 * @param e The Enemy whose fields will be initialised.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initEnemyStats(Enemy *e, Koliseo_Temp *t_kls)
{
    EnemyBaseStats *base = &baseenemystats[e->class];
    log_tag("debug_log.txt", "[DEBUG]", "Init stats for enemy (%s)",
            stringFromEClass(e->class));

    float beastf = 1;

    if (e->beast) {
        beastf = BSTFACTOR;
    }

    e->hp = round(beastf * base->hp);
    e->totalhp = round(beastf * base->totalhp);
    e->atk = round(beastf * base->atk);
    e->def = round(beastf * base->def);
    e->vel = round(beastf * base->vel);
    e->status = Normal;

    //Level should be set by caller
    //Index should be set by caller

    e->totalenergy = base->totalenergy;
    e->energy = e->totalenergy;
    e->totalstamina = base->totalstamina;
    e->stamina = e->totalstamina;

    //Set prize, double for beasts
    float prize = 2.8 * e->level;

    e->prize = floor((e->beast) ? 2 * prize : prize);

    initECounters(e, t_kls);

    //Triple xp for beasts
    e->xp = (e->beast) ? 3 * base->xp : base->xp;

    setEnemySprite(e);
}

/**
 * Takes one Boss pointer and initialises the boss fields.
 * The BossBaseStats pointer for the boss's bossClass is loaded.
 * If the beast field at the pointer is already set before this call, atk def vel and hp of the enemy will receive a multiplicative boost equal to BSTFACTOR . Xp value will also be multiplie by 3.
 * Notably, the level field is not set at all by this function and needs to be set by the caller.
 * All the fields common to BossBaseStats are initialised with the base value and eventual beast boost.
 * initBCounters() is called to init the counters field.
 * The status field is set to Normal.
 * @see Boss
 * @see bossClass
 * @see BossBaseStats
 * @see initBCounters()
 * @see fighterStatus
 * @param b The Boss whose fields will be initialised.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initBossStats(Boss *b, Koliseo_Temp *t_kls)
{
    //Class should be set by caller
    BossBaseStats *base = &basebossstats[b->class];

    float beastf = 1;

    if (b->beast) {
        beastf = BSTFACTOR;
    }

    b->hp = round(beastf * base->hp);
    b->totalhp = round(beastf * base->totalhp);
    b->atk = round(beastf * base->atk);
    b->def = round(beastf * base->def);
    b->vel = round(beastf * base->vel);
    b->status = Normal;

    //Level should be set by caller

    b->totalenergy = base->totalenergy;
    b->energy = b->totalenergy;

    b->totalstamina = base->totalstamina;
    b->stamina = b->totalstamina;

    //Set prize, double for beasts
    float prize = 4.5 * b->level;

    b->prize = (b->beast) ? 2 * prize : prize;

    initBCounters(b, t_kls);

    //Triple xp for beasts
    b->xp = (b->beast) ? 3 * base->xp : base->xp;

    setBossSprite(b);
}

/**
 * Takes one FoeParty pointer and initialises the party fields.
 * The FoePartyBaseStats pointer for the FoeParty's foePartyClass is loaded.
 * All the fields common to FoePartyBaseStats are initialised with the base value.
 * initFoePartyCounters() is called to init the counters field.
 * @see FoeParty
 * @see foePartyClass
 * @see FoePartyBaseStats
 * @see initFoePartyCounters()
 * @param fp The FoeParty whose fields will be initialised.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initFoePartyStats(FoeParty *fp, Koliseo_Temp *t_kls)
{
    //Class should be set by caller
    //FoePartyBaseStats* base = &basefoepartystats[fp->class];
    //TODO: add loading of more base stats not expected in prepareFoeParty()

    //Level should be set by caller

    //Zero the alive_enemies values
    for (int i = 0; i < ROOM_ENEMIES_MAX + 1; i++) {
        fp->alive_enemies[i] = 0;
    };
    //Zero the alive_bosses values
    for (int i = 0; i < FOES_BOSSES_MAX + 1; i++) {
        fp->alive_bosses[i] = 0;
    };

    //Zero the turnboost_STAT values
    fp->turnboost_atk = 0;
    fp->turnboost_def = 0;
    fp->turnboost_vel = 0;
    fp->turnboost_enr = 0;

    //Zero current enemy index
    fp->current_index = 0;

    //Size value should be set by caller
    fp->tot_alive = fp->size;

    switch (fp->class) {
    case Enemies: {
        //Set alive_enemies values
        for (int i = 0; i < fp->tot_alive + 1; i++) {
            fp->alive_enemies[i] = 1;
        };
    }
    break;
    case Bosses: {
        //Set alive_bosses values
        for (int i = 0; i < fp->tot_alive + 1; i++) {
            fp->alive_bosses[i] = 1;
        };
    }
    break;
    default: {
        fprintf(stderr, "UNEXPECTED FOEPARTY CLASS VALUE %i", fp->class);
        log_tag("debug_log.txt", "[PANIC]",
                "Invalid class value in initFoePartyStats()");
        exit(EXIT_FAILURE);
    }
    break;
    };

    //Init party counters
    initFoePartyCounters(fp, t_kls);
}

/**
 * Takes a FoeParty pointer, a size for complete party, and an integer for the current room index, and initialises all the fields making the FoeParty ready for use in battle.
 * Calls initFoePartyStats() after setting class and level
 * @see FoeParty
 * @see initFoePartyStats()
 * @param fp The allocated FoeParty pointer to initialise.
 * @param partysize Size of party.
 * @param roomindex Index of current room.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void prepareFoeParty(FoeParty *fp, int partysize, int roomindex,
                     Koliseo_Temp *t_kls)
{

    //Class must be set by caller

    FoePartyBaseStats *base = &basefoepartystats[fp->class];

    fp->level = base->level;
    //Set size value
    fp->size = partysize;

    //FoeParty's get 1 level every 2 rooms
    //fp->level += floor(roomindex / 2) ;

    //Load foeparty stats
    initFoePartyStats(fp, t_kls);

    log_tag("debug_log.txt", "[DEBUG]",
            "Prepared FoeParty with size (%i), room #%i.", partysize,
            roomindex);
}

/**
 * Takes a Fighter pointer and prepares its skillSlot fields by allocating FIGHTER_SKILL_SLOTS slots.
 * Skill slots are initialised.
 * @see Fighter
 * @see Skilllot
 * @see FIGHTER_SKILL_SLOTS
 * @see SKILL_TYPE_LAST_UNLOCKABLE
 * @see costFromSkill()
 * @see stringFromSkill()
 * @param kls The Koliseo used for allocations.
 * @param f The Fighter pointer whose skill slots will be initialised.
 */
void setSkills(Fighter *f, Koliseo *kls)
{
    char movename[80];
    char movedesc[80];
    for (int i = 0; i <= FIGHTER_SKILL_SLOTS; i++) {
        kls_log(kls, "DEBUG", "Prepping Skillslot (%i)", i);
        Skillslot *s =
            (Skillslot *) KLS_PUSH_TYPED(kls, Skillslot, HR_Skillslot,
                                         "Skillslot", "Skillslot");
        s->enabled = 0;
        s->class = i;
        s->cost = costFromSkill(i);
        strcpy(movename, nameStringFromSkill(i));
        strcpy(movedesc, descStringFromSkill(i));
        //printf("DEBUG\n%i\t%s\n",(i),stringFromSkill(i));
        strcpy(s->name, movename);
        strcpy(s->desc, movedesc);
        f->skills[i] = s;
    };
}

/**
 * Takes a Chest and Fighter pointers and initialises all the fields in chest based on chest class and fighter stats.
 * @see Chest
 * @param c The allocated Chest pointer with already set class to initialise.
 * @param f The Fighter pointer with stats.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initChest(Chest *c, Fighter *f, Koliseo_Temp *t_kls)
{

    setChestSprite(c);

    strcpy(c->desc, descStringFromChest(c->class));
    int cons_cnt, equip_cnt;
    Koliseo *kls = t_kls->kls;

    switch (c->class) {
    case CHEST_BASE: {
        cons_cnt = rand() % (CHEST_CONSUMABLES_MAX - 1);
        equip_cnt = rand() % (CHEST_EQUIPS_MAX - 1);
    }
    break;
    case CHEST_BEAST: {
        cons_cnt = (rand() % (CHEST_CONSUMABLES_MAX)) + 1;
        equip_cnt = (rand() % (CHEST_EQUIPS_MAX)) + 1;

    }
    break;
    default: {
        log_tag("debug_log.txt", "[ERROR]",
                "%i is not a valid chest class.", c->class);
        exit(EXIT_FAILURE);
    }
    break;

    }

    c->consumablesCount = cons_cnt;
    c->equipsCount = equip_cnt;

    if (c->consumablesCount > 0) {
        for (int i = 0; i < c->consumablesCount; i++) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Prepping Consumable (%i/%i) for Chest", i,
                    c->consumablesCount);
            kls_log(kls, "DEBUG", "Prepping Consumable (%i/%i) for Chest", i,
                    c->consumablesCount);
            Consumable *cns =
                (Consumable *) KLS_PUSH_T_TYPED(t_kls, Consumable,
                                                HR_Consumable, "Consumable",
                                                "Consumable");
            int drop = rand() % (CONSUMABLESMAX + 1);

            cns->class = drop;

            Consumable *base = &consumablesBase[drop];

            strcpy(cns->name, base->name);
            strcpy(cns->desc, base->desc);
            for (int j = 0; j < 8; j++) {
                strcpy(cns->sprite[j], base->sprite[j]);
            }
            cns->qty = 1;

            c->consumables[i] = cns;
        }
    }

    if (c->equipsCount > 0) {
        for (int i = 0; i < c->equipsCount; i++) {

            //Select a basic item from the list
            int drop = rand() % (EQUIPSMAX + 1);
            //Randomise quality
            quality q = rand() % (QUALITIESMAX + 1);

            //Prepare the item
            log_tag("debug_log.txt", "[DEBUG]",
                    "Prepping Equip (%i/%i) for Chest", i, c->equipsCount);
            kls_log(kls, "DEBUG", "Prepping Equip (%i/%i) for Chest", i,
                    c->equipsCount);
            Equip *e =
                (Equip *) KLS_PUSH_T_TYPED(t_kls, Equip, HR_Equip, "Equip",
                                           "Equip");

            //Get the base item and copy the stats to the drop
            Equip *base = &equips[drop];

            e->class = base->class;
            e->type = base->type;
            e->qual = q;

            setEquipSprite(e);
            strcpy(e->name, base->name);
            strcpy(e->desc, base->desc);

            e->qty = 1;
            e->equipped = 0;

            e->perksCount = 0;

            //Calc drop level
            e->level = base->level + round(f->level / EQUIPLVLBOOSTRATIO);

            //Chance for better leveled item
            if ((rand() % 8) - (f->luck / 10) <= 0) {	//Should use a defined constant
                e->level += 1;	//At least a simple +1
                if ((rand() % 25) - (f->luck / 10) <= 0) {	//Should use a defined constant
                    e->level += 1;	//A bonus roll for another +1

                }
            }

            float boostFactor = 0.7;

            float lvlBoost = boostFactor * (e->level - 1);

            e->atk = round((base->atk * 1.0) + lvlBoost);
            e->def = round((base->def * 1.0) + lvlBoost);
            e->vel = round((base->vel * 1.0) + lvlBoost);
            e->enr = round((base->enr * 1.0) + lvlBoost);

            //Bonus stats on better quality items? Simple for now

            if (q == Good) {
                e->atk += (rand() % 3);	//Should use a defined constant
                e->def += (rand() % 3);	//Should use a defined constant
                e->vel += (rand() % 3);	//Should use a defined constant
                e->enr += (rand() % 2);	//Should use a defined constant
            } else if (q == Bad) {
                e->atk -= (rand() % 3);	//Should use a defined constant
                e->def -= (rand() % 3);	//Should use a defined constant
                e->vel -= (rand() % 3);	//Should use a defined constant
                e->enr -= (rand() % 2);	//Should use a defined constant
                if (e->atk < 0) {
                    e->atk = 0;
                };
                if (e->def < 0) {
                    e->def = 0;
                };
                if (e->vel < 0) {
                    e->vel = 0;
                };
                if (e->enr < 0) {
                    e->enr = 0;
                };
            }
            //Possible perks for the Equip

            for (int j = 0; j < (EQUIPPERKSMAX); j++) {
                int chance = 20;

                if (q == Good) {
                    chance *= 1.5;
                }

                if ((rand() % 100) < chance
                    || (c->class == CHEST_BEAST && e->perksCount == 0)) {

                    e->perksCount += 1;

                    log_tag("debug_log.txt", "[DEBUG]",
                            "Prepping Perk (%i/%i) for Equip (%i/%i) for Chest",
                            j, e->perksCount, i, c->equipsCount);
                    kls_log(kls, "DEBUG",
                            "Prepping Perk (%i/%i) for Equip (%i/%i) for Chest",
                            j, e->perksCount, i, c->equipsCount);
                    Perk *p =
                        (Perk *) KLS_PUSH_T_TYPED(t_kls, Perk, HR_Perk,
                                                  "Perk", "Perk");
                    p->class = rand() % (PERKSMAX + 1);
                    //p->name = (char*)malloc(sizeof(nameStringFromPerk(p->class)));
                    strcpy(p->name, nameStringFromPerk(p->class));
                    //p->desc = (char*)malloc(sizeof(descStringFromPerk(p->class)));
                    strcpy(p->desc, descStringFromPerk(p->class));
                    p->innerValue = 1;
                    e->perks[(e->perksCount - 1)] = p;
                }
            }

            //Set value of how many bonuses we have. Although Equip only has ONE function pointer field for now
            e->bonus = base->bonus;
            //Randomise if the item will have an effect function.
            //Not yet implemented
            //Initialisation of function happens here
            //
            //e->equip_fun =    ;

            //Calc cost value

            float cost = 5;

            cost += 2.5 * (e->qual);
            cost += 3.5 * (e->perksCount);

            cost += 7.2 * (e->level);

            e->cost = floor(cost);

            c->equips[i] = e;
        }			//End for all equips
    }				//End if equipsCount > 0
}

/**
 * Takes one Shop pointer and initialises all the fields, depeding on stats from the Fighter pointer passed.
 * @see Shop
 * @param s The Shop whose fields will be initialised.
 * @param indexWeight The integer for room index weight.
 * @param player The Fighter player to influence item generation.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initShop(Shop *s, int indexWeight, Fighter *player, Koliseo_Temp *t_kls)
{

    int equipsCount = (rand() % EQUIP_SHOP_MAX) + 1;
    Koliseo *kls = t_kls->kls;

    if (equipsCount != 0) {

        for (int equip_index = 0; equip_index < equipsCount; equip_index++) {
            int curr = (rand() % (EQUIPSMAX + 1));
            log_tag("debug_log.txt", "[DEBUG]",
                    "Prepping Equip (%i/%i) for Shop", equip_index,
                    equipsCount);
            kls_log(kls, "DEBUG", "Prepping Equip (%i/%i) for Shop",
                    equip_index, equipsCount);
            Equip *e =
                (Equip *) KLS_PUSH_T_TYPED(t_kls, Equip, HR_Equip, "Equip",
                                           "Equip");

            //Randomise quality
            quality q = rand() % (QUALITIESMAX + 1);

            //Get the base item and copy the stats to the current equip
            Equip *base = &equips[curr];

            e->class = base->class;
            e->type = base->type;
            e->qual = q;

            setEquipSprite(e);
            strcpy(e->name, base->name);
            strcpy(e->desc, base->desc);

            e->qty = 1;
            e->equipped = 0;
            e->perksCount = 0;

            //Calc drop level
            e->level = base->level + round(player->level / EQUIPLVLBOOSTRATIO);

            //Chance for better leveled item
            if ((rand() % 8) - (player->luck / 10) <= 0) {	//Should use a defined constant
                e->level += 1;	//At least a simple +1
                if ((rand() % 25) - (player->luck / 10) <= 0) {	//Should use a defined constant
                    e->level += 1;	//A bonus roll for another +1

                }
            }

            float boostFactor = 0.7;

            float lvlBoost = boostFactor * (e->level - 1);

            e->atk = round((base->atk * 1.0) + lvlBoost);
            e->def = round((base->def * 1.0) + lvlBoost);
            e->vel = round((base->vel * 1.0) + lvlBoost);
            e->enr = round((base->enr * 1.0) + lvlBoost);

            //Bonus stats on better quality items? Simple for now
            //
            if (q == Good) {
                e->atk += (rand() % 3);	//Should use a defined constant
                e->def += (rand() % 3);	//Should use a defined constant
                e->vel += (rand() % 3);	//Should use a defined constant
                e->enr += (rand() % 2);	//Should use a defined constant
            } else if (q == Bad) {
                e->atk -= (rand() % 3);	//Should use a defined constant
                e->def -= (rand() % 3);	//Should use a defined constant
                e->vel -= (rand() % 3);	//Should use a defined constant
                e->enr -= (rand() % 2);	//Should use a defined constant
                if (e->atk < 0) {
                    e->atk = 0;
                };
                if (e->def < 0) {
                    e->def = 0;
                };
                if (e->vel < 0) {
                    e->vel = 0;
                };
                if (e->enr < 0) {
                    e->enr = 0;
                };
            }
            //Possible perks for the Equip

            for (int i = 0; i < (EQUIPPERKSMAX); i++) {
                int chance = 35;

                if (q == Good) {
                    chance *= 1.5;
                }

                if ((rand() % 100) < chance) {

                    e->perksCount += 1;
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Prepping Perk (%i) for Shop Equip (%i/%i)", i,
                            equip_index, equipsCount);
                    kls_log(kls, "DEBUG",
                            "Prepping Perk (%i) for Shop Equip (%i/%i)", i,
                            equip_index, equipsCount);
                    Perk *p =
                        (Perk *) KLS_PUSH_T_TYPED(t_kls, Perk, HR_Perk,
                                                  "Perk", "Perk");
                    p->class = rand() % (PERKSMAX + 1);
                    //p->name = (char*)malloc(sizeof(nameStringFromPerk(p->class)));
                    strcpy(p->name, nameStringFromPerk(p->class));
                    //p->desc = (char*)malloc(sizeof(descStringFromPerk(p->class)));
                    strcpy(p->desc, descStringFromPerk(p->class));
                    p->innerValue = 1;
                    e->perks[(e->perksCount - 1)] = p;
                }
            }

            //Set value of how many bonuses we have. Although Equip only has ONE function pointer field for now
            e->bonus = base->bonus;
            //Randomise if the item will have an effect function.
            //Not yet implemented
            //Initialisation of function happens here
            //
            //e->equip_fun =    ;

            //Calc cost value

            float cost = 3;

            cost += 4.5 * (e->qual + 1);
            cost += 7.5 * (e->perksCount);

            cost += 10.2 * (e->level);

            e->cost = floor(cost);

            s->equips[equip_index] = e;
            s->equipsCount++;

        }			//End for all equips
    }
    s->equipsCount = equipsCount;
    setEquipPrices(s->equipsCount, s->equipPrices, s->equips);

    int uniqueConsumablesCount = (rand() % SINGLE_CONSUMABLE_SHOP_MAX) + 1;
    int uniques = 0;
    //int not_uniques = 0;
    if (uniqueConsumablesCount != 0) {
        int already_rolled[CONSUMABLESMAX + 1];
        for (int i = 0; i < (CONSUMABLESMAX + 1); i++) {
            already_rolled[i] = 0;
        }
        int cons_prepared = 0;
        while (cons_prepared < uniqueConsumablesCount) {

            for (int i = 0; i < CONSUMABLESMAX + 1; i++) {
                if (cons_prepared < uniqueConsumablesCount) {
                    int curr = rand() % (CONSUMABLESMAX + 1);
                    if (!(already_rolled[curr])) {
                        log_tag("debug_log.txt", "[DEBUG]",
                                "Prepping Consumable (%i/%i) for Shop",
                                cons_prepared, uniqueConsumablesCount);
                        kls_log(kls, "DEBUG",
                                "Prepping Consumable (%i/%i) for Shop",
                                cons_prepared, uniqueConsumablesCount);
                        Consumable *cur =
                            (Consumable *) KLS_PUSH_T_TYPED(t_kls, Consumable,
                                                            HR_Consumable,
                                                            "Consumable",
                                                            "Consumable");
                        cur->class = curr;
                        already_rolled[curr] = 1;
                        if (uniqueConsumablesCount - cons_prepared > 0) {
                            cur->qty = (rand() % 4) + 1;
                        } else {
                            cur->qty = 1;
                            //cur->qty = (rand() % (consumablesCount - cons_index)) +1;
                        }
                        setConsumableSprite(cur);
                        //not_uniques += cur->qty;
                        cons_prepared++;	//= cur->qty;
                        s->consumables[uniques] = cur;
                        uniques++;
                    }
                }
            };
        }
    }
    s->consumablesCount = uniqueConsumablesCount;
    setConsumablePrices(s->consumablesCount, s->consumablePrices,
                        s->consumables);

    s->itemCount = s->consumablesCount + s->equipsCount;
}

/**
 * Takes a Treasure and Fighter pointers and initialises all the treasure fields based on its class and fighter's stats.
 * @see Treasure
 * @param t The allocated Treasure pointer with already set class to initialise.
 * @param f The Fighter pointer to influence item generation.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initTreasure(Treasure *t, Fighter *f, Koliseo_Temp *t_kls)
{

    Koliseo *kls = t_kls->kls;
    strcpy(t->desc, descStringFromTreasure(t->class));

    switch (t->class) {
    case TREASURE_CHEST: {

        log_tag("debug_log.txt", "[DEBUG]",
                "Allocated %lu for Treasure [Chest]:", sizeof(Chest));
        kls_log(kls, "DEBUG", "Allocated %lu for Treasure [Chest]:",
                sizeof(Chest));
        Chest *c =
            (Chest *) KLS_PUSH_T_TYPED(t_kls, Chest, HR_Chest, "Chest",
                                       "Chest");
        prepareChest(c, f, t_kls);
        t->chest = c;

    }
    break;
    case TREASURE_CONSUMABLE: {
        log_tag("debug_log.txt", "[DEBUG]",
                "Allocated %lu for Treasure [Consumable]:",
                sizeof(Consumable));
        kls_log(kls, "DEBUG", "Allocated %lu for Treasure [Consumable]:",
                sizeof(Consumable));
        Consumable *cns =
            (Consumable *) KLS_PUSH_T_TYPED(t_kls, Consumable,
                                            HR_Consumable, "Consumable",
                                            "Consumable");
        int drop = rand() % (CONSUMABLESMAX + 1);

        cns->class = drop;

        Consumable *base = (Consumable *) f->consumablesBag[drop];

        strcpy(cns->name, base->name);
        strcpy(cns->desc, base->desc);
        for (int j = 0; j < 8; j++) {
            strcpy(cns->sprite[j], base->sprite[j]);
        }
        cns->qty = 1;

        t->consumable = cns;
    }
    break;
    case TREASURE_ARTIFACT: {
        log_tag("debug_log.txt", "[DEBUG]",
                "Allocated %lu for Treasure [Artifact]:", sizeof(Artifact));
        kls_log(kls, "DEBUG", "Allocated %lu for Treasure [Artifact]:",
                sizeof(Artifact));
        Artifact *a =
            (Artifact *) KLS_PUSH_T_TYPED(t_kls, Artifact, HR_Artifact,
                                          "Artifact", "Artifact");
        int drop = -1;
        do {
            drop = rand() % (ARTIFACTSMAX + 1);
        } while (f->artifactsBag[drop]->qty > 0);

        a->class = drop;

        Artifact *base = f->artifactsBag[drop];

        strcpy(a->name, base->name);
        strcpy(a->desc, base->desc);
        for (int j = 0; j < 8; j++) {
            strcpy(a->sprite[j], base->sprite[j]);
        }
        a->qty = 0;

        t->artifact = a;

    }
    break;
    }
}

/**
 * Takes a Chest and Fighter pointers and initialises all the fields in chest making it ready for use in battle, using fighter stats to influence init.
 * Calls initChest() after setting class.
 * @see Chest
 * @see initChest()
 * @param c The allocated Chest pointer to initialise.
 * @param f The Fighter pointer to influence item generation.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void prepareChest(Chest *c, Fighter *f, Koliseo_Temp *t_kls)
{

    //Init chest class
    int drop = (rand() % 100) + 1;

    if (drop > 70) {
        c->class = CHEST_BASE;
    } else {
        c->class = CHEST_BEAST;
    }

    //Load Chest stats
    initChest(c, f, t_kls);

}

/**
 * Takes a Boss pointer and initialises all the fields making it ready for use in battle.
 * Calls initBossStats() after setting class and level, then forces a stat reset to update the stats
 * with the level boost.
 * @see Boss
 * @see initBossStats()
 * @param b The allocated Boss pointer to initialise.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void prepareBoss(Boss *b, Koliseo_Temp *t_kls)
{

    //Randomise boss class
    b->class = rand() % (BOSSCLASSESMAX + 1);

    b->beast = 0;
    BossBaseStats *base = &basebossstats[b->class];

    b->level = base->level;

    //Enemies get 1 level every 2 rooms
    //e->level += floor(roomindex / 2) ;

    //Load boss stats
    initBossStats(b, t_kls);

    //Set skill slots
    setBossSkills(b, t_kls);

    //Force load of level bonuses
    statResetBoss(b, 1);

}

/**
 * Takes a Enemy and three integers denoting current room index, how many enemies are in the room and current enemy index.
 * The class field of the enemy is randomised according to ENEMYCLASSESMAX.
 * If the room index is multiple of BEASTROOM and the enemy is the last one in he room, its beast field is set to 1.
 * The EnemyBaseStats pointer for the enemy's enemyClass is loaded and the level field for the enemy is set to base level, before increasing.
 * initEnemyStats() is called to set all stat fields and statResetEnemy() is called with force=true to apply the boosted stats to leveled enemies.
 * @see Enemy
 * @see enemyClass
 * @see ENEMYCLASSESMAX
 * @see BEASTROOM
 * @see BSTFACTOR
 * @see ENEMYLVLRATIO
 * @see EnemyBaseStats
 * @see initEnemyStats()
 * @see statResetEnemy()
 * @param e The Enemy pointer to prepare.
 * @param roomindex The index of current room.
 * @param enemiesInRoom The number of enemies in current room.
 * @param enemyindex The index of current enemy.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void prepareRoomEnemy(Enemy *e, int roomindex, int enemiesInRoom,
                      int enemyindex, Koliseo_Temp *t_kls)
{

    if (e == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s():    Passed Enemy was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    //Randomise enemy class
    e->class = rand() % (ENEMYCLASSESMAX + 1);

    if (G_DEBUG_ON && G_DEBUG_ENEMYTYPE_ON && (GAMEMODE != Rogue)) {	//Debug flag has a fixed enemy class when used outside of Rogue gamemode
        log_tag("debug_log.txt", "[DEBUG]",
                "prepareRoomEnemy(): Enemy debug flag was asserted outside of story mode, will always spawn a G_DEBUG_ENEMYTYPE (%s).",
                stringFromEClass(G_DEBUG_ENEMYTYPE));
        e->class = G_DEBUG_ENEMYTYPE;
    }

    //Check for spawning beast enemies
    if ((roomindex % BEASTROOM == 0) && (enemyindex == (enemiesInRoom - 1))) {
        //TODO: better mechanic for spawning beast enemies
        if (((rand() % 5) == 0)) {
            log_tag("debug_log.txt", "[DEBUG]", "Setting e->beast to 1.");
            e->beast = 1;
        }
    } else {
        e->beast = 0;
    };

    EnemyBaseStats *base = &baseenemystats[e->class];

    e->level = base->level;

    //Enemies get 1 level every 2 rooms
    e->level += floor(roomindex / 2);

    //Set current enemy index
    e->index = enemyindex;

    //Load enemy stats
    initEnemyStats(e, t_kls);

    //Load enemy skills
    //
    setEnemySkills(e, t_kls);

    //Force load of level bonuses
    statResetEnemy(e, 1);
}

/**
 * Takes a Treasure and Fighter pointer and initialises all the treasure fields making it ready for use in battle, based on the fighter stats.
 * Calls initTreasure() after setting class.
 * @see Treasure
 * @see initTreasure()
 * @param t The allocated Treasure pointer to initialise.
 * @param f The Fighter pointer to influence item generation.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void prepareTreasure(Treasure *t, Fighter *f, Koliseo_Temp *t_kls)
{

    //Init treasure class

    int roll = (rand() % 100) + 1;

    if (roll > 70) {
        t->class = TREASURE_CHEST;
    } else if (roll > 50) {
        t->class = TREASURE_ARTIFACT;
    } else {
        t->class = TREASURE_CONSUMABLE;
    }

    //Load Treasure stats
    initTreasure(t, f, t_kls);

}

/**
 * Takes a Roadfork pointer and initialises all the fields making it ready for use in battle.
 * Calls initRoadfork() after setting class.
 * @see Roadfork
 * @see initRoadfork()
 * @param r The allocated Roadfork pointer to initialise.
 */
void prepareRoadfork(Roadfork *r)
{

    //Randomise options
    r->options[0] = rand() % (ROADFORK_OPTIONS_MAX + 1);
    int previous = r->options[0];
    int new = -1;

    do {
        new = rand() % (ROADFORK_OPTIONS_MAX + 1);

    } while (new == previous);

    previous = new;
    r->options[1] = new;

}

/**
 * Takes a Enemy pointer and prepares its skillSlot fields by allocating ENEMY_SKILL_SLOTS slots.
 * Skill slots are initialised.
 * @see Enemy
 * @see Skilllot
 * @see ENEMY_SKILL_SLOTS
 * @see SKILLSTOTAL
 * @see costFromSkill()
 * @see stringFromSkill()
 * @param t_kls The Koliseo_Temp used for allocations.
 * @param e The Enemy pointer whose skill slots will be initialised.
 */
void setEnemySkills(Enemy *e, Koliseo_Temp *t_kls)
{
    char movename[80];
    char movedesc[80];
    for (int i = 0; i < ENEMY_SKILL_SLOTS; i++) {
        kls_log(t_kls->kls, "DEBUG", "Prepping Enemy Skillslot (%i)", i);
        Skillslot *s =
            (Skillslot *) KLS_PUSH_T_TYPED(t_kls, Skillslot, HR_Skillslot,
                                           "Enemy Skillslot", "Enemy Skillslot");
        s->enabled = 0;
        s->class = i;
        s->cost = costFromSkill(i);
        strcpy(movename, nameStringFromSkill(i));
        strcpy(movedesc, descStringFromSkill(i));
        //printf("DEBUG\n%i\t%s\n",(i),stringFromSkill(i));
        strcpy(s->name, movename);
        strcpy(s->desc, movedesc);
        e->skills[i] = s;
    };
}

/**
 * Takes a Boss pointer and prepares its skillSlot fields by allocating BOSS_SKILL_SLOTS slots.
 * Skill slots are initialised.
 * @see Boss
 * @see Skilllot
 * @see Boss_SKILL_SLOTS
 * @see SKILLSTOTAL
 * @see costFromSkill()
 * @see stringFromSkill()
 * @param t_kls The Koliseo_Temp used for allocations.
 * @param b The Boss pointer whose skill slots will be initialised.
 */
void setBossSkills(Boss *b, Koliseo_Temp *t_kls)
{
    char movename[80];
    char movedesc[80];
    for (int i = 0; i < BOSS_SKILL_SLOTS; i++) {
        kls_log(t_kls->kls, "DEBUG", "Prepping Boss Skillslot (%i)", i);
        Skillslot *s =
            (Skillslot *) KLS_PUSH_T_TYPED(t_kls, Skillslot, HR_Skillslot,
                                           "Boss Skillslot", "Boss Skillslot");
        s->enabled = 0;
        s->class = i;
        s->cost = costFromSkill(i);
        strcpy(movename, nameStringFromSkill(i));
        strcpy(movedesc, descStringFromSkill(i));
        //printf("DEBUG\n%i\t%s\n",(i),stringFromSkill(i));
        strcpy(s->name, movename);
        strcpy(s->desc, movedesc);
        b->skills[i] = s;
    };
}
