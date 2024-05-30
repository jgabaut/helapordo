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

static void hlpd_getopt(size_t argc, char** argv, const char* whoami)
{
    int option;
    while ((option = getopt(argc, argv, "f:r:E:S:tTGRXQLvdhaVDbjw")) != -1) {
        switch (option) {
        case 'j': {
            G_USE_VIM_DIRECTIONAL_KEYS = 1;
        }
        break;
        case 'w': {
            G_USE_WASD_DIRECTIONAL_KEYS = 1;
        }
        break;
        case 'b': {
            G_USE_DEFAULT_BACKGROUND = 1;
        }
        break;
        case 'D': {
            G_USE_CURRENTDIR = 1;
        }
        break;
        case 'S': {
            G_SEEDED_RUN_ON = 1;
            G_SEEDED_RUN_ARG = optarg;
        }
        break;
        case 'd': {
#ifndef HELAPORDO_DEBUG_ACCESS
#else
            G_DEBUG_ON = 1;
            G_LOG_ON = 1;
#endif
        }
        break;
        case 'r': {
            G_DEBUG_ROOMTYPE_ON = 1;
            G_DEBUG_ROOMTYPE_ARG = optarg;
        }
        break;
        case 'E': {
            G_DEBUG_ENEMYTYPE_ON = 1;
            G_DEBUG_ENEMYTYPE_ARG = optarg;
        }
        break;
        case 'L': {
            G_LOG_ON = 1;
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
            exit(EXIT_SUCCESS);
        }
        break;
        case 'T': {
            G_DOTUTORIAL_ON = 1;
            handleTutorial();
            usage(whoami);
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
            printf("  using: s4c-gui v%s\n", S4C_GUI_API_VERSION_STRING);
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
            exit(EXIT_SUCCESS);
        }
        break;
        case '?': {
            fprintf(stderr,
                    "Invalid option: %c\n Check your arguments.\n",
                    option);
            usage(whoami);
            // Handle invalid options
            exit(EXIT_FAILURE);
        }
        break;
        default: {
            // Should never get here
            fprintf(stderr, "Invalid option: %c\n, bad usage.\n",
                    option);
            exit(EXIT_FAILURE);
        }
        break;
        }
    }
}

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
#ifndef _WIN32
        (whoami = strrchr(argv[0], '/')) ? ++whoami : (whoami = argv[0]);
#else
        (whoami = strrchr(argv[0], '\\')) ? ++whoami : (whoami = argv[0]);
#endif
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

    char seed[PATH_SEED_BUFSIZE+1] = {0};

    bool is_seeded = false;

    hlpd_getopt(argc, argv, whoami);

    do {
        //Init default_kls
        default_kls = kls_new_conf(KLS_DEFAULT_SIZE * 16, default_kls_conf);
        temporary_kls = kls_new_conf(KLS_DEFAULT_SIZE * 32, temporary_kls_conf);


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
                || (NCURSES_VERSION_MAJOR == EXPECTED_NCURSES_VERSION_MAJOR && NCURSES_VERSION_MINOR < EXPECTED_NCURSES_VERSION_MINOR)
                || (NCURSES_VERSION_MAJOR == EXPECTED_NCURSES_VERSION_MAJOR && NCURSES_VERSION_MINOR == EXPECTED_NCURSES_VERSION_MINOR && NCURSES_VERSION_PATCH < EXPECTED_NCURSES_VERSION_PATCH)) {
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

        if (G_USE_DEFAULT_BACKGROUND == 1) {
#ifndef HELAPORDO_SUPPORT_DEFAULT_BACKGROUND
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Overriding flag G_USE_DEFAULT_BACKGROUND to 0, since reset_color_pairs() support is missing from this build.");
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Using ncurses v%i.%i.%i", __func__, NCURSES_VERSION_MAJOR, NCURSES_VERSION_MINOR, NCURSES_VERSION_PATCH);
            G_USE_DEFAULT_BACKGROUND = 0;
#endif // HELAPORDO_SUPPORT_DEFAULT_BACKGROUND
        }
        log_tag("debug_log.txt", "[DEBUG]", "Done getopt.");

        if (G_SEEDED_RUN_ON == 0) {
            log_tag("debug_log.txt", "[DEBUG]", "%s():    G_SEEDED_RUN_ON == 0 after getopt. Rolling random seed", __func__);
            gen_random_seed(seed);
        } else {
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Seeded run. Checking seed: {%s}", __func__, G_SEEDED_RUN_ARG);
            bool seed_check_res = check_seed(G_SEEDED_RUN_ARG);
            if (seed_check_res) {
                // Using a set seed. Uppercasing all letters
                for (size_t i=0; i < strlen(G_SEEDED_RUN_ARG); i++) {
                    char upp = toupper(G_SEEDED_RUN_ARG[i]);
                    G_SEEDED_RUN_ARG[i] = upp;
                }
                strncpy(seed, G_SEEDED_RUN_ARG, PATH_SEED_BUFSIZE);
                seed[PATH_SEED_BUFSIZE -1] = '\0';
                is_seeded = true;
            } else { //Go back to using a random seed
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Can't do a seeded run. Failed checking seed: {%s}. Using gen_random_seed().", __func__, G_SEEDED_RUN_ARG);
                gen_random_seed(seed);
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Using seed: {%s}", __func__, seed);
            }
        }

        log_tag("debug_log.txt", "[DEBUG]", "%s():    Calling srand(seed)", __func__);

        int hashed_seed = hlpd_hash((unsigned char*)seed);
        srand(hashed_seed);

        // Clear screen and print title, wait for user to press enter
        int clearres = system("clear");
        log_tag("debug_log.txt", "[DEBUG]",
                "gameloop() system(\"clear\") res was (%i)", clearres);
        printTitle();
        char c;
        yellow();
        printf("\n\n\n\n\t\t\tPRESS ENTER TO START\n\n");
        white();

        if (G_EXPERIMENTAL_ON) {
            lightPurple();
            printf("\t\t\t\t\t\t\tEXPERIMENTAL\n");
            white();
        }
        if (G_DEBUG_ON) {
            lightCyan();
            printf("\t\t\t\t\t\t\tDEBUG ON\n");
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

        WINDOW* screen = initscr();
        noecho();
        int cursorCheck = curs_set(0); // We try making the cursor invisible
        if (cursorCheck == ERR) {
            log_tag("debug_log.txt", "[ERROR]","%s():    Failed making the cursor invisible");
        }

        if (default_GameOptions.use_default_background || G_USE_DEFAULT_BACKGROUND == 1) {
            // game_options is initialised later ATM. But the default is overridden by G_USE_DEFAULT_BACKGROUND atm, so all good
            log_tag("debug_log.txt", "[DEBUG]",
                    "%s():    Calling use_default_colors()", __func__);
            short int pair0_fg;
            short int pair0_bg;
            pair_content(0, &pair0_fg, &pair0_bg);
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Pair 0 is {fg: %i, bg: %i}", __func__, pair0_fg, pair0_bg);
            int default_colors_res = use_default_colors();
            if (default_colors_res != OK) {
                log_tag("debug_log.txt", "[ERROR]", "%s():    Failed use_default_colors(). Res: {%i}", __func__, default_colors_res);
            }
        }

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

        GameOptions game_options = default_GameOptions;

        bool force_gameoptions_init = false;
        // Set static_path value to the correct static dir path
        resolve_staticPath(static_path);
        bool gameopts_prep_res = prep_GameOptions(&game_options, static_path, 0, default_kls, force_gameoptions_init);

        if (gameopts_prep_res) {
            log_tag("debug_log.txt", "[DEBUG]", "Done prep_GameOptions().");
        } else {
            log_tag("debug_log.txt", "[ERROR]", "Failed prep_GameOptions().");
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }

        log_tag("debug_log.txt", "[DEBUG]", "%s():    setting game_options.do_autosave to (GS_AUTOSAVE_ON == 1): {%s}", __func__, (GS_AUTOSAVE_ON == 1 ? "true" : "false"));
        if (GS_AUTOSAVE_ON == 1) {
            // Global var overtakes
            if (! game_options.do_autosave) log_tag("debug_log.txt", "[DEBUG]", "%s():    game_options autosave was false, but global var overtook", __func__);
            game_options.do_autosave = true;
        }
        if (G_USE_DEFAULT_BACKGROUND == 1) {
            // Global var overtakes
            if (! game_options.use_default_background) log_tag("debug_log.txt", "[DEBUG]", "%s():    game_options use_default_background was false, but global var overtook", __func__);
            game_options.use_default_background = true;
        }

        if (G_USE_VIM_DIRECTIONAL_KEYS == 1) { // Takes precedence over WASD option by being evaluated first
            if (game_options.directional_keys_schema != HLPD_VIM_KEYS) log_tag("debug_log.txt", "[DEBUG]", "%s():    game_options directional keys schema was not VIM_KEYS, but global var overtook", __func__);
            game_options.directional_keys_schema = HLPD_VIM_KEYS;
        } else if (G_USE_WASD_DIRECTIONAL_KEYS == 1) {
            if (game_options.directional_keys_schema != HLPD_WASD_KEYS) log_tag("debug_log.txt", "[DEBUG]", "%s():    game_options directional keys schema was not WASD_KEYS, but global var overtook", __func__);
            game_options.directional_keys_schema = HLPD_WASD_KEYS;
        }

        if (game_options.directional_keys_schema != default_GameOptions.directional_keys_schema) {
            log_tag("debug_log.txt", "[DEBUG]", "%s():    setting game_options.directional_keys_schema: {%s}", __func__, stringFrom_HLPD_DirectionalKeys_Schema(game_options.directional_keys_schema));
            HLPD_DirectionalKeys directional_keys = hlpd_default_directional_keys[game_options.directional_keys_schema];
            hlpd_default_keybinds[HLPD_KEY_UP] = directional_keys.up;
            hlpd_default_keybinds[HLPD_KEY_RIGHT] = directional_keys.right;
            hlpd_default_keybinds[HLPD_KEY_DOWN] = directional_keys.down;
            hlpd_default_keybinds[HLPD_KEY_LEFT] = directional_keys.left;
        }

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
            "Options",
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
        draw_buildinfo(savepick_side_win);
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
            init_s4c_color_pair_ex(&palette[i], 9 + i, ((game_options.use_default_background || G_USE_DEFAULT_BACKGROUND == 1 ) ? -1 : 0));
        }
        log_tag("debug_log.txt","[DEBUG]","%s():    Updating gamescreen->colors and colorpairs after init_s4c_color_pair() loop.", __func__);
        gamescreen->colors = COLORS;
        gamescreen->color_pairs = COLOR_PAIRS;
        int picked_saveslot_index = -1;

        while (!savepick_picked
               && (pickchar = wgetch(savepick_menu_win)) != KEY_F(1)) {
            if ( pickchar == hlpd_d_keyval(HLPD_KEY_DOWN)) {
                int menudriver_res = menu_driver(savepick_menu, REQ_DOWN_ITEM);
                if (menudriver_res == E_REQUEST_DENIED) {
                    menudriver_res = menu_driver(savepick_menu, REQ_FIRST_ITEM);
                }
            } else if ( pickchar == hlpd_d_keyval(HLPD_KEY_UP)) {
                int menudriver_res = menu_driver(savepick_menu, REQ_UP_ITEM);
                if (menudriver_res == E_REQUEST_DENIED) {
                    menudriver_res = menu_driver(savepick_menu, REQ_LAST_ITEM);
                }
            } else if ( pickchar == hlpd_d_keyval(HLPD_KEY_LEFT)) {
                /*Left option pick */
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
            } else if ( pickchar == hlpd_d_keyval(HLPD_KEY_RIGHT)) {
                /*Right option pick */
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
            } else if ( pickchar == hlpd_d_keyval(HLPD_KEY_DWNPAGE)) {
                menu_driver(savepick_menu, REQ_SCR_DPAGE);
            } else if ( pickchar == hlpd_d_keyval(HLPD_KEY_UPPAGE)) {
                menu_driver(savepick_menu, REQ_SCR_UPAGE);
            } else if ( pickchar == hlpd_d_keyval(HLPD_KEY_CONFIRM)) {
                /* Enter */
                ITEM *cur;

                //move(18,47);
                //clrtoeol();
                cur = current_item(savepick_menu);
                //mvprintw(18, 47, "Item selected is : %s", item_name(cur));
                savepick_choice = getTurnChoice((char *)item_name(cur));
                if (savepick_choice != GAME_OPTIONS) {
                    savepick_picked = 1;
                }
                pos_menu_cursor(savepick_menu);
                refresh();
            } else if ( pickchar == hlpd_d_keyval(HLPD_KEY_QUIT)) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "Player used q to quit from savepick menu.");
                //TODO: take some variable to disable quick quitting with q
                savepick_picked = 1;
                savepick_choice = getTurnChoice("Quit");
                pos_menu_cursor(savepick_menu);
                refresh();
            }
            wrefresh(savepick_menu_win);
            if (savepick_choice == NEW_GAME) {
                picked_saveslot_index = get_saveslot_index();
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
                picked_saveslot_index = get_saveslot_index();
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
            } else if (savepick_choice == GAME_OPTIONS) {
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Changing options from savepick menu", __func__);
                handleGameOptions(&game_options);
                clear();
                refresh();
                box(savepick_menu_win,0,0);
                //TODO: is it possible to avoid redrawing this?...
                wclear(savepick_side_win);
                draw_buildinfo(savepick_side_win);
                wrefresh(savepick_menu_win);
                wrefresh(savepick_side_win);
                savepick_choice = 999;
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
        Room* current_room = NULL;
        Floor* current_floor = NULL;

        if (load_info->is_new_game) {	// We prepare path and fighter
            path = randomise_path(seed, default_kls, current_save_path);
            path->current_saveslot->index = picked_saveslot_index;

            kls_log(default_kls, "DEBUG", "Prepping Fighter");
            player =
                (Fighter *) KLS_PUSH_TYPED(default_kls, Fighter, HR_Fighter,
                                           "Fighter", "Fighter");

            int optTot = optind;

            getParams(argc, argv, player, path, optTot, default_kls);
            initPlayerStats(player, path, default_kls);
        } else {		//Handle loading of gamestate

            log_tag("debug_log.txt", "[TURNOP]",
                    "Seed before loading, used to init path: [%s]", seed);
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
            path->current_saveslot->index = picked_saveslot_index;
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            gamestate =
                KLS_PUSH_TYPED(default_kls, Gamestate, HR_Gamestate, "Gamestate",
                               "Gamestate");
#ifndef KOLISEO_HAS_REGION
            log_tag("debug_log.txt", "[DEBUG]", "%s():    setting G_GAMESTATE", __func__);
            G_GAMESTATE = gamestate;
#endif
            init_Gamestate(gamestate, start_time, player->stats, path->win_condition, path,
                           player, GAMEMODE, gamescreen, &game_options, is_seeded);

            current_floor = KLS_PUSH_TYPED(default_kls, Floor, HR_Floor, "Floor",
                                           "Loading floor");
            gamestate->current_floor = current_floor;

            current_room = KLS_PUSH_TYPED(default_kls, Room, HR_Floor, "Room",
                                          "Loading room");
            gamestate->current_room = current_room;

            // TODO Prep loading room memory

            for (int e_idx=0; e_idx < ROOM_ENEMIES_MAX; e_idx++) {
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Preparing loading room enemy {%i}", __func__, e_idx);
                current_room->enemies[e_idx] = KLS_PUSH_TYPED(default_kls, Enemy, HR_Enemy, "Enemy", "Loading room enemy");
                prepareRoomEnemy(current_room->enemies[e_idx], 1, 3, 1,
                                 gamestate_kls);
            }
            bool did_exper_init = false;
            SaveHeader* current_saveHeader = prep_saveHeader(static_path, default_kls, false, &did_exper_init, path->current_saveslot->index);
            log_tag("debug_log.txt", "[DEBUG]", "Loaded Save Header version {%s}", current_saveHeader->game_version);

            bool prep_res = prep_Gamestate(gamestate, static_path, 0, default_kls, did_exper_init); //+ (idx* (sizeof(int64_t) + sizeof(SerGamestate))) , default_kls);
            if (prep_res) {
                log_tag("debug_log.txt", "[DEBUG]", "Done prep_Gamestate().");
            } else {
                log_tag("debug_log.txt", "[ERROR]", "Failed prep_Gamestate().");
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }


            load_info->enemy_index = gamestate->current_enemy_index;
            log_tag("debug_log.txt", "[DEBUG]", "%s():    load_info->enemy_index: {%i}", __func__, load_info->enemy_index);
            gamestate->path->seed[PATH_SEED_BUFSIZE] = '\0';
            memcpy(seed, gamestate->path->seed, PATH_SEED_BUFSIZE);
            seed[PATH_SEED_BUFSIZE] = '\0';
            log_tag("debug_log.txt", "[DEBUG]",
                    "Seed after loading: [%s]", seed);

            log_tag("debug_log.txt", "[DEBUG]", "%s():    Setting is_seeded {%s} to gamestate->is_seeded {%s}", __func__, (is_seeded ? "true" : "false"), (gamestate->is_seeded ? "true" : "false"));
            is_seeded = gamestate->is_seeded;

            //TODO: set the other load_info fields properly?
            //
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Checking save type", __func__);
            if (gamestate->current_room != NULL) {
                current_room = gamestate->current_room;
                switch (current_room->class) {
                case ENEMIES: {
                    if (load_info->enemy_index < 0) {
                        log_tag("debug_log.txt", "[ERROR]", "%s():    load_info->enemy_index was <0: {%i}", __func__, load_info->enemy_index);
                        endwin();
                        fprintf(stderr, "%s():    Failed preparing gamestate. Invalid enemy index.\n", __func__);
                        kls_free(default_kls);
                        kls_free(temporary_kls);
                        exit(EXIT_FAILURE);
                    }
                    load_info->save_type = ENEMIES_SAVE;
                }
                break;
                case HOME: {
                    load_info->save_type = HOME_SAVE;
                }
                break;
                case BASIC: {
                    load_info->save_type = FLOORMENU_SAVE;
                }
                break;
                default: {
                    log_tag("debug_log.txt", "[ERROR]", "%s():    unexpected class for current room. {%i}", __func__, current_room->class);
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                    break;
                }
                }

                log_tag("debug_log.txt", "[DEBUG]", "%s():    save_type: [%s]", __func__, stringFrom_saveType(load_info->save_type));
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Setting *(load_info->ptr_to_roomindex) to {%i}.", __func__, gamestate->player->stats->roomscompleted);
                *(load_info->ptr_to_roomindex) = gamestate->player->stats->roomscompleted;
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Setting *(load_info->ptr_to_roomtotalenemies) to {%i}.", __func__, current_room->enemyTotal);
                *(load_info->ptr_to_roomtotalenemies) = current_room->enemyTotal;

                if (current_room->class == ENEMIES) {
                    // TODO Load && Store current enemy somewhere.
                    kls_log(default_kls, "DEBUG", "Prepping Loady Enemy: gamestate->current_room->enemies[%i]", gamestate->current_enemy_index);
                    if (gamestate->current_enemy_index >= current_room->enemyTotal) {
                        log_tag("debug_log.txt", "[ERROR]", "%s():    gmst current_enemy_idex > gmst enemyTotal.", __func__);
                        kls_free(default_kls);
                        kls_free(temporary_kls);
                        exit(EXIT_FAILURE);
                    }
                    Enemy** loady_room_enemies = current_room->enemies;
                    if (loady_room_enemies == NULL) {
                        log_tag("debug_log.txt", "[ERROR]", "%s():    Loady room_enemies is NULL.", __func__);
                        log_tag("debug_log.txt", "[ERROR]", "idx/tot: {%i/%i}", gamestate->current_enemy_index, current_room->enemyTotal);
                        kls_free(default_kls);
                        kls_free(temporary_kls);
                        exit(EXIT_FAILURE);
                    }

                    Enemy* loady_enemy = loady_room_enemies[gamestate->current_enemy_index];
                    if (loady_enemy == NULL) {
                        log_tag("debug_log.txt", "[ERROR]", "%s():    Loady enemy_idx is NULL.", __func__);
                        log_tag("debug_log.txt", "[ERROR]", "idx/tot: {%i/%i}", gamestate->current_enemy_index, current_room->enemyTotal);
                        kls_free(default_kls);
                        kls_free(temporary_kls);
                        exit(EXIT_FAILURE);
                    } else {
                        //log_tag("debug_log.txt", "[DEBUG]", "%s():    Loady enemy: {%s}", __func__, stringFromEClass(loady_enemy->class));
                    }
                    load_info->loaded_enemy = loady_enemy;
                    //(Enemy *) KLS_PUSH_T_TYPED(gamestate_kls, Enemy, HR_Enemy,
                    //                         "Enemy", "Loaded Enemy");
                    //FIXME: the structs related to loaded enemy are not loaded on default_kls
                    //Update loading_room_turn_args->enemy pointer
                    //loading_room_turn_args->enemy = load_info->loaded_enemy;
                }
            } else {
                log_tag("debug_log.txt", "[WARN-TURNOP]",
                        "%s():    gamestate->room was NULL. Not setting load_info's room info.", __func__);
            }
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

        int roomsDone = (load_info->is_new_game == 1 ? 1 : loaded_roomindex);

        log_tag("debug_log.txt", "[DEBUG]", "roomsDone:    %i", roomsDone);
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

        if (!load_info->is_new_game) {
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Not pushing a new gamestate", __func__);
        } else {
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Pushing a new gamestate", __func__);
            gamestate =
                KLS_PUSH_TYPED(default_kls, Gamestate, HR_Gamestate, "Gamestate",
                               "Gamestate");
#ifndef KOLISEO_HAS_REGION
            log_tag("debug_log.txt", "[DEBUG]", "%s():    setting G_GAMESTATE", __func__);
            G_GAMESTATE = gamestate;
#endif
            init_Gamestate(gamestate, start_time, player->stats, path->win_condition, path,
                           player, GAMEMODE, gamescreen, &game_options, is_seeded);
        }
        if (gamestate->gamemode == Rogue) {
            //Note: different lifetime than gamestate
            //NO. The update_gamestate call is instead performed later.
            //Floor* current_floor = KLS_PUSH_T_TYPED(gamestate_kls,Floor,1,HR_Floor,"Floor","Init Curr floor");
            //NO. We pass NULL now.
            //
            //We also pass NULL for current room.
            update_Gamestate(gamestate, 1, HOME, roomsDone, -1, current_floor, NULL, &game_options);
        } else {
            update_Gamestate(gamestate, 1, HOME, roomsDone, -1, NULL, NULL, &game_options);
        }
        log_tag("debug_log.txt", "[DEBUG]", "Initialised Gamestate.");
        dbg_Gamestate(gamestate);

        if (load_info->is_new_game) {
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
                init_s4c_color_pair_ex(&palette[i], 9 + i, ((game_options.use_default_background || G_USE_DEFAULT_BACKGROUND == 1) ? -1 : 0));
            }
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

            bool is_bin_load_floor = (load_info->is_new_game == 0 && load_info->done_loading == 0);

            if (is_bin_load_floor) {
                update_Equipslots(player);
                log_tag("debug_log.txt", "[DEBUG]", "Using current_floor from loaded gamestate.");
            } else {
                log_tag("debug_log.txt", "[DEBUG]", "Prepping current_floor.");
                kls_log(default_kls, "DEBUG", "Prepping current_floor.");
                current_floor =
                    (Floor *) KLS_PUSH_T_TYPED(gamestate_kls, Floor,
                                               HR_Floor, "Floor", "Floor");
            }
            update_Gamestate(gamestate, 1, HOME, roomsDone, -1,
                             current_floor, NULL, &game_options); // NULL for current_room
            // Start the random walk from the center of the dungeon
            int center_x = FLOOR_MAX_COLS / 2;
            int center_y = FLOOR_MAX_ROWS / 2;

            if (is_bin_load_floor) {
                log_tag("debug_log.txt", "[DEBUG]", "Skipping current_floor init.");
            } else {
                // Init dbg_floor
                init_floor_layout(current_floor);

                //Set center as filled
                current_floor->floor_layout[center_x][center_y] = 1;

                //Init floor rooms
                init_floor_rooms(current_floor);

                if ((hlpd_rand() % 101) > 20) {
                    log_tag("debug_log.txt", "[DEBUG]", "%s():    Doing bsp init", __func__);
                    BSP_Room* bsp_tree = floor_bsp_gen(current_floor, gamestate_kls, center_x, center_y);
                    /*
                    WINDOW* test_win = newwin(FLOOR_MAX_COLS +2, FLOOR_MAX_ROWS+2, 0,0);
                    clear();
                    refresh();
                    box(test_win, 0, 0);
                    draw_BSP_Room(test_win, bsp_tree, 1, 1, 0);
                    refresh();
                    wgetch(test_win);
                    */
                    /*
                    WINDOW* win = newwin(LINES, COLS, 0, 0);
                    clear();
                    refresh();
                    draw_BSP_Tree(win, bsp_tree, 0, COLS/2, 1, 6, 2);
                    refresh();
                    getch();
                    */
                    dbg_BSP_Room(bsp_tree);
                    current_floor->from_bsp = true;
                } else {
                    log_tag("debug_log.txt", "[DEBUG]", "%s():    Doing random walk init", __func__);
                    //Random walk #1
                    floor_random_walk(current_floor, center_x, center_y, 100, 1);	// Perform 100 steps of random walk, reset floor_layout if needed.
                    //Random walk #2
                    floor_random_walk(current_floor, center_x, center_y, 100, 0);	// Perform 100 more steps of random walk, DON'T reset floor_layout if needed.
                    current_floor->from_bsp = false;
                }

                //Set floor explored matrix
                load_floor_explored(current_floor);

                //Set room types
                floor_set_room_types(current_floor);

                if (!current_floor->from_bsp) {
                    log_tag("debug_log.txt", "[DEBUG]", "Putting player at center: {%i,%i}", center_x, center_y);
                    player->floor_x = center_x;
                    player->floor_y = center_y;
                } else {
                    log_tag("debug_log.txt", "[DEBUG]", "%s():    Finding HOME room x/y for floor, and putting player there", __func__);
                    int home_room_x = -1;
                    int home_room_y = -1;
                    bool done_looking = false;
                    for(size_t i=0; i < FLOOR_MAX_COLS && !done_looking; i++) {
                        for (size_t j=0; j < FLOOR_MAX_ROWS && !done_looking; j++) {
                            if (current_floor->roomclass_layout[i][j] == HOME) {
                                log_tag("debug_log.txt", "[DEBUG]", "%s():    Found HOME room at {x:%i, y:%i}.", __func__, i, j);
                                home_room_x = i;
                                home_room_y = j;
                                done_looking = true;
                            }
                        }
                    }
                    if (!done_looking) {
                        log_tag("debug_log.txt", "[DEBUG]", "%s():    Could not find HOME room.", __func__);
                        kls_free(default_kls);
                        kls_free(temporary_kls);
                        exit(EXIT_FAILURE);
                    }
                    log_tag("debug_log.txt", "[DEBUG]", "Putting player at HOME room: {%i,%i}", home_room_x, home_room_y);
                    player->floor_x = home_room_x;
                    player->floor_y = home_room_y;
                }
            }

            //TODO: handle finishing all floors
            path->length = MAX_ROGUE_FLOORS;

            int floors_done = (load_info->is_new_game == 1 ? 0 : gamestate->player->stats->floorscompleted);

            log_tag("debug_log.txt", "[DEBUG]", "%s():    floors_done: {%i}", __func__, floors_done);

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
                } else {
                    if (!load_info->done_loading) {
                        log_tag("debug_log.txt", "[DEBUG-PREP]",
                                "Setting load_info->done_loading to 1.");
                    }
                    load_info->done_loading = 1;
                }

                Room *current_room = NULL;

                //Check if current room needs to be played
                if (current_floor->roomclass_layout[player->floor_x][player->floor_y] !=
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
                        roomclass_layout[player->floor_x][player->floor_y];
                    log_tag("debug_log.txt", "[ROOM]",
                            "Set Room #%i type:    (%s)\n", roomsDone,
                            stringFromRoom(room_type));

                    initRoom(current_room, player, roomsDone, room_type,
                             enemyTotal, load_info, gamestate_kls);
                    log_tag("debug_log.txt", "[ROOM]",
                            "Init Room #%i:    (%s)\n", roomsDone,
                            stringFromRoom(room_type));

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
                                     current_floor, current_room, &game_options);

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
                        //TODO: delete savefile?
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

                        if (current_room->class != HOME) {
                            log_tag("debug_log.txt", "[DEBUG]", "%s():    updating Gamestate to clear current_room reference", __func__);
                            update_Gamestate(gamestate, 1, BASIC,
                                             roomsDone, -1,
                                             current_floor, NULL, &game_options);  // Pass NULL for current room to gamestate
                        }

                        //Update floor's roomclass layout for finished rooms which should not be replayed
                        switch (current_floor->
                                roomclass_layout[player->floor_x][player->floor_y]) {
                        case ENEMIES: {
                            current_floor->
                            roomclass_layout[player->floor_x][player->floor_y] =
                                BASIC;
                        }
                        break;
                        case BOSS: {
                            current_floor->
                            roomclass_layout[player->floor_x][player->floor_y] =
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
                                             current_floor, NULL, &game_options); // Passing NULL for current_room

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
                            if ((hlpd_rand() % 101) > 20) {
                                log_tag("debug_log.txt", "[DEBUG]", "%s():    Doing bsp init", __func__);
                                BSP_Room* bsp_tree = floor_bsp_gen(current_floor, gamestate_kls, center_x, center_y);
                                dbg_BSP_Room(bsp_tree);
                                current_floor->from_bsp = true;
                            } else {
                                log_tag("debug_log.txt", "[DEBUG]", "%s():    Doing random walk init", __func__);
                                //Random walk #1
                                floor_random_walk(current_floor, center_x, center_y, 100, 1);	// Perform 100 steps of random walk, reset floor_layout if needed.
                                //Random walk #2
                                floor_random_walk(current_floor, center_x, center_y, 100, 0);	// Perform 100 more steps of random walk, DON'T reset floor_layout if needed.
                                current_floor->from_bsp = false;
                            }
                            //Set floor explored matrix
                            load_floor_explored(current_floor);
                            //Set room types
                            floor_set_room_types(current_floor);

                            if (!current_floor->from_bsp) {
                                log_tag("debug_log.txt", "[DEBUG]", "Putting player at center: {%i,%i}", center_x, center_y);
                                player->floor_x = center_x;
                                player->floor_y = center_y;
                            } else {
                                log_tag("debug_log.txt", "[DEBUG]", "%s():    Finding HOME room x/y for floor, and putting player there", __func__);
                                int home_room_x = -1;
                                int home_room_y = -1;
                                bool done_looking = false;
                                for(size_t i=0; i < FLOOR_MAX_COLS && !done_looking; i++) {
                                    for (size_t j=0; j < FLOOR_MAX_ROWS && !done_looking; j++) {
                                        if (current_floor->roomclass_layout[i][j] == HOME) {
                                            log_tag("debug_log.txt", "[DEBUG]", "%s():    Found HOME room at {x:%i, y:%i}.", __func__, i, j);
                                            home_room_x = i;
                                            home_room_y = j;
                                            done_looking = true;
                                        }
                                    }
                                }
                                if (!done_looking) {
                                    log_tag("debug_log.txt", "[DEBUG]", "%s():    Could not find HOME room.", __func__);
                                    kls_free(default_kls);
                                    kls_free(temporary_kls);
                                    exit(EXIT_FAILURE);
                                }
                                log_tag("debug_log.txt", "[DEBUG]", "Putting player at HOME room: {%i,%i}", home_room_x, home_room_y);
                                player->floor_x = home_room_x;
                                player->floor_y = home_room_y;
                            }
                            continue;	//Check win condition for loop
                        }
                        break;
                        case SHOP: {
                            current_floor->
                            roomclass_layout[player->floor_x][player->floor_y] =
                                BASIC;
                        }
                        break;
                        case TREASURE: {
                            current_floor->
                            roomclass_layout[player->floor_x][player->floor_y] =
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
                                    roomclass_layout[player->floor_x]
                                    [player->floor_x],
                                    stringFromRoom(current_floor->
                                                   roomclass_layout
                                                   [player->floor_x]
                                                   [player->floor_y]));
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
                                           roomclass_layout[player->floor_x]
                                           [player->floor_y]),
                            current_floor->
                            roomclass_layout[player->floor_x][player->floor_y]);
                }

                //Draw current FOV
                draw_floor_view(current_floor, player->floor_x, player->floor_y,
                                floor_win);
                //Take a step and update screen
                move_update(gamestate, current_floor, &(player->floor_x),
                            &(player->floor_y), floor_win, path, player,
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
                if (is_seeded) {
                    printf("\n\nSeeded run\n\n");
                    log_tag("debug_log.txt", "[DEBUG]", "%s():    Seeded run lost", __func__);
                }
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
