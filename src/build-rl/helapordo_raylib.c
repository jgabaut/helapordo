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

    char seed[PATH_SEED_BUFSIZE+1] = {0};

    bool is_seeded = false;
    int optTot = hlpd_getopt(argc, argv, whoami);
    char player_name[21] = "";
    char class_name[21] = "";
    if (argc >= 2) {
        if (argv[optTot] != NULL) {
            memcpy(player_name, argv[optTot], 21);
            player_name[20] = '\0';
            log_tag("debug_log.txt", "DEBUG", "%s():    Passed player name is {%s}", __func__, player_name);
        } else {
            log_tag("debug_log.txt", "WARN", "%s():    Passed player name was NULL.", __func__);
        }
    }
    if (argc >= 3) {
        if (argv[optTot+1] != NULL) {
            memcpy(class_name, argv[optTot+1], 21);
            class_name[20] = '\0';
            log_tag("debug_log.txt", "DEBUG", "%s():    Passed player class is {%s}", __func__, class_name);
        } else {
            log_tag("debug_log.txt", "WARN", "%s():    Passed player class was NULL.", __func__);
        }
    }
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
                                    1, //kls_block_usage_with_temp
                                    0, // allow_zerocount_push
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
                                      1, //kls_block_usage_with_temp
                                      0, // allow_zerocount_push
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

    log_tag("debug_log.txt", "[DEBUG]", "%s():    Run is%sseeded.", __func__, (is_seeded ? " " : " not "));
    log_tag("debug_log.txt", "[DEBUG]", "%s():    Calling srand(seed)", __func__);

    int hashed_seed = hlpd_hash((unsigned char*)seed);
    srand(hashed_seed);

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

    if (G_DEBUG_ON > 0) {
        fprintf(stderr, "%s():    Seed: {%s}\n", __func__, seed);
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

    Koliseo_Temp* floor_kls = NULL;
    Floor *current_floor = NULL;
    // Start the random walk from the center of the dungeon
    int center_x = FLOOR_MAX_COLS / 2;
    int center_y = FLOOR_MAX_ROWS / 2;

    int current_x = center_x;
    int current_y = center_y;

    char current_save_path[1500] = {0};

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
    Path* game_path = NULL;
    Fighter* player = NULL;
    Room* current_room = NULL;
    Gamestate* gameState = NULL;
    char* notifications_buffer = (char*) KLS_PUSH_ARR_TYPED(default_kls, Notification, NOTIFICATIONS_RINGBUFFER_SIZE+1, HR_Notification, "Notification buffer", "Notification");

    size_t ringabuf_size = rb_structsize__();
    size_t ringabuf_align = rb_structalign__();

#ifndef KOLISEO_HAS_REGION
    RingaBuf rb_notifications = kls_push_zero_AR(default_kls, ringabuf_size, ringabuf_align, 1);
#else
    RingaBuf rb_notifications = kls_push_zero_typed(default_kls, ringabuf_size, ringabuf_align, 1, HR_RingaBuf, "RingaBuf for notifications", "RingaBuf");
#endif // KOLISEO_HAS_REGION

    rb_notifications = rb_new_arr(rb_notifications, notifications_buffer, Notification, NOTIFICATIONS_RINGBUFFER_SIZE);
    size_t capacity = rb_get_capacity(rb_notifications);

    enqueue_notification("HI", 500, S4C_RED, &rb_notifications);

#ifndef _WIN32
    log_tag("debug_log.txt", "[DEBUG]", "%s():    Prepared notifications ring buffer. Capacity: {%li}", __func__, capacity);
#else
    log_tag("debug_log.txt", "[DEBUG]", "%s():    Prepared notifications ring buffer. Capacity: {%lli}", __func__, capacity);
#endif



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

    Gui_Button bt_newgame = {
        .r = (Rectangle){.x = 100, .y = 100, .width = 150, .height = 80},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "New Game",
        .label_len = strlen("New Game"),
        .box_color = DARKGREEN,
        .text_color = DARKGRAY,
    };

    Gui_Button bt_loadgame = {
        .r = (Rectangle){.x = 300, .y = 100, .width = 150, .height = 80},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Load Game",
        .label_len = strlen("Load Game"),
        .box_color = ORANGE,
        .text_color = DARKGRAY,
    };

    Gui_Button bt_namefield = {
        .r = (Rectangle){.x = 100, .y = 100, .width = 200, .height = 100},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "",
        .label_len = 0,
        .box_color = LIGHTGRAY,
        .text_color = BLACK,
    };

    if (strlen(player_name) > 0) {
        player_name[20] = '\0';
        memcpy(bt_namefield.label, player_name, 21);
        bt_namefield.label[20] = '\0';
        bt_namefield.label_len = strlen(bt_namefield.label);
    }

    Gui_Button bt_slot1 = {
        .r = (Rectangle){.x = 100, .y = 100, .width = 100, .height = 80},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Slot 1",
        .label_len = strlen("Slot 1"),
        .box_color = DARKGREEN,
        .text_color = DARKGRAY,
    };

    Gui_Button bt_slot2 = {
        .r = (Rectangle){.x = 210, .y = 100, .width = 100, .height = 80},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Slot 2",
        .label_len = strlen("Slot 2"),
        .box_color = DARKGREEN,
        .text_color = DARKGRAY,
    };

    Gui_Button bt_slot3 = {
        .r = (Rectangle){.x = 320, .y = 100, .width = 100, .height = 80},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Slot 3",
        .label_len = strlen("Slot 3"),
        .box_color = DARKGREEN,
        .text_color = DARKGRAY,
    };

    Gui_Button bt_classfield = {
        .r = (Rectangle){.x = 100, .y = 100, .width = 200, .height = 100},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "",
        .label_len = 0,
        .box_color = LIGHTGRAY,
        .text_color = BLACK,
    };

    if (strlen(class_name) > 0) {
        if ((strcmp(class_name, "Knight") == 0) ||
            (strcmp(class_name, "Archer") == 0) ||
            (strcmp(class_name, "Mage") == 0) ||
            (strcmp(class_name, "Assassin") == 0)) {
            class_name[20] = '\0';
            memcpy(bt_classfield.label, class_name, 21);
            bt_classfield.label[20] = '\0';
            bt_classfield.label_len = strlen(bt_classfield.label);
        } else {
            log_tag("debug_log.txt", "DEBUG", "%s():    Rejecting invalid class arg {%s}", __func__, class_name);
            fprintf(stderr, "[DEBUG] [%s()]    Rejecting invalid class arg {%s}\n", __func__, class_name);
        }
    }

    Gui_Button bt_class_knight = {
        .r = (Rectangle){.x = 50, .y = 100, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Knight",
        .label_len = strlen("Knight"),
        .box_color = DARKGREEN,
        .text_color = DARKGRAY,
    };

    Gui_Button bt_class_archer = {
        .r = (Rectangle){.x = 160, .y = 100, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Archer",
        .label_len = strlen("Archer"),
        .box_color = DARKGREEN,
        .text_color = DARKGRAY,
    };

    Gui_Button bt_class_mage = {
        .r = (Rectangle){.x = 270, .y = 100, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Mage",
        .label_len = strlen("Mage"),
        .box_color = DARKGREEN,
        .text_color = DARKGRAY,
    };

    Gui_Button bt_class_assassin = {
        .r = (Rectangle){.x = 380, .y = 100, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Assassin",
        .label_len = strlen("Assassin"),
        .box_color = DARKGREEN,
        .text_color = DARKGRAY,
    };

    Gui_State gui_state = (Gui_State) {
        .scale = scale,
        .gameScreenWidth = gameScreenWidth,
        .gameScreenHeight = gameScreenHeight,
        .currentScreen = currentScreen,
        .framesCounter = framesCounter,
        .mouse = mouse,
        .virtualMouse = virtualMouse,
        .buttons = {
            [BUTTON_NEW_GAME] = bt_newgame,
            [BUTTON_LOAD_GAME] = bt_loadgame,
            [BUTTON_CLASS_TXTFIELD] = bt_classfield,
            [BUTTON_CLASS_KNIGHT] = bt_class_knight,
            [BUTTON_CLASS_ARCHER] = bt_class_archer,
            [BUTTON_CLASS_MAGE] = bt_class_mage,
            [BUTTON_CLASS_ASSASSIN] = bt_class_assassin,
            [BUTTON_NAME_TXTFIELD] = bt_namefield,
            [BUTTON_SAVESLOT_1] = bt_slot1,
            [BUTTON_SAVESLOT_2] = bt_slot2,
            [BUTTON_SAVESLOT_3] = bt_slot3,
        },
        .theme = {
            .bg_color = GRAY,
            .txt_color = DARKGRAY,
        },
    };

    int roomsDone = 0;
    int enemyTotal = -1;

    while (!WindowShouldClose()) {
        // Update
        //----------------------------------------------------------------------------------
        //

        update_GameScreen(&gui_state, &current_floor, &game_path, &player, &current_room, &gameState, &rb_notifications, &current_x, &current_y, logo_sleep, &pause_animation, &floor_kls, temporary_kls_conf, &current_anim_frame, load_info, &saveslot_index, current_save_path, seed, is_seeded, &roomsDone, &enemyTotal);
        //----------------------------------------------------------------------------------

        // Draw render texture, will not go on screen yet
        //----------------------------------------------------------------------------------
        draw_GameScreen_Texture(target_txtr, gui_state, fps_target, current_anim_frame, current_floor, game_path, player, current_room, gameState, &rb_notifications, current_x, current_y, load_info, saveslot_index, current_save_path, seed);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        DrawTexturePro(target_txtr.texture, (Rectangle) {
            0.0f, 0.0f, (float)target_txtr.texture.width, (float)-target_txtr.texture.height
        }, (Rectangle) {
            (GetScreenWidth() - ((float) gui_state.gameScreenWidth * gui_state.scale))*0.5f, (GetScreenHeight() - ((float) gui_state.gameScreenHeight * gui_state.scale))*0.5f, (float) gui_state.gameScreenWidth * gui_state.scale, (float) gui_state.gameScreenHeight * gui_state.scale
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
