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
 * Takes a Wincon and a Path pointers and a winconClass and initialises the passed Wincon.
 * @see Wincon
 * @see Path
 * @see winconClass
 * @see WINCON_CLASS_MAX
 * @param w The Wincon pointer to initialise.
 * @param p The Path to use to initialise Wincon.
 * @param class The winconClass to use to initialise.
 */
void initWincon(Wincon *w, Path *p, winconClass class)
{

    w->class = class;

    switch (w->class) {
    case ALL_BOSSES: {
        w->current_val = 0;
        w->target_val = BOSSCLASSESMAX + 1;

    };
    break;
    case ALL_ARTIFACTS: {
        w->current_val = 0;
        w->target_val = ARTIFACTSMAX + 1;

    };
    break;
    case FULL_PATH: {
        w->current_val = 0;
        w->target_val = p->length;
    };
    break;
    default: {
        fprintf(stderr, "\nUnexpected Wincon Class %i\n", w->class);
        w->class = -1;
        w->current_val = 0;
        w->target_val = 1;
    }
    }
}

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

    log_tag("debug_log.txt", "[FIGHTER]", "Fighter floor X position: { %i }",
            fighter->floor_x);
    log_tag("debug_log.txt", "[FIGHTER]", "Fighter floor Y position: { %i }",
            fighter->floor_y);
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
    log_tag("debug_log.txt", "[GAMESTATE]", "is_localexe == (%s)", (gmst->is_localexe ? "true" : "false"));
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
 * @param current_room Pointer to current Room.
 */
void update_Gamestate(Gamestate *gmst, int current_fighters,
                      roomClass current_roomtype, int current_room_index,
                      int current_enemy_index, Floor *current_floor, Room* current_room)
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
    gmst->current_room = current_room;
}

/**
 * Updates the passed Fighter's Equipslots item fields, by reading the equipsBag field and setting the equipped ones.
 * @param f The Fighter to update.
 */
void update_Equipslots(Fighter* f)
{
    for (int i=0; i < f->equipsBagOccupiedSlots; i++) {
        Equip* e = f->equipsBag[i];
        if (e == NULL) {
            log_tag("debug_log.txt", "[WARN]", "%s():    equipsBag at index [%i] is NULL.", __func__, i);
            continue;
        }
        if (e->equipped) {
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Setting Equip [%s] [bag idx: %i] to Fighter's equipslot.", __func__, stringFromEquips(e->class), i);
            f->equipslots[e->type]->item = e;
        }
    }
}

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
 * Takes a Fighter pointer and an integer used to force execution.
 * If the force parameter is true, all checks are ignored.
 * If enemy's hp value is at least 50% of total, and none of atk, def or vel is 0 or less, nothing happens with an early return.
 * Otherwise, getBoost() is called to calc the level stat boost for each stat.
 * The BaseStats pointer for the fighter's fighterClass is loaded and each one of atk, def and vel is checked accounting for level boost.
 * If none of them is below the respective treshold of 35, 18 and 30 % of total, nothing happens.
 * Otherwise, all of them are reset to full amount accounting for permboosts and level boost.
 * @see Fighter
 * @see fighterClass
 * @see getBoost()
 * @param player The Fighter pointer to check the stats for.
 * @param force The integer to bypass all checks if true.
 */
void statReset(Fighter *player, int force)
{
    log_tag("debug_log.txt", "[DEBUG]",
            "Call to statReset() with ($force) == (%i)", force);
    if (!force && (player->hp >= 0.5 * player->totalhp)
        && !(player->atk <= 0 || player->def <= 0 || player->vel <= 0)) {
        return;
    }

    int boost = getBoost(player->level, player->luck);

    BaseStats *base = &basestats[player->class];
    if (force || player->vel <= 0.3 * (base->vel + boost)
        || player->atk <= 0.35 * (base->atk + boost)
        || player->def <= 0.18 * (base->def + boost)) {
        player->vel = base->vel + boost + player->permboost_vel;
        player->atk = base->atk + boost + player->permboost_atk;
        player->def = base->def + boost + player->permboost_def;
        //Reset stats
        if (!force) {
            //yellow();
            //printf("\n\n\t%s's stats reset.\n",player->name);
            //white();
        }
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

/**
 * Takes a Enemy pointer and two integers for current room index and current enemy index, and prints the spawn message formatted.
 * @see Enemy
 * @see stringFromEClass()
 * @param e The Enemy pointer to print.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 */
void printSpawnMessage(Enemy *e, int roomIndex, int enemyIndex)
{
    if (!e->beast) {
        //TODO:
        //Where to print a windowed spawn message?
        log_tag("debug_log.txt", "[ROOM]", "Room %i)\t\t%s\t\tEnemy #%i",
                roomIndex, stringFromEClass(e->class), enemyIndex + 1);
    } else {
        log_tag("debug_log.txt", "[ROOM]",
                "Room %i)\t\t%s\t\tEnemy #%i IS BEAST", roomIndex,
                stringFromEClass(e->class), enemyIndex + 1);
        //lightBlue();
        //printf("\nYou feel at discomfort.\n\nRoom %i)\t\t",roomIndex);
        //lightRed();
        //printf("Enemy #%i:\t%s",enemyIndex+1,stringFromEClass(e->class));
        //white();
    }
}

/**
 * Takes a Fighter pointer value and adds a random Consumable to consumablesBag.
 * @see Fighter
 * @see Consumable
 * @see consumableClass
 * @see CONSUMABLESMAX
 * @see stringFromConsumables()
 * @param player The Fighter pointer at hand.
 * @return int Returns the enum value of the drop as an integer.
 */
int dropConsumable(Fighter *player)
{
    int drop = rand() % (CONSUMABLESMAX + 1);

    //Special drop chances. Maybe a function for this?
    if (drop == Powergem) {
        if (rand() % 3 == 0) {
            drop = rand() % (CONSUMABLESMAX + 1);
        }
    }
    // Powergem has 33% chance to be rerolled

    Consumable *c = (Consumable *) player->consumablesBag[drop];
    c->qty++;

    //Update stats
    player->stats->consumablesfound++;

    return drop;
}

/**
 * Takes a Fighter pointer value and adds a random Artifact to artifactsBag.
 * The Artifact is randomised according to ARTIFACTSMAX until one which was not found yet drops.
 * Special Equip functions are also set up here.
 * @see Fighter
 * @see Artifact
 * @see artifactClass
 * @see stringFromArtifacts()
 * @param player The Fighter pointer at hand.
 * @return int Returns the enum value of the drop as an integer.
 */
int dropArtifact(Fighter *player)
{
    int drop = 0;
    do {
        drop = rand() % (ARTIFACTSMAX + 1);
    } while (player->artifactsBag[drop]->qty != 0);	//We reroll to get one we don't have

    player->artifactsBag[drop]->qty++;

    //Update stats
    player->stats->artifactsfound++;

    return drop;
}

/**
 * Takes a Fighter pointer and an integer denoting the consumableClass and returns the respective qty value from player's consumablesBag at the provided index.
 * @see Fighter
 * @see Consumable
 * @see consumableClass
 * @param f The Fighter pointer.
 * @param n The consumableClass value.
 * @return int The qty value in consumablesBag for selected consumableClass.
 */
int getConsumableQty(Fighter *f, int n)
{
    Consumable *c = (Consumable *) f->consumablesBag[n];
    return c->qty;
}

/**
 * Sets the qty value to 0 for all Consumable in f's consumablesBag with positive qty.
 * @see Consumable
 * @see consumableClass
 * @see CONSUMABLESMAX
 * @see getConsumableQty()
 * @param f The Fighter pointer at hand.
 */
void emptyConsumables(Fighter *f)
{
    for (int i = 0; i < CONSUMABLESMAX + 1; i++) {
        if (getConsumableQty(f, i) > 0) {
            Consumable *c = (Consumable *) f->consumablesBag[i];
            c->qty = 0;
        }
    };
}

/**
 * Takes a Fighter pointer and sets the qty value to 0 and the active flag to false for all Artifacts in the fighter's artifactsBag with positive qty.
 * @see Artifact
 * @see artifactClass
 * @see ARTIFACTSMAX
 * @param f The Fighter pointer at hand.
 */
void emptyArtifacts(Fighter *f)
{
    for (int i = 0; i < ARTIFACTSMAX + 1; i++) {
        if (f->artifactsBag[i]->qty > 0) {
            f->artifactsBag[i]->qty = 0;
            f->artifactsBag[i]->active = 0;
        }
    };
}

/**
 * Takes a Fighter pointer and, for all Equip in equipsBag field with positive qty, sets qty to 0 and frees the Equip pointer.
 * @see Fighter
 * @see Equip
 * @param player The fighter pointer whose equipsbag will be emptied.
 */
void emptyEquips(Fighter *player)
{
    //We REALLY need to be sure the items are in successive cells
    //Still thinking linked lists would be better than an array
    int occupied = player->equipsBagOccupiedSlots;

    for (int i = 0; i < occupied; i++) {

        Equip *e = (Equip *) player->equipsBag[i];
        if (e->qty > 0) {
            //free(e);
            e->qty = 0;
            log_tag("debug_log.txt", "[FIXME]",
                    "emptyEquips():  how do I empty them semantically?");
        }
    };
}

/**
 * Takes an integer seed and returns a Path pointer.
 * The seed provided is used to set the random seed and initialise the path values.
 * @see Path
 * @see MAXLENGTH
 * @see MAXLUCK
 * @param seed An integer seed.
 * @param kls The Koliseo used for allocation.
 * @param current_saveslot The Saveslot used to init the Path.
 * @return A Path pointer with stats.
 */
Path *randomise_path(int seed, Koliseo *kls, const char *path_to_savefile)
{
    char msg[200];
    sprintf(msg, "Prepping Path");
    kls_log(kls, "DEBUG", msg);
    Path *p = (Path *) KLS_PUSH_TYPED(kls, Path, HR_Path, "Path", msg);
    srand(seed);
    sprintf(msg, "Prepping Saveslot");
    kls_log(kls, "DEBUG", msg);
    sprintf(msg, "save_path: [%s]", path_to_savefile);
    Saveslot *save =
        (Saveslot *) KLS_PUSH_TYPED(kls, Saveslot, HR_Saveslot, "Saveslot",
                                    msg);
    sprintf(msg, "Seed: %i", seed);
    strcpy(save->name, msg);
    p->seed = seed;
    sprintf(msg, "%s", path_to_savefile);
    strcpy(save->save_path, msg);
    p->current_saveslot = save;
    kls_log(kls, "DEBUG",
            "Prepped Saveslot:  path->current_saveslot->save_path == [%s]",
            p->current_saveslot->save_path);
    log_tag("debug_log.txt", "[SAVESLOT]",
            "Prepped Saveslot:  path->current_saveslot->save_path == [%s]",
            p->current_saveslot->save_path);

    switch (GAMEMODE) {
    case Standard: {
        p->length = (rand() % MAXLENGTH) + 1;
        p->luck = (rand() % MAXLUCK) + 1;
        p->prize = 15 / p->luck * (rand() % 150) + 500;
    }
    break;
    case Story: {
        p->length = 41;
        p->luck = (rand() % MAXLUCK) + 1;
        p->prize = 15 / p->luck * (rand() % 150) + 500;
    }
    break;
    case Rogue: {
        p->length = 1;
        p->luck = (rand() % MAXLUCK) + 1;
        p->prize = 15 / p->luck * (rand() % 150) + 500;
    }
    break;
    default: {
        fprintf(stderr, "\nUnexpected GAMEMODE value %i.\n", GAMEMODE);
        exit(EXIT_FAILURE);
    }
    }
    return p;
}

/**
 * Takes a Fighter pointer and prints most of its values formatted.
 * @see Fighter
 * @see stringFromClass()
 * @see stringFromStatus()
 * @param f The Fighter pointer with stats to print.
 */
void printStats(Fighter *f)
{

    printf("\t%s's stats:\n\n", f->name);
    printf("\t\tClass:\t%s\n", stringFromClass(f->class));
    printf("\t\tHp:\t%i/%i\n", f->hp, f->totalhp);
    printf("\t\tEnergy:\t%i/%i\n", f->energy, f->totalenergy);
    printf("\t\tAtk:\t%i\n", f->atk);
    printf("\t\tDef:\t%i\n", f->def);
    printf("\t\tVel:\t%i\n", f->vel);
    printf("\t\tLvl:\t%i\n", f->level);
    printf("\t\tCurrent Level exp:\t%i/%i\n", f->currentlevelxp,
           f->totallevelxp);
    printf("\t\tTotal exp:\t%i\n", f->totalxp);
    printf("\t\tStatus:\t%s\n", stringFromStatus(f->status));
}

/**
 * Takes a Enemy pointer and prints most of its values formatted.
 * @see Enemy
 * @see stringFromEClass()
 * @see stringFromStatus()
 * @param e The Enemy pointer with stats to print.
 */
void printEStats(Enemy *e)
{
    if (e->beast) {
        lightRed();
    }
    printf("\t%s's stats:\n", stringFromEClass(e->class));
    if (e->beast) {
        white();
    }
    printf("\tHp:\t%i/%i\n", e->hp, e->totalhp);
    printf("\tEnergy:\t%i/%i\n", e->energy, e->totalenergy);
    printf("\tAtk:\t%i\n", e->atk);
    printf("\tDef:\t%i\n", e->def);
    printf("\tVel:\t%i\n", e->vel);
    printf("\tLvl:\t%i\n", e->level);
    printf("\tXp:\t%i\n", e->xp);
    printf("\tStatus:\t%s\n", stringFromStatus(e->status));
}

/**
 * Takes a Consumable pointer and prints most of its values formatted.
 * @see Consumable
 * @param c The Consumable pointer with stats to print.
 */
void printConsumablesStats(Consumable *c)
{
    printf("  (%i)\t%i\t%s\t\"%s\"\n", c->class, c->qty, c->name, c->desc);
}

/**
 * Takes a Artifact pointer and prints most of its values formatted.
 * @see Artifact
 * @param a The Artifact pointer with stats to print.
 */
void printArtifactStats(Artifact *a)
{
    printf("  (%i)\t\t", a->class);
    purple();
    printf("%s\t", a->name);
    yellow();
    printf("\"%s\"\n", a->desc);
    white();
}

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
 * @param is_localexe Denotes if current game was started from a relative path.
 */
void init_Gamestate(Gamestate *gmst, clock_t start_time, countStats *stats, Wincon *wincon,
                    Path *path, Fighter *player, Gamemode gamemode, GameScreen* screen, bool is_localexe)
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
    gmst->is_localexe = is_localexe;
}

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

/**
 * Takes a WINDOW pointer and prints the passed text to it ith wprintw(), before sleeping for the specified amount of milliseconds.
 * @see handleRoom_Enemies()
 * @see handleRoom_Boss()
 * @param w The WINDOW pointer to print to.
 * @param text The contents of the notification.
 * @param time The display time in milliseconds
 */
void display_notification(WINDOW *w, char *text, int time)
{
    wprintw(w, "\n  %s", text);
    wrefresh(w);
    //refresh();
    napms(time);
}

/**
 * Takes a WINDOW pointer and prints to it the passed string with the passed color.
 * Additional parameters set coordinates for the output.
 * @param win The WINDOW pointer to print to.
 * @param starty The integer indicating starting y coordinate.
 * @param startx The integer indicating starting x coordinate.
 * @param width The integer indicating panel width.
 * @param string The string to print to the window.
 * @param color The color to print in.
 */
void print_label(WINDOW *win, int starty, int startx, int width, char *string,
                 chtype color)
{
    int length, x, y;
    float temp;

    if (win == NULL) {
        log_tag("debug_log.txt", "[CURSES]",
                "win was NULL in boss_print_in_panel().");
        exit(EXIT_FAILURE);
    }
    getyx(win, y, x);
    if (startx != 0)
        x = startx;
    if (starty != 0)
        y = starty;
    if (width == 0)
        width = 80;

    length = strlen(string);
    temp = (width - length) / 2;
    x = startx + (int)temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}

/**
 * Takes a Equip pointer and prepares its sprite field by copying it line by line from equips_sprites, defined in sprites.h header.
 * @see Equip
 * @see dropEquip
 * @see equips_sprites
 * @param e The Equip pointer whose sprite field will be initialised.
 */
void setEquipSprite(Equip *e)
{
    if (e->class < EQUIPSMAX + 1) {
        for (int i = 0; i < 8; i++) {
            strcpy(e->sprite[i], equips_sprites[e->class][i]);
        }
    } else {
        log_tag("debug_log.txt", "[WARN]",
                "%s():    Unexpected equipClass {%i}.", __func__, e->class);
        return;
    }
}

/**
 * Takes a Consumable pointer and prepares its sprite field by copying it line by line from consumables_sprites, defined in sprites.h header.
 * @see Consumable
 * @see initPlayerStats
 * @see consumables_sprites
 * @param c The Consumable pointer whose sprite field will be initialised.
 */
void setConsumableSprite(Consumable *c)
{
    if (c->class < CONSUMABLESMAX + 1) {
        for (int i = 0; i < 8; i++) {
            strcpy(c->sprite[i], consumables_sprites[c->class][i]);
        }
    } else {
        fprintf(stderr,
                "[ERROR]    Unexpected consumableClass in setConsumableSprite().\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Takes a Artifact pointer and prepares its sprite field by copying it line by line from artifacts_sprites, defined in sprites.h header.
 * @see Artifact
 * @see gameloop()
 * @see artifacts_sprites
 * @param a The Artifact pointer whose sprite field will be initialised.
 */
void setArtifactSprite(Artifact *a)
{
    if (a->class < ARTIFACTSMAX + 1) {
        for (int i = 0; i < 8; i++) {
            strcpy(a->sprite[i], artifacts_sprites[a->class][i]);
        }
    } else {
        fprintf(stderr,
                "[ERROR]    Unexpected artifactClass in setArtifactSprite().\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Takes a quality value and calls the respective color function without actually printing text.
 * @see quality
 * @param q The quality value we want to set text color for.
 */
void printQualityColor(quality q)
{
    switch (q) {
    case Bad: {
        lightRed();
    }
    break;
    case Average: {
        strongWhite();
    }
    break;
    case Good: {
        lightCyan();
    }
    break;
    };
}

/**
 * Takes a Equip pointer and prints most of its values formatted.
 * The beginning of the format assumes the caller prepended "(x" on the output before calling, where x is the index of the equip.
 * @see Equip
 * @see stringFromQuality()
 * @param e The Equip pointer with stats to print.
 */
void printEquipStats(Equip *e)
{
    printf(")\t");		//This completes the print started in the calling loop, which supplies the index ATM

    //Quality color
    printQualityColor(e->qual);
    printf("%s  ", stringFromQuality(e->qual));

    printf("%s  ", e->name);
    white();

    printf("\"%s\"  (L%i)\t", e->desc, e->level);

    lightCyan();
    printf("%s  ", stringFromEquipzones(e->type));

    lightGreen();
    //Stats, will be then printed only if != 0
    if (e->atk != 0) {
        printf("A: %i ", e->atk);
    };
    if (e->def != 0) {
        printf("D: %i ", e->def);
    };
    if (e->vel != 0) {
        printf("V: %i ", e->vel);
    };
    if (e->enr != 0) {
        printf("E: %i", e->enr);
    };

    printf("\n");
    white();

    //Perks, will be then printed only if perksCount != 0

    for (int i = 0; i < e->perksCount; i++) {
        lightPurple();
        printf("\t\t%s\n", e->perks[i]->name);
    };
    white();
}

/**
 * Takes a Fighter pointer value and an integer indicating if the drop was from a beast enemy, and adds a random Equip to the fighter's equipsBag.
 * Prints notifications to the passed WINDOW pointer.
 * The Equip dropped is initalised here, including stat variations for quality and level boosts (stat increase from base level by adding player level over EQUIPLVLBOOSTRATIO.
 * The values of earliestBagSlot and equipsBagOccupiedSlots are managed.
 * If equipsBag is full (EQUIPSBAGSIZE), user has to choose one Equip not currently equipped to be deleted and replaced by the new one.
 * Special Equip functions are also set up here.
 * @see Fighter
 * @see Equip
 * @see equipClass
 * @see quality
 * @see EQUIPLVLBOOSTRATIO
 * @see EQUIPSBAGSIZE
 * @see stringFromQuality()
 * @see stringFromEquips()
 * @param player The Fighter pointer at hand.
 * @param beast The integer for drops coming from a beast kill if true.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 */
void dropEquip(Fighter *player, int beast, WINDOW *notify_win, Koliseo *kls)
{

    assert(player->equipsBagOccupiedSlots >= 0);

    if (player->equipsBagOccupiedSlots >= EQUIPSBAGSIZE) {
        // TODO: Handle full bag by asking player if we throw something away
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Player equip bag was full. Not dropping.", __func__);
        return;
    }

    //Select a basic item from the list
    int drop = rand() % (EQUIPSMAX + 1);
    //Randomise quality
    quality q = rand() % (QUALITIESMAX + 1);

    //Prepare the item
    kls_log(kls, "DEBUG", "Prepping dropped Equip");

    int equip_pos = player->equipsBagOccupiedSlots;
    Equip *e = player->equipsBag[equip_pos];
    //(Equip *) KLS_PUSH_TYPED(kls, Equip, HR_Equip, "Equip", "Equip");

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
    e->level = base->level + round(player->level / EQUIPLVLBOOSTRATIO);

    //Chance for better leveled item
    if ((rand() % 8) - (player->luck / 10) <= 0) {	//Should use a defined constant
        e->level += 1;		//At least a simple +1
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
        int chance = 20;

        if (q == Good) {
            chance *= 1.5;
        }

        if ((rand() % 100) < chance || (beast && e->perksCount == 0)) {

            e->perksCount += 1;

            log_tag("debug_log.txt", "[DEBUG]",
                    "Prepping Perk (%i) for dropped Equip)", e->perksCount);
            kls_log(kls, "DEBUG", "Prepping Perk (%i) for dropped Equip)",
                    e->perksCount);
            Perk *p = e->perks[e->perksCount-1];
            //(Perk *) KLS_PUSH_TYPED(kls, Perk, HR_Perk, "Perk", "Perk");
            p->class = rand() % (PERKSMAX + 1);
            //p->name = (char*)malloc(sizeof(nameStringFromPerk(p->class)));
            strcpy(p->name, nameStringFromPerk(p->class));
            //p->desc = (char*)malloc(sizeof(descStringFromPerk(p->class)));
            strcpy(p->desc, descStringFromPerk(p->class));
            p->innerValue = 1;
            // Set perk -- Unnecessary
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

    char msg[500];

    wattron(notify_win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
    sprintf(msg, "You found %s %s!", stringFromQuality(q),
            stringFromEquips(drop));
    display_notification(notify_win, msg, 800);
    wattroff(notify_win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
    log_tag("debug_log.txt", "[DEBUG-DROPS]", "Found Equip:    %s.",
            stringFromEquips(drop));

    /*
    if (player->equipsBagOccupiedSlots >= EQUIPSBAGSIZE) {	//Handle full bag by asking player if we throw something away
        //FIXME: The handling of full bag event is a mess as it does not support curses.
        lightRed();
        printf("\tYour bag is full. Want to throw something away?\n");
        white();
        log_tag("debug_log.txt", "[DEBUG-EQUIPS]",
                "Bag full, need to make space.\n");

        for (int i = 0; i < player->equipsBagOccupiedSlots; i++) {
            Equip *current = (Equip *) player->equipsBag[i];
            if (current->equipped) {
                green();
                printf("ON  ");
                white();
            };

            printf("(%i", i);	//This starts lines with the item index.
            printEquipStats(current);
        };

        int selected = -1;
        int c = -1;
        Equip *s = NULL;

        while (selected < 0 || selected >= player->equipsBagOccupiedSlots
               || c != 1) {

            c = scanf("%i", &selected);
            int res = scanf("%*c");
            log_tag("debug_log.txt", "[DEBUG]",
                    "dropEquip() scanf() res was (%i)", res);

            if (c == 1) {
                s = (Equip *) player->equipsBag[selected];
                if (s->equipped) {
                    printf("You can't delete an equipped item!");
                    selected = -1;
                }
            }
        }

        printf
        ("\tAre you sure you want to delete %s %s ?\n\t\t[0 to confirm, 1 to go back]\n",
         stringFromQuality(s->qual), s->name);

        int n = -1;
        c = -1;
        while (n < 0 || c != 1) {
            c = scanf("%i", &n);
            int res = scanf("%*c");
            log_tag("debug_log.txt", "[DEBUG]",
                    "dropEquip() scanf() res was (%i)", res);
        }

        if (n != 0) {		//Abort deletion, drop will not be awared.

               //int perkscount = e->perksCount;
               //if (perkscount > 0) {
               //for (int i=0; i < perkscount; i++) {
               //free(e->perks[i]);
               //}
               //}
               //free(e);
            log_tag("debug_log.txt", "[DEBUG-EQUIPS]",
                    "Equip was not taken.\n");
            return;
        };

        Equip *toDelete = (Equip *) player->equipsBag[selected];
        int perkscount = toDelete->perksCount;
           //if (perkscount > 0) {
           //for (int i=0; i < perkscount; i++) {
           //free(toDelete->perks[i]);
           //}
           //}
        log_tag("debug_log.txt", "[DEBUG-EQUIPS]",
                "Equip %s (%i Perks) was taken by deleting %s.\n",
                stringFromEquips(e->class), perkscount,
                stringFromEquips(toDelete->class));
           //free(toDelete);

        //Add drop to player bag replacing the one at the selected index
        player->equipsBag[selected] = (struct Equip *)e;

        //Update stats
        player->stats->equipsfound++;

        napms(500);
        return;
    };				//End if bag is full
    */

    //Add drop to player bag -- Unnecessary
    player->equipsBag[player->earliestBagSlot] = (struct Equip *)e;

    player->earliestBagSlot += 1;	//Advance equips bage pointer
    player->equipsBagOccupiedSlots += 1;	//Advance equips bage size counter

    //Update stats
    player->stats->equipsfound++;
}

/**
 * Takes a Fighter pointer and asks the user to select a specialMove to unlock with a formatted text menu.
 * From the specials field of fighter, only the SpecialSlot with the enabled falg not set are printed and selectable by user.
 * For the Fighter, the equipboost values are also displayed.
 * @see Fighter
 * @see SpecialSlot
 * @see SPECIALSMAX
 * @see setSpecials()
 * @see stringFromSpecial()
 * @param f The Fighter pointer that get one of his not yet unlocked specials.
 */
void unlockSpecial(Fighter *f)
{

    //Thanks to u/skeeto for the suggestions.
    ITEM *my_items[SPECIALSMAX + 2] = { 0 };
    MENU *my_menu;
    WINDOW *my_menu_win;
    WINDOW *display_win;

    int n_choices = 0;
    int selection = -1;
    int currentIndexed = -1;
    ITEM *cur;

    /* Initialize curses */
    //initscr();
    clear();
    refresh();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    /* Create menu items */
    for (int i = 0; i < SPECIALSMAX + 1; i++) {
        if (!(f->specials[i]->enabled)) {
            my_items[n_choices++] =
                new_item(nameStringFromSpecial(f->class, i), "  ");
        }
    }

    /* Create menu */
    my_menu = new_menu(my_items);

    /* Set description off */
    menu_opts_off(my_menu, O_SHOWDESC);

    /* Create the window to be associated with the menu */
    my_menu_win = newwin(18, 28, 2, 2);
    keypad(my_menu_win, TRUE);

    /* Set main window and sub window */
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, 12, 26, 4, 2));
    set_menu_format(my_menu, 12, 1);

    /* Set menu mark to the string " > " */
    set_menu_mark(my_menu, " > ");

    /* Print a border around the main window and print a title */
    box(my_menu_win, 0, 0);
    print_label(my_menu_win, 1, 0, 20, "New move unlocked",
                COLOR_PAIR(S4C_CYAN));
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwhline(my_menu_win, 2, 1, ACS_HLINE, 26);
    mvwaddch(my_menu_win, 2, 27, ACS_RTEE);

    /* Post the menu */
    post_menu(my_menu);
    wrefresh(my_menu_win);

    //attron(COLOR_PAIR(2));
    //mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scoll down or up a page of items");
    mvprintw(23, 0, "Select a new special move to learn.");
    //attroff(COLOR_PAIR(2));
    //refresh();

    refresh();

    /* Prepare selection display window */
    display_win = newwin(18, 40, 3, 32);
    box(display_win, 0, 0);
    //Update selected window
    cur = current_item(my_menu);
    for (int i = 0; i < SPECIALSMAX + 1; i++) {
        int check = -1;
        if ((check =
                 strcmp(nameStringFromSpecial(f->class, i), item_name(cur))) == 0) {
            currentIndexed = i;
            break;
        }
    }
    mvwprintw(display_win, 2, 2, "%s",
              descStringFromSpecial(f->class,
                                    f->specials[currentIndexed]->move));
    mvwprintw(display_win, 4, 2, "Energy Cost %i",
              f->specials[currentIndexed]->cost);
    wrefresh(my_menu_win);
    wrefresh(display_win);

    int picked = 0;
    int c;

    while (!picked && (c = wgetch(my_menu_win))) {
        switch (c) {
        case KEY_DOWN: {
            menu_driver(my_menu, REQ_DOWN_ITEM);
            cur = current_item(my_menu);
            //Update selected window
            for (int i = 0; i < SPECIALSMAX + 1; i++) {
                int check = -1;
                if ((check =
                         strcmp(nameStringFromSpecial(f->class, i),
                                item_name(cur))) == 0) {
                    currentIndexed = i;
                    break;
                }
            }
        }
        break;
        case KEY_UP: {
            menu_driver(my_menu, REQ_UP_ITEM);
            cur = current_item(my_menu);
            for (int i = 0; i < SPECIALSMAX + 1; i++) {
                int check = -1;
                if ((check =
                         strcmp(nameStringFromSpecial(f->class, i),
                                item_name(cur))) == 0) {
                    currentIndexed = i;
                    break;
                }
            }
        }
        break;
        case KEY_NPAGE: {
            menu_driver(my_menu, REQ_SCR_DPAGE);
            cur = current_item(my_menu);
            //Update selected window
            for (int i = 0; i < SPECIALSMAX + 1; i++) {
                int check = -1;
                if ((check =
                         strcmp(nameStringFromSpecial(f->class, i),
                                item_name(cur))) == 0) {
                    currentIndexed = i;
                    break;
                }
            }
        }
        break;
        case KEY_PPAGE: {
            menu_driver(my_menu, REQ_SCR_UPAGE);
            cur = current_item(my_menu);
            for (int i = 0; i < SPECIALSMAX + 1; i++) {
                int check = -1;
                if ((check =
                         strcmp(nameStringFromSpecial(f->class, i),
                                item_name(cur))) == 0) {
                    currentIndexed = i;
                    break;
                }
            }
        }
        break;
        case 10: {		/*Enter */
            picked = 1;
            cur = current_item(my_menu);
            for (int i = 0; i < SPECIALSMAX + 1; i++) {
                int check = -1;
                if ((check =
                         strcmp(nameStringFromSpecial(f->class, i),
                                item_name(cur))) == 0) {
                    selection = i;
                    break;
                }

                pos_menu_cursor(my_menu);
                refresh();
            };
            break;
        }
        }
        wclear(display_win);
        wrefresh(display_win);
        box(display_win, 0, 0);
        mvwprintw(display_win, 2, 2, "%s",
                  descStringFromSpecial(f->class,
                                        f->specials[currentIndexed]->move));
        mvwprintw(display_win, 4, 2, "Energy Cost %i",
                  f->specials[currentIndexed]->cost);
        wrefresh(my_menu_win);
        wrefresh(display_win);
        refresh();
    }
    /* Unpost and free all the memory taken up */
    unpost_menu(my_menu);
    free_menu(my_menu);
    for (int k = 0; k <= n_choices; k++) {
        free_item(my_items[k]);
    }
    delwin(my_menu_win);
    delwin(display_win);
    endwin();

    int num = selection;

    if (num < SPECIALSMAX + 1) {	//Check if selected move number is lower than the maximum
        Specialslot *selected = f->specials[num];

        //Check if the selected move is NOT enabled
        if (!(selected->enabled)) {
            //Enable the move
            selected->enabled = 1;
        }
    }
    f->stats->specialsunlocked += 1;
}

//TODO update this to actually handle different types of counters and reliably print them, maybe to a ncurses window
/**
 * Takes a Turncounter array.
 * For every Turncounter in the array, the values of count, innerVal, type and all the function pointers fields are printed.
 * @see Turncounter
 * @param counters The Turncounter array to be printed.
 */
void printCounters(Turncounter *counters[])
{
    yellow();
    printf
    ("%-10.10s\t%-10.10s\t%-3.3s\t%-3.3s\t%-11.11s\t%-11.11s\t%-11.11s\t%-11.11s\n",
     "Count", "Desc", "Val", "Typ", "*(eff())", "*(eff_e())", "*(boost())",
     "*(boost_e())");
    for (int i = 0; i < (COUNTERSMAX + 1); i++) {
        Turncounter *c = counters[i];
        lightBlue();
        printf("%-10.10i\t%-10.10s\t", c->count, c->desc);
        strongWhite();
        printf("(%-3.3i)\t(%-3.3i)\t", c->innerValue, c->type);
        purple();
        //printf("[%-11.11i]\t[%-11.11i]\t",*(c->effect_fun),*(c->effect_e_fun));
        cyan();
        //printf("[%-11.11i]\t[%-11.11i]\n",*(c->boost_fun), *(c->boost_e_fun));
    };
    white();
}

/**
 * Takes a Figher pointer and prints all of its active perks formatted.
 * @see Fighter
 * @see Perk
 * @see Equip
 * @param f The Fighter pointer with perks to print.
 */
void printActivePerks(Fighter *f)
{

    WINDOW *win;

    /* Initialize curses */
    //initscr();
    clear();
    refresh();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    /* Create the windows for player stats and lifetime counters */
    win = newwin(22, 65, 1, 2);
    keypad(win, TRUE);

    /* Print a border around the windows and print a title */
    box(win, 0, 0);
    print_label(win, 1, 0, 35, "Perks", COLOR_PAIR(S4C_BRIGHT_YELLOW));
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, 63);
    mvwaddch(win, 2, 64, ACS_RTEE);

    wrefresh(win);

    //attron(COLOR_PAIR(3));
    mvprintw(23, 0, "(Press q or Enter to Exit)");
    //attroff(COLOR_PAIR(3));

    int y = 4;
    int x = 2;
    //int startx = 2;
    //int width = 65;

    int empty = 1;

    int count = PERKSMAX + 1;

    for (int i = 0; i < count; i++) {
        Perk *p = f->perks[i];
        if (p->innerValue > 0) {
            empty = 0;

            wattron(win, COLOR_PAIR(S4C_CYAN));
            mvwprintw(win, y, x, " x%i %s ", p->innerValue,
                      nameStringFromPerk(p->class));
            wattroff(win, COLOR_PAIR(S4C_CYAN));
            char s[250];
            sprintf(s, " x%i %s", p->innerValue, nameStringFromPerk(p->class));
            int l = strlen(s);
            wattron(win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
            mvwprintw(win, y, x + l + 2, "\"%s\"",
                      descStringFromPerk(p->class));
            wattroff(win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
            y++;
        }
    };

    if (empty) {		//No perks are active
        wattron(win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
        mvwprintw(win, y, x, "You don't have any special power yet.");
        wattroff(win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
    }

    refresh();

    int picked = 0;
    int c = -1;
    wrefresh(win);

    while (!picked && (c = wgetch(win)) != 'q') {
        switch (c) {
        case 10: {		/*Enter */
            picked = 1;

        };
        break;
        }
    }
    delwin(win);
    endwin();
}

/**
 * Takes a WINDOW pointer to print notifications to, a Fighter pointer value and applies the effect pertaining to its status value.
 * @see Fighter
 * @see fighterStatus
 * @see printStatusText()
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param f The Fighter pointer at hand.
 */
void applyStatus(WINDOW *notify_win, Fighter *f)
{

    switch (f->status) {
    case Normal: {
        break;
    }
    break;
    case Poison: {

        //Account for penicillin perk
        //ATM multiples don't stack
        int penicillin = f->perks[PENICILLIN]->innerValue;
        if (penicillin > 0) {
            return;
        }

        if (f->hp >= 4) {
            f->hp -= 3;
        } else {
            f->hp = 1;	//Will this be a problem?
        }
        printStatusText(notify_win, Poison, f->name);
    }
    break;
    case Burned: {
        printStatusText(notify_win, Burned, f->name);
    }
    break;
    case Frozen: {
        printStatusText(notify_win, Frozen, f->name);
    }
    break;
    case Weak:

        break;
    case Strong:

        break;
    }
}

/**
 * Takes a WINDOW pointer to print notifications to, a Enemy pointer value and applies the effect pertaining to its status value.
 * @see Enemy
 * @see fighterStatus
 * @see printStatusText()
 * @see stringFromEClass()
 * @param notify_win The window pointer to call display_notification() on.
 * @param e The Enemy pointer at hand.
 * @see display_notification()
 */
void applyEStatus(WINDOW *notify_win, Enemy *e)
{

    wattron(notify_win, COLOR_PAIR(S4C_BRIGHT_GREEN));

    switch (e->status) {
    case Normal: {
        break;
    }
    break;
    case Poison: {
        if (e->hp >= 4) {
            e->hp -= 3;
        } else {
            e->hp = 1;	//Will this be a problem for kills in the enemy loop?
        }
        printStatusText(notify_win, Poison, stringFromEClass(e->class));
    }
    break;
    case Burned: {
        if (e->hp >= 5) {
            e->hp -= 4;
        } else {
            e->hp = 1;	//Will this be a problem for kills in the enemy loop?
        }

        if (e->atk >= 3) {
            e->atk -= 3;
        } else {
            e->atk = 1;
        }
        printStatusText(notify_win, Burned, stringFromEClass(e->class));
    }
    break;
    case Frozen: {
        if (e->vel >= 3) {
            e->vel -= 1;
        } else {
            e->vel = 1;	//Will this be a problem for kills in the enemy loop?
        }
        printStatusText(notify_win, Frozen, stringFromEClass(e->class));
    }

    break;
    case Weak:

        break;
    case Strong:

        break;
    }

    wattroff(notify_win, COLOR_PAIR(S4C_BRIGHT_GREEN));
}

/**
 * Takes a WINDOW pointer to print notifications to, a Boss pointer value and applies the effect pertaining to its status value.
 * @see Boss
 * @see fighterStatus
 * @see printStatusText()
 * @see stringFromBossClass()
 * @param notify_win The window pointer to call disaply_notification() on.
 * @param b The Boss pointer at hand.
 */
void applyBStatus(WINDOW *notify_win, Boss *b)
{

    wattron(notify_win, COLOR_PAIR(S4C_BRIGHT_GREEN));

    switch (b->status) {
    case Normal: {
        break;
    }
    break;
    case Poison: {
        if (b->hp >= 4) {
            b->hp -= 3;
        } else {
            b->hp = 1;	//Will this be a problem for kills in the enemy loop?
        }
        printStatusText(notify_win, Poison, stringFromBossClass(b->class));
    }
    break;
    case Burned: {
        if (b->hp >= 5) {
            b->hp -= 4;
        } else {
            b->hp = 1;	//Will this be a problem for kills in the enemy loop?
        }

        if (b->atk >= 3) {
            b->atk -= 3;
        } else {
            b->atk = 1;
        }
        printStatusText(notify_win, Burned, stringFromBossClass(b->class));
    }
    break;
    case Frozen: {
        if (b->vel >= 3) {
            b->vel -= 1;
        } else {
            b->vel = 1;	//Will this be a problem for kills in the enemy loop?
        }
        printStatusText(notify_win, Frozen, stringFromBossClass(b->class));
    }

    break;
    case Weak:

        break;
    case Strong:

        break;
    }

    wattroff(notify_win, COLOR_PAIR(S4C_BRIGHT_GREEN));
}

/**
 * Takes a WINDOW pointer to print notifications to, a fighterStatus value and a string of who's the entity to print the respective status message.
 * @see fighterStatus
 * @param notify_win The pointer to the window to use display_notification() on.
 * @param status The fighterStatus at hand.
 * @param subject A string with name of entity owning the fighterStatus.
 */
void printStatusText(WINDOW *notify_win, fighterStatus status, char *subject)
{
    char msg[500];
    switch (status) {
    case Normal: {
        return;
    };
    break;
    case Poison:
    case Burned: {
        sprintf(msg, "%s is hurt by its %s.", subject,
                stringFromStatus(status));
        display_notification(notify_win, msg, 500);
    }
    break;
    case Weak:
    case Strong: {
        sprintf(msg, "%s is feeling %s.", subject,
                stringFromStatus(status));
        display_notification(notify_win, msg, 500);
    }
    break;
    case Frozen: {
        sprintf(msg, "%s is frozen cold.", subject);
        display_notification(notify_win, msg, 500);
    }
    break;
    }
}

/**
 * Asks the user is they want to continue and returns the choice.
 * @return int True for trying again, false otherwise.
 */
int retry(int seed)
{
    lightGreen();
    printf("\n\nYou died. Want to try again?\n\nSeed: [%i]\n\n\n\t\t[type no / yes]\n\n", seed);
    white();
    char c[25] = { 0 };
    if (fgets(c, sizeof(c), stdin) != NULL) {
        log_tag("debug_log.txt", "[DEBUG]", "Read input for %s().", __func__);
        if (c[strlen(c) - 1] == '\n') {
            c[strlen(c) - 1] = '\0';
        }

        for (char *ptr = c; *ptr; ++ptr) {
            *ptr = tolower(*ptr);
        }

        if ( (c[0] == '\0') || (strcmp(c, "no") == 0) || (strcmp(c, "n") == 0) ) {
            log_tag("debug_log.txt", "[DEBUG]", "%s(): input was no.",
                    __func__);
            return 0;
        } else if ((strcmp(c, "yes") == 0) || (strcmp(c, "y") == 0) ) {
            log_tag("debug_log.txt", "[DEBUG]", "%s(): input was yes.",
                    __func__);
            return 1;
        } else {
            log_tag("debug_log.txt", "[DEBUG]",
                    "%s(): Invalid input, defaulting to 0.", __func__);

            return 0;
        }
    } else {
        log_tag("debug_log.txt", "[DEBUG]", "Failed reading input for %s.",
                __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
}

/**
 * Takes a integer and a string array (possibly from main), a Fighter pointer and a Path pointer to set some values.
 * Depending on argc value, the arguments in argv may be used instead of calling the functions to get user input.
 * @see Fighter
 * @see pickName()
 * @see pickClass()
 * @see pickWincon()
 * @param argc The number of argv values + 1 (0 is program name?).
 * @param argv Array of strings with argc - 1 values.
 * @param player The Fighter of which we set name and class.
 * @param path The Path pointer used for the game.
 * @param kls The Koliseo used for allocation.
 */
void getParams(int argc, char **argv, Fighter *player, Path *path, int optTot,
               Koliseo *kls)
{

    int argTot = argc - optTot;
    if (argTot == 0) {
        pickName(player);
        pickClass(player);
        kls_log(kls, "DEBUG", "Prepping Wincon");
        Wincon *w =
            (Wincon *) KLS_PUSH_TYPED(kls, Wincon, HR_Wincon, "Wincon",
                                      "Wincon");
        if (GAMEMODE == Story) {
            //Path length must be already initialised before getting here.
            initWincon(w, path, FULL_PATH);
        } else if (GAMEMODE == Rogue) {
            //Path length must be already initialised before getting here.
            initWincon(w, path, ALL_ARTIFACTS);
        } else {
            pickWincon(w);
            initWincon(w, path, w->class);
        }
        path->win_condition = w;
    } else if (argTot == 1 || argTot == 2) {
        char read_name[25];
        int i = 0, len = 0;
        len = strlen(argv[optTot]);
        if (len < 20) {
            for (; i < len; i++) {
                read_name[i] = argv[optTot][i];
            }
            read_name[i] = '\0';
            strcpy(player->name, read_name);
        } else {
            pickName(player);
        }
    } else {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argTot == 1) {
        pickClass(player);
        kls_log(kls, "DEBUG", "Prepping Wincon");
        Wincon *w =
            (Wincon *) KLS_PUSH_TYPED(kls, Wincon, HR_Wincon, "Wincon",
                                      "Wincon");
        if (GAMEMODE == Story) {
            //Path length must be already initialised before getting here.
            initWincon(w, path, FULL_PATH);
        } else if (GAMEMODE == Rogue) {
            //Path length must be already initialised before getting here.
            initWincon(w, path, ALL_ARTIFACTS);
        } else {
            pickWincon(w);
            initWincon(w, path, w->class);
        }
        path->win_condition = w;
    }

    if (argTot == 2) {
        int c = -1;
        int i = 0;
        for (i = 0; i <= CLASSESMAX; i++) {
            if (strcmp(argv[optTot + 1], classesstrings[i]) == 0) {
                c = 1;
                player->class = i;
                break;
            }
        }
        if (c < 0) {
            pickClass(player);
        }
        kls_log(kls, "DEBUG", "Prepping Wincon");
        Wincon *w =
            (Wincon *) KLS_PUSH_TYPED(kls, Wincon, HR_Wincon, "Wincon",
                                      "Wincon");
        if (GAMEMODE == Story) {
            //Path length must be already initialised before getting here.
            initWincon(w, path, FULL_PATH);
        } else if (GAMEMODE == Rogue) {
            //TODO: what do we set as path length? Number of floors?
            //Path length must be already initialised before getting here.
            initWincon(w, path, ALL_ARTIFACTS);
        } else {
            pickWincon(w);
            initWincon(w, path, w->class);
        }
        path->win_condition = w;
    }
}
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else

void test_game_color_pairs(Rectangle * win, int colors_per_row)
{
    (void) win;
    (void) colors_per_row;
    printf("%s():    TODO - Implement game color pairs test for rl-build\n", __func__);
    return;
}

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

turnOP_args *init_turnOP_args(Gamestate *gmst, Fighter *actor, Path *path,
                              Room *room, loadInfo *load_info, Enemy *enemy,
                              Boss *boss, FILE *save_file, Rectangle *notification_area,
                              Koliseo_Temp *t_kls, foeTurnOption_OP foe_op,
                              skillType picked_skill)
{
    printf("%s():    TODO - implement turnOP init for rl-build\n", __func__);
    return NULL;
}

/**
 * Takes a Fighter pointer and asks the user to select a specialMove to unlock with a formatted text menu.
 * From the specials field of fighter, only the SpecialSlot with the enabled falg not set are printed and selectable by user.
 * For the Fighter, the equipboost values are also displayed.
 * @see Fighter
 * @see SpecialSlot
 * @see SPECIALSMAX
 * @see setSpecials()
 * @see stringFromSpecial()
 * @param f The Fighter pointer that get one of his not yet unlocked specials.
 */
void unlockSpecial(Fighter *f)
{
    (void) f;
}

#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

/**
 * Takes a Fighter and loadInfo pointers and prints fighter stats and a game over message.
 * Consumables and Artifacts are emptied before freeing the player's specials, counters, perks and stats field.
 * At last, the player pointer is freed.
 * @see Fighter
 * @see handleStats()
 * @see emptyConsumables()
 * @see emptyArtifacts()
 * @param player The Fighter pointer to free.
 * @param load_info The loadInfo pointer to free.
 */
void death(Fighter *player, loadInfo *load_info)
{

    //FIXME:
    //dropping out of the Koliseo scope might render stat pointer invalid.
    //handleStats(player);

    //Free default kls
    kls_free(default_kls);
    //kls_log(kls,"DEBUG","Freed default KLS");
    log_tag("debug_log.txt", "[DEBUG-KLS]", "Freed default KLS");

    //Free temporary kls
    kls_free(temporary_kls);
    //kls_log(kls,"DEBUG","Freed temporary KLS");
    log_tag("debug_log.txt", "[DEBUG-KLS]", "Freed temporary KLS");

    /*
       free(load_info);
       sprintf(msg,"Freed loadInfo.\n");
       log_tag("debug_log.txt","[FREE]",msg);
     */

    //emptyConsumables(player);
    //emptyArtifacts(player);
    /*
       //Free player special slots
       for (int i=0; i < (SPECIALSMAX + 1) ; i++) {
       free(player->specials[i]);
       sprintf(msg,"Freed player special %i.",i);
       log_tag("debug_log.txt","[FREE]",msg);
       }
       log_tag("debug_log.txt","[FREE]","Done.\n");
     */

    /*
       //Free player equipbag
       int total = player->equipsBagOccupiedSlots;
       for (int i=0; i < (total ) ; i++) {
       Equip* e = (Equip*) player->equipsBag[i];
       int perkscount = e->perksCount;
       if (perkscount > 0) {
       for (int j=0; j < perkscount; j++) {
       free(e->perks[j]);
       sprintf(msg,"Freed equip %i perk %i.", i, j);
       log_tag("debug_log.txt","[FREE]",msg);
       }
       }
       free(e);
       sprintf(msg,"Freed equip %i.\n", i);
       log_tag("debug_log.txt","[FREE]",msg);
       }
       log_tag("debug_log.txt","[FREE]","Done.\n");
     */

    /*
       //Free player consumablebag
       int cons_total = CONSUMABLESMAX+1;
       for (int i=0; i < cons_total ; i++) {
       Consumable* c = (Consumable*) player->consumablesBag[i];
       sprintf(msg,"Freed consumable %i.", i);
       log_tag("debug_log.txt","[FREE]",msg);
       free(c);
       }
       log_tag("debug_log.txt","[FREE]","Done.\n");
     */

    /*
       //Free player equip slots
       for (int i=0; i < (EQUIPZONES + 1) ; i++) {
       Equipslot* s = (Equipslot*) player->equipslots[i];

       int perkscount = -1;

       if (s->active) { perkscount = s->item->perksCount;};
       if (perkscount > 0) {
       for (int i=0; i < perkscount; i++) {
       free(s->item->perks[i]);
       }
       free(s->item);
       }

       free(s);
       sprintf(msg,"Freed equipslot %i.", i);
       log_tag("debug_log.txt","[FREE]",msg);
       }
       log_tag("debug_log.txt","[FREE]","Done.\n");
     */

    /*
       //Free player artifactsbag
       int art_total = CONSUMABLESMAX+1;
       for (int i=0; i < art_total ; i++) {
       Artifact* a = (Artifact*) player->artifactsBag[i];
       free(a);
       sprintf(msg,"Freed artifact %i.", i);
       log_tag("debug_log.txt","[FREE]",msg);
       }
       log_tag("debug_log.txt","[FREE]","Done.\n");
     */

    /*
       //Free player turnCounters
       for (int i=0; i < (COUNTERSMAX + 1) ; i++) {
       Turncounter* c = (Turncounter*) player->counters[i];
       free(c->desc);
       sprintf(msg,"Freed turncounter %i desc.", i);
       log_tag("debug_log.txt","[FREE]",msg);
       free(c);
       sprintf(msg,"Freed turncounter %i.\n", i);
       log_tag("debug_log.txt","[FREE]",msg);
       }
       log_tag("debug_log.txt","[FREE]","Done.\n");
     */

    /*
       //Free player perks
       for (int i=0; i < (PERKSMAX + 1) ; i++) {
       free(player->perks[i]);
       sprintf(msg,"Freed player perk %i.", i);
       log_tag("debug_log.txt","[FREE]",msg);
       }
       log_tag("debug_log.txt","[FREE]","Done.\n");
     */

    //free(player->stats);
    //log_tag("debug_log.txt","[FREE]","Freed player stats.\n");
}

/**
 * Takes a Enemy pointer and frees its allocated memory.
 * The counters field is freed before the enemy pointer.
 * @see Enemy
 * @param e The Enemy pointer to free.
 */
void e_death(Enemy *e)
{

    //Free enemy special slots
    //for (int i=0; i < SPECIALSMAX + 1 ; i++) {
    //      free(player->specials[i]);
    //}

    /*
       //Free enemy turnCounters
       for (int i=0; i < (COUNTERSMAX + 1) ; i++) {
       Turncounter* c = (Turncounter*) e->counters[i];
       sprintf(msg,"Freed enemy turncounter %i desc:    %s.",i, c->desc);
       log_tag("debug_log.txt","[FREE]",msg);
       if (c->desc == NULL) {
       log_tag("debug_log.txt","[ERROR]", "Enemy turncounter desc was null.\n");
       } else {
       char* desc_to_free = c->desc;
       free(desc_to_free);
       }
       free(c);
       sprintf(msg,"Freed enemy turncounter %i.\n",i);
       log_tag("debug_log.txt","[FREE]",msg);
       }
     */

    //sprintf(msg,"Freeing enemy %s",stringFromEClass(e->class));
    //log_tag("debug_log.txt","[FREE]",msg);
    //free(e);
    log_tag("debug_log.txt", "[TODO]", "[%s]: remove this empty function.",
            __func__);
}

/**
 * Takes a Boss pointer and frees its allocated memory.
 * The counters field is freed before the boss pointer.
 * @see Boss
 * @param b The Boss pointer to free.
 */
void b_death(Boss *b)
{

    //TODO
    //Remove this bs
    log_tag("debug_log.txt", "[DEBUG]", "b_death():  I'm doing nothing.");

    //Free boss special slots
    //for (int i=0; i < SPECIALSMAX + 1 ; i++) {
    //      free(player->specials[i]);
    //}
    //Free boss turnCounters
    /*
       for (int i=0; i < (COUNTERSMAX + 1) ; i++) {
       Turncounter* c = (Turncounter*) b->counters[i];
       sprintf(msg,"Freed boss turncounter %i desc:    %s.",i, c->desc);
       log_tag("debug_log.txt","[FREE]",msg);
       free(c->desc);
       free(c);
       sprintf(msg,"Freed boss turncounter %i.\n",i);
       log_tag("debug_log.txt","[FREE]",msg);
       }
     */
    //free(b);
}

/**
 * Takes a Turncounter array, an integer, a Fighter pointer and an Enemy pointer.
 * For every Turncounter in the array count value is checked, and when it's equal to 1, the function pointer relevant to the type value of the Counter is called. Depending on the isEnemy input value, the function call will be for the Fighter or Enemy version (and the according pointer from the input will be passed to the called function).
 * When the count value is 0, counters are considered inactive, so when the count value is not 1, it is lowered by 1 if it's positive.
 * @see Fighter
 * @see Enemy
 * @see Turncounter
 * @see Countertype
 * @see COUNTERSMAX
 * @see counterIndexes
 * @param counters The Turncounter array to be updated.
 * @param isEnemy Dictates if the check is done for an Enemy or a Fighter.
 * @param f The Fighter pointer whose counters field will be updated if isEnemy is false.
 * @param e The Enemy pointer whose counters field will be updated if isEnemy is true.
 */
void updateCounters(Turncounter *counters[], int isEnemy, Fighter *f, Enemy *e)
{
    for (int i = 0; i < COUNTERSMAX + 1; i++) {

        Turncounter *c = counters[i];
        if (c->count == 1) {	//Counter is about to expire so we call the correct function:
            switch (c->type) {
            case CNT_STATUS: {	//Callback for status counters

                if (!isEnemy) {
                    (c->effect_fun) (f);
                    //green();
                    //printf("\t%s status returned to normal.\n",f->name);
                    //white();
                } else {	//Enemy function
                    (c->effect_e_fun) (e);
                    //lightRed();
                    //printf("\t%s status returned to normal.\n",stringFromEClass(e->class));
                    //white();
                    //TODO
                    //Display notification to win
                    log_tag("debug_log.txt", "[DEBUG-COUNTER]",
                            "Status reset for %s.",
                            stringFromEClass(e->class));
                }
            }
            break;
            case CNT_ATKBOOST:
            case CNT_DEFBOOST:
            case CNT_VELBOOST:
            case CNT_ENRBOOST: { //Callback for stat boosts
                if (!isEnemy) {
                    (c->boost_fun) (f, 0);	//Call ~setPermBoost(STAT)~ with value 0
                    log_tag("debug_log.txt", "[DEBUG-COUNTER]",
                            "Applied boost function for Fighter.");
                } else {
                    assert(0 && "UNACCESSIBLE CALL.\n");
                    (c->boost_e_fun) (e, 0);	//Call ~setPermBoost(STAT)~ with value 0
                }
            }
            break;
            }

            //Set counter to zero
            c->count = 0;
            c->innerValue = 0;
        } else if (c->count > 1) {	//We simply advance the count
            log_tag("debug_log.txt", "[DEBUG-COUNTER]",
                    "Advancing counter %i for %s.", i,
                    (isEnemy ? "Enemy" : "Fighter"));
            c->count -= 1;
        }
    };
}

/**
 * Takes a Turncounter array, an integer, a Fighter pointer and a Boss pointer.
 * For every Turncounter in the array count value is checked, and when it's equal to 1, the function pointer relevant to the type value of the Counter is called. Depending on the isBoss input value, the function call will be for the Fighter or Boss version (and the according pointer from the input will be passed to the called function).
 * When the count value is 0 Counters are considered inactive, so when the count value is not 1, it is lowered by 1 if it's positive.
 * @see Fighter
 * @see Boss
 * @see Turncounter
 * @see Countertype
 * @see COUNTERSMAX
 * @see counterIndexes
 * @param counters The Turncounter array to be updated.
 * @param isBoss Dictates if the check is done for a Boss or a Fighter.
 * @param f The Fighter pointer whose counters field will be updated if isBoss is false.
 * @param b The Boss pointer whose counters field will be updated if isBoss is true.
 */
void updateCounters_Boss(Turncounter *counters[], int isBoss, Fighter *f,
                         Boss *b)
{
    for (int i = 0; i < COUNTERSMAX + 1; i++) {

        Turncounter *c = counters[i];
        if (c->count == 1) {	//Counter is about to expire so we call the correct function:
            switch (c->type) {
            case CNT_STATUS: {	//Callback for status counters

                //TODO
                //Add notification to window
                if (!isBoss) {
                    (c->effect_fun) (f);
                    //green();
                    //printf("\t%s status returned to normal.\n",f->name);
                    //white();
                } else {	//Boss function
                    (c->effect_b_fun) (b);
                    //lightRed();
                    //printf("\t%s status returned to normal.\n",stringFromBossClass(b->class));
                    //white();
                }
            }
            break;
            case CNT_ATKBOOST:
            case CNT_DEFBOOST:
            case CNT_VELBOOST:
            case CNT_ENRBOOST: { //Callback for stat boosts
                if (!isBoss) {
                    (c->boost_fun) (f, 0);	//Invoke ~setPermBoost(STAT)~ with value 0
                } else {
                    assert(0 && "UNACCESSIBLE CALL.\n");
                    (c->boost_b_fun) (b, 0);	//Call ~setPermBoost(STAT)~ with value 0
                }
            }
            break;
            }

            //Set counter to zero
            c->count = 0;
            c->innerValue = 0;
        } else if (c->count > 1) {	//We simply advance the count
            c->count -= 1;
        }
    };
}

/**
 * Takes a Fighter pointer and the amount of xp to add.
 * Current level xp is managed by this function, including how much xp is needed to level up again. The values are stored in the fighter struct.
 * Thresholds for each level are checked and eventually onLevelUp() is called, with recursion on this function after.
 * @see Fighter
 * @see onLevelUp()
 * @param player The Fighter pointer that gets xp.
 * @param xp The amount of xp.
 */
void checkremainder(Fighter *player, int xp)
{
    int curr = player->currentlevelxp;
    int tot = player->totallevelxp;

    //TODO: Print notifications to a passed WINDOW

    if (curr + xp >= tot) {
        player->totalxp += xp;
        //if (xp !=0) printf("\n\t%s obtained %i xp.", player->name, xp);
        player->level++;

        //cyan();
        //printf("\n\t%s reached Lvl. %i !", player->name, player->level);
        //white();

        //Stats gains on level up
        onLevelUp(player);

        player->currentlevelxp = abs((tot - curr - xp));
        int nextLevelMoreXp =
            round(1.75 * (player->level + 1)) + player->level + 1;
        player->totallevelxp = (tot / player->level) + nextLevelMoreXp;
        checkremainder(player, 0);
    } else {
        player->currentlevelxp += xp;
        player->totalxp += xp;
        if (xp != 0) {
            //printf("\n\t%s obtained %i xp.", player->name, xp);
        }
    }
}

/**
 * Takes a Fighter and a Enemy pointers and handles xp gain by fighter.
 * @see Fighter
 * @see Enemy
 * @see getEnemyXpGain()
 * @see checkremainder()
 * @param player The Fighter pointer that gets xp.
 * @param e The Enemy pointer that gives xp.
 */
void giveXp(Fighter *player, Enemy *e)
{

    int xp = getEnemyXpGain(e);

    checkremainder(player, xp);
}

/**
 * Takes a Fighter and a Boss pointers and handles xp gain by fighter.
 * @see Fighter
 * @see Boss
 * @see getBossXpGain()
 * @see checkremainder()
 * @param player The Fighter pointer that gets xp.
 * @param b The Boss pointer that gives xp.
 */
void giveXp_Boss(Fighter *player, Boss *b)
{

    int xp = getBossXpGain(b);

    checkremainder(player, xp);
}

/**
 * Takes a Enemy pointer and returns its xp gain as sum of xp field value and level.
 * @see Enemy
 * @param e The Enemy pointer.
 * @return int The xp gain.
 */
int getEnemyXpGain(Enemy *e)
{

    int xp = e->xp + e->level;
    return xp;
}

/**
 * Takes a Boss pointer and returns its xp gain as sum of xp field value and level.
 * @see Boss
 * @param b The Boss pointer.
 * @return int The xp gain.
 */
int getBossXpGain(Boss *b)
{

    int xp = b->xp + b->level;
    return xp;
}

/**
 * Takes a Fighter pointer and updated its stats.
 * getBoost() is called to get the stat boost for current level, which is then applyed to atk, def and vel; while hp gets first multiplied by 1.13 and then gets the boost added.
 * The totalenergy value is increased by player level over 5.
 * Hp and energy are replenished.
 * If the level is multiple of SPECIALLVLRATIO and the player still has at least one SpecialSlot not enabled, unlockSpecial() is called.
 * @see Fighter
 * @see getBoost()
 * @see BaseStats
 * @see SpecialSlot
 * @see SPECIALSMAX
 * @see SPECIALLVLRATIO
 * @see unlockSpecial()
 * @param player The Fighter pointer that levels up.
 */
void onLevelUp(Fighter *player)
{
    int boost = getBoost(player->level, player->luck);

    BaseStats *base = &basestats[player->class];

    player->atk = (base->atk + boost);
    player->def = (base->def + boost);
    player->vel = (base->vel + boost);
    player->totalhp = (base->hp * 1.13) + boost;

    int energyboost = player->level / 5;	//Energy grows by lvl/5 on each lvl up
    player->totalenergy =
        base->totalenergy + energyboost + player->permboost_enr;

    player->hp = player->totalhp;	//Cure on level up
    player->energy = player->totalenergy;	//Refill energy on level up

    //Check if you get a new special move
    if (player->level % SPECIALLVLRATIO == 0
        && (player->stats->specialsunlocked < (SPECIALSMAX + 1))) {
        unlockSpecial(player);
    }

    log_tag("debug_log.txt", "[LEVELUP]", "Player leveled up.");

    /*
       lightCyan();
       printf("\n\n\tYour wounds were healed.");
       printf("\n\tYour energy is replenished.");
       white();
     */
}

/**
 * Takes two integers for level to calc against and luck, and returns the boost relative to the level with luck variations, as an integer.
 * At level 1, returns 0.
 * @param lvl The level to check the boost against.
 * @param luck The luck value to influence calcs.
 * @return int The boost for any given stat, at the level passed as argument.
 */
int getBoost(int lvl, int luck)
{

    float boost = (lvl * 1.25F) + ((luck % 2) * 2);

    if (lvl < 2) {
        boost = 1.0;		// Evitare conflitti
    }

    return (int)boost;
}

/**
 * Takes a Fighter pointer and deleted all the equips not in use, granting a payment to the Fighter balance.
 * @see Shop
 * @see handleRoom_Shop()
 * @see Fighter
 * @param f The Fighter pointer at hand.
 */
void sell_all_equips(Fighter *f, Koliseo_Temp *t_kls)
{
    char msg[200];

    Equip *saved_equips[EQUIPZONES + 1];
    int saved_count = 0;
    Koliseo *kls = t_kls->kls;

    for (int i = 0; i < EQUIPZONES + 1; i++) {
        if (f->equipslots[i]->active) {
            sprintf(msg,
                    "sell_all_equips(): Prepping Equip to save f->equipslot[%i]",
                    i);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            kls_log(kls, "DEBUG", msg);
            Equip *saved =
                (Equip *) KLS_PUSH_T_TYPED(t_kls, Equip, HR_Equip, "Equip",
                                           msg);
            Equip *to_save = f->equipslots[i]->item;

            saved->class = to_save->class;
            saved->type = to_save->type;
            strcpy(saved->name, to_save->name);
            strcpy(saved->desc, to_save->desc);
            saved->qty = to_save->qty;
            saved->equipped = 0;	//Will be set after when re-equipped
            saved->level = to_save->level;
            saved->atk = to_save->atk;
            saved->def = to_save->def;
            saved->vel = to_save->vel;
            saved->enr = to_save->enr;
            saved->bonus = to_save->bonus;
            saved->perksCount = 0;	//Will be set during perks copy
            saved->qual = to_save->qual;
            saved->equip_fun = to_save->equip_fun;

            for (int j = 0; j < to_save->perksCount; j++) {
                sprintf(msg,
                        "sell_all_equips(): Prepping Perk (%i) to save f->equipslot[%i]",
                        j, i);
                log_tag("debug_log.txt", "[DEBUG]", msg);
                kls_log(kls, "DEBUG", msg);
                Perk *save_pk =
                    (Perk *) KLS_PUSH_T_TYPED(t_kls, Perk, HR_Perk, "Perk",
                                              msg);
                save_pk->class = to_save->perks[j]->class;
                strcpy(save_pk->name, to_save->perks[j]->name);
                strcpy(save_pk->desc, to_save->perks[j]->desc);
                save_pk->innerValue = to_save->perks[j]->innerValue;
                saved->perks[saved->perksCount] = save_pk;
                saved->perksCount++;
            }

            for (int j = 0; j < 8; j++) {
                strcpy(saved->sprite[j], to_save->sprite[j]);
            }

            saved_equips[saved_count] = saved;
            saved_count++;
        }
    }

    int deleted_count = 0;
    int pay = 0;

    for (int i = 0; i < f->equipsBagOccupiedSlots; i++) {
        Equip *toDel = f->equipsBag[i];
        pay += toDel->cost / 2;
        //int perksTot = toDel->perksCount;
        /*
           for (int j = 0; j < perksTot; j++) {
           Perk* pk = toDel->perks[j];
           free(pk);
           }
         */
        //FIXME: are we deleting this correctly?
        //free(toDel);
        deleted_count++;
    }

    f->equipsBagOccupiedSlots -= deleted_count;

    for (int i = 0; i < saved_count; i++) {
        f->equipsBag[i] = saved_equips[i];
        f->equipslots[i]->item = saved_equips[i];
        saved_equips[i]->equipped = 1;
        f->equipsBagOccupiedSlots++;
    }
    f->earliestBagSlot = f->equipsBagOccupiedSlots;

    f->balance += pay;

}

/**
 * Returns the chosen option as a turnOption.
 * @param ch A string representing the turn choice.
 * @return The chosen turnOption value representing turn action.
 */
turnOption getTurnChoice(char *ch)
{
    int comp = 999;

    log_tag("debug_log.txt", "[TURNPICK]", "Turnchoice string was (%s)", ch);
    turnOption pick = INVALID;

    while (pick == INVALID) {
        if ((comp = strcmp(ch, "Fight")) == 0) {
            pick = FIGHT;
        } else if ((comp = strcmp(ch, "New game")) == 0) {
            pick = NEW_GAME;
        } else if ((comp = strcmp(ch, "Load save")) == 0) {
            pick = LOAD_GAME;
        } else if ((comp = strcmp(ch, "Special")) == 0) {
            pick = SPECIAL;
        } else if ((comp = strcmp(ch, "Consumables")) == 0) {
            pick = CONSUMABLE;
        } else if ((comp = strcmp(ch, "Artifacts")) == 0) {
            pick = ARTIFACTS;
        } else if ((comp = strcmp(ch, "Equips")) == 0) {
            pick = EQUIPS;
        } else if ((comp = strcmp(ch, "Perks")) == 0) {
            pick = PERKS;
        } else if ((comp = strcmp(ch, "Stats")) == 0) {
            pick = STATS;
        } else if ((comp = strcmp(ch, "Save")) == 0) {
            pick = SAVE;
        } else if ((comp = strcmp(ch, "Debug")) == 0) {
            pick = DEBUG;
        } else if ((comp = strcmp(ch, "Quit")) == 0) {
            pick = QUIT;
        } else if ((comp = strcmp(ch, "Explore")) == 0) {
            pick = EXPLORE;
        } else if ((comp = strcmp(ch, "Tutorial")) == 0) {
            pick = TUTORIAL;
        } else if ((comp = strcmp(ch, "Close")) == 0) {
            pick = CLOSE_MENU;
        } else {
            pick = INVALID;
        }
    }

    log_tag("debug_log.txt", "[TURNOPT]", "Pick was: (%i)", pick);

    if (pick == INVALID) {
        fprintf(stderr, "Error: unexpected turn choice value");
        log_tag("debug_log.txt", "[ERROR]",
                "Unexpected turn choice in getTurnChoice(), quitting");
        exit(EXIT_FAILURE);
    }
    return pick;
}

/**
 * Returns a randomised pick as foeTurnOption.
 * @param e Pointer to the enemy to pick for.
 * @param f Pointer to the target fighter.
 * @return The chosen foeturnOption value representing turn action.
 */
foeTurnOption enemyTurnPick(Enemy *e, Fighter *f)
{
    if (e == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "enemyTurnPick():  Enemy was NULL.");
        exit(EXIT_FAILURE);
    }
    if (f == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "enemyTurnPick():  Fighter was NULL.");
        exit(EXIT_FAILURE);
    }
    foeTurnOption pick = FOE_INVALID;

    while (pick == FOE_INVALID) {
        int rn = rand() % 101;
        /*
           if (rn > 80) {
           //TODO
           //pick = FOE_SPECIAL;
           pick = FOE_IDLE;

           } else
         */
        if (rn > 40) {
            pick = FOE_FIGHT;
        } else {
            pick = FOE_IDLE;
        }
    }

    log_tag("debug_log.txt", "[FOETURNOPTION]", "Pick was: ( %i ) [ %s ]", pick,
            stringFromFoeTurnOP(foeTurnOP_from_foeTurnOption(pick)));

    if (pick == FOE_INVALID) {
        fprintf(stderr, "Error: unexpected turn choice value");
        log_tag("debug_log.txt", "[ERROR]",
                "Unexpected turn choice in enemyTurnPick(), quitting");
        exit(EXIT_FAILURE);
    }
    return pick;
}

/**
 * Returns a randomised pick as foeTurnOption.
 * @param b Pointer to the boss to pick for.
 * @param f Pointer to the target fighter.
 * @return The chosen foeturnOption value representing turn action.
 */
foeTurnOption bossTurnPick(Boss *b, Fighter *f)
{
    if (b == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "bossTurnPick():  Boss was NULL.");
        exit(EXIT_FAILURE);
    }
    if (f == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "bossTurnPick():  Fighter was NULL.");
        exit(EXIT_FAILURE);
    }
    foeTurnOption pick = FOE_INVALID;

    while (pick == FOE_INVALID) {
        int rn = rand() % 101;
        /*
           if (rn > 80) {
           //TODO
           //pick = FOE_SPECIAL;
           pick = FOE_IDLE;
           } else
         */
        if (rn > 40) {
            pick = FOE_FIGHT;
        } else {
            pick = FOE_IDLE;
        }
    }

    log_tag("debug_log.txt", "[FOETURNOPTION]", "Pick was: ( %i ) [ %s ]", pick,
            stringFromFoeTurnOP(foeTurnOP_from_foeTurnOption(pick)));

    if (pick == FOE_INVALID) {
        fprintf(stderr, "Error: unexpected turn choice value");
        log_tag("debug_log.txt", "[ERROR]",
                "Unexpected turn choice in enemyTurnPick(), quitting");
        exit(EXIT_FAILURE);
    }
    return pick;
}

/**
 * Takes a Fighter, a Room and a loadInfo pointers, and prints fighter stats and a quitting message, before quitting the program and freeing Room.
 *
 * @see Fighter
 * @see printStats()
 * @see death()
 * @param p The Fighter pointer at hand.
 * @param room The Room pointer at hand.
 * @param load_info The loadInfo pointer at hand.
 * @param t_kls The Koliseo_Temp to end if possible.
 */
void quit(Fighter *p, Room *room, loadInfo *load_info, Koliseo_Temp *t_kls)
{
    char msg[500];
    Koliseo *kls = t_kls->kls;
#ifdef HELAPORDO_CURSES_BUILD
    endwin();
#endif
    int res = system("reset");
    sprintf(msg, "quit() system(\"reset\") res was (%i)", res);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    //printf("\n\n\tTHANKS 4 PLAYING\n");
    //FIXME
    //dropping out of the Koliseo scope might render stat pointer invalid.
    //Can't we print stats and clear the kls?
    //printStats(p);
    //printf("\n");
#ifndef _WIN32
    sprintf(msg, "Resetting Koliseo_Temp from: (%li)", t_kls->kls->offset);
#else
    sprintf(msg, "Resetting Koliseo_Temp from: (%lli)", t_kls->kls->offset);
#endif
    kls_log(kls, "DEBUG", msg);
    death(p, load_info);
    //FIXME
    //I think we should free those?
    //May cause a crash?
    //kls_free(default_kls);
    //kls_free(temporary_kls);
    //FIXME:
    //Calling this segfaults?
    //freeRoom(room);
    log_tag("debug_log.txt", "[DEBUG]", "Quitting program.");
    exit(EXIT_SUCCESS);
}

/**
 * Takes a Turncounter pointer and an integer.
 * If the count value at the pointer is 0 (counter is inactive), the turns valueis assigned.
 * @see Turncounter
 * @param c The Turncounter whose count value will be set.
 * @param turns The value to be assigned.
 */
void setCounter(Turncounter *c, int turns)
{

    if (c->count == 0) {	// Counter is NOT already active
        c->count = turns;
    } else {
        //Handle counters already activ
    }
}

/**
 * Takes a Fighter, an Enemy and a Boss pointers, a string denoting the consumableClass and an int for use on enemy or boss.
 * If qty value for the Consumable is 0, we have an early return. Otherise effect is applied and qty is decreased.
 * @see Fighter
 * @see Enemy
 * @see Boss
 * @see Consumable
 * @see consumableClass
 * @param f The Fighter pointer at hand.
 * @param e The Enemy pointer at hand.
 * @param b The Boss pointer at hand.
 * @param string The string value of consumable to use.
 * @param isBoss The mode of use (on boss if == 1)
 */
void useConsumable(Fighter *f, Enemy *e, Boss *b, char *string, int isBoss)
{
    //Mountain of ifs? Where are my damn maps

    int i = 0;
    int check;
    for (int j = 0; j < CONSUMABLESMAX + 1; j++) {
        if ((check = strcmp(consumablestrings[j], string)) == 0) {
            i = j;
        }
    };

    Consumable *c = (Consumable *) f->consumablesBag[i];

    if (c->qty == 0) {
        return;
    }
    if (c->class == Potion) {
        f->hp = f->totalhp;
        //green();
        //printf("\n\n\n\tYour health was restored.\n\n");
        //white();
    } else if (c->class == Rock) {
        if (isBoss) {
            b->def -= 8;
            b->hp -= 2;
            //printf("\n\n\n\t%s got smacked.",stringFromBossClass(b->class));
        } else {
            e->def -= 8;
            e->hp -= 2;
            //printf("\n\n\n\t%s got smacked.",stringFromEClass(e->class));
        }
        //red();
        //printf("\t-8 DEF and -2 HP.\n\n");
        //white();
    } else if (c->class == Bread) {
        f->vel -= 1;
        f->def += 6;
        f->atk += 4;
        //printf("\n\n\n\tYou ate elvish bread.");
        //green();
        //printf("\t+6 DEF , +4 ATK, ");
        //yellow();
        //printf("-1 VEL\n\n");
        //white();
    } else if (c->class == Coffee) {
        f->vel += 6;
        f->atk += 2;
        f->def -= 2;
        //printf("\n\n\n\tYou drank hot coffee.");
        //green();
        //printf("\t+6 SPE , +2 ATK, ");
        //yellow();
        //printf("-2 DEF\n\n");
        //white();
    } else if (c->class == Powergem) {
        //printf("\n\n\n\tYou became wiser.");
        //green();
        //printf("\t+25 XP\n\n");
        //white();
        checkremainder(f, 25);
    } else if (c->class == Magicbean) {
        //printf("\n\n\n\tYou aren't tired anymore.");
        //green();
        //printf("\tEnergy was restored\n\n");
        //white();
        f->energy = f->totalenergy;	//Refill energy
    } else {
        return;
    }

    c->qty--;
}
