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
#include "helapordo_win.h"

/**
 * Takes a integer and a string array (possibly from main).
 * @param argc The number of argv values + 1 (0 is program name?).
 * @param argv Array of strings with argc - 1 values.
 */
void gameloop_Win(int argc, char **argv)
{
    char *whoami;
    char path_to_kls_debug_file[600];
    char static_path[500];
    // Set static_path value to the correct static dir path
    resolve_staticPath(static_path);

    //Truncate "debug_log.txt"
    sprintf(path_to_kls_debug_file, "%s\\%s", static_path, "kls_debug_log.txt");

#ifdef KOLISEO_HAS_REGION
    KLS_RegList_Alloc_Backend reglist_backend = KLS_REGLIST_ALLOC_KLS_BASIC;
#else
    int reglist_backend = -1;
#endif

    KLS_Conf default_kls_conf = kls_conf_init(
                                    1, //kls_autoset_regions
                                    reglist_backend, //kls_reglist_alloc_backend
                                    KLS_DEFAULT_SIZE*16, //kls_reglist_kls_size
                                    1, //kls_autoset_temp_regions
                                    1, //collect_stats
                                    1, //kls_verbose_lvl
                                    NULL, //kls_log_fp
                                    path_to_kls_debug_file //.kls_log_filepath
                                );
    KLS_Conf temporary_kls_conf = kls_conf_init(
                                      1, //kls_autoset_regions
                                      reglist_backend, //kls_reglist_alloc_backend
                                      KLS_DEFAULT_SIZE*16, //kls_reglist_kls_size
                                      1, //kls_autoset_temp_regions
                                      1, //collect_stats
                                      0, //kls_verbose_lvl
                                      stderr, //kls_log_fp
                                      NULL
                                  );

    bool is_localexe = ( argv[0][0] == '.');
    (whoami = strrchr(argv[0], '\\')) ? ++whoami : (whoami = argv[0]);
    int seed = -1;
    do {
        default_kls = kls_new_conf(KLS_DEFAULT_SIZE * 8, default_kls_conf);
        temporary_kls = kls_new_conf(KLS_DEFAULT_SIZE * 8, temporary_kls_conf);
        char *kls_progname =
            (char *)KLS_PUSH_ARR_TYPED(default_kls, char, strlen(whoami),
                                       KLS_None, "progname", whoami);
        strcpy(kls_progname, whoami);
#ifndef HELAPORDO_DEBUG_LOG
#else
        FILE *debug_file = NULL;
        FILE *OPS_debug_file = NULL;
#endif
        // Parse command-line options
        int option;
        loadInfo *load_info =
            (loadInfo *) KLS_PUSH_TYPED(default_kls, loadInfo, HR_loadInfo,
                                        "loadInfo", "loadInfo");

        load_info->is_new_game = 1;	//By default we do a new game
        load_info->enemy_index = -1;
        load_info->total_foes = -1;
        load_info->save_type = -1;
        int loaded_roomtotalenemies = -1;
        int loaded_roomindex = -1;
        load_info->ptr_to_roomtotalenemies = &loaded_roomtotalenemies;
        load_info->ptr_to_roomindex = &loaded_roomindex;

        seed = rand();

        while ((option = getopt(argc, argv, "r:E:tTGRXQLlvdhsaV")) != -1) {
            switch (option) {
            case 'd': {
#ifndef HELAPORDO_DEBUG_ACCESS
#else
                G_DEBUG_ON += 1;
                G_LOG_ON = 1;
#endif
            }
            break;
            case 'r': {
                G_DEBUG_ROOMTYPE_ON += 1;
            }
            break;
            case 'E': {
                G_DEBUG_ENEMYTYPE_ON += 1;
            }
            break;
            case 'L': {
                G_LOG_ON = 1;
            }
            break;
            break;
            case 'G': {
                G_GODMODE_ON = 1;
            }
            break;
            case 'Q': {
                G_FASTQUIT_ON = 1;
            }
            break;
            case 'X': {
                G_EXPERIMENTAL_ON = 1;
            }
            break;
            case 'a': {
                GS_AUTOSAVE_ON = 0;
            }
            break;
            case 's': {
                GAMEMODE = Story;
            }
            break;
            case 'R': {
                GAMEMODE = Rogue;
            }
            break;
            break;
            case 'h': {
                usage(whoami);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            }
            break;
            case 'T': {
                G_DOTUTORIAL_ON = 1;
                handleTutorial();
                usage(whoami);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            }
            break;
            case 't': {
                //Test all colors
                printFormattedVersion(whoami);
                printf("Using:\n");
                printf("  \'animate\' :\n    s4c/animate.h    ");
                S4C_ECHOVERSION();
                printf("[DEBUG]    Testing terminal color capabilities.\n");
                napms(800);
                //TODO Win term color test?
                //display_colorpairs();
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            }
            break;
            case 'V': {
                printf("helapordo build: %s\n", helapordo_build_string);
                hlpd_dbg_features();
                printf("  using: s4c-animate v%s\n", S4C_ANIMATE_VERSION);
                s4c_dbg_features();
                printf("  using: koliseo v%s\n", string_koliseo_version());
                kls_dbg_features();
                printf("  using: ncurses v%s\n", NCURSES_VERSION);
#ifdef ANVIL__helapordo__
#ifndef INVIL__helapordo__HEADER__
                printf("  Built with: amboso v%s\n",
                       ANVIL__API_LEVEL__STRING);
#else
                printf("  Built with: invil v%s\n",
                       INVIL__VERSION__STRING);
                printf("Version Info: %.8s\n",
                       get_ANVIL__VERSION__DESC__());
                printf("Last commit: %s", get_INVIL__COMMIT__DESC__());
                const char* anvil_date = get_ANVIL__VERSION__DATE__();
                char* anvil_date_end;
#ifndef _WIN32
                time_t anvil_build_time = strtol(anvil_date, &anvil_date_end, 10);
#else
                time_t anvil_build_time = strtoll(anvil_date, &anvil_date_end, 10);
#endif //_WIN32

                if (anvil_date_end == anvil_date) {
                    //TODO: error
                } else {
                    char build_time_buff[20] = {0};
                    struct tm* build_time_tm = localtime(&anvil_build_time);

                    if (build_time_tm == NULL) {
                        //TODO: error
                    } else {
                        strftime(build_time_buff, 20, "%Y-%m-%d %H:%M:%S", build_time_tm);
                        printf("\nDate: %s\n", build_time_buff);
                    }
                }
#endif // INVIL__helapordo__HEADER__
#else
                printf("  Built without anvil\n");
#endif // ANVIL__helapordo__
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            }
            break;
            case 'v': {
                printVersion();
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            }
            case '?': {
                fprintf(stderr,
                        "Invalid option: %c\n Check your arguments.\n",
                        option);
                usage(whoami);
                // Handle invalid options
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            default: {
                // Should never get here
                fprintf(stderr, "Invalid option: %c\n, bad usage.\n",
                        option);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            }
        }

#ifndef HELAPORDO_DEBUG_LOG
        //TODO use is_localexe
        if (is_localexe) {
        }
#else
        // Open log file if log flag is set and reset it
        if (G_LOG_ON == 1) {
            char path_to_debug_file[600];
            char path_to_OPS_debug_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Truncate "debug_log.txt"
            sprintf(path_to_debug_file, "%s\\%s", static_path, "debug_log.txt");
            debug_file = fopen(path_to_debug_file, "w");
            if (!debug_file) {
                endwin();	//TODO: Can/should we check if we have to do this only in curses mode?
                fprintf(stderr,
                        "[ERROR]    Can't open debug logfile (%s\\debug_log.txt).\n",
                        static_path);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            fprintf(debug_file, "[DEBUGLOG]    --New game--  \n");
            fprintf(debug_file, "[DEBUG]    --Default kls debug info:--  \n");
            print_kls_2file(debug_file, default_kls);
            fprintf(debug_file, "[DEBUG]    --Temporary kls debug info:--  \n");
            print_kls_2file(debug_file, temporary_kls);
            fprintf(debug_file,
                    "[DEBUG]    --Closing header for new game.--  \n");
            fclose(debug_file);

            //Lay debug info
            log_tag("debug_log.txt", "[DEBUG]", "G_DEBUG_ON == (%i)",
                    G_DEBUG_ON);
            //FIXME we should have same behaviour as gameloop(), and actually log kls_progname...
            //Doesn't matter for now, kls_progname is declared later
            log_tag("debug_log.txt", "[DEBUG]", "whoami == (%s)", whoami);
            log_tag("debug_log.txt", "[DEBUG]", "is_localexe == (%s)", (is_localexe ? "true" : "false"));
            log_tag("debug_log.txt", "[DEBUG]", "G_LOG_ON == (%i)", G_LOG_ON);
            log_tag("debug_log.txt", "[DEBUG]", "small DEBUG FLAG ASSERTED");
            log_tag("debug_log.txt", "[DEBUG]",
                    "[Current position in default_kls] [pos: %lli]\n",
                    kls_get_pos(default_kls));

            //Truncate OPS_LOGFILE
            sprintf(path_to_OPS_debug_file, "%s\\%s", static_path, OPS_LOGFILE);
            OPS_debug_file = fopen(path_to_OPS_debug_file, "w");
            if (!OPS_debug_file) {
                endwin();	//TODO: Can/should we check if we have to do this only in curses mode?
                fprintf(stderr, "[ERROR]    Can't open OPS logfile (%s\\%s).\n",
                        static_path, OPS_LOGFILE);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            fprintf(OPS_debug_file, "[OPLOG]    --New game--  \n");
            fclose(OPS_debug_file);
            log_tag("debug_log.txt", "[DEBUG]", "Truncated [%s]", OPS_LOGFILE);
            log_Win_EnvVars();
            log_tag("debug_log.txt", "[WIN32-DEBUG]", "Printing title.");
        }
#endif
        printTitle();
        printf("\n\n\n\n\t\t\t\tSTART\n\n");
        if (G_DEBUG_ON) {
            printf("\t\t\t\t\t\t\t\tDEBUG ON\n");
        }
        printf("\t\t\t\t\t\t");
        printFormattedVersion(whoami);
        printf("\n\nThe Windows build of \"helapordo\" is very much WIP.\n\n");
        printf("\n  Press Enter to proceed.\n");
        scanf("%*c");
        system("cls");

        if (G_EXPERIMENTAL_ON == 1) {
            bool did_init = false;
            SaveHeader* current_saveHeader = prep_saveHeader(static_path, default_kls, false, &did_init);

            log_tag("debug_log.txt", "[DEBUG]", "Loaded Save Header version {%s}\n", current_saveHeader->game_version);
        }

        printGlobVars();
        printWin_EnvVars();
        printf("\n\n  Press Enter to demo a minimal rogue floor.\n");
        printf("  Quit with Ctrl+C, or explore enough of the map.\n\n");
        printf("  You move with the arrow keys.\n\n");
        scanf("%*c");
        test_floors();
        kls_free(temporary_kls);
        kls_free(default_kls);
    } while (retry(seed));

    //TODO
    //What is this?
    printf("\n\n\t\tTHANKS 4 PLAYING!\n\n");
    log_tag("debug_log.txt", "[DEBUG]", "End of program.");
    kls_free(default_kls);
    kls_free(temporary_kls);
    exit(0);
}
