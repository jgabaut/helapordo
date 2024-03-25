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
#include "saves.h"

#ifdef HELAPORDO_CURSES_BUILD
bool set_Saveslot_name(FILE *file, Saveslot *sv)
{

    //printf("\nLoading game...\n");
    log_tag("debug_log.txt", "[LOAD]", "%s():    Starting loading from savefile.", __func__);

//      FILE* file = fopen("save.txt", "r");
    if (G_EXPERIMENTAL_ON != 1 && file == NULL) {
        endwin();
        printf("Error with file while trying to load!\n");
        return false;
    }
    char buf[500];

    if (G_EXPERIMENTAL_ON == 0) {
        log_tag("debug_log.txt", "[LOAD]", "%s():    Doing text file loading.", __func__);
        char comment[300];
        int num_value = -1;
        const char version[] = "0.1.7";

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
    } else {
        log_tag("debug_log.txt", "[LOAD]", "%s():    Doing binary file loading.", __func__);
        char path_to_sv_file[800];
        char static_path[500];
        // Set static_path value to the correct static dir path
        resolve_staticPath(static_path);

#ifndef _WIN32
        sprintf(path_to_sv_file, "%s/%s/%s", static_path,
                    sv->save_path, "run-nc.bin");
#else
        sprintf(path_to_sv_file, "%s\\%s\\%s", static_path,
                    sv->save_path, "run-nc.bin");
#endif
        SerGamestate ser_gmst = {0};
        bool read_res = readSerGamestate(path_to_sv_file, 0, &ser_gmst);
        if (!read_res) {
            log_tag("debug_log.txt", "[WARN]", "%s():    Failed readSerGamestate() for {%s}.", __func__, path_to_sv_file);
            return false;
        } else {
            strncpy(buf, ser_gmst.player.name, 50);
        }
    }
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
 * Takes a FILE pointer from which the gamestate will be loaded.
 * Gets the pointers to relevant game structures, sets all the values by reading the save file.
 * Fighter, Path pointers, and an integer for room index.
 * Saves the state of the game.
 * @see Fighter
 * @see Path
 * @see gameloop()
 * @param file The FILE pointer to load from.
 * @param f The player struct.
 * @param p The path struct.
 * @param roomIndex Current room index.
 * @param done_loading Flag to check if loading ended.
 * @param kls The Koliseo used for allocations.
 * @return The OP_res corresponding to loading result.
 */
OP_res handleLoadgame_Home(FILE *file, Fighter *f, Path *p, int *roomIndex,
                           int *done_loading, Koliseo *kls)
{

    //printf("\nLoading game...\n");
    log_tag("debug_log.txt", "[LOAD]", "Starting loading from text file.");

//      FILE* file = fopen("save.txt", "r");
    if (file == NULL) {
        printf("Error with file while trying to load!\n");
        return OP_CANNOT_OPEN_SAVEFILE;
    }
    char buf[500];
    char comment[300];
    int num_value = -1;
    const char * version = HELAPORDO_SAVEFILE_VERSION;

    int scanres = -1;
    /* File version scanning */
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    int check = -1;
    if (!((check = strcmp(buf, version)) == 0)) {
        log_tag("debug_log.txt", "[LOAD-ERROR]",
                "Failed save format version check. Was [%s]. Quitting.\n", buf);
        fprintf(stderr, "[ERROR]    File version mismatch on load.\n");
        exit(EXIT_FAILURE);
    };
    log_tag("debug_log.txt", "[LOAD]", "Loaded save format version: (%s).",
            buf);

    /* Save type scanning */
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    check = -1;
    if (!((check = strcmp(buf, stringFrom_saveType(HOME_SAVE))) == 0)) {
        log_tag("debug_log.txt", "[LOAD-ERROR]",
                "handleLoadGame_Home():  Failed save type check, was [%s]. Quitting.",
                buf);
        fprintf(stderr, "[ERROR]    Save type version mismatch on load.\n");
        exit(EXIT_FAILURE);
    };
    log_tag("debug_log.txt", "[LOAD]", "Loaded save type: (%s).", buf);

    /* Gamemode scanning */
    scanres = fscanf(file, "%200[^#]# %s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    GAMEMODE = num_value;

    /* Fighter scanning */
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    strcpy(f->name, buf);

    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    for (int i = 0; i < CLASSESMAX + 1; i++) {
        int check = -1;
        if ((check = strcmp(buf, stringFromClass(i))) == 0) {
            f->class = i;
            break;
        }
    };

    //We have to reload the specials callbacks after setting fighter class
    setSpecials(f, kls);

    //HP
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->hp = num_value;
    //TotalHP
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->totalhp = num_value;
    //Atk
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->atk = num_value;
    //Def
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    sscanf(buf, "%3i", &num_value);
    f->def = num_value;
    //Vel
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->vel = num_value;
    //Level
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->level = num_value;
    //Luck
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->luck = num_value;
    //Totxp
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    f->totalxp = num_value;
    //Current level xp
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    f->currentlevelxp = num_value;
    //Totallevelxp
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    f->totallevelxp = num_value;
    //Energy
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->energy = num_value;
    //Totenergy
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->totalenergy = num_value;
    //Stamina
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stamina = num_value;
    //Totstamina
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->totalstamina = num_value;
    //Coin balance
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    f->balance = num_value;

    //Status
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    for (int i = 0; i < STATUSMAX + 1; i++) {
        int check = -1;
        if ((check = strcmp(buf, stringFromStatus(i))) == 0) {
            f->status = i;
            break;
        }
    };
    //Specials
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < SPECIALSMAX + 1; i++) {
        //Enabled flag
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        f->specials[i]->enabled = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //Skills
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < FIGHTER_SKILL_SLOTS; i++) {
        //Enabled flag
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        f->skills[i]->enabled = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //Fighter counters
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < COUNTERSMAX + 1; i++) {
        //innerValue
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        f->counters[i]->innerValue = num_value;
        //count
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        f->counters[i]->count = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);

    //turnboost_atk
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->turnboost_atk = num_value;
    //turnboost_def
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->turnboost_def = num_value;
    //turnboost_vel
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->turnboost_vel = num_value;
    //turnboost_enr
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->turnboost_enr = num_value;

    //equipsbag occupied slots
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->equipsBagOccupiedSlots = num_value;
    //equipsbag earliest slot
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->earliestBagSlot = num_value;

    //Equipsbag
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < f->equipsBagOccupiedSlots; i++) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Loadgame_Home(): Prepping Equip (%i)", i);
        kls_log(kls, "DEBUG", "Loadgame_Home(): Prepping Equip (%i)", i);
        Equip *eq = KLS_PUSH_TYPED(kls, Equip, HR_Equip, "Equip", "Equip");
        scanres = fscanf(file, "%200s\n", buf);	//Skip equip_i heading
        if (scanres != 1) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    1, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        //equipClass
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        for (int j = 0; j < EQUIPSMAX + 1; j++) {
            int check = -1;
            if ((check = strcmp(buf, stringFromEquips(j))) == 0) {
                eq->class = j;
                break;
            }
        };
        //equipzoneClass
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        for (int j = 0; j < EQUIPZONES + 1; j++) {
            int check = -1;
            if ((check = strcmp(buf, stringFromEquipzones(j))) == 0) {
                eq->type = j;
                break;
            }
        };

        //qty
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->qty = num_value;
        //equipped
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->equipped = num_value;
        //level
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->level = num_value;
        //atk
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->atk = num_value;
        //def
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->def = num_value;
        //vel
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->vel = num_value;
        //enr
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->enr = num_value;
        //bonus
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->bonus = num_value;
        //perksCount
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->perksCount = num_value;
        //cost
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%8i", &num_value);
        eq->cost = num_value;

        //Quality
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        for (int j = 0; j < QUALITIESMAX + 1; j++) {
            int check = -1;
            if ((check = strcmp(buf, stringFromQuality(j))) == 0) {
                eq->qual = j;
                break;
            }
        };

        //perks
        scanres = fscanf(file, "%200s\n", buf);	//Skip perks heading
        if (scanres != 1) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    1, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < eq->perksCount; j++) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Loadgame_Home(): Prepping Perk (%i)", j);
            kls_log(kls, "DEBUG", "Loadgame_Home(): Prepping Perk (%i)", j);
            Perk *pk =
                (Perk *) KLS_PUSH_TYPED(kls, Perk, HR_Perk, "Perk", "Perk");
            //perkClass
            scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
            if (scanres != 2) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                        2, scanres);
                fprintf(stderr, "Error while loading game.");
                exit(EXIT_FAILURE);
            }

            log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
            sscanf(buf, "%3i", &num_value);
            pk->class = num_value;
            char *name = nameStringFromPerk(pk->class);
            char *desc = descStringFromPerk(pk->class);
            //p->name = name; //(char*)malloc(sizeof(name));
            strcpy(pk->name, name);
            //p->desc = (char*)malloc(sizeof(desc));
            strcpy(pk->desc, desc);
            pk->innerValue = 0;
            eq->perks[j] = pk;

            //innerValue
            scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
            if (scanres != 2) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                        2, scanres);
                fprintf(stderr, "Error while loading game.");
                exit(EXIT_FAILURE);
            }

            log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
            sscanf(buf, "%3i", &num_value);
            pk->innerValue = num_value;
        }

        scanres = fscanf(file, "%200s\n", buf);	//Skip closing perks breacket
        if (scanres != 1) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    1, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        scanres = fscanf(file, "%200s\n", buf);	//Skip closing current equip bracket
        if (scanres != 1) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    1, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        //Get the base item and copy the stats to the loading item
        Equip *base = &equips[eq->class];
        setEquipSprite(eq);
        strcpy(eq->name, base->name);
        strcpy(eq->desc, base->desc);

        if (eq->equipped) {
            Equipslot *slot = f->equipslots[eq->type];
            //We equip the item
            slot->item = eq;
            applyEquipPerks(eq, f);
            slot->active = 1;
        }
        f->equipsBag[i] = eq;
    }				//End for Equips bag

    scanres = fscanf(file, "%200s\n", buf);	//Skip closing equipsbag bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    //Fighter boosts
    //permboost_atk
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->permboost_atk = num_value;
    //permboost_def
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->permboost_def = num_value;
    //permboost_vel
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->permboost_vel = num_value;
    //permboost_enr
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->permboost_enr = num_value;
    //equipboost_atk
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->equipboost_atk = num_value;
    //equipboost_def
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->equipboost_def = num_value;
    //equipboost_vel
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->equipboost_vel = num_value;
    //equipboost_enr
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->equipboost_enr = num_value;

    //Stats
    scanres = fscanf(file, "%200s\n", buf);	//Skip Stats bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //enemieskilled
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->enemieskilled = num_value;
    //consumablesfound
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->consumablesfound = num_value;
    //equipsfound
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->equipsfound = num_value;
    //artifactsfound
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->artifactsfound = num_value;
    //crits
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->criticalhits = num_value;
    //roomsdone
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->roomscompleted = num_value;
    //specials unlocked
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->specialsunlocked = num_value;
    //coinsfound
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    f->stats->coinsfound = num_value;
    //bosseskilled
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->bosseskilled = num_value;

    //Unique boss kills
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->unique_bosseskilled = num_value;

    scanres = fscanf(file, "%200s\n", buf);	//Skip unique bosses bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    for (int boss_index = 0; boss_index < BOSSCLASSESMAX + 1; boss_index++) {
        //boss %i was killed
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        f->stats->killed_bosses[boss_index] = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);	//Skip unique bosses closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //keysfound
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->keysfound = num_value;
    scanres = fscanf(file, "%200s\n", buf);	//Skip Stats closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    scanres = fscanf(file, "%200s\n", buf);	//Skip Fighter closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    log_tag("debug_log.txt", "[LOAD]", "Done loading Fighter.");

    //Path
    scanres = fscanf(file, "%200s\n", buf);	//Skip Path bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //Luck
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    p->luck = num_value;
    //Length
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    p->length = num_value;
    //Prize
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    p->prize = num_value;
    //loreCounter
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    p->loreCounter = num_value;

    //Wincon
    scanres = fscanf(file, "%200s\n", buf);	//Skip Wincon bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //winconClass
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    p->win_condition->class = num_value;
    //current_val
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    p->win_condition->current_val = num_value;
    //target_val
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    p->win_condition->target_val = num_value;

    scanres = fscanf(file, "%200s\n", buf);	//Skip Wincon closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    log_tag("debug_log.txt", "[LOAD]", "Done loading Wincon.");

    scanres = fscanf(file, "%200s\n", buf);	//Skip Path closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    log_tag("debug_log.txt", "[LOAD]", "Done loading Path.");

    //roomindex
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    *roomIndex = num_value;

    log_tag("debug_log.txt", "[LOAD]", "Done loading room info.");

    //Consumables bag
    scanres = fscanf(file, "%200s\n", buf);	//Skip Consumablebag bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    initConsumableBag(f, kls);
    for (int i = 0; i < CONSUMABLESMAX + 1; i++) {
        Consumable *cn = (Consumable *) f->consumablesBag[i];
        //qty
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        cn->qty = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);	//Skip Consumablebag closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    log_tag("debug_log.txt", "[LOAD]", "Done loading Consumable bag.");

    //Artifacts bag
    scanres = fscanf(file, "%200s\n", buf);	//Skip Artifactsbag bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < ARTIFACTSMAX + 1; i++) {
        Artifact *a = f->artifactsBag[i];
        //qty
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        a->qty = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);	//Skip Artifactsbag closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    log_tag("debug_log.txt", "[LOAD]", "Done loading Artifacts bag.");

    fclose(file);
    log_tag("debug_log.txt", "[LOAD]", "Done loading from text file!");
    //printf("[INFO]    Done loading!\n");
    //TODO:
    //Notification win for load status

    return OP_OK;
}

/**
 * Takes a FILE pointer from which the gamestate will be loaded.
 * Gets the pointers to relevant game structures, sets all the values by reading the save file.
 * Fighter, Path and Enemy pointers, and integers for room / path progression indexes.
 * Saves the state of the game.
 * @see Fighter
 * @see Enemy
 * @see Path
 * @see gameloop()
 * @param file The FILE pointer to load from.
 * @param f The player struct.
 * @param p The path struct.
 * @param e The current enemy struct.
 * @param enemyIndex Index in current room for current enemy.
 * @param roomTotalEnemies Total enemies for current room.
 * @param roomIndex Current room index.
 * @param total_foes Total foes in current room.
 * @param done_loading Flag to check if loading ended.
 * @param kls The Koliseo used for allocations.
 * @return The OP_res corresponding to loading result.
 */
OP_res handleLoadgame_Enemies(FILE *file, Fighter *f, Path *p, Enemy *e,
                              int *enemyIndex, int *roomTotalEnemies,
                              int *roomIndex, int *total_foes,
                              int *done_loading, Koliseo *kls)
{

    //printf("\nLoading game...\n");
    log_tag("debug_log.txt", "[LOAD]", "Starting loading from text file.");

//      FILE* file = fopen("save.txt", "r");
    if (file == NULL) {
        printf("Error with file while trying to load!\n");
        return OP_CANNOT_OPEN_SAVEFILE;
    }
    char buf[500];
    char comment[300];
    int num_value = -1;
    const char version[] = HELAPORDO_SAVEFILE_VERSION;

    int scanres = -1;
    /* File version scanning */
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    int check = -1;
    if (!((check = strcmp(buf, version)) == 0)) {
        log_tag("debug_log.txt", "[LOAD-ERROR]",
                "Failed save format version check. Was [%s]. Quitting.\n", buf);
        fprintf(stderr, "[ERROR]    File version mismatch on load.\n");
        exit(EXIT_FAILURE);
    };
    log_tag("debug_log.txt", "[LOAD]", "Loaded save format version: (%s).\n",
            buf);

    /* Save type scanning */
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    check = -1;
    if (!((check = strcmp(buf, stringFrom_saveType(ENEMIES_SAVE))) == 0)) {
        log_tag("debug_log.txt", "[LOAD-ERROR]",
                "handleLoadGame_Enemies():  Failed save type check, was [%s]. Quitting.\n",
                buf);
        fprintf(stderr, "[ERROR]    Save type version mismatch on load.\n");
        exit(EXIT_FAILURE);
    };
    log_tag("debug_log.txt", "[LOAD]", "Loaded save type: (%s).\n", buf);

    /* Gamemode scanning */
    scanres = fscanf(file, "%200[^#]# %s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.\n", comment, buf);
    sscanf(buf, "%3i", &num_value);
    GAMEMODE = num_value;

    /* Fighter scanning */
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    strcpy(f->name, buf);

    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    for (int i = 0; i < CLASSESMAX + 1; i++) {
        int check = -1;
        if ((check = strcmp(buf, stringFromClass(i))) == 0) {
            f->class = i;
            break;
        }
    };

    //We have to reload the specials callbacks after setting fighter class
    setSpecials(f, kls);

    //HP
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->hp = num_value;
    //TotalHP
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->totalhp = num_value;
    //Atk
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->atk = num_value;
    //Def
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    sscanf(buf, "%3i", &num_value);
    f->def = num_value;
    //Vel
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->vel = num_value;
    //Level
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->level = num_value;
    //Luck
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->luck = num_value;
    //Totxp
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    f->totalxp = num_value;
    //Current level xp
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    f->currentlevelxp = num_value;
    //Totallevelxp
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    f->totallevelxp = num_value;
    //Energy
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->energy = num_value;
    //Totenergy
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->totalenergy = num_value;
    //Stamina
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stamina = num_value;
    //Totstamina
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->totalstamina = num_value;
    //Coin balance
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    f->balance = num_value;

    //Status
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    for (int i = 0; i < STATUSMAX + 1; i++) {
        int check = -1;
        if ((check = strcmp(buf, stringFromStatus(i))) == 0) {
            f->status = i;
            break;
        }
    };
    //Specials
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < SPECIALSMAX + 1; i++) {
        //Enabled flag
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        f->specials[i]->enabled = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //Skills
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < FIGHTER_SKILL_SLOTS; i++) {
        //Enabled flag
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        f->skills[i]->enabled = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //Fighter counters
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < COUNTERSMAX + 1; i++) {
        //innerValue
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        f->counters[i]->innerValue = num_value;
        //count
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        f->counters[i]->count = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);

    //turnboost_atk
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->turnboost_atk = num_value;
    //turnboost_def
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->turnboost_def = num_value;
    //turnboost_vel
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->turnboost_vel = num_value;
    //turnboost_enr
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->turnboost_enr = num_value;

    //equipsbag occupied slots
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->equipsBagOccupiedSlots = num_value;
    //equipsbag earliest slot
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->earliestBagSlot = num_value;

    //Equipsbag
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < f->equipsBagOccupiedSlots; i++) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Loadgame_Enemies(): Prepping Equip (%i)", i);
        kls_log(kls, "DEBUG", "Loadgame_Enemies(): Prepping Equip (%i)", i);
        Equip *eq = KLS_PUSH_TYPED(kls, Equip, HR_Equip, "Equip", "Equip");
        scanres = fscanf(file, "%200s\n", buf);	//Skip equip_i heading
        if (scanres != 1) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    1, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        //equipClass
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        for (int j = 0; j < EQUIPSMAX + 1; j++) {
            int check = -1;
            if ((check = strcmp(buf, stringFromEquips(j))) == 0) {
                eq->class = j;
                break;
            }
        };
        //equipzoneClass
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        for (int j = 0; j < EQUIPZONES + 1; j++) {
            int check = -1;
            if ((check = strcmp(buf, stringFromEquipzones(j))) == 0) {
                eq->type = j;
                break;
            }
        };

        //qty
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->qty = num_value;
        //equipped
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->equipped = num_value;
        //level
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->level = num_value;
        //atk
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->atk = num_value;
        //def
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->def = num_value;
        //vel
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->vel = num_value;
        //enr
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->enr = num_value;
        //bonus
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->bonus = num_value;
        //perksCount
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        eq->perksCount = num_value;
        //cost
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%8i", &num_value);
        eq->cost = num_value;

        //Quality
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        for (int j = 0; j < QUALITIESMAX + 1; j++) {
            int check = -1;
            if ((check = strcmp(buf, stringFromQuality(j))) == 0) {
                eq->qual = j;
                break;
            }
        };

        //perks
        scanres = fscanf(file, "%200s\n", buf);	//Skip perks heading
        if (scanres != 1) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    1, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < eq->perksCount; j++) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Loadgame_Enemies(): Prepping Perk (%i/%i)", j,
                    eq->perksCount);
            kls_log(kls, "DEBUG", "Loadgame_Enemies(): Prepping Perk (%i/%i)",
                    j, eq->perksCount);
            Perk *pk = KLS_PUSH_TYPED(kls, Perk, HR_Perk, "Perk", "Perk");
            //perkClass
            scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
            if (scanres != 2) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                        2, scanres);
                fprintf(stderr, "Error while loading game.");
                exit(EXIT_FAILURE);
            }

            log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
            sscanf(buf, "%3i", &num_value);
            pk->class = num_value;
            char *name = nameStringFromPerk(pk->class);
            char *desc = descStringFromPerk(pk->class);
            //p->name = name; //(char*)malloc(sizeof(name));
            strcpy(pk->name, name);
            //p->desc = (char*)malloc(sizeof(desc));
            strcpy(pk->desc, desc);
            pk->innerValue = 0;
            eq->perks[j] = pk;

            //innerValue
            scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
            if (scanres != 2) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                        2, scanres);
                fprintf(stderr, "Error while loading game.");
                exit(EXIT_FAILURE);
            }

            log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
            sscanf(buf, "%3i", &num_value);
            pk->innerValue = num_value;
        }

        scanres = fscanf(file, "%200s\n", buf);	//Skip closing perks breacket
        if (scanres != 1) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    1, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        scanres = fscanf(file, "%200s\n", buf);	//Skip closing current equip bracket
        if (scanres != 1) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    1, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }
        //Get the base item and copy the stats to the loading item
        Equip *base = &equips[eq->class];
        setEquipSprite(eq);
        strcpy(eq->name, base->name);
        strcpy(eq->desc, base->desc);

        if (eq->equipped) {
            Equipslot *slot = f->equipslots[eq->type];
            //We equip the item
            slot->item = eq;
            applyEquipPerks(eq, f);
            slot->active = 1;
        }
        f->equipsBag[i] = eq;
    }				//End for Equips bag

    scanres = fscanf(file, "%200s\n", buf);	//Skip closing equipsbag bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    //Fighter boosts
    //permboost_atk
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->permboost_atk = num_value;
    //permboost_def
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->permboost_def = num_value;
    //permboost_vel
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->permboost_vel = num_value;
    //permboost_enr
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->permboost_enr = num_value;
    //equipboost_atk
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->equipboost_atk = num_value;
    //equipboost_def
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->equipboost_def = num_value;
    //equipboost_vel
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->equipboost_vel = num_value;
    //equipboost_enr
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->equipboost_enr = num_value;

    //Stats
    scanres = fscanf(file, "%200s\n", buf);	//Skip Stats bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //enemieskilled
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->enemieskilled = num_value;
    //consumablesfound
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->consumablesfound = num_value;
    //equipsfound
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->equipsfound = num_value;
    //artifactsfound
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->artifactsfound = num_value;
    //crits
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->criticalhits = num_value;
    //roomsdone
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->roomscompleted = num_value;
    //specials unlocked
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->specialsunlocked = num_value;
    //coinsfound
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    f->stats->coinsfound = num_value;
    //bosseskilled
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->bosseskilled = num_value;

    //Unique boss kills
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->unique_bosseskilled = num_value;

    scanres = fscanf(file, "%200s\n", buf);	//Skip unique bosses bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    for (int boss_index = 0; boss_index < BOSSCLASSESMAX + 1; boss_index++) {
        //boss %i was killed
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        f->stats->killed_bosses[boss_index] = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);	//Skip unique bosses closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //keysfound
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    f->stats->keysfound = num_value;
    scanres = fscanf(file, "%200s\n", buf);	//Skip Stats closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    scanres = fscanf(file, "%200s\n", buf);	//Skip Fighter closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    log_tag("debug_log.txt", "[LOAD]", "Done loading Fighter.");

    //Current enemy
    scanres = fscanf(file, "%200s\n", buf);	//Skip Enemy bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //Enemy class
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    for (int i = 0; i < ENEMYCLASSESMAX + 1; i++) {
        int check = -1;
        if ((check = strcmp(buf, stringFromEClass(i))) == 0) {
            e->class = i;
            break;
        };
    }
    //We have to reload the sprite after setting enemy class
    setEnemySprite(e);

    //hp
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    e->hp = num_value;
    //totalhp
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    e->totalhp = num_value;
    //atk
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    e->atk = num_value;
    //def
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    e->def = num_value;
    //vel
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    e->vel = num_value;
    //level
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    e->level = num_value;
    //xp
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    e->xp = num_value;
    //energy
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    e->energy = num_value;
    //totalenergy
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    e->totalenergy = num_value;
    //Stamina
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    e->stamina = num_value;
    //Totstamina
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    e->totalstamina = num_value;
    //beast value
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    e->beast = num_value;
    //prize
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    e->prize = num_value;

    //status
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    for (int i = 0; i < STATUSMAX + 1; i++) {
        int check = -1;
        if ((check = strcmp(buf, stringFromStatus(i))) == 0) {
            e->status = i;
            break;
        }
    };

    //Skills
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < ENEMY_SKILL_SLOTS; i++) {
        //Enabled flag
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        e->skills[i]->enabled = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //Enemy counters
    scanres = fscanf(file, "%200s\n", buf);	//Skip Enemy counters bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < COUNTERSMAX + 1; i++) {
        //innerValue
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        e->counters[i]->innerValue = num_value;
        //count
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        e->counters[i]->count = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);	//Skip Enemy counters closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    scanres = fscanf(file, "%200s\n", buf);	//Skip Enemy closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    log_tag("debug_log.txt", "[LOAD]", "Done loading current Enemy.");

    //Path
    scanres = fscanf(file, "%200s\n", buf);	//Skip Path bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //Luck
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    p->luck = num_value;
    //Length
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    p->length = num_value;
    //Prize
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    p->prize = num_value;
    //loreCounter
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    p->loreCounter = num_value;

    //Wincon
    scanres = fscanf(file, "%200s\n", buf);	//Skip Wincon bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    //winconClass
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    p->win_condition->class = num_value;
    //current_val
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    p->win_condition->current_val = num_value;
    //target_val
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    p->win_condition->target_val = num_value;

    scanres = fscanf(file, "%200s\n", buf);	//Skip Wincon closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    log_tag("debug_log.txt", "[LOAD]", "Done loading Wincon.");

    scanres = fscanf(file, "%200s\n", buf);	//Skip Path closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    log_tag("debug_log.txt", "[LOAD]", "Done loading Path.");

    //enemyindex
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    *enemyIndex = num_value;
    //roomtotalenemies
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%3i", &num_value);
    *roomTotalEnemies = num_value;
    *total_foes = num_value;
    log_tag("debug_log.txt", "[LOAD]",
            "Also Loaded %s: %s. Additional assignment to pointer for foes, see handleLoadGame(_Enemies)",
            "total_foes", buf);
    //roomindex
    scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
    if (scanres != 2) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                2, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
    sscanf(buf, "%8i", &num_value);
    *roomIndex = num_value;

    log_tag("debug_log.txt", "[LOAD]", "Done loading room info.");

    //Consumables bag
    scanres = fscanf(file, "%200s\n", buf);	//Skip Consumablebag bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    initConsumableBag(f, kls);
    for (int i = 0; i < CONSUMABLESMAX + 1; i++) {
        Consumable *cn = (Consumable *) f->consumablesBag[i];
        //qty
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        cn->qty = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);	//Skip Consumablebag closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    log_tag("debug_log.txt", "[LOAD]", "Done loading Consumable bag.");

    //Artifacts bag
    scanres = fscanf(file, "%200s\n", buf);	//Skip Artifactsbag bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < ARTIFACTSMAX + 1; i++) {
        Artifact *a = f->artifactsBag[i];
        //qty
        scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
        if (scanres != 2) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                    2, scanres);
            fprintf(stderr, "Error while loading game.");
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[LOAD]", "Loaded %s: %s.", comment, buf);
        sscanf(buf, "%3i", &num_value);
        a->qty = num_value;
    }
    scanres = fscanf(file, "%200s\n", buf);	//Skip Artifactsbag closing bracket
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while loading game.");
        exit(EXIT_FAILURE);
    }

    log_tag("debug_log.txt", "[LOAD]", "Done loading Artifacts bag.");

    fclose(file);
    log_tag("debug_log.txt", "[LOAD]", "Done loading from text file!");
    //printf("[INFO]    Done loading!\n");
    //TODO:
    //Notification win for load status

    return OP_OK;
}

/**
 * Takes a FILE pointer to which the gamestate from the passed args will be saved.
 * Fighter, Path and an integer for room index.
 * Saves the state of the game.
 * @see Fighter
 * @see Path
 * @see gameloop()
 * @param file The FILE pointer struct to write to.
 * @param f The player struct.
 * @param p The path struct.
 * @param roomIndex Current room index.
 */
OP_res handleSave_Home(FILE *file, Fighter *f, Path *p, int roomIndex)
{
    const char* version = HELAPORDO_SAVEFILE_VERSION;
    //FILE *file = fopen("save.txt", "w");
    log_tag("debug_log.txt", "[DEBUG]", "Saving with version %s", version);

    if (file == NULL) {
        fprintf(stderr, "[ERROR]    Can't open save file!\n");
        return OP_CANNOT_OPEN_SAVEFILE;
    }

    /* File version printing */
    fprintf(file, "%s\n", version);

    /* Save type printing */
    fprintf(file, "%s\n", stringFrom_saveType(HOME_SAVE));

    /* Gamemode printing */
    fprintf(file, "%i# gamemode\n", GAMEMODE);

    /* Fighter printing */
    fprintf(file, "Fighter{\n");
    fprintf(file, "%s# name\n", f->name);
    fprintf(file, "%s# class\n", stringFromClass(f->class));
    fprintf(file, "%i# hp\n", f->hp);
    fprintf(file, "%i# totalhp\n", f->totalhp);
    fprintf(file, "%i# atk\n", f->atk);
    fprintf(file, "%i# def\n", f->def);
    fprintf(file, "%i# vel\n", f->vel);
    fprintf(file, "%i# level\n", f->level);
    fprintf(file, "%i# luck\n", f->luck);
    fprintf(file, "%i# totxp\n", f->totalxp);
    fprintf(file, "%i# currlevxp\n", f->currentlevelxp);
    fprintf(file, "%i# totlevxp\n", f->totallevelxp);
    fprintf(file, "%i# energy\n", f->energy);
    fprintf(file, "%i# totenergy\n", f->totalenergy);
    fprintf(file, "%i# stamina\n", f->stamina);
    fprintf(file, "%i# totstamina\n", f->totalstamina);
    fprintf(file, "%i# coinbalance\n", f->balance);
    fprintf(file, "%s# status\n", stringFromStatus(f->status));
    fprintf(file, "    Specials{\n");
    for (int i = 0; i < SPECIALSMAX + 1; i++) {
        fprintf(file, "%i# %i_special_enabled_flag\n", f->specials[i]->enabled,
                i);
    }
    fprintf(file, "    },\n");
    fprintf(file, "    Skills{\n");
    for (int i = 0; i < FIGHTER_SKILL_SLOTS; i++) {
        fprintf(file, "%i# %i_skill_enabled_flag\n", f->skills[i]->enabled,
                i);
    }
    fprintf(file, "    },\n");
    fprintf(file, "    Counters{\n");
    for (int i = 0; i < COUNTERSMAX + 1; i++) {
        fprintf(file, "%i# innervalue\n", f->counters[i]->innerValue);
        fprintf(file, "%i# count\n", f->counters[i]->count);
    }
    fprintf(file, "    },\n");
    fprintf(file, "%i# tb_atk\n", f->turnboost_atk);
    fprintf(file, "%i# tb_def\n", f->turnboost_def);
    fprintf(file, "%i# tb_vel\n", f->turnboost_vel);
    fprintf(file, "%i# tb_enr\n", f->turnboost_enr);
    fprintf(file, "%i# eqpbag_occupied_slots\n", f->equipsBagOccupiedSlots);
    fprintf(file, "%i# earliestbagslot\n", f->earliestBagSlot);
    fprintf(file, "    EquipsBag{\n");
    for (int i = 0; i < f->equipsBagOccupiedSlots; i++) {
        Equip *eq = f->equipsBag[i];
        fprintf(file, "        Equip_%i{\n", i);

        fprintf(file, "%s# equipclass\n", stringFromEquips(eq->class));
        fprintf(file, "%s# equipzone\n", stringFromEquipzones(eq->type));
        fprintf(file, "%i# qty\n", eq->qty);
        fprintf(file, "%i# equipped\n", eq->equipped);
        fprintf(file, "%i# lvl\n", eq->level);
        fprintf(file, "%i# atk\n", eq->atk);
        fprintf(file, "%i# def\n", eq->def);
        fprintf(file, "%i# vel\n", eq->vel);
        fprintf(file, "%i# enr\n", eq->enr);
        fprintf(file, "%i# bonus\n", eq->bonus);
        fprintf(file, "%i# perkscount\n", eq->perksCount);
        fprintf(file, "%i# cost\n", eq->cost);
        fprintf(file, "%s# quality\n", stringFromQuality(eq->type));
        fprintf(file, "            Perks{\n");
        for (int j = 0; j < eq->perksCount; j++) {
            Perk *pk = eq->perks[j];
            fprintf(file, "%i# class\n", pk->class);
            fprintf(file, "%i# innervalue\n", pk->innerValue);
        }
        fprintf(file, "            }\n");
        fprintf(file, "        }\n");
    }
    fprintf(file, "    }\n");
    fprintf(file, "%i# pbst_atk\n", f->permboost_atk);
    fprintf(file, "%i# pbst_def\n", f->permboost_def);
    fprintf(file, "%i# pbst_vel\n", f->permboost_vel);
    fprintf(file, "%i# pbst_enr\n", f->permboost_enr);
    fprintf(file, "%i# ebst_atk\n", f->equipboost_atk);
    fprintf(file, "%i# ebst_atk\n", f->equipboost_def);
    fprintf(file, "%i# ebst_atk\n", f->equipboost_vel);
    fprintf(file, "%i# ebst_atk\n", f->equipboost_enr);
    fprintf(file, "    Stats{\n");
    fprintf(file, "%i# enemieskilled\n", f->stats->enemieskilled);
    fprintf(file, "%i# consumablesfound\n", f->stats->consumablesfound);
    fprintf(file, "%i# equipsfound\n", f->stats->equipsfound);
    fprintf(file, "%i# artifactsfound\n", f->stats->artifactsfound);
    fprintf(file, "%i# crits\n", f->stats->criticalhits);
    fprintf(file, "%i# roomsdone\n", f->stats->roomscompleted);
    fprintf(file, "%i# specialsunlocked\n", f->stats->specialsunlocked);
    fprintf(file, "%i# coinsfound\n", f->stats->coinsfound);
    fprintf(file, "%i# bosseskilled\n", f->stats->bosseskilled);
    fprintf(file, "%i# uniquebosseskilled\n", f->stats->unique_bosseskilled);
    fprintf(file, "        Bosses_killed{\n");
    for (int b_i = 0; b_i < BOSSCLASSESMAX + 1; b_i++) {
        fprintf(file, "%i# boss_%iwas_%s_killed\n",
                f->stats->killed_bosses[b_i], b_i,
                f->stats->killed_bosses[b_i] ? "" : "not");
    }
    fprintf(file, "        },\n");
    fprintf(file, "%i# keysfound\n", f->stats->keysfound);
    fprintf(file, "    },\n");
    fprintf(file, "},\n");

    /* Path and current room indexes printing */
    fprintf(file, "Path{\n");
    fprintf(file, "%i# luck\n", p->luck);
    fprintf(file, "%i# length\n", p->length);
    fprintf(file, "%i# prize\n", p->prize);
    fprintf(file, "%i# lorecounter\n", (p->loreCounter) - 1);	//We want to see the last message again when the game is loaded. This might need investigation. TODO
    fprintf(file, "    Wincon{\n");
    fprintf(file, "%i# winconclass\n", p->win_condition->class);
    fprintf(file, "%i# current_val\n", p->win_condition->current_val);
    fprintf(file, "%i# target_val\n", p->win_condition->target_val);
    fprintf(file, "    },\n");
    fprintf(file, "},\n");
    fprintf(file, "%i# roomindex\n", roomIndex);

    /* Consumables bag printing */
    fprintf(file, "ConsumableBag{\n");
    for (int i = 0; i < CONSUMABLESMAX + 1; i++) {
        Consumable *cn = (Consumable *) f->consumablesBag[i];
        fprintf(file, "%i# qty_for_consumable_%i\n", cn->qty, i);
    }
    fprintf(file, "},\n");

    /* Artifacts bag printing */
    fprintf(file, "ArtifactsBag{\n");
    for (int i = 0; i < ARTIFACTSMAX + 1; i++) {
        Artifact *a = f->artifactsBag[i];
        fprintf(file, "%i# qty_for_artifact_%i\n", a->qty, i);
    }
    fprintf(file, "},\n");

    log_tag("debug_log.txt", "[DEBUG]", "Done saving");

    return OP_OK;
}

/**
 * Takes a FILE pointer to which the gamestate from the passed args will be saved.
 * Fighter, Path and Enemy pointers, and integers for room / path progression indexes.
 * Saves the state of the game.
 * @see Fighter
 * @see Enemy
 * @see Path
 * @see room()
 * @see gameloop()
 * @param file The FILE pointer struct to write to.
 * @param f The player struct.
 * @param p The path struct.
 * @param e The current enemy struct.
 * @param enemyIndex Index in current room for current enemy.
 * @param roomTotalEnemies Total enemies for current room.
 * @param roomIndex Current room index.
 */
OP_res handleSave_Enemies(FILE *file, Fighter *f, Path *p, Enemy *e,
                          int enemyIndex, int roomTotalEnemies, int roomIndex)
{
    const char * version = HELAPORDO_SAVEFILE_VERSION;
    //FILE *file = fopen("save.txt", "w");
    log_tag("debug_log.txt", "[DEBUG]", "Saving with version %s", version);

    if (file == NULL) {
        fprintf(stderr, "[ERROR]    Can't open save file!\n");
        return OP_CANNOT_OPEN_SAVEFILE;
    }

    /* File version printing */
    fprintf(file, "%s\n", version);

    /* Save type printing */
    fprintf(file, "%s\n", stringFrom_saveType(ENEMIES_SAVE));

    /* Gamemode printing */
    fprintf(file, "%i# gamemode\n", GAMEMODE);

    /* Fighter printing */
    fprintf(file, "Fighter{\n");
    fprintf(file, "%s# name\n", f->name);
    fprintf(file, "%s# class\n", stringFromClass(f->class));
    fprintf(file, "%i# hp\n", f->hp);
    fprintf(file, "%i# totalhp\n", f->totalhp);
    fprintf(file, "%i# atk\n", f->atk);
    fprintf(file, "%i# def\n", f->def);
    fprintf(file, "%i# vel\n", f->vel);
    fprintf(file, "%i# level\n", f->level);
    fprintf(file, "%i# luck\n", f->luck);
    fprintf(file, "%i# totxp\n", f->totalxp);
    fprintf(file, "%i# currlevxp\n", f->currentlevelxp);
    fprintf(file, "%i# totlevxp\n", f->totallevelxp);
    fprintf(file, "%i# energy\n", f->energy);
    fprintf(file, "%i# totenergy\n", f->totalenergy);
    fprintf(file, "%i# stamina\n", f->stamina);
    fprintf(file, "%i# totstamina\n", f->totalstamina);
    fprintf(file, "%i# coinbalance\n", f->balance);
    fprintf(file, "%s# status\n", stringFromStatus(f->status));
    fprintf(file, "    Specials{\n");
    for (int i = 0; i < SPECIALSMAX + 1; i++) {
        fprintf(file, "%i# %i_special_enabled_flag\n", f->specials[i]->enabled,
                i);
    }
    fprintf(file, "    },\n");
    fprintf(file, "    Skills{\n");
    for (int i = 0; i < FIGHTER_SKILL_SLOTS; i++) {
        fprintf(file, "%i# %i_skill_enabled_flag\n", f->skills[i]->enabled,
                i);
    }
    fprintf(file, "    },\n");
    fprintf(file, "    Counters{\n");
    for (int i = 0; i < COUNTERSMAX + 1; i++) {
        fprintf(file, "%i# innervalue\n", f->counters[i]->innerValue);
        fprintf(file, "%i# count\n", f->counters[i]->count);
    }
    fprintf(file, "    },\n");
    fprintf(file, "%i# tb_atk\n", f->turnboost_atk);
    fprintf(file, "%i# tb_def\n", f->turnboost_def);
    fprintf(file, "%i# tb_vel\n", f->turnboost_vel);
    fprintf(file, "%i# tb_enr\n", f->turnboost_enr);
    fprintf(file, "%i# eqpbag_occupied_slots\n", f->equipsBagOccupiedSlots);
    fprintf(file, "%i# earliestbagslot\n", f->earliestBagSlot);
    fprintf(file, "    EquipsBag{\n");
    for (int i = 0; i < f->equipsBagOccupiedSlots; i++) {
        Equip *eq = f->equipsBag[i];
        fprintf(file, "        Equip_%i{\n", i);

        fprintf(file, "%s# equipclass\n", stringFromEquips(eq->class));
        fprintf(file, "%s# equipzone\n", stringFromEquipzones(eq->type));
        fprintf(file, "%i# qty\n", eq->qty);
        fprintf(file, "%i# equipped\n", eq->equipped);
        fprintf(file, "%i# lvl\n", eq->level);
        fprintf(file, "%i# atk\n", eq->atk);
        fprintf(file, "%i# def\n", eq->def);
        fprintf(file, "%i# vel\n", eq->vel);
        fprintf(file, "%i# enr\n", eq->enr);
        fprintf(file, "%i# bonus\n", eq->bonus);
        fprintf(file, "%i# perkscount\n", eq->perksCount);
        fprintf(file, "%i# cost\n", eq->cost);
        fprintf(file, "%s# quality\n", stringFromQuality(eq->type));
        fprintf(file, "            Perks{\n");
        for (int j = 0; j < eq->perksCount; j++) {
            Perk *pk = eq->perks[j];
            fprintf(file, "%i# class\n", pk->class);
            fprintf(file, "%i# innervalue\n", pk->innerValue);
        }
        fprintf(file, "            }\n");
        fprintf(file, "        }\n");
    }
    fprintf(file, "    }\n");
    fprintf(file, "%i# pbst_atk\n", f->permboost_atk);
    fprintf(file, "%i# pbst_def\n", f->permboost_def);
    fprintf(file, "%i# pbst_vel\n", f->permboost_vel);
    fprintf(file, "%i# pbst_enr\n", f->permboost_enr);
    fprintf(file, "%i# ebst_atk\n", f->equipboost_atk);
    fprintf(file, "%i# ebst_atk\n", f->equipboost_def);
    fprintf(file, "%i# ebst_atk\n", f->equipboost_vel);
    fprintf(file, "%i# ebst_atk\n", f->equipboost_enr);
    fprintf(file, "    Stats{\n");
    fprintf(file, "%i# enemieskilled\n", f->stats->enemieskilled);
    fprintf(file, "%i# consumablesfound\n", f->stats->consumablesfound);
    fprintf(file, "%i# equipsfound\n", f->stats->equipsfound);
    fprintf(file, "%i# artifactsfound\n", f->stats->artifactsfound);
    fprintf(file, "%i# crits\n", f->stats->criticalhits);
    fprintf(file, "%i# roomsdone\n", f->stats->roomscompleted);
    fprintf(file, "%i# specialsunlocked\n", f->stats->specialsunlocked);
    fprintf(file, "%i# coinsfound\n", f->stats->coinsfound);
    fprintf(file, "%i# bosseskilled\n", f->stats->bosseskilled);
    fprintf(file, "%i# uniquebosseskilled\n", f->stats->unique_bosseskilled);
    fprintf(file, "        Bosses_killed{\n");
    for (int b_i = 0; b_i < BOSSCLASSESMAX + 1; b_i++) {
        fprintf(file, "%i# boss_%iwas_%s_killed\n",
                f->stats->killed_bosses[b_i], b_i,
                f->stats->killed_bosses[b_i] ? "" : "not");
    }
    fprintf(file, "        },\n");
    fprintf(file, "%i# keysfound\n", f->stats->keysfound);
    fprintf(file, "    },\n");
    fprintf(file, "},\n");

    /* Current enemy printing */
    fprintf(file, "Enemy{\n");
    fprintf(file, "%s# class\n", stringFromEClass(e->class));
    fprintf(file, "%i# hp\n", e->hp);
    fprintf(file, "%i# totalhp\n", e->totalhp);
    fprintf(file, "%i# atk\n", e->atk);
    fprintf(file, "%i# def\n", e->def);
    fprintf(file, "%i# vel\n", e->vel);
    fprintf(file, "%i# level\n", e->level);
    fprintf(file, "%i# xp\n", e->xp);
    fprintf(file, "%i# energy\n", e->energy);
    fprintf(file, "%i# totenergy\n", e->totalenergy);
    fprintf(file, "%i# stamina\n", e->stamina);
    fprintf(file, "%i# totstamina\n", e->totalstamina);
    fprintf(file, "%i# beast\n", e->beast);
    fprintf(file, "%i# prize\n", e->prize);
    fprintf(file, "%s# status\n", stringFromStatus(e->status));
    fprintf(file, "    Skills{\n");
    for (int i = 0; i < ENEMY_SKILL_SLOTS; i++) {
        fprintf(file, "%i# %i_skill_enabled_flag\n", e->skills[i]->enabled,
                i);
    }
    fprintf(file, "    },\n");
    fprintf(file, "    Counters{\n");
    for (int i = 0; i < COUNTERSMAX + 1; i++) {
        fprintf(file, "%i# innervalue\n", e->counters[i]->innerValue);
        fprintf(file, "%i# count\n", e->counters[i]->count);
    }
    fprintf(file, "    },\n");
    fprintf(file, "},\n");

    /* Path and current room indexes printing */
    fprintf(file, "Path{\n");
    fprintf(file, "%i# luck\n", p->luck);
    fprintf(file, "%i# length\n", p->length);
    fprintf(file, "%i# prize\n", p->prize);
    fprintf(file, "%i# lorecounter\n", (p->loreCounter) - 1);	//We want to see the last message again when the game is loaded. This might need investigation. TODO
    fprintf(file, "    Wincon{\n");
    fprintf(file, "%i# winconclass\n", p->win_condition->class);
    fprintf(file, "%i# current_val\n", p->win_condition->current_val);
    fprintf(file, "%i# target_val\n", p->win_condition->target_val);
    fprintf(file, "    },\n");
    fprintf(file, "},\n");
    fprintf(file, "%i# enemyindex\n", enemyIndex);
    fprintf(file, "%i# roomtotalenemies\n", roomTotalEnemies);
    fprintf(file, "%i# roomindex\n", roomIndex);

    /* Consumables bag printing */
    fprintf(file, "ConsumableBag{\n");
    for (int i = 0; i < CONSUMABLESMAX + 1; i++) {
        Consumable *cn = (Consumable *) f->consumablesBag[i];
        fprintf(file, "%i# qty_for_consumable_%i\n", cn->qty, i);
    }
    fprintf(file, "},\n");

    /* Artifacts bag printing */
    fprintf(file, "ArtifactsBag{\n");
    for (int i = 0; i < ARTIFACTSMAX + 1; i++) {
        Artifact *a = f->artifactsBag[i];
        fprintf(file, "%i# qty_for_artifact_%i\n", a->qty, i);
    }
    fprintf(file, "},\n");

    log_tag("debug_log.txt", "[DEBUG]", "Done saving");

    return OP_OK;
}

/**
 * Takes a FILE pointer from which the save type will be loaded.
 * Returns the read savetype value or -1 if an invalid value is found.
 * @see saveType
 * @see gameloop()
 * @param file The FILE pointer to load from.
 * @return The saveType read from the file.
 */
saveType read_saveType(FILE *file)
{
    log_tag("debug_log.txt", "[DEBUG]", "Reading save type from text file.");

    if (file == NULL) {
#ifdef HELAPORDO_CURSES_BUILD
        endwin();
#endif // HELAPORDO_CURSES_BUILD
        printf("Error with file while trying to read save type!\n");
        return -1;
    }
    char buf[500];
    const char* version = HELAPORDO_SAVEFILE_VERSION;

    int scanres = -1;
    /* File version scanning */
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in read_saveType(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while reading save type. Quitting.\n");
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    int check = -1;
    if (!((check = strcmp(buf, version)) == 0)) {
        log_tag("debug_log.txt", "[LOAD-ERROR]",
                "Failed save format version check. Quitting.");
        fprintf(stderr,
                "[ERROR]    File version mismatch on save type reading.\n");
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    };
    log_tag("debug_log.txt", "[LOAD]", "Read save format version: (%s).\n",
            buf);

    /* Save type scanning */
    scanres = fscanf(file, "%200s\n", buf);
    if (scanres != 1) {
        log_tag("debug_log.txt", "[DEBUG]",
                "Bad fscanf() result in read_saveType(), expected [%i] was (%i)",
                1, scanres);
        fprintf(stderr, "Error while reading save type.");
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    saveType result = saveTypeFrom_string(buf);

    if (result == -1) {
        log_tag("debug_log.txt", "[LOAD-ERROR]",
                "read_saveType():  Failed save type reading. Read string Was [%s]. Quitting.",
                buf);
        fprintf(stderr, "[ERROR]    Save type version error.\n");
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);

    };

    log_tag("debug_log.txt", "[LOAD]", "Loaded save type: (%s) --> [%s].\n",
            buf, stringFrom_saveType(result));

    log_tag("debug_log.txt", "[LOAD]",
            "Doing rewind(save_file) before returning from read_saveType()!");
    rewind(file);
    return result;
}
