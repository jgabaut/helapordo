#include "game_debug.h"


#ifdef HELAPORDO_CURSES_BUILD
/**
 * Takes a Fighter and a Path pointers (and an integer for current room index) and asks user input to execute debug actions.
 * @see Fighter
 * @see Path
 * @see statReset()
 * @see GET_CALLBACK()
 * @see unlock_special()
 * @param gmst The Gamestate pointer.
 * @param player The Fighter pointer at hand.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param The Koliseo to debug.
 * @param The Koliseo_Temp used for allocations.
 */
void debug_generic(Gamestate *gmst, Fighter *player, Path *p, int roomIndex,
                   Koliseo *kls, Koliseo_Temp *t_kls)
{

    char msg[200];
    char ch[25];
    int picked_debug_proc = 0;
#ifndef _WIN32
    struct utsname uts;
    uname(&uts);
    sprintf(msg, "debug_generic() loaded utsname using uname().\n");
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "System is %s\n", uts.sysname);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "OS Release is %s\n", uts.release);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "OS Version is %s\n", uts.version);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "Machine is %s\n", uts.machine);
    log_tag("debug_log.txt", "[DEBUG]", msg);
#endif

    int res = system("clear");
    sprintf(msg, "debug_generic() system(\"clear\") res was (%i)", res);
    log_tag("debug_log.txt", "[DEBUG]", msg);

    int c = 0, n = -1;
    while (!picked_debug_proc) {
        int res = system("clear");
        sprintf(msg, "debug_generic() 2 system(\"clear\") res was (%i)", res);
        log_tag("debug_log.txt", "[DEBUG]", msg);
        printf("\n\
	What do you want to do? ('q' to quit)\n\
		[\n\
			'0':  Give xp points\t'1':  Give consumable\n\
			'2':  Reset stats\t'3':  Alter luck\n\
			'4':  Give coins\t'5':  Unlock special\n\
			'6':  Unlock Artifact\t'7':   Print turnCounter\n\
			's':  Sprites slideshow\t'd':  Dump debug symbols\n\
			'g':  Toggle godmode\t'A':  Toggle autosave\n\
			'L':  Toggle logging\t'F':  Try Floor functionality\n\
			'Q':  Toggle fast quit\t'k': Log passed kls state to debug log file.\n\
			't': Log global temporary_kls state to debug log file.\n\
			'K': Log usage stats for passed kls to debug log file.\n\
			'T': Log usage stats for temporary_kls to debug log file.\n\
			'G': Log Gamestate to debug log file.\n\
			{Return}  Process your input line.\t'q':  Quit\n\
		]\n\n\
	[%s@debug-func]$ ", player->name);

        char *fgetsres = fgets(ch, sizeof ch, stdin);
        sprintf(msg, "debug_generic() fgets() res was (%s)", fgetsres);
        log_tag("debug_log.txt", "[DEBUG]", msg);
        switch (ch[0]) {
        case '0': {
            picked_debug_proc = 1;
            int xp;
            do {
                printf("\nHow much?\n");
            } while ((c = scanf("%i", &xp)) != 1 || xp < 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_generic() scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            checkremainder(player, xp);
        }
        break;
        case '1': {
            picked_debug_proc = 1;
            int q = -1;
            do {
                printf("\nInsert consumable number:\n");
            } while ((c = scanf("%i", &n)) != 1 || n > CONSUMABLESMAX
                     || n < 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_generic() scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            do {
                printf("\nInsert quantity:\n");
            } while ((c = scanf("%i", &q)) != 1 && q <= 0);
            res = scanf("%*c");
            sprintf(msg, "debug_generic() 2 scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            Consumable *c = (Consumable *) player->consumablesBag[n];
            c->qty += q;
            player->stats->consumablesfound += q;
        }
        break;
        case '2': {
            picked_debug_proc = 1;
            statReset(player, 1);
        }
        break;
        case '3': {
            picked_debug_proc = 1;
            printf("\nCurrent luck: %i\tRL: %i\n", player->luck, p->luck);

            do {
                printf("\nInsert new luck:\n");
            } while ((c = scanf("%i", &n)) != 1 && n > 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_generic() 3 scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            //FIXME:
            //Copy-pasted the calc for player luck... might need a function
            p->luck = n;

            player->luck = (p->luck * MAXPLAYERLUCK) / MAXLUCK;
        }
        break;
        case '4': {
            picked_debug_proc = 1;
            /* Old code invoking the macro for special moves directly. Used for testing.
             * int s = -1;
             * printf("Insert special num:\n");
             * scanf("%i",&s);
             * scanf("%*c");
             * printf("Read: %i\n",s);
             * GET_CALLBACK(s,callback_special_t)(player,e,p,roomIndex,currentEnemyNum);
             */

            //printActivePerks(player);
            int c;
            int n = -1;
            do {
                printf("\nInsert coin number (0<n<100):\n");
            } while ((c = scanf("%i", &n)) < 0 || c > 3 || n < 0
                     || n > 100);
            int res = scanf("%*c");
            sprintf(msg, "debug_generic() 4 scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            player->balance += n;
        }
        break;
        case '5': {
            picked_debug_proc = 1;
            unlockSpecial(player);
        }
        break;
        case '6': {
            picked_debug_proc = 1;
            int n = -1;
            do {
                printf("\nInsert artifact number (0<n<%i):\n",
                       ARTIFACTSMAX + 1);
            } while ((c = scanf("%i", &n)) != 1 || n > ARTIFACTSMAX
                     || n < 0);

            int res = scanf("%*c");
            sprintf(msg, "debug_generic() scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            player->artifactsBag[n]->qty += 1;
            player->artifactsBag[n]->active = 0;
            player->stats->artifactsfound += 1;
        }
        break;
        case '7': {
            picked_debug_proc = 1;
            int res = system("clear");
            sprintf(msg, "debug_generic() 3 system(\"clear\") res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            printf("\nPlayer Counters:\n");
            printCounters((Turncounter **) player->counters);
            printf("\nPress Enter to resume game");
            res = scanf("%*c");
            sprintf(msg, "debug_generic() 5 scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'd': {
            picked_debug_proc = 1;
            printf("\nVERSION:    %s\n", VERSION);
#ifndef _WIN32
            printf("\nSystem:    %s\n", uts.sysname);
            printf("\nOS Release:    %s\n", uts.release);
            printf("\nOS Version:    %s\n", uts.version);
            printf("\nMachine:    %s\n", uts.machine);
#endif
            printf("\nGAMEMODE:    %s\n", stringFromGamemode(GAMEMODE));
            printf("\nPath->current_saveslot->save_path:    %s\n",
                   p->current_saveslot->save_path);
            printf("\nGS_AUTOSAVE_ON:    %i\n", GS_AUTOSAVE_ON);
            printf("\nG_FASTQUIT_ON:    %i\n", G_FASTQUIT_ON);
            printf("\nG_DEBUG_ON:    %i\n", G_DEBUG_ON);
            printf("\nG_LOG_ON:    %i\n", G_LOG_ON);
            printf("\nG_GODMODE_ON:    %i\n", G_GODMODE_ON);
            printf("\nG_EXPERIMENTAL_ON:    %i\n", G_EXPERIMENTAL_ON);
            printf("\nG_DEBUG_ROOMTYPE_ON:    %i\n", G_DEBUG_ROOMTYPE_ON);
            if (G_DEBUG_ROOMTYPE_ON == 1) {
                printf("\nG_DEBUG_ROOMTYPE:    %i\n", G_DEBUG_ROOMTYPE);
            }
            printf("\nG_DEBUG_ENEMYTYPE_ON:    %i\n", G_DEBUG_ENEMYTYPE_ON);
            if (G_DEBUG_ENEMYTYPE_ON == 1) {
                printf("\nG_DEBUG_ENEMYTYPE:    %i\n", G_DEBUG_ENEMYTYPE);
            }
            printf("\nPress Enter to resume game.\n");
            int res = scanf("%*c");
            sprintf(msg, "debug_generic() 7 scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'F': {
            sprintf(msg, "Trying out Floor functionality.");
            log_tag("debug_log.txt", "[DEBUG]", msg);
            // Declare dbg_floor
            sprintf(msg, "Pushing dbg_floor to tkls.");
            log_tag("debug_log.txt", "[DEBUG]", msg);
            kls_log(kls, "DEBUG", msg);
            Floor *dbg_floor =
                (Floor *) KLS_PUSH_T_TYPED(t_kls, Floor, HR_Floor,
                                           "Floor", msg);
            // Start the random walk from the center of the dungeon
            int center_x = FLOOR_MAX_COLS / 2;
            int center_y = FLOOR_MAX_ROWS / 2;
            // Init dbg_floor
            init_floor_layout(dbg_floor);
            //Set center as filled
            dbg_floor->floor_layout[center_x][center_y] = 1;

            init_floor_rooms(dbg_floor);

            floor_random_walk(dbg_floor, center_x, center_y, 100, 1);	// Perform 100 steps of random walk, reload floor_layout if needed
            load_floor_explored(dbg_floor);

            floor_set_room_types(dbg_floor);

            /*
               debug_print_floor_layout(dbg_floor);

               printf("\nPress Enter to see room layout.\n");
               int res = scanf("%*c");
               sprintf(msg,"debug_generic() 7 scanf() res was (%i)", res);
               log_tag("debug_log.txt","[DEBUG]",msg);
             */
            debug_print_roomclass_layout(dbg_floor, stdout);
            printf("\nPress Enter to return to game.\n");
            res = scanf("%*c");
            sprintf(msg, "debug_generic() 8 scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            //free(dbg_floor);
        }
        break;
        case 'A': {
            GS_AUTOSAVE_ON = (GS_AUTOSAVE_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_AUTOSAVE_ON, new value: (%i)",
                    GS_AUTOSAVE_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'G': {
            log_tag("debug_log.txt", "[DEBUG]", "Logging Gamestate");
            dbg_Gamestate(gmst);
            log_tag("debug_log.txt", "[DEBUG]", "Done logging Gamestate");
        }
        break;
        case 'T': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }

            fprintf(kls_file, "--BEGIN debug of temporary_kls--\n");
            for (int i = HR_Path; i < HLP_MAX_INDEX + 99; i++) {
                ptrdiff_t usage = kls_type_usage(i, temporary_kls);
#ifndef _WIN32
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %li }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#else
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %lli }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#endif
            }
            fprintf(kls_file, "--END debug of temporary_kls--\n\n");

            fclose(kls_file);
        }
        break;
        case 't': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }
            fprintf(kls_file, "--BEGIN debug of temporary_kls--\n");
            print_kls_2file(kls_file, temporary_kls);
            kls_showList_toFile(temporary_kls->regs, kls_file);
            kls_usageReport_toFile(temporary_kls, kls_file);
            fprintf(kls_file, "--END debug of temporary_kls--\n\n");
            WINDOW *win = NULL;
            /* Initialize curses */
            clear();
            refresh();
            start_color();
            cbreak();
            noecho();
            keypad(stdscr, TRUE);
            win = newwin(20, 50, 1, 2);
            keypad(win, TRUE);
            wclear(win);
            wrefresh(win);
            kls_showList_toWin(temporary_kls, win);
            delwin(win);
            endwin();

            fclose(kls_file);
        }
        break;
        case 'K': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }

            fprintf(kls_file, "--BEGIN debug of passed kls--\n");
            for (int i = HR_Path; i < HLP_MAX_INDEX + 99; i++) {
                ptrdiff_t usage = kls_type_usage(i, kls);
#ifndef _WIN32
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %li }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#else
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %lli }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#endif
            }
            fprintf(kls_file, "--END debug of passed kls--\n\n");

            fclose(kls_file);
        }
        break;
        case 'k': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }
            fprintf(kls_file, "--BEGIN debug of passed kls--\n");
            print_kls_2file(kls_file, kls);
            kls_showList_toFile(kls->regs, kls_file);
            kls_usageReport_toFile(kls, kls_file);
            fprintf(kls_file, "--END debug of passed kls--\n\n");
            WINDOW *win = NULL;
            /* Initialize curses */
            clear();
            refresh();
            start_color();
            cbreak();
            noecho();
            keypad(stdscr, TRUE);
            win = newwin(20, 50, 1, 2);
            keypad(win, TRUE);
            wclear(win);
            wrefresh(win);
            kls_showList_toWin(kls, win);
            delwin(win);
            endwin();

            fclose(kls_file);
        }
        break;
        case 'Q': {
            G_FASTQUIT_ON = (G_FASTQUIT_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_FASTQUIT_ON, new value: (%i)",
                    G_FASTQUIT_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'g': {
            G_GODMODE_ON = (G_GODMODE_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_GODMODE_ON, new value: (%i)",
                    G_GODMODE_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'L': {
            G_LOG_ON = (G_LOG_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_LOG_ON, new value: (%i)", G_LOG_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        case 's': {		//Query sprites slideshow
            picked_debug_proc = 1;
            int res = system("clear");
            sprintf(msg, "debug_generic() 4 system(\"clear\") res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            int comm_len = -1;
            int picked_comm = 0;
            char comm[15] = "empty";
            char *desc = NULL;
            int sprite_total = -1;
            int tot_cons = CONSUMABLESMAX + 1;
            int tot_enemy = ENEMYCLASSESMAX + 1;
            int tot_boss = BOSSCLASSESMAX + 1;
            int tot_art = ARTIFACTSMAX + 1;
            int tot_eq = EQUIPSMAX + 1;
            int tot_eqzones = EQUIPZONES + 1;
            int tot_misc = MISC_SPRITES_MAX + 1;
            int allsprites =
                tot_cons +
                tot_enemy +
                tot_boss + tot_art + tot_eq + tot_eqzones + tot_misc;

            printf("\nConsumables sprites: \t%d\
\nArtifacts sprites: \t%d\
\nEquips sprites: \t%d\
\nEquip zone sprites: \t%d\
\nEnemies sprites: \t%d\
\nBosses sprites: \t%d\
\nOther misc sprites: \t%d\n", tot_cons, tot_art, tot_eq, tot_eqzones, tot_enemy, tot_boss, tot_misc);
            printf("--------------------------");
            printf("\nTotal sprites: \t\t%d\n", allsprites);
            printf
            ("\nInsert sprites query, q to quit: (q|boss|cons|equip|eqzone|artf|enemy|misc):\n\n[%s@spriteshow]$ ",
             player->name);
            while (!picked_comm && (comm_len = scanf("%10s", comm)) > -1) {
                int res = scanf("%*c");
                sprintf(msg, "debug_generic() 7 scanf() res was (%i)", res);
                log_tag("debug_log.txt", "[DEBUG]", msg);
                if (strcmp(comm, "q") == 0) {
                    return;
                } else if (strcmp(comm, "boss") == 0) {
                    picked_comm = 1;
                    sprite_total = BOSSCLASSESMAX + 1;
                } else if (strcmp(comm, "cons") == 0) {
                    picked_comm = 1;
                    sprite_total = CONSUMABLESMAX + 1;
                } else if (strcmp(comm, "equip") == 0) {
                    picked_comm = 1;
                    sprite_total = EQUIPSMAX + 1;
                } else if (strcmp(comm, "eqzone") == 0) {
                    picked_comm = 1;
                    sprite_total = EQUIPZONES + 1;
                } else if (strcmp(comm, "artf") == 0) {
                    picked_comm = 1;
                    sprite_total = ARTIFACTSMAX + 1;
                } else if (strcmp(comm, "enemy") == 0) {
                    picked_comm = 1;
                    sprite_total = ENEMYCLASSESMAX + 1;
                } else if (strcmp(comm, "misc") == 0) {
                    picked_comm = 1;
                    sprite_total = MISC_SPRITES_MAX + 1;
                } else {
                    printf
                    ("\nInsert sprites query, q to quit: (q|boss|cons|equip|eqzone|artf|enemy|misc):\n\n[%s@spriteshow]$ ",
                     player->name);
                }
            }
            int sprite_count = -1;
            int c = -1;
            char s[20];
            int startx = 0;
            int x, y;
            WINDOW *spriteshow_win;
            /* Initialize curses */
            //initscr();
            start_color();
            clear();
            refresh();
            cbreak();
            noecho();
            keypad(stdscr, TRUE);
            spriteshow_win = newwin(19, 19, 2, 2);
            keypad(spriteshow_win, TRUE);
            for (sprite_count = 0; sprite_count < sprite_total;
                 sprite_count++) {
                if (strcmp(comm, "boss") == 0) {
                    desc = stringFromBossClass(sprite_count);
                } else if (strcmp(comm, "cons") == 0) {
                    desc = stringFromConsumables(sprite_count);
                } else if (strcmp(comm, "equip") == 0) {
                    desc = stringFromEquips(sprite_count);
                } else if (strcmp(comm, "eqzone") == 0) {
                    desc = stringFromEquipzones(sprite_count);
                } else if (strcmp(comm, "artf") == 0) {
                    desc = stringFromArtifacts(sprite_count);
                } else if (strcmp(comm, "enemy") == 0) {
                    desc = stringFromEClass(sprite_count);
                } else if (strcmp(comm, "misc") == 0) {
                    desc = "Misc";
                }
                wclear(spriteshow_win);
                wrefresh(spriteshow_win);
                wclear(stdscr);
                wrefresh(stdscr);
                sprintf(msg, "Showing sprite n.%d, class is: %s\n",
                        sprite_count, desc);
                log_tag("debug_log.txt", "[DEBUG]", msg);
                mvwprintw(stdscr, 5, 20, "Sprite for: \'%s\'", desc);
                mvwprintw(stdscr, 7, 20, "(%i/%i)", sprite_count + 1,
                          sprite_total);
                refresh();
                for (int i = 0; i < 8; i++) {
                    if (strcmp(comm, "boss") == 0) {
                        strcpy(s, bosses_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "cons") == 0) {
                        strcpy(s, consumables_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "equip") == 0) {
                        strcpy(s, equips_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "eqzone") == 0) {
                        strcpy(s, equipzones_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "artf") == 0) {
                        strcpy(s, artifacts_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "enemy") == 0) {
                        strcpy(s, enemies_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "misc") == 0) {
                        strcpy(s, misc_sprites[sprite_count][i]);
                    }
                    //sprintf(msg,"[DEBUG]    Copied string: \'%s\'\n", s);
                    //debug_log("debug_log.txt",msg);

                    for (int j = 0; j < 13; j++) {
                        x = startx + 1 + j;
                        y = 1 + i;

                        print_encoded_char(spriteshow_win, y, x, s[j]);
                    }	//End for line print
                }		//End for sprite print

                wrefresh(spriteshow_win);
                //printf("\nPress Enter for next, q to quit.\n");
                int picked = 0;
                int go_previous = 0;
                int quit_show = 0;
                while (!picked
                       && ((c = wgetch(spriteshow_win)) != KEY_F(1))) {
                    if (c == 10 || c == KEY_RIGHT) {	//Enter, right
                        sprintf(msg, "Going to next sprite.\n");
                        log_tag("debug_log.txt", "[DEBUG]", msg);
                        wclear(spriteshow_win);
                        wrefresh(spriteshow_win);
                        picked = 1;
                        continue;	//Go to next sprite
                    } else if (c == 'q') {	//Quit
                        sprintf(msg, "Stopping query sprite show.\n");
                        log_tag("debug_log.txt", "[DEBUG]", msg);
                        picked = 1;
                        quit_show = 1;
                    } else if (c == KEY_LEFT) {	//Go to previous sprite
                        go_previous = 1;
                        sprintf(msg, "Going to previous sprite.\n");
                        log_tag("debug_log.txt", "[DEBUG]", msg);
                        picked = 1;
                    } else {	//Unexpected char
                        sprintf(msg,
                                "Wrong operation. Continuing with next sprite.\n");
                        log_tag("debug_log.txt", "\033[1;31m[ERROR]", msg);
                        picked = 1;
                        continue;	//We still go to next sprite
                    }
                }		//End while wait for user input
                if (go_previous) {
                    sprite_count--;
                    sprite_count--;
                    if (sprite_count < -1)
                        sprite_count = -1;
                    go_previous = 0;
                }
                if (quit_show) {
                    break;
                }
            }		//End for all sprites
            sprintf(msg, "End of sprite show.\n");
            log_tag("debug_log.txt", "[DEBUG]", msg);

            delwin(spriteshow_win);
            endwin();
        }
        break;
        case 'q': {
            return;
        }
        default: {		//Echo the passed char and ask for one more.
            char cmd[50];
            sprintf(cmd, "\necho \"%c\\n\"\n%c\n\n", ch[0], ch[0]);
            printf("%s", cmd);
            napms(500);
        }
        break;
        }			//Close switch on ch[0]
    }				//Close while !picked_debug_proc

    res = system("clear");
    sprintf(msg, "debug_generic() final system(\"clear\") res was (%i)", res);
    log_tag("debug_log.txt", "[DEBUG]", msg);
}

/**
 * Takes a Room, a Fighter, a Enemy and a Path pointers (and integers for current room and enemy indexes) and asks user input to execute debug actions.
 * @see Room
 * @see Fighter
 * @see Enemy
 * @see Path
 * @see checkremainder()
 * @see statReset()
 * @see statResetEnemy()
 * @see GET_CALLBACK()
 * @see unlock_special()
 * @see printCounters()
 * @see dropEquip()
 * @param gmst The Gamestate pointer.
 * @param player The Fighter pointer at hand.
 * @param e The Enemy pointer for current enemy.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param currentEnemyNum The index of current enemy.
 * @param kls The Koliseo used for allocations.
 * @param t_kls The Koliseo_Temp used for temporary allocations.
 */
void debug_enemies_room(Gamestate *gmst, Room *room, Fighter *player, Enemy *e,
                        Path *p, int roomIndex, int currentEnemyNum,
                        Koliseo *kls, Koliseo_Temp *t_kls)
{

    char msg[200];
    char ch[25];
    int picked_debug_proc = 0;
#ifndef _WIN32
    struct utsname uts;
    uname(&uts);
    sprintf(msg, "debug_enemies_room() loaded utsname using uname().\n");
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "System is %s\n", uts.sysname);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "OS Release is %s\n", uts.release);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "OS Version is %s\n", uts.version);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "Machine is %s\n", uts.machine);
    log_tag("debug_log.txt", "[DEBUG]", msg);
#endif

    int res = system("clear");
    sprintf(msg, "debug_enemies_room() system(\"clear\") res was (%i)", res);
    log_tag("debug_log.txt", "[DEBUG]", msg);

    int c = 0, n = -1;
    while (!picked_debug_proc) {
        int res = system("clear");
        sprintf(msg, "debug_enemies_room() 2 system(\"clear\") res was (%i)",
                res);
        log_tag("debug_log.txt", "[DEBUG]", msg);
        printf("\n\
	What do you want to do? ('q' to quit)\n\
		[\n\
			'0':  Give xp points\t'1':  Give consumable\n\
			'2':  Reset stats\t'3':  Reset Enemy\n\
			'4':  Alter luck\t'5':  Give coins\n\
			'6':  Unlock special\t'7':  Unlock Artifact\n\
			'8':  Print counters\t'9':  Give Equip\n\
			's':  Sprites slideshow\t'd': Dump debug symbols\n\
			'f':  Show foes info\t'g': Toggle godmode\n\
			'A':  Toggle autosave\t'Q': Toggle fast quit\n\
			'L':  Toggle logging\t'k': Log passed Koliseo info\n\
			't': Log global temporary_kls Koliseo info\n\
			'K': Log usage stats for passed kls to debug log file.\n\
			'T': Log usage stats for temporary_kls to debug log file.\n\
			'G': Log Gamestate to debug log file.\n\
			'q': Quit\t{Return}  Process your input line.\n\
		]\n\n\
	[%s@debug-func]$ ", player->name);

        char *fgetsres = fgets(ch, sizeof ch, stdin);
        sprintf(msg, "debug_enemies_room() fgets() res was (%s)", fgetsres);
        log_tag("debug_log.txt", "[DEBUG]", msg);
        switch (ch[0]) {
        case '0': {
            picked_debug_proc = 1;
            int xp;
            do {
                printf("\nHow much?\n");
            } while ((c = scanf("%i", &xp)) != 1 || xp < 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            checkremainder(player, xp);
        }
        break;
        case '1': {
            picked_debug_proc = 1;
            int q = -1;
            do {
                printf("\nInsert consumable number:\n");
            } while ((c = scanf("%i", &n)) != 1 || n > CONSUMABLESMAX
                     || n < 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            do {
                printf("\nInsert quantity:\n");
            } while ((c = scanf("%i", &q)) != 1 && q <= 0);
            res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() 2 scanf() res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            Consumable *c = (Consumable *) player->consumablesBag[n];
            c->qty += q;
            player->stats->consumablesfound += q;
        }
        break;
        case '2': {
            picked_debug_proc = 1;
            statReset(player, 1);
        }
        break;
        case '3': {
            picked_debug_proc = 1;
            statResetEnemy(e, 1);
        }
        break;
        case '4': {
            picked_debug_proc = 1;
            printf("\nCurrent luck: %i\tRL: %i\n", player->luck, p->luck);

            do {
                printf("\nInsert new luck:\n");
            } while ((c = scanf("%i", &n)) != 1 && n > 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() 3 scanf() res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            //FIXME:
            //Copy-pasted the calc for player luck... might need a function
            p->luck = n;

            player->luck = (p->luck * MAXPLAYERLUCK) / MAXLUCK;
        }
        break;
        case '5': {
            picked_debug_proc = 1;
            /* Old code invoking the macro for special moves directly. Used for testing.
             * int s = -1;
             * printf("Insert special num:\n");
             * scanf("%i",&s);
             * scanf("%*c");
             * printf("Read: %i\n",s);
             * GET_CALLBACK(s,callback_special_t)(player,e,p,roomIndex,currentEnemyNum);
             */

            //printActivePerks(player);
            int c;
            int n = -1;
            do {
                printf("\nInsert coin number (0<n<100):\n");
            } while ((c = scanf("%i", &n)) < 0 || c > 3 || n < 0
                     || n > 100);
            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() 4 scanf() res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            player->balance += n;
        }
        break;
        case '6': {
            picked_debug_proc = 1;
            unlockSpecial(player);
        }
        break;
        case '7': {
            picked_debug_proc = 1;
            int n = -1;
            do {
                printf("\nInsert artifact number (0<n<%i):\n",
                       ARTIFACTSMAX + 1);
            } while ((c = scanf("%i", &n)) != 1 || n > ARTIFACTSMAX
                     || n < 0);

            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            player->artifactsBag[n]->qty += 1;
            player->artifactsBag[n]->active = 0;
            player->stats->artifactsfound += 1;
        }
        break;
        case '8': {
            picked_debug_proc = 1;
            int res = system("clear");
            sprintf(msg,
                    "debug_enemies_room() 3 system(\"clear\") res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            printf("\nPlayer Counters:\n");
            printCounters((Turncounter **) player->counters);
            printf("\nEnemy (%s) Counters:\n", stringFromEClass(e->class));
            printCounters((Turncounter **) e->counters);
            printf("\nPress Enter to resume game");
            res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() 5 scanf() res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case '9': {
            picked_debug_proc = 1;
            int q = -1;
            int c = -1;
            do {
                printf("\nInsert quantity:\n");
            } while ((c = scanf("%i", &q)) != 1 && q <= 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() 6 scanf() res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);

            //TODO: handle dropping an equip with a new notification window
            //for (int i=0; i<q;i++) {dropEquip(player,1,notify_win);};
            log_tag("debug_log.txt", "[DEBUG]", "TODO: not working ATM.");
        }
        break;
        case 'd': {
            picked_debug_proc = 1;
            printf("\nVERSION:    %s\n", VERSION);
#ifndef _WIN32
            printf("\nSystem:    %s\n", uts.sysname);
            printf("\nOS Release:    %s\n", uts.release);
            printf("\nOS Version:    %s\n", uts.version);
            printf("\nMachine:    %s\n", uts.machine);
#endif
            printf("\nGAMEMODE:    %s\n", stringFromGamemode(GAMEMODE));
            printf("\nPath->current_saveslot->save_path:    %s\n",
                   p->current_saveslot->save_path);
            printf("\nGS_AUTOSAVE_ON:    %i\n", GS_AUTOSAVE_ON);
            printf("\nG_FASTQUIT_ON:    %i\n", G_FASTQUIT_ON);
            printf("\nG_DEBUG_ON:    %i\n", G_DEBUG_ON);
            printf("\nG_LOG_ON:    %i\n", G_LOG_ON);
            printf("\nG_GODMODE_ON:    %i\n", G_GODMODE_ON);
            printf("\nG_EXPERIMENTAL_ON:    %i\n", G_EXPERIMENTAL_ON);
            printf("\nG_DEBUG_ROOMTYPE_ON:    %i\n", G_DEBUG_ROOMTYPE_ON);
            if (G_DEBUG_ROOMTYPE_ON == 1) {
                printf("\nG_DEBUG_ROOMTYPE:    %i\n", G_DEBUG_ROOMTYPE);
            }
            printf("\nG_DEBUG_ENEMYTYPE_ON:    %i\n", G_DEBUG_ENEMYTYPE_ON);
            if (G_DEBUG_ENEMYTYPE_ON == 1) {
                printf("\nG_DEBUG_ENEMYTYPE:    %i\n", G_DEBUG_ENEMYTYPE);
            }
            printf("\nPress Enter to resume game.\n");
            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() 7 scanf() res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'f': {
            clear();
            refresh();
            debug_printFoeParty(room->foes);
        }
        break;
        case 'G': {
            log_tag("debug_log.txt", "[DEBUG]", "Logging Gamestate");
            dbg_Gamestate(gmst);
            log_tag("debug_log.txt", "[DEBUG]", "Done logging Gamestate");
        }
        break;
        case 'T': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }

            fprintf(kls_file, "--BEGIN debug of temporary_kls--\n");
            for (int i = HR_Path; i < HLP_MAX_INDEX + 99; i++) {
                ptrdiff_t usage = kls_type_usage(i, temporary_kls);
#ifndef _WIN32
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %li }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#else
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %lli }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#endif
            }
            fprintf(kls_file, "--END debug of temporary_kls--\n\n");

            fclose(kls_file);
        }
        break;
        case 't': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }
            fprintf(kls_file, "--BEGIN debug of temporary_kls--\n");
            print_kls_2file(kls_file, temporary_kls);
            kls_showList_toFile(temporary_kls->regs, kls_file);
            kls_usageReport_toFile(temporary_kls, kls_file);
            fprintf(kls_file, "--END debug of temporary_kls--\n\n");
            WINDOW *win = NULL;
            /* Initialize curses */
            clear();
            refresh();
            start_color();
            cbreak();
            noecho();
            keypad(stdscr, TRUE);
            win = newwin(20, 50, 1, 2);
            keypad(win, TRUE);
            wclear(win);
            wrefresh(win);
            kls_showList_toWin(temporary_kls, win);
            delwin(win);
            endwin();

            fclose(kls_file);
        }
        break;
        case 'K': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }

            fprintf(kls_file, "--BEGIN debug of passed kls--\n");
            for (int i = HR_Path; i < HLP_MAX_INDEX + 99; i++) {
                ptrdiff_t usage = kls_type_usage(i, kls);
#ifndef _WIN32
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %li }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#else
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %lli }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#endif
            }
            fprintf(kls_file, "--END debug of passed kls--\n\n");

            fclose(kls_file);
        }
        break;
        case 'k': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }
            fprintf(kls_file, "--BEGIN debug of passed kls--\n");
            print_kls_2file(kls_file, kls);
            kls_showList_toFile(kls->regs, kls_file);
            kls_usageReport_toFile(kls, kls_file);
            fprintf(kls_file, "--END debug of passed kls--\n\n");
            WINDOW *win = NULL;
            /* Initialize curses */
            clear();
            refresh();
            start_color();
            cbreak();
            noecho();
            keypad(stdscr, TRUE);
            win = newwin(20, 50, 1, 2);
            keypad(win, TRUE);
            wclear(win);
            wrefresh(win);
            kls_showList_toWin(kls, win);
            delwin(win);
            endwin();
            fclose(kls_file);
        }
        break;
        case 'A': {
            GS_AUTOSAVE_ON = (GS_AUTOSAVE_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_AUTOSAVE_ON, new value: (%i)",
                    GS_AUTOSAVE_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'Q': {
            G_FASTQUIT_ON = (G_FASTQUIT_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_FASTQUIT_ON, new value: (%i)",
                    G_FASTQUIT_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'L': {
            G_LOG_ON = (G_LOG_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_LOG_ON, new value: (%i)", G_LOG_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'g': {
            G_GODMODE_ON = (G_GODMODE_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_GODMODE_ON, new value: (%i)",
                    G_GODMODE_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 's': {		//Query sprites slideshow
            picked_debug_proc = 1;
            int res = system("clear");
            sprintf(msg,
                    "debug_enemies_room() 4 system(\"clear\") res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            int comm_len = -1;
            int picked_comm = 0;
            char comm[15] = "empty";
            char *desc = NULL;
            int sprite_total = -1;
            int tot_cons = CONSUMABLESMAX + 1;
            int tot_enemy = ENEMYCLASSESMAX + 1;
            int tot_boss = BOSSCLASSESMAX + 1;
            int tot_art = ARTIFACTSMAX + 1;
            int tot_eq = EQUIPSMAX + 1;
            int tot_eqzones = EQUIPZONES + 1;
            int tot_misc = MISC_SPRITES_MAX + 1;
            int allsprites =
                tot_cons +
                tot_enemy +
                tot_boss + tot_art + tot_eq + tot_eqzones + tot_misc;

            printf("\nConsumables sprites: \t%d\
\nArtifacts sprites: \t%d\
\nEquips sprites: \t%d\
\nEquip zone sprites: \t%d\
\nEnemies sprites: \t%d\
\nBosses sprites: \t%d\
\nOther misc sprites: \t%d\n", tot_cons, tot_art, tot_eq, tot_eqzones, tot_enemy, tot_boss, tot_misc);
            printf("--------------------------");
            printf("\nTotal sprites: \t\t%d\n", allsprites);
            printf
            ("\nInsert sprites query, q to quit: (q|boss|cons|equip|eqzone|artf|enemy|misc):\n\n[%s@spriteshow]$ ",
             player->name);
            while (!picked_comm && (comm_len = scanf("%10s", comm)) > -1) {
                int res = scanf("%*c");
                sprintf(msg, "debug_enemies_room() 7 scanf() res was (%i)",
                        res);
                log_tag("debug_log.txt", "[DEBUG]", msg);
                if (strcmp(comm, "q") == 0) {
                    return;
                } else if (strcmp(comm, "boss") == 0) {
                    picked_comm = 1;
                    sprite_total = BOSSCLASSESMAX + 1;
                } else if (strcmp(comm, "cons") == 0) {
                    picked_comm = 1;
                    sprite_total = CONSUMABLESMAX + 1;
                } else if (strcmp(comm, "equip") == 0) {
                    picked_comm = 1;
                    sprite_total = EQUIPSMAX + 1;
                } else if (strcmp(comm, "eqzone") == 0) {
                    picked_comm = 1;
                    sprite_total = EQUIPZONES + 1;
                } else if (strcmp(comm, "artf") == 0) {
                    picked_comm = 1;
                    sprite_total = ARTIFACTSMAX + 1;
                } else if (strcmp(comm, "enemy") == 0) {
                    picked_comm = 1;
                    sprite_total = ENEMYCLASSESMAX + 1;
                } else if (strcmp(comm, "misc") == 0) {
                    picked_comm = 1;
                    sprite_total = MISC_SPRITES_MAX + 1;
                } else {
                    printf
                    ("\nInsert sprites query, q to quit: (q|boss|cons|equip|eqzone|artf|enemy|misc):\n\n[%s@spriteshow]$ ",
                     player->name);
                }
            }
            int sprite_count = -1;
            int c = -1;
            char s[20];
            int startx = 0;
            int x, y;
            WINDOW *spriteshow_win;
            /* Initialize curses */
            //initscr();
            start_color();
            clear();
            refresh();
            cbreak();
            noecho();
            keypad(stdscr, TRUE);
            spriteshow_win = newwin(19, 19, 2, 2);
            keypad(spriteshow_win, TRUE);
            for (sprite_count = 0; sprite_count < sprite_total;
                 sprite_count++) {
                if (strcmp(comm, "boss") == 0) {
                    desc = stringFromBossClass(sprite_count);
                } else if (strcmp(comm, "cons") == 0) {
                    desc = stringFromConsumables(sprite_count);
                } else if (strcmp(comm, "equip") == 0) {
                    desc = stringFromEquips(sprite_count);
                } else if (strcmp(comm, "eqzone") == 0) {
                    desc = stringFromEquipzones(sprite_count);
                } else if (strcmp(comm, "artf") == 0) {
                    desc = stringFromArtifacts(sprite_count);
                } else if (strcmp(comm, "enemy") == 0) {
                    desc = stringFromEClass(sprite_count);
                } else if (strcmp(comm, "misc") == 0) {
                    desc = "Misc";
                }
                wclear(spriteshow_win);
                wrefresh(spriteshow_win);
                wclear(stdscr);
                wrefresh(stdscr);
                sprintf(msg, "Showing sprite n.%d, class is: %s\n",
                        sprite_count, desc);
                log_tag("debug_log.txt", "[DEBUG]", msg);
                mvwprintw(stdscr, 5, 20, "Sprite for: \'%s\'", desc);
                mvwprintw(stdscr, 7, 20, "(%i/%i)", sprite_count + 1,
                          sprite_total);
                refresh();
                for (int i = 0; i < 8; i++) {
                    if (strcmp(comm, "boss") == 0) {
                        strcpy(s, bosses_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "cons") == 0) {
                        strcpy(s, consumables_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "equip") == 0) {
                        strcpy(s, equips_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "eqzone") == 0) {
                        strcpy(s, equipzones_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "artf") == 0) {
                        strcpy(s, artifacts_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "enemy") == 0) {
                        strcpy(s, enemies_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "misc") == 0) {
                        strcpy(s, misc_sprites[sprite_count][i]);
                    }
                    //sprintf(msg,"[DEBUG]    Copied string: \'%s\'\n", s);
                    //debug_log("debug_log.txt",msg);

                    for (int j = 0; j < 13; j++) {
                        x = startx + 1 + j;
                        y = 1 + i;

                        print_encoded_char(spriteshow_win, y, x, s[j]);
                    }	//End for line print
                }		//End for sprite print

                wrefresh(spriteshow_win);
                //printf("\nPress Enter for next, q to quit.\n");
                int picked = 0;
                int go_previous = 0;
                int quit_show = 0;
                while (!picked
                       && ((c = wgetch(spriteshow_win)) != KEY_F(1))) {
                    if (c == 10 || c == KEY_RIGHT) {	//Enter, right
                        sprintf(msg, "Going to next sprite.\n");
                        log_tag("debug_log.txt", "[DEBUG]", msg);
                        wclear(spriteshow_win);
                        wrefresh(spriteshow_win);
                        picked = 1;
                        continue;	//Go to next sprite
                    } else if (c == 'q') {	//Quit
                        sprintf(msg, "Stopping query sprite show.\n");
                        log_tag("debug_log.txt", "[DEBUG]", msg);
                        picked = 1;
                        quit_show = 1;
                    } else if (c == KEY_LEFT) {	//Go to previous sprite
                        go_previous = 1;
                        sprintf(msg, "Going to previous sprite.\n");
                        log_tag("debug_log.txt", "[DEBUG]", msg);
                        picked = 1;
                    } else {	//Unexpected char
                        sprintf(msg,
                                "Wrong operation. Continuing with next sprite.\n");
                        log_tag("debug_log.txt", "\033[1;31m[ERROR]", msg);
                        picked = 1;
                        continue;	//We still go to next sprite
                    }
                }		//End while wait for user input
                if (go_previous) {
                    sprite_count--;
                    sprite_count--;
                    if (sprite_count < -1)
                        sprite_count = -1;
                    go_previous = 0;
                }
                if (quit_show) {
                    break;
                }
            }		//End for all sprites
            sprintf(msg, "End of sprite show.\n");
            log_tag("debug_log.txt", "[DEBUG]", msg);

            delwin(spriteshow_win);
            endwin();
        }
        break;
        case 'q': {
            return;
        }
        default: {		//Echo the passed char and ask for one more.
            char cmd[50];
            sprintf(cmd, "\necho \"%c\\n\"\n%c\n\n", ch[0], ch[0]);
            printf("%s", cmd);
            napms(500);
        }
        break;
        }			//Close switch on ch[0]
    }				//Close while !picked_debug_proc

}

#endif // HELAPORDO_CURSES_BUILD
