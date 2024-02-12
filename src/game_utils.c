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
#include "game_utils.h"
//Functions useful in many areas
//

/**
 * Prints global vars to stdout.
 */
void printGlobVars(void)
{
    printf("\nGlobal vars:\n");
    printf("  G_PRELOAD_ANIMATIONS_ON: { %i }\n", G_PRELOAD_ANIMATIONS_ON);
    printf("  G_DEBUG_ON: { %i }\n", G_DEBUG_ON);
    printf("  G_LOG_ON: { %i }\n", G_LOG_ON);
    printf("  G_EXPERIMENTAL_ON: { %i }\n", G_EXPERIMENTAL_ON);
    printf("  G_FASTQUIT_ON: { %i }\n", G_FASTQUIT_ON);
    printf("  G_GODMODE_ON: { %i }\n", G_GODMODE_ON);
    printf("  G_DEBUG_ROOMTYPE_ON: { %i }\n", G_DEBUG_ROOMTYPE_ON);
    printf("  G_ROOMTYPE_ON: { %s } [ %i ]\n", stringFromRoom(G_DEBUG_ROOMTYPE),
           G_DEBUG_ROOMTYPE);
    printf("  G_ENEMYTYPE_ON: { %i }\n", G_DEBUG_ENEMYTYPE_ON);
    printf("  G_DEBUG_ENEMYTYPE { %s } [ %i ]\n",
           stringFromEClass(G_DEBUG_ENEMYTYPE), G_DEBUG_ENEMYTYPE);
    printf("  G_DOTUTORIAL_ON: { %i }\n", G_DOTUTORIAL_ON);
}

#ifdef _WIN32
/**
 * Prints Windows envvars to stdout.
 */
void printWin_EnvVars(void)
{
    printf("\nWindows Environment vars:\n");
    printf("  UserProfile: { %s }\n", getenv("UserProfile"));
    printf("  HomeDrive: { %s }\n", getenv("HomeDrive"));
    printf("  HomePath: { %s }\n", getenv("HomePath"));
    printf("  ComputerName: { %s }\n", getenv("ComputerName"));
    printf("  Processor_Revision: { %s }\n", getenv("Processor_Revision"));
    printf("  Processor_Identifier: { %s }\n", getenv("Processor_Identifier"));
    printf("  Processor_Level: { %s }\n", getenv("Processor_Level"));
    printf("  Number_Of_Processors: { %s }\n", getenv("Number_Of_Processors"));
    printf("  OS: { %s }\n", getenv("OS"));
}

/**
 * Logs Windows envvars to debug log file.
 */
void log_Win_EnvVars(void)
{
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "Windows Environment vars:");
    log_tag("debug_log.txt", "[WIND32-DEBUG", "UserProfile: { %s }",
            getenv("UserProfile"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "HomeDrive: { %s }",
            getenv("HomeDrive"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "HomePath: { %s }",
            getenv("HomePath"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "ComputerName: { %s }",
            getenv("ComputerName"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "Processor_Revision: { %s }",
            getenv("Processor_Revision"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "Processor_Identifier: { %s }",
            getenv("Processor_Identifier"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "Processor_Level: { %s }",
            getenv("Processor_Level"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "Number_Of_Processors: { %s }",
            getenv("Number_Of_Processors"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "OS: { %s }", getenv("OS"));
}
#endif

#ifdef HELAPORDO_CURSES_BUILD
bool set_Saveslot_name(FILE *file, Saveslot *sv)
{

    //printf("\nLoading game...\n");
    log_tag("debug_log.txt", "[LOAD]", "Starting loading from text file.");

//      FILE* file = fopen("save.txt", "r");
    if (file == NULL) {
        endwin();
        printf("Error with file while trying to load!\n");
        return false;
    }
    char buf[500];
    char comment[300];
    int num_value = -1;
    const char version[] = "v0.1.6";

    int scanres = -1;
    /* File version scanning */
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in %s(), expected [%i] was (%i)", __func__,
                1, scanres);
        endwin();
        fprintf(stderr, "Error while loading game.");
        return false;
    }

    int check = -1;
    if (!((check = strcmp(buf, version)) == 0)) {
        log_tag("debug_log.txt", "[LOAD-ERROR]",
                "Failed save format version check. Was [%s]. Quitting.", buf);
        endwin();
        fprintf(stderr, "[ERROR]    File version mismatch on load.\n");
        return false;
    };
    log_tag("debug_log.txt", "[LOAD]", "Loaded save format version: (%s).",
            buf);

    /* Save type scanning */
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in %s(), expected [%i] was (%i)", __func__,
                1, scanres);
        endwin();
        fprintf(stderr, "Error while loading game.");
        return false;
    }

    check = -1;
    if (!
        (((check = strcmp(buf, stringFrom_saveType(ENEMIES_SAVE)) == 0))
         || ((check = strcmp(buf, stringFrom_saveType(HOME_SAVE))) == 0))) {
        log_tag("debug_log.txt", "[LOAD-ERROR]",
                "%s():  Failed save type check, was [%s]. Quitting.", buf);
        endwin();
        fprintf(stderr, "[ERROR]    Save type version mismatch on load.\n");
        return false;
    };
    log_tag("debug_log.txt", "[LOAD]", "Loaded save type: (%s).", buf);

    /* Gamemode scanning */
    scanres = fscanf(file, "%200[^#]# %s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in %s(), expected [%i] was (%i)", __func__,
                2, scanres);
        endwin();
        fprintf(stderr, "Error while loading game.");
        return false;
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    int gmd = num_value;
    log_tag("debug_log.txt", "[LOAD]", "Gamemode was: {%i}", gmd);

    /* Fighter scanning */
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in %s(), expected [%i] was (%i)", __func__,
                1, scanres);
        endwin();
        fprintf(stderr, "Error while loading game.");
        return false;
    }
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in %s(), expected [%i] was (%i)", __func__,
                2, scanres);
        endwin();
        fprintf(stderr, "Error while loading game.");
        return false;
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    strncpy(sv->name, buf, 50);
    sv->name[49] = '\0';
    return true;
}
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined."
#else
bool set_Saveslot_name(FILE *file, Saveslot *sv)
{
    (void) file;
    (void) sv;
    printf("%s():    TODO - implement set_Saveslot_namefor rl-build.\n", __func__);
    return false;
}
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

/**
 * Debugs the passed (preallocated) Fighter with log_tag().
 * @param fighter The allocated Fighter to debug.
 */
void dbg_Fighter(Fighter *fighter)
{
    if (fighter == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "Fighter was NULL in dbg_Fighter()");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter name: { %s }",
            fighter->name);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter class: { %s } [ %i ]",
            stringFromClass(fighter->class), fighter->class);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter hp: { %i }", fighter->hp);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter atk: { %i }", fighter->atk);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter def: { %i }", fighter->def);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter vel: { %i }", fighter->vel);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter level: { %i }",
            fighter->level);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter luck: { %i }",
            fighter->luck);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter totalxp: { %i }",
            fighter->totalxp);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter currentlevelxp: { %i }",
            fighter->currentlevelxp);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter totallevelxp: { %i }",
            fighter->totallevelxp);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter totalhp: { %i }",
            fighter->totalhp);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter status: { %s } [ %i ]",
            stringFromStatus(fighter->status), fighter->status);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter energy: { %i }",
            fighter->energy);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter totalenergy: { %i }",
            fighter->totalenergy);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter stamina: { %i }",
            fighter->stamina);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter totalstamina: { %i }",
            fighter->totalstamina);

    //Specialslot* specials[SPECIALSMAX+1]; /**< Array with all the Specialslot*/

    //struct Turncounter* counters[COUNTERSMAX+1]; /**< Array with all the Turncounter pointers*/
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter turnboost_atk: { %i }",
            fighter->turnboost_atk);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter turnboost_def: { %i }",
            fighter->turnboost_def);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter turnboost_vel: { %i }",
            fighter->turnboost_vel);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter turnboost_enr: { %i }",
            fighter->turnboost_enr);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter perksCount: { %i }",
            fighter->perksCount);

    //Perk* perks[PERKSMAX+1]; /**< Array with all the Perk*/

    //struct Equipslot* equipslots[EQUIPZONES+1]; /**< Array with all the Equipslot*/
    //struct Equip* equipsBag[EQUIPSBAGSIZE+1]; /**< Array with all the Equip found*/
    //struct Consumable* consumablesBag[CONSUMABLESMAX+1]; /**< Array with all the Consumables found*/
    //struct Artifact* artifactsBag[ARTIFACTSMAX+1]; /**< Array with all the Artifacts found*/

    log_tag("debug_log.txt", "[FIGHTER]",
            "Fighter used equipsBag slots: { %i }",
            fighter->equipsBagOccupiedSlots);
    log_tag("debug_log.txt", "[FIGHTER]",
            "Fighter earliest equipsBag slot: { %i }",
            fighter->earliestBagSlot);

    log_tag("debug_log.txt", "[FIGHTER]", "Fighter permboost_atk: { %i }",
            fighter->permboost_atk);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter permboost_def: { %i }",
            fighter->permboost_def);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter permboost_vel: { %i }",
            fighter->permboost_vel);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter permboost_enr: { %i }",
            fighter->permboost_enr);

    log_tag("debug_log.txt", "[FIGHTER]", "Fighter equipboost_atk: { %i }",
            fighter->equipboost_atk);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter equipboost_def: { %i }",
            fighter->equipboost_def);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter equipboost_vel: { %i }",
            fighter->equipboost_vel);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter equipboost_enr: { %i }",
            fighter->equipboost_enr);

    //dbg_countStats(fighter->stats);

    log_tag("debug_log.txt", "[FIGHTER]", "Fighter coins balance: { %i }",
            fighter->balance);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter keys balance: { %i }",
            fighter->keys_balance);
}

/**
 * Debugs the passed (preallocated) Saveslot with log_tag().
 * @param saveslot The allocated Saveslot to debug.
 */
void dbg_Saveslot(Saveslot *saveslot)
{
    if (saveslot == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "Saveslot was NULL in dbg_Saveslot()");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[SAVESLOT]", "Saveslot name: { %s }",
            saveslot->name);
    log_tag("debug_log.txt", "[SAVESLOT]", "Saveslot save_path: { %s }",
            saveslot->save_path);
}

/**
 * Debugs the passed (preallocated) Path with log_tag().
 * @param path The allocated Path to debug.
 */
void dbg_Path(Path *path)
{
    if (path == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "Path was NULL in dbg_Path()");
        exit(EXIT_FAILURE);
    }

    log_tag("debug_log.txt", "[PATH]", "Path length: { %i }", path->length);
    log_tag("debug_log.txt", "[PATH]", "Path luck: { %i }", path->luck);
    log_tag("debug_log.txt", "[PATH]", "Path prize: { %i }", path->prize);
    log_tag("debug_log.txt", "[PATH]", "Path loreCounter: { %i }",
            path->loreCounter);
    log_tag("debug_log.txt", "[PATH]", "Path loreCounter: { %i }",
            path->loreCounter);
    dbg_Wincon(path->win_condition);
    dbg_Saveslot(path->current_saveslot);
}

/**
 * Debugs the passed (preallocated) Wincon with log_tag().
 * @param gmst The allocated countStats to debug.
 */
void dbg_Wincon(Wincon *wc)
{
    if (wc == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "Wincon was NULL in dbg_Wincon()");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[WINCON]", "Wincon class: { %s } [ %i ]",
            stringFromWinconClass(wc->class), wc->class);
    log_tag("debug_log.txt", "[WINCON]", "Wincon current value: { %i }",
            wc->current_val);
    log_tag("debug_log.txt", "[WINCON]", "Wincon target value: { %i }",
            wc->target_val);
}

/**
 * Debugs the passed (preallocated) countStats with log_tag().
 * @param gmst The allocated countStats to debug.
 */
void dbg_countStats(countStats *stats)
{
    if (stats == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "countStats was NULL in dbg_countStats()");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[countStats]", "Enemies killed:  { %i }",
            stats->enemieskilled);
    log_tag("debug_log.txt", "[countStats]", "Consumables found:  { %i }",
            stats->consumablesfound);
    log_tag("debug_log.txt", "[countStats]", "Equips found:  { %i }",
            stats->equipsfound);
    log_tag("debug_log.txt", "[countStats]", "Artifacts found:  { %i }",
            stats->artifactsfound);
    log_tag("debug_log.txt", "[countStats]", "Critical hits done:  { %i }",
            stats->criticalhits);
    log_tag("debug_log.txt", "[countStats]", "Rooms completed:  { %i }",
            stats->roomscompleted);
    log_tag("debug_log.txt", "[countStats]", "Floors completed:  { %i }",
            stats->floorscompleted);
    log_tag("debug_log.txt", "[countStats]", "Specials unlocked:  { %i }",
            stats->specialsunlocked);
    log_tag("debug_log.txt", "[countStats]", "Coins found:  { %i }",
            stats->coinsfound);
    log_tag("debug_log.txt", "[countStats]", "Bosses killed:  { %i }",
            stats->bosseskilled);
    log_tag("debug_log.txt", "[countStats]", "Unique Boss kills:  { %i }",
            stats->unique_bosseskilled);
    for (int i = 0; i < BOSSCLASSESMAX + 1; i++) {
        log_tag("debug_log.txt", "[countStats]", "Boss [%i] { %s }:  { %s }", i,
                stringFromBossClass(i),
                (stats->killed_bosses[i] == 1 ? "Killed" : "Not Killed"));
    }
    log_tag("debug_log.txt", "[countStats]", "Keys found:  { %i }",
            stats->keysfound);
}

/**
 * Logs floor layout for passed Floor.
 */
void dbg_print_floor_layout(Floor *floor)
{
    if (floor == NULL) {
        fprintf(stderr, "[ERROR]   at %s(): passed floor was NULL.\n",
                __func__);
        log_tag("debug_log.txt", "[ERROR]", "at %s(): passed floor was NULL.",
                __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
        char rowbuf[FLOOR_MAX_COLS + 1] = { 0 };
        for (int x = 0; x < FLOOR_MAX_COLS; x++) {
            rowbuf[x] = (floor->floor_layout[x][y] == 1 ? '1' : ' ');
        }
        log_tag("debug_log.txt", "[Floor_row]", "{%s} - %i", rowbuf, y);
    }
    log_tag("debug_log.txt", "[DEBUG]", "Logged floor layout.");
}

/**
 * Logs explored layout for passed Floor.
 */
void dbg_print_explored_layout(Floor *floor)
{
    if (floor == NULL) {
        fprintf(stderr, "[ERROR]   at %s(): passed floor was NULL.\n",
                __func__);
        log_tag("debug_log.txt", "[ERROR]", "at %s(): passed floor was NULL.",
                __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
        char rowbuf[FLOOR_MAX_COLS + 1] = { 0 };
        for (int x = 0; x < FLOOR_MAX_COLS; x++) {
            rowbuf[x] = (floor->explored_matrix[x][y] == 1 ? '1' : ' ');
        }
        log_tag("debug_log.txt", "[Floor_row]", "{%s} - %i", rowbuf, y);
    }
    log_tag("debug_log.txt", "[DEBUG]", "Logged explored layout.");
}

/**
 * Logs roomclass layout for passed Floor.
 */
void dbg_print_roomclass_layout(Floor *floor)
{
    if (floor == NULL) {
        fprintf(stderr, "[ERROR]   at %s(): passed floor was NULL.\n",
                __func__);
        log_tag("debug_log.txt", "[ERROR]", "at %s(): passed floor was NULL.",
                __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
        char rowbuf[FLOOR_MAX_COLS + 1] = { 0 };
        for (int x = 0; x < FLOOR_MAX_COLS; x++) {
            char ch = '.';
            switch (floor->roomclass_layout[x][y]) {
            case HOME: {
                ch = 'H';
            }
            break;
            case ENEMIES: {
                ch = 'E';
            }
            break;
            case BOSS: {
                ch = 'B';
            }
            break;
            case SHOP: {
                ch = '$';
            }
            break;
            case TREASURE: {
                ch = '*';
            }
            break;
            case WALL: {
                ch = '#';
            }
            break;
            case BASIC: {
                ch = ' ';
            }
            break;
            default: {
                ch = '?';
            }
            break;
            }
            rowbuf[x] = ch;
        }
        log_tag("debug_log.txt", "[Floor_row]", "{%s} - %i", rowbuf, y);
    }
    log_tag("debug_log.txt", "[DEBUG]", "Logged roomclass layout.");
}

/**
 * Debugs the passed (preallocated) Gamestate with log_tag().
 * @param gmst The allocated Gamestate to debug.
 */
void dbg_Gamestate(Gamestate *gmst)
{
    if (gmst == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "Gamestate was NULL in dbg_Gamestate()");
        exit(EXIT_FAILURE);
    }
#ifdef HELAPORDO_CURSES_BUILD
    if (gmst->screen == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "Screen was NULL in dbg_Gamestate()");
    } else {
        dbg_GameScreen(gmst->screen);
    }
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
    log_tag("debug_log.txt", "[DEBUG]", "Gamestate:{");
    log_tag("debug_log.txt", "[GAMESTATE]", "Current Gamemode: { %s } [ %i ]",
            stringFromGamemode(gmst->gamemode), gmst->gamemode);
    log_tag("debug_log.txt", "[GAMESTATE]", "Start time: {%llu}",
            (unsigned long long) gmst->start_time);
    clock_t run_time = clock() - gmst->start_time;
    int time_spent = run_time * 1000 / CLOCKS_PER_SEC;
    log_tag("debug_log.txt", "[GAMESTATE]", "Current run time: %d s, %d ms.",
            time_spent / 1000, time_spent % 1000);
    log_tag("debug_log.txt", "[GAMESTATE]", "Current fighters: { %i }",
            gmst->current_fighters);
    log_tag("debug_log.txt", "[GAMESTATE]", "Current room index: { %i }",
            gmst->current_room_index);
    log_tag("debug_log.txt", "[GAMESTATE]", "Current room type: { %s } [ %i ]",
            stringFromRoom(gmst->current_roomtype), gmst->current_roomtype);
    log_tag("debug_log.txt", "[GAMESTATE]", "Current enemy index: { %i }",
            gmst->current_enemy_index);
    dbg_countStats(gmst->stats);
    dbg_Path(gmst->path);
    dbg_Fighter(gmst->player);
    //TODO: print out current floor
    if (gmst->current_floor == NULL) {
        log_tag("debug_log.txt", "[GAMESTATE]", "Current floor was NULL.");
    } else {
        log_tag("debug_log.txt", "[GAMESTATE]", "Current floor: {");
        log_tag("debug_log.txt", "[Floor]", "index: {%i}",
                gmst->current_floor->index);
        if (gmst->current_floor->desc != NULL) {
            log_tag("debug_log.txt", "[Floor]", "desc: {%s}",
                    gmst->current_floor->desc);
        }
        log_tag("debug_log.txt", "[Floor]", "floorClass: {%i} {%s}",
                gmst->current_floor->class,
                stringFromFloorclass(gmst->current_floor->class));
        log_tag("debug_log.txt", "[Floor]", "area: {%i}",
                gmst->current_floor->area);
        log_tag("debug_log.txt", "[Floor]", "explorea area: {%i}",
                gmst->current_floor->explored_area);
        dbg_print_floor_layout(gmst->current_floor);
        dbg_print_explored_layout(gmst->current_floor);
        dbg_print_roomclass_layout(gmst->current_floor);
        log_tag("debug_log.txt", "[GAMESTATE]", "  }");
    }
    log_tag("debug_log.txt", "[GAMESTATE]", "}");
}

/**
 * Debugs the passed (preallocated) GameScreen with log_tag().
 * @param scr The allocated GameScreen to debug.
 */
void dbg_GameScreen(GameScreen * scr)
{
#ifdef HELAPORDO_CURSES_BUILD
    int y = 0;
    int x = 0;
    if (scr->win == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "win was NULL in %s()", __func__);
    } else {
        getmaxyx(scr->win, y, x);
        log_tag("debug_log.txt", "[GAMESCREEN]",
                "Screen size: y->(%i), x->(%i)",
                y, x);
    }
    log_tag("debug_log.txt", "[GAMESCREEN]",
            "Cols: {%i}", scr->cols);
    log_tag("debug_log.txt", "[GAMESCREEN]",
            "Rows: {%i}", scr->rows);
    log_tag("debug_log.txt", "[GAMESCREEN]",
            "Colors: {%i}", scr->colors);
    log_tag("debug_log.txt", "[GAMESCREEN]",
            "Color pairs: {%i}", scr->color_pairs);
    log_tag("debug_log.txt", "[GAMESCREEN]",
            "Escape delay: {%i}", scr->escape_delay);
    log_tag("debug_log.txt", "[GAMESCREEN]",
            "Tab size: {%i}", scr->tabsize);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
    printf("TODO: implement %s().\n", __func__);
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
}

/**
 * Updates the passed (preallocated) Gamestate with the passed int values.
 * @param gmst The allocated Gamestate to update.
 * @param current_fighters Number of current Fighters.
 * @param current_roomtype roomClass for current Room.
 * @param current_room_index Index for current Room.
 * @param current_enemy_index Index for current Enemy.
 * @param current_floor Pointer to current Floor, initialised if gmst->gamemode == Rogue.
 */
void update_Gamestate(Gamestate *gmst, int current_fighters,
                      roomClass current_roomtype, int current_room_index,
                      int current_enemy_index, Floor *current_floor)
{
    if (gmst == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "Gamestate was NULL in %s().",
                __func__);
        exit(EXIT_FAILURE);
    }
    gmst->current_fighters = current_fighters;
    gmst->current_roomtype = current_roomtype;
    gmst->current_room_index = current_room_index;
    gmst->current_enemy_index = current_enemy_index;
    if (gmst->gamemode == Rogue) {
        if (current_floor == NULL) {
            log_tag("debug_log.txt", "[WARN]",
                    "Passed current floor was NULL in %s().", __func__);
        } else {
            gmst->current_floor = current_floor;
        }
    }
}

#ifdef HELAPORDO_CURSES_BUILD
/**
 * Inits the passed (preallocated) Gamestate with the passed pointers.
 * @param gmst The allocated Gamestate to init.
 * @param start_time The start time for current game.
 * @param stats Game stats.
 * @param wincon Game Wincon.
 * @param path Game Path.
 * @param player Game main player.
 * @param gamemode Picked gamemode.
 * @param screen The main screen from initscr().
 */
void init_Gamestate(Gamestate *gmst, clock_t start_time, countStats *stats, Wincon *wincon,
                    Path *path, Fighter *player, Gamemode gamemode, GameScreen* screen)
{
    if (gmst == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "Gamestate was NULL in %s()",
                __func__);
        exit(EXIT_FAILURE);
    }
    if (stats == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "countStats was NULL in %s()",
                __func__);
        exit(EXIT_FAILURE);
    }
    if (wincon == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "Wincon was NULL in %s()",
                __func__);
        exit(EXIT_FAILURE);
    }
    if (path == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "Path was NULL in %s()", __func__);
        exit(EXIT_FAILURE);
    }
    if (player == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "Player was NULL in %s()",
                __func__);
        exit(EXIT_FAILURE);
    }
    if (gamemode != Story && gamemode != Rogue) {
        log_tag("debug_log.txt", "[ERROR]", "Invalid gamemode requested: [%i]",
                gamemode);
        exit(EXIT_FAILURE);
    }
    if (screen == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "Screen was NULL in %s()",
                __func__);
        exit(EXIT_FAILURE);
    }
    gmst->start_time = start_time;
    gmst->stats = stats;
    gmst->current_fighters = -1;
    gmst->current_roomtype = -1;
    gmst->current_room_index = -1;
    gmst->current_enemy_index = -1;
    gmst->wincon = wincon;
    gmst->path = path;
    gmst->player = player;
    gmst->gamemode = gamemode;
    gmst->screen = screen;
}

#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
/**
 * Inits the passed (preallocated) Gamestate with the passed pointers.
 * @param gmst The allocated Gamestate to init.
 * @param start_time The start time for current game.
 * @param stats Game stats.
 * @param wincon Game Wincon.
 * @param path Game Path.
 * @param player Game main player.
 * @param gamemode Picked gamemode.
 */
void init_Gamestate(Gamestate *gmst, clock_t start_time, countStats *stats, Wincon *wincon,
                    Path *path, Fighter *player, Gamemode gamemode)
{
    if (gmst == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "Gamestate was NULL in %s()",
                __func__);
        exit(EXIT_FAILURE);
    }
    if (stats == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "countStats was NULL in %s()",
                __func__);
        exit(EXIT_FAILURE);
    }
    if (wincon == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "Wincon was NULL in %s()",
                __func__);
        exit(EXIT_FAILURE);
    }
    if (path == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "Path was NULL in %s()", __func__);
        exit(EXIT_FAILURE);
    }
    if (player == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "Player was NULL in %s()",
                __func__);
        exit(EXIT_FAILURE);
    }
    if (gamemode != Story && gamemode != Rogue) {
        log_tag("debug_log.txt", "[ERROR]", "Invalid gamemode requested: [%i]",
                gamemode);
        exit(EXIT_FAILURE);
    }
    gmst->start_time = start_time;
    gmst->stats = stats;
    gmst->current_fighters = -1;
    gmst->current_roomtype = -1;
    gmst->current_room_index = -1;
    gmst->current_enemy_index = -1;
    gmst->wincon = wincon;
    gmst->path = path;
    gmst->player = player;
    gmst->gamemode = gamemode;
}
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

#ifdef HELAPORDO_CURSES_BUILD
/**
 * Allocates and prepares a turnOP_args and returns a pointer to it.
 * @see turnOP_args
 * @see turnOption_OP
 * @see turnOption
 * @see turnOP()
 * @param gmst The Gamestate pointer to assign to turnOP_args->gmst.
 * @param actor The Fighter pointer to assign to turnOP_args->actor.
 * @param path The Path pointer to assign to turnOP_args->path.
 * @param room The Room pointer to assign to turnOP_args->room.
 * @param Enemy The Enemy pointer to assign to turnOP_args->enemy.
 * @param Boss The Boss pointer to assign to turnOP_args->boss.
 * @param save_file The FILE pointer to assign to turnOP_args->save_file.
 * @param notify_win The WINDOW pointer to assign to turnOP_args->notify_win.
 * @param t_kls The Koliseo_Temp pointer to assign to turnOP_args->t_kls.
 * @param foe_op The foeTurnOption_OP to assign to turnOP_args->foe_op.
 * @param picked_skill The skillType to assign to turnOP_args->picked_skill.
 */
turnOP_args *init_turnOP_args(Gamestate *gmst, Fighter *actor, Path *path,
                              Room *room, loadInfo *load_info, Enemy *enemy,
                              Boss *boss, FILE *save_file, WINDOW *notify_win,
                              Koliseo_Temp *t_kls, foeTurnOption_OP foe_op,
                              skillType picked_skill)
{
    log_tag("debug_log.txt", "[TURNOP]",
            "Allocated size %lu for new turnOP_args", sizeof(turnOP_args));
    kls_log(t_kls->kls, "DEBUG", "[TURNOP]",
            "Allocated size %lu for new turnOP_args", sizeof(turnOP_args));
    turnOP_args *res =
        (turnOP_args *) KLS_PUSH_T_TYPED(t_kls, turnOP_args, HR_turnOP_args,
                                         "turnOP_args", "turnOP_args");

    res->gmst = gmst;
    res->actor = actor;
    res->path = path;
    res->room = room;
    res->load_info = load_info;
    res->enemy = enemy;
    res->boss = boss;
    res->save_file = save_file;
    res->notify_win = notify_win;
    res->t_kls = t_kls;
    res->foe_op = foe_op;
    res->picked_skill = picked_skill;

    return res;
}
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
turnOP_args *init_turnOP_args(Gamestate *gmst, Fighter *actor, Path *path,
                              Room *room, loadInfo *load_info, Enemy *enemy,
                              Boss *boss, FILE *save_file, Rectangle *notification_area,
                              Koliseo_Temp *t_kls, foeTurnOption_OP foe_op,
                              skillType picked_skill)
{
    printf("%s():    TODO - implement turnOP init for rl-build\n", __func__);
    return NULL;
}
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

/**
 * Takes a string and returns the corresponding saveType.
 * Will return -1 if no match is found.
 * @param s The string to convert.
 */
saveType saveTypeFrom_string(char *s)
{
    int check = -1;

    for (int i = 0; i < SAVETYPE_MAX + 1; i++) {
        if ((check = strcmp(s, stringFrom_saveType(i))) == 0) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Matched saveType string [%s] to (%i).", s, i);
            return (saveType) i;
        }
    }

    log_tag("debug_log.txt", "[ERROR]",
            "No match found for saveType string [%s].", s);
    return -1;
}

/**
 * Logs a turnOption_OP to "$STATICDIR/OPS_log.txt".
 * @param op The turnOption_OP to log.
 */
void log_OP(turnOption_OP op)
{
    log_tag(OPS_LOGFILE, "[LOG]", "[OP_code: %i, value: %s]", op,
            stringFromTurnOP(op));
    log_tag("debug_log.txt", "[LOG_TURNOP]", "[OP_code: %i, value: %s]", op,
            stringFromTurnOP(op));
}

/**
 * Prints an ASCII escape code that makes subsequent output a thick red color.
 */
void red(void)
{
    printf("\033[1;31m");
}

/**
 * Prints an ASCII escape code that makes subsequent output a light red color.
 */
void lightRed(void)
{
    printf("\033[0;31m");
}

/**
 * Prints an ASCII escape code that makes subsequent output a thick white color.
 */
void strongWhite(void)
{
    printf("\033[1;37m");
}

/**
 * Prints an ASCII escape code that makes subsequent output a light white color.
 */
void white(void)
{
    printf("\033[0;37m");
}

/**
 * Prints an ASCII escape code that makes subsequent output a thick green color.
 */
void green(void)
{
    printf("\033[1;32m");
}

/**
 * Prints an ASCII escape code that makes subsequent output a light green color.
 */
void lightGreen(void)
{
    printf("\033[0;32m");
}

/**
 * Prints an ASCII escape code that makes subsequent output a thick yellow color.
 */
void yellow(void)
{
    printf("\033[1;33m");
}

/**
 * Prints an ASCII escape code that makes subsequent output a light yellow color.
 */
void lightYellow(void)
{
    printf("\033[0;33m");
}

/**
 * Prints an ASCII escape code that makes subsequent output a thick blue color.
 */
void blue(void)
{
    printf("\033[1;34m");
}

/**
 * Prints an ASCII escape code that makes subsequent output a light blue color.
 */
void lightBlue(void)
{
    printf("\033[0;34m");
}

/**
 * Prints an ASCII escape code that makes subsequent output a thick purple color.
 */
void purple(void)
{
    printf("\033[1;35m");
}

/**
 * Prints an ASCII escape code that makes subsequent output a light purple color.
 */
void lightPurple(void)
{
    printf("\033[0;35m");
}

/**
 * Prints an ASCII escape code that makes subsequent output a thick cyan color.
 */
void cyan(void)
{
    printf("\033[1;36m");
}

/**
 * Prints an ASCII escape code that makes subsequent output a light cyan color.
 */
void lightCyan(void)
{
    printf("\033[0;36m");
}

#ifdef HELAPORDO_CURSES_BUILD
/**
 * Initialises color pairs for the game.
 */
void init_game_color_pairs(void)
{

    // Initialize all the colors
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_BLACK, COLOR_WHITE);
    init_pair(8, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(9, COLOR_WHITE, COLOR_RED);
    init_pair(10, COLOR_WHITE, COLOR_MAGENTA);

}
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined."
#else
void init_game_color_pairs(void)
{
    printf("%s():    TODO - Implement color pair init for rl-build\n", __func__);
    return;
}
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

#ifdef HELAPORDO_CURSES_BUILD
/**
 * Demoes color pairs from palette.c to the passed WINDOW.
 * @param win The Window pointer to print to.
 * @param colors_per_row How many colors to print in each row.
 */
void test_game_color_pairs(WINDOW *win, int colors_per_row)
{
    if (win == NULL) {
        fprintf(stderr, "[%s]:  Passed Window was NULL.", __func__);
        log_tag("debug_log.txt", "[ERROR]", "[%s]:  Passed Window was NULL.",
                __func__);
        exit(EXIT_FAILURE);
    }

    int x = 1;
    int y = 1;
    int x_offset = 0;

    for (int i = S4C_MIN_COLOR_INDEX; i < S4C_MAX_COLOR_INDEX + 1; i++) {
        int color_index = i;
        if (color_index >= 0) {
            wattron(win, COLOR_PAIR(color_index));
            mvwaddch(win, y, x + x_offset, ' ' | A_REVERSE);
            wattroff(win, COLOR_PAIR(color_index));
        }
        x_offset++;
        if ((color_index - S4C_MIN_COLOR_INDEX + 1) % colors_per_row == 0) {
            x = 1;
            x_offset = 0;
            y++;
        }
    }

    int picked = 0;
    int c = -1;
    wrefresh(win);
    refresh();

    while (!picked && (c = wgetch(win)) != 'q') {
        switch (c) {
        case 10: {		/*Enter */
            picked = 1;

        };
        break;
        }
    }
}
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined."
#else
void test_game_color_pairs(Rectangle * win, int colors_per_row)
{
    (void) win;
    (void) colors_per_row;
    printf("%s():    TODO - Implement game color pairs test for rl-build\n", __func__);
    return;
}
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

/**
 * Sets the passed char array to the expected path for /static/ folder.
 * @param static_path The array that will hold path to static folder.
 */
void resolve_staticPath(char static_path[500])
{
    char homedir_path[200];
#ifndef _WIN32
    sprintf(homedir_path, "%s", getenv("HOME"));
#else
    sprintf(homedir_path, "%s", getenv("UserProfile"));
#endif

#ifndef _WIN32
    const char *static_folder_path_wd = "./";
#else
    const char *static_folder_path_wd = ".\\";
#endif

#ifndef _WIN32
    const char *local_install_static_folder_path = "/helapordo-local/";
#else
    const char *local_install_static_folder_path = "\\helapordo-local\\";
#endif
    char static_folder_path_global[500];
    sprintf(static_folder_path_global, "%s", homedir_path);
    strncat(static_folder_path_global, local_install_static_folder_path, 50);
    struct stat sb;

    if (stat(static_folder_path_wd, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        //sprintf(msg, "[DEBUG]    resolve_staticPath(): Found \"/static/\" dir in working directory (%s).\n",static_folder_path_wd);
        strcpy(static_path, static_folder_path_wd);
    } else {
        //sprintf(msg, "[DEBUG]    resolve_staticPath(): Can't find \"/static/\" dir in working directory (%s). Will try \"%s/helapordo-local/static/\".\n", static_folder_path_wd, homedir_path);
        if (stat(static_folder_path_global, &sb) == 0 && S_ISDIR(sb.st_mode)) {
            //sprintf(msg, "[DEBUG]    resolve_staticPath(): Found \"/static/\" dir in global directory: \"%s/helapordo-local/static/\".\n", homedir_path);
            strcpy(static_path, static_folder_path_global);
        } else {
            //sprintf(msg,"[DEBUG]    resolve_staticPath(): Can't find \"/static/\" dir in \"%s/helapordo-local/static/\". Quitting.\n", homedir_path);
            fprintf(stderr, "\n[ERROR]    Can't find static dir. Quitting.\n");
            fprintf(stderr, "\nHome dir at: (%s).\n", homedir_path);
            fprintf(stderr, "\nGlobal static dir at: (%s).\n",
                    static_folder_path_global);
            fprintf(stderr, "\nWorking static dir at: (%s).\n",
                    static_folder_path_wd);
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * Loads the lore strings from the appropriate lore file.
 * @param lore_strings The string array to copy to.
 * @param loreKind The kind of lore, used to open the corresponding lore file.
 * @see gameloop()
 */
void loadLore(char **lore_strings, int loreKind)
{

    char static_path[500];

    // Set static_path value to the correct static dir path
    resolve_staticPath(static_path);

    char lore_filename[1000];

    sprintf(lore_filename, "%s/lore/lore%i.txt", static_path, loreKind);

    FILE *lorefile = fopen(lore_filename, "r");
    if (!lorefile) {
        fprintf(stderr, "\nCan't open %s\n", lore_filename);
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    //size_t len = 0;
    //ssize_t read;
    //int i = 0;

    // FIXME
    // If we ever need this again, we'd have to not depend on getline() for mingw32
    /*
       while ((read = getline(&line, &len, lorefile)) != -1) {
       log_tag("debug_log.txt","[LORE-LOAD]","Retrieved line of length %zu:", read);
       log_tag("debug_log.txt","[LORE-LOAD]","%s", line);
       strcpy(lore_strings[i],line);
       i++;
       }
     */
    fclose(lorefile);
    if (line)
        free(line);
}

/**
 * Takes a path pointer, a roadFork value pointer, a room type pointer, and an integer.
 * Depending on GAMEMODE (and eventually roadFork value), sets the room type pointer to a roomClass value.
 * @param roadFork_value The pointer value of the roadfork that we must use as result.
 * @param room_type The pointer value of the room type to set.
 * @param roomsDone The total of rooms completed.
 */
void setRoomType(Path *path, int *roadFork_value, roomClass *room_type,
                 int roomsDone)
{
    log_tag("debug_log.txt", "[DEBUG]",
            "setRoomType():  room type (%i) rooms done (%i)", (int)*room_type,
            roomsDone);
    switch (GAMEMODE) {
    case Standard: {
        if ((*roadFork_value >= 0)) {
            *room_type = *roadFork_value;
            *roadFork_value = -1;
            log_tag("debug_log.txt", "[TEST]",
                    "setRoomType() for ROADFORK");
        } else if ((roomsDone == 1) || (roomsDone % HOMEROOM == 0)) {	//Only the first and every nth room will be a HOME room.
            //FIXME: why the hell does roomsDone need to start from 1?
            *room_type = HOME;
            log_tag("debug_log.txt", "[TEST]", "setRoomType() for HOME");
        } else if (roomsDone % BOSSROOM == 0) {
            *room_type = BOSS;
        } else if (roomsDone % SHOPROOM == 0) {
            *room_type = SHOP;
        } else if (rand() % 5 == 0) {
            *room_type = TREASURE;
        } else if (rand() % 4 == 0 && (roomsDone + 2 < path->length)) {
            *room_type = ROADFORK;
        } else if (*room_type == -1) {
            *room_type = ENEMIES;
        }
        if (G_DEBUG_ON && G_DEBUG_ROOMTYPE_ON > 0) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "setRoomType(): Room debug flag asserted in standard gamemode, room type will always be equal to G_DEBUG_ROOMTYPE (%s).",
                    stringFromRoom(G_DEBUG_ROOMTYPE));
            *room_type = G_DEBUG_ROOMTYPE;
        }
    }
    break;
    case Story: {
        if (*roadFork_value >= 0) {	//Is this branch needed here?
            *room_type = *roadFork_value;
            *roadFork_value = -1;
        } else if ((roomsDone == 1) || (roomsDone % HOMEROOM == 0)) {	//Only the first and every nth room will be a HOME room.
            //FIXME: why the hell does roomsDone need to start from 1?
            *room_type = HOME;
            log_tag("debug_log.txt", "[TEST]",
                    "story mode, setRoomType() for HOME");
        } else if (roomsDone % BOSSROOM == 0) {
            *room_type = BOSS;
        } else if (roomsDone % 4 == 0) {
            *room_type = SHOP;
        } else if (rand() % 20 == 0) {
            *room_type = TREASURE;
        } else if (*room_type == -1) {
            *room_type = ENEMIES;
        }
        if (G_DEBUG_ON && G_DEBUG_ROOMTYPE_ON > 0) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "setRoomType(): Room debug flag asserted in standard gamemode, room type will always be equal to G_DEBUG_ROOMTYPE (%s).",
                    stringFromRoom(G_DEBUG_ROOMTYPE));
            *room_type = G_DEBUG_ROOMTYPE;
        }
    }
    break;
    default: {
        fprintf(stderr, "Unexpected GAMEMODE value: %i\n", GAMEMODE);
        exit(EXIT_FAILURE);
    }
    break;
    }
    log_tag("debug_log.txt", "[DEBUG]",
            "setRoomType():  room type (%i) rooms done (%i)", (int)*room_type,
            roomsDone);
}

/**
 * Frees the memory allocated for the passed room pointer.
 * @param room The Room pointer to free.
 * @see Room
 */
void freeRoom(Room *room)
{
    int room_type = room->class;

    log_tag("debug_log.txt", "[DEBUG-FREE]", "Freeing room desc: (%s).",
            room->desc);
    //Free room memory
    //FIXME: do we have all room descs handled ?
    //free(room->desc);

    if (room_type == SHOP) {
        /*
           for (int i = 0 ; i < room->shop->equipsCount; i++) {
           Equip* e = room->shop->equips[i];
           if (e->perksCount > 0) {
           for (int j = 0 ; j < e->perksCount; j++) {
           Perk* p = e->perks[j];
           free(p);
           sprintf(msg,"Freeing %s room (index %i), freed Perk %i for Equip %s.", stringFromRoom(room_type), room->index, j, stringFromEquips(e->class));
           log_tag("debug_log.txt","[FREE]",msg);
           }
           log_tag("debug_log.txt","[FREE]","Done freeing perks.\n");
           }
           sprintf(msg,"Freeing %s room (index %i), freed Equip %s.", stringFromRoom(room_type), room->index, stringFromEquips(e->class));
           free(e);
           log_tag("debug_log.txt","[DEBUG-FREE]",msg);
           }
           for (int i = 0 ; i < room->shop->consumablesCount; i++) {
           Consumable* c = room->shop->consumables[i];
           free(c);
           sprintf(msg,"Freeing %s room (index %i), freed Consumable %i.", stringFromRoom(room_type), room->index, i);
           log_tag("debug_log.txt","[DEBUG-FREE]",msg);
           }
         */
        //FIXME: remove all the commented out bs
        log_tag("debug_log.txt", "[FIXME]", "Empty freeRoom branch");
    } else if (room_type == TREASURE) {
        if (room->treasure->class == TREASURE_CONSUMABLE) {

            /*
               Consumable* dele = room->treasure->consumable;
               sprintf(msg,"Freeing %s room (index %i), freed Consumable %s.\n", stringFromRoom(room_type), room->index, stringFromConsumables(dele->class));
               free(dele);
             */
            //FIXME: remove all the commented out bs
            log_tag("debug_log.txt", "[FIXME]", "Empty freeRoom branch");
        } else if (room->treasure->class == TREASURE_ARTIFACT) {
            /*
               Artifact* dele = room->treasure->artifact;
               sprintf(msg,"Freeing %s room (index %i), freed Artifact %s.\n", stringFromRoom(room_type), room->index, stringFromArtifacts(dele->class));
               free(dele);
             */
            log_tag("debug_log.txt", "[FIXME]", "Empty freeRoom branch");
        } else if (room->treasure->class == TREASURE_CHEST) {
            log_tag("debug_log.txt", "[FIXME]",
                    "freeRoom: freeing Treasure room, CHEST");
            /*
               //FIXME: freeing Treasure Chest here
               Chest* chest = room->treasure->chest;
               for (int eq_i = 0; eq_i < chest->equipsCount; eq_i++) {
               Equip* equip = chest->equips[eq_i];
               for (int p_i = 0 ; p_i < equip->perksCount; p_i++) {
               Perk* pk = equip->perks[p_i];
               free(pk);
               sprintf(msg,"Freeing %s room (index %i), CHEST: freed Perk %i for Equip %s (%i).", stringFromRoom(room_type), room->index, p_i, stringFromEquips(equip->class), eq_i);
               log_tag("debug_log.txt","[DEBUG-FREE]",msg);
               }
               sprintf(msg,"Freeing %s room (index %i), CHEST: freed Equip %s (%i).", stringFromRoom(room_type), room->index, stringFromEquips(equip->class), eq_i);
               free(equip);
               log_tag("debug_log.txt","[DEBUG-FREE]",msg);
               }

               for (int i = 0 ; i < chest->consumablesCount; i++) {
               Consumable* c = chest->consumables[i];
               free(c);
               sprintf(msg,"Freeing %s room (index %i), CHEST: freed Consumable %i.", stringFromRoom(room_type), room->index, i);
               log_tag("debug_log.txt","[DEBUG-FREE]",msg);
               }
               sprintf(msg,"Freeing %s room (index %i), freed Chest.\n", stringFromRoom(room_type), room->index);
               //free(chest);
               log_tag("debug_log.txt","[DEBUG-FREE]",msg);
             */
            log_tag("debug_log.txt", "[FIXME]", "Empty freeRoom branch");
        }
    } else if (room_type == ROADFORK) {
        /*
           Roadfork* rfk = room->roadfork;
           free(rfk);
           sprintf(msg,"Freeing %s room (index %i), freed Roadfork.\n", stringFromRoom(room_type), room->index);
           log_tag("debug_log.txt","[DEBUG-FREE]",msg);
         */
        log_tag("debug_log.txt", "[FIXME]", "Empty freeRoom branch");
    }

    log_tag("debug_log.txt", "[DEBUG-FREE]", "Freed %s room, index %i.\n",
            stringFromRoom(room->class), room->index);
    log_tag("debug_log.txt", "[FIXME]", "freeRoom(): done fake free().");
    //free(room);
}

/**
 * Prints an hardcoded title screen.
 * @see helapordo_title_string
 */
void printTitle(void)
{
    printf("\n\n\n\n\n");
#ifndef _WIN32
    red();
#endif
    printf("%s\n", helapordo_title_string);
#ifndef _WIN32
    white();
#endif
}

/**
 * Prints version string.
 */
void printVersion(void)
{
    printf("%s\n", VERSION);
}

/**
 * Prints formatted version string.
 * @param progName The program name string.
 */
void printFormattedVersion(char *progName)
{
    printf("%s v. %s\n", progName, VERSION);
}

/**
 * Prints configuration info.
 */
void hlpd_dbg_features(void)
{

#ifdef HELAPORDO_DEBUG_ACCESS
    fprintf(stderr,"[HLP]    Debug access in enabled\n");
#else
    fprintf(stderr,"[HLP]    Debug access in not enabled\n");
#endif

#ifdef HELAPORDO_DEBUG_LOG
    fprintf(stderr,"[HLP]    Debug log is enabled\n");
#else
    fprintf(stderr,"[HLP]    Debug log is off\n");
#endif

#ifdef ANVIL__helapordo__
#ifdef INVIL__helapordo__HEADER__
    fprintf(stderr,"[HLP]    Built with invil\n");
#else
    fprintf(stderr,"[HLP]    Built with amboso\n");
#endif // INVIL
#else
    fprintf(stderr,"[HLP]    Built without anvil\n");
#endif // ANVIL

#ifdef HELAPORDO_CURSES_BUILD
    fprintf(stderr,"[HLP]    ncurses build enabled\n");
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
    fprintf(stderr,"[HLP]    raylib build enabled\n");
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
}

/**
 * Prints correct argument syntax for command line invocation.
 */
void usage(char *progname)
{
    fprintf(stderr, "\nUsage:        %s [...options] [name] [class]\n",
            progname);
    fprintf(stderr, "\n    [class]\n\n        [Knight|Archer|Mage|Assassin]\n");
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "\n    -R        Enable rogue mode\n");
    fprintf(stderr, "\n    -s        Enable story mode. Deprecated.\n");
    fprintf(stderr, "    -l        Load a game.\n");
#ifndef HELAPORDO_DEBUG_ACCESS
#else
    fprintf(stderr, "\n    -d        Enable debug mode\n");
    fprintf(stderr, "      -dr <roomType>      Enable forced room.\n");
    fprintf(stderr, "      -dE <enemyType>      Enable forced enemy.\n");
#endif
    fprintf(stderr, "\n    -h        Print this help message\n");
    fprintf(stderr, "    -T        Show a brief tutorial.\n");
    fprintf(stderr, "    -G        Enable godmode.\n");
    fprintf(stderr, "    -X        Enable experimental features.\n");
    fprintf(stderr, "    -v        Prints %s version.\n", progname);
    fprintf(stderr, "    -V        Prints %s build info.\n", progname);
    fprintf(stderr, "    -a        Disable autosave.\n");
    fprintf(stderr, "    -L        Enable logging.\n");
    fprintf(stderr, "    -Q        Enable fast quit.\n");
    fprintf(stderr, "    -t        Test color codes.\n");
}

/**
 * Takes a filename string, a string headear and a format string.
 * Tries logging the message to the passed file if global var G_DEBUG_ON is set.
 * @param filename The filename to open.
 * @param header The string header for the message to log.
 * @param format The format string for message.
 */
void log_tag(char *filename, char *header, const char *format, ...)
{
#ifndef HELAPORDO_DEBUG_LOG
#else
    // Open log file if log flag is set and append to it
    if (G_LOG_ON == 1) {
        char path_to_debug_file[500];
        char static_path[500];

        // Set static_path value to the correct static dir path
        resolve_staticPath(static_path);

        sprintf(path_to_debug_file, "%s", static_path);

#ifndef _WIN32
        strncat(path_to_debug_file, "/", 2);
#else
        strncat(path_to_debug_file, "\\", 2);
#endif

        strncat(path_to_debug_file, filename, 200);

        //fprintf(stderr, "Using %s as path to debug log.\n", path_to_debug_file);

        FILE *logfile = fopen(path_to_debug_file, "a");
        if (!logfile) {
            fprintf(stderr,
                    "Error opening log file.\n Static path: (%s) Filename : (%s).\n",
                    static_path, filename);
            fprintf(stderr, "Path to debug file was: (%s).",
                    path_to_debug_file);
            fprintf(stderr, "Format was:    %s", format);
            exit(EXIT_FAILURE);
        }
        va_list args;
        va_start(args, format);
        time_t now = time(0);
        struct tm *mytime = localtime(&now);
        char timeheader[500];
        if (strftime(timeheader, sizeof timeheader, "%X", mytime)) {
            fprintf(logfile, "[ %s ] [ %-12.12s ] [", timeheader, header);
            vfprintf(logfile, format, args);
            fprintf(logfile, "]\n");
        }
        va_end(args);
        fclose(logfile);
    }
#endif
}

/**
 * Takes a OP_res and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see OP_res
 * @param r The OP_res.
 * @return String corresponding to the OP result.
 */
char *stringFrom_OP_res(OP_res r)
{

    return opresstrings[r];
}

/**
 * Takes a saveType and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see saveType
 * @param s The saveType value.
 * @return String corresponding to the save type.
 */
char *stringFrom_saveType(saveType s)
{

    return savetypestrings[s];
}

/**
 * Takes a turnOption_OP and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see turnOption_OP
 * @param t The turn choice.
 * @return String corresponding to the turn choice.
 */
char *stringFromTurnOP(turnOption_OP t)
{

    return turnopstrings[t];
}

/**
 * Takes a foeTurnOption_OP and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see foeTurnOption_OP
 * @param ft The turn choice.
 * @return String corresponding to the turn choice.
 */
char *stringFromFoeTurnOP(foeTurnOption_OP ft)
{
    return foeturnopstrings[ft];
}

/**
 * Takes a fightResult fr and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fightResult
 * @param fr The fight result value.
 * @return String corresponding to the fight result value.
 */
char *stringFrom_fightResult(fightResult fr)
{

    return fightresultstrings[fr];
}

/**
 * Takes a fighterStatus and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterStatus
 * @param s The status.
 * @return String corresponding to the status.
 */
char *stringFromStatus(fighterStatus s)
{
    static char *strings[] = {
        "Normal",
        "Poison",
        "Frozen",
        "Burned",
        "Weak",
        "Strong"
    };

    return strings[s];
}

/**
 * Takes a winconClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see winconClass
 * @param w The class.
 * @return String corresponding to the class.
 */
char *stringFromWinconClass(winconClass w)
{

    return winconstrings[w];
}

/**
 * Takes a fighterClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @param f The class.
 * @return String corresponding to the class.
 */
char *stringFromClass(fighterClass f)
{

    return classesstrings[f];
}

/**
 * Takes a enemyClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see enemyClass
 * @param e The class.
 * @return String corresponding to the class.
 */
char *stringFromEClass(enemyClass e)
{

    return classenemystrings[e];
}

/**
 * Takes a bossClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see bossClass
 * @param b The class.
 * @return String corresponding to the class.
 */
char *stringFromBossClass(bossClass b)
{

    return classbossstrings[b];
}

/**
 * Takes a integer and returns the corresponding consumable string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see consumableClass
 * @param c The integer/consumableClass.
 * @return String corresponding to the consumable.
 */
char *stringFromConsumables(int c)
{

    return consumablestrings[c];
}

/**
 * Takes a integer and returns the corresponding equip string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see equipClass
 * @param e The integer/equipClass.
 * @return String corresponding to the equip.
 */
char *stringFromEquips(int e)
{

    return equipstrings[e];
}

/**
 * Takes a integer and returns the corresponding equipzone string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see Equipzone
 * @param z The integer/Equipzone.
 * @return String corresponding to the equipzone.
 */
char *stringFromEquipzones(int z)
{

    return equipzonestrings[z];
}

/**
 * Takes a integer and returns the corresponding quality string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see quality
 * @param q The integer/quality
 * @return String corresponding to the quality.
 */
char *stringFromQuality(int q)
{

    return qualitytrings[q];
}

/**
 * Takes a integer and returns the corresponding artifact string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see artifactClass
 * @param a The integer/artifactClass.
 * @return String corresponding to the artifact.
 */
char *stringFromArtifacts(int a)
{

    return artifactstrings[a];
}

/**
 * Takes a fighterClass and a specialMove and returns the corresponding name string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @see specialMove
 * @param class The fighterClass.
 * @param move The specialMove.
 * @return String corresponding to the special move.
 */
char *nameStringFromSpecial(fighterClass class, specialMove move)
{
    return specialsnamestrings[class][(move % (SPECIALSMAX + 1))];
}

/**
 * Takes a fighterClass and a specialMove and returns the corresponding desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @see specialMove
 * @param class The fighterClass.
 * @param move The specialMove.
 * @return String corresponding to the special move.
 */
char *descStringFromSpecial(fighterClass class, specialMove move)
{
    return specialsdescstrings[class][(move % (SPECIALSMAX + 1))];
}

/**
 * Takes a fighterClass and a specialMove and returns the corresponding cost by the inner array position, as an integer.
 * Correct result is only possible by having the enum values in a consistent order with the integer array.
 * @see fighterClass
 * @see specialMove
 * @param class The fighterClass.
 * @param move The specialMove.
 * @return int Cost of the corresponding special move.
 */
int costFromSpecial(fighterClass class, specialMove move)
{
    return specialscosts[class][(move % (SPECIALSMAX + 1))];
}

/**
 * Takes a skillType and returns the corresponding name string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @see specialMove
 * @param class The skillType.
 * @return String corresponding to the skillType.
 */
char *nameStringFromSkill(skillType class)
{
    return skillsnamestrings[class];
}

/**
 * Takes a fighterClass and a specialMove and returns the corresponding desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @see skillType
 * @param class The skillType.
 * @return String corresponding to the skillType.
 */
char *descStringFromSkill(skillType class)
{
    return skillsdescstrings[class];
}

/**
 * Takes a skillType and returns the corresponding cost by the inner array position, as an integer.
 * Correct result is only possible by having the enum values in a consistent order with the integer array.
 * @see fighterClass
 * @see skillType
 * @param class The skillType.
 * @return int Cost of the corresponding skillType.
 */
int costFromSkill(skillType class)
{
    return skillscosts[class];
}

/**
 * Takes a integer and returns the corresponding perk name string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see perkClass
 * @see Perk
 * @param p The integer/perkClass.
 * @return String corresponding to the perk name.
 */
char *nameStringFromPerk(int p)
{

    return perksnamestrings[p];
}

/**
 * Takes a integer and returns the corresponding perk desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see perkClass
 * @see Perk
 * @param p The integer/perkClass.
 * @return String corresponding to the perk desc.
 */
char *descStringFromPerk(int p)
{

    return perksdescstrings[p];
}

/**
 * Takes a integer and returns the corresponding treasure desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see treasureClass
 * @param t The integer/treasureClass.
 * @return String corresponding to the treasure desc.
 */
char *descStringFromTreasure(int t)
{

    return treasuredescstrings[t];
}

/**
 * Takes a integer and returns the corresponding chest desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see chestClass
 * @param c The integer/chestClass.
 * @return String corresponding to the chest desc.
 */
char *descStringFromChest(int c)
{
    return chestdescstrings[c];
}

/**
 * Takes a integer and returns the corresponding FoeParty desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see foePartyClass
 * @param c The integer/foePartyClass.
 * @return String corresponding to the FoeParty desc.
 */
char *stringFromFoePartyClass(foePartyClass fp)
{
    return foepartystrings[fp];
}

/**
 * Takes a integer and returns the corresponding HLP_Region_Type string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see HLP_Region_Type
 * @param t The HLP type.
 * @return String corresponding to the HLP_Region_Type name.
 */
char *stringFrom_HLP_Region_Type(HLP_Region_Type t)
{
    return hlp_regiontype_strings[t];
}

/**
 * Takes a integer and returns the corresponding room name string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see roomClass
 * @param r The integer/roomClass.
 * @return String corresponding to the room name.
 */
char *stringFromRoom(roomClass r)
{
    return roomnamestrings[r];
}

/**
 * Takes a integer and returns the corresponding gamemode string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see Gamemode
 * @param g The integer/Gamemode.
 * @return String corresponding to the gamemode.
 */
char *stringFromGamemode(Gamemode g)
{
    return gamemodenamestrings[g];
}

/**
 * Takes a integer and returns the corresponding floorClass string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see floorClass
 * @param fc The integer/floorClass.
 * @return String corresponding to the floorClass.
 */
char *stringFromFloorclass(floorClass fc)
{
    return floornamestrings[fc];
}

/**
 * Takes a Fighter pointer and sets its name value to the string provided on stdin.
 * @param player The pointer whose name value will be set.
 */
void scanName(Fighter *player)
{
    char name[50];
    int f;
    do {
        printf("\n\n\tWhat's your name?\n\n");
        f = scanf("%20s", name);
    } while (f >= 21 || f <= 0);
    strcpy(player->name, name);
}

/**
 * Prints all the fighterClass values as integers and as strings.
 * @see fighterClass
 */
void printClasses(void)
{
    printf("[");
    for (int i = 0; i <= CLASSESMAX; i++) {
        printf("\t%d)\t%s", i, stringFromClass(i));
        if (i != CLASSESMAX) {
            printf("\n");
        }
    };
    printf("\t]\n");
}

/**
 * Asks for an integer on stdin and returns it if it's a valid fighterClass index.
 * @see fighterClass
 * @see CLASSESMAX
 * @return The selected integer, negative for unvalid choices.
 */
int scanClass(void)
{
    int pick = -1;
    char c;
    while (pick < 0 || pick > CLASSESMAX) {
        printf("\nPick a class: (0-%i)\n", CLASSESMAX);
        int res = scanf("%1i", &pick);
        log_tag("debug_log.txt", "[DEBUG]", "scanClass scanf() res was %i",
                res);
        res = scanf("%c", &c);
        log_tag("debug_log.txt", "[DEBUG]", "scanClass 2 scanf() res was %i",
                res);
    };
    return pick;
}

/**
 * Takes a Fighter pointer and sets the class value after asking the user for input.
 * @see fighterClass
 * @see scanClass()
 * @param player The pointer whose class value will be set.
 */
void pickClass(Fighter *player)
{
    int pick = -1;
    do {
        int res = system("clear");
        log_tag("debug_log.txt", "[DEBUG]",
                "pickClass() system(\"clear\") res was (%i)", res);
        printf("\nPick a class.\n");
        printClasses();
        pick = scanClass();
    } while (pick < 0);

    player->class = pick;
    green();
    printf("\n\n\tClass: %s\n\n", stringFromClass(player->class));
    white();
}

/**
 * Prints all the winconClass values as integers and as strings.
 * @see winconClass
 */
void printWincons(void)
{
    printf("[");
    for (int i = 0; i <= WINCON_CLASS_MAX; i++) {
        printf("\t%d)\t%s", i, stringFromWinconClass(i));
        if (i != WINCON_CLASS_MAX) {
            printf("\n");
        }
    };
    printf("\t]\n");
}

/**
 * Asks for an integer on stdin and returns it if it's a valid winconClass index.
 * @see winconClass
 * @see WINCON_CLASS_MAX
 * @return The selected integer, negative for unvalid choices.
 */
int scanWincon(void)
{
    int pick = -1;
    char c;
    while (pick < 0 || pick > WINCON_CLASS_MAX) {
        printf("\nPick a win condition: (0-%i)\n", WINCON_CLASS_MAX);
        int res = scanf("%1i", &pick);
        log_tag("debug_log.txt", "[DEBUG]", "scanWincon() scanf() res was (%i)",
                res);
        res = scanf("%c", &c);
        log_tag("debug_log.txt", "[DEBUG]",
                "scanWincon() 2 scanf() res was (%i)", res);
    };
    return pick;
}

/**
 * Takes a Fighter pointer and sets the name value after asking the user for input.
 * @see fighterClass
 * @see scanName()
 * @param player The pointer whose name value will be set.
 */
void pickName(Fighter *player)
{
    scanName(player);
    red();
    printf("\n\n\tName: %s\n\n", player->name);
    white();
}

/**
 * Takes a Wincon pointer and sets its class after asking the user for input.
 * @see winconClass
 * @see scanWincon()
 * @param w The wincon pointer whose class value will be set.
 */
void pickWincon(Wincon *w)
{
    int pick = -1;
    do {
        int res = system("clear");
        log_tag("debug_log.txt", "[DEBUG]",
                "pickWincon() system(\"clear\") res was (%i)", res);
        printf("\nPick a win condition.\n");
        printWincons();
        pick = scanWincon();
    } while (pick < 0);

    w->class = pick;
    green();
    printf("\n\n\tWincon: %s\n\n", stringFromWinconClass(w->class));
    white();
}

/**
 * Takes two Fighter pointers, with all their fields already allocated, and copies all valaues from source to dest.
 * @see Fighter
 * @param source The Fighter pointer to be copied.
 * @param dest The Fighter pointer to copy to.
 */
void copy_fighter(Fighter *source, Fighter *dest)
{

    strcpy(source->name, dest->name);
    dest->class = source->class;
    dest->hp = source->hp;
    dest->atk = source->atk;
    dest->def = source->def;
    dest->vel = source->vel;
    dest->level = source->level;
    dest->luck = source->luck;
    dest->totalxp = source->totalxp;
    dest->currentlevelxp = source->currentlevelxp;
    dest->totallevelxp = source->totallevelxp;
    dest->totalhp = source->totalhp;
    dest->status = source->status;

    dest->energy = source->energy;
    dest->totalenergy = source->totalenergy;
    for (int i = 0; i < SPECIALSMAX + 1; i++) {
        dest->specials[i] = source->specials[i];
    }

    for (int i = 0; i < COUNTERSMAX + 1; i++) {
        dest->counters[i] = source->counters[i];
    }
    dest->turnboost_atk = source->turnboost_atk;
    dest->turnboost_def = source->turnboost_def;
    dest->turnboost_vel = source->turnboost_vel;
    dest->turnboost_enr = source->turnboost_enr;

    dest->perksCount = source->perksCount;
    for (int i = 0; i < PERKSMAX + 1; i++) {
        dest->perks[i] = source->perks[i];
    }

    for (int i = 0; i < EQUIPZONES + 1; i++) {
        dest->equipslots[i] = source->equipslots[i];
    }
    for (int i = 0; i < EQUIPSBAGSIZE + 1; i++) {
        dest->equipsBag[i] = source->equipsBag[i];
    }
    for (int i = 0; i < CONSUMABLESMAX + 1; i++) {
        dest->consumablesBag[i] = source->consumablesBag[i];
    }
    for (int i = 0; i < ARTIFACTSMAX + 1; i++) {
        dest->artifactsBag[i] = source->artifactsBag[i];
    }

    dest->equipsBagOccupiedSlots = source->equipsBagOccupiedSlots;
    dest->earliestBagSlot = source->earliestBagSlot;

    dest->permboost_atk = source->permboost_atk;
    dest->permboost_def = source->permboost_def;
    dest->permboost_vel = source->permboost_vel;
    dest->permboost_enr = source->permboost_enr;

    dest->equipboost_atk = source->equipboost_atk;
    dest->equipboost_def = source->equipboost_def;
    dest->equipboost_vel = source->equipboost_vel;
    dest->equipboost_enr = source->equipboost_enr;

    dest->stats = source->stats;
    dest->balance = source->balance;
}

/**
 * Takes a fighterStatus and returns the corresponding effect_fun pointer for the function relating to the status.
 * @see fighterStatus
 * @see effect_fun
 * @param status The fighterStatus that the caller needs to match to a effect_fun pointer.
 * @return The effect_fun pointer related to the status.
 */
effect_fun getStatusCounterFun(fighterStatus status)
{
    switch (status) {
    //case Normal: {
    default: {
        return &resetFighterStatus;
    }
    break;
    };
}

/**
 * Takes a fighterStatus and returns the corresponding effect_e_fun pointer for the function relating to the status.
 * @see fighterStatus
 * @see effect_e_fun
 * @param status The fighterStatus that the caller needs to match to a effect_e_fun pointer.
 * @return The effect_e_fun pointer related to the status.
 */
effect_e_fun getStatusCounterEnemyFun(fighterStatus status)
{
    switch (status) {
    //case Normal: {
    default: {
        return &resetEnemyStatus;
    }
    break;
    };
}

/**
 * Takes a fighterStatus and returns the corresponding effect_e_fun pointer for the function relating to the status.
 * @see fighterStatus
 * @see effect_e_fun
 * @param status The fighterStatus that the caller needs to match to a effect_e_fun pointer.
 * @return The effect_e_fun pointer related to the status.
 */
effect_b_fun getStatusCounterBossFun(fighterStatus status)
{
    switch (status) {
    //case Normal: {
    default: {
        return &resetBossStatus;
    }
    break;
    };
}

//Counter callback getters

/**
 * Takes a Stat and returns the corresponding boost_fun pointer for the function relating to the stat.
 * @see Stat
 * @see boost_fun
 * @param s The Stat that the caller needs to match to a boost_fun pointer.
 * @return The boost_fun pointer related to the stat.
 */
boost_fun getStatBoostCounterFun(Stat s)
{

    switch (s) {
    case ATK: {
        return &set_turnboost_atk;
    }
    break;
    case DEF: {
        return &set_turnboost_def;
    }
    break;
    case VEL: {
        return &set_turnboost_vel;
    }
    break;
    case ENR: {
        return &set_turnboost_enr;
    }
    break;
    default: {
        log_tag("debug_log.txt", "[ERROR]",
                "Invalid stat value in getStatBoostCounterFun()");
        exit(EXIT_FAILURE);
    }
    break;
    };
}

/**
 * Takes a Stat and returns the corresponding boost_e_fun pointer for the function relating to the stat.
 * @see Stat
 * @see boost_e_fun
 * @param s The Stat that the caller needs to match to a boost_e_fun pointer.
 * @return The boost_e_fun pointer related to the stat.
 */
boost_e_fun getStatBoostCounterEnemyFun(Stat s)
{

    switch (s) {
    case ATK: {
        return &set_enemy_turnboost_atk;
    }
    break;
    case DEF: {
        return &set_enemy_turnboost_def;
    }
    break;
    case VEL: {
        return &set_enemy_turnboost_vel;
    }
    break;
    case ENR: {
        return &set_enemy_turnboost_enr;
    }
    break;
    default: {
        log_tag("debug_log.txt", "[ERROR]",
                "Invalid stat value in getStatBoostCounterEnemyFun()");
        exit(EXIT_FAILURE);
    }
    break;
    };
}

/**
 * Takes a Stat and returns the corresponding boost_b_fun pointer for the function relating to the stat.
 * @see Stat
 * @see boost_b_fun
 * @param s The Stat that the caller needs to match to a boost_b_fun pointer.
 * @return The boost_b_fun pointer related to the stat.
 */
boost_b_fun getStatBoostCounterBossFun(Stat s)
{

    switch (s) {
    case ATK: {
        return &set_boss_turnboost_atk;
    }
    break;
    case DEF: {
        return &set_boss_turnboost_def;
    }
    break;
    case VEL: {
        return &set_boss_turnboost_vel;
    }
    break;
    case ENR: {
        return &set_boss_turnboost_enr;
    }
    break;
    default: {
        log_tag("debug_log.txt", "[ERROR]",
                "Invalid stat value in getStatBoostCounterBossFun()");
        exit(EXIT_FAILURE);
    }
    break;
    };
}

/**
 * Takes a Stat and returns the corresponding boost_fp_fun pointer for the function relating to the stat.
 * @see Stat
 * @see boost_fp_fun
 * @param s The Stat that the caller needs to match to a boost_fp_fun pointer.
 * @return The boost_fp_fun pointer related to the stat.
 */
boost_fp_fun get_StatBoostCounter_FoeParty_Fun(Stat s)
{

    switch (s) {
    case ATK: {
        return &set_foeparty_turnboost_atk;
    }
    break;
    case DEF: {
        return &set_foeparty_turnboost_def;
    }
    break;
    case VEL: {
        return &set_foeparty_turnboost_vel;
    }
    break;
    case ENR: {
        return &set_foeparty_turnboost_enr;
    }
    break;
    default: {
        log_tag("debug_log.txt", "[ERROR]",
                "Invalid stat value in getStatBoostCounter_FoeParty_Fun()");
        exit(EXIT_FAILURE);
    }
    break;
    };
}

//Stat boost functions

/**
 * Takes a Fighter pointer and an integer, used to set the turnboost_atk value at the pointer.
 * @see Fighter
 * @see Turncounter
 * @param f The Fighter pointer whose turnboost_atk value will be set.
 * @param boost The value that will be set.
 */
void set_turnboost_atk(Fighter *f, int boost)
{
    //Straight addition of the boost
    f->turnboost_atk = boost;
}

/**
 * Takes a Fighter pointer and an integer, used to set the turnboost_def value at the pointer.
 * @see Fighter
 * @see Turncounter
 * @param f The Fighter pointer whose turnboost_def value will be set.
 * @param boost The value that will be set.
 */
void set_turnboost_def(Fighter *f, int boost)
{
    //Straight addition of the boost
    f->turnboost_def = boost;
}

/**
 * Takes a Fighter pointer and an integer, used to set the turnboost_vel value at the pointer.
 * @see Fighter
 * @see Turncounter
 * @param f The Fighter pointer whose turnboost_vel value will be set.
 * @param boost The value that will be set.
 */
void set_turnboost_vel(Fighter *f, int boost)
{
    //Straight addition of the boost
    f->turnboost_vel = boost;
}

/**
 * Takes a Fighter pointer and an integer, used to set the turnboost_enr value at the pointer.
 * @see Fighter
 * @see Turncounter
 * @param f The Fighter pointer whose turnboost_enr value will be set.
 * @param boost The value that will be set.
 */
void set_turnboost_enr(Fighter *f, int boost)
{
    //Straight addition of the boost
    f->turnboost_enr = boost;
}

/**
 * Takes a Enemy pointer and an integer, used to set the turnboost_atk value at the pointer.
 * @see Enemy
 * @see Turncounter
 * @param e The Enemy pointer whose turnboost_atk value will be set.
 * @param boost The value that will be set.
 */
void set_enemy_turnboost_atk(Enemy *e, int boost)
{
    //Straight addition of the boost
    e->turnboost_atk = boost;
}

/**
 * Takes a Enemy pointer and an integer, used to set the turnboost_def value at the pointer.
 * @see Enemy
 * @see Turncounter
 * @param e The Enemy pointer whose turnboost_def value will be set.
 * @param boost The value that will be set.
 */
void set_enemy_turnboost_def(Enemy *e, int boost)
{
    //Straight addition of the boost
    e->turnboost_def = boost;
}

/**
 * Takes a Enemy pointer and an integer, used to set the turnboost_vel value at the pointer.
 * @see Enemy
 * @see Turncounter
 * @param e The Enemy pointer whose turnboost_vel value will be set.
 * @param boost The value that will be set.
 */
void set_enemy_turnboost_vel(Enemy *e, int boost)
{
    //Straight addition of the boost
    e->turnboost_vel = boost;
}

/**
 * Takes a Enemy pointer and an integer, used to set the turnboost_enr value at the pointer.
 * @see Enemy
 * @see Turncounter
 * @param e The Enemy pointer whose turnboost_enr value will be set.
 * @param boost The value that will be set.
 */
void set_enemy_turnboost_enr(Enemy *e, int boost)
{
    //Straight addition of the boost
    e->turnboost_enr = boost;
}

/**
 * Takes a Boss pointer and an integer, used to set the turnboost_atk value at the pointer.
 * @see Boss
 * @see Turncounter
 * @param b The Boss pointer whose turnboost_atk value will be set.
 * @param boost The value that will be set.
 */
void set_boss_turnboost_atk(Boss *b, int boost)
{
    //Straight addition of the boost
    b->turnboost_atk = boost;
}

/**
 * Takes a Boss pointer and an integer, used to set the turnboost_def value at the pointer.
 * @see Boss
 * @see Turncounter
 * @param b The Boss pointer whose turnboost_def value will be set.
 * @param boost The value that will be set.
 */
void set_boss_turnboost_def(Boss *b, int boost)
{
    //Straight addition of the boost
    b->turnboost_def = boost;
}

/**
 * Takes a Boss pointer and an integer, used to set the turnboost_vel value at the pointer.
 * @see Boss
 * @see Turncounter
 * @param b The Boss pointer whose turnboost_vel value will be set.
 * @param boost The value that will be set.
 */
void set_boss_turnboost_vel(Boss *b, int boost)
{
    //Straight addition of the boost
    b->turnboost_vel = boost;
}

/**
 * Takes a Boss pointer and an integer, used to set the turnboost_enr value at the pointer.
 * @see Boss
 * @see Turncounter
 * @param b The Boss pointer whose turnboost_enr value will be set.
 * @param boost The value that will be set.
 */
void set_boss_turnboost_enr(Boss *b, int boost)
{
    //Straight addition of the boost
    b->turnboost_enr = boost;
}

/**
 * Takes a FoeParty pointer and an integer, used to set the turnboost_atk value at the pointer.
 * @see FoeParty
 * @see Turncounter
 * @param fp The FoeParty pointer whose turnboost_atk value will be set.
 * @param boost The value that will be set.
 */
void set_foeparty_turnboost_atk(FoeParty *fp, int boost)
{
    //Straight addition of the boost
    fp->turnboost_atk = boost;
}

/**
 * Takes a FoeParty pointer and an integer, used to set the turnboost_def value at the pointer.
 * @see FoeParty
 * @see Turncounter
 * @param fp The FoeParty pointer whose turnboost_def value will be set.
 * @param boost The value that will be set.
 */
void set_foeparty_turnboost_def(FoeParty *fp, int boost)
{
    //Straight addition of the boost
    fp->turnboost_def = boost;
}

/**
 * Takes a FoeParty pointer and an integer, used to set the turnboost_vel value at the pointer.
 * @see FoeParty
 * @see Turncounter
 * @param fp The FoeParty pointer whose turnboost_vel value will be set.
 * @param boost The value that will be set.
 */
void set_foeparty_turnboost_vel(FoeParty *fp, int boost)
{
    //Straight addition of the boost
    fp->turnboost_vel = boost;
}

/**
 * Takes a FoeParty pointer and an integer, used to set the turnboost_enr value at the pointer.
 * @see FoeParty
 * @see Turncounter
 * @param fp The FoeParty pointer whose turnboost_enr value will be set.
 * @param boost The value that will be set.
 */
void set_foeparty_turnboost_enr(FoeParty *fp, int boost)
{
    //Straight addition of the boost
    fp->turnboost_atk = boost;
}

//Status effect functions

/**
 * Takes a Fighter pointer and resets its status value to Normal.
 * @see Fighter
 * @see fighterStatus
 * @param f The pointer whose status value will be reset.
 */
void resetFighterStatus(Fighter *f)
{
    f->status = Normal;
}

/**
 * Takes a Enemy pointer and resets its status value to Normal.
 * @see Enemy
 * @see fighterStatus
 * @param e The pointer whose status value will be reset.
 */
void resetEnemyStatus(Enemy *e)
{
    e->status = Normal;
}

/**
 * Takes a Boss pointer and resets its status value to Normal.
 * @see Boss
 * @see fighterStatus
 * @param b The pointer whose status value will be reset.
 */
void resetBossStatus(Boss *b)
{
    b->status = Normal;
}

/**
 * Takes a size int and a pointer to integer array, initialises all the prices, depending on stats from the Equips array pointer.
 * @see initShop()
 * @see Equip
 * @param size Size of both arrays.
 * @param equipPrices Pointer to integer array for prices.
 * @param equips Pointer to Equip pointer to influence pricing.
 */
void setEquipPrices(int size, int *equipPrices, Equip *equips[])
{

    for (int i = 0; i < size; i++) {
        int *cur_price = &equipPrices[i];
        Equip *e = equips[i];

        int baseprice = e->cost;
        int price = baseprice;

        *cur_price = price;
    }

}

/**
 * Takes a size int and a pointer to integer array, initialises all the prices.
 * @see initShop()
 * @param size Size of both arrays.
 * @param consumablePrices Pointer to integer array for prices.
 * @param consumables Pointer to Consumable pointer to influence pricing.
 */
void setConsumablePrices(int size, int *consumablePrices,
                         Consumable **consumables)
{

    for (int i = 0; i < size; i++) {
        int *cur_price = &consumablePrices[i];
        //Consumable* c = consumables[i];

        //Price evaluation
        int baseprice = 4;
        int price = baseprice + (rand() % 5) - 1;

        *cur_price = price;
    }
}

/**
 * Takes a Boss pointer and an integer used to force execution.
 * If the force parameter is true, all checks are ignored.
 * If boss's hp value is at least 40% of total, and none of atk, def or vel is 0 or less, nothing happens with an early return.
 * Otherwise, getBossBoost() is called to calc the level stat boost for each stat.
 * The BossBaseStats pointer for the boss's bossClass is loaded and each one of atk, def and vel is checked accounting for level boost.
 * If none of them is below the respective treshold of 30, 30 and 20 % of total, nothing happens.
 * Otherwise, all of them are reset to full amount accounting for beast boost and level boost.
 * @see Boss
 * @see bossClass
 * @see getBossBoost()
 * @see BSTFACTOR
 * @see BossBaseStats
 * @param b The Boss pointer to check the stats for.
 * @param force The integer to bypass all checks if true.
 */
void statResetBoss(Boss *b, int force)
{
    if (!force && (b->hp >= 0.4 * b->totalhp)
        && !(b->atk <= 0 || b->def <= 0 || b->vel <= 0)) {
        return;
    }
    int boost = getBossBoost(b->level, b->class);

    float beastf = 1;

    if (b->beast) {
        beastf = BSTFACTOR;
    }
    BossBaseStats *base = &basebossstats[b->class];

    if (force) {		//We also update hp values
        int hpBoost =
            boost + round(base->level * 0.75) + (base->hp / 10) +
            ((base->def / 4) % 10);
        b->hp = round(beastf * (base->hp + hpBoost));
        b->totalhp = b->hp;
    }

    if (force || b->vel <= (0.3 * (base->vel + boost))
        || (b->atk <= (0.3 * (base->atk + boost)))
        || b->def <= (0.2 * (base->def + boost))) {
        b->vel = round(beastf * (base->vel + boost));
        b->atk = round(beastf * (base->atk + boost));
        b->def = round(beastf * (base->def + boost));
        //Reset stats
        if (!force) {
            //yellow();
            //printf("\n\n\t%s's stats reset.\n",stringFromEClass(e->class));
            //white();
        }
    }
}

/**
 * Takes an Enemy pointer and an integer used to force execution.
 * If the force parameter is true, all checks are ignored.
 * If enemy's hp value is at least 40% of total, and none of atk, def or vel is 0 or less, nothing happens with an early return.
 * Otherwise, getEnemyBoost() is called to calc the level stat boost for each stat.
 * The EnemyBaseStats pointer for the enemy's enemyClass is loaded and each one of atk, def and vel is checked accounting for level boost.
 * If none of them is below the respective treshold of 30, 30 and 20 % of total, nothing happens.
 * Otherwise, all of them are reset to full amount accounting for beast boost and level boost.
 * @see Enemy
 * @see enemyClass
 * @see getEnemyBoost()
 * @see BSTFACTOR
 * @see EnemyBaseStats
 * @param e The Enemy pointer to check the stats for.
 * @param force The integer to bypass all checks if true.
 */
void statResetEnemy(Enemy *e, int force)
{
    log_tag("debug_log.txt", "[DEBUG]",
            "Call to statResetEnemy() with ($force) == (%i)", force);
    if (!force && (e->hp >= 0.4 * e->totalhp)
        && !(e->atk <= 0 || e->def <= 0 || e->vel <= 0)) {
        return;
    }
    int boost = getEnemyBoost(e->level, e->class);

    float beastf = 1;

    if (e->beast) {
        beastf = BSTFACTOR;
    }
    EnemyBaseStats *base = &baseenemystats[e->class];

    if (force) {		//We also update hp values
        int hpBoost =
            floor(0.5 * boost + round(base->level * 0.75) + (base->hp / 10) +
                  ((base->def / 4) % 10));
        e->hp = round(beastf * (base->hp + hpBoost));
        e->totalhp = e->hp;
    }

    if (force || e->vel <= (0.3 * (base->vel + boost))
        || (e->atk <= (0.3 * (base->atk + boost)))
        || e->def <= (0.2 * (base->def + boost))) {
        e->vel = round(beastf * (base->vel + boost));
        e->atk = round(beastf * (base->atk + boost));
        e->def = round(beastf * (base->def + boost));
        //Reset stats
        if (!force) {
            //yellow();
            //printf("\n\n\t%s's stats reset.\n",stringFromEClass(e->class));
            //white();
        }
    }
}

/**
 * Takes one integer and a bossClass and returns the boost relative to the level delta with base boss stats, as an integer.
 * The EnemyBossStats pointer for the boss's bossClass is loaded.
 * If the boost is negative, returns 0.
 * @see Boss
 * @see bossClass
 * @see BossBaseStats
 * @param lvl The level to check the boost against.
 * @param bclass The bossClass used to determine base level.
 * @return int The boost for any given stat, at the level passed as argument.
 */
int getBossBoost(int lvl, bossClass bclass)
{

    BossBaseStats *base = &basebossstats[bclass];

    float boost = ((lvl - base->level) * 1.25);
    if (boost <= 0) {
        boost = 0;
    }

    return (int)boost;
}

/**
 * Takes one integer and an enemyClass and returns the boost relative to the level delta with base enemy stats, as an integer.
 * The EnemyBaseStats pointer for the enemy's enemyClass is loaded.
 * If the boost is negative, returns 0.
 * @see Enemy
 * @see enemyClass
 * @see EnemyBaseStats
 * @param lvl The level to check the boost against.
 * @param eclass The enemyClass used to determine base level.
 * @return int The boost for any given stat, at the level passed as argument.
 */
int getEnemyBoost(int lvl, enemyClass eclass)
{

    EnemyBaseStats *base = &baseenemystats[eclass];

    float boost = ((lvl - base->level) * 1.25);
    if (boost <= 0) {
        boost = 0;
    }

    return (int)boost;
}

/**
 * Takes a Fighter pointer and resets all of its permboost_STAT values to 0, also correctly updating the current stat values.
 * @see Fighter
 * @param f The fighter pointer whose permboosts will be reset.
 */
void resetPermboosts(Fighter *f)
{
    for (int i = 0; i < STATMAX + 1; i++) {
        switch (i) {
        case ATK: {
            f->atk -= f->permboost_atk;
            if (f->atk < 0) {
                f->atk = 0;
            };
            f->permboost_atk = 0;
        }
        break;
        case DEF: {
            f->def -= f->permboost_def;
            if (f->def < 0) {
                f->def = 0;
            };
            f->permboost_def = 0;
        }
        break;
        case VEL: {
            f->vel -= f->permboost_vel;
            if (f->vel < 0) {
                f->vel = 0;
            };
            f->permboost_vel = 0;
        }
        break;
        case ENR: {
            f->totalenergy -= f->permboost_enr;
            f->energy -= f->permboost_enr;
            if (f->energy < 0) {
                f->energy = 0;
            };
            f->permboost_enr = 0;
        }
        break;
        };
    };
}

/**
 * Takes a Fighter pointer and applies all of its permboost_STAT values by adding them to the current stat values.
 * @see Fighter
 * @param f The fighter pointer whose permboosts will be applied.
 */
void applyPermboosts(Fighter *f)
{
    for (int i = 0; i < STATMAX + 1; i++) {
        switch (i) {
        case ATK: {
            f->atk += f->permboost_atk;
        }
        break;
        case DEF: {
            f->def += f->permboost_def;
        }
        break;
        case VEL: {
            f->vel += f->permboost_vel;
        }
        break;
        case ENR: {
            f->totalenergy += f->permboost_enr;
            f->energy += f->permboost_enr;
        }
        break;
        };
    };
}

/**
 * Takes a Fighter pointer and Resets the active value for each Artifact in the fighter's artifactsBag array that doesn't have an always active trait.
 * At the moment, this only excludes CHAOSORB.
 * @see Artifact
 * @see artifactClass
 * @see Fighter
 */
void resetArtifactsState(Fighter *f)
{
    for (int i = 0; i < (ARTIFACTSMAX + 1); i++) {

        //if (i == CHAOSORB) { //Chaosorb never gets reset
        //      continue;
        //};

        if (f->artifactsBag[i]->qty != 0) {	//We only reset the ones we have
            f->artifactsBag[i]->active = 0;
        };
    };
}
