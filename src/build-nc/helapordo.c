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
#include "helapordo.h"

//TODO Drop dead code
/*
 * Takes an integer, a callback_void_t pointer function and a Fighter holding the array for the callback registration.
 * Not working as of v0.5.2.
 * Registers the pointer to the function pointer array for counter callback.
 * @see Turncounter
 * @param index An integer.
 * @param ptr A pointer to function of type callback_void_t.
 * @param f The fighter pointer holding the callback array.
 *
void register_counter_callback(int index, callback_void_t ptr, Fighter* f) {
	f->callback_counter_ptrs[index] = ptr;
}
*/

/**
 * Takes a integer and a string array (possibly from main).
 * Initialises a Path pointer and a Fighter pointer, before looping for each oom in path length by calling the correct room function.
 * If all the rooms are cleared, prints a victory message and exits the program.
 * Then getParams() is called to initialise a Fighter pointer's name and class fields.
 * Takes an integer seed and frees path pointer.
 * Notably, player pointer is not freed here.
 * @see Path
 * @see Fighter
 * @see getParams()
 * @see initPlayerStats()
 * @see room()
 * @see handleStats()
 * @see printStats()
 * @param argc The number of argv values + 1 (0 is program name?).
 * @param argv Array of strings with argc - 1 values.
 */
void gameloop(int argc, char **argv)
{

    char *whoami;		// This will reference argv[0] at basename, it's the same string in memory, just starting later
    char path_to_kls_debug_file[600];
    char static_path[500];
    // Set static_path value to the correct static dir path
    resolve_staticPath(static_path);

    //Truncate "debug_log.txt"
    sprintf(path_to_kls_debug_file, "%s/%s", static_path, "kls_debug_log.txt");

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

    int seed = -1;

    do {
        //Init default_kls
        default_kls = kls_new_conf(KLS_DEFAULT_SIZE * 256, default_kls_conf);
        temporary_kls = kls_new_conf(KLS_DEFAULT_SIZE * 32, temporary_kls_conf);
        seed = rand();

#ifndef _WIN32
        (whoami = strrchr(argv[0], '/')) ? ++whoami : (whoami = argv[0]);
#else
        (whoami = strrchr(argv[0], '\\')) ? ++whoami : (whoami = argv[0]);
#endif
        bool is_localexe = ( argv[0][0] == '.');

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

        while ((option = getopt(argc, argv, "f:r:E:tTGRXQLlvdhsaV")) != -1) {
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
                G_DEBUG_ROOMTYPE_ARG = optarg;
            }
            break;
            case 'E': {
                G_DEBUG_ENEMYTYPE_ON += 1;
                G_DEBUG_ENEMYTYPE_ARG = optarg;
            }
            break;
            case 'L': {
                G_LOG_ON = 1;
            }
            break;
            case 'l': {
                load_info->is_new_game = 0;
            }
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
            case 'f': {
                //filename = optarg;
            }
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
                napms(200);
                display_colorpairs();
                napms(200);
                WINDOW *test_win;
                initscr();
                start_color();
                for (int i = 0; i < PALETTE_S4C_H_TOTCOLORS; i++) {
                    init_s4c_color_pair(&palette[i], 9 + i);
                }
                clear();
                refresh();
                cbreak();
                noecho();
                test_win = newwin(9, 7, 1, 1);
                keypad(test_win, TRUE);
                box(test_win, 0, 0);

                refresh();

                test_game_color_pairs(test_win, 5);

                napms(200);
                delwin(test_win);
                endwin();
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
                printf("Last commit: %s", get_INVIL__COMMIT__DESC__());
#endif // INVIL__helapordo__HEADER__
                printf("Version Info: %.8s\n",
                       get_ANVIL__VERSION__DESC__());
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
                const char* headergen_date = get_ANVIL__HEADER__GENTIME__();
                char* headergen_date_end;
#ifndef _WIN32
                time_t headergen_time = strtol(headergen_date, &headergen_date_end, 10);
#else
                time_t headergen_time = strtoll(headergen_date, &headergen_date_end, 10);
#endif //_WIN32

                if (headergen_date_end == headergen_date) {
                    //TODO: error
                } else {
                    char headergen_time_buff[20] = {0};
                    struct tm* headergen_time_tm = localtime(&headergen_time);

                    if (headergen_time_tm == NULL) {
                        //TODO: error
                    } else {
                        strftime(headergen_time_buff, 20, "%Y-%m-%d %H:%M:%S", headergen_time_tm);
                        printf("Anvil Gen Date: %s\n", headergen_time_buff);
                    }
                }
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
                /*
                   printf("Using:\n");
                   printf("  \'animate\' :\n    s4c/animate.h    ");
                   S4C_ECHOVERSION();
                   printf("\n  \'anvil\' :\n");
                   int status = system("echo \"    $( anvil -vv 2>/dev/null ) \"");
                   int exitcode = status / 256;
                   if (exitcode != 0) {
                   printf("\033[1;31m[DEBUG]\e[0m    \"anvil -vv\" failed.\n\n    Maybe amboso is not installed globally?\n");
                   exit(exitcode);
                   }
                   exit(exitcode);
                 */
#if 0
#ifdef HELAPORDO_DEBUG_ACCESS
                printf("\nSeed: [%i]\n", seed);
#endif
#endif
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            }
            break;
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
#else
        // Open log file if log flag is set and reset it
        if (G_LOG_ON == 1) {
            char path_to_debug_file[600];
            char path_to_OPS_debug_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Truncate "debug_log.txt"
            sprintf(path_to_debug_file, "%s/%s", static_path, "debug_log.txt");
            debug_file = fopen(path_to_debug_file, "w");
            if (!debug_file) {
                endwin();	//TODO: Can/should we check if we have to do this only in curses mode?
                fprintf(stderr,
                        "[ERROR]    Can't open debug logfile (%s/debug_log.txt).\n",
                        static_path);
                exit(EXIT_FAILURE);
            }
            fprintf(debug_file, "[DEBUGLOG]    --New game--  \n");
            if (NCURSES_VERSION_MAJOR < EXPECTED_NCURSES_VERSION_MAJOR
                && NCURSES_VERSION_MINOR < EXPECTED_NCURSES_VERSION_MINOR
                && NCURSES_VERSION_PATCH < EXPECTED_NCURSES_VERSION_PATCH) {
                fprintf(debug_file,
                        "[WARN]    ncurses version is lower than expected {%s: %i.%i.%i} < {%i.%i.%i}\n",
                        NCURSES_VERSION, NCURSES_VERSION_MAJOR,
                        NCURSES_VERSION_MINOR, NCURSES_VERSION_PATCH,
                        EXPECTED_NCURSES_VERSION_MAJOR,
                        EXPECTED_NCURSES_VERSION_MINOR,
                        EXPECTED_NCURSES_VERSION_PATCH);
            }
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
            log_tag("debug_log.txt", "[DEBUG]", "kls_progname == (%s)",
                    kls_progname);
            log_tag("debug_log.txt", "[DEBUG]", "is_localexe == (%s)", (is_localexe ? "true" : "false"));
            log_tag("debug_log.txt", "[DEBUG]", "G_LOG_ON == (%i)", G_LOG_ON);
            log_tag("debug_log.txt", "[DEBUG]", "small DEBUG FLAG ASSERTED");
            log_tag("debug_log.txt", "[DEBUG]",
                    "[Current position in default_kls] [pos: %li]\n",
                    kls_get_pos(default_kls));

            //Truncate OPS_LOGFILE
            sprintf(path_to_OPS_debug_file, "%s/%s", static_path, OPS_LOGFILE);
            OPS_debug_file = fopen(path_to_OPS_debug_file, "w");
            if (!OPS_debug_file) {
                endwin();	//TODO: Can/should we check if we have to do this only in curses mode?
                fprintf(stderr, "[ERROR]    Can't open OPS logfile (%s/%s).\n",
                        static_path, OPS_LOGFILE);
                exit(EXIT_FAILURE);
            }
            fprintf(OPS_debug_file, "[OPLOG]    --New game--  \n");
            fclose(OPS_debug_file);
            log_tag("debug_log.txt", "[DEBUG]", "Truncated [%s]", OPS_LOGFILE);
        }
#endif

        if (G_DEBUG_ENEMYTYPE_ON == 1) {
            log_tag("debug_log.txt", "[DEBUG]", "G_DEBUG_ENEMYTYPE_ON == (%i)",
                    G_DEBUG_ENEMYTYPE_ON);
            log_tag("debug_log.txt", "[DEBUG]", "ENEMY DEBUG FLAG ASSERTED");
            if ((G_DEBUG_ON > 0)) {
                G_DEBUG_ON += 1;
                log_tag("debug_log.txt", "[DEBUG]", "G_DEBUG_ON == (%i)",
                        G_DEBUG_ON);
                log_tag("debug_log.txt", "[DEBUG]", "Forcing enemy type: (%s)",
                        G_DEBUG_ENEMYTYPE_ARG);
                int setenemy_debug = 0;
                for (int ec = 0; ec < ENEMYCLASSESMAX && (setenemy_debug == 0);
                     ec++) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Checking optarg for -E: (%s)",
                            stringFromEClass(ec));
                    if ((strcmp(G_DEBUG_ENEMYTYPE_ARG, stringFromEClass(ec)) ==
                         0)) {
                        log_tag("debug_log.txt", "[DEBUG]",
                                "Match on optarg (%s), setting G_DEBUG_ENEMYTYPE to (%i).",
                                stringFromEClass(ec), ec);
                        G_DEBUG_ENEMYTYPE = ec;
                        setenemy_debug = 1;
                    }
                }
                if (setenemy_debug == 0) {
                    log_tag("debug_log.txt", "[ERROR]",
                            "Invalid optarg for -E flag: {%s}.\n",
                            G_DEBUG_ENEMYTYPE_ARG);
                    fprintf(stderr,
                            "[ERROR]    Incorrect -E \"enemyType\" arg: {%s}.\n",
                            G_DEBUG_ENEMYTYPE_ARG);
                    fprintf(stderr, "[ERROR]    Run \"%s -h\" for help.\n",
                            kls_progname);
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                };
            }
        }
        if (G_DEBUG_ROOMTYPE_ON == 1) {
            log_tag("debug_log.txt", "[DEBUG]", "G_DEBUG_ROOMTYPE_ON == (%i)",
                    G_DEBUG_ROOMTYPE_ON);
            log_tag("debug_log.txt", "[DEBUG]", "ROOM DEBUG FLAG ASSERTED");
            if ((G_DEBUG_ON > 0)) {
                G_DEBUG_ON += 1;
                log_tag("debug_log.txt", "[DEBUG]", "G_DEBUG_ON == (%i)",
                        G_DEBUG_ON);
                log_tag("debug_log.txt", "[DEBUG]",
                        "Forcing room type: optarg was (%s)",
                        G_DEBUG_ROOMTYPE_ARG);
                int setroom_debug = 0;
                for (int rc = 0;
                     (rc < ROOM_CLASS_MAX + 1) && (setroom_debug == 0); rc++) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Checking optarg (%s) for -R: (%s)", optarg,
                            stringFromRoom(rc));
                    if ((strcmp(G_DEBUG_ROOMTYPE_ARG, stringFromRoom(rc)) == 0)) {
                        log_tag("debug_log.txt", "[DEBUG]",
                                "Match on optarg (%s), setting G_DEBUG_ROOMTYPE to (%i).",
                                stringFromRoom(rc), rc);
                        G_DEBUG_ROOMTYPE = rc;
                        setroom_debug = 1;
                    }
                }
                if (setroom_debug == 0) {
                    log_tag("debug_log.txt", "[ERROR]",
                            "Invalid optarg for -R flag: {%s}.",
                            G_DEBUG_ROOMTYPE_ARG);
                    fprintf(stderr,
                            "[ERROR]    Incorrect -R \"roomType\" arg: {%s}.\n",
                            G_DEBUG_ROOMTYPE_ARG);
                    fprintf(stderr, "[ERROR]    Run \"%s -h\" for help.\n",
                            kls_progname);
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                };
            }

        }
        log_tag("debug_log.txt", "[DEBUG]", "Done getopt.");

        // Clear screen and print title, wait for user to press enter
        int clearres = system("clear");
        log_tag("debug_log.txt", "[DEBUG]",
                "gameloop() system(\"clear\") res was (%i)", clearres);
        printTitle();
        char c;
        yellow();
        printf("\n\n\n\n\t\t\tPRESS ENTER TO START\n\n");
        white();

        if (G_DEBUG_ON) {
            lightCyan();
            printf("\t\t\t\t\t\t\t\tDEBUG ON\n");
            white();
        }
        printf("\t\t\t\t\t\t\tncurses build\n");
        printf("\t\t\t\t\t\t");
        printFormattedVersion(whoami);
        int scanfres = scanf("%c", &c);
        log_tag("debug_log.txt", "[DEBUG]", "gameloop() scanf() res was (%i)",
                scanfres);

        // Parse positional arguments
        //for (int i = optind; i < argc; i++) {
        // Handle positional arguments
        Path *path = NULL;
        Fighter *player = NULL;

        clock_t start_time = clock(), diff_time;

        // Prepare the fighter frames
        char fighter_sprites[CLASSESMAX + 1][MAXFRAMES][MAXROWS][MAXCOLS];

        char static_path[500];

        // Set static_path value to the correct static dir path
        resolve_staticPath(static_path);

        /*
         * Legacy code for loading animations from an s4c-file.
         char fighter_filename[600];
         FILE* fighter_sprite_file;
         */
        for (int i = 0; i < CLASSESMAX + 1; i++) {

            int n_load_frames = 60;
            int n_load_rows = 17;
            int n_load_cols = 17;

            switch (i) {
            case Knight: {
                s4c_copy_animation(knight_tapis, fighter_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Mage: {
                s4c_copy_animation(mage_spark, fighter_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Archer: {
                s4c_copy_animation(archer_drop, fighter_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Assassin: {
                s4c_copy_animation(assassin_poof, fighter_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected fighterclass index while loading animation for class (%i): [%s]",
                        i, stringFromClass(i));
                exit(EXIT_FAILURE);
            }
            break;
            }

            /*
             * Legacy code for loading animations from an s4c-file
             fighter_sprite_file = fopen(fighter_filename, "r");
             if (!fighter_sprite_file) {
             fprintf(stderr,"Error opening animation file at (%s).\n",fighter_filename);
             fprintf(stderr,"Static path was (%s).\n",static_path);
             exit(EXIT_FAILURE);
             }
             int fighter_loadCheck = load_sprites(fighter_sprites[i], fighter_sprite_file, 17, 17);
             sprintf(load_msg,"Loaded animation for %s, load result was %i.", stringFromClass(i), fighter_loadCheck);
             log_tag("debug_log.txt","[PREP]",load_msg);
             */

            log_tag("debug_log.txt", "[PREP]",
                    "Copied animation from default matrix vector for: [%s] with dimensions: [%i][%i][%i].",
                    stringFromClass(i), n_load_frames, n_load_rows,
                    n_load_cols);

            //Massive log of all loaded lines
            /*
               for (int k=0; k<MAXFRAMES+1; k++) {
               sprintf(load_msg,"[DEBUG]    frame %i:", k);
               debug_log("debug_log.txt",load_msg);
               for (int z=0; z<MAXROWS+1;z++) {
               sprintf(load_msg,"[DEBUG] %i  ( %s ).", z, fighter_sprites[i][k][z]);
               debug_log("debug_log.txt",load_msg);
               }
               }
             */
        }

        //Try preloading all enemy animations
        char enemy_sprites[ENEMYCLASSESMAX + 1][MAXFRAMES][MAXROWS][MAXCOLS];
        for (int i = 0; i < (ENEMYCLASSESMAX + 1); i++) {
            /* Legacy code to load animations from an s4c-file
               char static_path[500];

               // Set static_path value to the correct static dir path
               resolve_staticPath(static_path);

               char enemy_filename[600];
               FILE* enemy_sprite_file;
             */

            int n_load_frames = 60;
            int n_load_rows = 17;
            int n_load_cols = 17;
            // Prepare the enemy frames
            switch (i) {
            case Imp: {
                s4c_copy_animation(imp_fireball, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Zombie: {
                s4c_copy_animation(zombie_walk, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Troll: {
                s4c_copy_animation(troll_club, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Ghost: {
                s4c_copy_animation(ghost_spell, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Boar: {
                s4c_copy_animation(boar_scream, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Mummy: {
                s4c_copy_animation(mummy_shuffle, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Goblin: {
                s4c_copy_animation(goblin_shoot, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Werewolf: {
                s4c_copy_animation(werewolf_transform, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected enemy index while loading animation for enemy (%i): [%s]",
                        i, stringFromEClass(i));
                exit(EXIT_FAILURE);
            }
            }

            /* Legacy code loading the animations from an s4c-file
               enemy_sprite_file = fopen(enemy_filename, "r");
               if (!enemy_sprite_file) {
               fprintf(stderr,"Error opening animation file at (%s/%s).\n",static_path,enemy_filename);
               exit(EXIT_FAILURE);
               }
               int enemy_loadCheck = load_sprites(enemy_sprites[i], enemy_sprite_file, 17, 17);
               sprintf(load_msg,"Loaded animation for %s, load result was %i.", stringFromEClass(i), enemy_loadCheck);
               log_tag("debug_log.txt","[PREP]",load_msg);
             */

            log_tag("debug_log.txt", "[PREP]",
                    "Copied animation from default matrix vector for: [%s] with dimensions: [%i][%i][%i].",
                    stringFromEClass(i), n_load_frames, n_load_rows,
                    n_load_cols);

            //Massive log of all loaded lines
            /*
               for (int k=0; k<MAXFRAMES+1; k++) {
               sprintf(load_msg,"[DEBUG]    frame %i:", k);
               debug_log("debug_log.txt",load_msg);
               for (int z=0; z<MAXROWS+1;z++) {
               sprintf(load_msg,"[DEBUG] %i  ( %s ).", z, enemy_sprites[i][k][z]);
               debug_log("debug_log.txt",load_msg);
               }
               }
             */
        }
        //Try preloading all boss animations
        char boss_sprites[BOSSCLASSESMAX + 1][MAXFRAMES][MAXROWS][MAXCOLS];
        for (int i = 0; i < (BOSSCLASSESMAX + 1); i++) {
            int n_load_frames = 60;
            int n_load_rows = 17;
            int n_load_cols = 17;

            /*
             * Legacy code for loading animations from an s4c-file.
             char static_path[500];

             // Set static_path value to the correct static dir path
             resolve_staticPath(static_path);

             char boss_filename[600];
             FILE* boss_sprite_file;
             */

            // Prepare the boss frames
            switch (i) {
            case Blue_Troll: {
                s4c_copy_animation(bluetroll_wonder, boss_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Headless_Ninja: {
                s4c_copy_animation(headlessninja_throw, boss_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Crawling_Dude: {
                s4c_copy_animation(crawlingdude_crawl, boss_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Sr_Warthog: {
                s4c_copy_animation(srwarthog_square, boss_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Doppelganger: {
                s4c_copy_animation(knight_tapis, boss_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected boss index while loading animation for boss (%i): [%s]",
                        i, stringFromBossClass(i));
                exit(EXIT_FAILURE);
            }
            }
            /*
             * Legacy code for loading animations from an s4c-file.
             boss_sprite_file = fopen(boss_filename, "r");
             if (!boss_sprite_file) {
             fprintf(stderr,"Error opening animation file at (%s/%s).\n",static_path,boss_filename);
             exit(EXIT_FAILURE);
             }
             int boss_loadCheck = load_sprites(boss_sprites[i], boss_sprite_file, 17, 17);
             sprintf(load_msg,"Loaded animation for %s, load result was %i.", stringFromBossClass(i), boss_loadCheck);
             log_tag("debug_log.txt","[PREP]",load_msg);
             */

            log_tag("debug_log.txt", "[PREP]",
                    "Copied animation from default matrix vector for: [%s] with dimensions: [%i][%i][%i].",
                    stringFromBossClass(i), n_load_frames, n_load_rows,
                    n_load_cols);

            //Massive log of all loaded lines
            /*
               for (int k=0; k<MAXFRAMES+1; k++) {
               sprintf(load_msg,"[DEBUG]    frame %i:", k);
               debug_log("debug_log.txt",load_msg);
               for (int z=0; z<MAXROWS+1;z++) {
               sprintf(load_msg,"[DEBUG] %i  ( %s ).", z, enemy_sprites[i][k][z]);
               debug_log("debug_log.txt",load_msg);
               }
               }
             */
        }			//End loading boss animations
        //End of loading animations
        clock_t animationload_time = clock() - start_time;
        double time_spent_loading_animations =
            (double)animationload_time / CLOCKS_PER_SEC;
        log_tag("debug_log.txt", "[DEBUG]",
                "Animation loading took %0.7f seconds.",
                time_spent_loading_animations);

        bool did_exper_init = false;
        if (G_EXPERIMENTAL_ON == 1) {
            SaveHeader* current_saveHeader = prep_saveHeader(static_path, default_kls, false, &did_exper_init);

            log_tag("debug_log.txt", "[DEBUG]", "Loaded Save Header version {%s}\n", current_saveHeader->game_version);
        }

        WINDOW* screen = initscr();
        bool screen_is_big_enough = false;
        int screen_rows = 0;
        int screen_cols = 0;
        //TODO: factor this out to ensure the check is done for on any gien screen draw.
        while (!screen_is_big_enough) {
            getmaxyx(screen, screen_rows, screen_cols);
            if (screen_rows >= HLPD_MIN_SCREEN_ROWS && screen_cols >= HLPD_MIN_SCREEN_COLS) {
                screen_is_big_enough = true;
                clear();
                refresh();
            } else if (screen_rows >= HLPD_DEFAULT_SCREEN_ROWS && screen_cols >= HLPD_MIN_SCREEN_COLS) {
                mvwprintw(screen, 0, 0, "%s", "Current screen is too small to see battle notifications.");
                mvwprintw(screen, 1, 0, "%s", "Enlarge vertically to fit it.");
                refresh();
            } else {
                mvwprintw(screen, 0, 0, "%s", "Screen too small, please resize.");
                refresh();
            }
        }
        GameScreen* gamescreen = (GameScreen*) KLS_PUSH_TYPED(default_kls, GameScreen, HR_Gamescreen, "Main GameScreen", "Wrap stdscr");
        gamescreen->colors = COLORS;
        gamescreen->color_pairs = COLOR_PAIRS;
        gamescreen->cols = COLS;
        gamescreen->rows = LINES;
        gamescreen->escape_delay = ESCDELAY;
        gamescreen->tabsize = TABSIZE;

        gamescreen->win = screen;
        ITEM **savepick_items;
        MENU *savepick_menu;
        WINDOW *savepick_menu_win;
        WINDOW *savepick_side_win;
        char current_save_path[300];	//Will hold picked path

        Koliseo_Temp *savepick_kls = kls_temp_start(temporary_kls);

        //Declare turnOP_args
        Room *fakeroom = NULL;
        Enemy *fakeenemy = NULL;
        Boss *fakeboss = NULL;
        FILE *fakesavefile = NULL;
        WINDOW *fakenotifywin = NULL;
        Gamestate *fakegmst = NULL;
        foeTurnOption_OP fake_foe_op = FOE_OP_INVALID;
        skillType fake_skill = -1;
        turnOP_args *savepick_turn_args =
            init_turnOP_args(fakegmst, player, path, fakeroom, load_info,
                             fakeenemy, fakeboss, fakesavefile, fakenotifywin,
                             savepick_kls, fake_foe_op, fake_skill);
        char *savepick_choices[] = {
            "New game",
            "Load save",
            "Tutorial",
            "Quit",
            (char *)NULL,
        };
        int savepick_n_choices = ARRAY_SIZE(savepick_choices);
        //FIXME: remove magic numbers
        turnOption savepick_choice = 999;

        /* Create menu items */
        savepick_items = (ITEM **) calloc(savepick_n_choices, sizeof(ITEM *));
        for (int i = 0; i < savepick_n_choices; i++) {
            savepick_items[i] =
                new_item(savepick_choices[i], savepick_choices[i]);
        }
        savepick_items[savepick_n_choices - 1] = (ITEM *) NULL;

        /* Create menu */
        savepick_menu = new_menu((ITEM **) savepick_items);

        /* Set description off */
        menu_opts_off(savepick_menu, O_SHOWDESC);

        /* Create the window to be associated with the menu */
        savepick_menu_win = newwin(11, 16, 5, 35);
        keypad(savepick_menu_win, TRUE);

        /* Set main window and sub window */
        set_menu_win(savepick_menu, savepick_menu_win);
        set_menu_sub(savepick_menu, derwin(savepick_menu_win, 4, 14, 4, 1));
        set_menu_format(savepick_menu, 4, 1);

        /* Set menu mark to the string " >  " */
        set_menu_mark(savepick_menu, " >  ");

        /* Print a border around main menu window */
        box(savepick_menu_win, 0, 0);
        print_label(savepick_menu_win, 1, 0, 16, "Select save", COLOR_PAIR(6));
        mvwaddch(savepick_menu_win, 2, 0, ACS_LTEE);
        mvwhline(savepick_menu_win, 2, 1, ACS_HLINE, 16);
        mvwaddch(savepick_menu_win, 2, 15, ACS_RTEE);

        /* Post the menu */
        post_menu(savepick_menu);
        wrefresh(savepick_menu_win);

        //Handle side window for welcome info
        savepick_side_win = newwin(12, 32, 2, 2);
        scrollok(savepick_side_win, TRUE);
        wprintw(savepick_side_win, "  \nhelapordo");
        wprintw(savepick_side_win, "  \n  build: %s", helapordo_build_string);
        wprintw(savepick_side_win, "  \n  using: s4c-animate v%s",
                S4C_ANIMATE_VERSION);
        wprintw(savepick_side_win, "  \n  using: koliseo v%s",
                KOLISEO_API_VERSION_STRING);
        wprintw(savepick_side_win, "  \n  using: ncurses v%s", NCURSES_VERSION);
#ifdef ANVIL__helapordo__
#ifndef INVIL__helapordo__HEADER__
        wprintw(savepick_side_win, "  \nBuilt with: amboso v%s",
                ANVIL__API_LEVEL__STRING);
#else
        wprintw(savepick_side_win, "  \nBuilt with: invil v%s",
                INVIL__VERSION__STRING);
        wprintw(savepick_side_win, "  \nVersion Info: %.8s",
                get_ANVIL__VERSION__DESC__());
        const char* anvil_date = get_ANVIL__VERSION__DATE__();
        char* anvil_date_end;
#ifndef _WIN32
        time_t anvil_build_time = strtol(anvil_date, &anvil_date_end, 10);
#else
        time_t anvil_build_time = strtoll(anvil_date, &anvil_date_end, 10);
#endif //_WIN32

        if (anvil_date_end == anvil_date) {
            log_tag("debug_log.txt", "ERROR", "anvil date was invalid");
        } else {
            char build_time_buff[20] = {0};
            struct tm* build_time_tm = localtime(&anvil_build_time);

            if (build_time_tm == NULL) {
                log_tag("debug_log.txt", "ERROR", "localtime() failed");
            } else {
                strftime(build_time_buff, 20, "%Y-%m-%d %H:%M:%S", build_time_tm);
                wprintw(savepick_side_win, "  \nDate: %s", build_time_buff);
            }
        }
#endif // INVIL__helapordo__HEADER__
#else
        wprintw(savepick_side_win, "  \nBuilt without anvil");
#endif // ANVIL__helapordo__
        //wprintw(savepick_side_win,"  \n  %s",get_ANVIL__VERSION__DESC__());
        wrefresh(savepick_side_win);
        refresh();

        int savepick_picked = 0;

        /*
           //We set the colors to use s4c's palette file...
           FILE* palette_file;
           char path_to_palette[600];
           char palette_name[50] = "palette.gpl";
         */
        int pickchar = -1;

        /*
           // Set static_path value to the correct static dir path
           resolve_staticPath(static_path);

           sprintf(path_to_palette,"%s/%s",static_path,palette_name);

           palette_file = fopen(path_to_palette, "r");

           init_s4c_color_pairs(palette_file);
         */

        for (int i = 0; i < PALETTE_S4C_H_TOTCOLORS; i++) {
            init_s4c_color_pair(&palette[i], 9 + i);
        }
        log_tag("debug_log.txt","[DEBUG]","%s():    Updating gamescreen->colors and colorpairs after init_s4c_color_pair() loop.", __func__);
        gamescreen->colors = COLORS;
        gamescreen->color_pairs = COLOR_PAIRS;

        while (!savepick_picked
               && (pickchar = wgetch(savepick_menu_win)) != KEY_F(1)) {
            switch (pickchar) {
            case KEY_DOWN: {
                menu_driver(savepick_menu, REQ_DOWN_ITEM);
            }
            break;
            case KEY_UP: {
                menu_driver(savepick_menu, REQ_UP_ITEM);
            }
            break;
            case KEY_LEFT: {	/*Left option pick */
                ITEM *cur;
                cur = current_item(savepick_menu);
                savepick_choice = getTurnChoice((char *)item_name(cur));
                log_tag("debug_log.txt", "[DEBUG]",
                        "Left on choice: [ %s ] value (%i)", item_name(cur),
                        savepick_choice);
                if (savepick_choice == NEW_GAME) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Should do something");
                }
            }
            break;
            case KEY_RIGHT: {	/*Right option pick */
                ITEM *cur;
                cur = current_item(savepick_menu);
                savepick_choice = getTurnChoice((char *)item_name(cur));
                log_tag("debug_log.txt", "[DEBUG]",
                        "Right on choice: [ %s ] value (%i)",
                        item_name(cur), savepick_choice);
                if (savepick_choice == NEW_GAME) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Should do something");
                }
            }
            break;
            case KEY_NPAGE: {
                menu_driver(savepick_menu, REQ_SCR_DPAGE);
            }
            break;
            case KEY_PPAGE: {
                menu_driver(savepick_menu, REQ_SCR_UPAGE);
            }
            break;
            case 10: {	/* Enter */
                savepick_picked = 1;
                ITEM *cur;

                //move(18,47);
                //clrtoeol();
                cur = current_item(savepick_menu);
                //mvprintw(18, 47, "Item selected is : %s", item_name(cur));
                savepick_choice = getTurnChoice((char *)item_name(cur));
                pos_menu_cursor(savepick_menu);
                refresh();
            }
            break;
            case 'q': {
                log_tag("debug_log.txt", "[DEBUG]",
                        "Player used q to quit from savepick menu.");
                //TODO: take some variable to disable quick quitting with q
                savepick_picked = 1;
                savepick_choice = getTurnChoice("Quit");
                pos_menu_cursor(savepick_menu);
                refresh();
            }
            break;
            default: {
                break;
            }
            }
            wrefresh(savepick_menu_win);
            if (savepick_choice == NEW_GAME) {
                int picked_saveslot_index = get_saveslot_index();
                log_tag("debug_log.txt", "[DEBUG]",
                        "Saveslot index picked: [%i]", picked_saveslot_index);
                sprintf(current_save_path, "%s", default_saveslots[picked_saveslot_index].save_path);	//Update saveslot_path value
                //TODO
                //Get picked_slot with a curses menu.
                //int picked_slot = handle_pickSave();
                //sprintf(current_save_path,default_saveslots[picked_slot].save_path);
                //TODO
                //By default we expect the user to press new game, no action needed?
                log_tag("debug_log.txt", "[DEBUG]",
                        "Running new game from savepick menu");
                turnOP(OP_NEW_GAME, savepick_turn_args, default_kls,
                       savepick_kls);
            } else if (savepick_choice == LOAD_GAME) {
                int picked_saveslot_index = get_saveslot_index();
                log_tag("debug_log.txt", "[DEBUG]",
                        "Saveslot index picked: [%i]", picked_saveslot_index);
                sprintf(current_save_path, "%s", default_saveslots[picked_saveslot_index].save_path);	//Update saveslot_path value
                //TODO
                //Get picked_slot with a curses menu.
                //int picked_slot = handle_pickSave();
                //sprintf(current_save_path,default_saveslots[picked_slot].save_path);
                //ATM we expect a single save.
                //Setting this to 0 is the only thing we expect here, the actual load is done later.
                load_info->is_new_game = 0;
                log_tag("debug_log.txt", "[DEBUG]",
                        "Set load value: load_info->is_new_game == (%i)",
                        load_info->is_new_game);
                turnOP(OP_LOAD_GAME, savepick_turn_args, default_kls,
                       savepick_kls);
                //TODO
                //Select which game to load, by preparing the necessary handles to code below (correct savefile/name, for now)
            } else if (savepick_choice == QUIT) {
                //TODO
                //We can quit, I guess.
                log_tag("debug_log.txt", "[DEBUG]",
                        "Savepick menu: doing exit(%i)", EXIT_SUCCESS);
                // Unpost menu and free all the memory taken up
                unpost_menu(savepick_menu);
                free_menu(savepick_menu);
                log_tag("debug_log.txt", "[FREE]", "Freed savepick menu");
                for (int k = 0; k < savepick_n_choices; k++) {
                    free_item(savepick_items[k]);
                    log_tag("debug_log.txt", "[FREE]",
                            "Freed %i savepick menu item", k);
                }

                delwin(savepick_menu_win);
                endwin();
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            } else if (savepick_choice == TUTORIAL) {
                log_tag("debug_log.txt", "[DEBUG]", "Doing tutorial.");
                handleTutorial();
                exit(EXIT_SUCCESS);
            }
        }			//End while !savepick_picked

        //Free turnOP_args
        //free(savepick_turn_args);

        // Unpost menu and free all the memory taken up
        unpost_menu(savepick_menu);
        free_menu(savepick_menu);
        log_tag("debug_log.txt", "[FREE]", "Freed savepick menu");
        for (int k = 0; k < savepick_n_choices; k++) {
            free_item(savepick_items[k]);
            log_tag("debug_log.txt", "[FREE]", "Freed %i savepick menu item",
                    k);
        }

        delwin(savepick_menu_win);
        endwin();
        log_tag("debug_log.txt", "[DEBUG]",
                "Ended window mode for savepick menu");

        kls_temp_end(savepick_kls);

        //Flush the terminal
        int clrres = system("clear");
        log_tag("debug_log.txt", "[DEBUG]",
                "gameloop() system(\"clear\") after savepick res was (%i)",
                clrres);

        //By now, we expect load_info->is_new_game to be set to 0 or 1.
        log_tag("debug_log.txt", "[DEBUG]",
                "  Checking is_new_game:  load_info->is_new_game == (%i)",
                load_info->is_new_game);

        Koliseo_Temp *gamestate_kls = kls_temp_start(temporary_kls);

        Gamestate* gamestate = NULL;

        if (load_info->is_new_game) {	// We prepare path and fighter
            path = randomise_path(seed, default_kls, current_save_path);
            path->loreCounter = -1;

            kls_log(default_kls, "DEBUG", "Prepping Fighter");
            player =
                (Fighter *) KLS_PUSH_TYPED(default_kls, Fighter, HR_Fighter,
                                           "Fighter", "Fighter");

            int optTot = optind;

            getParams(argc, argv, player, path, optTot, default_kls);
            initPlayerStats(player, path, default_kls);
        } else {		//Handle loading of gamestate

            if (G_EXPERIMENTAL_ON == 1) { //Bin load
                log_tag("debug_log.txt", "[DEBUG]", "%s():    TODO bin load", __func__);
                log_tag("debug_log.txt", "[TURNOP]",
                        "Old seed: [%i]", seed);
                seed = rand();
                log_tag("debug_log.txt", "[TURNOP]",
                        "New seed: [%i]", seed);
                path = randomise_path(seed, default_kls, current_save_path);
                kls_log(default_kls, "DEBUG", "Prepping Loady Fighter");
                player =
                    (Fighter *) KLS_PUSH_TYPED(default_kls, Fighter, HR_Fighter,
                                               "Fighter", "Loady Fighter");
                player->class = Knight;

                strcpy(player->name, "Loady");

                kls_log(default_kls, "DEBUG", "Prepping Loady Wincon");
                Wincon *w =
                    (Wincon *) KLS_PUSH_TYPED(default_kls, Wincon, HR_Wincon,
                                              "Wincon", "Loady Wincon");
                w->class = FULL_PATH;
                initWincon(w, path, w->class);
                initPlayerStats(player, path, default_kls);
                path->win_condition = w;
                char static_path[500];
                // Set static_path value to the correct static dir path
                resolve_staticPath(static_path);

                gamestate =
                    KLS_PUSH_TYPED(default_kls, Gamestate, HR_Gamestate, "Gamestate",
                                   "Gamestate");
                init_Gamestate(gamestate, start_time, player->stats, path->win_condition, path,
                               player, GAMEMODE, gamescreen, is_localexe);

                bool prep_res = prep_Gamestate(gamestate, static_path, sizeof(int64_t) + sizeof(SerSaveHeader), default_kls, did_exper_init); //+ (idx* (sizeof(int64_t) + sizeof(SerGamestate))) , default_kls);
                if (prep_res) {
                    log_tag("debug_log.txt", "[DEBUG]", "Done prep_Gamestate().");
                } else {
                    log_tag("debug_log.txt", "[ERROR]", "Failed prep_Gamesate().");
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                }

                load_info->enemy_index = gamestate->current_enemy_index;
                seed = gamestate->path->seed;
                //TODO: set the other load_info fields properly?
                load_info->done_loading = 1;
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Set load_info->done_loading to 1.", __func__);
            } else {
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Doing text file loading", __func__);
                //Declar turnOP_args
                Room *fakeroom = NULL;
                Enemy *fakeenemy = NULL;
                Boss *fakeboss = NULL;
                FILE *fakesavefile = NULL;
                WINDOW *fakenotifywin = NULL;
                Gamestate *fakegmst = NULL;
                foeTurnOption_OP fake_foe_op = FOE_OP_INVALID;
                skillType fake_skill = -1;
                turnOP_args *loading_room_turn_args =
                    init_turnOP_args(fakegmst, player, path, fakeroom, load_info,
                                     fakeenemy, fakeboss, fakesavefile,
                                     fakenotifywin, gamestate_kls, fake_foe_op,
                                     fake_skill);
                FILE *save_file;
                char path_to_savefile[1000];
                char static_path[500];
                char savefile_name[300];

                //Copy current_save_path
                sprintf(savefile_name, "%s", current_save_path);

                // Set static_path value to the correct static dir path
                resolve_staticPath(static_path);

                sprintf(path_to_savefile, "%s/%s", static_path, savefile_name);

                save_file = fopen(path_to_savefile, "r");
                if (!save_file) {
                    //User error
                    fprintf(stderr,
                            "[ERROR]    Can't open savefile for loading game.\n");
                    fprintf(stderr, "[ERROR]    Expected at path [%s].\n",
                            path_to_savefile);
                    //Debug error
                    log_tag("debug_log.txt", "[ERROR]",
                            "Could not load savefile at (%s)", path_to_savefile);
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                }
                //Update loading_room_turn_args->save_file pointer
                loading_room_turn_args->save_file = save_file;
                log_tag("debug_log.txt", "[TURNOP]",
                        "Assigned loading_room_turn_args->save_file: path [%s]",
                        path_to_savefile);

                //Read save type
                //
                saveType loaded_save_type = -1;
                loaded_save_type = read_saveType(save_file);
                log_tag("debug_log.txt", "[TURNOP]", "Read saveType, was [%s].",
                        stringFrom_saveType(loaded_save_type));

                if (loaded_save_type == -1) {
                    log_tag("debug_log.txt", "[ERROR]",
                            "Failed setting loaded_save_type. Quitting.");
                    fprintf(stderr, "[ERROR]    Failed setting a save type.");

                    exit(EXIT_FAILURE);
                }

                load_info->save_type = loaded_save_type;

                log_tag("debug_log.txt", "[TURNOP]",
                        "Assigned load_info->save_type: [%s]",
                        stringFrom_saveType(load_info->save_type));
                log_tag("debug_log.txt", "[TURNOP]",
                        "Old seed: [%i]", seed);
                seed = rand();
                log_tag("debug_log.txt", "[TURNOP]",
                        "New seed: [%i]", seed);
                path = randomise_path(seed, default_kls, current_save_path);
                kls_log(default_kls, "DEBUG", "Prepping Loady Fighter");
                player =
                    (Fighter *) KLS_PUSH_TYPED(default_kls, Fighter, HR_Fighter,
                                               "Fighter", "Loady Fighter");
                player->class = Knight;

                strcpy(player->name, "Loady");

                //Update loading_room_turn_args->actor pointer
                loading_room_turn_args->actor = player;
                log_tag("debug_log.txt", "[TURNOP]",
                        "Assigned Fighter [%s]. loading_room_turn_args->actor->name: [%s]",
                        player->name, loading_room_turn_args->actor->name);

                kls_log(default_kls, "DEBUG", "Prepping Loady Wincon");
                Wincon *w =
                    (Wincon *) KLS_PUSH_TYPED(default_kls, Wincon, HR_Wincon,
                                              "Wincon", "Loady Wincon");
                w->class = FULL_PATH;
                initWincon(w, path, w->class);
                initPlayerStats(player, path, default_kls);
                path->win_condition = w;

                if (load_info->save_type == ENEMIES_SAVE) {

                    kls_log(default_kls, "DEBUG", "Prepping Loady Enemy");
                    load_info->loaded_enemy =
                        (Enemy *) KLS_PUSH_TYPED(default_kls, Enemy, HR_Enemy,
                                                 "Enemy", "Loaded Enemy");
                    //FIXME: the structs related to loaded enemy are not loaded on default_kls
                    prepareRoomEnemy(load_info->loaded_enemy, 1, 3, 1,
                                     gamestate_kls);

                    //Update loading_room_turn_args->enemy pointer
                    loading_room_turn_args->enemy = load_info->loaded_enemy;
                    log_tag("debug_log.txt", "[TURNOP]",
                            "Assigned load_info->loaded_enemy->class == [%s]. loading_room_turn_args->loaded_enemy->class == [%s]",
                            stringFromEClass(load_info->loaded_enemy->class),
                            stringFromEClass(loading_room_turn_args->enemy->class));
                }
                //Update loading_room_turn_args->path pointer
                loading_room_turn_args->path = path;
                log_tag("debug_log.txt", "[TURNOP]",
                        "Assigned loading_room_turn_args->path == [path] (len %i)",
                        path->length);

                switch (load_info->save_type) {
                case ENEMIES_SAVE: {
                    log_tag("debug_log.txt", "[TURNOP]",
                            "Doing OP_LOAD_ENEMYROOM.");
                    //int* loadinfo_totfoes = &(load_info->total_foes);
                    //FIXME: the structs related to loaded enemy are not loaded on default_kls
                    OP_res load_op_result =
                        turnOP(OP_LOAD_ENEMYROOM, loading_room_turn_args,
                               default_kls, gamestate_kls);
                    log_tag("debug_log.txt", "[TURNOP]",
                            "OP_LOAD_ENEMYROOM:  result was [%s].",
                            stringFrom_OP_res(load_op_result));
                    log_tag("debug_log.txt", "[FREE]",
                            "Freed loading_room_turn_args. Load result was [%s].",
                            stringFrom_OP_res(load_op_result));
                    //free(loading_room_turn_args);
                }
                break;
                case HOME_SAVE: {
                    log_tag("debug_log.txt", "[TURNOP]",
                            "Doing OP_LOAD_HOMEROOM.");
                    //int* loadinfo_totfoes = &(load_info->total_foes);
                    //FIXME: the structs related to loaded enemy are not loaded on default_kls
                    OP_res load_op_result =
                        turnOP(OP_LOAD_HOMEROOM, loading_room_turn_args,
                               default_kls, gamestate_kls);
                    log_tag("debug_log.txt", "[TURNOP]",
                            "OP_LOAD_HOMEROOM:  result was [%s].",
                            stringFrom_OP_res(load_op_result));
                    //log_tag("debug_log.txt","[FREE]","Freed loading_room_turn_args. Load result was [%s].",stringFrom_OP_res(load_op_result));
                    load_info->done_loading = 1;
                    log_tag("debug_log.txt", "[PREP]",
                            "Set load_info->done_loading to 1.");
                    //free(loading_room_turn_args);
                }
                break;
                default: {
                    log_tag("debug_log.txt", "[ERROR]",
                            "gameloop():  Unexpected save type, value was (%i).",
                            (int)load_info->save_type);
                    exit(EXIT_FAILURE);
                }
                break;
                }

                //e_death(loaded_enemy);
                //death(player);
                //exit(0)
            } // End text load else
        }

        /*
         * TODO
         * Remove me
         * Legacy code to load lores from a text file.
         for (int i=0; i<5; i++) {
         sprintf(msg,"Prepping lore (%i)",i);
         kls_log("DEBUG",msg);
         lore_strings[i] = (char*) KLS_PUSH_NAMED(default_kls, char, 300, "Lore", msg);
         }
         */

        int *loreCounter = &(path->loreCounter);
        log_tag("debug_log.txt", "[DEBUG]", "loreCounter == (%i)",
                *loreCounter);

        if (GAMEMODE == Story) {

            /*
             * TODO
             * Remove me
             * Legacy code to load lores from a text file.
             int loreKind = 0; //rand() % LORES_MAX;
             */

            if (load_info->is_new_game) {
                log_tag("debug_log.txt", "[FIXME]",
                        "loreCounter was (%i), setting it to 0.", *loreCounter);
                *loreCounter = 0;	//We must set the counter before going forward
                //FIXME:
                //loreCounter should not start from 0 again.
            }

            /*
             * TODO
             *Remove me
             *Legacy code for loading lores from a text file.
             loadLore(lore_strings,loreKind);
             */

        } else {
            log_tag("debug_log.txt", "[WARN]",
                    "GAMEMODE is not Story. Value was: (%i)", GAMEMODE);
        }

        //Set consumables sprites
        for (int i = 0; i < CONSUMABLESMAX + 1; i++) {
            setConsumableSprite((Consumable *) player->consumablesBag[i]);
            log_tag("debug_log.txt", "[PREP]", "Set sprite for %s",
                    stringFromConsumables(i));
        }
        log_tag("debug_log.txt", "[DEBUG-PREP]",
                "Done setting sprites for Consumables.");
        //Set artifact sprites
        for (int i = 0; i < ARTIFACTSMAX + 1; i++) {
            setArtifactSprite(player->artifactsBag[i]);
            log_tag("debug_log.txt", "[PREP]", "Set sprite for %s",
                    stringFromArtifacts(i));
        }
        log_tag("debug_log.txt", "[PREP]",
                "Done setting sprites for Artifacts.");
        //Set base equips sprites...
        for (int i = 0; i < EQUIPSMAX + 1; i++) {
            setEquipSprite(&equips[i]);
            log_tag("debug_log.txt", "[PREP]", "Set sprite for %s",
                    stringFromEquips(i));
        }
        log_tag("debug_log.txt", "[PREP]", "Done setting sprites for Equips.");

        if (load_info->is_new_game) {
            log_tag("debug_log.txt", "[PREP]", "New game from scratch.");
        } else {
            log_tag("debug_log.txt", "[PREP]", "New game from loading.");
        }
        log_tag("debug_log.txt", "[DEBUG]", "Name:    %s", player->name);
        log_tag("debug_log.txt", "[DEBUG]", "Class:    %s",
                stringFromClass(player->class));
        log_tag("debug_log.txt", "[DEBUG]", "Gamemode:    %s",
                stringFromGamemode(GAMEMODE));

        //purple();
        //printStats(player);
        //white();

        int roomsDone = load_info->is_new_game ? 1 : loaded_roomindex;
        OP_res res = OP_RES_NO_DMG;
        int roadFork_value = -1;	//0 may be used as a value, so

        Wincon *win_con = path->win_condition;
        log_tag("debug_log.txt", "[DEBUG]", "Wincon:    %s\n",
                stringFromWinconClass(win_con->class));

        //int refresh_artifact_wincon = 0;
        if (load_info->save_type == ENEMIES_SAVE) {
            load_info->done_loading = 0;
            log_tag("debug_log.txt", "[DEBUG-PREP]",
                    "Set load_info->done_loading to 0.");
        }
        log_tag("debug_log.txt", "[DEBUG-PREP]", "Prepping done.\n");
        log_tag("debug_log.txt", "[DEBUG]", "Starting wincon loop.\n");

        diff_time = clock() - start_time;
        int time_spent = diff_time * 1000 / CLOCKS_PER_SEC;
        //sprintf(msg,"[DEBUG]    Prep took %0.7f seconds.\n",time_spent);
        log_tag("debug_log.txt", "[DEBUG]", "Prep took %d s, %d ms.",
                time_spent / 1000, time_spent % 1000);

        if (!load_info->is_new_game && G_EXPERIMENTAL_ON == 1) {
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Not pushing a new gamestate", __func__);
        } else {
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Pushing a new gamestate", __func__);
            gamestate =
                KLS_PUSH_TYPED(default_kls, Gamestate, HR_Gamestate, "Gamestate",
                               "Gamestate");
            init_Gamestate(gamestate, start_time, player->stats, path->win_condition, path,
                           player, GAMEMODE, gamescreen, is_localexe);
        }
        if (gamestate->gamemode == Rogue) {
            //Note: different lifetime than gamestate
            //NO. The update_gamestate call is instead performed later.
            //Floor* current_floor = KLS_PUSH_T_TYPED(gamestate_kls,Floor,1,HR_Floor,"Floor","Init Curr floor");
            //NO. We pass NULL now.
            update_Gamestate(gamestate, 1, HOME, roomsDone, -1, NULL);
        } else {
            update_Gamestate(gamestate, 1, HOME, roomsDone, -1, NULL);
        }
        log_tag("debug_log.txt", "[DEBUG]", "Initialised Gamestate.");
        dbg_Gamestate(gamestate);

        if (load_info->is_new_game) {
            if (G_EXPERIMENTAL_ON == 1) {
                //TODO: make sure to delete a matching savefile for this slot?
                //
                //Note: the code below is not to be added, since it would force a new game to reload an existing save.
                /*
                bool prep_res = prep_Gamestate(gamestate, static_path, sizeof(int64_t) + sizeof(SerSaveHeader), default_kls, did_exper_init); //+ (idx* (sizeof(int64_t) + sizeof(SerGamestate))) , default_kls);
                if (prep_res) {
                    log_tag("debug_log.txt", "[DEBUG]", "Read Gamestate.");
                } else {
                    log_tag("debug_log.txt", "[ERROR]", "Failed reading Gamestate.");
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                }
                */
            }
        }

        if (GAMEMODE == Story || GAMEMODE == Standard) {

            //Loop till wincon reached

            while (win_con->current_val < win_con->target_val) {

                //Flush the terminal
                int loop_clrres = system("clear");
                log_tag("debug_log.txt", "[DEBUG]",
                        "gameloop() system(\"clear\") in wincon loop, res: (%i)",
                        loop_clrres);

                //Check if we have to update the wincon value
                if (path->win_condition->class == ALL_ARTIFACTS) {
                    path->win_condition->current_val =
                        player->stats->artifactsfound;
                    //Are we forced to do one more room?
                }

                int enemyTotal = -1;
                roomClass room_type = -1;

                if (!(load_info->is_new_game) && !(load_info->done_loading)
                    && (load_info->save_type == ENEMIES_SAVE)) {
                    enemyTotal = loaded_roomtotalenemies;
                }

                kls_log(temporary_kls, "DEBUG",
                        "Prepping Room for Story Gamemode. roomsDone=(%i)",
                        roomsDone);
                Room *current_room =
                    (Room *) KLS_PUSH_T_TYPED(gamestate_kls, Room, HR_Room,
                                              "Room", "Story Room");

                current_room->index = roomsDone;
                setRoomType(path, &roadFork_value, &room_type, roomsDone);
                log_tag("debug_log.txt", "[ROOM]",
                        "Set Room #%i type:    (%s)\n", roomsDone,
                        stringFromRoom(room_type));

                initRoom(current_room, player, roomsDone, room_type, enemyTotal,
                         load_info, gamestate_kls);
                log_tag("debug_log.txt", "[ROOM]", "Init Room #%i:    (%s)\n",
                        roomsDone, stringFromRoom(room_type));

                start_color();
                int colorCheck = has_colors();

                if (colorCheck == FALSE) {
                    fprintf(stderr, "Terminal can't use colors, abort.\n");
                    exit(S4C_ERR_TERMCOLOR);
                }

                colorCheck = can_change_color();

                if (colorCheck == FALSE) {
                    fprintf(stderr, "Terminal can't change colors, abort.\n");
                    exit(S4C_ERR_TERMCHANGECOLOR);
                }
                for (int i = 0; i < PALETTE_S4C_H_TOTCOLORS; i++) {
                    init_s4c_color_pair(&palette[i], 9 + i);
                }
                cbreak();
                noecho();
                keypad(stdscr, TRUE);

                //Check if we need to display a story prompt
                if (GAMEMODE == Story && (roomsDone == 1 || room_type == BOSS)) {
                    displayLore(lore_strings, *loreCounter);
                    (*loreCounter)++;
                }
                //Play room animation

                /*
                   FILE* palette_file;
                   char path_to_palette[600];
                   char static_path[500];
                   char palette_name[50] = "palette.gpl" ;

                   // Set static_path value to the correct static dir path
                   resolve_staticPath(static_path);

                   sprintf(path_to_palette,"%s/%s",static_path,palette_name);

                   palette_file = fopen(path_to_palette, "r");
                   if (palette_file == NULL) {
                   fprintf(stderr, "Error: could not open palette file (%s/%s).\n",static_path, palette_name);
                   exit(EXIT_FAILURE);
                   }
                 */

                WINDOW *door_win;
                //initscr();
                clear();
                refresh();

                int reps = 1;
                int frametime = 27;
                int num_frames = 60;
                int frame_height = 22;
                int frame_width = 22;
                door_win = newwin(frame_height + 1, frame_width + 1, 0, 25);

                char door_sprites[MAXFRAMES][MAXROWS][MAXCOLS];

                s4c_copy_animation(enter_door, door_sprites, num_frames,
                                   frame_height, frame_width);

                log_tag("debug_log.txt", "[PREP]",
                        "Copied animation from matrix vector for enter_door with dimensions: [%i][%i][%i].",
                        num_frames, frame_height, frame_width);

                /*
                 * TODO
                 * Remove me
                 * Legacy code for loading animation from an s4c-file.
                 *
                 // Set static_path value to the correct static dir path
                 resolve_staticPath(static_path);

                 char door_file_path[600];

                 sprintf(door_file_path,"%s/animations/enter_door.txt",static_path);

                 FILE* door_file = fopen(door_file_path,"r");
                 if (!door_file) {
                 fprintf(stderr,"[ERROR]    Can't open enter_door file.\n");
                 exit(EXIT_FAILURE);
                 }
                 int loadCheck = load_sprites(door_sprites, door_file, frame_height-1, frame_width-1);

                 // Check for possible loadCheck() errors and in this case we return early if we couldn't load
                 if (loadCheck < 0) {
                 endwin();
                 switch (loadCheck) {
                 case S4C_ERR_FILEVERSION: {
                 fprintf(stderr,"S4C_ERR_FILEVERSION : Failed file version check.\n");
                 }
                 break;
                 case S4C_ERR_LOADSPRITES: {
                 fprintf(stderr,"S4C_ERR_LOADSPRITES : Failed loading the sprites.\n");
                 }
                 break;
                 }
                 exit(loadCheck);
                 }
                 */

                // We make sure we have the background correcly set up and expect animate_sprites to refresh it
                wclear(door_win);
                wrefresh(door_win);

                int result =
                    s4c_animate_sprites_at_coords(door_sprites, door_win, reps,
                                                  frametime, num_frames,
                                                  frame_height, frame_width, 0,
                                                  0);
                log_tag("debug_log.txt", "[DEBUG]", "animate() result was (%i)",
                        result);
                wclear(door_win);
                wrefresh(door_win);
                delwin(door_win);
                endwin();

                update_Gamestate(gamestate, 1, current_room->class, roomsDone,
                                 -1, NULL);

                if (current_room->class == HOME) {
                    res =
                        handleRoom_Home(gamestate, current_room, roomsDone,
                                        path, player, load_info,
                                        fighter_sprites, default_kls,
                                        gamestate_kls);
                } else if (current_room->class == ENEMIES) {
                    res =
                        handleRoom_Enemies(gamestate, current_room, roomsDone,
                                           path, player, load_info,
                                           enemy_sprites, fighter_sprites,
                                           default_kls, gamestate_kls);
                } else if (current_room->class == SHOP) {
                    //FIXME: does shop require usage of gameloop kls?
                    res =
                        handleRoom_Shop(current_room, roomsDone, path, player,
                                        default_kls, gamestate_kls);
                } else if (current_room->class == BOSS) {
                    res =
                        handleRoom_Boss(gamestate, current_room, roomsDone,
                                        path, player, load_info, boss_sprites,
                                        fighter_sprites, default_kls,
                                        gamestate_kls);
                } else if (current_room->class == TREASURE) {
                    res =
                        handleRoom_Treasure(current_room, roomsDone, path,
                                            player, default_kls, gamestate_kls);
                } else if (current_room->class == ROADFORK) {
                    res =
                        handleRoom_Roadfork(current_room, &roadFork_value,
                                            roomsDone, path, player);
                } else {
                    log_tag("debug_log.txt", "[ERROR]",
                            "Unexpected current_room->class value: [%i] [%s]",
                            current_room->class,
                            stringFromRoom(current_room->class));
                    //freeRoom(current_room);
                    log_tag("debug_log.txt", "[ERROR]",
                            "Freed current room, quitting program.");
                    exit(EXIT_FAILURE);
                }

                if (res == OP_RES_DEATH) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Room resulted in DEATH.\n");
                    if (G_EXPERIMENTAL_ON == 1) {
                        //TODO: delete savefile?
                    }
                    //Free room memory
                    //freeRoom(current_room);
                    break;
                } else {
                    //Flush the terminal
                    int clrres = system("clear");
                    log_tag("debug_log.txt", "[DEBUG]",
                            "gameloop() system(\"clear\") res was (%i)",
                            clrres);

                    if (roadFork_value > 0) {
                        //TODO
                        //What is this?
                        lightYellow();
                        printStats(player);
                        lightGreen();
                        log_tag("debug_log.txt", "[ROADFORK?]",
                                "You completed room %i.", roomsDone);
                        white();
                    }
                    roomsDone++;

                    //Update stats
                    player->stats->roomscompleted++;

                    //Check if we need to update the win condition
                    if (win_con->class == FULL_PATH) {
                        win_con->current_val++;
                    }
                    //Free room memory
                    //freeRoom(current_room);
                    // Reset gamestate_kls
                    kls_temp_end(gamestate_kls);
                    gamestate_kls = kls_temp_start(temporary_kls);
                }
            }			// Win condition loop

            // Clear default_kls
            //kls_clear(default_kls);

            //Got out of the loop with res not being DEATH; so i won
            if (res != OP_RES_DEATH) {	//I guess player and enemy were freed already?
                int clrres = system("clear");
                log_tag("debug_log.txt", "[DEBUG]",
                        "gameloop() 2 system(\"clear\") res was (%i)", clrres);
                handleStats(player);
                printf("\n\n\tYOU WON!\n\n");
                log_tag("debug_log.txt", "[DEBUG]", "Game won.");
                //Free default kls
                kls_log(default_kls, "DEBUG", "Freeing default KLS");
                kls_free(default_kls);
                log_tag("debug_log.txt", "[DEBUG-KLS]", "Freed default KLS");

                //Free temporary kls
                kls_log(temporary_kls, "DEBUG", "Freeing temporary KLS");
                kls_free(temporary_kls);
                log_tag("debug_log.txt", "[DEBUG-KLS]", "Freed temporary KLS");
            }

            /*
               //Free lore strings if they were loaded
               if (GAMEMODE == Story) {
               for (int i=0; i<5; i++) {
               char msg[1000];
               sprintf(msg,"Freed lore string %i",i);
               log_tag("debug_log.txt","[FREE]",msg);
               sprintf(msg,"%s",lore_strings[i]);
               log_tag("debug_log.txt","[FREE]",msg);
               free(lore_strings[i]);
               }
               }
             */
            //free(path->win_condition);
            //free(path);
            log_tag("debug_log.txt", "[DEBUG]", "End of wincon loop.\n");

        } else {		//Gamemode is not Story or Standard
            log_tag("debug_log.txt", "[DEBUG]", "Gamemode was [%i]", GAMEMODE);

            if (GAMEMODE == Rogue) {
                log_tag("debug_log.txt", "[DEBUG]", "Doing a Rogue run.");
                char static_path[500];

                // Set static_path value to the correct static dir path
                resolve_staticPath(static_path);

                /**
                 * Legacy code for reading palette.gpl. Was it ever needed at runtime?
                 * TODO check which tags may use the palette.gpl at runtime (other than trying to read it pointlessly).
                 *
                char path_to_palette[600];
                FILE* palette_file = NULL;
                char palette_name[50] = "palette.gpl";
                sprintf(path_to_palette, "%s/%s", static_path, palette_name);
                palette_file = fopen(path_to_palette, "r");
                if (palette_file == NULL) {
                    fprintf(stderr,
                            "Error: could not open palette file (%s/%s).\n",
                            static_path, palette_name);
                    exit(EXIT_FAILURE);
                }
                */

                WINDOW *floor_win;
                //TODO: store if we have done initsrc() or endwin(). Am sure we can get this from ncurses with some MACRO
                //initscr();
                clear();
                refresh();
                start_color();

                int colorCheck = has_colors();

                if (colorCheck == FALSE) {
                    fprintf(stderr, "Terminal can't use colors, abort.\n");
                    exit(S4C_ERR_TERMCOLOR);
                }

                colorCheck = can_change_color();

                if (colorCheck == FALSE) {
                    fprintf(stderr, "Terminal can't change colors, abort.\n");
                    exit(S4C_ERR_TERMCHANGECOLOR);
                }
                for (int i = 0; i < PALETTE_S4C_H_TOTCOLORS; i++) {
                    init_s4c_color_pair(&palette[i], 9 + i);
                }
                cbreak();
                noecho();
                keypad(stdscr, TRUE);

                cbreak();
                noecho();
                keypad(stdscr, TRUE);

                // Create the window
                floor_win = newwin(23, 24, 1, 2);
                wclear(floor_win);
                wrefresh(floor_win);
                keypad(floor_win, TRUE);

                /* Print a border around the windows and print a title */
                box(floor_win, 0, 0);
                wrefresh(floor_win);
                refresh();
                int res = -1;
                char msg[500];

                log_tag("debug_log.txt", "[DEBUG]", "Prepping current_floor.");
                kls_log(default_kls, "DEBUG", "Prepping current_floor.");
                Floor *current_floor =
                    (Floor *) KLS_PUSH_T_TYPED(gamestate_kls, Floor,
                                               HR_Floor, "Floor", "Floor");
                update_Gamestate(gamestate, 1, HOME, roomsDone, -1,
                                 current_floor);
                // Start the random walk from the center of the dungeon
                int center_x = FLOOR_MAX_COLS / 2;
                int center_y = FLOOR_MAX_ROWS / 2;

                // Init dbg_floor
                init_floor_layout(current_floor);

                //Set center as filled
                current_floor->floor_layout[center_x][center_y] = 1;

                //Init floor rooms
                init_floor_rooms(current_floor);

                //Random walk #1
                floor_random_walk(current_floor, center_x, center_y, 100, 1);	// Perform 100 steps of random walk, reset floor_layout if needed.
                //Random walk #2
                floor_random_walk(current_floor, center_x, center_y, 100, 0);	// Perform 100 more steps of random walk, DON'T reset floor_layout if needed.

                //Set floor explored matrix
                load_floor_explored(current_floor);

                //Set room types
                floor_set_room_types(current_floor);

                int current_x = center_x;
                int current_y = center_y;

                //TODO: handle finishing all floors
                path->length = MAX_ROGUE_FLOORS;

                int floors_done = 0;

                //Loop till wincon reached

                //while (dbg_floor->explored_area*1.0 < (dbg_floor->area*0.8)) {
                while (win_con->current_val < win_con->target_val) {

                    //Check if we have to update the wincon value
                    if (path->win_condition->class == ALL_ARTIFACTS) {
                        path->win_condition->current_val =
                            player->stats->artifactsfound;
                        //Are we forced to do one more room?
                    }

                    int enemyTotal = -1;
                    roomClass room_type = -1;

                    if (!(load_info->is_new_game) && !(load_info->done_loading)
                        && (load_info->save_type == ENEMIES_SAVE)) {
                        enemyTotal = loaded_roomtotalenemies;
                    }

                    Room *current_room = NULL;

                    //Check if current room needs to be played
                    if (current_floor->roomclass_layout[current_x][current_y] !=
                        BASIC) {
                        kls_log(temporary_kls, "DEBUG",
                                "Prepping Room for Rogue Gamemode. roomsDone=(%i)",
                                roomsDone);
                        current_room =
                            (Room *) KLS_PUSH_T_TYPED(gamestate_kls, Room,
                                                      HR_Room, "Room", msg);

                        current_room->index = roomsDone;
                        //setRoomType(path, &roadFork_value, &room_type, roomsDone);

                        room_type =
                            current_floor->
                            roomclass_layout[current_x][current_y];
                        log_tag("debug_log.txt", "[ROOM]",
                                "Set Room #%i type:    (%s)\n", roomsDone,
                                stringFromRoom(room_type));

                        initRoom(current_room, player, roomsDone, room_type,
                                 enemyTotal, load_info, gamestate_kls);
                        log_tag("debug_log.txt", "[ROOM]",
                                "Init Room #%i:    (%s)\n", roomsDone,
                                stringFromRoom(room_type));

                        /*
                           //Check if we need to display a story prompt
                           if (GAMEMODE == Story && (roomsDone == 1 || room_type == BOSS)) {
                           displayLore(lore_strings,*loreCounter);
                           (*loreCounter)++;
                           }
                         */

                        //Play room animation

                        WINDOW *door_win;
                        //initscr();
                        clear();
                        refresh();
                        start_color();

                        int reps = 1;
                        int frametime = 27;
                        int num_frames = 60;
                        int frame_height = 22;
                        int frame_width = 22;
                        door_win =
                            newwin(frame_height + 1, frame_width + 1, 0, 25);

                        char door_sprites[MAXFRAMES][MAXROWS][MAXCOLS];

                        s4c_copy_animation(enter_door, door_sprites, num_frames,
                                           frame_height, frame_width);

                        log_tag("debug_log.txt", "[PREP]",
                                "Copied animation from matrix vector for enter_door with dimensions: [%i][%i][%i].",
                                num_frames, frame_height, frame_width);

                        /*
                         * TODO
                         * Remove me
                         * Legacy code for loading animation from an s4c-file.
                         *
                         // Set static_path value to the correct static dir path

                         // Set static_path value to the correct static dir path
                         resolve_staticPath(static_path);

                         char door_file_path[600];

                         sprintf(door_file_path,"%s/animations/enter_door.txt",static_path);

                         FILE* door_file = fopen(door_file_path,"r");
                         if (!door_file) {
                         fprintf(stderr,"[ERROR]    Can't open enter_door file.\n");
                         exit(EXIT_FAILURE);
                         }
                         int loadCheck = load_sprites(door_sprites, door_file, frame_height-1, frame_width-1);

                         // Check for possible loadCheck() errors and in this case we return early if we couldn't load
                         if (loadCheck < 0) {
                         endwin();
                         switch (loadCheck) {
                         case S4C_ERR_FILEVERSION: {
                         fprintf(stderr,"S4C_ERR_FILEVERSION : Failed file version check.\n");
                         }
                         break;
                         case S4C_ERR_LOADSPRITES: {
                         fprintf(stderr,"S4C_ERR_LOADSPRITES : Failed loading the sprites.\n");
                         }
                         break;
                         }
                         exit(loadCheck);
                         }
                         */

                        // We make sure we have the background correcly set up and expect animate_sprites to refresh it
                        wclear(door_win);
                        wrefresh(door_win);

                        int result =
                            s4c_animate_sprites_at_coords(door_sprites,
                                                          door_win, reps,
                                                          frametime, num_frames,
                                                          frame_height,
                                                          frame_width, 0, 0);
                        log_tag("debug_log.txt", "[DEBUG]",
                                "animate() result was (%i)", result);
                        wclear(door_win);
                        wrefresh(door_win);
                        delwin(door_win);
                        endwin();

                        update_Gamestate(gamestate, 1, current_room->class,
                                         current_room->index, -1,
                                         current_floor);

                        if (current_room->class == HOME) {
                            res =
                                handleRoom_Home(gamestate, current_room,
                                                roomsDone, path, player,
                                                load_info, fighter_sprites,
                                                default_kls, gamestate_kls);
                        } else if (current_room->class == ENEMIES) {
                            res =
                                handleRoom_Enemies(gamestate, current_room,
                                                   roomsDone, path, player,
                                                   load_info, enemy_sprites,
                                                   fighter_sprites, default_kls,
                                                   gamestate_kls);
                        } else if (current_room->class == SHOP) {
                            res =
                                handleRoom_Shop(current_room, roomsDone, path,
                                                player, default_kls,
                                                gamestate_kls);
                        } else if (current_room->class == BOSS) {
                            res =
                                handleRoom_Boss(gamestate, current_room,
                                                roomsDone, path, player,
                                                load_info, boss_sprites,
                                                fighter_sprites, default_kls,
                                                gamestate_kls);
                        } else if (current_room->class == TREASURE) {
                            res =
                                handleRoom_Treasure(current_room, roomsDone,
                                                    path, player, default_kls,
                                                    gamestate_kls);
                        } else if (current_room->class == ROADFORK) {
                            res =
                                handleRoom_Roadfork(current_room,
                                                    &roadFork_value, roomsDone,
                                                    path, player);
                        } else {
                            sprintf(msg,
                                    "Unexpected current_room->class value: [%i] [%s]",
                                    current_room->class,
                                    stringFromRoom(current_room->class));
                            log_tag("debug_log.txt", "[ERROR]", msg);
                            //freeRoom(current_room);
                            log_tag("debug_log.txt", "[ERROR]",
                                    "Freed current room, quitting program.");
                            exit(EXIT_FAILURE);
                        }

                        if (res == OP_RES_DEATH) {
                            log_tag("debug_log.txt", "[DEBUG]",
                                    "Room resulted in DEATH.");
                            if (G_EXPERIMENTAL_ON == 1) {
                                //TODO: delete savefile?
                            }
                            //Free room memory
                            //freeRoom(current_room);
                            break;
                        } else {
                            //Flush the terminal
                            int clrres = system("clear");
                            log_tag("debug_log.txt", "[DEBUG]",
                                    "gameloop() system(\"clear\") res was (%i)",
                                    clrres);

                            if (roadFork_value > 0) {
                                //lightYellow();
                                //TODO
                                //What is this?
                                printStats(player);
                                //lightGreen();
                                log_tag("debug_logx.txt", "[ROADFORK?]",
                                        "You completed room %i.", roomsDone);
                                //white();
                            }
                            roomsDone++;

                            //Update stats
                            player->stats->roomscompleted++;

                            //Free room memory
                            //freeRoom(current_room);

                            //Update floor's roomclass layout for finished rooms which should not be replayed
                            switch (current_floor->
                                    roomclass_layout[current_x][current_y]) {
                            case ENEMIES: {
                                current_floor->
                                roomclass_layout[current_x][current_y] =
                                    BASIC;
                            }
                            break;
                            case BOSS: {
                                current_floor->
                                roomclass_layout[current_x][current_y] =
                                    BASIC;
                                floors_done++;
                                player->stats->floorscompleted++;
                                log_tag("debug_log.txt", "[DEBUG]",
                                        "Floors done: [%i]", floors_done);
                                //Check if we need to update the win condition
                                if (win_con->class == FULL_PATH) {
                                    win_con->current_val++;
                                }
                                // Reset gamestate_kls
                                kls_temp_end(gamestate_kls);
                                gamestate_kls =
                                    kls_temp_start(temporary_kls);

                                current_floor =
                                    (Floor *)
                                    KLS_PUSH_T_TYPED(gamestate_kls, Floor,
                                                     HR_Floor, "Floor",
                                                     "Floor");
                                update_Gamestate(gamestate, 1, HOME,
                                                 roomsDone, -1,
                                                 current_floor);

                                //Regenerate floor
                                log_tag("debug_log.txt", "[DEBUG]",
                                        "Beaten a boss, regenerating current floor.");
                                // Init
                                init_floor_layout(current_floor);
                                //Set center as filled
                                current_floor->
                                floor_layout[center_x][center_y] = 1;
                                //Init floor rooms
                                init_floor_rooms(current_floor);
                                //Random walk #1
                                floor_random_walk(current_floor, center_x, center_y, 100, 1);	// Perform 100 steps of random walk, reset floor_layout if needed.
                                //Random walk #2
                                floor_random_walk(current_floor, center_x, center_y, 100, 0);	// Perform 100 more steps of random walk, DON'T reset floor_layout if needed.
                                //Set floor explored matrix
                                load_floor_explored(current_floor);
                                //Set room types
                                floor_set_room_types(current_floor);

                                //Center current coords
                                current_x = center_x;
                                current_y = center_y;
                                continue;	//Check win condition for loop

                            }
                            break;
                            case SHOP: {
                                current_floor->
                                roomclass_layout[current_x][current_y] =
                                    BASIC;
                            }
                            break;
                            case TREASURE: {
                                current_floor->
                                roomclass_layout[current_x][current_y] =
                                    BASIC;
                            }
                            break;
                            case HOME: {
                                //We leave it available
                                log_tag("debug_log.txt", "[DEBUG]",
                                        "Skipping reset of roomclass for HOME room");
                            }
                            break;
                            default: {
                                log_tag("debug_log.txt", "[ERROR]",
                                        "Unexpected roomclass value in Rogue loop: [%i] [%s]",
                                        current_floor->
                                        roomclass_layout[current_x]
                                        [current_y],
                                        stringFromRoom(current_floor->
                                                       roomclass_layout
                                                       [current_x]
                                                       [current_y]));
                                kls_free(default_kls);
                                kls_free(temporary_kls);
                                exit(EXIT_FAILURE);
                            }
                            break;
                            }
                        }
                    } else {
                        log_tag("debug_log.txt", "[DEBUG]",
                                "Current room class was [%s] (val: %i), not playable.",
                                stringFromRoom(current_floor->
                                               roomclass_layout[current_x]
                                               [current_y]),
                                current_floor->
                                roomclass_layout[current_x][current_y]);
                    }

                    //Draw current FOV
                    draw_floor_view(current_floor, current_x, current_y,
                                    floor_win);
                    //Take a step and update screen
                    move_update(gamestate, current_floor, &current_x,
                                &current_y, floor_win, path, player,
                                current_room, load_info, default_kls,
                                gamestate_kls);
                }		// Win condition loop

                //FIXME: do we need this?
                //kls_temp_end(gamestate_kls);
                // Clear default_kls
                //kls_clear(default_kls);

                //Got out of the loop with res not being DEATH; so i won
                if (res != OP_RES_DEATH) {	//I guess player and enemy were freed already?
                    int clrres = system("clear");
                    //TODO
                    //What is this?
                    log_tag("debug_log.txt", "[DEBUG]",
                            "gameloop() 2 system(\"clear\") res was (%i)",
                            clrres);
                    handleStats(player);
                    printf("\n\n\tYOU WON!\n\n");
                    log_tag("debug_log.txt", "[DEBUG]", "Game won.");
                    //Free default kls
                    kls_log(default_kls, "DEBUG", "Freeing default KLS");
                    kls_free(default_kls);
                    log_tag("debug_log.txt", "[DEBUG-KLS]",
                            "Freed default KLS");

                    //Free temporary kls
                    kls_log(temporary_kls, "DEBUG", "Freeing temporary KLS");
                    kls_free(temporary_kls);
                    log_tag("debug_log.txt", "[DEBUG-KLS]",
                            "Freed temporary KLS");
                } else {
                    //TODO
                    //What is this?
                    int clrres = system("clear");
                    log_tag("debug_log.txt", "[DEBUG]",
                            "gameloop() 3 system(\"clear\") res was (%i)",
                            clrres);
                    printf("\n\n\tYOU DIED.\n\n");
                    log_tag("debug_log.txt", "[DEBUG]", "Game lost.");
                }

                /*
                   //Free lore strings if they were loaded
                   if (GAMEMODE == Story) {
                   for (int i=0; i<5; i++) {
                   char msg[1000];
                   sprintf(msg,"Freed lore string %i",i);
                   log_tag("debug_log.txt","[FREE]",msg);
                   sprintf(msg,"%s",lore_strings[i]);
                   log_tag("debug_log.txt","[FREE]",msg);
                   //free(lore_strings[i]);
                   }
                   }
                 */

                //free(path->win_condition);
                //free(path);
                log_tag("debug_log.txt", "[DEBUG]", "End of wincon loop.");

                //free(current_floor);

                endwin();
            } else {
                log_tag("debug_log.txt", "[ERROR]", "Error in gameloop().");
                exit(EXIT_FAILURE);
            }
        }
        //kls_temp_end(gamestate_kls);
    } while (retry(seed));

    //TODO
    //What is this?
    purple();
    printf("\n\n\t\tTHANKS 4 PLAYING!\n\n");
    white();
    log_tag("debug_log.txt", "[DEBUG]", "End of program.");
    exit(EXIT_SUCCESS);
}
