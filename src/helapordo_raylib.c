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
#include "helapordo_raylib.h"

void gameloop_rl(int argc, char** argv)
{

    char *whoami;		// This will reference argv[0] at basename, it's the same string in memory, just starting later
    char path_to_kls_debug_file[600];
    char static_path[500];
    // Set static_path value to the correct static dir path
    resolve_staticPath(static_path);

    //Truncate "debug_log.txt"
    sprintf(path_to_kls_debug_file, "%s/%s", static_path, "kls_debug_log.txt");
    KLS_Conf default_kls_conf = {
        .kls_autoset_regions = 1,
        .kls_autoset_temp_regions = 1,
        .kls_verbose_lvl = 1,
        .kls_log_filepath = path_to_kls_debug_file,
        .kls_reglist_kls_size = KLS_DEFAULT_SIZE * 16,
        .kls_reglist_alloc_backend = KLS_REGLIST_ALLOC_KLS_BASIC,
    };
    KLS_Conf temporary_kls_conf = {
        .kls_autoset_regions = 1,
        .kls_autoset_temp_regions = 1,
        .kls_verbose_lvl = 0,
        .kls_log_fp = stderr,
        .kls_reglist_kls_size = KLS_DEFAULT_SIZE * 16,
        .kls_reglist_alloc_backend = KLS_REGLIST_ALLOC_KLS_BASIC,
    };

    //Init default_kls
    default_kls = kls_new_conf(KLS_DEFAULT_SIZE * 16, default_kls_conf);
    temporary_kls = kls_new_conf(KLS_DEFAULT_SIZE * 32, temporary_kls_conf);

#ifndef _WIN32
    (whoami = strrchr(argv[0], '/')) ? ++whoami : (whoami = argv[0]);
#else
    (whoami = strrchr(argv[0], '\\')) ? ++whoami : (whoami = argv[0]);
#endif

    char *kls_progname = (char *)KLS_PUSH_TYPED(default_kls, char *, sizeof(whoami),
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
        (loadInfo *) KLS_PUSH_TYPED(default_kls, loadInfo, 1, HR_loadInfo,
                                    "loadInfo", "loadInfo");

    load_info->is_new_game = 1;	//By default we do a new game
    load_info->enemy_index = -1;
    load_info->total_foes = -1;
    load_info->save_type = -1;
    int loaded_roomtotalenemies = -1;
    int loaded_roomindex = -1;
    load_info->ptr_to_roomtotalenemies = &loaded_roomtotalenemies;
    load_info->ptr_to_roomindex = &loaded_roomindex;

    while ((option = getopt(argc, argv, "f:r:E:tTGRXQLlvdhsa")) != -1) {
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
            int screenWidth = 1000;
            int screenHeight = 450;

            SetConfigFlags(FLAG_WINDOW_RESIZABLE);

            InitWindow(screenWidth, screenHeight, "helapordo Tutorial");
            int framesCounter = 0;
            int fps_target = 30;
            SetTargetFPS(fps_target);
            while (!WindowShouldClose()) {
                screenWidth = GetScreenWidth();
                screenHeight = GetScreenHeight();
                framesCounter++;
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    break;
                }
                handleTutorial(palette);
            }
            CloseWindow();
            usage(whoami);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_SUCCESS);
        }
        break;
        case 't': {
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
#ifndef _WIN32
    int clearres = system("clear");
    log_tag("debug_log.txt", "[DEBUG]",
            "gameloop() system(\"clear\") res was (%i)", clearres);
#else
    int clearres = system("cls");
    log_tag("debug_log.txt", "[DEBUG]",
            "gameloop() system(\"cls\") res was (%i)", clearres);
#endif

    printf("\n\tDISCLAIMER: THIS BUILD IS STILL WIP.\n\n\tNO GUARANTEES ARE MADE.\n\n");
    printf("helapordo v%s\n", VERSION);
    printf("  build: %s\n", helapordo_build_string);
    printf("  using: s4c-animate v%s\n", S4C_ANIMATE_VERSION);
    printf("  using: koliseo v%s\n", string_koliseo_version());
    printf("  using: raylib v%s\n", RAYLIB_VERSION);
#ifdef ANVIL__helapordo__
#ifndef INVIL__helapordo__HEADER__
    printf("Built with: amboso v%s\n",
                ANVIL__API_LEVEL__STRING);
#else
    printf("Built with: invil v%s\n",
                INVIL__VERSION__STRING);
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
        log_tag("debug_log.txt", "ERROR", "anvil date was invalid");
    } else {
        char build_time_buff[20] = {0};
        struct tm* build_time_tm = localtime(&anvil_build_time);

        if (build_time_tm == NULL) {
            log_tag("debug_log.txt", "ERROR", "localtime() failed");
        } else {
            strftime(build_time_buff, 20, "%Y-%m-%d %H:%M:%S", build_time_tm);
            printf("Date: %s\n", build_time_buff);
        }
    }
#endif // INVIL__helapordo__HEADER__
#else
    printf("Built without anvil\n");
#endif // ANVIL__helapordo__

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
    printf("\t\t\t\t\t\t\traylib build\n");
    printf("\t\t\t\t\t\t");
    printFormattedVersion(whoami);
    int scanfres = scanf("%c", &c);
    log_tag("debug_log.txt", "[DEBUG]", "gameloop() scanf() res was (%i)",
            scanfres);

    int screenWidth = 800;
    int screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "helapordo raylib test");

    GameScreen currentScreen = LOGO;

    // TODO: Initialize all required variables and load all required data here!

    int framesCounter = 0;          // Useful to count frames

    int fps_target = 30;
    SetTargetFPS(fps_target);               // Set desired framerate (frames-per-second)
    int logo_sleep = fps_target*2;
    //
    int current_anim_frame = 0;
    bool pause_animation = false;

    while (!WindowShouldClose()) {
        // Update
        //----------------------------------------------------------------------------------

        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        switch(currentScreen) {
        case LOGO: {
            // TODO: Update LOGO screen variables here!

            framesCounter++;    // Count frames

            // Wait for 2 seconds (120 frames) before jumping to TITLE screen
            if (framesCounter > logo_sleep) {
                currentScreen = TITLE;
            }
        }
        break;
        case TITLE: {
            // TODO: Update TITLE screen variables here!

            // Press enter to change to GAMEPLAY screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                currentScreen = GAMEPLAY;
            }
        }
        break;
        case GAMEPLAY: {
            // TODO: Update GAMEPLAY screen variables here!
            framesCounter++;    // Count frames

            // Press enter to change to ENDING screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                currentScreen = ENDING;
            }
            if (IsKeyPressed(KEY_P)) {
                pause_animation = !pause_animation;
            }
            if (!pause_animation) {
                current_anim_frame = framesCounter%60;
            }
        }
        break;
        case ENDING: {
            // TODO: Update ENDING screen variables here!

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                currentScreen = TITLE;
            }
        }
        break;
        default:
            break;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        switch(currentScreen) {
        case LOGO: {
            // TODO: Draw LOGO screen here!
            DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
            DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);
            for (int i = 0;  i <  KLS_TITLEROWS+1; i++) {
                DrawText(kls_title[i], 0, i*12, 12, BLACK);
            }
        }
        break;
        case TITLE: {
            // TODO: Draw TITLE screen here!
            DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
            DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
            DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

            char txt[30] = {0};
            char txt_b[30] = {0};
            char txt_s4c[30] = {0};
            int txt_StartX = screenWidth * 0.4;
            int txt_StartY = screenHeight * 0.65;
            DrawRectangle(txt_StartX, txt_StartY, screenWidth - txt_StartX, screenHeight - txt_StartY, YELLOW);
            sprintf(txt,"Koliseo API version: %i\n", int_koliseo_version());
            DrawText(txt, txt_StartX + ( txt_StartX * 0.16), txt_StartY + ( txt_StartY * 0.25), 24, BLACK);
            sprintf(txt_b,"Koliseo version: %s\n", string_koliseo_version());
            DrawText(txt_b, txt_StartX + ( txt_StartX * 0.16), txt_StartY + ( txt_StartY * 0.35), 24, BLACK);
            sprintf(txt_s4c,"s4c-animate version: %s\n", S4C_ANIMATE_VERSION );
            DrawText(txt_s4c, txt_StartX + ( txt_StartX * 0.16), txt_StartY + ( txt_StartY * 0.45), 24, BLACK);
        }
        break;
        case GAMEPLAY: {
            // TODO: Draw GAMEPLAY screen here!
            DrawRectangle(0, 0, screenWidth, screenHeight, RAYWHITE);
            DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
            DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
            Rectangle r = CLITERAL(Rectangle){20,20,screenWidth-2,screenHeight-2};
            DrawSpriteRect(mage_spark[current_anim_frame], r, 17, 17,0.02*sqrt(screenWidth*screenHeight), palette, PALETTE_S4C_H_TOTCOLORS);
        }
        break;
        case ENDING: {
            // TODO: Draw ENDING screen here!
            DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
            DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
            DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);

        }
        break;
        default:
            break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();
    return;
}
