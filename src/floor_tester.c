#include "floors.h"

int test_floors(void) {
	G_LOG_ON = 1;

	WINDOW* test_win;
	initscr();
	clear();
	refresh();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	init_game_color_pairs();

	// Create the window
        test_win = newwin(28, 30, 1, 2);
	wclear(test_win);
	wrefresh(test_win);
        keypad(test_win, TRUE);

	/* Print a border around the windows and print a title */
        box(test_win, 0, 0);
	wrefresh(test_win);
	refresh();
	int res = -1;
	char msg[500];

	FILE* debug_file = fopen("static/debug_log.txt","w");
	if (debug_file == NULL) {
		fprintf(stderr,"Error while opening debug log file for truncation.");
		exit(EXIT_FAILURE);
	}
	fprintf(debug_file,"%s","");
	fclose(debug_file);

	Floor* dbg_floor = (Floor*) malloc(sizeof(Floor));
	// Start the random walk from the center of the dungeon
	int center_x = FLOOR_MAX_COLS / 2;
	int center_y = FLOOR_MAX_ROWS / 2;

	display_floor_layout(dbg_floor,test_win);
	//debug_print_floor_layout(dbg_floor,stdout);
	//printf("\nDeclared floor. Press Enter to return.\n");
	res = scanf("%*c");
	sprintf(msg,"test_floors() scanf() res was (%i)", res);
	log_tag("debug_log.txt","[DEBUG]",msg);

	// Init dbg_floor
	init_floor_layout(dbg_floor);
	display_floor_layout(dbg_floor,test_win);
	//debug_print_floor_layout(dbg_floor,stdout);
	//printf("\ninit_floor_layout(). Press Enter to return.\n");
	res = scanf("%*c");
	sprintf(msg,"test_floors() scanf() res was (%i)", res);
	log_tag("debug_log.txt","[DEBUG]",msg);

	//Set center as filled
	dbg_floor->floor_layout[center_x][center_y] = 1;
	display_floor_layout(dbg_floor,test_win);
	//debug_print_floor_layout(dbg_floor,stdout);
	//printf("\nSet center as filled. Press Enter to return.\n");
	res = scanf("%*c");
	sprintf(msg,"test_floors() scanf() res was (%i)", res);
	log_tag("debug_log.txt","[DEBUG]",msg);

	//Init floor rooms
	init_floor_rooms(dbg_floor);
	display_floor_layout(dbg_floor,test_win);
	//debug_print_floor_layout(dbg_floor,stdout);
	//printf("\nprep_floor(). Press Enter to return.\n");
	res = scanf("%*c");
	sprintf(msg,"test_floors() scanf() res was (%i)", res);
	log_tag("debug_log.txt","[DEBUG]",msg);

	//Random walk #1
	floor_random_walk(dbg_floor, center_x, center_y, 100, 1); // Perform 100 steps of random walk, reset floor_layout if needed.

	display_floor_layout(dbg_floor,test_win);
	//debug_print_floor_layout(dbg_floor,stdout);
	//printf("\nfloor_random_walk(). Press Enter to return.\n");
	res = scanf("%*c");
	sprintf(msg,"test_floors() scanf() res was (%i)", res);
	log_tag("debug_log.txt","[DEBUG]",msg);

	//Random walk #2
	floor_random_walk(dbg_floor, center_x, center_y, 100, 0); // Perform 100 more steps of random walk, DON'T reset floor_layout if needed.

	display_floor_layout(dbg_floor,test_win);
	//debug_print_floor_layout(dbg_floor,stdout);
	//printf("\nfloor_random_walk() second call. Press Enter to return.\n");
	res = scanf("%*c");
	sprintf(msg,"test_floors() scanf() res was (%i)", res);
	log_tag("debug_log.txt","[DEBUG]",msg);

	//Set floor explored matrix
	load_floor_explored(dbg_floor);
	display_floor_layout(dbg_floor,test_win);
	//debug_print_floor_layout(dbg_floor,stdout);
	//printf("\nprep_floor(). Press Enter to return.\n");
	res = scanf("%*c");
	sprintf(msg,"test_floors() scanf() res was (%i)", res);
	log_tag("debug_log.txt","[DEBUG]",msg);

	//Set room types
	floor_set_room_types(dbg_floor);

	display_roomclass_layout(dbg_floor,test_win);
	//debug_print_floor_layout(dbg_floor,stdout);
	//printf("\nfloor_set_room_types(). Press Enter to return.\n");
	res = scanf("%*c");
	sprintf(msg,"test_floors() scanf() res was (%i)", res);
	log_tag("debug_log.txt","[DEBUG]",msg);


	/*
	debug_print_floor_layout(dbg_floor);

	printf("\nPress Enter to see room layout.\n");
	int res = scanf("%*c");
	sprintf(msg,"debug_generic() 7 scanf() res was (%i)", res);
	log_tag("debug_log.txt","[DEBUG]",msg);
	*/
	display_floor_layout(dbg_floor,test_win);
	//debug_print_roomclass_layout(dbg_floor,stdout);
	//printf("\nShown classes. Press Enter to return.\n");
	res = scanf("%*c");
	sprintf(msg,"test_floors() scanf() res was (%i)", res);
	log_tag("debug_log.txt","[DEBUG]",msg);

	//draw_floor_view(dbg_floor,center_x,center_y,test_win);

	clear();
	refresh();

	int current_x = center_x;
	int current_y = center_y;

	while (dbg_floor->explored_area < dbg_floor->area) {
		move_update(dbg_floor, &current_x, &current_y, test_win);
	}

	free(dbg_floor);

	endwin();

	return 0;
}
