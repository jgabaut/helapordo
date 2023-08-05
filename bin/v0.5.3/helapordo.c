#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

#include "typedefs.h"
#include "myutils.h"

#define VERSION "v0.5.3"

int main(int argc, char** argv) {

	white(); //Set color for always white text.
	
	//Randomise seed
	srand(time(NULL));

	system("clear");
	printTitle();
	char c;
	yellow();
	printf("\n\n\n\n\t\t\tPRESS ENTER TO START\n\n\n");
	white();
	
	printVersion(VERSION);

	scanf("%c",&c);

	// Randomise path 
	// 	Branches?
	//
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
