#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

#include "typedefs.h"
#include "myutils.h"

void main(int argc, char** argv) {
	white(); //Set color for always white text.
	srand(time(NULL));	
	// Randomise path 
	// 	Branches?
	//
	system("clear");
	printTitle();
	char c;
	yellow();
	printf("\n\n\n\n\t\t\t\tPRESS TO START\n\n\n");
	white();
	
	printVersion();

	scanf("%c",&c);

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
