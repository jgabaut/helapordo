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
#include "rooms.h"


#ifdef HELAPORDO_CURSES_BUILD
/**
 * Takes an integer for room index and a Fighter and a Path pointers and handles the progression of the room.
 * It also takes a loadInfo pointer to a struct used for loading a game, and a 4D char array containing animations for all fighters, preloaded in the prep section of gameloop().
 * User is asked for turn choice and according to the input, the corresponding action will be called.
 * @see Fighter
 * @see Path
 * @see handleConsumables()
 * @see handleEquips()
 * @see handleStats()
 * @see handleArtifacts()
 * @param gamestate The pointer to Gamestate.
 * @param room The pointer to current room.
 * @param index The index of current room.
 * @param p The Path pointer.
 * @param player The Fighter pointer at hand.
 * @param load_info A pointer to loadInfo struct containing stuff for loading a game.
 * @param kls The Koliseo used for allocations.
 * @param t_kls The Koliseo_Temp used for temporary allocations.
 * @param fighter_sprites The sprites array for all fighter classes.
 * @see gameloop()
 * @return When exiting room, should return NO_DMG.
 */
int handleRoom_Home(Gamestate *gamestate, Room *room, int index, Path *p,
                    Fighter *player, loadInfo *load_info,
                    char fighter_sprites[CLASSESMAX +
                            1][MAXFRAMES][MAXROWS][MAXCOLS],
                    Koliseo *kls, Koliseo_Temp *t_kls)
{
    Enemy *dummy_enemy = NULL;
    Boss *dummy_boss = NULL;
    FILE *dummy_savefile = NULL;
    FILE *save_file;
    FILE *autosave_file;
    WINDOW *dummy_notify_win = NULL;
    foeTurnOption_OP dummy_foe_op = FOE_OP_INVALID;
    skillType dummy_skill_pick = -1;
    //Declare turnOP_args
    turnOP_args *args =
        init_turnOP_args(gamestate, player, p, room, load_info, dummy_enemy,
                         dummy_boss, dummy_savefile, dummy_notify_win, t_kls,
                         dummy_foe_op, dummy_skill_pick);

    //Strings for turn menu choices
    char *choices[] = {
        "Explore",
        "Artifacts",
        "Equips",
        "Perks",
        "Save",
        "Stats",
        "Quit",
        (char *)NULL,
    };

    char *debug_choices[] = {
        "Debug",
        (char *)NULL,
    };

    fightResult fightStatus = FIGHTRES_NO_DMG;
    log_tag("debug_log.txt", "[ROOM]", "New HOME room, index %i", room->index);

    if (GS_AUTOSAVE_ON == 1 && GAMEMODE != Rogue) {
        log_tag("debug_log.txt", "[DEBUG]", "Doing autosave.");

        char path_to_autosave_file[800];
        char autosave_static_path[500];
        char autosave_file_name[300];
        strcpy(autosave_file_name, p->current_saveslot->save_path);

        // Set static_path value to the correct static dir path
        resolve_staticPath(autosave_static_path);

        sprintf(path_to_autosave_file, "%s/%s", autosave_static_path,
                autosave_file_name);

        autosave_file = fopen(path_to_autosave_file, "w");
        if (autosave_file == NULL) {
            fprintf(stderr, "[ERROR]    Can't open save file %s!\n",
                    path_to_autosave_file);
            exit(EXIT_FAILURE);
        } else {
            log_tag("debug_log.txt", "[TURNOP]",
                    "Assigning autosave_file pointer to args->save_file. Path: [%s]",
                    path_to_autosave_file);
            args->save_file = autosave_file;
        }
        turnOP(turnOP_from_turnOption(SAVE), args, kls, t_kls);
        fclose(autosave_file);
        log_tag("debug_log.txt", "[DEBUG]", "Closed autosave_file pointer.");
        log_tag("debug_log.txt", "[DEBUG]", "Done autosave.");
    }

    turnOption choice = 999;

    ITEM **menu_items;
    MENU *home_menu;
    WINDOW *menu_win;
    WINDOW *home_win;
    int n_choices, c;

    while ((choice != EXPLORE)) {
        choice = 999;
        log_tag("debug_log.txt", "[DEBUG]",
                "Initialising curses for handleRoom_Home()");
        /* Initialize curses */
        //initscr();
        start_color();
        clear();
        refresh();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);

        int cursorCheck = curs_set(0);	// We make the cursor invisible or return early with the error

        if (cursorCheck == ERR) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Failed curs_set(0) at handleRoom_Home()");
            return S4C_ERR_CURSOR;	//fprintf(stderr,"animate => Terminal does not support cursor visibility state.\n");
        }

        /* Create turn menu items */
        n_choices = ARRAY_SIZE(choices);
        int debug_n_choices = 0;
        if (G_DEBUG_ON) {	//We have to include debug menu options
            debug_n_choices = ARRAY_SIZE(debug_choices);
        }
        menu_items =
            (ITEM **) calloc(n_choices + debug_n_choices, sizeof(ITEM *));

        // Prepare menu items
        for (int k = 0; k < n_choices; k++) {
            if (G_DEBUG_ON && k == n_choices - 1) {
                //We skip adding the null string item
            } else {
                menu_items[k] = new_item(choices[k], choices[k]);
            }
            /* Set the user pointer */
            //set_item_userptr(my_items[i]);
        }
        // Prepare debug items
        if (G_DEBUG_ON) {
            for (int k = 0; k < debug_n_choices; k++) {
                menu_items[k + n_choices - 1] =
                    new_item(debug_choices[k], debug_choices[k]);
            }
            /* Set the user pointer */
            //set_item_userptr(my_items[i]);
        }

        /* Create menu */
        home_menu = new_menu((ITEM **) menu_items);

        /* Set description off */
        menu_opts_off(home_menu, O_SHOWDESC);

        /* Create the window to be associated with the menu */
        menu_win = newwin(9, 11, 5, 35);
        //nodelay(menu_win,TRUE); //We make getch non-blocking
        keypad(menu_win, TRUE);

        /* Set main window and sub window */
        set_menu_win(home_menu, menu_win);
        set_menu_sub(home_menu, derwin(menu_win, 8, 10, 1, 1));
        set_menu_format(home_menu, 7, 1);

        /* Set menu mark to the string "" */
        set_menu_mark(home_menu, "");

        /* Print a border around the menu */
        box(menu_win, 0, 0);

        /* Set menu colors */
        set_menu_fore(home_menu, COLOR_PAIR(S4C_RED));
        set_menu_back(home_menu, COLOR_PAIR(S4C_WHITE));

        //mvprintw(LINES - 1, 0, "Arrow Keys to navigate (F1 to Exit)");
        //attroff(COLOR_PAIR(2));

        /* Post the menu */
        post_menu(home_menu);
        wrefresh(menu_win);

        home_win = newwin(12, 24, 2, 5);
        scrollok(home_win, TRUE);
        wprintw(home_win, "\n  This is your base.");
        wprintw(home_win, "\n  Take a nap before exploring.");
        wrefresh(home_win);
        refresh();

        int picked = 0;
        int picked_explore = 0;

        while (!picked && (c = wgetch(menu_win)) != KEY_F(1) && !picked_explore) {
            switch (c) {
            case KEY_DOWN:
                menu_driver(home_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(home_menu, REQ_UP_ITEM);
                break;
            case KEY_LEFT: {	/*Left option pick */
                ITEM *cur;
                cur = current_item(home_menu);
                choice = getTurnChoice((char *)item_name(cur));
                log_tag("debug_log.txt", "[DEBUG]",
                        "Left on choice: [ %s ] value (%i)", item_name(cur),
                        choice);
                if (choice == EQUIPS) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Should do something");
                }
            }
            break;
            case KEY_RIGHT: {	/*Right option pick */
                ITEM *cur;
                cur = current_item(home_menu);
                choice = getTurnChoice((char *)item_name(cur));
                log_tag("debug_log.txt", "[DEBUG]",
                        "Right on choice: [ %s ] value (%i)",
                        item_name(cur), choice);
                if (choice == EQUIPS) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Should do something");
                }
            }
            break;
            case KEY_NPAGE:
                menu_driver(home_menu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:
                menu_driver(home_menu, REQ_SCR_UPAGE);
                break;
            case 10: {	/* Enter */
                picked = 1;
                ITEM *cur;

                //move(18,47);
                //clrtoeol();
                cur = current_item(home_menu);
                //mvprintw(18, 47, "Item selected is : %s", item_name(cur));
                choice = getTurnChoice((char *)item_name(cur));
                pos_menu_cursor(home_menu);
                refresh();
            };
            break;
            case 'q': {
                if (G_FASTQUIT_ON == 1) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Player used q to quit from home room menu.");
                    picked = 1;
                    choice = getTurnChoice("Quit");
                    pos_menu_cursor(home_menu);
                    refresh();
                } else {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Player used q in home room menu, but G_FASTQUIT_ON was not 1.");
                }
            }
            break;
            default: {
                log_tag("debug_log.txt", "[DEBUG]",
                        "Invalid keystroke in home room menu");
            }
            break;
            }
            wrefresh(menu_win);
            if (c == 10) {	// Player char was enter
                if (turnOP_from_turnOption(choice) == OP_EXPLORE) {
                    picked_explore = 1;
                }
                if (choice == SAVE) {
                    char path_to_savefile[800];
                    char static_path[500];
                    char savefile_name[300];
                    sprintf(savefile_name, "%s",
                            p->current_saveslot->save_path);

                    // Set static_path value to the correct static dir path
                    resolve_staticPath(static_path);
                    log_tag("debug_log.txt", "[DEBUG]",
                            "handleRoom_Home:  savefile_name is [%s].",
                            savefile_name);

                    sprintf(path_to_savefile, "%s/%s", static_path,
                            savefile_name);

                    save_file = fopen(path_to_savefile, "w");
                    if (save_file == NULL) {
                        fprintf(stderr, "[ERROR]    Can't open save file %s!\n",
                                path_to_savefile);
                        exit(EXIT_FAILURE);
                    } else {
                        log_tag("debug_log.txt", "[TURNOP]",
                                "Assigning save_file pointer to args->save_file. Path: [%s]",
                                path_to_savefile);
                        args->save_file = save_file;
                    }
                }
                turnOP(turnOP_from_turnOption(choice), args, kls, t_kls);
                if (choice == SAVE) {
                    fclose(save_file);
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Closed save_file pointer.");
                }
            }			//End if Player char was enter
        }

        // Unpost menu and free all the memory taken up
        unpost_menu(home_menu);
        free_menu(home_menu);
        log_tag("debug_log.txt", "[FREE]", "Freed home menu");
        int totalChoices = n_choices + debug_n_choices;
        if (G_DEBUG_ON) {
            totalChoices -= 1;	//We subtract 1 to account for the discarded null choice in the base option set.
        }
        for (int k = 0; k < totalChoices; k++) {
            free_item(menu_items[k]);
            log_tag("debug_log.txt", "[FREE]", "Freed %i home menu item", k);
        }
        free(menu_items);
        delwin(home_win);
        endwin();
        log_tag("debug_log.txt", "[DEBUG]",
                "Ended window mode for handleRoom_Home()");

    }
    //Free turnOP_args
    //free(args);
    log_tag("debug_log.txt", "[FREE]", "handleRoom_Home():  Freed turnOP_args");
    log_tag("debug_log.txt", "[DEBUG]",
            "Ended handleRoom_Home(), returning fightStatus [%i]", fightStatus);
    return fightStatus;
}

/**
 * Takes an integer for room index and a Fighter and a Path pointers and handles the progression of the room.
 * It also takes a loadInfo pointer to a struct used for loading a game, and a 4D char array containing animations for all enemies and fighters, preloaded in the prep section of gameloop().
 * The number of enemies in the room is randomised, up to 6. If it's a beast room (index multiple of BEASTROOM), there will be 1 extra enemy.
 * For each enemy, their stats are prepared before resetting permboosts and artifacts and applying them again.
 * User is asked for turn choice and according to the input, the corresponding action will be called.
 * Then, status is applied and counters are updated.
 * On enemy death, there's a chance to call dropConsumable, dropEquip or dropArtifact (guaranteed for beast enemies).
 * When the last enemy is dead, the function returns KILL_DONE.
 * @see Fighter
 * @see Path
 * @see BEASTROOM
 * @see prepareRoomEnemy()
 * @see printSpawnMessage()
 * @see resetPermBoosts()
 * @see resetArtifacts()
 * @see applyArtifacts()
 * @see applyPermBoosts()
 * @see printBattleStats()
 * @see getTurnChoice()
 * @see dropConsumable()
 * @see dropEquip()
 * @see dropArtifact()
 * @see giveXp()
 * @see e_death()
 * @see death()
 * @see fight()
 * @see handleSpecials()
 * @see handleConsumables()
 * @see handleEquips()
 * @see handleStats()
 * @see debug_enemies_room()
 * @see handleArtifacts()
 * @see applyStatus()
 * @see applyEStatus()
 * @param gamestate The pointer to Gamestate.
 * @param room The pointer to current room.
 * @param index The index of current room.
 * @param p The Path pointer.
 * @param player The Fighter pointer at hand.
 * @param load_info A pointer to loadInfo struct containing stuff for loading a game.
 * @param enemy_sprites The sprites array for all enemies.
 * @param fighter_sprites The sprites array for all fighter classes.
 * @param kls The Koliseo used for allocations.
 * @param t_kls The Koliseo_Temp used for temp allocations.
 * @see gameloop()
 * @see turnOP()
 * @see enemyClass
 * @return When room is cleared, should return KILL_DONE.
 */
int handleRoom_Enemies(Gamestate *gamestate, Room *room, int index, Path *p,
                       Fighter *player, loadInfo *load_info,
                       char enemy_sprites[ENEMYCLASSESMAX +
                               1][MAXFRAMES][MAXROWS][MAXCOLS],
                       char fighter_sprites[CLASSESMAX +
                               1][MAXFRAMES][MAXROWS][MAXCOLS],
                       Koliseo *kls, Koliseo_Temp *t_kls)
{

    Boss *dummy_boss = NULL;
    Enemy *args_enemy = NULL;
    FILE *args_save_file = NULL;
    WINDOW *args_notify_win = NULL;
    foeTurnOption_OP dummy_foe_op = FOE_OP_INVALID;
    skillType dummy_picked_skill = -1;

    //Declare turnOP_args
    turnOP_args *args =
        init_turnOP_args(gamestate, player, p, room, load_info, args_enemy,
                         dummy_boss, args_save_file, args_notify_win, t_kls,
                         dummy_foe_op, dummy_picked_skill);

    //Strings for turn menu choices
    char *choices[] = {
        "Fight",
        "Special",
        "Consumables",
        "Artifacts",
        "Equips",
        "Perks",
        "Stats",
        "Save",
        "Quit",
        (char *)NULL,
    };

    char *debug_choices[] = {
        "Debug",
        (char *)NULL,
    };

    //system("clear");

    //printf("\n\tYou are now in room %i.\n",index);
    int enemies;

    enemies = room->enemyTotal;

    //At least 2 enemies in every Xth room, up to 7
    //if (index % BEASTROOM == 0) {
    //      enemies++;
    //} ;

    //printf("\tThere are %i enemies.\n",enemies);

    //screenTime(1);

    //system("clear");
    OP_res fightStatus = OP_RES_NO_DMG;
    log_tag("debug_log.txt", "[ROOM]", "New room, index %i, %i enemies",
            room->index, enemies);

    for (int i = 0; i < enemies;) {

        update_Gamestate(gamestate, 1, room->class, room->index, i,
                         gamestate->current_floor);

        fightStatus = OP_RES_NO_DMG;
        Enemy *e = room->enemies[i];
        //Update turnOP_args->enemy pointer
        args->enemy = e;
        log_tag("debug_log.txt", "[TURNOP]",
                "Assigned enemy %s to turnOP_args: args->enemy == [%s]",
                stringFromEClass(e->class),
                stringFromEClass(args->enemy->class));
        log_tag("debug_log.txt", "[DEBUG]", "Declared enemy %s",
                stringFromEClass(e->class));
        printSpawnMessage(e, index, i);

        //Reset player permboosts... and calc them again...
        resetPermboosts(player);

        //Reset artifacts state, for the ones we have unlocked
        resetArtifactsState(player);
        //Apply artifacts
        Boss *dummy = NULL;
        int isBoss = 0;
        applyArtifacts(player, e, dummy, isBoss);

        //Calc permboosts and turn boosts
        //Turn boosts

        //Perm boosts
        applyPermboosts(player);

        //Fight preparation is done, on to turn choice
        turnOption choice = 999;
        WINDOW *my_wins[3];
        for (int winindex = 0; winindex < 3; winindex++) {
            my_wins[winindex] = NULL;
        }
        WINDOW *enemy_animation_win = NULL;
        WINDOW *fighter_animation_win = NULL;
        WINDOW *notifications_win = NULL;

        //fprintf(stderr,"%i\n",animation_thread_args->win);
        //fprintf(stderr,"%i\n",animation_thread_args->stop_animation);

        ITEM **my_items;
        MENU *my_menu;
        WINDOW *my_menu_win = NULL;
        int n_choices, c;

        log_tag("debug_log.txt", "[ANIMATE]", "Declared new enemy animation.");

        log_tag("debug_log.txt", "[ANIMATE]",
                "Declared new fighter animation.");
        int debug_n_choices = 0;

        //Keeps track of current animation frame
        int frame_counter = 0;
        int animation_loops_done = 0;

        char time_str[20] = {0};
        clock_t run_time = {0};
#ifndef _WIN32
        long time_spent;
#else
        long long time_spent;
#endif

        while (!
               (fightStatus == OP_RES_DEATH || fightStatus == OP_RES_KILL_DONE
                || choice == QUIT)) {
            if (choice != FIGHT) {
                /* Initialize curses */
                //initscr();
                start_color();
                clear();
                refresh();
                cbreak();
                noecho();
                keypad(stdscr, TRUE);

                int cursorCheck = curs_set(0);	// We make the cursor invisible or return early with the error

                if (cursorCheck == ERR) {
                    log_tag("debug_log.txt", "[ERROR]",
                            "Failed curs_set(0) at handleRoom_Enemies()");
                    return S4C_ERR_CURSOR;	//fprintf(stderr,"animate => Terminal does not support cursor visibility state.\n");
                }

                /* Create turn menu items */
                n_choices = ARRAY_SIZE(choices);
                if (G_DEBUG_ON) {	//We have to include debug menu options
                    debug_n_choices = ARRAY_SIZE(debug_choices);
                }
                my_items =
                    (ITEM **) calloc(n_choices + debug_n_choices,
                                     sizeof(ITEM *));

                // Prepare menu items
                for (int k = 0; k < n_choices; k++) {
                    if (G_DEBUG_ON && k == n_choices - 1) {
                        //We skip adding the null string item
                    } else {
                        my_items[k] = new_item(choices[k], choices[k]);
                    }
                    /* Set the user pointer */
                    //set_item_userptr(my_items[i]);
                }
                // Prepare debug items
                if (G_DEBUG_ON) {
                    for (int k = 0; k < debug_n_choices; k++) {
                        my_items[k + n_choices - 1] =
                            new_item(debug_choices[k], debug_choices[k]);
                    }
                    /* Set the user pointer */
                    //set_item_userptr(my_items[i]);
                }

                /* Create menu */
                my_menu = new_menu((ITEM **) my_items);

                /* Set description off */
                menu_opts_off(my_menu, O_SHOWDESC);

                /* Create the window to be associated with the menu */
                my_menu_win = newwin(9, 13, 14, 35);
                nodelay(my_menu_win, TRUE);	//We make getch non-blocking
                keypad(my_menu_win, TRUE);

                /* Set main window and sub window */
                set_menu_win(my_menu, my_menu_win);
                set_menu_sub(my_menu, derwin(my_menu_win, 8, 12, 1, 1));
                set_menu_format(my_menu, 7, 1);

                /* Set menu mark to the string "" */
                set_menu_mark(my_menu, "");

                /* Print a border around the menu */
                box(my_menu_win, 0, 0);

                /* Set menu colors */
                if (e->beast) {
                    set_menu_fore(my_menu, COLOR_PAIR(S4C_MAGENTA));
                } else {
                    set_menu_fore(my_menu, COLOR_PAIR(S4C_RED));
                }
                set_menu_back(my_menu, COLOR_PAIR(S4C_WHITE));

                //attron(COLOR_PAIR(2));
                //mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scoll down or up a page of items");
                //mvprintw(LINES - 1, 0, "Arrow Keys to navigate (F1 to Exit)");
                //attroff(COLOR_PAIR(2));
                //refresh();

                log_tag("debug_log.txt", "[DEBUG]", "Doing init_wins().");
                init_wins(my_wins, e, player);
                log_tag("debug_log.txt", "[DEBUG]", "Done init_wins().");

                log_tag("debug_log.txt", "[DEBUG]",
                        "Doing printBattleStats().");
                //Print first battle stats for this enemy
                printBattleStats(my_wins, player, e);
                log_tag("debug_log.txt", "[DEBUG]", "Done printBattleStats().");
                refresh();

                /* Post the menu */
                post_menu(my_menu);
                wrefresh(my_menu_win);
                refresh();

                // Prepare enemy animation window
                enemy_animation_win = newwin(19, 19, 3, 2);

                // Prepare fighter animation window
                fighter_animation_win = newwin(19, 19, 3, 58);

                // Prepare notifications window
                notifications_win = newwin(2, 70, 24, 4);
                // Set notifications window to be scrolling
                scrollok(notifications_win, TRUE);
                //Update turnOP_args->notify_win pointer
                args->notify_win = notifications_win;
                log_tag("debug_log.txt", "[TURNOP]",
                        "Assigned notifications_wins to turnOP_args: args->notify_win");

                wrefresh(notifications_win);
                refresh();
            } else {		//End if choice is not FIGHT
                // Set notifications window to be scrolling
                log_tag("debug_log.txt", "[DEBUG]", "Doing init_wins().");
                init_wins(my_wins, e, player);
                log_tag("debug_log.txt", "[DEBUG]", "Done init_wins().");
                log_tag("debug_log.txt", "[DEBUG]",
                        "Doing printBattleStats().");
                //Print first battle stats for this enemy
                printBattleStats(my_wins, player, e);
                log_tag("debug_log.txt", "[DEBUG]", "Done printBattleStats().");
                refresh();
                /* Create turn menu items */
                n_choices = ARRAY_SIZE(choices);
                if (G_DEBUG_ON) {	//We have to include debug menu options
                    debug_n_choices = ARRAY_SIZE(debug_choices);
                }
                my_items =
                    (ITEM **) calloc(n_choices + debug_n_choices,
                                     sizeof(ITEM *));

                // Prepare menu items
                for (int k = 0; k < n_choices; k++) {
                    if (G_DEBUG_ON && k == n_choices - 1) {
                        //We skip adding the null string item
                    } else {
                        my_items[k] = new_item(choices[k], choices[k]);
                    }
                    /* Set the user pointer */
                    //set_item_userptr(my_items[i]);
                }
                // Prepare debug items
                if (G_DEBUG_ON) {
                    for (int k = 0; k < debug_n_choices; k++) {
                        my_items[k + n_choices - 1] =
                            new_item(debug_choices[k], debug_choices[k]);
                    }
                    /* Set the user pointer */
                    //set_item_userptr(my_items[i]);
                }

                /* Create menu */
                my_menu = new_menu((ITEM **) my_items);

                /* Set description off */
                menu_opts_off(my_menu, O_SHOWDESC);

                /* Create the window to be associated with the menu */
                my_menu_win = newwin(9, 13, 14, 35);
                nodelay(my_menu_win, TRUE);	//We make getch non-blocking
                keypad(my_menu_win, TRUE);

                /* Set main window and sub window */
                set_menu_win(my_menu, my_menu_win);
                set_menu_sub(my_menu, derwin(my_menu_win, 8, 12, 1, 1));
                set_menu_format(my_menu, 7, 1);

                /* Set menu mark to the string "" */
                set_menu_mark(my_menu, "");

                /* Print a border around the menu */
                box(my_menu_win, 0, 0);

                /* Set menu colors */
                if (e->beast) {
                    set_menu_fore(my_menu, COLOR_PAIR(S4C_MAGENTA));
                } else {
                    set_menu_fore(my_menu, COLOR_PAIR(S4C_RED));
                }
                set_menu_back(my_menu, COLOR_PAIR(S4C_WHITE));

                /* Post the menu */
                post_menu(my_menu);
                wrefresh(my_menu_win);

                //box(notifications_win, 0, 0);
                wrefresh(notifications_win);
                refresh();
            }

            if (e->hp <= 0) {	//Check if enemy hp got below zero during last loop
                log_tag("debug_log.txt", "[DEBUG-ROOM]",
                        "Enemy %s hp is 0 outside of main turn loop...",
                        stringFromEClass(e->class));
                fightStatus = OP_RES_KILL_DONE;	//Set the variable that will makes us skip the loop when we continue
                delwin(enemy_animation_win);
                delwin(fighter_animation_win);
                // Unpost menu and free all the memory taken up
                unpost_menu(my_menu);
                free_menu(my_menu);
                for (int k = 0; k < (n_choices + debug_n_choices); k++) {
                    free_item(my_items[k]);
                }
                free(my_items);

                /*
                   if (e->beast) {
                   purple();
                   } else {
                   green();
                   }
                   printf("\n\n\t%s fainted.\n",stringFromEClass(e->class));
                   white();
                 */

                //Update stats
                player->stats->enemieskilled++;

                //Consumable drop, guaranteed on killing a beast
                if (e->beast || ((rand() % 9) - (player->luck / 10) <= 0)) {
                    int consDrop = dropConsumable(player);
                    log_tag("debug_log.txt", "[DEBUG]", "consDrop was (%i)",
                            consDrop);

                }
                //Artifact drop (if we don't have all of them), guaranteed on killing a beast
                if ((player->stats->artifactsfound != ARTIFACTSMAX + 1)
                    && (e->beast
                        || ((rand() % 1001) - (player->luck / 10) <= 0))) {
                    int artifactDrop = dropArtifact(player);
                    log_tag("debug_log.txt", "[DEBUG]",
                            "artifactDrop was (%i) == [%s]", artifactDrop,
                            stringFromArtifacts(artifactDrop));
                }
                //Equip drop, guaranteed on killing a beast
                if (e->beast || ((rand() % 15) - (player->luck / 10) <= 0)) {
                    dropEquip(player, e->beast, notifications_win, kls);
                }
                //Get xp and free memory from enemy

                //Account for harvester perk
                int harvester_perks = player->perks[HARVESTER]->innerValue;
                if (harvester_perks > 0) {
                    float xpboost = harvester_perks * 0.1;
                    e->xp *= (1 + xpboost);
                }

                //Give coins

                player->balance += e->prize;
                player->stats->coinsfound += e->prize;
                //lightYellow();
                //printf("\n\n\t+%i coins.",e->prize);
                //white();
                //screenTime(1);

                giveXp(player, e);
                endwin();

                e_death(e);

                room->foes->alive_enemies[i] = 0;
                room->foes->current_index++;
                room->foes->tot_alive--;
                if (room->foes->tot_alive > 0) {
                    clear();
                    refresh();
                    //Display current party info
                    display_printFoeParty(room->foes);
                    refresh();
                }
                //Go to next battle
                i++;

                //Reset frame counter
                frame_counter = 0;

                if (enemies - i != 1) {
                    //TODO
                    //Notification of enemies left in room, if needed?
                    //lightYellow();
                    log_tag("debug_log.txt", "[ROOM]",
                            "%i enemies left in room %i.", enemies - i - 1,
                            index);
                    //white();
                }
                if (enemies - i == 0) {
                    log_tag("debug_log.txt", "\033[1;34m[DEBUG]",
                            "\'$i\' got to \'%d\'. \'$enemies\' was: %d\n", i,
                            enemies);
                }

                int res = system("clear");
                log_tag("debug_log.txt", "[DEBUG]",
                        "handleRoom_Enemies() system(\"clear\") res was (%i)",
                        res);
                continue;	//Check while condition again...
            }			//End check for deaths

            int picked = 0;
            clock_t menu_start_time = clock(), menu_diff_time;	//, menu_loop_diff_time;
            int frame_tot = 60;

            while (!picked && (c = wgetch(my_menu_win)) != KEY_F(1)) {
                //clock_t menu_loop_time = clock();

                switch (c) {
                case KEY_DOWN:
                    menu_driver(my_menu, REQ_DOWN_ITEM);
                    break;
                case KEY_UP:
                    menu_driver(my_menu, REQ_UP_ITEM);
                    break;
                case KEY_LEFT: { /*Left option pick */
                    ITEM *cur;
                    cur = current_item(my_menu);
                    choice = getTurnChoice((char *)item_name(cur));
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Left on choice: [ %s ] value (%i)",
                            item_name(cur), choice);
                    if (choice == EQUIPS) {
                        log_tag("debug_log.txt", "[DEBUG]",
                                "Should do something");
                    }
                }
                break;
                case KEY_RIGHT: {
                    /*Right option pick */
                    ITEM *cur;
                    cur = current_item(my_menu);
                    choice = getTurnChoice((char *)item_name(cur));
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Right on choice: [ %s ] value (%i)",
                            item_name(cur), choice);
                    if (choice == EQUIPS) {
                        log_tag("debug_log.txt", "[DEBUG]",
                                "Should do something");
                    }
                }
                break;
                case KEY_NPAGE:
                    menu_driver(my_menu, REQ_SCR_DPAGE);
                    break;
                case KEY_PPAGE:
                    menu_driver(my_menu, REQ_SCR_UPAGE);
                    break;
                case 10: {	/* Enter */
                    picked = 1;
                    ITEM *cur;

                    //move(18,47);
                    //clrtoeol();
                    cur = current_item(my_menu);
                    //mvprintw(18, 47, "Item selected is : %s", item_name(cur));
                    choice = getTurnChoice((char *)item_name(cur));
                    pos_menu_cursor(my_menu);
                    //refresh();
                };
                break;
                case 'q': {
                    if (G_FASTQUIT_ON == 1) {
                        log_tag("debug_log.txt", "[DEBUG]",
                                "Player used q to quit from enemy room menu.");
                        picked = 1;
                        choice = getTurnChoice("Quit");
                        pos_menu_cursor(my_menu);
                        refresh();
                    } else {
                        log_tag("debug_log.txt", "[DEBUG]",
                                "Player used q in enemy room menu, but G_FASTQUIT_ON was not 1.");
                    }
                }
                break;
                default: {
                    break;
                }
                }
                wrefresh(my_menu_win);
                menu_diff_time = clock() - menu_start_time;
                //menu_loop_diff_time = clock() - menu_loop_time;
                //sprintf(msg,"menu_loop_diff_time was (%li) [CPS %li]", menu_loop_diff_time, (long) CLOCKS_PER_SEC);
                //log_tag("debug_log.txt","[DEBUG]",msg);

                int menu_time_spent = menu_diff_time * 1000 / CLOCKS_PER_SEC;
                //sprintf(time_msg,"[DEBUG-TIME]    Time: %d s %d ms.", menu_time_spent/1000, menu_time_spent%1000);
                //debug_log("debug_log.txt", time_msg);
                if (menu_time_spent % 1000 > 37) {
                    s4c_animate_rangeof_sprites_at_coords(enemy_sprites
                                                          [e->class],
                                                          enemy_animation_win,
                                                          frame_counter,
                                                          frame_counter, 1, 1,
                                                          60, 19, 19, 0, 0);
                    s4c_animate_rangeof_sprites_at_coords(fighter_sprites
                                                          [player->class],
                                                          fighter_animation_win,
                                                          frame_counter,
                                                          frame_counter, 1, 1,
                                                          60, 19, 19, 0, 0);
                    frame_counter++;

                    if (G_EXPERIMENTAL_ON == 1) {
                        run_time = clock() - gamestate->start_time;
                        time_spent = run_time / CLOCKS_PER_SEC;
                        //log_tag("debug_log.txt", "[DEBUG]", "Current run time: %d s, %d ms.",
                        //    time_spent / 1000, time_spent % 1000);
                        struct tm* time_tm = localtime(&time_spent);

                        if (time_tm == NULL) {
                            log_tag("debug_log.txt", "[ERROR]", "%s():    time_tm was NULL.\n", __func__);
                        } else {
                            strftime(time_str, 20, "Time: %M:%S", time_tm);
                            mvwprintw(notifications_win, 0, 0, "%s", time_str);
                            wrefresh(notifications_win);
                        }
                    }

                    if (frame_counter > frame_tot) {
                        animation_loops_done++;
                        if (animation_loops_done == 1) {
                            log_tag("debug_log.txt", "[ANIMATE]",
                                    "Got to frame %i, restarting animation.",
                                    frame_counter);
                        }
                        frame_counter = 0;	//Reset animation loop on last frame
                    }
                    menu_start_time = clock();	//We reset the timer
                    /*
                       sprintf(time_msg,"[DEBUG-TIME]    Time: %d s. %d ms from last msg.",menu_time_spent/1000,menu_time_spent%1000);
                       debug_log("debug_log.txt", time_msg);
                     */
                }

            }

            //We don't endwin immediately if we pick fight

            if (choice == FIGHT) {
                //TODO:
                //Foe picks its turn
                //
                args->foe_op =
                    foeTurnOP_from_foeTurnOption(enemyTurnPick(e, player));
                log_tag("debug_log.txt", "[FOETURN]", "foePick was: [ %s ]",
                        stringFromFoeTurnOP(args->foe_op));

                //TODO:
                //Handle FIGHT as turnOP(OP_FIGHT)
                fightStatus = turnOP(OP_FIGHT, args, kls, t_kls);
                refresh();
                //Lost battle
                if (fightStatus == OP_RES_DEATH) {
                    // Unpost menu and free all the memory taken up
                    unpost_menu(my_menu);
                    free_menu(my_menu);
                    int totalChoices = n_choices + debug_n_choices;
                    if (G_DEBUG_ON) {
                        totalChoices -= 1;	//We subtract 1 to account for the discarded null choice in the base option set.
                    }
                    for (int k = 0; k < totalChoices; k++) {
                        free_item(my_items[k]);
                    }
                    free(my_items);

                    log_tag("debug_log.txt", "[ANIMATE]",
                            "Ending enemy animation.");
                    log_tag("debug_log.txt", "[ANIMATE]",
                            "Ending fighter animation.");
                    endwin();

                    //Account for oracle gift perk
                    int oracle_perks = player->perks[ORACLE_GIFT]->innerValue;
                    if (oracle_perks > 0) {
                        int res = system("clear");
                        log_tag("debug_log.txt", "[DEBUG]",
                                "handleRoom_Enemies() 2 system(\"clear\") res was (%i)",
                                res);
                        //blue();
                        //printf("\n\n\t\tYou get a second chance, as the prophecy said.\n");
                        //white();
                        player->hp = round(player->totalhp / 3);

                        player->perks[ORACLE_GIFT]->innerValue -= 1;

                        e_death(e);
                        log_tag("debug_log.txt", "[DEBUG-ROOM-PERKS]",
                                "Oraclegift proc.");

                        break;	//We go to next enemy
                    }

                    e_death(e);
                    int player_luck = player->luck;
                    log_tag("debug_log.txt", "[DEBUG]", "Player luck was [%i]",
                            player_luck);
                    death(player, load_info);
                    //free(room->foes);
                    endwin();

                    //printf("\t\tLuck:  %i Path luck:  %i\n",player_luck,p->luck);
                    //red();
                    //printf("\n\n\tYOU DIED.\n");
                    //white();
                    //free(args);
                    log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");

                    return OP_RES_DEATH;
                } else if (fightStatus == OP_RES_KILL_DONE) {
                    int dropch;
                    int dropped_chars = 0;
                    while ((dropch = wgetch(my_menu_win)) != ERR) {
                        // Discard any input characters
                        dropped_chars++;
                    }
                    log_tag("debug_log.txt", "[DEBUG]", "Dropped ( %i ) inputs",
                            dropped_chars);

                    // Unpost menu and free all the memory taken up
                    unpost_menu(my_menu);
                    free_menu(my_menu);
                    int totalChoices = n_choices + debug_n_choices;
                    if (G_DEBUG_ON) {
                        totalChoices -= 1;	//We subtract 1 to account for the discarded null choice in the base option set.
                    }
                    for (int k = 0; k < totalChoices; k++) {
                        free_item(my_items[k]);
                    }
                    free(my_items);

                    log_tag("debug_log.txt", "[ANIMATE]",
                            "Ending enemy animation.");
                    log_tag("debug_log.txt", "[ANIMATE]",
                            "Ending fighter animation.");

                    //Give coins

                    player->balance += e->prize;
                    player->stats->coinsfound += e->prize;

                    char msg[50];
                    sprintf(msg, "You found +%i coins.", e->prize);
                    wattron(notifications_win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
                    display_notification(notifications_win, msg, 500);
                    wattroff(notifications_win, COLOR_PAIR(S4C_BRIGHT_YELLOW));

                    //Win, get xp and free memory from enemy
                    giveXp(player, e);

                    endwin();
                    e_death(e);

                    room->foes->alive_enemies[i] = 0;
                    room->foes->current_index++;
                    room->foes->tot_alive--;
                    if (room->foes->tot_alive > 0) {
                        clear();
                        refresh();
                        //Display current party info
                        display_printFoeParty(room->foes);
                        refresh();
                    }

                    //Go to next battle
                    i++;

                    frame_counter = 0;	//Reset animation loop on last frame

                    if (enemies - i > 1) {
                        //TODO
                        //Notification of enemies left in room, if needed?
                        //lightYellow();
                        log_tag("debug_log.txt", "[ROOM]",
                                "%i enemies left in room %i.", enemies - i - 1,
                                index);
                        //white();
                        int res = system("clear");
                        log_tag("debug_log.txt", "[DEBUG]",
                                "handleRoom_Enemies() 3 system(\"clear\") res was (%i)",
                                res);
                        fightStatus = OP_RES_NO_DMG;
                        log_tag("debug_log.txt", "[ROOM]",
                                "Onto next enemy, %i left.", enemies - i);
                    }
                    if (enemies - i == 0) {
                        log_tag("debug_log.txt", "[DEBUG]",
                                "\'$i\' got to \'%d\'. \'$enemies\' was: %d\n",
                                i, enemies);
                    }

                    break;
                }
            } else if (choice == SPECIAL) {
                //TODO:
                //Foe picks its turn
                // Unpost menu and free all the memory taken up
                unpost_menu(my_menu);
                free_menu(my_menu);
                int totalChoices = n_choices + debug_n_choices;
                if (G_DEBUG_ON) {
                    totalChoices -= 1;	//We subtract 1 to account for the discarded null choice in the base option set.
                }
                for (int k = 0; k < totalChoices; k++) {
                    free_item(my_items[k]);
                }
                free(my_items);

                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending enemy animation.");
                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending fighter animation.");

                endwin();
                //FIXME
                //Should we ensure death of foe is correctly handled before OP ends?
                turnOP(OP_SPECIAL, args, kls, t_kls);
            } else if (choice == CONSUMABLE) {
                // Unpost menu and free all the memory taken up
                unpost_menu(my_menu);
                free_menu(my_menu);
                int totalChoices = n_choices + debug_n_choices;
                if (G_DEBUG_ON) {
                    totalChoices -= 1;	//We subtract 1 to account for the discarded null choice in the base option set.
                }
                for (int k = 0; k < totalChoices; k++) {
                    free_item(my_items[k]);
                }
                free(my_items);

                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending enemy animation.");
                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending fighter animation.");

                endwin();
                turnOP(OP_CONSUMABLE, args, kls, t_kls);
            } else if (choice == EQUIPS) {
                // Unpost menu and free all the memory taken up
                unpost_menu(my_menu);
                free_menu(my_menu);
                int totalChoices = n_choices + debug_n_choices;
                if (G_DEBUG_ON) {
                    totalChoices -= 1;	//We subtract 1 to account for the discarded null choice in the base option set.
                }
                for (int k = 0; k < totalChoices; k++) {
                    free_item(my_items[k]);
                }
                free(my_items);

                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending enemy animation.");
                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending fighter animation.");

                endwin();
                turnOP(OP_EQUIPS, args, kls, t_kls);
            } else if (choice == PERKS) {
                // Unpost menu and free all the memory taken up
                unpost_menu(my_menu);
                free_menu(my_menu);
                int totalChoices = n_choices + debug_n_choices;
                if (G_DEBUG_ON) {
                    totalChoices -= 1;	//We subtract 1 to account for the discarded null choice in the base option set.
                }
                for (int k = 0; k < totalChoices; k++) {
                    free_item(my_items[k]);
                }
                free(my_items);
                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending enemy animation.");
                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending fighter animation.");

                endwin();

                turnOP(OP_PERKS, args, kls, t_kls);
            } else if (choice == STATS) {
                // Unpost menu and free all the memory taken up
                unpost_menu(my_menu);
                free_menu(my_menu);
                int totalChoices = n_choices + debug_n_choices;
                if (G_DEBUG_ON) {
                    totalChoices -= 1;	//We subtract 1 to account for the discarded null choice in the base option set.
                }
                for (int k = 0; k < totalChoices; k++) {
                    free_item(my_items[k]);
                }
                free(my_items);
                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending enemy animation.");
                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending fighter animation.");

                endwin();
                turnOP(OP_STATS, args, kls, t_kls);
            } else if (choice == SAVE) {
                FILE *save_file;
                char path_to_savefile[800];
                char static_path[500];
                char savefile_name[300];
                sprintf(savefile_name, "%s", p->current_saveslot->save_path);
                log_tag("debug_log.txt", "[DEBUG]",
                        "handleRoom_Enemies:  savefile_name is [%s].",
                        savefile_name);

                // Set static_path value to the correct static dir path
                resolve_staticPath(static_path);

                sprintf(path_to_savefile, "%s/%s", static_path, savefile_name);

                save_file = fopen(path_to_savefile, "w");
                if (save_file == NULL) {
                    fprintf(stderr, "[ERROR]    Can't open save file %s!\n",
                            path_to_savefile);
                    exit(EXIT_FAILURE);
                } else {
                    log_tag("debug_log.txt", "[TURNOP]",
                            "Assigning save_file pointer to args->save_file. Path: [%s]",
                            path_to_savefile);
                    args->save_file = save_file;
                }
                turnOP(OP_SAVE, args, kls, t_kls);
                fclose(save_file);
            } else if (choice == DEBUG) {
                // Unpost menu and free all the memory taken up
                unpost_menu(my_menu);
                free_menu(my_menu);
                int totalChoices = n_choices + debug_n_choices;
                if (G_DEBUG_ON) {
                    totalChoices -= 1;	//We subtract 1 to account for the discarded null choice in the base option set.
                }
                for (int k = 0; k < totalChoices; k++) {
                    free_item(my_items[k]);
                }
                free(my_items);

                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending enemy animation.");
                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending fighter animation.");

                endwin();
                turnOP(OP_DEBUG, args, kls, t_kls);
            } else if (choice == ARTIFACTS) {
                // Unpost menu and free all the memory taken up
                unpost_menu(my_menu);
                free_menu(my_menu);
                int totalChoices = n_choices + debug_n_choices;
                if (G_DEBUG_ON) {
                    totalChoices -= 1;	//We subtract 1 to account for the discarded null choice in the base option set.
                }
                for (int k = 0; k < totalChoices; k++) {
                    free_item(my_items[k]);
                }
                free(my_items);

                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending enemy animation.");
                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending fighter animation.");

                endwin();
                turnOP(OP_ARTIFACTS, args, kls, t_kls);
            } else if (choice == QUIT) {
                // Unpost menu and free all the memory taken up
                unpost_menu(my_menu);
                free_menu(my_menu);
                int totalChoices = n_choices + debug_n_choices;
                if (G_DEBUG_ON) {
                    totalChoices -= 1;	//We subtract 1 to account for the discarded null choice in the base option set.
                }
                for (int k = 0; k < totalChoices; k++) {
                    free_item(my_items[k]);
                }
                free(my_items);

                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending enemy animation.");
                log_tag("debug_log.txt", "[ANIMATE]",
                        "Ending fighter animation.");

                e_death(e);
                delwin(my_wins[0]);
                delwin(my_wins[1]);
                delwin(my_wins[2]);
                //free(room->foes);
                endwin();
                turnOP(OP_QUIT, args, kls, t_kls);
            }
            //Update all the player counters
            updateCounters((Turncounter **) player->counters, 0, player, e);
            //Update all the enemy counters
            updateCounters((Turncounter **) e->counters, 1, player, e);

            //printBattleStats(my_wins,player,e);

            //if (e->beast) {
            //white();
            //}
            //update_panels();
            refresh();
        }			//End while current enemy
    }				//End for all enemies
    update_Gamestate(gamestate, 1, -1, room->index, -1,
                     gamestate->current_floor);
    log_tag("debug_log.txt", "[ROOM]", "End of room %i", room->index);
    //free(args);
    //log_tag("debug_log.txt","[FREE]","Freed turnOP_args");
    return fightStatus;
}

/**
 * Takes an integer for room index and a Fighter and a Path pointers and handles the progression of the room.
 * For each enemy, their stats are prepared before resetting permboosts and artifacts and applying them again.
 * User is asked for turn choice and according to the input, the corresponding action will be called.
 * Then, status is applied and counters are updated.
 * On enemy death, there's a chance to call dropConsumable, dropEquip or dropArtifact (guaranteed for beast enemies).
 * When the last enemy is dead, the function returns KILL_DONE.
 * @see Fighter
 * @see Path
 * @see BEASTROOM
 * @see resetPermBoosts()
 * @see resetArtifacts()
 * @see applyArtifacts()
 * @see applyPermBoosts()
 * @see printBossBattleStats()
 * @see getTurnChoice()
 * @see dropConsumable()
 * @see dropEquip()
 * @see dropArtifact()
 * @see giveXp_Boss()
 * @see boss_fight()
 * @see b_death()
 * @see death()
 * @see fight_Boss()
 * @see handleSpecials()
 * @see handleConsumables()
 * @see handleEquips()
 * @see handleStats()
 * @see debug_boss_room()
 * @see handleArtifacts()
 * @see applyStatus()
 * @see applyBStatus()
 * @param gamestate The pointer to Gamestate.
 * @param room The pointer to current room.
 * @param index The index of current room.
 * @param p The Path pointer.
 * @param player The Fighter pointer at hand.
 * @param kls The Koliseo used for allocations.
 * @param t_kls The Koliseo_Temp used for temp allocations.
 * @see turnOP()
 * @return When room is cleared, should return KILL_DONE.
 */
int handleRoom_Boss(Gamestate *gamestate, Room *room, int index, Path *p,
                    Fighter *player, loadInfo *load_info,
                    char boss_sprites[BOSSCLASSESMAX +
                                      1][MAXFRAMES][MAXROWS][MAXCOLS],
                    char fighter_sprites[CLASSESMAX +
                            1][MAXFRAMES][MAXROWS][MAXCOLS],
                    Koliseo *kls, Koliseo_Temp *t_kls)
{

    Boss *args_boss = NULL;
    Enemy *dummy_enemy = NULL;
    FILE *args_save_file = NULL;
    WINDOW *args_notify_win = NULL;
    foeTurnOption_OP dummy_foe_op = FOE_OP_INVALID;
    skillType dummy_skill_pick = -1;
    int isBoss = 1;
    //Declare turnOP_args
    turnOP_args *args =
        init_turnOP_args(gamestate, player, p, room, load_info, dummy_enemy,
                         args_boss, args_save_file, args_notify_win, t_kls,
                         dummy_foe_op, dummy_skill_pick);

    //Strings for turn menu choices
    char *choices[] = {
        "Fight",
        "Special",
        "Consumables",
        "Artifacts",
        "Equips",
        "Perks",
        "Stats",
        "Quit",
        (char *)NULL,
    };

    char *debug_choices[] = {
        "Debug",
        (char *)NULL,
    };
    //system("clear");

    //printf("\n\tYou are now in room %i.\n",index);

    OP_res fightStatus = OP_RES_NO_DMG;
    Boss *b = room->boss;
    //Update turnOP_args->boss pointer
    args->boss = b;
    log_tag("debug_log.txt", "[TURNOP]",
            "Assigned boss %s to turnOP_args: args->boss == [%s]",
            stringFromBossClass(b->class),
            stringFromBossClass(args->boss->class));
    log_tag("debug_log.txt", "[ROOM]", "New room, index %i, boss: %s",
            room->index, stringFromBossClass(b->class));

    //Reset player permboosts... and calc them again...
    resetPermboosts(player);

    //Reset artifacts state, for the ones we have unlocked
    resetArtifactsState(player);
    //Apply artifacts
    Enemy *dummy = NULL;
    //TODO: all functions displaying behaviour depending on a boolean parameter should be split into different functions, and treat arguments accordingly.
    applyArtifacts(player, dummy, b, isBoss);

    //Calc permboosts and turn boosts
    //Turn boosts

    //Perm boosts
    applyPermboosts(player);

    //Fight preparation is done, on to turn choice
    turnOption choice = 999;
    WINDOW *my_wins[3];
    ITEM **my_items;
    MENU *my_menu;
    WINDOW *my_menu_win = NULL;
    WINDOW *fighter_animation_win = NULL;
    WINDOW *boss_animation_win = NULL;
    WINDOW *notifications_win = NULL;
    for (int winindex = 0; winindex < 3; winindex++) {
        my_wins[winindex] = NULL;
    }
    int n_choices, c;

    int debug_n_choices = 0;

    update_Gamestate(gamestate, 1, room->class, room->index - 1, 1,
                     gamestate->current_floor);

    int frame_counter = 0;
    int animation_loops_done = 0;
    char time_str[20] = {0};
    clock_t run_time = {0};
#ifndef _WIN32
    long time_spent;
#else
    long long time_spent;
#endif

    while (!
           (fightStatus == OP_RES_DEATH || fightStatus == OP_RES_KILL_DONE
            || choice == QUIT)) {

        if (choice != FIGHT) {
            /* Initialize curses */
            //initscr();
            start_color();
            clear();
            refresh();
            cbreak();
            noecho();
            keypad(stdscr, TRUE);
            log_tag("debug_log.txt", "[ANIMATE]",
                    "Starting new fighter animation.");
            log_tag("debug_log.txt", "[ANIMATE]",
                    "Starting new boss animation.");

            int cursorCheck = curs_set(0);	// We make the cursor invisible or return early with the error

            if (cursorCheck == ERR) {
                return S4C_ERR_CURSOR;	//fprintf(stderr,"animate => Terminal does not support cursor visibility state.\n");
            }

            /* Create turn menu items */
            n_choices = ARRAY_SIZE(choices);
            if (G_DEBUG_ON) {	//We have to include debug menu options
                debug_n_choices = ARRAY_SIZE(debug_choices);
            }
            my_items =
                (ITEM **) calloc(n_choices + debug_n_choices, sizeof(ITEM *));

            // Prepare menu items
            for (int k = 0; k < n_choices; k++) {
                if (G_DEBUG_ON && k == n_choices - 1) {
                    //We skip adding the null string item
                } else {
                    my_items[k] = new_item(choices[k], choices[k]);
                }
                /* Set the user pointer */
                //set_item_userptr(my_items[i]);
            }
            // Prepare debug items
            if (G_DEBUG_ON) {
                for (int k = 0; k < debug_n_choices; k++) {
                    my_items[k + n_choices - 1] =
                        new_item(debug_choices[k], debug_choices[k]);
                }
                /* Set the user pointer */
                //set_item_userptr(my_items[i]);
            }

            /* Create menu */
            my_menu = new_menu((ITEM **) my_items);
            /* Set description off */
            menu_opts_off(my_menu, O_SHOWDESC);
            /* Create the window to be associated with the menu */
            my_menu_win = newwin(9, 13, 14, 35);
            nodelay(my_menu_win, TRUE);	//We make getch non-blocking
            keypad(my_menu_win, TRUE);

            /* Set main window and sub window */
            set_menu_win(my_menu, my_menu_win);
            set_menu_sub(my_menu, derwin(my_menu_win, 8, 12, 1, 1));
            set_menu_format(my_menu, 7, 1);

            /* Set menu mark to the string "" */
            set_menu_mark(my_menu, "");

            /* Print a border around the menu */
            box(my_menu_win, 0, 0);

            /* Set menu colors */
            set_menu_fore(my_menu, COLOR_PAIR(S4C_MAGENTA));
            set_menu_back(my_menu, COLOR_PAIR(S4C_WHITE));

            log_tag("debug_log.txt", "[ROOM]", "Doing init_boss_wins()");
            init_boss_wins(my_wins, b, player);
            log_tag("debug_log.txt", "[ROOM]", "Done init_boss_wins()");
            log_tag("debug_log.txt", "[ROOM]", "Doing printBossBattleStats()");
            //Print first battle stats for this enemy
            printBossBattleStats(my_wins, player, b);
            log_tag("debug_log.txt", "[ROOM]", "Done printBossBattleStats()");
            refresh();
            /* Post the menu */
            post_menu(my_menu);
            wrefresh(my_menu_win);
            refresh();
            //sleep(2);
            //endwin();
            // Prepare boss animation window
            boss_animation_win = newwin(19, 19, 3, 2);
            // Prepare fighter animation window
            fighter_animation_win = newwin(19, 19, 3, 58);
            // Prepare notifications window
            notifications_win = newwin(2, 70, 24, 4);
            // Set notifications window to be scrolling
            scrollok(notifications_win, TRUE);

            //Update turnOP_args->notify_win pointer
            args->notify_win = notifications_win;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Assigned notifications_wins to turnOP_args: args->notify_win");

            wrefresh(notifications_win);
            refresh();
        } else {		//We chose FIGHT so we handle that
            /* Create items */
            n_choices = ARRAY_SIZE(choices);
            my_items = (ITEM **) calloc(n_choices, sizeof(ITEM *));
            for (int k = 0; k < n_choices; k++) {
                my_items[k] = new_item(choices[k], choices[k]);
                /* Set the user pointer */
                //set_item_userptr(my_items[i]);
            }

            /* Create menu */
            my_menu = new_menu((ITEM **) my_items);
            /* Set description off */
            menu_opts_off(my_menu, O_SHOWDESC);
            /* Create the window to be associated with the menu */
            my_menu_win = newwin(9, 13, 14, 35);
            nodelay(my_menu_win, TRUE);	//We make getch non-blocking
            keypad(my_menu_win, TRUE);

            /* Set main window and sub window */
            set_menu_win(my_menu, my_menu_win);
            set_menu_sub(my_menu, derwin(my_menu_win, 8, 12, 1, 1));
            set_menu_format(my_menu, 7, 1);

            /* Set menu mark to the string "" */
            set_menu_mark(my_menu, "");

            /* Print a border around the menu */
            box(my_menu_win, 0, 0);

            /* Set menu colors */
            set_menu_fore(my_menu, COLOR_PAIR(S4C_MAGENTA));
            set_menu_back(my_menu, COLOR_PAIR(S4C_WHITE));

            log_tag("debug_log.txt", "[ROOM]", "Doing init_boss_wins()");
            init_boss_wins(my_wins, b, player);
            log_tag("debug_log.txt", "[ROOM]", "Done init_boss_wins()");
            log_tag("debug_log.txt", "[ROOM]", "Doing printBossBattleStats()");
            //Print first battle stats for this enemy
            printBossBattleStats(my_wins, player, b);
            log_tag("debug_log.txt", "[ROOM]", "Done printBossBattleStats()");
            /* Post the menu */
            post_menu(my_menu);
            wrefresh(my_menu_win);
            wrefresh(notifications_win);
            refresh();
        }

        if (b->hp <= 0) {	//Check if boss hp got below zero during last loop

            fightStatus = OP_RES_KILL_DONE;	//Set the variable that will makes us skip the loop when we continue

            log_tag("debug_log.txt", "[DEBUG-ROOM]",
                    "Boss %s hp is 0 outside of main turn loop...",
                    stringFromBossClass(b->class));

            delwin(boss_animation_win);
            delwin(fighter_animation_win);

            // Unpost menu and free all the memory taken up
            unpost_menu(my_menu);
            free_menu(my_menu);
            int totalChoices = n_choices + debug_n_choices;
            if (G_DEBUG_ON) {
                totalChoices -= 1;	//We subtract 1 to account for the discarded null choice in the base option set.
            }
            for (int k = 0; k < totalChoices; k++) {
                free_item(my_items[k]);
            }
            free(my_items);

            //TODO:notifications for BOSS room
            //purple();
            //printf("\n\n\t%s fainted.\n",stringFromBossClass(b->class));
            //white();
            //screenTime(1.5);

            //Update stats
            player->stats->bosseskilled++;
            //Check if we killed this boss before
            if (player->stats->killed_bosses[b->class] == 0) {
                //Update stats
                player->stats->unique_bosseskilled++;
                player->stats->killed_bosses[b->class] += 1;
                //Check if we have to update the wincon value
                if (p->win_condition->class == ALL_BOSSES) {
                    p->win_condition->current_val++;
                }
            }
            //Consumable drop
            int consDrop = dropConsumable(player);
            log_tag("debug_log.txt", "[DEBUG]", "consDrop was (%i)", consDrop);

            //Artifact drop (if we don't have all of them)
            if ((player->stats->artifactsfound != ARTIFACTSMAX + 1)) {
                int artifactDrop = dropArtifact(player);
                log_tag("debug_log.txt", "[DEBUG]", "artifactDrop was (%i)",
                        artifactDrop);
            }
            //Equip drop
            dropEquip(player, b->beast, notifications_win, kls);

            //Account for harvester perk
            int harvester_perks = player->perks[HARVESTER]->innerValue;
            if (harvester_perks > 0) {
                float xpboost = harvester_perks * 0.1;
                b->xp *= (1 + xpboost);
            }
            //Give coins

            player->balance += b->prize;
            player->stats->coinsfound += b->prize;

            //TODO: notifications for boss window.
            //lightYellow();
            //printf("\n\n\t+%i coins.",b->prize);
            //white();
            //screenTime(1);

            //Give key
            player->keys_balance += 1;
            player->stats->keysfound += 1;

            //TODO: notifications for boss window.
            //lightYellow();
            //printf("\n\t+1 key.\n");
            //white();
            //screenTime(1);

            //Get xp and free memory from boss
            giveXp_Boss(player, b);

            b_death(b);

            delwin(fighter_animation_win);
            delwin(boss_animation_win);

            //FIXME: is this causing a crash?
            delwin(my_wins[0]);
            delwin(my_wins[1]);
            delwin(my_wins[2]);
            clear();
            refresh();
            endwin();
            continue;		//Check while condition again...
            //
        }			//End check for deaths

        int picked = 0;
        clock_t menu_start_time = clock(), menu_diff_time;	//, menu_loop_diff_time;
        int frame_tot = 60;

        while (!picked && (c = wgetch(my_menu_win)) != KEY_F(1)) {
            //clock_t menu_loop_time = clock();
            switch (c) {
            case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                break;
            case KEY_NPAGE:
                menu_driver(my_menu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:
                menu_driver(my_menu, REQ_SCR_UPAGE);
                break;
            case 10: {		/* Enter */
                picked = 1;
                ITEM *cur;

                cur = current_item(my_menu);
                choice = getTurnChoice((char *)item_name(cur));
                pos_menu_cursor(my_menu);
                //refresh();
            };
            break;
            case 'q': {
                if (G_FASTQUIT_ON == 1) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Player used q to quit from boss room menu.");
                    picked = 1;
                    choice = getTurnChoice("Quit");
                    pos_menu_cursor(my_menu);
                    //refresh();
                } else {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Player used q in boss room menu, but G_FASTQUIT_ON was not 1.");
                }
            }
            break;
            default: {
                break;
            }
            }
            wrefresh(my_menu_win);
            menu_diff_time = clock() - menu_start_time;
            //menu_loop_diff_time = clock() - menu_loop_time;
            //sprintf(msg,"menu_loop_diff_time was (%li) [CPS %li]", menu_loop_diff_time, (long) CLOCKS_PER_SEC);
            //log_tag("debug_log.txt","[DEBUG]",msg);

            int menu_time_spent = menu_diff_time * 1000 / CLOCKS_PER_SEC;
            //sprintf(time_msg,"[DEBUG-TIME]    Time: %d s %d ms.", menu_time_spent/1000, menu_time_spent%1000);
            //debug_log("debug_log.txt", time_msg);
            if (menu_time_spent % 1000 > 37) {
                //animate_rangeof_sprites_at_coords(enemy_sprites[e->class],enemy_animation_win,frame_counter,frame_counter, 1 , 1, 60, 19, 19, 0, 0);
                s4c_animate_rangeof_sprites_at_coords(fighter_sprites
                                                      [player->class],
                                                      fighter_animation_win,
                                                      frame_counter,
                                                      frame_counter, 1, 1, 60,
                                                      19, 19, 0, 0);
                s4c_animate_rangeof_sprites_at_coords(boss_sprites[b->class],
                                                      boss_animation_win,
                                                      frame_counter,
                                                      frame_counter, 1, 1, 60,
                                                      19, 19, 0, 0);

                if (G_EXPERIMENTAL_ON == 1) {
                    run_time = clock() - gamestate->start_time;
                    time_spent = run_time / CLOCKS_PER_SEC;
                    //log_tag("debug_log.txt", "[DEBUG]", "Current run time: %d s, %d ms.",
                    //    time_spent / 1000, time_spent % 1000);
                    struct tm* time_tm = localtime(&time_spent);

                    if (time_tm == NULL) {
                        log_tag("debug_log.txt", "[ERROR]", "%s():    time_tm was NULL.\n", __func__);
                    } else {
                        strftime(time_str, 20, "Time: %M:%S", time_tm);
                        mvwprintw(notifications_win, 0, 0, "%s", time_str);
                        wrefresh(notifications_win);
                    }
                }

                frame_counter++;
                if (frame_counter > frame_tot) {
                    animation_loops_done++;
                    if (animation_loops_done == 1) {
                        log_tag("debug_log.txt", "[ANIMATE]",
                                "Got to frame %i, restarting animation.",
                                frame_counter);
                    }
                    frame_counter = 0;	//Reset animation loop on last frame
                }
                menu_start_time = clock();	//We reset the timer
                /*
                   sprintf(time_msg,"[DEBUG-TIME]    Time: %d s. %d ms from last msg.",menu_time_spent/1000,menu_time_spent%1000);
                   debug_log("debug_log.txt", time_msg);
                 */
            }
        }			//End while menu
        //We don't endwin immediately if we pick fight

        if (choice == FIGHT) {
            //TODO:
            //Foe picks its turn
            //TODO:
            //Foe picks its turn
            //
            args->foe_op =
                foeTurnOP_from_foeTurnOption(bossTurnPick(b, player));
            log_tag("debug_log.txt", "[FOETURN]", "foePick was: [ %s ]",
                    stringFromFoeTurnOP(args->foe_op));
            //TODO:
            //Handle FIGHT as turnOP(OP_FIGHT)
            fightStatus = turnOP(OP_FIGHT, args, kls, t_kls);
            refresh();
            //Lost battle
            if (fightStatus == OP_RES_DEATH) {
                /* Unpost and free all the memory taken up */
                unpost_menu(my_menu);
                free_menu(my_menu);
                for (int k = 0; k < n_choices; k++) {
                    free_item(my_items[k]);
                }
                free(my_items);
                endwin();

                //Account for oracle gift perk
                int oracle_perks = player->perks[ORACLE_GIFT]->innerValue;
                if (oracle_perks > 0) {
                    int res = system("clear");
                    log_tag("debug_log.txt", "[DEBUG]",
                            "handleRoom_Boss() system(\"clear\") res was (%i)",
                            res);
                    //TODO:
                    //Notification for oracle gift
                    //blue();
                    //printf("\n\n\t\tYou get a second chance, as the prophecy said.\n");
                    //white();
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Fighter [%s] died in room #[%i], Oraclegift activated.",
                            player->name, room->index);
                    player->hp = round(player->totalhp / 3);

                    player->perks[ORACLE_GIFT]->innerValue -= 1;

                    b_death(b);
                    fightStatus = OP_RES_NO_DMG;
                    break;	//Room ends
                }

                b_death(b);

                death(player, load_info);
                //FIXME:is this causing a crash?
                delwin(my_wins[0]);
                delwin(my_wins[1]);
                delwin(my_wins[2]);
                //free(args);
                log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
                return OP_RES_DEATH;
            } else if (fightStatus == OP_RES_KILL_DONE) {
                int dropch;
                int dropped_chars = 0;
                while ((dropch = wgetch(my_menu_win)) != ERR) {
                    // Discard any input characters
                    dropped_chars++;
                }
                log_tag("debug_log.txt", "[DEBUG]", "Dropped ( %i ) inputs",
                        dropped_chars);
                /* Unpost and free all the memory taken up */
                unpost_menu(my_menu);
                free_menu(my_menu);
                for (int k = 0; k < n_choices; k++) {
                    free_item(my_items[k]);
                }
                free(my_items);

                //Give coins

                player->balance += b->prize;
                player->stats->coinsfound += b->prize;

                char msg[50];
                sprintf(msg, "You found +%i coins.", b->prize);
                wattron(notifications_win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
                display_notification(notifications_win, msg, 500);
                wattroff(notifications_win, COLOR_PAIR(S4C_BRIGHT_YELLOW));

                //Give key
                player->keys_balance += 1;
                player->stats->keysfound += 1;

                wattron(notifications_win, COLOR_PAIR(S4C_MAGENTA));
                display_notification(notifications_win,
                                     "You found a key. May be useful.", 800);
                wattroff(notifications_win, COLOR_PAIR(S4C_MAGENTA));

                //Win, get xp and free memory from boss
                giveXp_Boss(player, b);

                b_death(b);

                int res = system("clear");
                log_tag("debug_log.txt", "[DEBUG]",
                        "handleRoom_Boss() 2 system(\"clear\") res was (%i)",
                        res);

                fightStatus = OP_RES_NO_DMG;
                //FIXME: is this causing a crash?
                //YES, doing this causes the game to crash when trying to load a subsequent enemies room.
                //delwin(my_wins[0]);
                //delwin(my_wins[1]);
                //delwin(my_wins[2]);
                clear();
                refresh();
                endwin();
                break;
            }

        } else if (choice == SPECIAL) {
            //TODO:
            //Foe picks its turn
            /* Unpost and free all the memory taken up */
            unpost_menu(my_menu);
            free_menu(my_menu);
            for (int k = 0; k < n_choices; k++) {
                free_item(my_items[k]);
            }
            free(my_items);
            endwin();
            turnOP(OP_SPECIAL, args, kls, t_kls);
        } else if (choice == CONSUMABLE) {
            /* Unpost and free all the memory taken up */
            unpost_menu(my_menu);
            free_menu(my_menu);
            for (int k = 0; k < n_choices; k++) {
                free_item(my_items[k]);
            }
            free(my_items);
            endwin();
            turnOP(OP_CONSUMABLE, args, kls, t_kls);
        } else if (choice == EQUIPS) {
            /* Unpost and free all the memory taken up */
            unpost_menu(my_menu);
            free_menu(my_menu);
            for (int k = 0; k < n_choices; k++) {
                free_item(my_items[k]);
            }
            free(my_items);
            endwin();
            turnOP(OP_EQUIPS, args, kls, t_kls);
        } else if (choice == PERKS) {
            /* Unpost and free all the memory taken up */
            unpost_menu(my_menu);
            free_menu(my_menu);
            for (int k = 0; k < n_choices; k++) {
                free_item(my_items[k]);
            }
            free(my_items);
            endwin();
            turnOP(OP_PERKS, args, kls, t_kls);
        } else if (choice == STATS) {
            /* Unpost and free all the memory taken up */
            unpost_menu(my_menu);
            free_menu(my_menu);
            for (int k = 0; k < n_choices; k++) {
                free_item(my_items[k]);
            }
            free(my_items);
            endwin();
            turnOP(OP_STATS, args, kls, t_kls);
        } else if (choice == DEBUG) {
            // Unpost menu and free all the memory taken up
            unpost_menu(my_menu);
            free_menu(my_menu);
            int totalChoices = n_choices + debug_n_choices;
            if (G_DEBUG_ON) {
                totalChoices -= 1;	//We subtract 1 to account for the discarded null choice in the base option set.
            }
            for (int k = 0; k < totalChoices; k++) {
                free_item(my_items[k]);
            }
            free(my_items);
            endwin();
            turnOP(OP_DEBUG, args, kls, t_kls);
        } else if (choice == ARTIFACTS) {
            /* Unpost and free all the memory taken up */
            unpost_menu(my_menu);
            free_menu(my_menu);
            for (int k = 0; k < n_choices; k++) {
                free_item(my_items[k]);
            }
            free(my_items);
            endwin();
            turnOP(OP_ARTIFACTS, args, kls, t_kls);
        } else if (choice == QUIT) {
            /* Unpost and free all the memory taken up */
            unpost_menu(my_menu);
            free_menu(my_menu);
            for (int k = 0; k < n_choices; k++) {
                free_item(my_items[k]);
            }
            free(my_items);
            endwin();
            turnOP(OP_QUIT, args, kls, t_kls);
        }
        //Update all the player counters
        updateCounters_Boss((Turncounter **) player->counters, 0, player, b);
        //Update all the boss counters
        updateCounters_Boss((Turncounter **) b->counters, 1, player, b);

        //printBattleStats(my_wins,player,e);

        //update_panels();
        refresh();
    }				//End while fightStatus
    //free(args);
    log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");

    update_Gamestate(gamestate, 1, -1, room->index, -1,
                     gamestate->current_floor);
    return fightStatus;
}

/**
 * Takes a Room pointer and a Fighter pointer, displays items for sale and offers a menu to access all the functions.
 * @see Shop
 * @see Fighter
 * @param room The pointer to current room.
 * @param roomsDone The total of rooms completed.
 * @param path The Path pointer.
 * @param f The Fighter pointer at hand.
 * @param kls The Koliseo used for allocations.
 * @param t_kls The Koliseo_Temp used for temporary allocations.
 * @return When shop is exited, should return NO_DMG.
 */
int handleRoom_Shop(Room *room, int roomsDone, Path *path, Fighter *f,
                    Koliseo *kls, Koliseo_Temp *t_kls)
{
    //Strings for turn menu choices
    char *shop_choices[] = {
        "Buy",
        "Sell Equips",
        "View Equips",
        "View Consum.",
        "Leave",
        (char *)NULL,
    };
    WINDOW *wins[2];
    ITEM **my_items;
    MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices, c;

    /* Initialize curses */
    //initscr();
    start_color();

    clear();
    refresh();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    /* Create items */
    n_choices = ARRAY_SIZE(shop_choices);
    my_items = (ITEM **) calloc(n_choices, sizeof(ITEM *));
    for (int k = 0; k < n_choices; k++) {
        my_items[k] = new_item(shop_choices[k], shop_choices[k]);
        /* Set the user pointer */
        //set_item_userptr(my_items[i]);
    }

    /* Create menu */
    my_menu = new_menu((ITEM **) my_items);

    /* Set description off */
    menu_opts_off(my_menu, O_SHOWDESC);

    /* Create the window to be associated with the menu */
    my_menu_win = newwin(7, 20, 18, 0);
    keypad(my_menu_win, TRUE);

    /* Set main window and sub window */
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, 3, 18, 3, 2));
    set_menu_format(my_menu, 3, 1);

    /* Set menu mark to the string " >  " */
    set_menu_mark(my_menu, " >  ");

    char balance_label[25];
    sprintf(balance_label, "Balance: %i $", f->balance);

    /* Print a border around the main window and print a title */
    box(my_menu_win, 0, 0);
    print_label(my_menu_win, 1, 0, 20, balance_label, COLOR_PAIR(S4C_MAGENTA));
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwhline(my_menu_win, 2, 1, ACS_HLINE, 18);
    mvwaddch(my_menu_win, 2, 19, ACS_RTEE);

    /* Post the menu */
    post_menu(my_menu);
    wrefresh(my_menu_win);

    /* Create the shop inventory window */
    wins[0] = newwin(25, 47, 0, 32);
    keypad(wins[0], TRUE);
    box(wins[0], 0, 0);

    /* Create the selection display window */
    wins[1] = newwin(18, 30, 0, 0);
    keypad(wins[1], TRUE);
    box(wins[1], 0, 0);
    print_label(wins[1], 1, 0, 30, "Selected item",
                COLOR_PAIR(S4C_BRIGHT_YELLOW));
    mvwaddch(wins[1], 2, 0, ACS_LTEE);
    mvwhline(wins[1], 2, 1, ACS_HLINE, 28);
    mvwaddch(wins[1], 2, 29, ACS_RTEE);

    Equip *firstEq = room->shop->equips[0];
    updateShopDisplayW_Equip(wins[1], firstEq);
    attron(COLOR_PAIR(S4C_CYAN));
    mvprintw(20, 21, "Arrows to");
    mvprintw(21, 21, "move");
    mvprintw(22, 21, "(q to undo)");
    attroff(COLOR_PAIR(S4C_CYAN));
    refresh();

    int y, x;
    x = 2;
    //Print all the equips
    for (int i = 0; i < room->shop->equipsCount; i++) {
        mvwprintw(wins[0], 1, i * 14 + 3, "%s",
                  stringFromEquips(room->shop->equips[i]->class));
        wattron(wins[0], COLOR_PAIR(S4C_BRIGHT_YELLOW));
        mvwprintw(wins[0], 2, i * 14 + 5, "$ %i", room->shop->equipPrices[i]);
        wattroff(wins[0], COLOR_PAIR(S4C_BRIGHT_YELLOW));
        char item_sprite_line[80];
        for (int rows = 0; rows < 8; rows++) {

            y = 4 + rows;
            x = i * 14 + 2;

            Equip *item = room->shop->equips[i];
            strcpy(item_sprite_line, item->sprite[rows]);
            for (int cols = 0; cols < 14; cols++) {
                print_encoded_char(wins[0], y, x + cols,
                                   item_sprite_line[cols]);
            }
        }
    }

    //Print all the consumables
    for (int i = 0; i < room->shop->consumablesCount; i++) {
        mvwprintw(wins[0], 13, i * 14 + 3, "%s",
                  stringFromConsumables(room->shop->consumables[i]->class));
        wattron(wins[0], COLOR_PAIR(S4C_BRIGHT_YELLOW));
        mvwprintw(wins[0], 14, i * 14 + 5, "$ %i",
                  room->shop->consumablePrices[i]);
        wattroff(wins[0], COLOR_PAIR(S4C_BRIGHT_YELLOW));
        char item_sprite_line[80];
        for (int rows = 0; rows < 8; rows++) {

            y = 16 + rows;
            x = i * 14 + 2;

            Consumable *item = room->shop->consumables[i];
            strcpy(item_sprite_line, item->sprite[rows]);
            for (int cols = 0; cols < 14; cols++) {
                print_encoded_char(wins[0], y, x + cols,
                                   item_sprite_line[cols]);
            }
        }
    }

    wrefresh(wins[0]);
    wrefresh(wins[1]);
    refresh();

    int end_shop = 0;
    int end_scrolling = 0;
    int check = -1;

    int buyEquip = 0;
    int buyConsumable = 0;

    Equip *equipToBuy = NULL;
    Consumable *consumableToBuy = NULL;
    int consumables_index = 0;
    int equips_index = 0;

    while (!end_shop && (c = wgetch(my_menu_win)) != KEY_F(1)) {
        switch (c) {
        case KEY_DOWN:
            menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(my_menu, REQ_UP_ITEM);
            break;
        case KEY_NPAGE:
            menu_driver(my_menu, REQ_SCR_DPAGE);
            break;
        case KEY_PPAGE:
            menu_driver(my_menu, REQ_SCR_UPAGE);
            break;
        case 10: {	/* Enter */
            int ec, cc;
            ITEM *cur;
            end_scrolling = 0;

            cur = current_item(my_menu);
            //mvprintw(18, 47, "Item selected is : %s", item_name(cur));
            if ((check = strcmp("Leave", item_name(cur))) == 0) {
                end_shop = 1;
            } else if ((check = strcmp("Sell Equips", item_name(cur))) == 0) {
                end_shop = 1;
                sell_all_equips(f, t_kls);
            } else if ((check = strcmp("View Equips", item_name(cur))) == 0) {
                equips_index = 0;
                while (!end_scrolling && (ec = wgetch(wins[1])) != 'q') {
                    switch (ec) {
                    case KEY_LEFT: {
                        if (equips_index > 0) {
                            equips_index--;
                        }
                        Equip *e = room->shop->equips[equips_index];
                        wclear(wins[1]);
                        wrefresh(wins[1]);
                        updateShopDisplayW_Equip(wins[1], e);
                        mvwprintw(wins[0], 13, 6, " ");
                        mvwprintw(wins[0], 13, 6 + 12, " ");
                        mvwprintw(wins[0], 13, 6 + 24, " ");
                        mvwprintw(wins[0], 13, 6 + (12 * equips_index),
                                  "X");
                        wrefresh(wins[0]);
                        refresh();
                    }
                    break;
                    case KEY_RIGHT: {
                        if (equips_index < room->shop->equipsCount - 1) {
                            equips_index++;
                        }
                        Equip *e = room->shop->equips[equips_index];
                        wclear(wins[1]);
                        wrefresh(wins[1]);
                        updateShopDisplayW_Equip(wins[1], e);
                        mvwprintw(wins[0], 13, 6, " ");
                        mvwprintw(wins[0], 13, 6 + 12, " ");
                        mvwprintw(wins[0], 13, 6 + 24, " ");
                        mvwprintw(wins[0], 13, 6 + (12 * equips_index),
                                  "X");
                        wrefresh(wins[0]);
                        refresh();
                    }
                    break;
                    case 10: {	/* Enter */
                        equipToBuy = room->shop->equips[equips_index];
                        buyEquip = 1;
                        buyConsumable = 0;
                        end_scrolling = 1;
                    }
                    break;
                    }
                }
            } else if ((check = strcmp("View Consum.", item_name(cur))) ==
                       0) {
                consumables_index = 0;

                while (!end_scrolling && (cc = wgetch(wins[1])) != 'q') {
                    switch (cc) {
                    case KEY_LEFT: {
                        if (consumables_index > 0) {
                            consumables_index--;
                        }
                        Consumable *c =
                            room->shop->consumables[consumables_index];
                        wclear(wins[1]);
                        wrefresh(wins[1]);
                        updateShopDisplayW_Consumable(wins[1], c);
                        mvwprintw(wins[0], 13, 6, " ");
                        mvwprintw(wins[0], 13, 6 + 14, " ");
                        mvwprintw(wins[0], 13, 6 + 28, " ");
                        mvwprintw(wins[0], 13, 6 + 42, " ");
                        mvwprintw(wins[0], 13,
                                  6 + (14 * consumables_index), "X");
                        wrefresh(wins[0]);
                        refresh();
                    }
                    break;
                    case KEY_RIGHT: {
                        if (consumables_index <
                            room->shop->consumablesCount - 1) {
                            consumables_index++;
                        }
                        Consumable *c =
                            room->shop->consumables[consumables_index];
                        wclear(wins[1]);
                        wrefresh(wins[1]);
                        updateShopDisplayW_Consumable(wins[1], c);
                        mvwprintw(wins[0], 13, 6, "  ");
                        mvwprintw(wins[0], 13, 6 + 14, "  ");
                        mvwprintw(wins[0], 13, 6 + 28, "  ");
                        mvwprintw(wins[0], 13, 6 + 42, "  ");
                        mvwprintw(wins[0], 13,
                                  6 + (14 * consumables_index), "X");
                        wrefresh(wins[0]);
                        refresh();
                    }
                    break;
                    case 10: {	/* Enter */
                        consumableToBuy =
                            room->shop->consumables[consumables_index];
                        buyEquip = 0;
                        buyConsumable = 1;
                        end_scrolling = 1;
                    }
                    break;
                    }
                }

            } else if ((check = strcmp("Buy", item_name(cur))) == 0) {
                if (buyEquip) {
                    int price = room->shop->equipPrices[equips_index];
                    if (f->balance >= price) {
                        int slotnum = f->equipsBagOccupiedSlots;
                        //We create a deep copy of the equip so we can free the shop without worrying about the memory sharing with the bag.
                        log_tag("debug_log.txt", "[SHOP]",
                                "Buying Equip %s, deep copy stuff.",
                                stringFromEquips(equipToBuy->class));
                        //TODO
                        //Should use a function to avoid refactoring more points when changing Equip generation.
                        log_tag("debug_log.txt", "[SHOP]",
                                "Prepping Equip for purchase, push to raw default_kls.");
                        kls_log(kls, "DEBUG",
                                "Prepping Equip for purchase, push to raw default_kls.");
                        Equip *saved =
                            (Equip *) KLS_PUSH_TYPED(kls, Equip,
                                                     HR_Equip, "Equip",
                                                     "Equip");
                        Equip *to_save = equipToBuy;

                        saved->class = to_save->class;
                        saved->type = to_save->type;
                        strcpy(saved->name, to_save->name);
                        strcpy(saved->desc, to_save->desc);
                        saved->qty = to_save->qty;
                        saved->equipped = 0;
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
                            log_tag("debug_log.txt", "[SHOP]",
                                    "Prepping Perk (%i/%i) for Equip purchase, push to raw default_kls.",
                                    j, to_save->perksCount);
                            kls_log(default_kls, "DEBUG",
                                    "Prepping Perk (%i/%i) for Equip purchase, push to raw default_kls.",
                                    j, to_save->perksCount);
                            Perk *save_pk =
                                (Perk *) KLS_PUSH_TYPED(default_kls, Perk,
                                                        HR_Perk, "Perk",
                                                        "Perk");
                            save_pk->class = to_save->perks[j]->class;
                            strcpy(save_pk->name, to_save->perks[j]->name);
                            strcpy(save_pk->desc, to_save->perks[j]->desc);
                            save_pk->innerValue =
                                to_save->perks[j]->innerValue;
                            saved->perks[saved->perksCount] = save_pk;
                            saved->perksCount++;
                        }

                        for (int j = 0; j < 8; j++) {
                            strcpy(saved->sprite[j], to_save->sprite[j]);
                        }

                        f->equipsBag[slotnum] = saved;
                        f->equipsBagOccupiedSlots++;
                        f->earliestBagSlot = f->equipsBagOccupiedSlots;
                        f->balance -= price;
                        end_shop = 1;
                    } else {
                        //TODO
                        //PRINT NOT ENOUGH MONEY
                        log_tag("debug_log.txt", "[SHOP]",
                                "Buying Equip %s, TODO: Print NOT ENOUGH MONEY.\n",
                                stringFromEquips(equipToBuy->class));
                    }

                } else if (buyConsumable) {
                    int price =
                        room->shop->consumablePrices[consumables_index];
                    int qty = consumableToBuy->qty;
                    if (f->balance >= price * qty) {
                        log_tag("debug_log.txt", "[SHOP]",
                                "Buying x%i of Consumable %s.\n", qty,
                                stringFromConsumables(consumableToBuy->
                                                      class));
                        Consumable *bagged =
                            (Consumable *) f->
                            consumablesBag[consumableToBuy->class];
                        bagged->qty += qty;
                        f->balance -= (qty * price);
                        end_shop = 1;
                    } else {
                        //TODO
                        //PRINT NOT ENOUGH MONEY
                        log_tag("debug_log.txt", "[SHOP]",
                                "Buying Consumable %s, TODO: Print NOT ENOUGH MONEY.\n",
                                stringFromConsumables(consumableToBuy->
                                                      class));
                    }
                }
            }
            pos_menu_cursor(my_menu);
            refresh();
        };
        break;
        }
        wrefresh(my_menu_win);
    }
    /* Unpost and free all the memory taken up */
    unpost_menu(my_menu);
    free_menu(my_menu);
    for (int k = 0; k < n_choices; k++) {
        free_item(my_items[k]);
    }
    free(my_items);
    delwin(wins[0]);
    delwin(wins[1]);
    endwin();

    return FIGHTRES_NO_DMG;
}

/**
 * Takes a WINDOW, a Chest and a Fighter pointer and opens the chest adding the contents to the fighter's bags.
 * Prints result to the passed window.
 * @see handleRoom_Treasure()
 * @param w The window to print results to.
 * @param c The chest to open.
 * @param f The fighter holding the bags to add the items to.
 * @param kls The Koliseo used for allocations.
 * @param t_kls The Koliseo_Temp used for temp allocations.
 */
void open_chest(WINDOW *w, Chest *c, Fighter *f, Koliseo *kls,
                Koliseo_Temp *t_kls)
{
    wclear(w);
    wrefresh(w);
    box(w, 0, 0);
    wrefresh(w);

    char static_path[500];

    // Set static_path value to the correct static dir path
    resolve_staticPath(static_path);

    /*
     * TODO
     * Remove me
     * Legacy code for loading animation from an s4c-file.
     *
     char path_to_chest_animation[600];

     char chest_animation_filename[200] = "animations/alt_chest_opening.txt";

     sprintf(path_to_chest_animation,"%s/",static_path);
     strncat(path_to_chest_animation,chest_animation_filename,200);

     FILE *file;
     file = fopen(path_to_chest_animation, "r");
     if (!file) {
     fprintf(stderr,"Error opening file %s\n",path_to_chest_animation);
     exit(EXIT_FAILURE);
     }
     */

    int reps = 1;

    int frametime = CHEST_FRAMETIME;

    int num_frames = CHEST_FRAMES;

    int frame_height = CHEST_ROWS;

    int frame_width = CHEST_COLS;

    // Prepare the frames
    char sprites[MAXFRAMES][MAXROWS][MAXCOLS];

    s4c_copy_animation(alt_chest_opening, sprites, num_frames, frame_height,
                       frame_width);

    log_tag("debug_log.txt", "[PREP]",
            "Copied animation from matrix vector for alt_chest_opening with dimensions: [%i][%i][%i].",
            num_frames, frame_height, frame_width);

    /*
     * TODO
     * Remove me
     * Legacy code for loading animation from an s4c-file.
     *
     int loadCheck = load_sprites(sprites, file, frame_height-1, frame_width-1);

     // Check for loading errors and in this case we return early if we couldn't load
     if (loadCheck < 0) {
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
     exit(EXIT_FAILURE);
     }
     */

    int result =
        s4c_animate_sprites_at_coords(sprites, w, reps, frametime, num_frames,
                                      frame_height, frame_width, 2, 2);
    napms(200);

    wclear(w);
    wrefresh(w);

    box(w, 0, 0);
    wrefresh(w);

    // We can check the result to do some actions:
    if (result < 0) {
        switch (result) {
        case S4C_ERR_SMALL_WIN: {
            fprintf(stderr,
                    "[%s] animate => S4C_ERR_SMALL_WIN : Window was too small.\n",
                    __func__);
        }
        break;
        case S4C_ERR_LOADSPRITES: {
            fprintf(stderr,
                    "[%s] animate => S4C_ERR_LOADSPRITES : Failed loading the sprites.\n",
                    __func__);
        }
        break;
        case S4C_ERR_FILEVERSION: {
            fprintf(stderr,
                    "[%s] animate => S4C_ERR_FILEVERSION : Failed file version check.\n",
                    __func__);
        }
        break;
        case S4C_ERR_CURSOR: {
            fprintf(stderr,
                    "[%s] animate => S4C_ERR_CURSOR : Failed to change the cursor.\n",
                    __func__);
        }
        break;
        }
    }

    int y = 2;
    int x = 2;

    if (c->equipsCount > 0) {
        for (int i = 0; i < c->equipsCount; i++) {
            int slotnum = f->equipsBagOccupiedSlots;
            //We create a deep copy of the equip so we can free the chest without worrying about the memory sharing with the bag.
            //TODO
            //Should use a function to avoid refactoring more points when changing Equip generation.
            log_tag("debug_log.txt", "[DEBUG]",
                    "Prepping Equip for Chest, push to raw default_kls");
            kls_log(kls, "DEBUG",
                    "Prepping Equip for Chest, push to raw default_kls");
            Equip *saved =
                (Equip *) KLS_PUSH_TYPED(kls, Equip, HR_Equip, "Equip",
                                         "Equip");
            Equip *to_save = c->equips[i];

            saved->class = to_save->class;
            saved->type = to_save->type;
            strcpy(saved->name, to_save->name);
            strcpy(saved->desc, to_save->desc);
            saved->qty = to_save->qty;
            saved->equipped = 0;
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
                log_tag("debug_log.txt", "[SHOP]",
                        "Prepping Perk (%i/%i) for Equip for Chest.", j,
                        to_save->perksCount);
                kls_log(t_kls->kls, "DEBUG",
                        "Prepping Perk (%i/%i) for Equip for Chest.", j,
                        to_save->perksCount);
                Perk *save_pk =
                    (Perk *) KLS_PUSH_T_TYPED(t_kls, Perk, HR_Perk, "Perk",
                                              "Perk");
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

            f->equipsBag[slotnum] = saved;
            f->equipsBagOccupiedSlots++;
            f->earliestBagSlot = f->equipsBagOccupiedSlots;
            wattron(w, COLOR_PAIR(S4C_MAGENTA));
            mvwprintw(w, y, x, "You found a %s!", saved->name);
            wattroff(w, COLOR_PAIR(S4C_MAGENTA));
            y += 1;
            wrefresh(w);
            napms(800);
        }			//End for all equips
    }				//End if equipsCount > 0
    if (c->consumablesCount > 0) {
        for (int i = 0; i < c->consumablesCount; i++) {
            int num = c->consumables[i]->class;
            Consumable *consum = (Consumable *) f->consumablesBag[num];
            consum->qty += c->consumables[i]->qty;
            f->stats->consumablesfound++;
            wattron(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
            mvwprintw(w, y, x, "You found a %s!",
                      stringFromConsumables(consum->class));
            wattroff(w, COLOR_PAIR(S4C_BRIGHT_GREEN));
            y += 1;
            wrefresh(w);
            napms(800);
        }			//End for all consumables
    }
    wrefresh(w);
}

/**
 * Takes a Room pointer and a Fighter pointer, displays the treasure contents and offers a menu to access all the functions.
 * @see Treasure
 * @see Fighter
 * @param room The pointer to current room.
 * @param roomsDone The total of rooms completed.
 * @param path The Path pointer.
 * @param f The Fighter pointer at hand.
 * @param kls The Koliseo used for allocations.
 * @param t_kls The Koliseo_Temp used for temporary allocations.
 * @return When room is exited, should return NO_DMG.
 */
int handleRoom_Treasure(Room *room, int roomsDone, Path *path, Fighter *f,
                        Koliseo *kls, Koliseo_Temp *t_kls)
{
    //Strings for turn menu choices
    char *treasure_choices[] = {
        "Take Item",
        "Leave",
        (char *)NULL,
    };
    char *chest_choices[] = {
        "Open Chest",
        "Leave",
        (char *)NULL,
    };
    WINDOW *win;
    ITEM **my_items;
    MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices, c;

    /* Initialize curses */
    //initscr();
    start_color();
    clear();
    refresh();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    if (room->treasure->class == TREASURE_CHEST) {
        /* Create items */
        n_choices = ARRAY_SIZE(chest_choices);
        my_items = (ITEM **) calloc(n_choices, sizeof(ITEM *));
        for (int k = 0; k < n_choices; k++) {
            my_items[k] = new_item(chest_choices[k], chest_choices[k]);
            /* Set the user pointer */
            //set_item_userptr(my_items[i]);
        }
    } else {
        /* Create items */
        n_choices = ARRAY_SIZE(treasure_choices);
        my_items = (ITEM **) calloc(n_choices, sizeof(ITEM *));
        for (int k = 0; k < n_choices; k++) {
            my_items[k] = new_item(treasure_choices[k], treasure_choices[k]);
            /* Set the user pointer */
            //set_item_userptr(my_items[i]);
        }
    }

    /* Create menu */
    my_menu = new_menu((ITEM **) my_items);

    /* Set description off */
    menu_opts_off(my_menu, O_SHOWDESC);

    /* Create the window to be associated with the menu */
    my_menu_win = newwin(8, 20, 1, 1);
    keypad(my_menu_win, TRUE);

    /* Set main window and sub window */
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, 3, 18, 3, 2));
    set_menu_format(my_menu, 3, 1);

    /* Set menu mark to the string " >  " */
    set_menu_mark(my_menu, " >  ");

    char label[25];
    sprintf(label, "Menu");

    /* Print a border around the main window and print a title */
    box(my_menu_win, 0, 0);
    print_label(my_menu_win, 1, 0, 20, label, COLOR_PAIR(S4C_MAGENTA));
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwhline(my_menu_win, 2, 1, ACS_HLINE, 18);
    mvwaddch(my_menu_win, 2, 19, ACS_RTEE);

    /* Post the menu */
    post_menu(my_menu);
    wrefresh(my_menu_win);

    /* Create the items window */
    win = newwin(22, 54, 1, 23);
    keypad(win, TRUE);
    box(win, 0, 0);

    int title_color = -1;
    char title[80];
    switch (room->treasure->class) {
    case TREASURE_CHEST: {
        title_color = S4C_MAGENTA;
        strcpy(title, "Chest");
    }
    break;
    case TREASURE_CONSUMABLE: {
        title_color = S4C_BRIGHT_GREEN;
        strcpy(title, "Consumable");
    }
    break;
    case TREASURE_ARTIFACT: {
        title_color = S4C_CYAN;
        strcpy(title, "Artifact");
    }
    break;
    }

    print_label(win, 1, 0, 54, title, COLOR_PAIR(title_color));
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, 52);
    mvwaddch(win, 2, 53, ACS_RTEE);

    attron(COLOR_PAIR(S4C_CYAN));
    mvprintw(20, 2, "Arrows to move");
    mvprintw(21, 2, "(q to Exit)");
    attroff(COLOR_PAIR(S4C_CYAN));
    refresh();

    int y, x;
    x = 2;

    if (room->treasure->class == TREASURE_CONSUMABLE) {
        wattron(win, COLOR_PAIR(title_color));
        mvwprintw(win, 4, 2, "You found a %s!",
                  stringFromConsumables(room->treasure->consumable->class));
        wattroff(win, COLOR_PAIR(title_color));
        char item_sprite_line[80];
        for (int rows = 0; rows < 8; rows++) {
            y = 8 + rows;
            x = 15;

            Consumable *item = room->treasure->consumable;
            strcpy(item_sprite_line, item->sprite[rows]);
            for (int cols = 0; cols < 14; cols++) {
                print_encoded_char(win, y, x + cols, item_sprite_line[cols]);
            }
        }

    } else if (room->treasure->class == TREASURE_ARTIFACT) {
        wattron(win, COLOR_PAIR(title_color));
        mvwprintw(win, 4, 2, "You found a %s!",
                  stringFromArtifacts(room->treasure->artifact->class));
        wattroff(win, COLOR_PAIR(title_color));
        char item_sprite_line[80];
        for (int rows = 0; rows < 8; rows++) {
            y = 8 + rows;
            x = 15;

            Artifact *item = room->treasure->artifact;
            strcpy(item_sprite_line, item->sprite[rows]);
            for (int cols = 0; cols < 14; cols++) {
                print_encoded_char(win, y, x + cols, item_sprite_line[cols]);
            }
        }

    } else if (room->treasure->class == TREASURE_CHEST) {
        char chest_name[80];
        switch (room->treasure->chest->class) {
        case CHEST_BASE: {
            strcpy(chest_name, " ");
        }
        break;
        case CHEST_BEAST: {
            strcpy(chest_name, " Golden ");
        }
        break;

        }
        wattron(win, COLOR_PAIR(title_color));
        mvwprintw(win, 4, 2, "You found a%sChest!", chest_name);
        wattroff(win, COLOR_PAIR(title_color));
        char item_sprite_line[80];
        for (int rows = 0; rows < 8; rows++) {
            y = 6 + rows;
            x = 15;

            Chest *c = room->treasure->chest;
            strcpy(item_sprite_line, c->sprite[rows]);
            for (int cols = 0; cols < 14; cols++) {
                print_encoded_char(win, y, x + cols, item_sprite_line[cols]);
            }
        }

    }

    wrefresh(win);
    refresh();

    int end_room = 0;
    int check = -1;

    while (!end_room && (c = wgetch(my_menu_win)) != KEY_F(1)) {
        switch (c) {
        case KEY_DOWN:
            menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(my_menu, REQ_UP_ITEM);
            break;
        case KEY_NPAGE:
            menu_driver(my_menu, REQ_SCR_DPAGE);
            break;
        case KEY_PPAGE:
            menu_driver(my_menu, REQ_SCR_UPAGE);
            break;
        case 10: {	// Enter
            ITEM *cur;

            cur = current_item(my_menu);
            if ((check = strcmp("Leave", item_name(cur))) == 0) {
                end_room = 1;
            } else if (room->treasure->class == TREASURE_CHEST
                       && (check =
                               strcmp("Open Chest", item_name(cur)) == 0)) {
                if (f->keys_balance > 0) {
                    open_chest(win, room->treasure->chest, f, kls, t_kls);
                    log_tag("debug_log.txt", "[TREASURE]",
                            "Opened chest in Treasure room, index %i.\n",
                            room->index);
                    f->keys_balance--;
                    end_room = 1;
                } else {
                    wattron(win, COLOR_PAIR(S4C_RED));
                    mvwprintw(win, 18, 5, "You don't have any key.");
                    wattroff(win, COLOR_PAIR(S4C_RED));
                    wrefresh(win);
                    log_tag("debug_log.txt", "[TREASURE]",
                            "Tried Opening a chest in Treasure room with no keys, index %i.\n",
                            room->index);
                }
            } else if (room->treasure->class == TREASURE_CONSUMABLE
                       && (check =
                               strcmp("Take Item", item_name(cur)) == 0)) {
                Consumable *bagged =
                    (Consumable *) f->consumablesBag[room->treasure->
                                                     consumable->class];
                bagged->qty += room->treasure->consumable->qty;
                f->stats->consumablesfound++;
                end_room = 1;
            } else if (room->treasure->class == TREASURE_ARTIFACT
                       && (check =
                               strcmp("Take Item", item_name(cur)) == 0)) {
                Artifact *bagged =
                    (Artifact *) f->artifactsBag[room->treasure->artifact->
                                                 class];
                bagged->qty += 1;
                f->stats->artifactsfound++;
                end_room = 1;
            }
            pos_menu_cursor(my_menu);
            refresh();
        };
        break;
        }
        wrefresh(my_menu_win);
    }
    /* Unpost and free all the memory taken up */
    unpost_menu(my_menu);
    free_menu(my_menu);
    for (int k = 0; k < n_choices; k++) {
        free_item(my_items[k]);
    }
    free(my_items);
    delwin(win);
    endwin();

    return FIGHTRES_NO_DMG;
}

#endif // HELAPORDO_CURSES_BUILD

/**
 * Takes one Room pointer of class HOME and initialises all the fields.
 * @see roomClass
 * @see Room
 * @param r The Room whose fields will be initialised.
 * @param roomIndex The index of the room to initialise.
 * @param f The fighter pointer.
 * @param load_info Contains values used to handle loading a gme.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initRoom_Home(Room *r, int roomIndex, Fighter *f, loadInfo *load_info,
                   Koliseo_Temp *t_kls)
{
    log_tag("debug_log.txt", "[DEBUG]", "Allocated size %lu for Room desc:",
            sizeof("Home"));
    kls_log(t_kls->kls, "DEBUG", "Allocated size %lu for Room desc:",
            sizeof("Home"));
    r->desc =
        (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *, sizeof("HOME"), HR_Room_desc,
                                     "Room desc", "Room desc");
    strcpy(r->desc, "Home");
    if (!(load_info->is_new_game)) {
        log_tag("debug_log.txt", "[DEBUG]",
                "initRoom_Home() for a loaded game");
    }
}

/**
 * Takes one Room pointer of class ENEMIES and initialises all the fields.
 * @see roomClass
 * @see Room
 * @param r The Room whose fields will be initialised.
 * @param roomIndex The index of the room to initialise.
 * @param enemyTotal The total number of enemies to prepare for the room.
 * @param load_info Contains values used to handle loading a gme.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initRoom_Enemies(Room *r, int roomIndex, int enemyTotal,
                      loadInfo *load_info, Koliseo_Temp *t_kls)
{
    log_tag("debug_log.txt", "[DEBUG]", "Allocated size %lu for Room desc:",
            sizeof("Enemies"));
    kls_log(t_kls->kls, "DEBUG", "Allocated size %lu for Room desc:",
            sizeof("Enemies"));
    r->desc =
        (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *, sizeof("Enemies"), HR_Room_desc,
                                     "Room desc", "Room desc");
    strcpy(r->desc, "Enemies");
    if (enemyTotal <= ROOM_ENEMIES_MAX) {
        r->index = roomIndex;

        r->enemyTotal = enemyTotal;
        log_tag("debug_log.txt", "[DEBUG]", "Allocated size %lu for FoeParty:",
                sizeof(FoeParty));
        kls_log(t_kls->kls, "DEBUG", "Allocated size %lu for FoeParty:",
                sizeof(FoeParty));
        FoeParty *foes =
            (FoeParty *) KLS_PUSH_T_TYPED(t_kls, FoeParty, HR_FoeParty,
                                          "Foeparty", "Foeparty");

        //Randomise fp class
        //foes->class = rand() % (FOEPARTY_CLASS_MAX + 1);
        foes->class = Enemies;
        int enemyIndex = 0;
        int total_foes = r->enemyTotal;
        if (!(load_info->is_new_game) && !(load_info->done_loading)
            && (load_info->save_type == ENEMIES_SAVE)) {
            //enemyIndex = load_info->enemy_index;
            r->enemies[enemyIndex] = load_info->loaded_enemy;
            total_foes = load_info->total_foes - load_info->enemy_index;
            log_tag("debug_log.txt", "[DEBUG-LOAD]",
                    "Set total_foes to %i, will be used to prepare FoeParty.",
                    total_foes);

            //enemyIndex is the local var used to loop through enemies in a room and needs to be updated here
            enemyIndex++;

            load_info->done_loading = 1;	//Done all loading
            log_tag("debug_log.txt", "[DEBUG-LOAD]", "Set done_loading to 1.");
            log_tag("debug_log.txt", "[DEBUG-LOAD]",
                    "Done loading: %s as first enemy, room #%i.",
                    stringFromEClass(r->enemies[0]->class), r->index);

        }
        //Reminder: Foes are manipulated when loading
        prepareFoeParty(foes, total_foes, roomIndex, t_kls);

        r->foes = foes;
        log_tag("debug_log.txt", "[DEBUG-LOAD]",
                "Set (%s) FoeParty for room #%i , size is (%i).",
                stringFromFoePartyClass(r->foes->class), r->index,
                r->foes->size);

        for (; enemyIndex < enemyTotal; enemyIndex++) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "Allocated size %lu for room Enemy (%i/%i):", sizeof(Enemy),
                    enemyIndex, enemyTotal);
            kls_log(t_kls->kls, "DEBUG",
                    "Allocated size %lu for room Enemy (%i/%i):", sizeof(Enemy),
                    enemyIndex, enemyTotal);
            Enemy *e = KLS_PUSH_T_TYPED(t_kls, Enemy, HR_Enemy, "Enemy", "Enemy");	//&room_enemies[enemyIndex];
            prepareRoomEnemy(e, r->index, r->enemyTotal, enemyIndex, t_kls);
            r->enemies[enemyIndex] = e;
            //Set FoeParty links
            r->foes->enemy_foes[enemyIndex] = e;
            log_tag("debug_log.txt", "[FOEPARTY]",
                    "Room #(%i):    Set (%s) into room->foes->enemy_foes[%i]).",
                    r->index,
                    stringFromEClass(r->foes->enemy_foes[enemyIndex]->class),
                    enemyIndex);
        }			//End for all enemies

        //Set room enemies
        //for (int i = 0; i < enemyTotal +1; i++) {
        //      r->enemies[i] = &room_enemies[i];
        //}
        //r->enemies = enemies;
    } else {
        log_tag("debug_log.txt", "[FOEPARTY]",
                "ERROR: Room %i can't have %i enemies (>MAX %i)\n", roomIndex,
                enemyTotal, ROOM_ENEMIES_MAX);
        exit(EXIT_FAILURE);
    }
}

/**
 * Takes one Room pointer of class SHOP and initialises all the fields, using stats from the passed fighter pointer to generate items.
 * @see roomClass
 * @see Room
 * @see Shop
 * @param r The Room whose fields will be initialised.
 * @param roomIndex The index of the room to initialise.
 * @param f The Fighter pointer to influence item generation.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initRoom_Shop(Room *r, int roomIndex, Fighter *f, Koliseo_Temp *t_kls)
{
    log_tag("debug_log.txt", "[DEBUG]",
            "Allocated size %lu for Room Shop desc:", sizeof("Shop"));
    kls_log(t_kls->kls, "DEBUG", "Allocated size %lu for Room Shop desc:",
            sizeof("Shop"));
    r->desc =
        (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *, sizeof("Shop"), HR_Room_desc,
                                     "Room desc", "Room desc");
    strcpy(r->desc, "Shop");
    log_tag("debug_log.txt", "[DEBUG]", "Allocated size %lu for Room Shop :",
            sizeof("Shop"));
    kls_log(t_kls->kls, "DEBUG", "Allocated size %lu for Room Shop :",
            sizeof("Shop"));
    Shop *shop =
        (Shop *) KLS_PUSH_T_TYPED(t_kls, Shop, HR_Shop, "Shop", "Shop");

    int indexWeight = roomIndex;
    initShop(shop, indexWeight, f, t_kls);
    r->shop = shop;
}

/**
 * Takes one Room pointer of class BOSS and initialises all the fields, using stats from the passed fighter pointer to generate items.
 * @see roomClass
 * @see Room
 * @see Shop
 * @param r The Room whose fields will be initialised.
 * @param roomIndex The index of the room to initialise.
 * @param f The Fighter pointer to influence item generation.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initRoom_Boss(Room *r, int roomIndex, Fighter *f, Koliseo_Temp *t_kls)
{
    log_tag("debug_log.txt", "[DEBUG]", "Allocated size %lu for Room desc:",
            sizeof("Boss"));
    kls_log(t_kls->kls, "DEBUG", "Allocated size %lu for Room desc:",
            sizeof("Boss"));
    r->desc =
        (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *, sizeof("Boss"), HR_Room_desc,
                                     "Room desc", "Room desc");
    strcpy(r->desc, "Boss");
    log_tag("debug_log.txt", "[DEBUG]", "Allocated size %lu for Room Boss:",
            sizeof(Boss));
    kls_log(t_kls->kls, "DEBUG", "Allocated size %lu for Room Boss:",
            sizeof(Boss));
    Boss *b =
        (Boss *) KLS_PUSH_T_TYPED(t_kls, Boss, HR_Boss, "Boss", "Boss");

    prepareBoss(b, t_kls);
    r->boss = b;
}

/**
 * Takes one Room pointer of class TREASURE and initialises all the fields, using stats from the passed fighter pointer to generate items.
 * @see roomClass
 * @see Room
 * @see Treasure
 * @param r The Room whose fields will be initialised.
 * @param roomIndex The index of the room to initialise.
 * @param f The Fighter pointer to influence item generation.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initRoom_Treasure(Room *r, int roomIndex, Fighter *f, Koliseo_Temp *t_kls)
{
    log_tag("debug_log.txt", "[DEBUG]", "Allocated size %lu for Room desc:",
            sizeof("Treasure"));
    kls_log(t_kls->kls, "DEBUG", "Allocated size %lu for Room desc:",
            sizeof("Treasure"));
    r->desc =
        (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *, sizeof("Treasure"),
                                     HR_Room_desc, "Room desc", "Room desc");
    strcpy(r->desc, "Treasure");
    log_tag("debug_log.txt", "[DEBUG]", "Allocated size %lu for Room Treasure:",
            sizeof(Treasure));
    kls_log(t_kls->kls, "DEBUG", "Allocated size %lu for Room Treasure:",
            sizeof(Treasure));
    Treasure *t =
        (Treasure *) KLS_PUSH_T_TYPED(t_kls, Treasure, HR_Treasure,
                                      "Treasure", "Treasure");
    prepareTreasure(t, f, t_kls);
    r->treasure = t;
}

/**
 * Takes one Room pointer of class ROADFORK and initialises all the fields, using stats from the passed fighter pointer to generate items.
 * @see roomClass
 * @see Room
 * @see Roadfork
 * @param r The Room whose fields will be initialised.
 * @param roomIndex The index of the room to initialise.
 * @param f The Fighter pointer to influence item generation.
 */
void initRoom_Roadfork(Room *r, int roomIndex, Fighter *f, Koliseo_Temp *t_kls)
{
    log_tag("debug_log.txt", "[WARN]", "Entering legacy path.");
    log_tag("debug_log.txt", "[DEBUG]", "Allocated size %lu for Room desc:",
            sizeof("Roadfork"));
    kls_log(t_kls->kls, "DEBUG", "Allocated size %lu for Room desc:",
            sizeof("Roadfork"));
    r->desc =
        (char *)KLS_PUSH_ARR_T_TYPED(t_kls, char *, sizeof("Roadfork"),
                                     HR_Room_desc, "Room desc", "Room desc");
    strcpy(r->desc, "Roadfork");
    log_tag("debug_log.txt", "[DEBUG]", "Allocated size %lu for Room Roadfork:",
            sizeof(Roadfork));
    kls_log(t_kls->kls, "DEBUG", "Allocated size %lu for Room Roadfork:",
            sizeof(Roadfork));
    Roadfork *fk =
        (Roadfork *) KLS_PUSH_T_TYPED(t_kls, Roadfork, HR_Roadfork,
                                      "Roadfork", "Roadfork");

    prepareRoadfork(fk);
    r->roadfork = fk;
}

/**
 * Takes one Room pointer and calls the correct initialising function, after setting the room class value.
 * The passed Fighter pointer is used to influence room generation.
 * @see Path
 * @param r The Room whose fields will be initialised.
 * @param f The Fighter to influence room generation.
 * @param index The index of the room to initialise.
 * @param type The roomClass of the room that will be initialised.
 * @param enemyTotal The total number of enemies to prepare for the room.
 * @param load_info Contains values used to handle loading a gme.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initRoom(Room *r, Fighter *f, int index, roomClass type, int enemyTotal,
              loadInfo *load_info, Koliseo_Temp *t_kls)
{
    //Init room
    if (load_info->is_new_game || load_info->done_loading) {
        enemyTotal = (rand() % (ROOM_ENEMIES_MAX)) + 1;
    } else {			//We subtract how many enemies we have already done before loading
        enemyTotal -= load_info->enemy_index;
    }

    switch (type) {
    case BASIC: {
        log_tag("debug_log.txt", "[DEBUG]", "initRoom() for BASIC");
        r->class = type;
        log_tag("debug_log.txt", "[TODO]", "initRoom() for BASIC");
        exit(EXIT_FAILURE);
    }
    break;
    case WALL: {
        log_tag("debug_log.txt", "[DEBUG]", "initRoom() for WALL");
        r->class = type;
        log_tag("debug_log.txt", "[TODO]", "initRoom() for WALL");
        exit(EXIT_FAILURE);
    }
    break;
    case HOME: {
        log_tag("debug_log.txt", "[DEBUG]", "initRoom() for HOME");
        r->class = type;
        initRoom_Home(r, index, f, load_info, t_kls);
    }
    break;
    case ENEMIES: {
        r->class = type;
        initRoom_Enemies(r, index, enemyTotal, load_info, t_kls);
    }
    break;
    case SHOP: {
        r->class = type;
        initRoom_Shop(r, index, f, t_kls);
    }
    break;
    case BOSS: {
        r->class = type;
        initRoom_Boss(r, index, f, t_kls);
    }
    break;
    case TREASURE: {
        r->class = type;
        initRoom_Treasure(r, index, f, t_kls);
    }
    break;
    case ROADFORK: {
        r->class = type;
        initRoom_Roadfork(r, index, f, t_kls);
    }
    break;
    }

}
