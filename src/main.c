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

	// Randomise path
	// 	Branches?
	//
	setlocale(LC_ALL, "");
	//test_floors();
	gameloop(argc, argv);

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
