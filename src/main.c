#include "helapordo.h"
#include "floor_tester.h"

int G_PRELOAD_ANIMATIONS_ON = 0;
int G_DEBUG_ON = 0;
int G_LOG_ON = 0;
int G_EXPERIMENTAL_ON = 0;
int G_FASTQUIT_ON = 0;
int G_GODMODE_ON = 0;
int G_DEBUG_ROOMTYPE = ENEMIES;
int G_DEBUG_ROOMTYPE_ON = 0;
int G_DEBUG_ENEMYTYPE = Zombie;
int G_DEBUG_ENEMYTYPE_ON = 0;
int G_DOTUTORIAL_ON = 0;

int GS_AUTOSAVE_ON = 1;
Gamemode GAMEMODE = Rogue;
Koliseo* default_kls = NULL;
Koliseo* temporary_kls = NULL;


int main(int argc, char** argv) {
	KOLISEO_AUTOSET_TEMP_REGIONS = 0; //Disable regions for Koliseo_Temp allocations
	//Randomise seed
	srand(time(NULL));

	setlocale(LC_ALL, "");
	// Randomise path
	// 	Branches?
	//
	//test_floors();
	#ifndef _WIN32
	gameloop(argc, argv);
	#else
	FILE* debug_file = NULL;
	char path_to_debug_file[600];
	char static_path[500];
	// Set static_path value to the correct static dir path
	resolve_staticPath(static_path);

	//Truncate "debug_log.txt"
	sprintf(path_to_debug_file,"%s\\%s",static_path,"debug_log.txt");
	debug_file = fopen(path_to_debug_file, "w");
	if (!debug_file) {
		endwin(); //TODO: Can/should we check if we have to do this only in curses mode?
		fprintf(stderr,"[ERROR]    Can't open debug logfile (%s\\debug_log.txt).\n", static_path);
		exit(EXIT_FAILURE);
	}
	fprintf(debug_file,"[DEBUGLOG]    --New game--  \n");
	fprintf(debug_file,"[DEBUG]    --Closing header for new game.--  \n");
	fclose(debug_file);

	char* whoami;
  	(whoami = strrchr(argv[0], '\\')) ? ++whoami : (whoami = argv[0]);
	printTitle();
	printf("\n\n\n\n\t\t\t\tSTART\n\n");
	printf("\t\t\t\t\t\t");
        printFormattedVersion(whoami);
	printf("\n\nThe Windows build of \"helapordo\" is very much WIP.\n\n");
	printf("\n  Press Enter to proceed.\n");
	scanf("%*c");
	system("cls");
	printGlobVars();
	printWin_EnvVars();
	printf("\n\n  Press Enter to demo a minimal rogue floor.\n");
	printf("  Quit with Ctrl+C, or explore enough of the map.\n\n");
	printf("  You may need to spam Enter or the arrow keys.\n\n");
	scanf("%*c");
	test_floors();
	return 1;
	#endif
	// 	Monsters
	// 	Leveling
	// 	Abilities?
	// 	Items?
	// 	Turns

	// Settings
	//
	// Saves
	//
	// Victory / Death
}
