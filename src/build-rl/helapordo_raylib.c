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

int GetDisplayWidth(void)
{
    if (IsWindowFullscreen()) {
        int mon = GetCurrentMonitor();
        return GetMonitorWidth(mon);
    } else {
        return GetScreenWidth();
    }
}

int GetDisplayHeigth(void)
{
    if (IsWindowFullscreen()) {
        int mon = GetCurrentMonitor();
        return GetMonitorHeight(mon);
    } else {
        return GetScreenHeight();
    }
}

void gameloop_rl(int argc, char** argv)
{

    char *whoami;		// This will reference argv[0] at basename, it's the same string in memory, just starting later
#ifndef _WIN32
    (whoami = strrchr(argv[0], '/')) ? ++whoami : (whoami = argv[0]);
#else
    (whoami = strrchr(argv[0], '\\')) ? ++whoami : (whoami = argv[0]);
#endif

    int optTot = hlpd_getopt(argc, argv, whoami);
    if (G_DOTUTORIAL_ON == 1) {
        int screenWidth = 1000;
        int screenHeight = 450;

        SetConfigFlags(FLAG_WINDOW_RESIZABLE);

        InitWindow(screenWidth, screenHeight, "helapordo Tutorial");
        int fps_target = 30;
        SetTargetFPS(fps_target);
        while (!WindowShouldClose()) {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                break;
            }
            handleTutorial(palette);
        }
        CloseWindow();
        usage(whoami);
        exit(EXIT_SUCCESS);
    }

    log_tag("debug_log.txt", "[DEBUG]", "%s():    optTot: {%i}", __func__, optTot);
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

    //Init default_kls
    default_kls = kls_new_conf(KLS_DEFAULT_SIZE * 16, default_kls_conf);
    temporary_kls = kls_new_conf(KLS_DEFAULT_SIZE * 32, temporary_kls_conf);

    char *kls_progname = (char *)KLS_PUSH_ARR_TYPED(default_kls, char, strlen(whoami),
                         KLS_None, "progname", whoami);
    strcpy(kls_progname, whoami);

#ifndef HELAPORDO_DEBUG_LOG
#else
    hlpd_reset_logfile();
#endif

    // Parse command-line options
    loadInfo *load_info =
        (loadInfo *) KLS_PUSH_TYPED(default_kls, loadInfo, HR_loadInfo,
                                    "loadInfo", "loadInfo");

    load_info->is_new_game = -1;     // This is used as state to prompt the user to pick between new game (1) and load game (0)
    load_info->enemy_index = -1;
    load_info->total_foes = -1;
    load_info->save_type = -1;
    int loaded_roomtotalenemies = -1;
    int loaded_roomindex = -1;
    load_info->ptr_to_roomtotalenemies = &loaded_roomtotalenemies;
    load_info->ptr_to_roomindex = &loaded_roomindex;

    int saveslot_index = -1;  // This is used as state to prompt the user to pick a saveslot

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

    if (G_EXPERIMENTAL_ON == 1) {
        bool did_init = false;
        int saveslot_idx = 0;
        //TODO: handle saveslot selection instead of passing 0
        SaveHeader* current_saveHeader = prep_saveHeader(static_path, default_kls, false, &did_init, saveslot_idx);

        log_tag("debug_log.txt", "[DEBUG]", "Loaded Save Header version {%s}\n", current_saveHeader->game_version);
    }

    printf("\n\tDISCLAIMER: THIS BUILD IS STILL WIP.\n\n\tNO GUARANTEES ARE MADE.\n\n");
    printf("helapordo\n");
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

    log_tag("debug_log.txt", "[DEBUG]", "Prepping current_floor.");
    kls_log(default_kls, "DEBUG", "Prepping current_floor.");

    Koliseo_Temp* floor_kls = kls_temp_start(temporary_kls);
    Floor *current_floor =
        (Floor *) KLS_PUSH_TYPED(temporary_kls, Floor,
                                 HR_Floor, "Floor", "Floor");
    // Start the random walk from the center of the dungeon
    int center_x = FLOOR_MAX_COLS / 2;
    int center_y = FLOOR_MAX_ROWS / 2;

    int current_x = center_x;
    int current_y = center_y;

    // Init dbg_floor
    init_floor_layout(current_floor);

    //Set center as filled
    current_floor->floor_layout[center_x][center_y] = 1;

    //Init floor rooms
    init_floor_rooms(current_floor);

    if ((hlpd_rand() % 101) > 20) {
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Doing bsp init", __func__);
        floor_bsp_gen(current_floor, floor_kls, center_x, center_y);
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
        current_x = center_x;
        current_y = center_y;
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
        current_x = home_room_x;
        current_y = home_room_y;
    }

    // TODO: Initialize all required variables and load all required data here!

    const int windowWidth = 1000;
    const int windowHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

    InitWindow(windowWidth, windowHeight, "helapordo raylib test");
    SetWindowMinSize(320, 240);

    int gameScreenWidth = 640;
    int gameScreenHeight = 480;

    RenderTexture2D target_txtr = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target_txtr.texture, TEXTURE_FILTER_BILINEAR);

    GameScreen currentScreen = LOGO;


    int framesCounter = 0;          // Useful to count frames

    int fps_target = 30;
    SetTargetFPS(fps_target);               // Set desired framerate (frames-per-second)
    int logo_sleep = fps_target*2;
    //
    int current_anim_frame = 0;
    bool pause_animation = false;
    float scale = -1;
    Vector2 mouse = {0};
    Vector2 virtualMouse = {0};

    while (!WindowShouldClose()) {
        // Update
        //----------------------------------------------------------------------------------
        //

        update_GameScreen(&scale, gameScreenWidth, gameScreenHeight, &currentScreen, &framesCounter, &current_floor, &current_x, &current_y, logo_sleep, &pause_animation, &floor_kls, temporary_kls_conf, &current_anim_frame, &mouse, &virtualMouse, load_info, &saveslot_index);
        //----------------------------------------------------------------------------------

        // Draw render texture, will not go on screen yet
        //----------------------------------------------------------------------------------
        draw_GameScreen_Texture(target_txtr, currentScreen, gameScreenWidth, gameScreenHeight, mouse, virtualMouse, framesCounter, fps_target, current_anim_frame, current_floor, current_x, current_y, scale, load_info, saveslot_index);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        DrawTexturePro(target_txtr.texture, (Rectangle) {
            0.0f, 0.0f, (float)target_txtr.texture.width, (float)-target_txtr.texture.height
        }, (Rectangle) {
            (GetScreenWidth() - ((float) gameScreenWidth*scale))*0.5f, (GetScreenHeight() - ((float) gameScreenHeight*scale))*0.5f, (float) gameScreenWidth*scale, (float) gameScreenHeight*scale
        }, (Vector2) {
            0, 0
        }, 0.0f, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    UnloadRenderTexture(target_txtr);

    CloseWindow();
    kls_free(default_kls);
    kls_free(temporary_kls);
    return;
}
