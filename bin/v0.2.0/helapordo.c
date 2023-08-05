#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "typedefs.h"
#include "myutils.h"

#define MAXLENGTH 100
#define MAXLUCK 25

void quit(Fighter* p) {
	system("clear");
	printf("\n\n\tTHANKS 4 PLAYING\n");
	printStats(p);
	printf("\n");
	exit(1);
}

Path* randomise_path(int seed){
	Path* p = (Path*) malloc(sizeof(Path));
	srand(seed);

	p->length = (rand() % MAXLENGTH ) + 1;
	p->luck = (rand() % MAXLUCK ) + 1;
	p->prize = 15 / p->luck * (rand() % 150) + 500;

	return p;
}

int room(int index, Path* p, Fighter* player) {
	printf("\n\n\tYou are now in room %i.\n",index);
	int enemies = (rand() % 6) + 1 ;
	
	printf("\nThere are %i enemies.\n",enemies);
	int fightStatus = NO_DMG;
	for (int i = 1; i < enemies ;) {
		Enemy* e = (Enemy *) malloc (sizeof(Enemy));
		//Randomise enemy class
		e->class = rand() % (ENEMYCLASSESMAX + 1);
		initEnemyStats(e);
		printf("\n\tEnemy #%i:\t%s\n",i,stringFromEClass(e->class));
		
		printEStats(e);
		
		//Fight
			
		int choice = 999;

		while (!( fightStatus == DEATH || fightStatus == KILL_DONE || choice == QUIT ) ){

			choice = getTurnChoice();
			if (choice == FIGHT) {
				fightStatus = fight(player,e);
			} else if (choice == QUIT) {
				quit(player);
			}
			printf("\n\n\t%s's hp:\t%i\tYour hp:\t%i\n",stringFromEClass(e->class),e->hp,player->hp);
		
			//Lost battle
			if (fightStatus == DEATH) {
				death(player);
			} else if (fightStatus == KILL_DONE) {
			//Win, get xp and free memory from enemy
				giveXp(player,e);
				free(e);
			//Go to next battle
				i++;
printf("\n\n%i enemies left in room %i.",(enemies-i),index);
				fightStatus=NO_DMG;
				break;
			}
		}
	}

	return fightStatus;
}

void main(int argc, char** argv) {
	white(); //Set color for always white text.
	srand(time(NULL));	
	// Randomise path 
	// 	Branches?
	//

	Fighter* player = (Fighter *) malloc(sizeof(Fighter));
	
	Path* path = randomise_path(rand());
	
	getParams(argc, argv, player);
	initPlayerStats(player,path);
	printf("\033[0;31m");
		printStats(player);
	printf("\033[0;37m");

	// Fights
	//
	
	int roomsDone = 0;

	while (roomsDone < path->length) {
		int res = room(roomsDone,path,player);
		if (res == DEATH) {
			death(player);
		}
		else {
			roomsDone++;
		}

		//Flush the terminal
		system("clear");
		printStats(player);
	}

	printf("\n\n\tYOU WON!\n\n");

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
