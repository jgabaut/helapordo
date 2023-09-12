#include "game_curses.h"
#include "helapordo.h"

callback_void_t callback_func_ptrs[SPECIALSMAX];
callback_void_t callback_artifact_ptrs[ARTIFACTSMAX];
callback_void_t callback_counter_ptrs[COUNTERSMAX];

/**
 * Runs some shell commands to see all color pairs, then returns exitcode
 * Prints the encoded value of the passed char to the window at the coordinates.
 */
int display_colorpairs(void) {
	//Done checking versions, we check colors
	int status = system(" clear; for C in {0..255}; do {     tput setab $C;     echo -n \"$C \"; } ; done ; tput sgr0 ; echo");
	int exitcode = status / 256;
	if (exitcode != 0) {
		log_tag("debug_log.txt","[DEBUG]","\"Diplay colors\" failed.\n");
	}
	return(exitcode);
}

/**
 * Takes a WINDOW pointer, two integers for coordinates and a char.
 * Prints the encoded value of the passed char to the window at the coordinates.
 * @param w A pointer to the window to print to.
 * @param y Coordinate to print at.
 * @param x Coordinate to print at.
 * @param c The encoded char to print.
 */
void print_encoded_char(WINDOW* w, int y, int x, char c) {
	if (c == ' ') {
		wattron(w,COLOR_PAIR(S4C_BLACK));
		//mvwprintw(win, y, x, "%c",s[j]);
		mvwaddch(w, y, x,' '|A_REVERSE);
		wattroff(w,COLOR_PAIR(S4C_BLACK));
	} else if (c == '@') {
		wattron(w,COLOR_PAIR(S4C_BLACK));
		//mvwaddch(win, y, x, cBlock);
		mvwaddch(w, y, x,' '|A_REVERSE);
		wattroff(w,COLOR_PAIR(S4C_BLACK));
	} else if (c == 'w') {
		wattron(w,COLOR_PAIR(S4C_WHITE));
		mvwaddch(w, y, x, ' '|A_REVERSE);
		wattroff(w,COLOR_PAIR(S4C_WHITE));
	} else if (c == '|') {
		wattron(w,COLOR_PAIR(S4C_BRIGHT_GREEN));
		mvwaddch(w, y, x, '|');
		wattroff(w,COLOR_PAIR(S4C_BRIGHT_GREEN));
	} else if (c == 'g') {
		wattron(w,COLOR_PAIR(S4C_BRIGHT_GREEN));
		mvwaddch(w, y, x, ' '|A_REVERSE);
		wattroff(w,COLOR_PAIR(S4C_BRIGHT_GREEN));
	} else if (c == 'b') {
		wattron(w,COLOR_PAIR(S4C_BLUE));
		mvwaddch(w, y, x, ' '|A_REVERSE);
		wattroff(w,COLOR_PAIR(S4C_BLUE));
	} else if (c == 'y') {
		wattron(w,COLOR_PAIR(S4C_BRIGHT_YELLOW));
		mvwaddch(w, y, x, ' '|A_REVERSE);
		wattroff(w,COLOR_PAIR(S4C_BRIGHT_YELLOW));
	} else if (c == 'r') {
		wattron(w,COLOR_PAIR(S4C_RED));
		mvwaddch(w, y, x, ' '|A_REVERSE);
		wattroff(w,COLOR_PAIR(S4C_RED));
	} else if (c == 'm') {
		wattron(w,COLOR_PAIR(S4C_MAGENTA));
		mvwaddch(w, y, x, ' '|A_REVERSE);
		wattroff(w,COLOR_PAIR(S4C_MAGENTA));
	} else if (c == 'c') {
		wattron(w,COLOR_PAIR(S4C_CYAN));
		mvwaddch(w, y, x, ' '|A_REVERSE);
		wattroff(w,COLOR_PAIR(S4C_CYAN));
	} else if (c == 'I') {
		wattron(w,COLOR_PAIR(S4C_WHITE));
		mvwaddch(w, y, x, '|');
		wattroff(w,COLOR_PAIR(S4C_WHITE));
	}

}

/**
 * Returns index of saveslot picked by user.
 * @see Path
 * @see gameloop()
 * @return When exiting room, should return NO_DMG.
 */
int get_saveslot_index(void) {
	//Strings for turn menu choices
 	char *choices[] = {
		"1",
		"2",
		"3",
		(char *)NULL,
	};

	log_tag("debug_log.txt","[DEBUG]","get_saveslot_index(): getting index from user.");

	int choice = 999;
	int picked = 0;

	ITEM **menu_items;
	MENU *saveslots_menu;
	WINDOW *menu_win;
	WINDOW *saveslots_win;
	int n_choices, c;

	log_tag("debug_log.txt","[DEBUG]","Initialising curses for get_saveslots_index()");
	/* Initialize curses */
	start_color();
	clear();
	refresh();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	int cursorCheck = curs_set(0); // We make the cursor invisible or return early with the error

	if (cursorCheck == ERR) {
		log_tag("debug_log.txt","[ERROR]","Failed curs_set(0) at handleRoom_Home()");
		return S4C_ERR_CURSOR; //fprintf(stderr,"animate => Terminal does not support cursor visibility state.\n");
	}

	/* Create turn menu items */
	n_choices = ARRAY_SIZE(choices);
	menu_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));

	// Prepare menu items
	for(int k = 0; k < n_choices; k++) {
		menu_items[k] = new_item(choices[k], choices[k]);
	    /* Set the user pointer */
	    //set_item_userptr(my_items[i]);
	}

	/* Create menu */
	saveslots_menu = new_menu((ITEM **)menu_items);

	/* Set description off */
	menu_opts_off(saveslots_menu,O_SHOWDESC);
	/* Create the window to be associated with the menu */
	menu_win = newwin(9, 11, 5, 35);
	//nodelay(menu_win,TRUE); //We make getch non-blocking
	keypad(menu_win, TRUE);

	/* Set main window and sub window */
	set_menu_win(saveslots_menu, menu_win);
	set_menu_sub(saveslots_menu, derwin(menu_win, 8, 10, 1, 1));
	set_menu_format(saveslots_menu, 7, 1);

	/* Set menu mark to the string "" */
	set_menu_mark(saveslots_menu, "");

	/* Print a border around the menu */
	box(menu_win, 0, 0);

	/* Set menu colors */
	//set_menu_fore(saveslots_menu,COLOR_PAIR(5));
	//set_menu_back(saveslots_menu,COLOR_PAIR(7));

	//mvprintw(LINES - 1, 0, "Arrow Keys to navigate (F1 to Exit)");
	//attroff(COLOR_PAIR(2));

	/* Post the menu */
	post_menu(saveslots_menu);
	wrefresh(menu_win);

	saveslots_win = newwin(12, 24, 2, 5);
	scrollok(saveslots_win,TRUE);
	wprintw(saveslots_win,"\n  Select your save slot.");
	wprintw(saveslots_win,"\n  1: %s",default_saveslots[0].name);
	wprintw(saveslots_win,"\n  2: %s",default_saveslots[1].name);
	wprintw(saveslots_win,"\n  3: %s",default_saveslots[2].name);
	wrefresh(saveslots_win);
	refresh();

	/*
	//We set the colors to use s4c's palette file...
	FILE* palette_file;
	char path_to_palette[600];
	char static_path[500];
	char palette_name[50] = "palette.gpl";

	// Set static_path value to the correct static dir path
	resolve_staticPath(static_path);

	sprintf(path_to_palette,"%s/%s",static_path,palette_name);

	palette_file = fopen(path_to_palette, "r");

	init_s4c_color_pairs(palette_file);
	*/

	while ( !picked && (c = wgetch(menu_win)) != KEY_F(1)) {
		switch(c) {
			case KEY_DOWN:
				menu_driver(saveslots_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(saveslots_menu, REQ_UP_ITEM);
				break;
			case KEY_LEFT: { /*Left option pick*/
				ITEM *cur;
				cur = current_item(saveslots_menu);
				choice = getTurnChoice((char*)item_name(cur));
				log_tag("debug_log.txt","[DEBUG]","Left on choice: [ %s ] value (%i)",item_name(cur),choice);
				if (choice == EQUIPS) {
					log_tag("debug_log.txt","[DEBUG]","Should do something");
				}
				}
				break;
			case KEY_RIGHT: { /*Right option pick*/
				ITEM *cur;
				cur = current_item(saveslots_menu);
				choice = getTurnChoice((char*)item_name(cur));
				log_tag("debug_log.txt","[DEBUG]","Right on choice: [ %s ] value (%i)",item_name(cur),choice);
				if (choice == EQUIPS) {
					log_tag("debug_log.txt","[DEBUG]","Should do something");
				}
				}
				break;
			case KEY_NPAGE:
				menu_driver(saveslots_menu, REQ_SCR_DPAGE);
				break;
			case KEY_PPAGE:
				menu_driver(saveslots_menu, REQ_SCR_UPAGE);
				break;
			case 10: /* Enter */
			{
				picked = 1;
				ITEM *cur;
				//move(18,47);
				//clrtoeol();
				cur = current_item(saveslots_menu);
				//mvprintw(18, 47, "Item selected is : %s", item_name(cur));
				choice = atoi(item_name(cur));
				pos_menu_cursor(saveslots_menu);
				refresh();
			};
			break;
			case 'q':
			{
				if (G_FASTQUIT_ON == 1) {
					log_tag("debug_log.txt","[DEBUG]","Player used q to quit from home room menu.");
					picked = 1;
					choice = getTurnChoice("Quit");
					pos_menu_cursor(saveslots_menu);
					refresh();
				} else {
					log_tag("debug_log.txt","[DEBUG]","Player used q in home room menu, but G_FASTQUIT_ON was not 1.");
				}
			}
			break;
			default: {
					log_tag("debug_log.txt","[DEBUG]","Invalid keystroke in home room menu");
			}
			break;
		}
		wrefresh(menu_win);
		refresh();

		if (c == 10) { // Player char was enter
			if (choice < 0 || choice > 3) {
				log_tag("debug_log.txt","[ERROR]","Invalid choice in get_saveslot_index().");
				return -1;
			}
		} //End if Player char was enter

	} // end while !picked
	// Unpost menu and free all the memory taken up
	unpost_menu(saveslots_menu);
	free_menu(saveslots_menu);
	log_tag("debug_log.txt","[FREE]","Freed saveslots menu");
	int totalChoices = n_choices;
	for(int k = 0; k < totalChoices; k++) {
		free_item(menu_items[k]);
		log_tag("debug_log.txt","[FREE]","Freed %i saveslots menu item",k);
	}
	free(menu_items);

	delwin(saveslots_win);
	endwin();
	log_tag("debug_log.txt","[DEBUG]","Ended window mode for get_saveslot_index()");
	log_tag("debug_log.txt","[DEBUG]","Ended get_saveslot_index(), returning [%i]",choice);
	//TODO
	//Why have the choice misaligned?
	return choice -1;
}

/**
 * Displays a certain lore string.
 * @param lore_strings The array of strings to display from.
 * @param lore_counter The index of current string.
 * @see gameloop()
 */
void displayLore(char** lore_strings, int lore_counter) {

	WINDOW *win;

	/* Initialize curses */
	//setlocale(LC_CTYPE, "it_IT.UTF-8");
	//initscr();
	clear();
	refresh();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	/* Create the windows for lore */
        win = newwin(20, 70, 1, 2);
        keypad(win, TRUE);

	wclear(win);
	wrefresh(win);

	/* Print a border around the windows and print a title */
        //box(win, 0, 0);
	char label[25];
	sprintf(label,"Chapter %i",lore_counter+1);
	print_label(win, 1, 0, 70, label, COLOR_PAIR(S4C_BLUE));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 68);
	mvwaddch(win, 2, 69, ACS_RTEE);

	log_tag("debug_log.txt","[LORE]","%s",lore_strings[lore_counter]);
	wattron(win,COLOR_PAIR(S4C_BRIGHT_YELLOW));
	mvwprintw(win, 4, 1, "%s", lore_strings[lore_counter]);
	wattroff(win,COLOR_PAIR(S4C_BRIGHT_YELLOW));

	wrefresh(win);
	napms(800);
	char c;
	int res = scanf("%c",&c);
	log_tag("debug_log.txt","[DEBUG]","displayLore() scanf() res was (%i)",res);
	delwin(win);
	endwin();
}

/**
 * Takes a Fighter and a Enemy pointers and prints most of their values formatted.
 * For the Fighter, the equipboost values are also displayed.
 * @see Fighter
 * @see Enemy
 * @see stringFromClass()
 * @see stringFromEClass()
 * @see stringFromStatus()
 * @see getEnemyXpGain()
 * @param wins The WINDOW array to print fighter, enemy an stat legend to.
 * @param f The Fighter pointer with stats to print.
 * @param e The Enemy pointer with stats to print.
 */
void printBattleStats(WINDOW* wins[3], Fighter* f, Enemy* e) {

	for (int i=0; i < 3; i++) {
		if ( wins[i] == NULL ) {
			log_tag("debug_log.txt","[ERROR]","printBattleStats():  wins[%i] was NULL",i);
			exit(EXIT_FAILURE);
		}
	}


	PANEL  *my_panels[3];
	PANEL  *top;


	/* Attach a panel to each window */ 	/* Order is bottom up */
	my_panels[0] = new_panel(wins[2]); 	/* Push 0, order: stdscr-0 */
	my_panels[1] = new_panel(wins[1]); 	/* Push 1, order: stdscr-0-1 */
	my_panels[2] = new_panel(wins[0]); 	/* Push 2, order: stdscr-0-1-2 */


	/* Set up the user pointers to the next panel */
	set_panel_userptr(my_panels[0], my_panels[1]);
	set_panel_userptr(my_panels[1], my_panels[2]);
	set_panel_userptr(my_panels[2], my_panels[0]);

	/* Update the stacking order. 3nd panel will be the last one*/


	update_panels();


	/* Show it on the screen */
	//attron(COLOR_PAIR(4));
	//mvprintw(LINES - 2, 0, "MENU PROMPT (F1 to Exit)");
	//attroff(COLOR_PAIR(4));
	doupdate();

	top = my_panels[2];
	top = (PANEL *)panel_userptr(top);
	top_panel(top);
	top = (PANEL *)panel_userptr(top);
	top_panel(top);
	update_panels();
	doupdate();

}

/**
 * Takes an array pointer of WINDOW pointers, a Fighter and a Enemy pointers and initialises the passed windows.
 * @see Fighter
 * @see Enemy
 * @param wins The WINDOW array pointer to initialise.
 * @param f The Fighter pointer with stats to print.
 * @param e The Enemy pointer with stats to print.
 * @see handleRoom_Enemies()
 * @see win_show()
 */
void init_wins(WINDOW **wins, Enemy* e, Fighter* f) {
	int x, y;
	int lines, cols, border, isEnemy;

	for(int i = 0; i < 3; i++) {
		if (i==0) { //Stat panel
			isEnemy=2;
			y = 2;
			x = 35;
			lines = 13;
			cols = 13;
			border = 0;
			wins[i] = newwin(lines, cols, y, x);
            		wclear(wins[i]);
			wrefresh(wins[i]);
			win_show(wins[i], e, f, isEnemy, border);
			wrefresh(wins[i]);
		}
		if (i==1) { //Fighter panel
			isEnemy=0;
       			lines = 23;
			cols = 32;
			y = 0;
			x = 47;
			wins[i] = newwin(lines, cols, y, x);
            		wclear(wins[i]);
			wrefresh(wins[i]);
			win_show(wins[i], e, f, isEnemy, border);
			wrefresh(wins[i]);
		}
		if (i==2) { //Enemy panel
			lines = 23;
			cols = 36;
			border = 1;
			isEnemy = 1;
			y = 0;
			x = 0;
			wins[i] = newwin(lines, cols, y, x);
            		wclear(wins[i]);
			wrefresh(wins[i]);
			win_show(wins[i], e, f, isEnemy, border);
			wrefresh(wins[i]);
		}
	}
}

/**
 * Takes a WINDOW , a Fighter and a Enemy pointers and prints the appropriate content to it based on isEnemy integer passed.
 * @see Fighter
 * @see Enemy
 * @param win The WINDOW pointer to print to.
 * @param f The Fighter pointer with stats to print.
 * @param e The Enemy pointer with stats to print.
 * @param isEnemy The integer indicating if the current window is for the enemy stats, the fighter stats or the legend panel.
 * @param border The integer indicating if the current window should have a horizontal line bordering the label text.
 * @see init_wins()
 * @see print_in_panel()
 */
void win_show(WINDOW *win, Enemy* e, Fighter* f, int isEnemy, int border)
{	int startx, starty, height, width;

	getbegyx(win, starty, startx);
	getmaxyx(win, height, width);

	log_tag("debug_log.txt","[DEBUG]","win_show():  Begin y (%i) height (%i)", starty, height);
	log_tag("debug_log.txt","[DEBUG]","win_show():  Begin x (%i) width (%i)", startx, width);

	box(win, 0, 0);
	if (border) mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, width - 2);
	if (border) mvwaddch(win, 2, width - 1, ACS_RTEE);

	//Print label
	print_in_panel(win, 1, 0, width, e, f, isEnemy);
}

/**
 * Takes a WINDOW , a Fighter and a Enemy pointers and prints the appropriate content to it based on isEnemy integer passed.
 * Additional parameters set coordinates for the output.
 * @see Fighter
 * @see Enemy
 * @param win The WINDOW pointer to print to.
 * @param f The Fighter pointer with stats to print.
 * @param e The Enemy pointer with stats to print.
 * @param isEnemy The integer indicating if the current window is for the enemy stats, the fighter stats or the legend panel.
 * @param starty The integer indicating starting y coordinate.
 * @param startx The integer indicating starting x coordinate.
 * @param width The integer indicating panel width.
 * @see init_wins()
 * @see print_in_panel()
 */
void print_in_panel(WINDOW *win, int starty, int startx, int width, Enemy* e, Fighter* f, int isEnemy) {

	int x, y;
	float temp;

	if (win == NULL) {
		log_tag("debug_log.txt","[CURSES]","win was NULL in print_in_panel().");
		exit(EXIT_FAILURE);
	}
	getyx(win, y, x);
	if(startx != 0) {
		x = startx;
	}
	if (starty != 0) {
		y = starty;
	}
	if(width == 0) {
		width = 80;
	}

	if (isEnemy == 1) { //Enemy panel

		int xShift = 28;

		int resetColor = 0;

		int l = strlen(stringFromEClass(e->class));
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y = 1;
		if (e->beast) {
			wattron(win,COLOR_PAIR(8));
		} else {
			wattron(win,COLOR_PAIR(1));
		}
		mvwprintw(win, y, x, "%s",stringFromEClass(e->class));
		if (e->beast) {
			wattroff(win,COLOR_PAIR(8));
		} else {
			wattroff(win,COLOR_PAIR(1));
		}

		temp = (width - (e->hp/10)) / 2 ;
		x = startx + xShift; //(int) temp;
		y += 4;
		if ( (e->hp / (e->totalhp / 1.0)) <= 0.25) {
			wattron(win,COLOR_PAIR(1));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", e->hp, e->totalhp);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(1));
			resetColor = 0;
		};

		temp = (width - (e->energy/10) - (e->totalenergy/10)) / 2 ;
		x = startx + xShift; //(int) temp;
		y += 1;
		if ( (e->energy / (e->totalenergy / 1.0)) <= 0.25) {
			wattron(win,COLOR_PAIR(1));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", e->energy, e->totalenergy);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(1));
			resetColor = 0;
		};

		temp = (width - (e->stamina/10) - (e->totalstamina/10)) / 2 ;
		x = startx + xShift; //(int) temp;
		y += 1;
		if ( (e->stamina / (e->totalstamina / 1.0)) <= 0.25) {
			wattron(win,COLOR_PAIR(1));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", e->stamina, e->totalstamina);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(1));
			resetColor = 0;
		};

		temp = (width - (e->atk/10)) / 2 ;
		x = startx + xShift;
		y += 1;
		mvwprintw(win, y, x, "%i", e->atk);

		temp = (width - (e->def/10)) / 2 ;
		x = startx + xShift;
		y += 1;
		mvwprintw(win, y, x, "%i", e->def);

		temp = (width - (e->vel/10)) / 2 ;
		x = startx + xShift;
		y += 1;
		mvwprintw(win, y, x, "%i", e->vel);

		temp = (width - (e->level/10)) / 2 ;
		x = startx + xShift;
		y += 1;
		mvwprintw(win, y, x, "%i", e->level);

		int xp = getEnemyXpGain(e);
		temp = (width - (xp/10)) / 2 ;
		x = startx + xShift;
		y += 1;
		mvwprintw(win, y, x, "%i", xp);

		l = strlen(stringFromStatus(e->status));
		temp = (width - l) / 2 ;
		x = startx + xShift;
		y += 1;
		if (e->status != Normal) {
			wattron(win, COLOR_PAIR(2));
		}
		mvwprintw(win, y, x, "%s", stringFromStatus(e->status));
		if (e->status != Normal) {
			wattroff(win, COLOR_PAIR(2));
		}
	} else if (isEnemy == 0) { //Fighter panel


		int resetColor = 0;

		int l = strlen(f->name);
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y = 1;
		wattron(win,COLOR_PAIR(2));
		mvwprintw(win, y, x, "%s",f->name);
		wattroff(win,COLOR_PAIR(2));

		/*
		l = strlen(stringFromClass(f->class));
		temp = (width - l) / 2;
		x = startx + (int) temp;
		y += 2;
		mvwprintw(win, y, x, "%s",stringFromClass(f->class));
		*/

		temp = (width - (f->hp/10) ) / 2 ;
		x = startx + 2;
		y += 4;
		if ( (f->hp / (f->totalhp / 1.0)) <= 0.25) {
			wattron(win,COLOR_PAIR(1));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", f->hp, f->totalhp);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(1));
			resetColor = 0;
		};

		temp = (width - (f->energy/10) - (f->totalenergy/10)) / 2 ;
		x = startx + 2;
		y += 1;
		if ( (f->energy / (f->totalenergy / 1.0)) <= 0.25) {
			wattron(win,COLOR_PAIR(1));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", f->energy, f->totalenergy);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(1));
			resetColor = 0;
		};

		if (f->equipboost_enr != 0) {
			if (f->equipboost_enr > 0) {
				wattron(win,COLOR_PAIR(2));
				mvwprintw(win, y, x + 5, "(+%i)", f->equipboost_enr);
				wattroff(win,COLOR_PAIR(2));
			} else {
				wattron(win,COLOR_PAIR(1));
				mvwprintw(win, y, x + 5 , "(-%i)", f->equipboost_enr);
				wattroff(win,COLOR_PAIR(1));
			};
		}

		temp = (width - (f->stamina/10) - (f->totalstamina/10)) / 2 ;
		x = startx + 2;
		y += 1;
		if ( (f->stamina / (f->totalstamina / 1.0)) <= 0.25) {
			wattron(win,COLOR_PAIR(1));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", f->stamina, f->totalstamina);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(1));
			resetColor = 0;
		};

		//TODO
		//equipboost_sta?

		temp = (width - (f->atk/10)) / 2 ;
		x = startx + 2;
		y += 1;
		mvwprintw(win, y, x, "%i", f->atk);

		if (f->equipboost_atk != 0) {
			if (f->equipboost_atk > 0) {
				wattron(win,COLOR_PAIR(2));
				mvwprintw(win, y, x + 3, "(+%i)", f->equipboost_atk);
				wattroff(win,COLOR_PAIR(2));
			} else {
				wattron(win,COLOR_PAIR(1));
				mvwprintw(win, y, x + 3 , "(-%i)", f->equipboost_atk);
				wattroff(win,COLOR_PAIR(1));
			};
		}

		temp = (width - (f->def/10)) / 2 ;
		x = startx + 2;
		y += 1;
		mvwprintw(win, y, x, "%i", f->def);

		if (f->equipboost_def != 0) {
			if (f->equipboost_def > 0) {
				wattron(win,COLOR_PAIR(2));
				mvwprintw(win, y, x + 3, "(+%i)", f->equipboost_def);
				wattroff(win,COLOR_PAIR(2));
			} else {
				wattron(win,COLOR_PAIR(1));
				mvwprintw(win, y, x + 3 , "(-%i)", f->equipboost_def);
				wattroff(win,COLOR_PAIR(1));
			};
		}

		temp = (width - (f->vel/10)) / 2 ;
		x = startx + 2;
		y += 1;
		mvwprintw(win, y, x, "%i", f->vel);

		if (f->equipboost_vel != 0) {
			if (f->equipboost_vel > 0) {
				wattron(win,COLOR_PAIR(2));
				mvwprintw(win, y, x + 3, "(+%i)", f->equipboost_vel);
				wattroff(win,COLOR_PAIR(2));
			} else {
				wattron(win,COLOR_PAIR(1));
				mvwprintw(win, y, x + 3 , "(-%i)", f->equipboost_vel);
				wattroff(win,COLOR_PAIR(1));
			};
		}

		temp = (width - (f->level/10)) / 2 ;
		x = startx + 2;
		y += 1;
		mvwprintw(win, y, x, "%i", f->level);

		temp = (width - (f->currentlevelxp/10) - (f->totallevelxp/10)) / 2 ;
		x = startx + 2;
		y += 1;
		if ( (f->currentlevelxp / (f->totallevelxp / 1.0)) >= 0.75) {
			wattron(win,COLOR_PAIR(2));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", f->currentlevelxp, f->totallevelxp);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(2));
			resetColor = 0;
		};

		l = strlen(stringFromStatus(f->status));
		temp = (width - l) / 2 ;
		x = startx + 2;
		y += 1;
		if (f->status != Normal) {
			wattron(win, COLOR_PAIR(1));
		}
		mvwprintw(win, y, x, "%s", stringFromStatus(f->status));
		if (f->status != Normal) {
			wattroff(win, COLOR_PAIR(1));
		}

	} else if (isEnemy == 2) { //Stat panel

		int l = strlen("Stat");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y = 1;
		wattron(win,COLOR_PAIR(4));
		mvwprintw(win, y, x, "Stat");
		wattroff(win,COLOR_PAIR(4));

		l = strlen("Hp");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y = 3;
		mvwprintw(win, y, x, "Hp");

		l = strlen("Energy");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Energy");

		l = strlen("Stamina");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Stamina");

		l = strlen("Atk");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Atk");

		l = strlen("Def");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Def");

		l = strlen("Vel");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Vel");

		l = strlen("Lvl");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Lvl");

		l = strlen("Xp");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Xp");

		l = strlen("Status");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Status");
	}

	refresh();
}

/**
 * Takes a Fighter and a Boss pointers and prints most of their values formatted.
 * For the Fighter, the equipboost values are also displayed.
 * @see Fighter
 * @see Boss
 * @see stringFromClass()
 * @see stringFromBossClass()
 * @see stringFromStatus()
 * @see getBossXpGain()
 * @param wins The WINDOW array to print fighter, enemy an stat legend to.
 * @param f The Fighter pointer with stats to print.
 * @param b The Boss pointer with stats to print.
 */
void printBossBattleStats(WINDOW* wins[3], Fighter* f, Boss* b) {

	for (int i=0; i < 3; i++) {
		if ( wins[i] == NULL ) {
			log_tag("debug_log.txt","[ERROR]","printBossBattleStats():  wins[%i] was NULL",i);
			exit(EXIT_FAILURE);
		}
	}

	PANEL  *my_panels[3];
	PANEL  *top;

	/* Attach a panel to each window */ 	/* Order is bottom up */
	my_panels[0] = new_panel(wins[0]); 	/* Push 0, order: stdscr-0 */
	my_panels[1] = new_panel(wins[1]); 	/* Push 1, order: stdscr-0-1 */
	my_panels[2] = new_panel(wins[2]); 	/* Push 2, order: stdscr-0-1-2 */


	/* Set up the user pointers to the next panel */
	set_panel_userptr(my_panels[0], my_panels[1]);
	set_panel_userptr(my_panels[1], my_panels[2]);
	set_panel_userptr(my_panels[2], my_panels[0]);

	/* Update the stacking order. 3nd panel will be the last one*/
	update_panels();

	/* Show it on the screen */
	//attron(COLOR_PAIR(4));
	//mvprintw(LINES - 2, 0, "MENU PROMPT (F1 to Exit)");
	//attroff(COLOR_PAIR(4));
	doupdate();

	top = my_panels[2];
	top = (PANEL *)panel_userptr(top);
	top_panel(top);
	top = (PANEL *)panel_userptr(top);
	top_panel(top);
	update_panels();
	doupdate();

}

/**
 * Takes an array pointer of WINDOW pointers, a Fighter and a Boss pointers and initialises the passed windows.
 * @see Fighter
 * @see Boss
 * @param wins The WINDOW array pointer to initialise.
 * @param f The Fighter pointer with stats to print.
 * @param b The Boss pointer with stats to print.
 * @see handleRoom_Boss()
 * @see boss_win_show()
 */
void init_boss_wins(WINDOW **wins, Boss* b, Fighter* f)
{
	int x, y, i = 0;
	int lines, cols, border, isBoss;

       	lines = 23;
	cols = 36;
	border = 1;
	isBoss = 1;

	y = 0;
	x = 0;

	for(i = 0; i < 3; i++) {
		if (i==2) { //Stat panel
			isBoss=2;
			y += 2;
			x = 35;
			lines = 13;
			cols = 13;
			border = 0;
			wins[i] = newwin(lines, cols, y, x);
			wclear(wins[i]);
			wrefresh(wins[i]);
			boss_win_show(wins[i], b, f, isBoss, border);
			wrefresh(wins[i]);
		}
		if (i==1) { //Fighter panel
			isBoss=0;
       			lines = 23;
			cols = 32;
			y = 0;
			x = 47;
			wins[i] = newwin(lines, cols, y, x);
			wclear(wins[i]);
			wrefresh(wins[i]);
			boss_win_show(wins[i], b, f, isBoss, border);
			wrefresh(wins[i]);
		}
		if (i==0) { //Boss panel
			wins[i] = newwin(lines, cols, y, x);
			wclear(wins[i]);
			wrefresh(wins[i]);
			boss_win_show(wins[i], b, f, isBoss, border);
		        wrefresh(wins[i]);
		}
	}
}

/**
 * Takes a WINDOW , a Fighter and a Boss pointers and prints the appropriate content to it based on isBoss integer passed.
 * @see Fighter
 * @see Boss
 * @param win The WINDOW pointer to print to.
 * @param f The Fighter pointer with stats to print.
 * @param b The Boss pointer with stats to print.
 * @param isBoss The integer indicating if the current window is for the boss stats, the fighter stats or the legend panel.
 * @param border The integer indicating if the current window should have a horizontal line bordering the label text.
 * @see init_boss_wins()
 * @see boss_print_in_panel()
 */
void boss_win_show(WINDOW *win, Boss* b, Fighter* f, int isBoss, int border)
{
	log_tag("debug_log.txt","[DEBUG]","Doing boss_win_show().");

        int startx, starty, height, width;

	getbegyx(win, starty, startx);
	getmaxyx(win, height, width);
	log_tag("debug_log.txt","[DEBUG]","boss_win_show():  Begin y (%i) height (%i)", starty, height);
	log_tag("debug_log.txt","[DEBUG]","boss_win_show():  Begin x (%i) width (%i)", startx, width);

	box(win, 0, 0);
	if (border) mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, width - 2);
	if (border) mvwaddch(win, 2, width - 1, ACS_RTEE);

	//Print label
	boss_print_in_panel(win, 1, 0, width, b, f, isBoss);

	log_tag("debug_log.txt","[DEBUG]","Done boss_win_show().");
}

/**
 * Takes a WINDOW , a Fighter and a Boss pointers and prints the appropriate content to it based on isBoss integer passed.
 * Additional parameters set coordinates for the output.
 * @see Fighter
 * @see Boss
 * @param win The WINDOW pointer to print to.
 * @param f The Fighter pointer with stats to print.
 * @param b The Boss pointer with stats to print.
 * @param isBoss The integer indicating if the current window is for the boss stats, the fighter stats or the legend panel.
 * @param starty The integer indicating starting y coordinate.
 * @param startx The integer indicating starting x coordinate.
 * @param width The integer indicating panel width.
 * @see init_boss_wins()
 * @see boss_print_in_panel()
 */
void boss_print_in_panel(WINDOW *win, int starty, int startx, int width, Boss* b, Fighter* f, int isBoss)
{	int x, y;
	float temp;

	if(win == NULL) {
		log_tag("debug_log.txt","[CURSES]","win was NULL in boss_print_in_panel().");
		exit(EXIT_FAILURE);
	}
	getyx(win, y, x);
	if(startx != 0) {
		x = startx;
	}
	if (starty != 0) {
		y = starty;
	}
	if(width == 0) {
		width = 80;
	}

	if (isBoss == 1) { //Boss panel

		int resetColor = 0;
		int xShift = 28;

		int l = strlen(stringFromBossClass(b->class));
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y = 1;
		if (b->beast) {
			wattron(win,COLOR_PAIR(8));
		} else {
			wattron(win,COLOR_PAIR(1));
		}
		mvwprintw(win, y, x, "%s",stringFromBossClass(b->class));
		if (b->beast) {
			wattroff(win,COLOR_PAIR(8));
		} else {
			wattroff(win,COLOR_PAIR(1));
		}

		temp = (width - (b->hp/10)) / 2 ;
		x = startx + xShift; //(int) temp;
		y += 4;
		if ( (b->hp / (b->totalhp / 1.0)) <= 0.25) {
			wattron(win,COLOR_PAIR(1));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", b->hp, b->totalhp);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(1));
			resetColor = 0;
		};

		temp = (width - (b->energy/10) - (b->totalenergy/10)) / 2 ;
		x = startx + xShift; //(int) temp;
		y += 1;
		if ( (b->energy / (b->totalenergy / 1.0)) <= 0.25) {
			wattron(win,COLOR_PAIR(1));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", b->energy, b->totalenergy);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(1));
			resetColor = 0;
		};

		temp = (width - (b->stamina/10) - (b->totalstamina/10)) / 2 ;
		x = startx + xShift; //(int) temp;
		y += 1;
		if ( (b->stamina / (b->totalstamina / 1.0)) <= 0.25) {
			wattron(win,COLOR_PAIR(1));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", b->stamina, b->totalstamina);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(1));
			resetColor = 0;
		};

		temp = (width - (b->atk/10)) / 2 ;
		x = startx + xShift;
		y += 1;
		mvwprintw(win, y, x, "%i", b->atk);

		temp = (width - (b->def/10)) / 2 ;
		x = startx + xShift;
		y += 1;
		mvwprintw(win, y, x, "%i", b->def);

		temp = (width - (b->vel/10)) / 2 ;
		x = startx + xShift;
		y += 1;
		mvwprintw(win, y, x, "%i", b->vel);

		temp = (width - (b->level/10)) / 2 ;
		x = startx + xShift;
		y += 1;
		mvwprintw(win, y, x, "%i", b->level);

		int xp = getBossXpGain(b);
		temp = (width - (xp/10)) / 2 ;
		x = startx + xShift;
		y += 1;
		mvwprintw(win, y, x, "%i", xp);

		l = strlen(stringFromStatus(b->status));
		temp = (width - l) / 2 ;
		x = startx + xShift;
		y += 1;
		if (b->status != Normal) {
			wattron(win, COLOR_PAIR(2));
		}
		mvwprintw(win, y, x, "%s", stringFromStatus(b->status));
		if (b->status != Normal) {
			wattroff(win, COLOR_PAIR(2));
		}
	} else if (isBoss == 0) { //Fighter panel

		int resetColor = 0;

		int l = strlen(f->name);
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y = 1;
		wattron(win,COLOR_PAIR(2));
		mvwprintw(win, y, x, "%s",f->name);
		wattroff(win,COLOR_PAIR(2));

		/*
		l = strlen(stringFromClass(f->class));
		temp = (width - l) / 2;
		x = startx + (int) temp;
		y += 2;
		mvwprintw(win, y, x, "%s",stringFromClass(f->class));
		*/

		temp = (width - (f->hp/10) ) / 2 ;
		x = startx + 2;
		y += 4;
		if ( (f->hp / (f->totalhp / 1.0)) <= 0.25) {
			wattron(win,COLOR_PAIR(1));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", f->hp, f->totalhp);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(1));
			resetColor = 0;
		};

		temp = (width - (f->energy/10) - (f->totalenergy/10)) / 2 ;
		x = startx + 2;
		y += 1;
		if ( (f->energy / (f->totalenergy / 1.0)) <= 0.25) {
			wattron(win,COLOR_PAIR(1));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", f->energy, f->totalenergy);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(1));
			resetColor = 0;
		};

		if (f->equipboost_enr != 0) {
			if (f->equipboost_enr > 0) {
				wattron(win,COLOR_PAIR(2));
				mvwprintw(win, y, x + 5, "(+%i)", f->equipboost_enr);
				wattroff(win,COLOR_PAIR(2));
			} else {
				wattron(win,COLOR_PAIR(1));
				mvwprintw(win, y, x + 5 , "(-%i)", f->equipboost_enr);
				wattroff(win,COLOR_PAIR(1));
			};
		}

		temp = (width - (f->stamina/10) - (f->totalstamina/10)) / 2 ;
		x = startx + 2;
		y += 1;
		if ( (f->stamina / (f->totalstamina / 1.0)) <= 0.25) {
			wattron(win,COLOR_PAIR(1));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", f->stamina, f->totalstamina);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(1));
			resetColor = 0;
		};

		temp = (width - (f->atk/10)) / 2 ;
		x = startx + 2;
		y += 1;
		mvwprintw(win, y, x, "%i", f->atk);

		if (f->equipboost_atk != 0) {
			if (f->equipboost_atk > 0) {
				wattron(win,COLOR_PAIR(2));
				mvwprintw(win, y, x + 3, "(+%i)", f->equipboost_atk);
				wattroff(win,COLOR_PAIR(2));
			} else {
				wattron(win,COLOR_PAIR(1));
				mvwprintw(win, y, x + 3 , "(-%i)", f->equipboost_atk);
				wattroff(win,COLOR_PAIR(1));
			};
		}

		temp = (width - (f->def/10)) / 2 ;
		x = startx + 2;
		y += 1;
		mvwprintw(win, y, x, "%i", f->def);

		if (f->equipboost_def != 0) {
			if (f->equipboost_def > 0) {
				wattron(win,COLOR_PAIR(2));
				mvwprintw(win, y, x + 3, "(+%i)", f->equipboost_def);
				wattroff(win,COLOR_PAIR(2));
			} else {
				wattron(win,COLOR_PAIR(1));
				mvwprintw(win, y, x + 3 , "(-%i)", f->equipboost_def);
				wattroff(win,COLOR_PAIR(1));
			};
		}

		temp = (width - (f->vel/10)) / 2 ;
		x = startx + 2;
		y += 1;
		mvwprintw(win, y, x, "%i", f->vel);

		if (f->equipboost_vel != 0) {
			if (f->equipboost_vel > 0) {
				wattron(win,COLOR_PAIR(2));
				mvwprintw(win, y, x + 3, "(+%i)", f->equipboost_vel);
				wattroff(win,COLOR_PAIR(2));
			} else {
				wattron(win,COLOR_PAIR(1));
				mvwprintw(win, y, x + 3 , "(-%i)", f->equipboost_vel);
				wattroff(win,COLOR_PAIR(1));
			};
		}

		temp = (width - (f->level/10)) / 2 ;
		x = startx + 2;
		y += 1;
		mvwprintw(win, y, x, "%i", f->level);

		temp = (width - (f->currentlevelxp/10) - (f->totallevelxp/10)) / 2 ;
		x = startx + 2;
		y += 1;
		if ( (f->currentlevelxp / (f->totallevelxp / 1.0)) >= 0.75) {
			wattron(win,COLOR_PAIR(2));
			resetColor = 1;
		}
		mvwprintw(win, y, x, "%i/%i", f->currentlevelxp, f->totallevelxp);
		if (resetColor) {
			wattroff(win,COLOR_PAIR(2));
			resetColor = 0;
		};

		l = strlen(stringFromStatus(f->status));
		temp = (width - l) / 2 ;
		x = startx + 2;
		y += 1;
		if (f->status != Normal) {
			wattron(win, COLOR_PAIR(1));
		}
		mvwprintw(win, y, x, "%s", stringFromStatus(f->status));
		if (f->status != Normal) {
			wattroff(win, COLOR_PAIR(1));
		}

	} else if (isBoss == 2) { //Stat panel

		int l = strlen("Stat");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y = 1;
		wattron(win,COLOR_PAIR(4));
		mvwprintw(win, y, x, "Stat");
		wattroff(win,COLOR_PAIR(4));

		l = strlen("Hp");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y = 3;
		mvwprintw(win, y, x, "Hp");

		l = strlen("Energy");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Energy");

		l = strlen("Stamina");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Stamina");

		l = strlen("Atk");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Atk");

		l = strlen("Def");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Def");

		l = strlen("Vel");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Vel");

		l = strlen("Lvl");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Lvl");

		l = strlen("Xp");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Xp");

		l = strlen("Status");
		temp = (width - l) / 2 ;
		x = startx + (int) temp;
		y += 1;
		mvwprintw(win, y, x, "Status");
	}

	refresh();
}

/**
 * Takes a WINDOW pointer and prints to it the passed string with the passed color.
 * Additional parameters set coordinates for the output.
 * @param win The WINDOW pointer to print to.
 * @param starty The integer indicating starting y coordinate.
 * @param startx The integer indicating starting x coordinate.
 * @param width The integer indicating panel width.
 * @param string The string to print to the window.
 * @param color The color to print in.
 */
void print_label(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
	float temp;

	if (win == NULL) {
		log_tag("debug_log.txt","[CURSES]","win was NULL in boss_print_in_panel().");
		exit(EXIT_FAILURE);
	}
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}

/**
 * Takes a WINDOW pointer to print notifications to, a fighterStatus value and a string of who's the entity to print the respective status message.
 * @see fighterStatus
 * @param notify_win The pointer to the window to use display_notification() on.
 * @param status The fighterStatus at hand.
 * @param subject A string with name of entity owning the fighterStatus.
 */
void printStatusText(WINDOW *notify_win, fighterStatus status, char* subject) {
	char msg[500];
	switch (status) {
		case Normal: {
			return;
		};
		break;
		case Poison: case Burned: {
			sprintf(msg, "%s is hurt by its %s.",subject, stringFromStatus(status));
			display_notification(notify_win, msg, 500);
		}
		break;
		case Weak: case Strong: {
			sprintf(msg, "%s is feeling %s.",subject, stringFromStatus(status));
			display_notification(notify_win, msg, 500);
		}
		break;
		case Frozen: {
			sprintf(msg, "%s is frozen cold.",subject);
			display_notification(notify_win, msg, 500);
		}
		break;
	}
}

/**
 * Takes a WINDOW pointer to print notifications to, a Fighter pointer value and applies the effect pertaining to its status value.
 * @see Fighter
 * @see fighterStatus
 * @see printStatusText()
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param f The Fighter pointer at hand.
 */
void applyStatus(WINDOW* notify_win, Fighter* f){

	switch(f->status) {
		case Normal: {
				break;
			}
		break;
		case Poison:
			{

			//Account for penicillin perk
			//ATM multiples don't stack
			int penicillin = f->perks[PENICILLIN]->innerValue;
			if (penicillin > 0) {
				return;
			}


			if (f->hp >= 4) {
				f->hp -= 3;
			} else {
				f->hp = 1; //Will this be a problem?
			}
			printStatusText(notify_win, Poison, f->name);
		}
		break;
		case Burned: {
				printStatusText(notify_win, Burned, f->name);
			}
		break;
		case Frozen: {
				printStatusText(notify_win, Frozen, f->name);
			}
		break;
		case Weak:

		break;
		case Strong:

		break;
	}
}

/**
 * Takes a WINDOW pointer to print notifications to, a Enemy pointer value and applies the effect pertaining to its status value.
 * @see Enemy
 * @see fighterStatus
 * @see printStatusText()
 * @see stringFromEClass()
 * @param notify_win The window pointer to call display_notification() on.
 * @param e The Enemy pointer at hand.
 * @see display_notification()
 */
void applyEStatus(WINDOW *notify_win, Enemy* e){

	wattron(notify_win, COLOR_PAIR(2));

	switch(e->status) {
		case Normal:
			{
				break;
			}
		break;
		case Poison: {
			if (e->hp >= 4) {
				e->hp -= 3;
			} else {
				e->hp = 1; //Will this be a problem for kills in the enemy loop?
			}
			printStatusText(notify_win, Poison,stringFromEClass(e->class));
		}
		break;
		case Burned: {
			if (e->hp >= 5) {
				e->hp -= 4;
			} else {
				e->hp = 1; //Will this be a problem for kills in the enemy loop?
			}

			if (e->atk >= 3) {
				e->atk -= 3;
			} else {
				e->atk = 1;
			}
			printStatusText(notify_win, Burned,stringFromEClass(e->class));
		}
		break;
		case Frozen: {
			if (e->vel >= 3) {
				e->vel -= 1;
			} else {
				e->vel = 1; //Will this be a problem for kills in the enemy loop?
			}
			printStatusText(notify_win, Frozen,stringFromEClass(e->class));
		}

		break;
		case Weak:

		break;
		case Strong:

		break;
	}

	wattroff(notify_win, COLOR_PAIR(2));
}

/**
 * Takes a WINDOW pointer to print notifications to, a Boss pointer value and applies the effect pertaining to its status value.
 * @see Boss
 * @see fighterStatus
 * @see printStatusText()
 * @see stringFromBossClass()
 * @param notify_win The window pointer to call disaply_notification() on.
 * @param b The Boss pointer at hand.
 */
void applyBStatus(WINDOW *notify_win, Boss* b){

	wattron(notify_win, COLOR_PAIR(2));

	switch(b->status) {
		case Normal:
			{
				break;
			}
		break;
		case Poison: {
			if (b->hp >= 4) {
				b->hp -= 3;
			} else {
				b->hp = 1; //Will this be a problem for kills in the enemy loop?
			}
			printStatusText(notify_win, Poison,stringFromBossClass(b->class));
		}
		break;
		case Burned: {
			if (b->hp >= 5) {
				b->hp -= 4;
			} else {
				b->hp = 1; //Will this be a problem for kills in the enemy loop?
			}

			if (b->atk >= 3) {
				b->atk -= 3;
			} else {
				b->atk = 1;
			}
			printStatusText(notify_win, Burned,stringFromBossClass(b->class));
		}
		break;
		case Frozen: {
			if (b->vel >= 3) {
				b->vel -= 1;
			} else {
				b->vel = 1; //Will this be a problem for kills in the enemy loop?
			}
			printStatusText(notify_win, Frozen,stringFromBossClass(b->class));
		}

		break;
		case Weak:

		break;
		case Strong:

		break;
	}

	wattroff(notify_win, COLOR_PAIR(2));
}

/**
 * Takes a WINDOW pointer and prints the passed text to it ith wprintw(), before sleeping for the specified amount of milliseconds.
 * @see handleRoom_Enemies()
 * @see handleRoom_Boss()
 * @param w The WINDOW pointer to print to.
 * @param text The contents of the notification.
 * @param time The display time in milliseconds
 */
void display_notification(WINDOW* w, char* text, int time) {
	wprintw(w,"\n  %s", text);
        wrefresh(w);
	//refresh();
	napms(time);
}

/**
 * Takes a Enemy pointer and prepares its sprite field by copying it line by line from enemies_sprites, defined in sprites.h header.
 * @see Enemy
 * @see initEnemyStats
 * @see enemies_sprites
 * @param e The Enemy pointer whose sprite field will be initialised.
 */
void setEnemySprite(Enemy* e) {
	if (e->class < ENEMYCLASSESMAX+1) {
		for (int i = 0; i < 8; i++) {
			strcpy(e->sprite[i],enemies_sprites[e->class][i]);
		}
	} else {
		fprintf(stderr,"[ERROR]    Unexpected enemyClass in setEnemySprite().\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * Takes a Boss pointer and prepares its sprite field by copying it line by line from bosses_sprites, defined in sprites.h header.
 * @see Boss
 * @see initBossStats
 * @see bosses_sprites
 * @param b The Boss pointer whose sprite field will be initialised.
 */
void setBossSprite(Boss* b) {
	if (b->class < BOSSCLASSESMAX+1) {
		for (int i = 0; i < 8; i++) {
			strcpy(b->sprite[i],bosses_sprites[b->class][i]);
		}
	} else {
		fprintf(stderr,"[ERROR]    Unexpected bossclass in setBossSprite().\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * Takes a Fighter pointer and prepares its sprite field by copying it line by line.
 * @see Fighter
 * @see initPlayerStats
 * @param f The Fighter pointer whose sprite field will be initialised.
 */
void setFighterSprite(Fighter* f) {
	//TODO: this sprite is also present in misc_sprites, defined in sprites.h
	//Should follow suit as the other setter functions and grab from there, not from this local copy.
	switch(f->class) {
		default: {
				strcpy(f->sprite[0],"I    yy    I");
				strcpy(f->sprite[1],"I    yy    I");
				strcpy(f->sprite[2],"I   yyyy y I");
				strcpy(f->sprite[3],"I yy yy y  I");
				strcpy(f->sprite[4],"I    yy    I");
				strcpy(f->sprite[5],"I   y  y   I");
				strcpy(f->sprite[6],"I   y  y   I");
				strcpy(f->sprite[7],"I    y y   I");
		}
		break;

	};
}

/**
 * Takes a Consumable pointer and prepares its sprite field by copying it line by line from consumables_sprites, defined in sprites.h header.
 * @see Consumable
 * @see initPlayerStats
 * @see consumables_sprites
 * @param c The Consumable pointer whose sprite field will be initialised.
 */
void setConsumableSprite(Consumable* c) {
	if (c->class < CONSUMABLESMAX+1) {
		for (int i = 0; i < 8; i++) {
			strcpy(c->sprite[i],consumables_sprites[c->class][i]);
		}
	} else {
		fprintf(stderr,"[ERROR]    Unexpected consumableClass in setConsumableSprite().\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * Takes a Artifact pointer and prepares its sprite field by copying it line by line from artifacts_sprites, defined in sprites.h header.
 * @see Artifact
 * @see gameloop()
 * @see artifacts_sprites
 * @param a The Artifact pointer whose sprite field will be initialised.
 */
void setArtifactSprite(Artifact* a) {
	if (a->class < ARTIFACTSMAX+1) {
		for (int i = 0; i < 8; i++) {
			strcpy(a->sprite[i],artifacts_sprites[a->class][i]);
		}
	} else {
		fprintf(stderr,"[ERROR]    Unexpected artifactClass in setArtifactSprite().\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * Takes a Equip pointer and prepares its sprite field by copying it line by line from equips_sprites, defined in sprites.h header.
 * @see Equip
 * @see dropEquip
 * @see equips_sprites
 * @param e The Equip pointer whose sprite field will be initialised.
 */
void setEquipSprite(Equip* e) {
	if (e->class < EQUIPSMAX+1) {
		for (int i = 0; i < 8; i++) {
			strcpy(e->sprite[i],equips_sprites[e->class][i]);
		}
	} else {
		fprintf(stderr,"[ERROR]    Unexpected equipClass in setEquipSprite().\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * Takes a Equipslot pointer and prepares its sprite field by copying it line by line from equipzones_sprites, defined in sprites.h header.
 * @see Equipslot
 * @see initEquipSlots()
 * @see equipzones_sprites
 * @param s The Equipslot pointer whose sprite field will be initialised.
 */
void setEquipslotSprite(Equipslot* s) {
	if (s->type < EQUIPZONES+1) {
		for (int i = 0; i < 8; i++) {
			strcpy(s->sprite[i],equipzones_sprites[s->type][i]);
		}
	} else {
		fprintf(stderr,"[ERROR]    Unexpected Equipslot type in setEquipslotSprite().\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * Takes a Chest pointer and prepares its sprite field by copying it line by line.
 * @see Chest
 * @see initChest
 * @param c The Chest pointer whose sprite field will be initialised.
 */
void setChestSprite(Chest* c) {

	switch(c->class) {
		case CHEST_BASE: {
				strcpy(c->sprite[0],"            ");
				strcpy(c->sprite[1],"   bbbbbbb  ");
				strcpy(c->sprite[2],"  bbcccccbb ");
				strcpy(c->sprite[3],"  bcccccccb ");
				strcpy(c->sprite[4],"  bbbbbbbbb ");
				strcpy(c->sprite[5],"  bcccrcccb ");
				strcpy(c->sprite[6],"  bcccccccb ");
				strcpy(c->sprite[7],"  bbbbbbbbb ");
		}
		break;
		case CHEST_BEAST: {
				strcpy(c->sprite[0],"            ");
				strcpy(c->sprite[1],"   rrrrrrr  ");
				strcpy(c->sprite[2],"  rryyyyyrr ");
				strcpy(c->sprite[3],"  ryyyyyyyr ");
				strcpy(c->sprite[4],"  rrrrrrrrr ");
				strcpy(c->sprite[5],"  ryyymyyyr ");
				strcpy(c->sprite[6],"  ryyyyyyyr ");
				strcpy(c->sprite[7],"  rrrrrrrrr ");
		}
		break;
		default: {
				strcpy(c->sprite[0],"I    yy    I");
				strcpy(c->sprite[1],"I    yy    I");
				strcpy(c->sprite[2],"I yyyyyyyy I");
				strcpy(c->sprite[3],"I    yy    I");
				strcpy(c->sprite[4],"I    yy    I");
				strcpy(c->sprite[5],"I   yyyy   I");
				strcpy(c->sprite[6],"I   y  y   I");
				strcpy(c->sprite[7],"I    y y   I");
		}
		break;

	};

}

/**
 * Takes a WINDOW, a MENU and a Fighter pointers and updates the window contents with the selected Consumable from the fighter's bag, in the passed MENU pointer.
 * @see Consumable
 * @see consumableClass
 * @see CONSUMABLESMAX
 * @see handleConsumables
 * @param w The window pointer to print into.
 * @param my_menu The menu to get the selection from.
 * @param f The fighter pointer to get the bag from.
 */
void updateSelectedConsumableW(WINDOW* w, MENU* my_menu, Fighter* f) {

	box(w, 0, 0);
	ITEM *cur;
	cur = current_item(my_menu);

	int i=0;
	int c;
	for (int j=0; j < CONSUMABLESMAX+1; j++) {
		if ((c = strcmp(consumablestrings[j],item_name(cur))) == 0) {
			i = j;
		}
	};
	char s[80];
	Consumable* selected = (Consumable*) f->consumablesBag[i];

	int x,y;
	for (int i = 0; i < 8; i++) {

		strcpy(s,selected->sprite[i]);
		for (int j = 0; j < 14; j++) {
			x = 2 + 1 + j;
			y = 4 + i;
			print_encoded_char(w, y, x, s[j]);
		}
	}
	//mvwprintw(w,2, 20, "Selected %s",item_name(cur));
	wattron(w,COLOR_PAIR(6));
	mvwprintw(w,2, 2, "%s",selected->desc);
	wattroff(w,COLOR_PAIR(6));
	mvwprintw(w,15, 2, "Quantity: x%i",selected->qty);
	wrefresh(w);

}

/**
 * Takes a WINDOW, a MENU and a Fighter pointers and updates the window contents with the selected Artifact from the Fighter's artifactsBag, in the passed MENU pointer.
 * @see Artifact
 * @see artifactClass
 * @see ARTIFACTSMAX
 * @see handleArtifacts
 * @param w The window pointer to print into.
 * @param my_menu The menu pointer to get the selection from.
 * @param f The fighter pointer holding the artifactsBag.
 */
void updateSelectedArtifactW(WINDOW* w, MENU* my_menu, Fighter* f) {

	box(w, 0, 0);
	ITEM *cur;
	cur = current_item(my_menu);

	int i=0;
	int c;
	for (int j=0; j < ARTIFACTSMAX+1; j++) {
		if ((c = strcmp(artifactstrings[j],item_name(cur))) == 0) {
			i = j;
		}
	};
	char s[80];
	Artifact selected = *(f->artifactsBag[i]);

	int x,y;
	for (int i = 0; i < 8; i++) {

		strcpy(s,selected.sprite[i]);
		for (int j = 0; j < 14; j++) {
			x = 2 + 1 + j;
			y = 4 + i;
			print_encoded_char(w, y, x, s[j]);
		}
	}
	//mvwprintw(w,2, 20, "Selected %s",item_name(cur));
	wattron(w,COLOR_PAIR(6));
	mvwprintw(w,2, 2, "%s",selected.desc);
	wattroff(w,COLOR_PAIR(6));
	mvwprintw(w,15, 2, "Quantity: x%i",selected.qty);
	wrefresh(w);

}

/**
 * Takes a WINDOW and MENU pointers and updates the window contents with the passed Equip.
 * @see Equip
 * @see equipClass
 * @see EQUIPSMAX
 * @see handleEquips
 * @see displayEquipbagMenu()
 * @param w The window pointer to point into.
 * @param my_menu The menu pointer to get the selection from.
 * @param f The fighter pointer to get the equipslots from.
 * @param selected The equip pointer to print.
 */
void updateSelectedEquipW(WINDOW* w, MENU* my_menu, Equip* selected,Fighter* f) {

	box(w, 0, 0);
	//ITEM *cur;
	//cur = current_item(my_menu);

	char s[80];

	int x,y;
	for (int i = 0; i < 8; i++) {

		strcpy(s,selected->sprite[i]);
		for (int j = 0; j < 14; j++) {
			x = 2 + 1 + j;
			y = 4 + i;
			print_encoded_char(w, y, x, s[j]);
		}
	}

	wattron(w,COLOR_PAIR(6));
	mvwprintw(w,2, 2, "%s",selected->desc);
	wattroff(w,COLOR_PAIR(6));
	Equipslot * slot = (Equipslot *) f->equipslots[selected->type];

	if (slot->active) {
		Equip* current = slot->item;
		mvwprintw(w,3, 28, "Current:");
		if (current->atk > 0) mvwprintw(w,4, 28, "%i", current->atk);
		if (current->def > 0) mvwprintw(w,5, 28, "%i", current->def);
		if (current->vel > 0) mvwprintw(w,6, 28, "%i", current->vel);
		if (current->enr > 0) mvwprintw(w,7, 28, "%i", current->enr);
	}

	wattron(w,COLOR_PAIR(2));
	if (selected->atk > 0) {
		mvwprintw(w,4, 18, "Atk: %i",selected->atk);
	}
	if (selected->def > 0) {
		mvwprintw(w,5, 18, "Def: %i",selected->def);
	}
	if (selected->vel > 0) {
		mvwprintw(w,6, 18, "Vel: %i",selected->atk);
	}
	if (selected->enr > 0) {
		mvwprintw(w,7, 18, "Enr: %i",selected->enr);
	}
	wattroff(w,COLOR_PAIR(2));
	int color = -1;
	switch (selected->qual) {
		case Bad: {
			wattron(w,COLOR_PAIR(1));
			color = 1;
		}
		break;
		case Average: {
			wattron(w,COLOR_PAIR(6));
			color = 6;
		}
		break;
		case Good: {
			wattron(w,COLOR_PAIR(4));
			color = 4;
		}
		break;
		default: {

		}
		break;
	};
	mvwprintw(w, 9, 18, "Quality:    %s",stringFromQuality(selected->qual));
	wattroff(w,COLOR_PAIR(color));

	wattron(w,COLOR_PAIR(2));
	mvwprintw(w, 10, 18, "Level:    %i",selected->level);
	wattroff(w,COLOR_PAIR(2));

	wattron(w,COLOR_PAIR(4));
	mvwprintw(w, 11, 18, "Perks:    %i",selected->perksCount);
	int count = selected->perksCount;
	for (int i = 0; i < count; i++) {
		Perk* p = selected->perks[i];

		mvwprintw(w, 11 + i, 18, " x%i %s", p->innerValue, nameStringFromPerk(p->class));
	};
	wattroff(w,COLOR_PAIR(4));
	wrefresh(w);

}

/**
 * Takes a WINDOW and Fighter pointers and updates the window contents with the current Equipzone information.
 * @see Equipslot
 * @see Equipzone
 * @see EQUIPZONES
 * @see handleEquips
 * @param w The window pointer to print to.
 * @param f The fighter pointer to get the equipslots from.
 */
void updateEquipslotsWin(WINDOW* w, Fighter* f) {

	box(w, 0, 0);
	Equipslot** slots = (Equipslot **) f->equipslots;
	int y = 2;
	int x = 2;

	for (int k=0; k < EQUIPZONES+1; k++) {
		char s[80];
		Equipslot* selected = slots[k];

		for (int i = 0; i < 8; i++) {

			y = 1 + i;

			strcpy(s,selected->sprite[i]);
			for (int j = 0; j < 14; j++) {
				print_encoded_char(w, y, x + j, s[j]);
			}
		}

		if (selected->active) {
			wattron(w,COLOR_PAIR(2));
			mvwprintw(w, y + 1, x, "    %s", stringFromEquipzones(selected->type));
			wattroff(w,COLOR_PAIR(2));
			int color = -1;
			switch (selected->item->qual) {
				case Bad: {
					wattron(w,COLOR_PAIR(1));
					color = 1;
				}
				break;
				case Average: {
					wattron(w,COLOR_PAIR(6));
					color = 6;
				}
				break;
				case Good: {
					wattron(w,COLOR_PAIR(4));
					color = 4;
				}
				break;
				default: {

				}
				break;
			};
			mvwprintw(w, y +11, x, "    %s", selected->item->name);
			wattroff(w,color);
			mvwprintw(w, y +12, x, "    L%i", selected->item->level);
				char item_sprite_line[80];
			for (int rows = 0; rows < 8; rows++) {

				y = 10 + rows;

				Equip* item = selected->item;
				strcpy(item_sprite_line,item->sprite[rows]);
				for (int cols = 0; cols < 14; cols++) {
					print_encoded_char(w, y, x + cols, item_sprite_line[cols]);
				}
			}
		} else {
			mvwprintw(w, y +3, x, "EMPTY");
			wattron(w,COLOR_PAIR(6));
			mvwprintw(w, y +2, x, "%s", stringFromEquipzones(selected->type));
			wattroff(w,COLOR_PAIR(6));
		}
		x += 13;

	}
	wrefresh(w);
}

/**
 * Takes a WINDOW and MENU pointers and updates the window contents with the selected Special in the passed MENU pointer.
 * @see Special
 * @see Specialslot
 * @see SPECIALSMAX
 * @see handleSpecials
 * @param w The window pointer to print into.
 * @param my_menu The menu pointer to get the selection from.
 * @param f The Fighter pointer regarding the specials.
 */
void updateSelectedSpecialW(WINDOW* w, MENU* my_menu, Fighter* f) {

	box(w, 0, 0);
	ITEM *cur;
	cur = current_item(my_menu);

	int i=0;
	int c;
	for (int j=0; j < SPECIALSMAX+1; j++) {
		if ((c = strcmp(nameStringFromSpecial(f->class, i),item_name(cur))) == 0) {
			i = j;
		}
	};
	Specialslot* selected = f->specials[i];

	int x = 4;
	int y = 2;



	//mvwprintw(w,2, 20, "Selected %s",item_name(cur));
	wattron(w,COLOR_PAIR(6));
	mvwprintw(w, y, x, "%s",descStringFromSpecial(f->class,selected->move));
	mvwprintw(w, y+2, x, "Energy Cost:    %i",selected->cost);
	wattroff(w,COLOR_PAIR(6));
	wrefresh(w);
}

/**
 * Takes a WINDOW and Equip pointers and updates the window contents with the Equip stats.
 * @see Equip
 * @see Shop
 * @see handleRoom_Shop()
 * @param w The window pointer to print into.
 * @param e The Equip to be printed.
 */
void updateShopDisplayW_Equip(WINDOW* w, Equip* e) {
	box(w,0,0);
	print_label(w, 1, 0, 30, "Selected item", COLOR_PAIR(6));
	mvwaddch(w, 2, 0, ACS_LTEE);
	mvwhline(w, 2, 1, ACS_HLINE, 28);
	mvwaddch(w, 2, 29, ACS_RTEE);

	char item_sprite_line[80];
	int y,x;
	for (int rows = 0; rows < 8; rows++) {

		y = 3 + rows;
		x = 4;

		strcpy(item_sprite_line,e->sprite[rows]);
		for (int cols = 0; cols < 14; cols++) {
			print_encoded_char(w, y, x + cols, item_sprite_line[cols]);
		}
	}

	y = 5;
	x = 19;

	mvwprintw(w, y, x, "Atk: %i", e->atk);
	mvwprintw(w, y+1, x, "Def: %i", e->def);
	mvwprintw(w, y+2, x, "Vel: %i", e->vel);
	mvwprintw(w, y+3, x, "Enr: %i", e->enr);

	y = 12;
	x = 2;

	mvwprintw(w, y, x, "%s", stringFromEquips(e->class));
	mvwprintw(w, y+1, x, "Slot: %s", stringFromEquipzones(e->type));



	wattron(w,COLOR_PAIR(6));
	mvwprintw(w, y, x + 12, "Level: %i", e->level);
	wattroff(w,COLOR_PAIR(6));
	y = 13;
	wattron(w,COLOR_PAIR(8));
	mvwprintw(w, y, x + 12, "Perks: %i", e->perksCount);
	wattroff(w,COLOR_PAIR(8));
	y++;
	wattron(w,COLOR_PAIR(3));
	for (int i = 0; i < e->perksCount; i++) {
		mvwprintw(w, y +i, x + 13, "%s", nameStringFromPerk(e->perks[i]->class));
	}
	wattroff(w,COLOR_PAIR(3));
}

/**
 * Takes a WINDOW and Consumable pointers and updates the window contents with the Consumable stats.
 * @see Consumable
 * @see Shop
 * @see handleRoom_Shop()
 * @param w The window pointer to print into.
 * @param c The Consumable to be printed.
 */
void updateShopDisplayW_Consumable(WINDOW* w, Consumable* c) {
	box(w,0,0);
	print_label(w, 1, 0, 30, "Selected item", COLOR_PAIR(6));
	mvwaddch(w, 2, 0, ACS_LTEE);
	mvwhline(w, 2, 1, ACS_HLINE, 28);
	mvwaddch(w, 2, 29, ACS_RTEE);

	char item_sprite_line[80];
	int y,x;
	for (int rows = 0; rows < 8; rows++) {

		y = 4 + rows;
		x = 4;

		strcpy(item_sprite_line,c->sprite[rows]);
		for (int cols = 0; cols < 14; cols++) {
			print_encoded_char(w, y, x + cols, item_sprite_line[cols]);
		}
	}

	y = 14;
	x = 2;

	mvwprintw(w, y, x, "Qty: %i", c->qty);
}

/**
 * Takes a Fighter pointer and prints its current loadout from the equipslots field.
 * @see Fighter
 * @see Equipslot
 * @see Equipzone
 * @see EQUIPZONES
 * @see stringFromEquipzones()
 * @see stringFromQuality()
 * @see printQualityColor()
 * @param f The Fighter pointer with a loadout to print.
 */
void printLoadout(Fighter* f) {
	WINDOW *w;

	/* Initialize curses */
	//setlocale(LC_CTYPE, "it_IT.UTF-8");
	//initscr();
	clear();
	refresh();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);


	/* Initialize all the colors */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_BLACK, COLOR_WHITE);
	init_pair(8, COLOR_MAGENTA, COLOR_BLACK);


	/* Create the window to be associated with the menu */
	w = newwin(22, 70, 1, 2);
        keypad(w, TRUE);

	/* Print a border around the main window and print a title */
        box(w, 0, 0);

	//attron(COLOR_PAIR(2));
	//mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scoll down or up a page of items");
	mvprintw(24, 0, "(Press q to Exit)");
	//attroff(COLOR_PAIR(2));

	refresh();
	int x = 2;
	int y = 3;
	for (int z = 0; z < EQUIPZONES +1 ; z++) {
	        Equipslot * slot = (Equipslot *) f->equipslots[z];
		Equip* selected;

	        if (slot->active) {
	        	/* Prepare selection display window*/
	      	  	selected = slot->item;
	     	   	char s[80];

	        	for (int i = 0; i < 8; i++) {

				y = 1+i;

		   		strcpy(s,selected->sprite[i]);
		 		for (int j = 0; j < 14; j++) {

					print_encoded_char(w, y, x + j, s[j]);
		       		}
			}

			if (selected->atk > 0) {
		       		mvwprintw(w, y + 2, x, "Atk: %i", selected->atk);
			}
			if (selected->def > 0) {
				mvwprintw(w, y + 3, x, "Def: %i", selected->def);
			}
			if (selected->vel > 0) {
				mvwprintw(w, y + 4, x, "Vel: %i", selected->vel);
			}
			if (selected->enr > 0) {
				mvwprintw(w, y + 5, x, "Enr: %i", selected->enr);
			}

			int color = -1;
			switch (selected->qual) {
		        	case Bad: {
			        	wattron(w,COLOR_PAIR(1));
					color = 1;
		       		}
		       		break;
		        	case Average: {
			        	wattron(w,COLOR_PAIR(6));
			        	color = 6;
		        	}
		        	break;
		        	case Good: {
			        	wattron(w,COLOR_PAIR(4));
			        	color = 4;
		        	}
		        	break;
		        	default: {

		        	}
		        	break;
	        	};
	        	mvwprintw(w, y + 6, x, "Quality:    %s",stringFromQuality(selected->qual));
	        	wattroff(w,COLOR_PAIR(color));

	        	wattron(w,COLOR_PAIR(2));
	        	mvwprintw(w, y + 7, x, "Level:    %i",selected->level);
	        	wattroff(w,COLOR_PAIR(2));

	        	wattron(w,COLOR_PAIR(6));
	        	mvwprintw(w, y + 8, x, "Perks:    %i",selected->perksCount);
	        	int count = selected->perksCount;
			wattroff(w,COLOR_PAIR(6));

			wattron(w,COLOR_PAIR(4));
	        	for (int i = 0; i < count; i++) {
		        	Perk* p = selected->perks[i];

		        	mvwprintw(w, y + 9 + i, x, " x%i %s", p->innerValue, nameStringFromPerk(p->class));
	        	};
	        	wattroff(w,COLOR_PAIR(4));
	        }

	        wrefresh(w);

		x += 24;
	}//End for all equipzones
	int picked = 0;
	char c;
	while(!picked && (c = wgetch(w)) != 'q') {
		sleep(1); //We wait for user to exit
	}
}

/**
 * Takes a FoeParty pointer and prints its current layout into up to 3 new WINDOWS, deleted before returning.
 * @see FoeParty
 * @see stringFromFoePartyClass()
 * @param fp The FoeParty pointer whose layout will be displayed.
 */
void display_printFoeParty(FoeParty* fp) {
	WINDOW *wins[4];
	int cells = -1;
	int party_size = fp->size;
	//Check how many cells to display

	if ((party_size >= 3)) {
		cells = fp->tot_alive %5;
	} else if ( (party_size > 1)) {
		cells = fp->tot_alive %3;
	} else {
		cells = 1;
	}

	cells = fp->size;
	//FIXME: assert error if this is invalid
	if (cells > 4) cells = 4;

	log_tag("debug_log.txt","[FOEPARTY]","FP size was: (%i) - iCURR was (%i).", party_size, fp->current_index);
	log_tag("debug_log.txt","[FOEPARTY]","Set total cells to (%i).", cells);

	/* Initialize curses */
	//setlocale(LC_CTYPE, "it_IT.UTF-8");
	clear();
	refresh();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	//Define cells size
	int cell_cols = 18;
	int cell_rows = 12;

	//Prep win cells
	for (int i = 0 ; i < cells; i++) {
		wins[i] = newwin(cell_rows, cell_cols, 6, 2 + i*(cell_cols) +1);
	};

	//Display cells
	for (int i = 0; i < cells; i++) {
		int isalive = -1;
		switch (fp->class) {
			case Enemies: {
				isalive = fp->alive_enemies[i];
			}
			break;
			case Bosses: {
				isalive = fp->alive_bosses[i];
			}
			break;
			default: {
				fprintf(stderr,"UNEXPECTED FOEPARTYCLASS VALUE");
				log_tag("debug_log.txt","[PANIC]","Invalid foePartyClass value in debugFoeParty().");
				exit(EXIT_FAILURE);
			}
			break;
		}
		log_tag("debug_log.txt","[FOEPARTY]","Cell (%i) status was (%i)", i, isalive);
		if (isalive == 1) {
			Enemy* curr_foe = fp->enemy_foes[fp->current_index];
			mvwprintw(wins[i], 2, 2, "%s", stringFromEClass(curr_foe->class));
			mvwprintw(wins[i], 4, 2, "Hp: (%i/%i)", curr_foe->hp, curr_foe->totalhp);
			mvwprintw(wins[i], 6, 2, "Enemy (%i/%i)", i+1, fp->size);
		} else {
			mvwprintw(wins[i], 5, 2, "[Dead]");
		}
	}

	//mvwprintw(wins[0], 9, 2, "[Enter]-> quit.");
	//Box and refresh cells
        for (int i=0; i < cells; i++) {
	       box(wins[i], 0, 0);
	       wrefresh(wins[i]);
	}
	//Flash for half a sec
	napms(500);
	//Free cells
        for (int i=0; i < cells; i++) {
		delwin(wins[i]);
		log_tag("debug_log.txt","[FREE]","Freed win (%i) for display_printFoeParty().", i);
	}
	endwin();
}

/**
 * Takes a FoeParty pointer and prints its current values.
 * @see FoeParty
 * @see stringFromFoePartyClass()
 * @param fp The FoeParty pointer with to print.
 */
void debug_printFoeParty(FoeParty* fp) {
	WINDOW *w;

	/* Initialize curses */
	//setlocale(LC_CTYPE, "it_IT.UTF-8");
	//initscr();
	clear();
	refresh();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	w = newwin(24, 50, 1, 1);
	mvwprintw(w,1,1,"  FoeParty class: (%s), lvl: (%i)", stringFromFoePartyClass(fp->class), fp->level);
	mvwprintw(w,2,1,"  Size (%i), tot_alive (%i)", fp->size, fp->tot_alive);
	mvwprintw(w,3,1,"  Turnboosts: (%i) (%i) (%i) (%i)", fp->turnboost_atk, fp->turnboost_def, fp->turnboost_vel, fp->turnboost_enr);
	mvwprintw(w,4,1,"  Enemies:        Bosses:");
	for (int i=0; i < fp->size ; i++) {
		int isalive = fp->alive_enemies[i];
		if (isalive) {
			mvwprintw(w,6+i,2,"    Alive");
		} else {
			mvwprintw(w,6+i,2,"    Dead");
		}
	};
	for (int i=0; i < FOES_BOSSES_MAX +1; i++) {
		int isalive = fp->alive_bosses[i];
		if (isalive) {
			mvwprintw(w,6+i,16,"    Alive");
		} else {
			mvwprintw(w,6+i,16,"    Dead");
		}
	};
	int skip_height = (FOES_BOSSES_MAX > fp->size ? FOES_BOSSES_MAX : fp->size);

	mvwprintw(w,8+skip_height,2,"    Layout:");
	for (int i=0; i< fp->size; i++) {
		int isalive = -1;
		switch (fp->class) {
			case Enemies: {
				isalive = fp->alive_enemies[i];
			}
			break;
			case Bosses: {
				isalive = fp->alive_bosses[i];
			}
			break;
			default: {
				fprintf(stderr,"UNEXPECTED FOEPARTYCLASS VALUE");
				log_tag("debug_log.txt","[PANIC]","Invalid foePartyClass value in debugFoeParty().");
			}
			break;
		}
		if (isalive) {
			mvwprintw(w,9+skip_height,4+(i*7),"[A]");
			if (i != (fp->size-1)) mvwprintw(w,9+skip_height,5+(i*7)+3,"-> ");
		} else {
			mvwprintw(w,9+ skip_height,4+(i*7),"[D]-> ");
		}
		if (fp->current_index == i) {
			mvwprintw(w,9+skip_height,3+(i*7),"C");
		}
	}

	mvwprintw(w,10+skip_height,2,"Press [Enter] to return.");
        box(w, 0, 0);
	wrefresh(w);
	int res = scanf("%*c");
	log_tag("debug_log.txt","[DEBUG]","debug_printFoeParty() scanf() res was (%i)",res);
	delwin(w);
	endwin();
}

/**
 * Takes a Fighter pointer and displays its equipsbag information.
 * @see Fighter
 * @see Equip
 * @see EQUIPSMAX
 * @see handleEquips
 * @param f The fighter pointer whose equipsbag will be displayed.
 */
void displayEquipbagMenu(Fighter* f){
	WINDOW *my_wins[1];
	ITEM **my_items;
	MENU *my_menu;
        WINDOW *my_menu_win;
        int n_choices = 0;
	int c;

	/* Initialize curses */
	//setlocale(LC_CTYPE, "it_IT.UTF-8");
	//initscr();
	clear();
	refresh();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);


	/* Initialize all the colors */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_BLACK, COLOR_WHITE);
	init_pair(8, COLOR_MAGENTA, COLOR_BLACK);

	n_choices = f->equipsBagOccupiedSlots ;
	/* Create menu items */
      	my_items = (ITEM **)calloc(n_choices+1,sizeof(ITEM *));
	for (int i = 0; i < n_choices ; i++) {
		Equip * e = (Equip *) f->equipsBag[i];
        	my_items[i] = new_item(stringFromEquips(e->class),"Desc");
	}
        my_items[n_choices] = (ITEM*) NULL;

	/* Create menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Set description off */
	menu_opts_off(my_menu,O_SHOWDESC);

	/* Create the window to be associated with the menu */
        my_menu_win = newwin(20, 25, 2, 2);
        keypad(my_menu_win, TRUE);

        /* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 15, 20, 4, 2));
	set_menu_format(my_menu, 14, 1);

	/* Set menu mark to the string " >  " */
        set_menu_mark(my_menu, " >  ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_label(my_menu_win, 1, 0, 26, "Equips", COLOR_PAIR(4));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 23);
	mvwaddch(my_menu_win, 2, 24, ACS_RTEE);

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	//attron(COLOR_PAIR(2));
	//mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scoll down or up a page of items");
	mvprintw(24, 0, "Arrow Keys to navigate, Enter to wear selected item (q to Exit)");
	//attroff(COLOR_PAIR(2));
	//refresh();

	/* Prepare selection display window*/
	my_wins[0] = newwin(18, 44, 4, 34);
	Equip* eq = (Equip*) f->equipsBag[0];
	updateSelectedEquipW(my_wins[0], my_menu,eq, f);

	refresh();

	int picked = 0;
	ITEM* cur = NULL;

	while(!picked && (c = wgetch(my_menu_win)) != 'q') {
		switch(c) {
			case KEY_DOWN: {
				menu_driver(my_menu, REQ_DOWN_ITEM);
				cur = current_item(my_menu);
				}
				break;
			case KEY_UP: {
			        menu_driver(my_menu, REQ_UP_ITEM);
				cur = current_item(my_menu);
				}
			        break;
			case KEY_NPAGE: {
				menu_driver(my_menu, REQ_SCR_DPAGE);
				cur = current_item(my_menu);
				}
				break;
			case KEY_PPAGE: {
			        menu_driver(my_menu, REQ_SCR_UPAGE);
				cur = current_item(my_menu);
				}
			        break;
			case 10: { /*Enter, set equip*/
				picked = 1;

				cur = current_item(my_menu);
				int num = item_index(cur);
				//Retrieve item info
				Equip* e = (Equip*) f->equipsBag[num];
				Equipslot* slot = (Equipslot*) f->equipslots[e->type];
				if (slot->active == 1) {
					//We reset status for equipped item
					slot->item->equipped = 0;

					removeEquipPerks(slot->item, f);

					//We adjust total boost removing current values
					f->equipboost_atk -= slot->item->atk;
					f->equipboost_def -= slot->item->def;
					f->equipboost_vel -= slot->item->vel;
					f->equipboost_enr -= slot->item->enr;

				};

				//We equip the item
				slot->item = e;
				slot->item->equipped = 1;

				applyEquipPerks(slot->item, f);
				slot->active = 1;

				//Apply the new item stats
				f->equipboost_atk += slot->item->atk;
				f->equipboost_def += slot->item->def;
				f->equipboost_vel += slot->item->vel;
				f->equipboost_enr += slot->item->enr;

				pos_menu_cursor(my_menu);
				refresh();
				};
				break;
		        }

			int num = item_index(cur);
			num = (num >= 0 && num <= n_choices ? num : 0);
			Equip * selected = (Equip *) f->equipsBag[num];

			//Update selected window
			wclear(my_wins[0]);
			wrefresh(my_wins[0]);
			updateSelectedEquipW(my_wins[0], my_menu,selected,f);
			refresh();
	}
	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        for(int k = 0; k <= n_choices ; k++) {
        	free_item(my_items[k]);
	}
        free_menu(my_menu);
	endwin();

}

/**
 * Takes a Fighter, an Enemy and a Boss pointers and prints content of consumablesBag before asking user input to use a Consumable. isBoss is passed to useConsumable().
 * @see Fighter
 * @see Enemy
 * @see Consumable
 * @see CONSUMABLESMAX
 * @see printConsumableStats()
 * @see useConsumable()
 * @param f The Fighter pointer using the Consumable.
 * @param e The Enemy pointer for current enemy.
 * @param b The Boss pointer for current boss.
 * @param isBoss The integer defining if effect applies enemy or boss (==1).
 */
void handleConsumables(Fighter* f, Enemy* e, Boss* b, int isBoss) {

	WINDOW *my_wins[1];
	ITEM **my_items;
	MENU *my_menu;
        WINDOW *my_menu_win;
        int n_choices = 0;
	int c;

	/* Initialize curses */
	//setlocale(LC_CTYPE, "it_IT.UTF-8");
	//initscr();
	clear();
	refresh();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);


	/* Initialize all the colors */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_BLACK, COLOR_WHITE);
	init_pair(8, COLOR_MAGENTA, COLOR_BLACK);

	//To print something when you have no consumables

	for (int i = 0; i < CONSUMABLESMAX +1; i++) {
		Consumable * c = (Consumable*) f->consumablesBag[i];
		int q = c->qty;
		if (q > 0) {
			n_choices++;
		}
	}

	if (n_choices == 0) {

		my_menu_win = newwin(10, 40, 2, 2);

		box(my_menu_win, 0, 0);


		//mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
		//mvwhline(my_menu_win, 2, 1, ACS_HLINE, 23);
		//mvwaddch(my_menu_win, 2, 33, ACS_RTEE);

		wattron(my_menu_win,COLOR_PAIR(6));
		mvwprintw(my_menu_win,2, 2, "Your bag is empty.");
		mvwprintw(my_menu_win,3, 2, "You'll find more consumables soon.");
		wattroff(my_menu_win,COLOR_PAIR(6));
		wrefresh(my_menu_win);
		refresh();

		napms(1500);
		endwin();
		return;
	}

	/* Create menu items */
	int tot = 0;
	int q = 0;
      	my_items = (ITEM **)calloc(n_choices,sizeof(ITEM *));
	for (int i = 0; i < CONSUMABLESMAX +1; i++) {
		if (tot < n_choices) {
			Consumable * c = (Consumable*) f->consumablesBag[i];
			q = c->qty;
			if ( q > 0) {
        			my_items[tot] = new_item(consumablestrings[i],consumablestrings[i]);
				tot++;
			}
		} else {
			break;
		}
	}
        my_items[tot] = (ITEM*) NULL;

	/* Create menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Set description off */
	menu_opts_off(my_menu,O_SHOWDESC);

	/* Create the window to be associated with the menu */
        my_menu_win = newwin(20, 25, 2, 2);
        keypad(my_menu_win, TRUE);

        /* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 15, 20, 4, 2));
	set_menu_format(my_menu, 14, 1);

	/* Set menu mark to the string " >  " */
        set_menu_mark(my_menu, " >  ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_label(my_menu_win, 1, 0, 26, "Consumables", COLOR_PAIR(4));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 23);
	mvwaddch(my_menu_win, 2, 24, ACS_RTEE);

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	//attron(COLOR_PAIR(2));
	//mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scoll down or up a page of items");
	mvprintw(23, 0, "Arrow Keys to navigate (Press q to Exit)");
	//attroff(COLOR_PAIR(2));
	//refresh();

	/* Prepare selection display window*/
	my_wins[0] = newwin(18, 44, 4, 34);
	updateSelectedConsumableW(my_wins[0], my_menu, f);


 	//init_consumable_wins(my_wins, e, player, 2);

	refresh();

	int picked = 0;
	ITEM* cur;

	while(!picked && (c = wgetch(my_menu_win)) != 'q') {
		switch(c) {
			case KEY_DOWN: {
				menu_driver(my_menu, REQ_DOWN_ITEM);
				cur = current_item(my_menu);
				//Update selected window
				wclear(my_wins[0]);
				wrefresh(my_wins[0]);
				updateSelectedConsumableW(my_wins[0], my_menu,f);
				}
				break;
			case KEY_UP: {
			        menu_driver(my_menu, REQ_UP_ITEM);
				cur = current_item(my_menu);
				//Update selected window
				wclear(my_wins[0]);
				wrefresh(my_wins[0]);
				updateSelectedConsumableW(my_wins[0], my_menu,f);
				}
			        break;
			case KEY_NPAGE: {
				menu_driver(my_menu, REQ_SCR_DPAGE);
				cur = current_item(my_menu);
				//Update selected window
				wclear(my_wins[0]);
				wrefresh(my_wins[0]);
				updateSelectedConsumableW(my_wins[0], my_menu,f);
				}
				break;
			case KEY_PPAGE: {
			        menu_driver(my_menu, REQ_SCR_UPAGE);
				cur = current_item(my_menu);
				//Update selected window
				wclear(my_wins[0]);
				wrefresh(my_wins[0]);
				updateSelectedConsumableW(my_wins[0], my_menu,f);
				}
			        break;
			case 10: { /*Enter*/
				picked = 1;

				cur = current_item(my_menu);

				useConsumable(f,e,b,(char*)item_name(cur),isBoss);
				pos_menu_cursor(my_menu);
				refresh();
				};
				break;
		        }
                wrefresh(my_menu_win);
	}
	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        for(int k = 0; k <= n_choices; k++) {
        	free_item(my_items[k]);
	}
        free_menu(my_menu);
	endwin();
}

/**
 * Takes a Fighter pointer and prints content of artifactsBag.
 * @see Fighter
 * @see Artifact
 * @see ARTIFACTSMAX
 * @see printArtifactStats()
 * @param f The Fighter pointer at hand.
 */
void handleArtifacts(Fighter* f) {

	WINDOW *my_wins[1];
	ITEM **my_items;
	MENU *my_menu;
        WINDOW *my_menu_win;
        int n_choices = 0;
	int c;

	/* Initialize curses */
	//setlocale(LC_CTYPE, "it_IT.UTF-8");
	//initscr();
	clear();
	refresh();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);


	/* Initialize all the colors */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_BLACK, COLOR_WHITE);
	init_pair(8, COLOR_MAGENTA, COLOR_BLACK);

	//To print something when you have no consumables

	for (int i = 0; i < ARTIFACTSMAX +1; i++) {
		Artifact * a = f->artifactsBag[i];
		int q = a->qty;
		if (q > 0) {
			n_choices++;
		}
	}

	if (n_choices == 0) {

		my_menu_win = newwin(10, 40, 2, 2);

		box(my_menu_win, 0, 0);


		//mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
		//mvwhline(my_menu_win, 2, 1, ACS_HLINE, 23);
		//mvwaddch(my_menu_win, 2, 33, ACS_RTEE);

		wattron(my_menu_win,COLOR_PAIR(6));
		mvwprintw(my_menu_win,2, 2, "Your bag is empty.");
		mvwprintw(my_menu_win,3, 2, "You'll find more artifacts soon.");
		wattroff(my_menu_win,COLOR_PAIR(6));
		wrefresh(my_menu_win);
		refresh();

		napms(1500);
		endwin();
		return;
	}

	/* Create menu items */
	int tot = 0;
	int q = 0;
      	my_items = (ITEM **)calloc(n_choices,sizeof(ITEM *));
	for (int i = 0; i < ARTIFACTSMAX +1; i++) {
		if (tot < n_choices) {
			Artifact * a = f->artifactsBag[i];
			q = a->qty;
			if ( q > 0) {
				char qty[15];
				sprintf(qty, "x%i", q);
        			my_items[tot] = new_item(artifactstrings[i],artifactstrings[i]);
				tot++;
			}
		} else {
			break;
		}
	}
        my_items[tot] = (ITEM*) NULL;

	/* Create menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Set description off */
	menu_opts_off(my_menu,O_SHOWDESC);

	/* Create the window to be associated with the menu */
        my_menu_win = newwin(20, 25, 2, 2);
        keypad(my_menu_win, TRUE);

        /* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 15, 20, 4, 2));
	set_menu_format(my_menu, 14, 1);

	/* Set menu mark to the string " >  " */
        set_menu_mark(my_menu, " >  ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_label(my_menu_win, 1, 0, 26, "Artifacts", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 23);
	mvwaddch(my_menu_win, 2, 24, ACS_RTEE);

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	//attron(COLOR_PAIR(2));
	//mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scoll down or up a page of items");
	mvprintw(23, 0, "Arrow Keys to navigate (Press q to Exit)");
	//attroff(COLOR_PAIR(2));
	//refresh();

	/* Prepare selection display window*/
	my_wins[0] = newwin(18, 44, 4, 34);
	updateSelectedArtifactW(my_wins[0], my_menu,f);


 	//init_consumable_wins(my_wins, e, player, 2);

	refresh();

	int picked = 0;
	ITEM* cur;

	while(!picked && (c = wgetch(my_menu_win)) != 'q') {
		switch(c) {
			case KEY_DOWN: {
				menu_driver(my_menu, REQ_DOWN_ITEM);
				cur = current_item(my_menu);
				//Update selected window
				wclear(my_wins[0]);
				wrefresh(my_wins[0]);
				updateSelectedArtifactW(my_wins[0], my_menu,f);
				}
				break;
			case KEY_UP: {
			        menu_driver(my_menu, REQ_UP_ITEM);
				cur = current_item(my_menu);
				//Update selected window
				wclear(my_wins[0]);
				wrefresh(my_wins[0]);
				updateSelectedArtifactW(my_wins[0], my_menu,f);
				}
			        break;
			case KEY_NPAGE: {
				menu_driver(my_menu, REQ_SCR_DPAGE);
				cur = current_item(my_menu);
				//Update selected window
				wclear(my_wins[0]);
				wrefresh(my_wins[0]);
				updateSelectedArtifactW(my_wins[0], my_menu,f);
				}
				break;
			case KEY_PPAGE: {
			        menu_driver(my_menu, REQ_SCR_UPAGE);
				cur = current_item(my_menu);
				//Update selected window
				wclear(my_wins[0]);
				wrefresh(my_wins[0]);
				updateSelectedArtifactW(my_wins[0], my_menu,f);
				}
			        break;
			case 10: { /*Enter*/
				picked = 1;

				move(18,47);
				clrtoeol();
				cur = current_item(my_menu);
				mvprintw(18, 47, "Item selected is : %s", item_name(cur));
				//useConsumable(f,e,item_name(cur));
				pos_menu_cursor(my_menu);
				refresh();
				};
				break;
		        }
                wrefresh(my_menu_win);
	}
	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        for(int k = 0; k <= n_choices; k++) {
        	free_item(my_items[k]);
	}
        free_menu(my_menu);
	endwin();
}

/**
 * Takes a Fighter and a Path pointers and asks user input to manage the Equips in fighter's equipsBag.
 * User can view the bag, equip a new item, view its loadout or exit.
 * Equipping an item causes all the relevant field in Fighter to be updated.
 * @see Fighter
 * @see Equipslot
 * @see Equipzone
 * @see EQUIPZONES
 * @param f The Fighter pointer with a equipsBag.
 * @param p The Path pointer of the current game.
 */
void handleEquips(Fighter* f, Path* p) {

	WINDOW *my_wins[1];
	ITEM **my_items;
	MENU *my_menu;
        WINDOW *my_menu_win;
        int n_choices = 0;
	int c;

	/* Initialize curses */
	//setlocale(LC_CTYPE, "it_IT.UTF-8");
	//initscr();
	clear();
	refresh();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);


	/* Initialize all the colors */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_BLACK, COLOR_WHITE);
	init_pair(8, COLOR_MAGENTA, COLOR_BLACK);

	//To print something when you have no equips

	if ( f->equipsBagOccupiedSlots == 0) {

		my_menu_win = newwin(10, 40, 2, 2);

		box(my_menu_win, 0, 0);


		//mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
		//mvwhline(my_menu_win, 2, 1, ACS_HLINE, 23);
		//mvwaddch(my_menu_win, 2, 33, ACS_RTEE);

		wattron(my_menu_win,COLOR_PAIR(6));
		mvwprintw(my_menu_win,2, 2, "Your bag is empty.");
		mvwprintw(my_menu_win,3, 2, "You'll find more items soon.");
		wattroff(my_menu_win,COLOR_PAIR(6));
		wrefresh(my_menu_win);
		refresh();

		napms(1500);
		endwin();
		return;
	}

	char *choices_equips[] = {
		"Open Bag",
		"Check loadout",
		(char *) NULL,
	};
        n_choices = ARRAY_SIZE(choices_equips);
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));

	/* Create menu items */
      	my_items = (ITEM **)calloc(n_choices,sizeof(ITEM *));
	for (int i = 0; i < n_choices ; i++) {
        	my_items[i] = new_item(choices_equips[i],choices_equips[i]);
	}
        //my_items[n_choices] = (ITEM*) NULL;

	/* Create menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Set description off */
	menu_opts_off(my_menu,O_SHOWDESC);

	/* Create the window to be associated with the menu */
        my_menu_win = newwin(20, 25, 2, 2);
        keypad(my_menu_win, TRUE);

        /* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 15, 20, 4, 2));
	set_menu_format(my_menu, 14, 1);

	/* Set menu mark to the string " >  " */
        set_menu_mark(my_menu, " >  ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_label(my_menu_win, 1, 0, 26, "Equips", COLOR_PAIR(4));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 23);
	mvwaddch(my_menu_win, 2, 24, ACS_RTEE);

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	//attron(COLOR_PAIR(2));
	//mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scoll down or up a page of items");
	mvprintw(24, 0, "Arrow Keys to navigate (Press q to Exit)");
	//attroff(COLOR_PAIR(2));
	//refresh();

	/* Prepare loadout display window*/
	my_wins[0] = newwin(22, 44, 1, 34);
	updateEquipslotsWin(my_wins[0],f);

	refresh();

	int picked = 0;
	int n = -1;
	ITEM* cur;

	while(!picked && (c = wgetch(my_menu_win)) != 'q') {
		switch(c) {
			case KEY_DOWN: {
				menu_driver(my_menu, REQ_DOWN_ITEM);
				cur = current_item(my_menu);
				}
				break;
			case KEY_UP: {
			        menu_driver(my_menu, REQ_UP_ITEM);
				cur = current_item(my_menu);
				}
			        break;
			case KEY_NPAGE: {
				menu_driver(my_menu, REQ_SCR_DPAGE);
				cur = current_item(my_menu);
				}
				break;
			case KEY_PPAGE: {
			        menu_driver(my_menu, REQ_SCR_UPAGE);
				cur = current_item(my_menu);
				}
			        break;
			case 10: { /*Enter*/
				picked = 1;
				int check = -1;

				move(18,47);
				clrtoeol();
				cur = current_item(my_menu);
				//mvprintw(18, 47, "Item selected is : %s", item_name(cur));
				if ((check = strcmp(item_name(cur),"Open Bag")) == 0) {
					n = 0;
				}
				if ((check = strcmp(item_name(cur),"Check loadout")) == 0) {
					n = 2;
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
        for(int k = 0; k < n_choices; k++) {
        	free_item(my_items[k]);
	}
        free_menu(my_menu);
	endwin();

	switch (n) {
		case 0: { //List all equips in bag
			displayEquipbagMenu(f);
		}
		break;
		case 2: { //Print all equipped items

			printLoadout(f);
		};
		break;
		case -1: {
			int res = system("clear");
			log_tag("debug_log.txt","[DEBUG]","handleEquips() system(\"clear\") res was (%i)",res);
			return;
		};
		break;
	};

}

/**
 * Takes a specialMove, a Fighter, a Enemy, a Boss and a Path pointers (and integers for current room and enemy indexes) and uses the requested special move.
 * Prints the result to the passed WINDOW.
 * The isBoss integer determines if the receiver is the Enemy or the Boss.
 * @see Fighter
 * @see SpecialSlot
 * @see SPECIALSMAX
 * @param move The specialMove to execute.
 * @param w The WINDOW pointer to print results to.
 * @param f The Fighter pointer with a equipsBag.
 * @param e The Enemy pointer for current enemy.
 * @param b The Boss pointer.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Is equal to 1 when receiver is a Boss.
 */
void fight_Special(specialMove move, WINDOW* w, Fighter* f, Enemy* e, Boss* b, Path* p, int roomIndex, int enemyIndex, int isBoss) {

	switch(move) {
		case KSlash: {
			knightSpecial_Slash(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case KCover: {
			knightSpecial_Cover(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case KArmordrop: {
			knightSpecial_Armordrop(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case KBerserk: {
			knightSpecial_Berserk(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case AHeadshot: {
			archerSpecial_Headshot(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case AQuivercheck: {
			archerSpecial_Quivercheck(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case APoisonshot: {
			archerSpecial_Poisonshot(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case AFireshot: {
			archerSpecial_Fireshot(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case MFatewarp: {
			mageSpecial_Fatewarp(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case MPowerup: {
			mageSpecial_Powerup(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case MSpellstrike: {
			mageSpecial_Spellstrike(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case MFlamering: {
			mageSpecial_Flamering(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case XGrimdagger: {
			assassinSpecial_Grimdagger(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case XLeechknife: {
			assassinSpecial_Leechknife(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case XDisguise: {
			assassinSpecial_Disguise(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		case XVenomblade: {
			assassinSpecial_Venomblade(w,f,e,b,p,roomIndex,enemyIndex,isBoss);
			     }
		break;
		default: {
			fprintf(stderr,"%i is not a valid move.\n",move);
			exit(EXIT_FAILURE);
			 }
		break;
	}

}

/**
 * Takes a Fighter, a Enemy, a Boss and a Path pointers (and integers for current room and enemy indexes) and asks user input to use an unlocked special attack.
 * The isBoss integer determines if the receiver is the Enemy or the Boss.
 * If the selected slot is enabled and fighter has enough energy, GET_CALLBACK() is called to have the special attack execute.
 * @see Fighter
 * @see SpecialSlot
 * @see SPECIALSMAX
 * @see GET_CALLBACK()
 * @param f The Fighter pointer with a equipsBag.
 * @param e The Enemy pointer for current enemy.
 * @param b The Boss pointer.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Is equal to 1 when receiver is a Boss.
 */
void handleSpecials(Fighter* f, Enemy* e, Boss* b, Path* p, int roomIndex, int enemyIndex, int isBoss) {
	WINDOW *my_wins[1];
	ITEM **my_items;
	MENU *my_menu;
        WINDOW *my_menu_win;
        int n_choices = 0;
	int c;

	/* Initialize curses */
	//setlocale(LC_CTYPE, "it_IT.UTF-8");
	//initscr();
	clear();
	refresh();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);


	/* Initialize all the colors */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_BLACK, COLOR_WHITE);
	init_pair(8, COLOR_MAGENTA, COLOR_BLACK);

	//To print something when you have no specials
	for (int i = 0; i < SPECIALSMAX + 1; i++) {
		Specialslot* s = f->specials[i];
		if (s->enabled) {
			n_choices++;
		}
	}


	if (n_choices == 0) {

		my_menu_win = newwin(10, 40, 2, 2);

		box(my_menu_win, 0, 0);

		wattron(my_menu_win,COLOR_PAIR(6));
		mvwprintw(my_menu_win,3, 2, "You don't know any special move yet.");
		wattroff(my_menu_win,COLOR_PAIR(6));
		wrefresh(my_menu_win);
		refresh();

		napms(800);
		endwin();
		return;
	}

	/* Create menu items */
	int tot = 0;
      	my_items = (ITEM **)calloc(n_choices,sizeof(ITEM *));
	for (int i = 0; i < SPECIALSMAX +1; i++) {
		if (tot < n_choices) {
			Specialslot* s = f->specials[i];
			if (s->enabled) {
        			my_items[tot] = new_item(nameStringFromSpecial(f->class,i),"a");
				tot++;
			}
		} else {
			break;
		}
	}
        my_items[tot] = (ITEM*) NULL;

	/* Create menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Set description off */
	menu_opts_off(my_menu,O_SHOWDESC);

	/* Create the window to be associated with the menu */
        my_menu_win = newwin(20, 26, 2, 2);
        keypad(my_menu_win, TRUE);

        /* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 15, 24, 4, 2));
	set_menu_format(my_menu, 14, 1);

	/* Set menu mark to the string " >  " */
        set_menu_mark(my_menu, " >  ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_label(my_menu_win, 1, 0, 26, "Specials", COLOR_PAIR(4));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 24);
	mvwaddch(my_menu_win, 2, 25, ACS_RTEE);

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	//attron(COLOR_PAIR(2));
	//mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scoll down or up a page of items");
	mvprintw(23, 0, "Arrow Keys to navigate (Press q to Exit)");
	//attroff(COLOR_PAIR(2));

	/* Prepare selection display window*/
	my_wins[0] = newwin(18, 40, 4, 30);
	updateSelectedSpecialW(my_wins[0], my_menu, f);

	refresh();

	int picked = 0;
	ITEM* cur;

	while(!picked && (c = wgetch(my_menu_win)) != 'q') {
		switch(c) {
			case KEY_DOWN: {
				menu_driver(my_menu, REQ_DOWN_ITEM);
				cur = current_item(my_menu);
				//Update selected window
				wclear(my_wins[0]);
				wrefresh(my_wins[0]);
				updateSelectedSpecialW(my_wins[0], my_menu,f);
				}
				break;
			case KEY_UP: {
			        menu_driver(my_menu, REQ_UP_ITEM);
				cur = current_item(my_menu);
				//Update selected window
				wclear(my_wins[0]);
				wrefresh(my_wins[0]);
				updateSelectedSpecialW(my_wins[0], my_menu,f);
				}
			        break;
			case KEY_NPAGE: {
				menu_driver(my_menu, REQ_SCR_DPAGE);
				cur = current_item(my_menu);
				//Update selected window
				wclear(my_wins[0]);
				wrefresh(my_wins[0]);
				updateSelectedSpecialW(my_wins[0], my_menu,f);
				}
				break;
			case KEY_PPAGE: {
			        menu_driver(my_menu, REQ_SCR_UPAGE);
				cur = current_item(my_menu);
				//Update selected window
				wclear(my_wins[0]);
				wrefresh(my_wins[0]);
				updateSelectedSpecialW(my_wins[0], my_menu,f);
				}
			        break;
			case 10: { /*Enter*/
				picked = 1;

				cur = current_item(my_menu);
				//mvprintw(18, 47, "Item selected is : %s", item_name(cur));
				int check = -1;
				Specialslot * selected = NULL;
				for (int i = 0; i < SPECIALSMAX + 1; i++) {
					if ((check = strcmp(item_name(cur),nameStringFromSpecial(f->class,i))) == 0) {
					selected = f->specials[i];
					break;
					}
				}

				if (selected == NULL) {
					log_tag("debug_log.txt","[ERROR]","Failed to match a Specialslot in handleSpecials()");
					exit(EXIT_FAILURE);
				}
				//Check if the selected move is enabled AND if user has enough energy
				//Considering the equipboost in this way makes it so that you can't get to 0 energy
				if (selected->enabled && selected->cost <= f->energy + f->equipboost_enr) {
					wclear(my_wins[0]);
					wrefresh(my_wins[0]);

					//do to the move
					fight_Special(selected->move,my_wins[0],f,e,b,p,roomIndex,enemyIndex,isBoss);
					log_tag("debug_log.txt","[DEBUG-SPECIALS]","Fighter [%s] used special: [%s].", f->name, nameStringFromSpecial(f->class,selected->move));
					pos_menu_cursor(my_menu);
					wrefresh(my_wins[0]);
					refresh();
					napms(1000);
				};
				break;
		       } //End case 10 (Enter)
		} //End switch
                wrefresh(my_menu_win);
	} //End while
	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        for(int k = 0; k <= n_choices; k++) {
        	free_item(my_items[k]);
	}
        free_menu(my_menu);
	endwin();
}

/**
 * Takes a Fighter pointer and prints its stats field with all the values, formatted.
 * @see Fighter
 * @see countStats
 * @param f The Fighter pointer with stats to print.
 */
void handleStats(Fighter* f){

	log_tag("debug_log.txt","[DEBUG]","Doing handleStats()");

	WINDOW *win; /**< Main window, used for fighter stats.*/
	WINDOW *history_win; /**< Second window, used for lifetime stats.*/

	/* Initialize curses */
	clear();
	refresh();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	/* Initialize all the colors */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_BLACK, COLOR_WHITE);
	init_pair(8, COLOR_MAGENTA, COLOR_BLACK);

	/* Create the windows for player stats and lifetime counters */
        win = newwin(22, 35, 1, 2);
        history_win = newwin(24, 30, 1, 44);
        keypad(win, TRUE);
        keypad(history_win, TRUE);

	/* Print a border around the windows and print a title */
        box(win, 0, 0);
        box(history_win, 0, 0);
	print_label(win, 1, 0, 35, "Player", COLOR_PAIR(6));
	print_label(history_win, 1, 0, 30, "Stats", COLOR_PAIR(6));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 33);
	mvwaddch(win, 2, 34, ACS_RTEE);
	mvwaddch(history_win, 2, 0, ACS_LTEE);
	mvwhline(history_win, 2, 1, ACS_HLINE, 28);
	mvwaddch(history_win, 2, 29, ACS_RTEE);

	wrefresh(win);
	wrefresh(history_win);

	attron(COLOR_PAIR(6));
	mvprintw(23, 0, "(Press q or Enter to Exit)");
	attroff(COLOR_PAIR(6));

	//Print fighter stats. Code is mostly from win_show()

	int y, x;
	int startx = 2;
	int width = 25;

	char s[80];
	for (int i = 0; i < 8; i++) {
		strcpy(s,f->sprite[i]);
		for (int j = 0; j < 14; j++) {
			x = startx + j;
			y = 4 + i;
			print_encoded_char(win, y, x, s[j]);
		}
	}

	int resetColor = 0;
	int temp;
	startx = 14;

	int l = strlen(f->name);
	temp = (width - l) / 2 - 4;
	x = startx + (int) temp;
	y = 4;
	wattron(win,COLOR_PAIR(2));
	mvwprintw(win, y, x, "%s",f->name);
	wattroff(win,COLOR_PAIR(2));

	l = strlen(stringFromClass(f->class));
	temp = (width - l) / 2 - 3;
	x = startx + (int) temp;
	y += 1;
	mvwprintw(win, y, x, "%s",stringFromClass(f->class));

	temp = (width - (f->hp/10) ) / 2 ;
	x = startx + 2;
	y += 2;
	if ( (f->hp / (f->totalhp / 1.0)) <= 0.25) {
		wattron(win,COLOR_PAIR(1));
		resetColor = 1;
	}
	mvwprintw(win, y, x, "Hp:      %i/%i", f->hp, f->totalhp);
	if (resetColor) {
		wattroff(win,COLOR_PAIR(1));
		resetColor = 0;
	};

	temp = (width - (f->energy/10) - (f->totalenergy/10)) / 2 ;
	x = startx + 2;
	y += 2;
	if ( (f->energy / (f->totalenergy / 1.0)) <= 0.25) {
		wattron(win,COLOR_PAIR(1));
		resetColor = 1;
	}
	mvwprintw(win, y, x, "Energy: %i/%i", f->energy, f->totalenergy);
	if (resetColor) {
		wattroff(win,COLOR_PAIR(1));
		resetColor = 0;
	};

	x = startx + 5;

	if (f->equipboost_enr != 0) {
		if (f->equipboost_enr > 0) {
			wattron(win,COLOR_PAIR(2));
			mvwprintw(win, y, x + 11, "+%i", f->equipboost_enr);
			wattroff(win,COLOR_PAIR(2));
		} else {
			wattron(win,COLOR_PAIR(1));
			mvwprintw(win, y, x + 11 , "-%i", f->equipboost_enr);
			wattroff(win,COLOR_PAIR(1));
		};
	}

	temp = (width - (f->atk/10)) / 2 ;
	x = startx + 2;
	y += 2;
	mvwprintw(win, y, x, "Atk:    %i", f->atk);

	x = startx + 5;

	if (f->equipboost_atk != 0) {
		if (f->equipboost_atk > 0) {
			wattron(win,COLOR_PAIR(2));
			mvwprintw(win, y, x + 8, "(+%i)", f->equipboost_atk);
			wattroff(win,COLOR_PAIR(2));
		} else {
			wattron(win,COLOR_PAIR(1));
			mvwprintw(win, y, x + 8 , "(-%i)", f->equipboost_atk);
			wattroff(win,COLOR_PAIR(1));
		};
	}

	temp = (width - (f->def/10)) / 2 ;
	x = startx + 2;
	y += 1;
	mvwprintw(win, y, x, "Def:    %i", f->def);

	x = startx + 5;

	if (f->equipboost_def != 0) {
		if (f->equipboost_def > 0) {
			wattron(win,COLOR_PAIR(2));
			mvwprintw(win, y, x + 8, "(+%i)", f->equipboost_def);
			wattroff(win,COLOR_PAIR(2));
		} else {
			wattron(win,COLOR_PAIR(1));
			mvwprintw(win, y, x + 8 , "(-%i)", f->equipboost_def);
			wattroff(win,COLOR_PAIR(1));
		};
	}

	temp = (width - (f->vel/10)) / 2 ;
	x = startx + 2;
	y += 1;
	mvwprintw(win, y, x, "Vel:    %i", f->vel);

	x = startx + 5;

	if (f->equipboost_vel != 0) {
		if (f->equipboost_vel > 0) {
			wattron(win,COLOR_PAIR(2));
			mvwprintw(win, y, x + 8, "(+%i)", f->equipboost_vel);
			wattroff(win,COLOR_PAIR(2));
		} else {
			wattron(win,COLOR_PAIR(1));
			mvwprintw(win, y, x + 8 , "(-%i)", f->equipboost_vel);
			wattroff(win,COLOR_PAIR(1));
			};
	}

	temp = (width - (f->level/10)) / 2 ;
	x = startx + 2;
	y += 2;
	mvwprintw(win, y, x, "Level:       %i", f->level);

	temp = (width - (f->currentlevelxp/10) - (f->totallevelxp/10)) / 2 ;
	x = startx + 2;
	y += 1;
	if ( (f->currentlevelxp / (f->totallevelxp / 1.0)) >= 0.75) {
		wattron(win,COLOR_PAIR(2));
		resetColor = 1;
	}
	mvwprintw(win, y, x, "Xp:        %i/%i", f->currentlevelxp, f->totallevelxp);
	if (resetColor) {
		wattroff(win,COLOR_PAIR(2));
		resetColor = 0;
	};

	l = strlen(stringFromStatus(f->status));
	temp = (width - l) / 2 ;
	x = startx + 2;
	y += 2;
	mvwprintw(win, y, x, "Status:  %s", stringFromStatus(f->status));


	//Print lifetime stats.

	y = 3;
	x = 4;

	mvwprintw(history_win, y, x, "Enemies killed:     %i", f->stats->enemieskilled);
	y += 2;
	mvwprintw(history_win, y, x, "Critical hits done: %i", f->stats->criticalhits);
	y += 2;
	mvwprintw(history_win, y, x, "Consumables found:  %i", f->stats->consumablesfound);
	y += 2;
	mvwprintw(history_win, y, x, "Rooms completed:    %i", f->stats->roomscompleted);
	y += 2;
	mvwprintw(history_win, y, x, "Equips found:       %i", f->stats->equipsfound);
	y += 2;
	mvwprintw(history_win, y, x, "Artifacts found:    %i", f->stats->artifactsfound);
	y += 2;
	mvwprintw(history_win, y, x, "Coins found:        %i", f->stats->coinsfound);
	y += 2;
	mvwprintw(history_win, y, x, "Bosses killed:     %i", f->stats->bosseskilled);
	y += 2;
	mvwprintw(history_win, y, x, "Keys found:        %i", f->stats->keysfound);
	y += 2;
	mvwprintw(history_win, y, x, "Floors completed:    %i", f->stats->floorscompleted);
	y += 2;

	refresh();

	int picked = 0;
	int c = -1;
	wrefresh(win);
	wrefresh(history_win);

	while(!picked && (c = wgetch(history_win)) != 'q') {
		switch(c) {
			case 10: { /*Enter*/
				picked = 1;

				};
				break;
		        }
	}
	clear();
	refresh();
	delwin(win);
	delwin(history_win);
	endwin();
	log_tag("debug_log.txt","[DEBUG]","Ended window mode for handleStats()");
}

/**
 * Shows tutorial info.
 * @see gameloop()
 */
void handleTutorial(void) {
	WINDOW *win;

 	//If we get to this function straight from getopts, we need to do initscr()

	/* Initialize curses */
	if (G_DOTUTORIAL_ON == 1) {
		initscr();
	}

	clear();
	refresh();
	start_color();
	for (int i = 0; i < PALETTE_S4C_H_TOTCOLORS; i++) {
		init_s4c_color_pair(&palette[i],9+i);
	}
	cbreak();
	noecho();
	keypad(stdscr, TRUE);


	/* Create the windows for tutorial */
        win = newwin(20, 70, 1, 2);
        keypad(win, TRUE);

	wclear(win);
	wrefresh(win);

	/* Print a border around the windows and print a title */
        //box(win, 0, 0);
	char label[25];
	sprintf(label,"Tutorial");
	print_label(win, 1, 0, 70, label, COLOR_PAIR(S4C_BLUE));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 68);
	mvwaddch(win, 2, 69, ACS_RTEE);

	wattron(win,COLOR_PAIR(S4C_BRIGHT_YELLOW));
	mvwprintw(win, 4, 1, "%s", "You can use the arrow keys and Enter to do everything needed for the game.");
	mvwprintw(win, 7, 1, "%s", "Buying things from a Shop may be tricky: you have to select one, then choose Buy. To select one, First go up/down to 'View Item', then press Enter, then you can scroll them with left/right. Press Enter to confirm your selection, then go back up to Buy.");
	mvwprintw(win, 12, 1, "%s", "When running in Rogue mode, you can change floors by killing a Boss.");

	wattroff(win,COLOR_PAIR(S4C_BRIGHT_YELLOW));

	wrefresh(win);
	char c;
	int res = scanf("%c",&c);
	log_tag("debug_log.txt","[DEBUG]","handle_Tutorial() scanf() res was (%i)",res);
	delwin(win);
	endwin();
}

/**
 * Takes a Fighter, a Path, and a Room pointers and handles menu for Rogue mode.
 * It also takes a loadInfo pointer to a struct used for loading a game.
 * User is asked for turn choice and according to the input, the corresponding action will be called.
 * @see Fighter
 * @see Path
 * @param p The Path pointer.
 * @param player The Fighter pointer at hand.
 * @param room The Room pointer for current room.
 * @param load_info A pointer to loadInfo struct containing stuff for loading a game.
 * @param kls The Koliseo used for allocations.
 * @param t_kls The Koliseo_Temp used for temporary allocations.
 */
int handleRogueMenu(Gamestate* gmst, Path* p, Fighter* player, Room* room, loadInfo* load_info, Koliseo* kls, Koliseo_Temp* t_kls) {
	Enemy* dummy_enemy = NULL;
	Boss* dummy_boss = NULL;
	FILE* dummy_savefile = NULL;
	FILE* save_file;
	WINDOW* dummy_notify_win = NULL;
	//Declare turnOP_args
	turnOP_args* args = init_turnOP_args(gmst, player, p, room, load_info, dummy_enemy, dummy_boss, dummy_savefile, dummy_notify_win, t_kls);

	//Strings for turn menu choices
 	char *choices[] = {
		"Artifacts",
		"Equips",
		"Perks",
		"Save",
		"Stats",
		"Quit",
		"Close",
		(char *)NULL,
	};

	char *debug_choices[] = {
		"Debug",
		(char*)NULL,
	};

	turnOption choice = 999;

	ITEM **menu_items;
	MENU *rogue_menu;
	WINDOW *menu_win;
	WINDOW *side_win;
	int n_choices, c;

	while( (choice != CLOSE_MENU ) ) {
		choice = 999;
		log_tag("debug_log.txt","[DEBUG]","Initialising curses for handleRogueMenu()");
		/* Initialize curses */
		//setlocale(LC_CTYPE, "it_IT.UTF-8");
		//initscr();
		start_color();
		clear();
		refresh();
		cbreak();
		noecho();
		keypad(stdscr, TRUE);

		//TODO: clear ambigue color definitions.

		int cursorCheck = curs_set(0); // We make the cursor invisible or return early with the error

		if (cursorCheck == ERR) {
			log_tag("debug_log.txt","[ERROR]","Failed curs_set(0) at handleRoom_Home()");
			return S4C_ERR_CURSOR; //fprintf(stderr,"animate => Terminal does not support cursor visibility state.\n");
		}

		/* Create turn menu items */
		n_choices = ARRAY_SIZE(choices);
		int debug_n_choices = 0;
		if (G_DEBUG_ON && G_EXPERIMENTAL_ON) { //We have to include debug menu options
			debug_n_choices = ARRAY_SIZE(debug_choices);
		}
		menu_items = (ITEM **)calloc(n_choices + debug_n_choices, sizeof(ITEM *));

		// Prepare menu items
		for(int k = 0; k < n_choices; k++) {
		    if (G_DEBUG_ON && G_EXPERIMENTAL_ON && k == n_choices -1) {
			//We skip adding the null string item
		    } else {
			menu_items[k] = new_item(choices[k], choices[k]);
		    }
		    /* Set the user pointer */
		    //set_item_userptr(my_items[i]);
		}
		// Prepare debug items
		if (G_DEBUG_ON && G_EXPERIMENTAL_ON) {
			for(int k = 0; k < debug_n_choices; k++) {
				menu_items[k + n_choices-1] = new_item(debug_choices[k],debug_choices[k]);
			}
		    /* Set the user pointer */
		    //set_item_userptr(my_items[i]);
		}

		/* Create menu */
		rogue_menu = new_menu((ITEM **)menu_items);

		/* Set description off */
		menu_opts_off(rogue_menu,O_SHOWDESC);

		/* Create the window to be associated with the menu */
		menu_win = newwin(9, 11, 5, 35);
		//nodelay(menu_win,TRUE); //We make getch non-blocking
		keypad(menu_win, TRUE);

		/* Set main window and sub window */
		set_menu_win(rogue_menu, menu_win);
		set_menu_sub(rogue_menu, derwin(menu_win, 8, 10, 1, 1));
		set_menu_format(rogue_menu, 7, 1);

		/* Set menu mark to the string "" */
		set_menu_mark(rogue_menu, "");

		/* Print a border around the menu */
		box(menu_win, 0, 0);

		/* Set menu colors */
		set_menu_fore(rogue_menu,COLOR_PAIR(S4C_RED));
		set_menu_back(rogue_menu,COLOR_PAIR(S4C_WHITE));

		//mvprintw(LINES - 1, 0, "Arrow Keys to navigate (F1 to Exit)");
		//attroff(COLOR_PAIR(2));

		/* Post the menu */
		post_menu(rogue_menu);
		wrefresh(menu_win);

		side_win = newwin(12, 24, 2, 5);
		scrollok(side_win,TRUE);
		wprintw(side_win,"\nYour menu.");
		wprintw(side_win,"\nWhat do you want to do?");
		wrefresh(side_win);
		refresh();

		int picked = 0;
		int picked_close = 0;

		/*
		//We set the colors to use s4c's palette file...
		FILE* palette_file;
		char path_to_palette[600];
		char static_path[500];
		char palette_name[50] = "palette.gpl";

		// Set static_path value to the correct static dir path
		resolve_staticPath(static_path);

		sprintf(path_to_palette,"%s/%s",static_path,palette_name);

		palette_file = fopen(path_to_palette, "r");

		init_s4c_color_pairs(palette_file);
		*/

		for (int i = 0; i < PALETTE_S4C_H_TOTCOLORS; i++) {
			init_s4c_color_pair(&palette[i],9+i);
		}

		while ( !picked && (c = wgetch(menu_win)) != KEY_F(1) && !picked_close) {
			switch(c) {
				case KEY_DOWN:
					menu_driver(rogue_menu, REQ_DOWN_ITEM);
					break;
				case KEY_UP:
					menu_driver(rogue_menu, REQ_UP_ITEM);
					break;
				case KEY_LEFT: { /*Left option pick*/
					ITEM *cur;
					cur = current_item(rogue_menu);
					choice = getTurnChoice((char*)item_name(cur));
					log_tag("debug_log.txt","[DEBUG]","Left on choice: [ %s ] value (%i)",item_name(cur),choice);
					if (choice == EQUIPS) {
						log_tag("debug_log.txt","[DEBUG]","Should do something");
					}
					}
					break;
				case KEY_RIGHT: { /*Right option pick*/
					ITEM *cur;
					cur = current_item(rogue_menu);
					choice = getTurnChoice((char*)item_name(cur));
					log_tag("debug_log.txt","[DEBUG]","Right on choice: [ %s ] value (%i)",item_name(cur),choice);
					if (choice == EQUIPS) {
						log_tag("debug_log.txt","[DEBUG]","Should do something");
					}
					}
					break;
				case KEY_NPAGE:
					menu_driver(rogue_menu, REQ_SCR_DPAGE);
					break;
				case KEY_PPAGE:
					menu_driver(rogue_menu, REQ_SCR_UPAGE);
					break;
				case 10: /* Enter */
				{
					picked = 1;
					ITEM *cur;

					//move(18,47);
					//clrtoeol();
					cur = current_item(rogue_menu);
					//mvprintw(18, 47, "Item selected is : %s", item_name(cur));
					choice = getTurnChoice((char*)item_name(cur));
					pos_menu_cursor(rogue_menu);
					refresh();
				};
				break;
				case 'q':
				{
					if (G_FASTQUIT_ON == 1) {
						log_tag("debug_log.txt","[DEBUG]","Player used q to quit from Rogue menu.");
						picked = 1;
						choice = getTurnChoice("Quit");
						pos_menu_cursor(rogue_menu);
						refresh();
					} else {
						log_tag("debug_log.txt","[DEBUG]","Player used q in Rogue menu, but G_FASTQUIT_ON was not 1.");
					}
				}
				break;
				default: {
						log_tag("debug_log.txt","[DEBUG]","Invalid keystroke in Rogue menu");
				}
				break;
			}
			wrefresh(menu_win);
			if (c == 10) { // Player char was enter
				if (choice == CLOSE_MENU) {
					picked_close = 1;
				}
				if (choice == SAVE) {
					char path_to_savefile[600];
					char static_path[500];
					char savefile_name[50] = HELAPORDO_SAVEPATH_1 ;

					// Set static_path value to the correct static dir path
					resolve_staticPath(static_path);

					sprintf(path_to_savefile,"%s/%s",static_path,savefile_name);
					save_file = fopen(path_to_savefile, "w");
					if (save_file == NULL)
					{
						fprintf(stderr,"[ERROR]    Can't open save file %s!\n",path_to_savefile);
						exit(EXIT_FAILURE);
					} else {
						log_tag("debug_log.txt","[TURNOP]","Assigning save_file pointer to args->save_file. Path: [%s]",path_to_savefile);
						args->save_file = save_file;
					}
				}
				turnOP(turnOP_from_turnOption(choice),args,kls,t_kls);
				if (choice == SAVE) {
					fclose(save_file);
					log_tag("debug_log.txt","[DEBUG]","Closed save_file pointer.");
				}
			} //End if Player char was enter
		}

		// Unpost menu and free all the memory taken up
		unpost_menu(rogue_menu);
		free_menu(rogue_menu);
		log_tag("debug_log.txt","[FREE]","Freed Rogue menu");
		int totalChoices = n_choices + debug_n_choices;
		if (G_DEBUG_ON) {
			totalChoices -= 1; //We subtract 1 to account for the discarded null choice in the base option set.
		}
		for(int k = 0; k < totalChoices; k++) {
			free_item(menu_items[k]);
			log_tag("debug_log.txt","[FREE]","Freed %i Rogue menu item",k);
		}
		free(menu_items);

		delwin(side_win);
		endwin();
		log_tag("debug_log.txt","[DEBUG]","Ended window mode for handleRogueMenu().");

	}
	//Free turnOP_args
	//free(args);
	log_tag("debug_log.txt","[FREE]","handleRogueMenu():  Freed turnOP_args");
	log_tag("debug_log.txt","[DEBUG]","Ended handleRogueMenu()");
	return 0;
}
