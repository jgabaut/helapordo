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
char* G_DEBUG_ROOMTYPE_ARG = NULL;
int G_DEBUG_ENEMYTYPE = Zombie;
int G_DEBUG_ENEMYTYPE_ON = 0;
char* G_DEBUG_ENEMYTYPE_ARG = NULL;

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
	gameloop_Win(argc, argv);
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

	return 1;
}
