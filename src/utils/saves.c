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
            ser_gmst.player.name[49] = '\0';
            memcpy(buf, ser_gmst.player.name, 50);
        }
    }
    buf[49] = '\0';
    memcpy(sv->name, buf, 50);
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
    return NO_OP;
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

    return NO_OP;
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
    return NO_OP;
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
    return NO_OP;
}
