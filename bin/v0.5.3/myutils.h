static callback_void_t callback_func_ptrs[SPECIALSMAX];
static callback_void_t callback_counter_ptrs[COUNTERSMAX];

/**
 * Takes a specialMove and a callback_void_t pointer function.
 * Registers the pointer to the function pointer array for special moves callback.
 * @see specialMove
 * @see SpecialSlot
 * @see GET_CALLBACK()
 * @see REGISTER_CALLBACK()
 * @param move A specialMove.
 * @param ptr A pointer to function of type callback_void_t.
 * @return No return
 */
void register_callback(specialMove move, callback_void_t ptr) {
	callback_func_ptrs[move] = ptr;
}

/**
 * Takes an integer and a callback_void_t pointer function.
 * Not working as of v0.5.2.
 * Registers the pointer to the function pointer array for counter callback.
 * @see Turncounter
 * @param index An integer..
 * @param ptr A pointer to function of type callback_void_t.
 * @return No return
 */
void register_counter_callback(int index, callback_void_t ptr) {
	callback_counter_ptrs[index] = ptr;
}

/**
 * Prints an ASCII escape code that makes subsequent output a thick red color.
 * @return No return
 */
void red() {
	printf("\033[1;31m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light red color.
 * @return No return
 */
void lightRed() {
	printf("\033[0;31m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a thick white color.
 * @return No return
 */
void strongWhite() {
	printf("\033[1;37m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light white color.
 * @return No return
 */
void white() {
	printf("\033[0;37m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a thick green color.
 * @return No return
 */
void green() {
	printf("\033[1;32m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light green color.
 * @return No return
 */
void lightGreen() {
	printf("\033[0;32m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a thick yellow color.
 * @return No return
 */
void yellow() {
	printf("\033[1;33m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light yellow color.
 * @return No return
 */
void lightYellow() {
	printf("\033[0;33m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a thick blue color.
 * @return No return
 */
void blue() {
	printf("\033[1;34m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light blue color.
 * @return No return
 */
void lightBlue() {
	printf("\033[0;34m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a thick purple color.
 * @return No return
 */
void purple() {
	printf("\033[1;35m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light purple color.
 * @return No return
 */
void lightPurple() {
	printf("\033[0;35m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a thick cyan color.
 * @return No return
 */
void cyan() {
	printf("\033[1;36m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light cyan color.
 * @return No return
 */
void lightCyan() {
	printf("\033[0;36m");
}

/**
 * Sleeps t seconds and then flushes stdin.
 * The tcflush() function comes from termios and may not be supported on all platforms.
 * @param t A float for how many seconds to sleep.
 * @return No return
 */
void screenTime(float t) {
	sleep(t);

	//Avoid leftovers from stdin be picked up while sleeping
	tcflush(0,TCIFLUSH);
}

/**
 * Prints an hardcoded title screen.
 * @return No return
 */
void printTitle(){
	red();
	printf("\n\n\n\n\n\n");
	printf("          __  __  ____   __     __   ______    ___    ____    ______  ______  \n");
	printf("         / / / / / __/  / /    / /  / ____/   / __,  / __ ,  / __  / / ____/ \n");
	printf("        / /_/ / / /_   / /    / /  / /___    / /    / / / / / /_/ / / /___  \n");
	printf("       / __  / / __/  / /    / /   ,___  ,  / /    / ,_/ / / ____/ / ____/ \n");
	printf("      / / / / / /_   / /__  / /__  ___/  / / /__  / __  / / /     / /___  \n");
	printf("     /_/ /_/ /____/ /____/ /____/ /_____/  ,___/ /_/ /_/ /_/     /_____/ \n");
	white();
}

/**
 * Prints version string.
 * @param v The version string.
 * @return No return
 */
void printVersion(char* v){
	printf("\t\t\t\t\t\t\t\t%s\n",v);
}

/**
 * Prints correct argument syntax for command line invocation.
 * @return No return
 */
void usage() {
	perror("\nUsage:\t./battlesim [name] [class]");
	exit(1);
}

/**
 * Takes a fighterStatus and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterStatus
 * @param s The status.
 * @return String corresponding to the status.
 */
static inline char* stringFromStatus(fighterStatus s) {
	static char* strings[] = {
		"Normal",
		"Poison",
		"Frozen",
		"Burn",
		"Weak",
		"Strong"
	};

	return strings[s];
}

/**
 * Takes a fighterClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @param f The class.
 * @return String corresponding to the class.
 */
static inline char* stringFromClass(fighterClass f) {

	return classesstrings[f];
}

/**
 * Takes a enemyClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see enemyClass
 * @param e The class.
 * @return String corresponding to the class.
 */
static inline char* stringFromEClass(enemyClass e) {

	return classenemystrings[e];
};

/**
 * Takes a integer and returns the corresponding consumable string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see consumableClass
 * @param c The integer/consumableClass.
 * @return String corresponding to the consumable.
 */
static inline char* stringFromConsumables(int c) {

	return consumablestrings[c];
}

/**
 * Takes a integer and returns the corresponding equip string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see equipClass
 * @param e The integer/equipClass.
 * @return String corresponding to the equip.
 */
static inline char* stringFromEquips(int e) {

	return equipstrings[e];
}

/**
 * Takes a integer and returns the corresponding equipzone string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see Equipzone
 * @param z The integer/Equipzone.
 * @return String corresponding to the equipzone.
 */
static inline char* stringFromEquipzones(int z) {

	return equipzonestrings[z];
}

/**
 * Takes a integer and returns the corresponding quality string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see quality
 * @param q The integer/quality
 * @return String corresponding to the quality.
 */
static inline char* stringFromQuality(int q) {

	return qualitytrings[q];
}

/**
 * Takes a integer and returns the corresponding artifact string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see artifactClass
 * @param a The integer/artifactClass.
 * @return String corresponding to the artifact.
 */
static inline char* stringFromArtifacts(int a) {

	return artifactstrings[a];
}

/**
 * Takes a fighterClass and a specialMove and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @see specialMove
 * @param class The fighterClass.
 * @param move The specialMove.
 * @return String corresponding to the special move.
 */
static inline char* stringFromSpecial(fighterClass class, specialMove move) {
	return specialsstrings[class][(move % (SPECIALSMAX + 1))];
}

/**
 * Takes a fighterClass and a specialMove and returns the corresponding cost by the inner array position, as an integer.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @see specialMove
 * @param class The fighterClass.
 * @param move The specialMove.
 * @return int Cost of the corresponding special move.
 */
static inline int costFromSpecial(fighterClass class, specialMove move) {
	return specialscosts[class][(move % (SPECIALSMAX + 1)) ];
}

/**
 * Takes a integer and returns the corresponding perk name string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see perkClass
 * @see Perk
 * @param e The integer/perkClass.
 * @return String corresponding to the perk name.
 */
static inline char* nameStringFromPerk(int p) {

	return perksnamestrings[p];
}

/**
 * Takes a integer and returns the corresponding perk desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see perkClass
 * @see Perk
 * @param e The integer/perkClass.
 * @return String corresponding to the perk desc.
 */
static inline char* descStringFromPerk(int p) {

	return perksdescstrings[p];
}

/**
 * Takes a Fighter pointer and sets its name value to the string provided on stdin.
 * @param player The pointer whose name value will be set.
 * @return No return.
 */
void scanName(Fighter* player) {
	char name[25];
	int f;
	do {
		printf("\n\n\tWhat's your name?\n\n");
		f = scanf("%s",name);
	} while (f >= 24 || f <= 0 );
	strcpy(player->name,name);
}

/**
 * Prints all the fighterClass values as integers and as strings.
 * @see fighterClass
 * @return No return.
 */
void printClasses() {
	printf("[");
	for (int i=0; i<=CLASSESMAX; i++) {
		printf("\t%d)\t%s",i,stringFromClass(i));
		if (i!=CLASSESMAX) { printf("\n"); }
	};
	printf("\t]\n");
}

/**
 * Asks for an integer on stdin and returns it if it's a valid fighterClass index.
 * @see fighterClass
 * @see CLASSESMAX
 * @return The selected integer, negative for unvalid choices.
 */
int scanClass() {
	int pick=-1;
	char c;
	while (pick < 0 || pick > CLASSESMAX ) {
		printf("\nPick a class: (0-%i)\n",CLASSESMAX);
		scanf("%i",&pick);
		scanf("%c",&c);
	};
	return pick;
}

/**
 * Takes a Fighter pointer and sets the class value after asking the user for input.
 * @see fighterClass
 * @see scanClass()
 * @param player The pointer whose class value will be set.
 * @return No return.
 */
void pickClass(Fighter* player) {
	int pick=-1;
	do {
		system("clear");
		printf("\nPick a class.\n");
		printClasses();
		pick = scanClass();
	} while (pick<0);

	player->class = pick;
	green();
	printf("\n\n\tClass: %s\n\n",stringFromClass(player->class));
	white();
}

/**
 * Takes a Fighter pointer and sets the name value after asking the user for input.
 * @see fighterClass
 * @see scanName()
 * @param player The pointer whose name value will be set.
 * @return No return.
 */
void pickName(Fighter* player) {
	scanName(player);
	red();
	printf("\n\n\tName: %s\n\n",player->name);
	white();
}

//Status effect functions

/**
 * Takes a Fighter pointer and resets its status value to Normal.
 * @see Fighter
 * @see fighterStatus
 * @param f The pointer whose status value will be reset.
 * @return No return.
 */
void resetFighterStatus(Fighter* f) {
	f->status = Normal;
}

/**
 * Takes a Enemy pointer and resets its status value to Normal.
 * @see Enemy
 * @see fighterStatus
 * @param e The pointer whose status value will be reset.
 * @return No return.
 */
void resetEnemyStatus(Enemy* e) {
	e->status = Normal;
}


/**
 * Takes a fighterStatus and returns the corresponding effect_fun pointer for the function relating to the status.
 * @see fighterStatus
 * @see effect_fun
 * @param status The fighterStatus that the caller needs to match to a effect_fun pointer.
 * @return The effect_fun pointer related to the status.
 */
effect_fun getStatusCounterFun(fighterStatus status) {
	switch (status) {
		//case Normal: {
		default: {
			return &resetFighterStatus;
		}
		break;
	};
};

/**
 * Takes a fighterStatus and returns the corresponding effect_e_fun pointer for the function relating to the status.
 * @see fighterStatus
 * @see effect_e_fun
 * @param status The fighterStatus that the caller needs to match to a effect_e_fun pointer.
 * @return The effect_e_fun pointer related to the status.
 */
effect_e_fun getStatusCounterEnemyFun(fighterStatus status) {
	switch (status) {
		//case Normal: {
		default: {
			return &resetEnemyStatus;
		}
		break;
	};
};

//Stat boost functions

/**
 * Takes a Fighter pointer and an integer, used to set the turnboost_atk value at the pointer.
 * @see Fighter
 * @see Turncounter
 * @param f The Fighter pointer whose turnboost_atk value will be set.
 * @param boost The value that will be set.
 * @return No return.
 */
void set_turnboost_atk(Fighter* f,int boost) {
	//Straight addition of the boost
	f->turnboost_atk = boost;
};

/**
 * Takes a Fighter pointer and an integer, used to set the turnboost_def value at the pointer.
 * @see Fighter
 * @see Turncounter
 * @param f The Fighter pointer whose turnboost_def value will be set.
 * @param boost The value that will be set.
 * @return No return.
 */
void set_turnboost_def(Fighter* f,int boost) {
	//Straight addition of the boost
	f->turnboost_def = boost;
};

/**
 * Takes a Fighter pointer and an integer, used to set the turnboost_vel value at the pointer.
 * @see Fighter
 * @see Turncounter
 * @param f The Fighter pointer whose turnboost_vel value will be set.
 * @param boost The value that will be set.
 * @return No return.
 */
void set_turnboost_vel(Fighter* f,int boost) {
	//Straight addition of the boost
	f->turnboost_vel = boost;
};

/**
 * Takes a Fighter pointer and an integer, used to set the turnboost_enr value at the pointer.
 * @see Fighter
 * @see Turncounter
 * @param f The Fighter pointer whose turnboost_enr value will be set.
 * @param boost The value that will be set.
 * @return No return.
 */
void set_turnboost_enr(Fighter* f,int boost) {
	//Straight addition of the boost
	f->turnboost_enr = boost;
};

/**
 * Takes a Enemy pointer and an integer, used to set the turnboost_atk value at the pointer.
 * @see Enemy
 * @see Turncounter
 * @param e The Enemy pointer whose turnboost_enr value will be set.
 * @param boost The value that will be set.
 * @return No return.
 */
void set_enemy_turnboost_atk(Enemy* e,int boost) {
	//Straight addition of the boost
	e->turnboost_atk = boost;
};

/**
 * Takes a Enemy pointer and an integer, used to set the turnboost_def value at the pointer.
 * @see Enemy
 * @see Turncounter
 * @param e The Enemy pointer whose turnboost_def value will be set.
 * @param boost The value that will be set.
 * @return No return.
 */
void set_enemy_turnboost_def(Enemy* e,int boost) {
	//Straight addition of the boost
	e->turnboost_def = boost;
};

/**
 * Takes a Enemy pointer and an integer, used to set the turnboost_vel value at the pointer.
 * @see Enemy
 * @see Turncounter
 * @param e The Enemy pointer whose turnboost_vel value will be set.
 * @param boost The value that will be set.
 * @return No return.
 */
void set_enemy_turnboost_vel(Enemy* e,int boost) {
	//Straight addition of the boost
	e->turnboost_vel = boost;
};

/**
 * Takes a Enemy pointer and an integer, used to set the turnboost_enr value at the pointer.
 * @see Enemy
 * @see Turncounter
 * @param e The Enemy pointer whose turnboost_enr value will be set.
 * @param boost The value that will be set.
 * @return No return.
 */
void set_enemy_turnboost_enr(Enemy* e,int boost) {
	//Straight addition of the boost
	e->turnboost_enr = boost;
};

//Counter callback getters

/**
 * Takes a Stat and returns the corresponding boost_fun pointer for the function relating to the stat.
 * @see Stat
 * @see boost_fun
 * @param s The Stat that the caller needs to match to a boost_fun pointer.
 * @return The boost_fun pointer related to the stat.
 */
boost_fun getStatBoostCounterFun(Stat s) {

	switch (s) {
		case ATK: {
			return &set_turnboost_atk;
		}
		break;
		case DEF: {
			return &set_turnboost_def;
		}
		break;
		case VEL: {
			return &set_turnboost_vel;
		}
		break;
		case ENR: {
			return &set_turnboost_enr;
		}
		break;
	};
}

/**
 * Takes a Stat and returns the corresponding boost_e_fun pointer for the function relating to the stat.
 * @see Stat
 * @see boost_e_fun
 * @param s The Stat that the caller needs to match to a boost_e_fun pointer.
 * @return The boost_e_fun pointer related to the stat.
 */
boost_e_fun getStatBoostCounterEnemyFun(Stat s) {

	switch (s) {
		case ATK: {
			return &set_enemy_turnboost_atk;
		}
		break;
		case DEF: {
			return &set_enemy_turnboost_def;
		}
		break;
		case VEL: {
			return &set_enemy_turnboost_vel;
		}
		break;
		case ENR: {
			return &set_enemy_turnboost_enr;
		}
		break;
	};
}
	
/**
 * Takes a Fighter pointer and prepares its perks field by allocating all the Perk accounted in perkClass.
 * Perks are initialised using the ordering of perkClass.
 * @see Fighter
 * @see Perk
 * @see perkClass
 * @see PERKSMAX
 * @param f The Fighter pointer whose perks field will be initialised.
 * @return No return.
 */
void initPerks(Fighter* f) {

	f->perksCount = 0;
	//Ordering of i corresponds to perksClass enum	
	int total = (PERKSMAX+1);
	for (int i = 0; i < total; i++) {
		Perk *p = (Perk*) malloc(sizeof(Perk));
		p->class = i;
		char* name = nameStringFromPerk(i);
		char* desc = descStringFromPerk(i);
		

		//p->name = name; //(char*)malloc(sizeof(name));
		strcpy(p->name,name);
		
		//p->desc = (char*)malloc(sizeof(desc));
		strcpy(p->desc,desc);
		
		p->innerValue = 0;
		
		f->perks[i] = p;
	}
}	
		
/**
 * Takes a Equip and a Fighter pointers.
 * Iterates over the equip's perks and adds them to the fighter perks.
 * @see Perk
 * @see Equip
 * @see Fighter
 * @param e An Equip pointer.
 * @param f A Fighter pointer.
 * @return No return
 */
void applyEquipPerks(Equip* e, Fighter* f) {

	for (int i = 0; i < (e->perksCount); i++) {
		Perk* p = e->perks[i];

		Perk* fighterPerk = f->perks[p->class];
		fighterPerk->innerValue += 1;
	}
};

/**
 * Takes a Equip and a Fighter pointers.
 * Iterates over the equip's perks and removes them to the fighter perks.
 * @see Perk
 * @see Equip
 * @see Fighter
 * @param e An Equip pointer.
 * @param f A Fighter pointer.
 * @return No return
 */
void removeEquipPerks(Equip* e, Fighter* f) {

	for (int i = 0; i < (e->perksCount); i++) {
		Perk* p = e->perks[i];

		Perk* fighterPerk = f->perks[p->class];
		fighterPerk->innerValue -= 1;
	}
};

/**
 * Takes a Figher pointer and prints all of its active perks formatted.
 * @see Fighter
 * @see Perk
 * @see Equip
 * @param f The Fighter pointer with perks to print.
 * @return No return.
 */
void printActivePerks(Fighter* f) {
	system("clear");
	yellow();
	printf("\tActive Perks:\n\n");
	white();	
	for (int i = 0; i < (PERKSMAX +1); i++) {
		Perk* p = f->perks[i];

		if (p->innerValue > 0) {
			lightCyan();
			printf("\t%i  ",p->innerValue);
			cyan();
			printf("%s\t",nameStringFromPerk(p->class));
			lightYellow();
			printf("\"%s\"\n",descStringFromPerk(p->class));
			white();
		}
	}

	screenTime(1.5);
};

//Counter functions 

/**
 * Takes a Fighter pointer and prepares its counters field by allocating all the Turncounter accounted in counterIndexes.
 * Counters are initialised using the ordering of counterIndexes, primarily for distinguinsing what kind of function pointer will be registered in the counter.
 * For a given Counter, only the correct pointer field is assigned and the others are left untouched.
 * @see Fighter
 * @see Turncounter
 * @see Countertype
 * @see COUNTERSMAX
 * @see counterIndexes
 * @see getStatBoostCounterFun()
 * @see getStatusCounterFun()
 * @param f The Fighter pointer whose counters field will be initialised.
 * @return No return.
 */
void initCounters(Fighter* f){
	//Ordering of i corresponds to counterIndexes enum	
	int i = 0 ; //Counter for all counters	
	int total = (COUNTERSMAX+1);
	for (int i = 0; i < total; i++) {
		Turncounter* c = (Turncounter*)malloc(sizeof(Turncounter));
		
		//First, prepare counters for statuses
		if (i < STATUSMAX+1 ) {
			c->desc = (char*)malloc(sizeof(stringFromStatus(i)));
			strcpy(c->desc,stringFromStatus(i));

			c->effect_fun = getStatusCounterFun(i);
			c->type = CNT_STATUS;

			//Register function for the specific status counter
			//
			//REGISTER_COUNTER_CALLBACK(i,resetFighterStatus);
			//
			//Why does uncommenting this cause problems to special moves callback?
			//More specific, ONLY MAGE MOVES
			//debug
			//printf("%s",stringFromStatus(j));
		} else { //Then, counters for boosts to (all?) stats
			
			switch(i) {
				case TURNBOOST_ATK: {
					c->desc = (char*)malloc(sizeof("ATK boost"));
					strcpy(c->desc,"ATK boost");
					
					c->boost_fun = getStatBoostCounterFun(ATK);
					c->type = CNT_ATKBOOST;
				}
				break;
				case TURNBOOST_DEF: {
					c->desc = (char*)malloc(sizeof("DEF boost"));
					strcpy(c->desc,"DEF boost");
					
					c->boost_fun = getStatBoostCounterFun(DEF);
					c->type = CNT_DEFBOOST;
				}
				break;
				case TURNBOOST_VEL: {
					c->desc = (char*)malloc(sizeof("VEL boost"));
					strcpy(c->desc,"VEL boost");
					
					c->boost_fun = getStatBoostCounterFun(VEL);
					c->type = CNT_VELBOOST;
				}
				break;
				case TURNBOOST_ENR: {
					c->desc = (char*)malloc(sizeof("ENR boost"));
					strcpy(c->desc,"ENR boost");
					
					c->boost_fun = getStatBoostCounterFun(ENR);
					c->type = CNT_ENRBOOST;
				}
				break;
				default: {
					printf("\n\nERROR\n\n");
				}
			}	
		} //End else
		c->count = 0;
		c->innerValue = 0;
		f->counters[i] = (struct Turncounter*) c;
	};
};	

/**
 * Takes a Enemy pointer and prepares its counters field by allocating all the Turncounter accounted in counterIndexes.
 * Counters are initialised using the ordering of counterIndexes, primarily for distinguinsing what kind of function pointer will be registered in the counter.
 * For a given Counter, only the correct pointer field is assigned and the others are left untouched.
 * @see Enemy
 * @see Turncounter
 * @see Countertype
 * @see COUNTERSMAX
 * @see counterIndexes
 * @see getStatBoostCounterEnemyFun()
 * @see getStatusCounterEnemyFun()
 * @param e The Enemy pointer whose counters field will be initialised.
 * @return No return.
 */
void initECounters(Enemy* e){
	//Ordering of i corresponds to counterIndexes enum	
	int i = 0 ; //Counter for all counters	
	int total = (COUNTERSMAX+1);
	for (int i = 0; i < total; i++) {
		Turncounter* c = (Turncounter*)malloc(sizeof(Turncounter));
		
		//First, prepare counters for statuses
		if (i < STATUSMAX+1 ) {
			c->desc = (char*)malloc(sizeof(stringFromStatus(i)));
			strcpy(c->desc,stringFromStatus(i));

			c->effect_e_fun = getStatusCounterEnemyFun(i);
			c->type = CNT_STATUS;

		} else { //Then, counters for boosts to (all?) stats
			
			switch(i) {
				case TURNBOOST_ATK: {
					c->desc = (char*)malloc(sizeof("ATK boost"));
					strcpy(c->desc,"ATK boost");
					
					c->boost_e_fun = getStatBoostCounterEnemyFun(ATK);
					c->type = CNT_ATKBOOST;
				}
				break;
				case TURNBOOST_DEF: {
					c->desc = (char*)malloc(sizeof("DEF boost"));
					strcpy(c->desc,"DEF boost");
					
					c->boost_e_fun = getStatBoostCounterEnemyFun(DEF);
					c->type = CNT_DEFBOOST;
				}
				break;
				case TURNBOOST_VEL: {
					c->desc = (char*)malloc(sizeof("VEL boost"));
					strcpy(c->desc,"VEL boost");
					
					c->boost_e_fun = getStatBoostCounterEnemyFun(VEL);
					c->type = CNT_VELBOOST;
				}
				break;
				case TURNBOOST_ENR: {
					c->desc = (char*)malloc(sizeof("ENR boost"));
					strcpy(c->desc,"ENR boost");
					
					c->boost_e_fun = getStatBoostCounterEnemyFun(ENR);
					c->type = CNT_ENRBOOST;
				}
				break;
				default: {
					printf("\n\nERROR\n\n");
				}
			}	
		} //End else
		c->count = 0;
		c->innerValue = 0;
		e->counters[i] = (struct Turncounter*) c;
	};
};	

/**
 * Takes a Turncounter array.
 * For every Turncounter in the array, the values of count, innerVal, type and all the function pointers fields are printed.
 * @see Turncounter
 * @param counters The Turncounter array to be printed.
 * @return No return.
 */
void printCounters(Turncounter* counters[]) {
		lightBlue();
	for (int i = 0; i < (COUNTERSMAX + 1) ; i++) {
		Turncounter *c = counters[i];
		printf("\tCount: %i\t%s\tInnerVal: %i\tType: %i,PF %i,PE %i,FB %i,EB %i\n",c->count,c->desc,c->innerValue,c->type,c->effect_fun,c->effect_e_fun,c->boost_fun, c->boost_e_fun);
	};
		white();
		screenTime(1.5);
};

/**
 * Takes a Turncounter array, an integer, a Fighter pointer and an Enemy pointer.
 * For every Turncounter in the array count value is checked, and when it's equal to 1, the function pointer relevant to the type value of the Counter is called. Depending on the isEnemy input value, the function call will be for the Fighter or Enemy version (and the according pointer from the input will be passed to the called function).
 * When the count value 0 Counters are considered inactive, so when the count value is not 1, it is lowered by 1 if it's positive.
 * @see Fighter
 * @see Enemy
 * @see Turncounter
 * @see Countertype
 * @see COUNTERSMAX
 * @see counterIndexes
 * @param counters The Turncounter array to be updated.
 * @param isEnemy Dictates if the check is done for an Enemy or a Fighter.
 * @param f The Fighter pointer whose counters field will be updated if isEnemy is false.
 * @param e The Enemy pointer whose counters field will be updated if isEnemy is true.
 * @return No return.
 */
void updateCounters(Turncounter* counters[], int isEnemy, Fighter* f, Enemy* e){
	for (int i=0; i < COUNTERSMAX + 1; i++) {

		Turncounter* c = counters[i]; 
		if (c->count == 1) { //Counter is about to expire so we call the correct function:
			switch (c->type) {
				case CNT_STATUS: { //Callback for status counters
					
					if (!isEnemy) {
						(c->effect_fun)(f);
						green();
						printf("\t%s status returned to normal.\n",f->name);
						white();
					} else { //Enemy function
						(c->effect_e_fun)(e);
						lightRed();
						printf("\t%s status returned to normal.\n",stringFromEClass(e->class));
						white();
					}
				}
				break;
				case CNT_ATKBOOST: case CNT_DEFBOOST: case CNT_VELBOOST: case CNT_ENRBOOST: { //Callback for stat boosts
					if (!isEnemy) {
						(c->boost_fun)(f,0); //Invoke ~setPermBoost(STAT)~ with value 0
					} else {
						//Not yet implemented	
					}
				}
				break;
			}	

			//Set counter to zero
			c->count = 0;
			c->innerValue = 0;
		} else if (c->count > 1) { //We simply advance the count
			c->count -= 1;
		}	
	};	
};

/**
 * Takes a Turncounter pointer and an integer.
 * If the count value at the pointer is 0 (counter is inactive), the turns valueis assigned.
 * @see Turncounter
 * @param c The Turncounter whose count value will be set.
 * @param turns The value to be assigned.
 * @return No return.
 */
void setCounter(Turncounter* c,int turns) {
	
	if (c->count == 0) { // Counter is NOT already active
		c->count = turns;
	} else {
		//Handle counters already activ
	}
};

//Special moves functions

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * The enemy pointer hp value is reduced by half of the fighter atk value, then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void knightSpecial_Slash(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	
	printf("\t\t%s strikes %s.\n\n",f->name,stringFromEClass(e->class));
	
	e->hp -= f->atk * 0.5; //Dmg the enemy
	f->energy -= costFromSpecial(f->class,KSlash);        //Reduce fighter energy
	lightGreen();
	printf("\t\t%s was deeply hurt.\n\n",stringFromEClass(e->class));
	white();
	screenTime(1.5);
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * The fighter pointer def value is increased by 10, then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void knightSpecial_Cover(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	printf("\t\t%s uses his shield to cover.\n\n",f->name);

	f->def += 10;	//Raise fighter def
	f->energy -= costFromSpecial(f->class,KCover);	//Reduce fighter energy
	
	lightGreen();
	printf("\t\t+10 DEF\n\n");
	white();
	screenTime(1);
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * If the fighter pointer def value is at least 11; the def, vel and atk values are respectfully decreased by 10, increased by 10, increased by 2. Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void knightSpecial_Armordrop(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if ( f->def >= 11 ) {
		printf("\t\t%s drops his armor to strike faster.\n\n",f->name);
		f->def -= 10;
		f->vel += 10; 
		f->atk += 2;
		
		f->energy -= costFromSpecial(f->class,KArmordrop);        //Reduce fighter energy
		
		lightGreen();
		printf("\t\t+10 VEL\t+2 ATK\t");
		lightRed();
		printf("-10 DEF\n\n");
		white();
		screenTime(1);
	} else {
		lightRed();
		printf("\t\t%s is too exposed to drop his armor.\n\n",f->name);
		white();
		screenTime(1);
	}
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * If the fighter pointer hp value is at least 3 and the enemy hp value is at least 6; they are respectfully lowered by 2 and 5, and the atk value of the fighter is increased by 9. Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void knightSpecial_Berserk(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if (f->hp > 2 && e->hp > 5) { //Check if both enemy and you can lose at least 2 hp...
			
		printf("\t\t%s is getting mad at %s!\n",f->name,stringFromEClass(e->class));
		f->atk += 9;
	        e->hp -= 5; //Deal damage	
	        f->hp -= 2; //Deal damage	
		f->energy -= costFromSpecial(f->class,KBerserk);        //Reduce fighter energy

		lightGreen();
		printf("\t\t%s was stunned and hurt!\t\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	} else if (e->hp <= 2) {
		lightRed();
		printf("\t\t%s's is weak and not impressionable.\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	} else if (f->hp <= 2) {
		lightRed();
		printf("\t\t%s's is too weak.\n\n",f->name);
		white();
		screenTime(1);
	}
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * If the enemy pointer hp value is at least 2 it gets halved, then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void archerSpecial_Headshot(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if (e->hp > 1) { //Check if enemy has some hp
			
		printf("\t\t%s aims an arrow at %s's head!\n",f->name,stringFromEClass(e->class));
		e->hp /= 2; //Deal damage	
		f->energy -= costFromSpecial(f->class,AHeadshot);        //Reduce fighter energy

		lightGreen();
		printf("\t\t%s got headshot!\t\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	} else {
		lightRed();
		printf("\t\t%s's is weak already.\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	}
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * If the fighter pointer atk value is at least 11, it increases by 8, then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void archerSpecial_Quivercheck(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	printf("\t\t%s checks its quiver for more arrows.\n",f->name);
	if (f->atk > 10) { //Check if atk is high enough
			f->atk += 8; //Gain atk	
		f->energy -= costFromSpecial(f->class,AQuivercheck);        //Reduce fighter energy

		lightGreen();
		printf("\t\t%s found stronger arrows inside itself.\t\n\n",f->name);
		white();
		screenTime(1);
	} else {
		lightRed();
		printf("\t\t%s can't help but feel weak.\n\n",f->name);
		white();
		screenTime(1);
	}
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * If the enemy pointer hp value is at least 2, its status value is set to Poison. The count value of enemy's Turncounter related to Poison status gets set to 4. If the enemy hp value is at least 4, it decreases by 3.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void archerSpecial_Poisonshot(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if (e->hp > 1) { //Check if enemy has some hp
		if (e->hp >= 4) {
			e->hp -= 3;
		}
		
		printf("\t\t%s aims a poisoned arrow at %s's!\n",f->name,stringFromEClass(e->class));
		
		e->status = Poison; //Set status to Poison. May need change to manage multiple statuses active at once
		setCounter((Turncounter *)e->counters[POISON],4); //Give 4 turns of Poison status
		f->energy -= costFromSpecial(f->class,APoisonshot);        //Reduce fighter energy

		lightGreen();
		printf("\t\t%s was hit and poisoned!\t\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	} else {
		lightRed();
		printf("\t\t%s's is weak already.\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	}
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * If the enemy pointer hp value is at least 2, its status value is set to Burned. The count value of enemy's Turncounter related to Burned status gets set to 3. If the enemy hp value is at least 5, it decreases by 4.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void archerSpecial_Fireshot(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if (e->hp > 1) { //Check if enemy has some hp
		if (e->hp >= 5) {
			e->hp -= 4;
		}
		
		printf("\t\t%s lights an arrow on fire and shoots at %s's!\n",f->name,stringFromEClass(e->class));
		
		e->status = Burned; //Set status to Burned. May need change to manage multiple statuses active at once
		setCounter((Turncounter *)e->counters[BURNED],3); //Give 3 turns of Burned status
		f->energy -= costFromSpecial(f->class,AFireshot);        //Reduce fighter energy

		lightGreen();
		printf("\t\t%s was hit and set on fire!\t\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	} else {
		lightRed();
		printf("\t\t%s's is weak already.\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	}
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * If the fighter pointer luck value is less than MAXPLAYERLUCK - 6, it increases by 5.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void mageSpecial_Fatewarp(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if ( f->luck < MAXPLAYERLUCK - 6 ) {
		printf("\t\t%s warps fate to fulfill its destiny.\n\n",f->name);
		f->luck += 5;

		f->energy -= costFromSpecial(f->class,MFatewarp);        //Reduce fighter energy

		lightGreen();
		printf("\t\t+5 LUCK\t\n\n");
		white();
		screenTime(1);
	} else {
		lightRed();
		printf("\t\t%s's fate is bright already.\n\n",f->name);
		white();
		screenTime(1);
	}
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * The fighter pointer atk value increases by 5.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void mageSpecial_Powerup(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	printf("\t\t%s channels its energy.\n",f->name);
	f->atk += 5;

	f->energy -= costFromSpecial(f->class,MPowerup);        //Reduce fighter energy

	lightGreen();
	printf("\t\t+5 ATK\t\n\n");
	white();
	screenTime(1);
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * If the enemy pointer hp value is at least 2, it either decreases by 10 if it's greater than 10 or get set to 1.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void mageSpecial_Spellstrike(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if (e->hp > 1 ) { //Check if enemy has some hp
		if (e->hp >= 11 ) {
			e->hp -= 10; //Deal damage	
		} else if (e->hp > 1) {
			e->hp = 1; //Set enemy hp to 1
		}
		
		printf("\t\t%s casts a deadly spell on %s!\n",f->name,stringFromEClass(e->class));

		f->energy -= costFromSpecial(f->class,MSpellstrike);        //Reduce fighter energy

		lightGreen();
		printf("\t\t%s feels the spell!\t\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	} else {
		lightRed();
		printf("\t\t%s's is weak already.\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);

	}
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * If the enemy pointer hp value is at least 2, its status value is set to Burned. The count value of enemy's Turncounter related to Burned status gets set to 3. If the enemy hp value is at least 5, it decreases by 4.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void mageSpecial_Flamering(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if (e->hp > 1) { //Check if enemy has some hp
		if (e->hp >= 5) {
			e->hp -= 4;
		}
		
		printf("\t\t%s summons a ring of fire around %s!\n",f->name,stringFromEClass(e->class));
		
		e->status = Burned; //Set status to Burned. May need change to manage multiple statuses active at once
		setCounter((Turncounter *)e->counters[BURNED],3); //Give 3 turns of Burned status
		f->energy -= costFromSpecial(f->class,MFlamering);        //Reduce fighter energy

		lightGreen();
		printf("\t\t%s was set on fire!\t\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	} else {
		lightRed();
		printf("\t\t%s's is weak already.\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	}
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * If the enemy pointer hp value is at least 2 and the def value is at least 5, the def value is set to 0.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void assassinSpecial_Grimdagger(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if (e->hp > 1 && e->def >= 5) { //Check if enemy has some hp and defense

		printf("\t\t%s throws its dagger at %s's heart.\n",f->name,stringFromEClass(e->class));
		e->def = 0; //Set enemy def
		
		f->energy -= costFromSpecial(f->class,XGrimdagger);        //Reduce fighter energy

		lightGreen();
		printf("\t\t%s is defenseless!\t\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	} else {
		lightRed();
		printf("\t\t%s's is weak already.\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	}
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * If the enemy pointer hp value is at least 6 and the fighter pointer hp value is equal or lower than its totalhp value, the enemy hp decreases by 5 and the fighter hp increases by the same amount.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void assassinSpecial_Leechknife(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if (e->hp >= 6 ) { //Check if enemy has some hp 
		if (f->hp <= f->totalhp - 6) { //Check if fighter has some space in his healthbar
			
			printf("\t\t%s strikes %s with its vampire knife.\n",f->name,stringFromEClass(e->class));
			f->hp +=  5;
			e->hp -= 5;
	
			f->energy -= costFromSpecial(f->class,XLeechknife);        //Reduce fighter energy

			lightGreen();
			printf("\t\t%s's health was sapped!\t\n\n",stringFromEClass(e->class));
			white();
			screenTime(1);
		} else {
			lightRed();
			printf("\t\t%s is feeling good enough.\n\n",f->name);
			white();
			screenTime(1);
		}
	} else {
		lightRed();
		printf("\t\t%s's is too weak.\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	}
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * If the fighter pointer def value is at least 8 and the vel value is at least 3, the def value increases by 8 and the vel value decreases by 2.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void assassinSpecial_Disguise(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if ( f->def >= 8 && f->vel >= 3 ) {
		printf("\t\t%s disguises himself as a %s.\n\n",f->name,stringFromEClass(e->class));
		f->def += 8;
		f->vel -= 2;
		f->energy -= costFromSpecial(f->class,XDisguise);        //Reduce fighter energy
		
		lightGreen();
		printf("\t\t+8DEF\t\t");
		lightRed();
		printf("-2 VEL\n\n");
		white();
		screenTime(1);
	} else {
		lightRed();
		printf("\t\t%s is too weak to disguise.\n\n",f->name);
		white();
		screenTime(1);
	}
}

/**
 * Takes one each of Fighter, Enemy and Path pointers and two integers denoting the index of current room and current enemy when the function is called.
 * If the enemy pointer hp value is at least 2, its status value is set to Poison. The count value of enemy's Turncounter related to Poison status gets set to 4. If the enemy hp value is at least 4, it decreases by 3.
 * Then energy cost is applied to the fighter.
 * @see Fighter
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter doing the attack.
 * @param e The Enemy receiving the attack.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void assassinSpecial_Venomblade(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if (e->hp > 1) { //Check if enemy has some hp
		if (e->hp >= 4) {
			e->hp -= 3;
		}
		
		printf("\t\t%s throws venomous daggers at %s's!\n",f->name,stringFromEClass(e->class));
		
		e->status = Poison; //Set status to Poison. May need change to manage multiple statuses active at once
		setCounter((Turncounter *)e->counters[POISON],4); //Give 4 turns of Poison status
		f->energy -= costFromSpecial(f->class,XVenomblade);        //Reduce fighter energy

		lightGreen();
		printf("\t\t%s was hit and poisoned!\t\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	} else {
		lightRed();
		printf("\t\t%s's is weak already.\n\n",stringFromEClass(e->class));
		white();
		screenTime(1);
	}
}

/**
 * Takes a Fighter pointer and prepares its specialSlot fields by allocating SPECIALSMAX slots.
 * Special slots are initialised, and depending on the fighter's class value, the REGISTER_CALLBACK() macro is used to register the function pointers corresponding to the special attacks.
 * The specific move assigned is determined using the ordering of both fighterClass values and SPECIALSMAX.
 * @see Fighter
 * @see Specialslot
 * @see SPECIALSMAX
 * @see REGISTER_CALLBACK()
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param f The Fighter pointer whose special slots will be initialised.
 * @return No return.
 */
void setSpecials(Fighter* f){

	for (int i = 0; i <= SPECIALSMAX ; i++) {
		Specialslot* s = (Specialslot*)malloc(sizeof(Specialslot));
		s->enabled = 0;
		s->move = i + ( f->class  * (SPECIALSMAX + 1) ) ; // Assign the i-th move offsetting by classNum * specialsMax
		s->cost = costFromSpecial(f->class,i); 
		//movename = stringFromSpecial(f->class,i);
		//printf("DEBUG\n%i\t%s\n",(i+ (f->class * (SPECIALSMAX + 1))),stringFromSpecial(f->class,i));
		//strcpy(s->name, movename);
		f->specials[i] = s;
	};

	switch (f->class) {
		
		case Knight:
			{
				REGISTER_CALLBACK(KSlash, knightSpecial_Slash);	
				REGISTER_CALLBACK(KCover, knightSpecial_Cover);	
				REGISTER_CALLBACK(KArmordrop, knightSpecial_Armordrop);	
				REGISTER_CALLBACK(KBerserk, knightSpecial_Berserk);	
			}
		break;
		case Archer:
			{
				REGISTER_CALLBACK(AHeadshot, archerSpecial_Headshot);
				REGISTER_CALLBACK(AQuivercheck, archerSpecial_Quivercheck);
				REGISTER_CALLBACK(APoisonshot, archerSpecial_Poisonshot);
				REGISTER_CALLBACK(AFireshot, archerSpecial_Fireshot);
			}
		break;
		case Mage: 
			{
				REGISTER_CALLBACK(MFatewarp, mageSpecial_Fatewarp);
				REGISTER_CALLBACK(MPowerup, mageSpecial_Powerup);
				REGISTER_CALLBACK(MSpellstrike, mageSpecial_Spellstrike);
				REGISTER_CALLBACK(MFlamering, mageSpecial_Flamering);
			}
		break;
		case Assassin:
			{
				REGISTER_CALLBACK(XGrimdagger, assassinSpecial_Grimdagger);
				REGISTER_CALLBACK(XLeechknife, assassinSpecial_Leechknife);
				REGISTER_CALLBACK(XDisguise, assassinSpecial_Disguise);
				REGISTER_CALLBACK(XVenomblade, assassinSpecial_Venomblade);
			}
		break;
	}

}


//Artifact functions

/**
 * Takes a Fighter pointer and an Enemy pointer.
 * If the active value inside the Artifact at THKSKULL index of artifactsBag array is false, fighter's permboost_def value increases by 4.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @return No return.
 */
void artifact_thinkingskull(Fighter* f,Enemy* e) {
	if (!(artifactsBag[THKSKULL].active)) { //Gives permanent +4 DEF to f
		f->permboost_def += 4;
		artifactsBag[THKSKULL].active = 1;
	}
};

/**
 * Takes a Fighter pointer and an Enemy pointer.
 * If the active value inside the Artifact at TWINKIE index of artifactsBag array is false, and the class value of enemy pointer is Zombie, enemy's xp value is doubled and then increased by its level value.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @return No return.
 */
void artifact_twinkie(Fighter* f,Enemy* e) {
	if (!(artifactsBag[TWINKIE].active)) { //Zombies give double exp
		if (e->class == Zombie ) {
			e->xp = e->level + (e->xp * 2);
		}
		artifactsBag[TWINKIE].active = 1;
	}
};

/**
 * Takes a Fighter pointer and an Enemy pointer.
 * If the active value inside the Artifact at WRISTBAND index of artifactsBag array is false, fighter's permboost_enr value increases by 2.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @return No return.
 */
void artifact_wristband(Fighter* f,Enemy* e) {
	if (!(artifactsBag[WRISTBAND].active)) { //Gives permanent +2 ENR to f
		f->permboost_enr += 2;
		artifactsBag[WRISTBAND].active = 1;
	}
};

/**
 * Takes a Fighter pointer and an Enemy pointer.
 * If the active value inside the Artifact at BOARTAIL index of artifactsBag array is false, and the class value of enemy pointer is Boar, enemy's vel value is multiplied by 2/3.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @return No return.
 */
void artifact_boartail(Fighter* f,Enemy* e) {
	if (!(artifactsBag[BOARTAIL].active)) { //Boars get slowed down
		if (e->class == Boar ) {
			e->vel /= 1.5;
		}
		artifactsBag[BOARTAIL].active = 1;
	}
};

/**
 * Takes a Fighter pointer and an Enemy pointer.
 * If the active value inside the Artifact at CHAOSORB index of artifactsBag array is false, all the and the level values for the baseenemystats array increase by 2.
 * If the artifact is already active, the same action as before occurs but the increase is by 3 and only if we hit 0 on a 20 roll.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @return No return.
 */
void artifact_chaosorb(Fighter* f,Enemy* e) {
	if (!(artifactsBag[CHAOSORB].active) && artifactsBag[CHAOSORB].innerValue == 0) { //Every monster levels up twice
		for (int i = 0; i < (ENEMYCLASSESMAX + 1) ; i++) {
			EnemyBaseStats* base = &baseenemystats[i];
			base->level += 2;
		}
		red();
		printf("\t\tYou feel an ominous power spike.\n");
		white();
		artifactsBag[CHAOSORB].active = 1;
		artifactsBag[CHAOSORB].innerValue += 1;
	} else if (!(artifactsBag[CHAOSORB].active) && (artifactsBag[CHAOSORB].innerValue > 0) && (rand() % 19 == 0)) { //Every monster levels up 3 times if we roll a 20
		for (int i = 0; i < ENEMYCLASSESMAX + 1; i++) {
			EnemyBaseStats* base = &baseenemystats[i];
			base->level += 3;
		}
		red();
		printf("\t\tYou hear screams from behind the enemy.\n");
		white();
	}	
};

/**
 * Takes a Fighter pointer and an Enemy pointer.
 * If the active value inside the Artifact at POWERSYPHON index of artifactsBag array is false, fighter energy value increases by 1.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @return No return.
 */
void artifact_powersyphon(Fighter* f,Enemy* e) {
	if (!(artifactsBag[POWERSYPHON].active) && (f->energy < f->totalenergy)) { //You gain back 1 energy if you're not full already
		f->energy += 1; 
		artifactsBag[POWERSYPHON].active = 1;
	}
};

/**
 * Takes a Fighter pointer and an Enemy pointer.
 * If the active value inside the Artifact at GIANTFOSSILE index of artifactsBag array is false, and the class value of enemy pointer is Troll, the enemy hp value is either decreased by 10 if it's at least 11 or set to 1 otherwise.
 * Then the artifact is set as active.
 * @see Fighter
 * @see Enemy
 * @see Artifact
 * @see artifactClass
 * @param f The fighter involved in the artifact action.
 * @param e The enemy involved in the artifact action.
 * @return No return.
 */
void artifact_giantfossile(Fighter* f,Enemy* e) {
	if (!(artifactsBag[GIANTFOSSILE].active)) { //Troll lose some hp
		if (e->class == Troll) {
			if (e->hp > 10) { //This actual value will mean little since artifacts are called before fighting the enemy
				e->hp -= 10;
			} else {
				e->hp = 1;
			}
		}
		artifactsBag[GIANTFOSSILE].active = 1;
	}
};
	
/**
 * Prepares the artifact_fun value for each Artifact in artifactsBag array, according to its artifactClass.
 * @see Fighter
 * @see Artifact
 * @see artifactClass
 * @see artifactsBag
 * @see ARTIFACTSMAX
 * @return No return.
 */
void initArtifactsFun() {
	for (int i=0; i < ARTIFACTSMAX + 1; i++) {
		switch(artifactsBag[i].class) {
			case THKSKULL: {
				artifactsBag[i].artifact_fun = &artifact_thinkingskull;
			}
			break;
			case TWINKIE: {
				artifactsBag[i].artifact_fun = &artifact_twinkie; 	
			}
			break;
			case WRISTBAND: {
				artifactsBag[i].artifact_fun = &artifact_wristband; 	
			}
			break;
			case BOARTAIL: {
				artifactsBag[i].artifact_fun = &artifact_boartail; 	
			}
			break;
			case CHAOSORB: {
				artifactsBag[i].artifact_fun = &artifact_chaosorb; 	
			}
			break;
			case POWERSYPHON: {
				artifactsBag[i].artifact_fun = &artifact_powersyphon; 	
			}
			break;
			case GIANTFOSSILE: {
				artifactsBag[i].artifact_fun = &artifact_giantfossile; 	
			}
			break;
		}
	};
};

/**
 * Takes a Fighter pointer and resets all of its permboost_STAT values to 0, also correctly updating the current stat values.
 * @see Fighter
 * @param f The fighter pointer whose permboosts will be reset.
 * @return No return.
 */
void resetPermboosts(Fighter* f) {
	for (int i=0; i < STATMAX + 1; i++) {
		switch (i) {
			case ATK: {
				f->atk -= f->permboost_atk;
				if (f->atk < 0) { f->atk = 0;};
				f->permboost_atk = 0;
			}
			break;
			case DEF: {
				f->def -= f->permboost_def;
				if (f->def < 0) { f->def = 0;};
				f->permboost_def = 0;
			}
			break;
			case VEL: {
				f->vel -= f->permboost_vel;
				if (f->vel < 0) { f->vel = 0;};
				f->permboost_vel = 0;
			}
			break;
			case ENR: {
				f->totalenergy -= f->permboost_enr;
				f->energy -= f->permboost_enr;
				if (f->energy < 0) { f->energy = 0;};
				f->permboost_enr = 0;
			}
			break;
		};
	};
};

/**
 * Takes a Fighter pointer and applies all of its permboost_STAT values by adding them to the current stat values.
 * @see Fighter
 * @param f The fighter pointer whose permboosts will be applied.
 * @return No return.
 */
void applyPermboosts(Fighter* f) {
	for (int i=0; i < STATMAX + 1; i++) {
		switch (i) {
			case ATK: {
				f->atk += f->permboost_atk;
			}
			break;
			case DEF: {
				f->def += f->permboost_def;
			}
			break;
			case VEL: {
				f->vel += f->permboost_vel;
			}
			break;
			case ENR: {
				f->totalenergy += f->permboost_enr;
				f->energy += f->permboost_enr;
			}
			break;
		};
	};
};

/**
 * Resets the active value for each Artifact in artifactsBag array that doesn't have an always active trait.
 * At the moment, this only excludes CHAOSORB.
 * @see Artifact
 * @see artifactClass
 * @see artifactsBag
 * @return No return.
 */
void resetArtifactsState() {
	for (int i=0; i < (ARTIFACTSMAX + 1) ; i++) {

		//if (i == CHAOSORB) { //Chaosorb never gets reset
		//	continue;
		//};

		if (artifactsBag[i].qty != 0 ) { //We only reset the ones we have
			artifactsBag[i].active = 0;
		};
	};
};	

/**
 * Takes a Fighter and a Enemy .
 * Iterates over artifacts bag and calls the inner function pointer if an artifact has positive qty and its active member is false.
 * @see Artifact
 * @param f A Fighter pointer.
 * @param e An Enemy pointer.
 * @return No return
 */
void applyArtifacts(Fighter* f, Enemy* e){
	for (int i=0; i < ARTIFACTSMAX + 1; i++) {
		if (artifactsBag[i].qty != 0 && !(artifactsBag[i].active)) { //We only apply the ones we have unlocked && aren't active already
			artifact_fun fun;
			fun = artifactsBag[i].artifact_fun;
			(fun)(f,e);
		};
	};
}

/**
 * Takes a Fighter pointer and prepares its equipslots field by allocating an Equipslot for each Equipzone.
 * @see Fighter
 * @see Equipslot
 * @see Equipzone
 * @see counterIndexes
 * @see EQUIPZONES
 * @param f The Fighter pointer whose equipslots field will be initialised.
 * @return No return.
 */
void initEquipSlots(Fighter* f){

	for (int i = 0; i <= EQUIPZONES ; i++) {
		Equipslot* s = (Equipslot*)malloc(sizeof(Equipslot));
		s->active = 0;
		s->type = i;	
		f->equipslots[i] = (struct Equipslot*) s;
	};
}

/**
 * Takes one Fighter and one Path pointers and initialises the fighter fields.
 * Luck value is set as path luck value modulo MAXPLAYERLUCK.
 * The BaseStats pointer for the fighter's figtherClass is loaded.
 * The stats field is initalised with all inner values at 0.
 * setSpecials(), initCounters() are called to init the specialslots and counters fields.
 * initEquipSlots() is called to init the equipslots field, and the relative int field are set to 0.
 * initArtifactsFun() is called to init all the Artifact effect_fun field.
 * All the tempboost_STAT and permboost_STAT fields are set to 0.
 * All the fields common to BaseStats are initialised with the base value.
 * The status field is set to Normal.
 * @see Fighter
 * @see fighterClass
 * @see Path
 * @see BaseStats
 * @see countStats
 * @see MAXPLAYERLUCK
 * @see setSpecials()
 * @see initCounters()
 * @see initEquipSlots()
 * @see initArtifactsFun()
 * @see fighterStatus
 * @param f The Fighter whose fields will be initialised.
 * @param p The Path pointer of the current game.
 * @return No return.
 */
void initPlayerStats(Fighter* player, Path* path) {
	
	//player luck : MAXPLAYERLUCK = path luck : MAXLUCK

	player->luck = (path->luck * MAXPLAYERLUCK) / MAXLUCK ;
	
	BaseStats* base = &basestats[player->class];

	countStats* s = (countStats*)malloc(sizeof(countStats));
	
	s->enemieskilled=0;
	s->criticalhits=0;
	s->consumablesfound=0;
	s->equipsfound=0;
	s->artifactsfound=0;
	s->roomscompleted=0;
	s->specialsunlocked=0;

	setSpecials(player);	
	initCounters(player);	
	initPerks(player);
	
	initEquipSlots(player);	
	player->equipsBagOccupiedSlots = 0; //Keeps track of how many slots are occupied.
	player->earliestBagSlot = 0; //To alwasy use the array efficiently (???) I sense linked lists may be better
	
	initArtifactsFun();
	
	player->permboost_atk = 0;
	player->permboost_def = 0;
	player->permboost_vel = 0;
	player->permboost_enr = 0;

	player->equipboost_atk = 0;
	player->equipboost_def = 0;
	player->equipboost_vel = 0;
	player->equipboost_enr = 0;

	player->stats = s;
	player->hp = base->hp;
	player->totalhp = base->totalhp;
	player->atk = base->atk;
	player->def = base->def;
	player->vel = base->vel;
	player->level = base->level;
	player->totalxp = base->totalxp;
	player->currentlevelxp = base->currentlevelxp;
	player->totallevelxp = base->totallevelxp;
	player->status = Normal;
	player->totalenergy = base->totalenergy;
	player->energy = player->totalenergy;
}

/**
 * Takes one Enemy pointer and initialises the enemy fields.
 * The EnemyBaseStats pointer for the enemy's enemyClass is loaded.
 * If the beast field at the pointer is already set before this call, atk def vel and hp of the enemy will receive a multiplicative boost equal to BSTFACTOR . Xp value will also be multiplie by 3.
 * Notably, the level field is not set at all by this function and needs to be set by the caller.
 * All the fields common to EnemyBaseStats are initialised with the base value and eventual beast boost.
 * initECounters() is called to init the counters field.
 * The status field is set to Normal.
 * @see Enemy
 * @see enemyClass
 * @see EnemyBaseStats
 * @see initECounters()
 * @see fighterStatus
 * @param e The Enemy whose fields will be initialised.
 * @return No return.
 */
void initEnemyStats(Enemy* e) {
	EnemyBaseStats* base = &baseenemystats[e->class];

	float beastf = 1;

	if (e->beast) {
		beastf = BSTFACTOR;
	}
	
	e->hp = round(beastf * base->hp);
	e->totalhp = round(beastf * base->totalhp);
	e->atk = round(beastf * base->atk);
	e->def = round(beastf * base->def);
	e->vel = round(beastf * base->vel);
	
	//Level should be set by caller
	
	e->totalenergy = base->totalenergy;
	e->energy = e->totalenergy;

	initECounters(e);	
	//Triple xp for beasts
	e->xp = (e->beast) ? 3 * base->xp : base->xp;
	e->status = Normal;
}

/**
 * Takes one integer and an enemyClass and returns the boost relative to the level delta with base enemy stats, as an integer.
 * The EnemyBaseStats pointer for the enemy's enemyClass is loaded.
 * If the boost is negative, returns 0.
 * @see Enemy
 * @see enemyClass
 * @see EnemyBaseStats
 * @param lvl The level to check the boost against.
 * @param eclass The enemyClass used to determine base level.
 * @return int The boost for any given stat, at the level passed as argument.
 */
int getEnemyBoost(int lvl, enemyClass eclass) {

	EnemyBaseStats* base = &baseenemystats[eclass];

	float boost = ((lvl - base->level) * 1.15) ;
	if (boost <= 0) {
		boost = 0;
	}	
	
	return (int) boost;
}

/**
 * Takes an Enemy pointer and an integer used to force execution.
 * If the force parameter is true, all checks are ignored.
 * If enemy's hp value is at least 40% of total, and none of atk, def or vel is 0 or less, nothing happens with an early return.
 * Otherwise, getEnemyBoost() is called to calc the level stat boost for each stat.
 * The EnemyBaseStats pointer for the enemy's enemyClass is loaded and each one of atk, def and vel is checked accounting for level boost.
 * If none of them is below the respective treshold of 30, 30 and 20 % of total, nothing happens.
 * Otherwise, all of them are reset to full amount accounting for beast boost and level boost.
 * @see Enemy
 * @see enemyClass
 * @see getEnemyBoost()
 * @see BSTFACTOR
 * @see EnemyBaseStats
 * @param e The Enemy pointer to check the stats for.
 * @param force The integer to bypass all checks if true.
 * @return No return.
 */
void statResetEnemy(Enemy* e, int force) {
	if ( !force && (e->hp >= 0.4 * e->totalhp) && !( e->atk <=0 || e->def <= 0 || e->vel <=0) ) {
		return;
	}	
	int boost = getEnemyBoost(e->level,e->class);
	
	float beastf = 1 ;

	if (e->beast) {
		beastf = BSTFACTOR;
	}
	EnemyBaseStats* base = &baseenemystats[e->class];

	if (force) { //We also update hp values
		int hpBoost = boost + round(base->level * 0.75) + (base->hp / 10 ) + ((base->def / 4) % 10) ;
		e->hp = round(beastf * (base->hp + hpBoost));
		e->totalhp = e->hp;
	}

	if (force || e->vel <= (0.3 * (base->vel + boost)) || (e->atk <= (0.3 * (base->atk + boost))) || e->def <= (0.2 * (base->def + boost))) {
		e->vel = round(beastf * (base->vel + boost));
		e->atk = round(beastf * (base->atk + boost));
		e->def = round(beastf * (base->def + boost));
		//Reset stats
		if (!force) {
			yellow();
			printf("\n\n\t%s's stats reset.\n",stringFromEClass(e->class));
			white();
		}
	}
}

/**
 * Takes a Enemy and a Fighter pointers and three integers denoting current room index, how many enemies are in the room and current enemy index.
 * The class field of the enemy is randomised according to ENEMYCLASSESMAX.
 * If the room index is multiple of BEASTROOM and the enemy is the last one in he room, its beast field is set to 1.
 * The EnemyBaseStats pointer for the enemy's enemyClass is loaded and the level field for the enemy is set to base level, before increasing by fighter level over ENEMYLVLRATIO .
 * initEnemyStats() is called to set all stat fields and statResetEnemy() is called with force=true to apply the boosted stats to leveled enemies.
 * @see Enemy
 * @see enemyClass
 * @see ENEMYCLASSESMAX
 * @see BEASTROOM
 * @see BSTFACTOR
 * @see ENEMYLVLRATIO
 * @see EnemyBaseStats
 * @see initEnemyStats()
 * @see statResetEnemy()
 * @param e The Enemy pointer to prepare.
 * @param player The Fighter.
 * @param roomindex The index of current room.
 * @param enemiesInRoom The number of enemies in current room.
 * @param enemyindex The index of current enemy.
 * @return No return.
 */
void prepareRoomEnemy(Enemy* e, Fighter* player, int roomindex, int enemiesInRoom, int enemyindex) {

		//Randomise enemy class
		e->class = rand() % (ENEMYCLASSESMAX + 1);

		
		//Check for spawning beast enemies
		if ((roomindex % BEASTROOM == 0) && (enemyindex == (enemiesInRoom - 1))) {
			e->beast = 1;	
		} else {
			e->beast = 0;	
		};

	
		EnemyBaseStats* base = &baseenemystats[e->class];
		
		e->level = base->level;
		
		//Enemies get 1 level for each X player levels
		if (player->level > 1) {
			e->level += round(player->level / ENEMYLVLRATIO) ;
		}
		
		//Load enemy stats
		initEnemyStats(e);
		
		//Force load of level bonuses	
		statResetEnemy(e,1);
}

/**
 * Takes a Fighter pointer and prints most of its values formatted.
 * @see Fighter
 * @see stringFromClass()
 * @see stringFromStatus()
 * @param f The Fighter pointer with stats to print.
 * @return No return.
 */
void printStats(Fighter* f) {
	
	printf("\t%s's stats:\n\n",f->name);
	printf("\t\tClass:\t%s\n",stringFromClass(f->class));
	printf("\t\tHp:\t%i/%i\n",f->hp,f->totalhp);
	printf("\t\tEnergy:\t%i/%i\n",f->energy,f->totalenergy);
	printf("\t\tAtk:\t%i\n",f->atk);
	printf("\t\tDef:\t%i\n",f->def);
	printf("\t\tVel:\t%i\n",f->vel);
	printf("\t\tLvl:\t%i\n",f->level);
	printf("\t\tCurrent Level exp:\t%i/%i\n",f->currentlevelxp,f->totallevelxp);
	printf("\t\tTotal exp:\t%i\n",f->totalxp);
	printf("\t\tStatus:\t%s\n",stringFromStatus(f->status));
}

/**
 * Takes a Enemy pointer and prints most of its values formatted.
 * @see Enemy
 * @see stringFromEClass()
 * @see stringFromStatus()
 * @param e The Enemy pointer with stats to print.
 * @return No return.
 */
void printEStats(Enemy* e) {
	if (e->beast) {
		lightRed();
	}	
	printf("\t%s's stats:\n",stringFromEClass(e->class));
	if (e->beast) {
		white();
	}	
	printf("\tHp:\t%i/%i\n",e->hp,e->totalhp);
	printf("\tEnergy:\t%i/%i\n",e->energy,e->totalenergy);
	printf("\tAtk:\t%i\n",e->atk);
	printf("\tDef:\t%i\n",e->def);
	printf("\tVel:\t%i\n",e->vel);
	printf("\tLvl:\t%i\n",e->level);
	printf("\tXp:\t%i\n",e->xp);
	printf("\tStatus:\t%s\n",stringFromStatus(e->status));
}

/**
 * Takes a Enemy pointer and returns its xp gain as sum of xp field value and level.
 * @see Enemy
 * @param e The Enemy pointer.
 * @return int The xp gain.
 */
int getEnemyXpGain(Enemy* e) {
	
	int xp = e->xp + e->level;
	return xp;
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
 * @param f The Fighter pointer with stats to print.
 * @param e The Enemy pointer with stats to print.
 * @return No return.
 */
void printBattleStats(Fighter* f, Enemy* e) {


	//Lazy highlighting of the beast enemies fights
	if (e->beast) {
		lightPurple();
	}
	printf("\t\t\t%s's stats:\t\t",stringFromEClass(e->class));
	if (e->beast) {
		white();
	}

	printf("%s's stats:\n",f->name);
	
	//Lazy highlighting of the beast enemies fights
	if (e->beast) {
		lightPurple();
	}
	printf("Class:\t\t%s\t\t",stringFromEClass(e->class));
	if (e->beast) {
		white();
	}
	
	printf("\t%s\n",stringFromClass(f->class));
	printf("Hp:\t\t");
	if ( (e->hp / (e->totalhp / 1.0F)) <= 0.25F) {
		red();
	}
	printf("%i/%i",e->hp,e->totalhp);
	white();
	if ( (f->hp / (f->totalhp / 1.0F)) <= 0.25F) {
		red();
	}
	printf("\t\t\t%i/%i\n",f->hp,f->totalhp);
	white();
	printf("Energy:\t\t");
	if ( (e->energy / (e->totalenergy / 1.0F)) <= 0.25F) {
		red();
	}
	printf("%i/%i",e->energy,e->totalenergy);
	white();
	if ( (f->energy / (f->totalenergy / 1.0F)) <= 0.25F) {
		red();
	}
	printf("\t\t\t%i/%i  ",f->energy,f->totalenergy);
	white();
	if (f->equipboost_enr != 0) {
		if (f->equipboost_enr > 0) {
			lightGreen(); printf("(+");
		} else { 
			lightRed(); printf("(-");	
		};
		printf("%i)",f->equipboost_enr);
		white();
	}
	printf("\n");
	printf("Atk:\t\t%i\t\t\t%i ",e->atk, f->atk);
	if (f->equipboost_atk != 0) {
		if (f->equipboost_atk > 0) {
			lightGreen(); printf("(+");
		} else { 
			lightRed(); printf("(-");	
		};
		printf("%i)",f->equipboost_atk);
		white();
	}
	printf("\n");
	printf("Def:\t\t%i\t\t\t%i  ",e->def, f->def);
	if (f->equipboost_def != 0) {
		if (f->equipboost_def > 0) {
			lightGreen(); printf("(+");
		} else { 
			lightRed(); printf("(-");	
		};
		printf("%i)",f->equipboost_def);
		white();
	}
	printf("\n");
	printf("Vel:\t\t%i\t\t\t%i  ",e->vel,f->vel);
	if (f->equipboost_vel != 0) {
		if (f->equipboost_vel > 0) {
			lightGreen(); printf("(+");
		} else { 
			lightRed(); printf("(-");	
		};
		printf("%i)",f->equipboost_vel);
		white();
	}
	printf("\n");
	printf("Lvl:\t\t%i\t\t\t%i\n",e->level,f->level);
	printf("Xp:\t\t(%i)",getEnemyXpGain(e));
	if ( (f->currentlevelxp / (f->totallevelxp / 1.0F)) >= 0.75F) {
		green();
	}
	printf("\t\t\t%i/%i\n",f->currentlevelxp,f->totallevelxp);
	white();
	if (e->status != Normal) {
		red();
	}
	printf("Status:\t\t%s",stringFromStatus(e->status));
	white();
	printf("\t\t\t%s\n",stringFromStatus(f->status));
	
}

/**
 * Takes a Consumable pointer and prints most of its values formatted.
 * @see Consumable
 * @param c The Consumable pointer with stats to print.
 * @return No return.
 */
void printConsumablesStats(Consumable* c) {
	printf("  (%i)\t%i\t%s\t\"%s\"\n",c->class,c->qty,c->name,c->desc);	
};

/**
 * Takes a Artifact pointer and prints most of its values formatted.
 * @see Artifact
 * @param a The Artifact pointer with stats to print.
 * @return No return.
 */
void printArtifactStats(Artifact* a) {
	printf("  (%i)\t\t",a->class);
	purple();
	printf("%s\t",a->name);
	yellow();
	printf("\"%s\"\n",a->desc);	
	white();
};

/**
 * Takes a quality value and calls the respective color function without actually printing text.
 * @see quality
 * @param q The quality value we want to set text color for.
 * @return No return.
 */
void printQualityColor(quality q) {
	switch (q) {
		case Bad: {
			lightRed();
		}
		break;
		case Average: {
			strongWhite();
		}
		break;
		case Good: {
			lightCyan();
		}
		break;
	};
};

/**
 * Takes a Equip pointer and prints most of its values formatted.
 * The beginning of the format assumes the caller prepended "(x" on the output before calling, where x is the index of the equip.
 * @see Equip
 * @see stringFromQuality()
 * @param e The Equip pointer with stats to print.
 * @return No return.
 */
void printEquipStats(Equip* e) {
	printf(")\t"); //This completes the print started in the calling loop, which supplies the index ATM
	
	//Quality color
	printQualityColor(e->qual);
	printf("%s  ",stringFromQuality(e->qual));

	printf("%s  ",e->name);
	white();

	printf("\"%s\"  (L%i)\t",e->desc,e->level);
	
	lightCyan();
	printf("%s  ",stringFromEquipzones(e->type));

	lightGreen();
	//Stats, will be then printed only if != 0
	if (e->atk != 0) {	printf("A: %i ",e->atk); };
	if (e->def != 0) {	printf("D: %i ",e->def); };
	if (e->vel != 0) {	printf("V: %i ",e->vel); };
	if (e->enr != 0) {      printf("E: %i",e->enr); };

	printf("\n");
	white();
	
	//Perks, will be then printed only if perksCount != 0

	for (int i = 0; i < e->perksCount; i++) {
		lightPurple();
		printf("\t\t%s\n",e->perks[i]->name);
	};
	white();
};

/**
 * Takes a Enemy pointer and two integers for current room index and current enemy index, and prints the spawn message formatted.
 * @see Enemy
 * @see stringFromEClass()
 * @param e The Enemy pointer to print.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void printSpawnMessage(Enemy* e, int roomIndex, int enemyIndex) {
	if (!e->beast) {
		printf("\nRoom %i)\t\tEnemy #%i:\t%s\n",roomIndex,enemyIndex+1,stringFromEClass(e->class));
	} else {
		lightBlue();
		printf("\nYou feel at discomfort.\nRoom %i)\t\t",roomIndex);
		lightRed();
		printf("Enemy #%i:\t%s\n",enemyIndex+1,stringFromEClass(e->class));
			white();
	}
}

/**
 * Takes a integer and a string array (possibly from main) and a Fighter pointer to set its name and class values.
 * Depending on argc value, the arguments in argv may be used instead of calling the functions to get user input.
 * @see Fighter
 * @see pickName()
 * @see pickClass()
 * @param argc The number of argv values + 1 (0 is program name?).
 * @param argv Array of strings with argc - 1 values.
 * @param player The Fighter of which we set name and class.
 * @return No return.
 */
void getParams(int argc, char** argv, Fighter* player){
	if (argc == 1) {
		pickName(player);
		pickClass(player);
	} else if (argc == 2 || argc == 3){
		char n[25];
		int i = 0, len=0;
		for (i=0;i<23;i++) {
			len++;
		}
		if (len>0) {
			for (int i=0; i<len;i++) {
				n[i]=argv[1][i];
			}
			n[i+1]='\0';
			strcpy(player->name,n);
		} else {
			pickName(player);
		}
	} else {
		usage();
	} 
	if ( argc == 2 ) {
		pickClass(player);
	}

	if ( argc == 3 ) {
		int c= -1;
		int i = 0;
		for (i=0; i <= CLASSESMAX ; i++ ) {
			if (strcmp(argv[2],classesstrings[i]) == 0){
				c=1;
				player->class=i;
				break;
			}
		}
		if (c < 0 ) {
			pickClass(player);
		}
	}
}

/**
 * Prints the turn options formatted and returns the chosen option as an integer.
 * Holding down Enter will spam fight().
 * Debug menu is accessed using 'd'.
 * @return The chosen integer value representing turn action.
 */
int getTurnChoice() {
	char choice;
	printf("\tWhat do you want to do?\n\n\t[\n\t\t0-1{Return}\tFight\t\t2\tSpecial Attack\n");
	printf("\t\t3\t\tConsum.\t\t4\tArtifacts\n");
	printf("\t\t5\t\tEquips\t\t6\tPerks\n");
	printf("\t\t7-8\t\tStats\t\t9\tQuit\t\t\t\t]\n");
	char c[25];
	fgets(c, sizeof c, stdin);

	switch(c[0]) {
		case '0' ... '1':
			return FIGHT;
		break;
		case '2':
			return SPECIAL;
		case '3':
			return CONSUMABLE;
		break;
		case '4':
			return ARTIFACTS;
		break;
		case '5':
			return EQUIPS;
		break;
		case '6':
	       		return PERKS;
		break;
		case '7' ... '8':
			return STATS;
		break;
		//DEBUG
		case 'd':
			return DEBUG;
		case '9':
			return QUIT;
		break;	
		default :
	       		return FIGHT;
		break;	
	}	
	
}

/**
 * Takes two integers for level to calc against and luck, and returns the boost relative to the level with luck variations, as an integer.
 * At level 1, returns 0.
 * @param lvl The level to check the boost against.
 * @param luck The luck value to influence calcs.
 * @return int The boost for any given stat, at the level passed as argument.
 */
int getBoost(int lvl, int luck) {

	float boost = (lvl * 1.25F) + ((luck % 2 ) * 2);
	
	if (lvl < 2 ) {
		boost = 1.0; // Evitare conflitti
	}

	return (int) boost;
}

/**
 * Takes a Fighter pointer and asks the user to select a specialMove to unlock with a formatted text menu.
 * From the specials field of fighter, only the SpecialSlot with the enabled falg not set are printed and selectable by user.
 * For the Fighter, the equipboost values are also displayed.
 * @see Fighter
 * @see SpecialSlot
 * @see SPECIALSMAX
 * @see setSpecials()
 * @see stringFromSpecial()
 * @param f The Fighter pointer that get one of his not yet unlocked specials.
 * @return No return.
 */
void unlockSpecial(Fighter* f) {
	
	purple();
	printf("\n\t\tYou unlocked a new tecnique.\n\n");
	white();
	printf("\t\tSpecial moves:\t\tNumber:\t\tEnergy Cost:\n\n");
	for (int i = 0; i < SPECIALSMAX + 1; i++) {
		if (! (f->specials[i]->enabled) ) {
			printf("\t\t%s\t\t%i\t\t%i\n",stringFromSpecial(f->class,i),i,f->specials[i]->cost);
		}
	}
	
	int success = 0;
	
	do {
		int num = -1;
		printf("\n\n\tInsert special move number:\n\n\t");
	
		char c[25];
		fgets(c, sizeof c, stdin);
		if (c[0]=='\n') {
			continue;
		}

		//Converting the first char into an integer...
		num = c[0] - '0';

		if (num < SPECIALSMAX + 1) {	//Check if selected move number is lower than the maximum
			Specialslot* selected = f->specials[num];
		
			//Check if the selected move is NOT enabled
			if (! (selected->enabled) ) {
				//Enable the move
				selected->enabled = 1;

				//Success condition, we can leave the loop
				success = 1;
			}
		}
	} while (!success);

	f->stats->specialsunlocked += 1;

};

/**
 * Takes a Fighter pointer and updated its stats.
 * getBoost() is called to get the stat boost for current level, which is then applyed to atk, def and vel; while hp gets first multiplied by 1.13 and then gets the boost added.
 * The totalenergy value is increased by player level over 5.
 * Hp and energy are replenished.
 * If the level is multiple of SPECIALLVLRATIO and the player still has at least one SpecialSlot not enabled, unlockSpecial() is called.
 * @see Fighter
 * @see getBoost()
 * @see BaseStats
 * @see SpecialSlot
 * @see SPECIALSMAX
 * @see SPECIALLVLRATIO
 * @see unlockSpecial()
 * @param player The Fighter pointer that levels up.
 * @return No return.
 */
void onLevelUp(Fighter* player) {
	int boost = getBoost(player->level,player->luck);

	BaseStats* base = &basestats[player->class];

	player->atk = ( base->atk + boost );
	player->def = ( base->def + boost );
	player->vel = ( base->vel + boost );
	player->totalhp = ( base->hp * 1.13 ) + boost;

	int energyboost = player->level / 5; //Energy grows by lvl/5 on each lvl up
	player->totalenergy = base->totalenergy + energyboost + player->permboost_enr; 

	player->hp = player->totalhp; //Cure on level up
	player->energy = player->totalenergy; //Refill energy on level up

	//Check if you get a new special move
	if (player->level % SPECIALLVLRATIO == 0 && (player->stats->specialsunlocked < ( SPECIALSMAX + 1 ) ) ) {
		unlockSpecial(player);
	}
	
	lightCyan();
	printf("\n\n\tYour wounds were healed.\n");
	printf("\n\n\tYour energy is replenished.\n");
	white();
	screenTime(2.0);
}

/**
 * Takes a Fighter pointer and the amount of xp to add.
 * Current level xp is managed by this function, including how much xp is needed to level up again. The values are stored in the fighter struct.
 * Thresholds for each level are checked and eventually onLevelUp() is called, with recursion on this function after.
 * @see Fighter
 * @see onLevelUp()
 * @param player The Fighter pointer that gets xp.
 * @param xp The amount of xp.
 * @return No return.
 */
void checkremainder(Fighter* player, int xp) {
	int curr = player->currentlevelxp;
	int tot = player->totallevelxp;
		
	if (curr + xp >= tot) {
		player->totalxp += xp;
		if (xp !=0) { printf("\n\n\t%s obtained %i xp.\n", player->name, xp);
		}
		player->level++;
		
		cyan();
		printf("\n\n\t%s reached Lvl. %i !\n", player->name, player->level);
		white();

		//Stats gains on level up
		onLevelUp(player);	

		player->currentlevelxp = abs((tot-curr-xp));
		int nextLevelMoreXp = round(1.75 * ( player->level + 1 )) + player->level + 1 ;
		player->totallevelxp = (tot / player->level ) + nextLevelMoreXp;
		checkremainder(player,0);
	} else {
		player->currentlevelxp += xp;
		player->totalxp += xp;
		if (xp!=0) {
			printf("\n\n\t%s obtained %i xp.\n", player->name, xp);
		}
	}
}

/**
 * Takes a Fighter and a Enemy pointers and handles xp gain by fighter.
 * @see Fighter
 * @see Enemy
 * @see getEnemyXpGain()
 * @see checkremainder()
 * @param player The Fighter pointer that gets xp.
 * @param e The Enemy pointer that gives xp.
 * @return No return.
 */
void giveXp(Fighter* player, Enemy* e){
	
	
	int xp = getEnemyXpGain(e);

	checkremainder(player, xp);
}

/**
 * Takes a Fighter pointer and an integer used to force execution.
 * If the force parameter is true, all checks are ignored.
 * If enemy's hp value is at least 50% of total, and none of atk, def or vel is 0 or less, nothing happens with an early return.
 * Otherwise, getBoost() is called to calc the level stat boost for each stat.
 * The BaseStats pointer for the fighter's fighterClass is loaded and each one of atk, def and vel is checked accounting for level boost.
 * If none of them is below the respective treshold of 35, 18 and 30 % of total, nothing happens.
 * Otherwise, all of them are reset to full amount accounting for permboosts and level boost.
 * @see Fighter
 * @see fighterClass
 * @see getBoost()
 * @param e The Enemy pointer to check the stats for.
 * @param force The integer to bypass all checks if true.
 * @return No return.
 */
void statReset(Fighter* player, int force) {
	if ( !force && (player->hp >= 0.5 * player->totalhp) && !(player->atk <=0 || player->def <=0 || player->vel <= 0) ) {
		return;
	}
	
	int boost = getBoost(player->level,player->luck);

	BaseStats* base = &basestats[player->class];
	if (force || player->vel <= 0.3 * (base->vel + boost) || player->atk <= 0.35 * (base->atk + boost) || player->def <= 0.18 * (base->def + boost )) {
		player->vel = base->vel + boost + player->permboost_vel;
		player->atk = base->atk + boost + player->permboost_atk;
		player->def = base->def + boost + player->permboost_def;
		//Reset stats
		if (!force) {
			yellow();
			printf("\n\n\t%s's stats reset.\n",player->name);
			white();
		}
	}
}

/**
 * Takes a fighterStatus value and a string of who's the entity to print the respective status message.
 * @see fighterStatus
 * @param status The fighterStatus at hand.
 * @param subject A string with name of entity owning the fighterStatus.
 * @return No return.
 */
void printStatusText(fighterStatus status, char* subject) {
	switch (status) {
		case Normal: { 
			return;
		};
		break;
		case Poison: case Burned: {
			printf("\n\t%s is hurt by its %s.\n\n",subject, stringFromStatus(status));
		}
		break;
		case Weak: case Strong: {
			printf("\n\t%s is feeling %s.\n\n",subject, stringFromStatus(status));
		}
		break;
		case Frozen: {
			printf("\n\t%s is frozen cold.\n",subject);
		}
		break;
	}
}

/**
 * Takes a Fighter pointer value and applies the effect pertaining to its status value.
 * @see Fighter
 * @see fighterStatus
 * @see printStatusText()
 * @param f The Fighter pointer at hand.
 * @return No return.
 */
void applyStatus(Fighter* f){
	
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
			lightRed();
			printStatusText(Poison,f->name);
			white();
		}
		break;
		case Burned: {
				lightRed();
				printStatusText(Burned,f->name);
				white();
			}
		break;
		case Frozen: {
				lightRed();
				printStatusText(Frozen,f->name);
				white();
			}
		break;
		case Weak:

		break;
		case Strong:

		break;
	}	
};

/**
 * Takes a Enemy pointer value and applies the effect pertaining to its status value.
 * @see Enemy
 * @see fighterStatus
 * @see printStatusText()
 * @see stringFromEClass()
 * @param e The Enemy pointer at hand.
 * @return No return.
 */
void applyEStatus(Enemy* e){
	
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
			lightGreen();
			printStatusText(Poison,stringFromEClass(e->class));
			white();
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
			lightGreen();
			printStatusText(Burned,stringFromEClass(e->class));
			white();
		}
		break;
		case Frozen: { 
			if (e->vel >= 3) {
				e->vel -= 1;
			} else {
				e->vel = 1; //Will this be a problem for kills in the enemy loop?
			}
			lightGreen();
			printStatusText(Frozen,stringFromEClass(e->class));
			white();
		}

		break;
		case Weak:

		break;
		case Strong:

		break;
	}	
};

/**
 * Takes a Fighter pointer value and adds a random Consumable to consumablesBag.
 * @see Fighter
 * @see Consumable
 * @see consumableClass
 * @see CONSUMABLESMAX
 * @see stringFromConsumables()
 * @param f The Fighter pointer at hand.
 * @return No return.
 */
void dropConsumable(Fighter* player) {
	int drop = rand() % (CONSUMABLESMAX + 1);
	
	//Special drop chances. Maybe a function for this?
	while (drop == Powergem) {
		if ( !(rand() % 2 == 0)) {
			drop = rand() % (CONSUMABLESMAX + 1);
		}
	}
			
	cyan();
	printf("\n\n\tYou found a %s!\n",stringFromConsumables(drop));
	white();
	consumablesBag[drop].qty++;
		
	//Update stats
	player->stats->consumablesfound++;
	
	screenTime(1);
}

/**
 * Takes a Fighter pointer value and an integer indicating if the drop was from a beast enemy, and adds a random Equip to the fighter's equipsBag.
 * The Equip dropped is initalised here, including stat variations for quality and level boosts (stat increase from base level by adding player level over EQUIPLVLBOOSTRATIO.
 * The values of earliestBagSlot and equipsBagOccupiedSlots are managed.
 * If equipsBag is full (EQUIPSBAGSIZE), user has to choose one Equip not currently equipped to be deleted and replaced by the new one.
 * Special Equip functions are also set up here.
 * @see Fighter
 * @see Equip
 * @see equipClass
 * @see quality
 * @see EQUIPLVLBOOSTRATIO
 * @see EQUIPSBAGSIZE
 * @see stringFromQuality()
 * @see stringFromEquips()
 * @param f The Fighter pointer at hand.
 * @param beast The integer for drops coming from a beast kill if true.
 * @return No return.
 */
void dropEquip(Fighter* player, int beast) {

	//Select a basic item from the list
	int drop = rand() % (EQUIPSMAX + 1); 
	//Randomise quality
	quality q = rand() % (QUALITIESMAX + 1);
	
	//Prepare the item	
	Equip* e = (Equip*)malloc(sizeof(Equip));

	//Get the base item and copy the stats to the drop
	Equip* base = &equips[drop];
	
	e->class = base->class;
	e->type = base->type;
	e->qual = q;
	strcpy(e->name,base->name);
	strcpy(e->desc,base->desc);

	e->qty = 1;
	e->equipped = 0;

	e->perksCount = 0;

	//Calc drop level
	e->level = base->level + round(player->level / EQUIPLVLBOOSTRATIO);
	
	//Chance for better leveled item
	if ( ( rand() % 8 ) - (player->luck / 10) <= 0) { //Should use a defined constant
		e->level += 1; //At least a simple +1	
		if ( ( rand() % 25 ) - (player->luck / 10) <= 0) { //Should use a defined constant
			e->level += 1; //A bonus roll for another +1	
	
		}
	}
	
	float boostFactor = 0.7;
	
	float lvlBoost = boostFactor * (e->level - 1);

	e->atk = round((base->atk * 1.0) + lvlBoost);
	e->def = round((base->def * 1.0) + lvlBoost);
	e->vel = round((base->vel * 1.0) + lvlBoost);
	e->enr = round((base->enr * 1.0) + lvlBoost);


	//Bonus stats on better quality items? Simple for now
	//
	if (q == Good) {
		e->atk += (rand() % 3);	//Should use a defined constant	
		e->def += (rand() % 3);	//Should use a defined constant	
		e->vel += (rand() % 3);	//Should use a defined constant	
		e->enr += (rand() % 2);	//Should use a defined constant	
	} else if (q == Bad) {
		e->atk -= (rand() % 3);	//Should use a defined constant	
		e->def -= (rand() % 3);	//Should use a defined constant	
		e->vel -= (rand() % 3);	//Should use a defined constant	
		e->enr -= (rand() % 2);	//Should use a defined constant	
		if (e->atk < 0) { e->atk = 0; };
		if (e->def < 0) { e->def = 0; };
		if (e->vel < 0) { e->vel = 0; };
		if (e->enr < 0) { e->enr = 0; };
	}

	//Possible perks for the Equip
	
	for (int i = 0; i < (EQUIPPERKSMAX); i++) {
		int chance = 20;

		if (q == Good) {
			chance *= 1.5;
		}

		if ( (rand() % 100) < chance || (beast && e->perksCount == 0)) {

			e->perksCount += 1;
	
			Perk* p = (Perk*)malloc(sizeof(Perk));
			p->class = rand() % (PERKSMAX +1) ;
			//p->name = (char*)malloc(sizeof(nameStringFromPerk(p->class)));
			strcpy(p->name,nameStringFromPerk(p->class));
			//p->desc = (char*)malloc(sizeof(descStringFromPerk(p->class)));
			strcpy(p->desc,descStringFromPerk(p->class));
			p->innerValue = 1;
			e->perks[(e->perksCount -1)] = p;
		}
	}

	//Set value of how many bonuses we have. Although Equip only has ONE function pointer field for now
	e->bonus = base->bonus;
	//Randomise if the item will have an effect function. 
	//Not yet implemented
	//Initialisation of function happens here
	//
	//e->equip_fun =    ;

	lightBlue();
	printf("\n\n\tYou found %s %s!\n",stringFromQuality(q),stringFromEquips(drop));
	white();
	
	if (player->equipsBagOccupiedSlots >= EQUIPSBAGSIZE) { //Handle full bag by asking player if we throw something away
		lightRed();
		printf("\tYour bag is full. Want to throw something away?\n");
		white();
		
		for (int i = 0; i < player->equipsBagOccupiedSlots ; i++) {
			Equip* current = (Equip*)player->equipsBag[i];
			if (current->equipped) { green(); printf("ON  "); white(); }; 

			printf("(%i",i); //This starts lines with the item index.
			printEquipStats(current);
		} ;

		int selected = -1;
		int c = -1;
		Equip* s;

		while (selected < 0 || selected >= player->equipsBagOccupiedSlots || c != 1) {
		
			c = scanf("%i",&selected);
			scanf("%*c");
			
			if (c == 1) {
				s = (Equip*) player->equipsBag[selected];
				if (s->equipped) {
					printf("You can't delete an equipped item!");
					selected = -1;
				}
			}
		}


		
		printf("\tAre you sure you want to delete %s %s ?\n\t\t[0 to confirm, 1 to go back]\n",stringFromQuality(s->qual),s->name);
		
		int n = -1;
		c = -1;
		while (n < 0 || c != 1) {
			c = scanf("%i",&n);
			scanf("%*c");
		}
		
		if (n != 0) { //Abort deletion, drop will not be awared.
			
			int perkscount = e->perksCount;
			if (perkscount > 0) {
				for (int i=0; i < perkscount; i++) {
					free(e->perks[i]);
				}
			}
			free(e);
			return;
		};	
		
		Equip* toDelete = (Equip*) player->equipsBag[selected];
		int perkscount = toDelete->perksCount;
		if (perkscount > 0) {
			for (int i=0; i < perkscount; i++) {
				free(toDelete->perks[i]);
			}
		}
		free(toDelete);
		
		//Add drop to player bag replacing the one at the selected index
		player->equipsBag[selected] = (struct Equip*) e; 

		//Update stats
		player->stats->equipsfound++;
	
		screenTime(1.5);
		return;
	};//End if bag is full
	
	//Add drop to player bag
	player->equipsBag[player->earliestBagSlot] = (struct Equip*) e;

	player->earliestBagSlot += 1; //Advance equips bage pointer
	player->equipsBagOccupiedSlots += 1; //Advance equips bage size counter
		
	//Update stats
	player->stats->equipsfound++;
	
	screenTime(1.5);
}

/**
 * Takes a Fighter pointer value and adds a random Artifact to artifactsBag.
 * The Artifact is randomised according to ARTIFACTSMAX until one which was not found yet drops.
 * Special Equip functions are also set up here.
 * @see Fighter
 * @see Artifact
 * @see artifactClass
 * @see stringFromArtifacts()
 * @param f The Fighter pointer at hand.
 * @return No return.
 */
void dropArtifact(Fighter* player) {
	int drop = 0;
	do {
		drop = rand() % (ARTIFACTSMAX + 1);
	} while (artifactsBag[drop].qty != 0 ); //We reroll to get one we don't have
	
	purple();
	printf("\n\n\tYou found a %s!\n",stringFromArtifacts(drop));
	white();
	artifactsBag[drop].qty++;
		
	//Update stats
	player->stats->artifactsfound++;
		
	screenTime(1.5);
}

/**
 * Takes a Fighter and a Enemy pointers and compares their stats to determine who gets damaged and returns the fightStatus value.
 * On enemy death, there's a chance to call dropConsumable, dropEquip or dropArtifact (guaranteed for beast enemies).
 * @see Fighter
 * @see Enemy
 * @see statReset()
 * @see statResetEnemy()
 * @see stringFromEClass()
 * @see dropConsumable()	
 * @see dropEquip()	
 * @see dropArtifact()	
 * @param f The Fighter pointer at hand.
 * @param e The Enemy pointer at hand.
 * @return No return.
 */
int fight(Fighter* player, Enemy* e){

	system("clear");
	
	int res = 0;
	//Stat comparisons
	//
	
	int atkdelta = (player->atk + player->equipboost_atk) - e->atk - (rand() % 3 ) - 1 ; //Skewed with defender
	int defdelta = (player->def + player->equipboost_def) - e->def + (rand() % 2 ) + 1  ; //Skewed with attacker
	int veldelta = (player->vel + player->equipboost_vel) - e->vel + (rand() % 3 ) + 1 ;

	int atkOnPlayer = e->atk - (player->def + player->equipboost_def + (player->vel / 6)) ;
	int atkOnEnemy = (player->atk + player->equipboost_atk)- (e->def + (e->vel / 6)) ;

	int damageDealt = -1;

	if (veldelta >= 0) {
		
		if ( atkOnEnemy > 3 ) {
			damageDealt = atkOnEnemy;
			e->hp -= damageDealt;
			res=DMG_DEALT;
		} else if ( atkOnEnemy >= 0) {
			damageDealt = abs(atkOnEnemy - atkdelta);
			e->hp -= damageDealt;
			res = DMG_DEALT;	
		} else {
			if ( atkOnEnemy < -3 ) {
				damageDealt = fabsf(atkOnPlayer - 0.75F * (player->vel + player->equipboost_vel) );
			} else {
			       	damageDealt = abs (atkOnPlayer - 1 );
			}
			player->hp -= damageDealt;
			res = DMG_TAKEN;
		}
	} else { 
		atkdelta = -atkdelta;
		if (atkOnPlayer > 3) {
			damageDealt = atkOnPlayer;
			player->hp -= damageDealt;
			res = DMG_TAKEN;
		} else if ( atkOnPlayer >= 0 ) {
			damageDealt = abs( atkOnPlayer - atkdelta);
			player->hp -= damageDealt;
			res = DMG_TAKEN;
		} else {
			if ( atkOnPlayer < -3 ) {
				damageDealt = fabsf(atkOnEnemy - 0.75F * e->vel );
			} else {
				damageDealt = abs (atkOnEnemy - 1 );
			}
			e->hp -= damageDealt;
			res = DMG_DEALT;
		}
	}

	int yourhit = (res == DMG_DEALT ) ? 1 : 0 ;
	//system("clear");
	char victim[25];
	
	if (!yourhit) {

		e->vel--;
		e->atk--;
		e->def -= 2;

		//Check if someone earned a stat reset after the fight	
		statReset(player,0);
		statResetEnemy(e,0);	
	
		strcpy(victim,player->name);
	} else {

		player->vel--;
		player->atk--;
		player->def -= 2;

		//Account for vampirism perk
		int vampire_perks = player->perks[VAMPIRISM]->innerValue;
		if (vampire_perks > 0) {
			player->hp += floor(damageDealt * (0.1 * vampire_perks));
			if (player->hp >= player->totalhp) { player->hp = player->totalhp;}; 
		}
		
		//Account for burn on touch perk
		int hotbody_perks = player->perks[HOT_BODY]->innerValue;
		if (hotbody_perks > 0) {
			int burnchance = 11 - hotbody_perks;
			if (rand() % burnchance == 0) {
				e->status = Burned; //Set status to Burned. May need change to manage multiple statuses active at once
				setCounter((Turncounter *)e->counters[Burned],2); //Give 2 turns of Burned status
			}
		}
		
		//Account for poison on touch perk. Order of checks with hot_body perk may cause issues?
		int biohazard_perks = player->perks[BIOHAZARD]->innerValue;
		if (biohazard_perks > 0) {
			int poisonchance = 11 - biohazard_perks;
			if (rand() % poisonchance == 0) {
				e->status = Poison; //Set status to Poison. May need change to manage multiple statuses active at once
				setCounter((Turncounter *)e->counters[POISON],2); //Give 2 turns of Poison status
			}
		}
		
		//Check if someone earned a stat reset after the fight	
		statResetEnemy(e,0);	
		statReset(player,0);
		
		strcpy(victim,stringFromEClass(e->class));
	}
	
	if (yourhit) {
		yellow();
	} else {
		red();
	}

	printf("\n\n\t%s was hit.\t(%i DMG)\n",victim,damageDealt);
	white();

	//Rolls
	//
	//Critical hit roll
	
	//Account for critboost_chance perks
	int critMax =  round(10.0 - floor(player->luck/5) - (1.5* player->perks[CRITBOOST_CHANCE]->innerValue)  ) ;
	
	int critRes = (rand() % critMax);

	if (res == DMG_DEALT && (critRes <= 0)) { 

		//Account for critboost_dmg perks
		int dmgboost_perks = player->perks[CRITBOOST_DMG]->innerValue;
		damageDealt *= (0.30 + (0.12* dmgboost_perks));
		e->hp -= damageDealt;
		lightPurple();
		printf("\n\n\tA critical hit!\t(%i DMG)\n",damageDealt);
		white();

		//Update stats
		player->stats->criticalhits++;
	}
	//Check for deaths -> exit condition from loop
	//
	//
	//
	if (e->hp <= 0 ) {
		res = KILL_DONE;
		
		//Account for runic circle perk
		int runic_perks = player->perks[RUNIC_MAGNET]->innerValue;
		if (runic_perks > 0) {
			player->energy += round(0.51 * runic_perks);
		}
		
		if (e->beast) {
			purple();
		} else {
			green();
		}
			printf("\n\n\t%s fainted.\n",stringFromEClass(e->class));
		white();
		screenTime(1.5);
		
		//Update stats
		player->stats->enemieskilled++;
	}
	if (player->hp <= 0) {
		res = DEATH;
	}

	//Consumable drop, guaranteed on killing a beast
	if (res == KILL_DONE && (e->beast || ( (rand() % 9)  - (player->luck/10)  <= 0 ))) {
		dropConsumable(player);	
	}	
	
	//Equip drop, guaranteed on killing a beast
	if (res == KILL_DONE && (e->beast || ( (rand() % 12)  - (player->luck/10)  <= 0 ))) {
		dropEquip(player,e->beast);	
	}	
	
	//Artifact drop (if we don't have all of them), guaranteed on killing a beast
	if ( (player->stats->artifactsfound != ARTIFACTSMAX + 1)  && res == KILL_DONE && (e->beast || ( (rand() % 1001)  - (player->luck/10)  <= 0 ))) {
		dropArtifact(player);	
	}	

	return res;
}

/**
 * Takes a Fighter and a Enemy pointers, and an integer denoting the consumableClass of the Consumable to use.
 * If qty value for the Consumable is 0, we have an early return. Otherise effect is applied and qty is decreased.
 * @see Fighter
 * @see Enemy
 * @see Consumable
 * @see consumableClass
 * @param f The Fighter pointer at hand.
 * @param e The Enemy pointer at hand.
 * @param i The consumableClass value.
 * @return No return.
 */
void useConsumable(Fighter* f, Enemy* e, int i){
	//Mountain of ifs? Where are my damn maps
	
	if (consumablesBag[i].qty == 0) {
		return;
	}	
	if (consumablesBag[i].class == Potion) {
		f->hp=f->totalhp;
		green();
		printf("\n\n\n\tYour health was restored.\n\n");
		white();
	} else if (consumablesBag[i].class == Rock) {
		e->def -= 8;
		e->hp -= 2;
		printf("\n\n\n\t%s got smacked.",stringFromEClass(e->class));
		red();
		printf("\t-8 DEF and -2 HP.\n\n");
		white();
	} else if (consumablesBag[i].class == Bread) {
		f->vel -= 1 ;
		f->def += 6 ;
		f->atk += 4;
		printf("\n\n\n\tYou ate elvish bread.");
		green();
		printf("\t+6 DEF , +4 ATK, ");
		yellow();
		printf("-1 VEL\n\n");
		white();
	} else if (consumablesBag[i].class == Coffee) {
		f->vel += 6 ;
		f->atk += 2 ;
		f->def -= 2 ;
		printf("\n\n\n\tYou drank hot coffee.");
		green();
		printf("\t+6 SPE , +2 ATK, ");
		yellow();
		printf("-2 DEF\n\n");
		white();
	} else if (consumablesBag[i].class == Powergem) {
		printf("\n\n\n\tYou became wiser.");
		green();
		printf("\t+25 XP\n\n");
		white();
		checkremainder(f,25);	
	} else if (consumablesBag[i].class == Magicbean) {
		printf("\n\n\n\tYou aren't tired anymore.");
		green();
		printf("\tEnergy was restored\n\n");
		white();
		f->energy = f->totalenergy; //Refill energy
	} else {
		return;
	}

	consumablesBag[i].qty--;
};

/**
 * Takes an integer denoting the consumableClass and returns the respective qty value from consumablesBag at the provided index.
 * @see Consumable
 * @see consumableClass
 * @param n The consumableClass value.
 * @return int The qty value in consumablesBag for selected consumableClass.
 */
int getConsumableQty(int n) {
	return consumablesBag[n].qty;	
}

/**
 * Sets the qty value to 0 for all Consumable in consumablesBag with positive qty.
 * @see Consumable
 * @see consumableClass
 * @see CONSUMABLESMAX
 * @see getConsumableQty()
 * @return No return.
 */
void emptyConsumables() {
	for (int i=0; i < CONSUMABLESMAX + 1; i++) {
		if (getConsumableQty(i) > 0 ) {
			consumablesBag[i].qty = 0;
		}
	};
}

/**
 * Takes a Fighter and a Enemy pointers and prints content of consumablesBag before asking user input to use a Consumable.
 * @see Fighter
 * @see Enemy
 * @see Consumable
 * @see CONSUMABLESMAX
 * @see printConsumableStats()
 * @see useConsumable()
 * @param f The Fighter pointer using the Consumable.
 * @param e The Enemy pointer for current enemy.
 * @return No return.
 */
void handleConsumables(Fighter* f, Enemy* e) {
	system("clear");
	
	printf("\tConsumables:\tQuantity:\n\n");
	int empty = 1; //To print something when you have no consumables

	for (int i = 0; i < CONSUMABLESMAX +1; i++) {
		Consumable * c = &consumablesBag[i];
		if (c->qty > 0) {
			printConsumablesStats(c);
			if (empty) { empty = 0; }; //Bag is not empty
		}
	}

	if (empty) {
		lightYellow();
		printf("\t\t\tYou have nothing.\n\n");
		white();
		screenTime(1);
		system("clear");
	} else {
		int num = -1;
		printf("\n\n\tInsert (x) number or {Return} to exit:\n\n\t");
		//scanf("%i",&num);
	
		char c[25];
		fgets(c, sizeof c, stdin);
		if (c[0]=='\n') {
			return;
		}

		//Converting the first char into an integer...
		num = c[0] - '0';

		useConsumable(f,e,num);
	}
}

/**
 * Sets the qty value to 0 and the active flag to false for all Artifacts in artifactsBag with positive qty.
 * @see Artifact
 * @see artifactClass
 * @see ARTIFACTSMAX
 * @return No return.
 */
void emptyArtifacts() {
	for (int i=0; i < ARTIFACTSMAX + 1; i++) {
		if (artifactsBag[i].qty > 0 ) {
			artifactsBag[i].qty = 0;
			artifactsBag[i].active = 0;
		}
	};
}

/**
 * Takes a Fighter pointer and prints content of artifactsBag.
 * @see Fighter
 * @see Artifact
 * @see ARTIFACTSMAX
 * @see printArtifactStats()
 * @param f The Fighter pointer at hand.
 * @return No return.
 */
void handleArtifacts(Fighter* f) {
	system("clear");
	printf("\tArtifacts:\n\n");
	
	int empty = 1; //To print something when you have nothing
	for (int i = 0; i < ARTIFACTSMAX +1; i++) {
		Artifact* a = &artifactsBag[i];
		if (a->qty > 0) {
			printArtifactStats(a);
			if (empty) { empty = 0 ;};
		}
	}

	if (empty) {
		lightYellow();
		printf("\t\t\tYou haven't found any artifact yet.\n\n");
		white();
		screenTime(1);
		system("clear");
	}
}

/**
 * Takes a Fighter pointer and, for all Equip in equipsBag field with positive qty, sets qty to 0 and frees the Equip pointer.
 * @see Fighter
 * @see Equip
 * @return No return.
 */
void emptyEquips(Fighter* player) {
	//We REALLY need to be sure the items are in successive cells
	//Still thinking linked lists would be better than an array 
	int occupied = player->equipsBagOccupiedSlots;

	for (int i=0; i < occupied ; i++) {
		
		Equip* e = (Equip*) player->equipsBag[i];
		if (e->qty > 0 ) {
			free(e);
		}
	};
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
 * @return No return.
 */
void printLoadout(Fighter* f) {
	system("clear");
	printf("\tLoadout:\t\t\tATK\tDEF\tVEL\tENR\n\t\t\tLvl & Perks\n");

	for (int i = 0; i < EQUIPZONES + 1; i++ ) {
		Equipslot* s = (Equipslot*) f->equipslots[i];
		if (s->active) {
			printf("%s\t",stringFromEquipzones(s->type));
			printQualityColor(s->item->qual);
			printf("%s %s\t",stringFromQuality(s->item->qual),s->item->name);
			white();
			printf("L%i",s->item->level);
			white();
			printf("\t");
			if (s->item->atk != 0) {
				green();
				printf("%i",s->item->atk);
				white();
			}
			printf("\t");
			if (s->item->def != 0) {
				green();
				printf("%i",s->item->def);
				white();
			}
			printf("\t");
			if (s->item->vel != 0) {
				green();
				printf("%i",s->item->vel);
				white();
			}
			printf("\t");
			if (s->item->enr != 0) {
				green();
				printf("%i",s->item->enr);
				white();
			}
			if (s->item->perksCount != 0) {
				lightPurple();
				for (int i = 0 ; i < s->item->perksCount; i++) {
					printf("\n\t\t\t%s",s->item->perks[i]->name);
				}
				white();
			}
			printf("\n");
		} else {
			printf("\t%s\tEMPTY\n",stringFromEquipzones(s->type));
		}
	}
}

/**
 * Takes a Fighter, a Enemy and a Path pointers and asks user input to manage the Equips in fighter's equipsBag.
 * User can view the bag, equip a new item, view its loadout or exit.
 * Equipping an item causes all the relevant field in Fighter to be updated.
 * @see Fighter
 * @see Equipslot
 * @see Equipzone
 * @see EQUIPZONES
 * @param f The Fighter pointer with a equipsBag.
 * @param e The Enemy pointer for current enemy.
 * @param p The Path pointer of the current game.
 * @return No return.
 */
void handleEquips(Fighter* f,Enemy* e, Path* p) {
	int occupied = f->equipsBagOccupiedSlots;
	
	if (occupied == 0) {
		lightYellow();
		printf("\t\t\tYou haven't found anything yet.\n\n");
		white();
		screenTime(1);
		system("clear");
		return;
	}

	//system("clear");
	
	//Always prints loadout
	printLoadout(f);

	printf("\n");

	printf("\t[ 0\tEquips Bag\t\t1\tEdit loadout\n");
	printf("\t  2\tCheck loadout\t\t3\tExit ]\n");

	int n = -1, c = -1;
	while (n < 0 || n > 3 || c != 1) {
		c = scanf("%i",&n);
		scanf("%*c");
	}

	switch (n) {
		case 0: { //List all equips in bag
			printf("\tEquips:\n\n");
	
			int entriesPerPage = 10;
			int displayed = 0;
	
		
			for (int i = 0; i < occupied ; i++) {
			Equip* e = (Equip*) f->equipsBag[i];
		
			if (displayed % entriesPerPage == 0) { printf("Page %i)\n",displayed / entriesPerPage); };
			if (e->qty > 0) {
				
				displayed++;

				if (e->equipped) { green(); printf("ON  "); white(); }; 
				printf("(%i",i); //This starts lines with the item index.
				printEquipStats(e);
			}
		
			if (displayed % entriesPerPage == 0) {
				printf("\t[ 0\tNext\t\t1\tPrevious\n");
				printf("\t\t\t\t2\tExit ]\n");
			
				int n = -1, c = -1;
				while (n < 0 || n > 2 || c != 1) {
					c = scanf("%i",&n);
				}
				switch (n) {
					case 0: {
						//we let the cycle continue
					}
					break;
					case 1: {
						i -= entriesPerPage; //We roll the cursor back
						displayed -= entriesPerPage; //We roll the counter back
					}
					break;
					case 2: {
						system("clear");
						return; //We go back to menu
					}
					break;
				}
				system("clear");
			}; //End if for multiple pages
			if ( i == occupied - 1)  {
				lightYellow();
				printf("\n\n\tDo you want to start from the beginning?\t(1 - Yes)\t(0 - No)\n");
				white();
				int n = -1, c = -1;
				while (n < 0 || n > 2 || c != 1) {
					c = scanf("%i",&n);
					scanf("%*c");
				}
				switch (n) {
					case 1: {
						i = -1;
						displayed = 0;
						system("clear");
						printf("\tEquips:\n\n");
					}
					break;
				};

			};
		}
		}; //End case printBagEquips
		break;
		case 1: { //Equip different items
			printf("\tInsert equip number:\n");
			int n = -1, c = -1;
			while (n < 0 || n > f->equipsBagOccupiedSlots - 1 || c != 1) {
				c = scanf("%i",&n);
				scanf("%*c");
			}
			//Retrieve item info
			Equip* e = (Equip*) f->equipsBag[n];
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
		};
		break;
		case 2: { //Print all equipped items

			printLoadout(f);
			printf("\n\t{Enter} to go back\n");
			scanf("%*c");
			//screenTime(2);
		};
		break;
		case 3: {
			system("clear");
			return;
		};
		break;
	};

}

/**
 * Takes a Fighter, a Enemy and a Path pointers (and integers for current room and enemy indexes) and asks user input to use an unlocked special attack.
 * If the selected slot is enabled and fighter has enough energy, GET_CALLBACK() is called to have the special attack execute. 
 * @see Fighter
 * @see SpecialSlot
 * @see SPECIALSMAX
 * @see GET_CALLBACK()
 * @param f The Fighter pointer with a equipsBag.
 * @param e The Enemy pointer for current enemy.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @return No return.
 */
void handleSpecials(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	system("clear");
	printf("\tYour special moves:\t\tNumber:\t\tEnergy Cost:\n\n");
	for (int i = 0; i < SPECIALSMAX + 1; i++) {
		Specialslot* s = f->specials[i];
		if (s->enabled) {
			printf("\t\t%s\t\t%i\t\t%i\n",stringFromSpecial(f->class,i),i,s->cost);
		}
	}
	
	int num = -1;
	printf("\n\n\tInsert special number or {Return} to exit:\n\n\t");
	
	char c[25];
	fgets(c, sizeof c, stdin);
	if (c[0]=='\n') {
		return;
	}


	//Converting the first char into an integer...
	num = c[0] - '0';

	if (num < SPECIALSMAX + 1) {	//Check if selected move number is lower than the maximum
		Specialslot* selected = f->specials[num];
		
		//Check if the selected move is enabled AND if user has enough energy
		//Considering the equipboost in this way makes it so that you can't get to 0 energy
		if (selected->enabled && selected->cost <= f->energy + f->equipboost_enr) {
			
			//callback to the move
			GET_CALLBACK(selected->move,callback_special_t)(f,e,p,roomIndex,enemyIndex); 
		}
	}
}

/**
 * Takes a Fighter pointer and prints its stats field with all the values, formatted.
 * @see Fighter
 * @see countStats
 * @param f The Fighter pointer with stats to print.
 * @return No return.
 */
void handleStats(Fighter* player){

	system("clear");
	printStats(player);

	printf("\n\t\tEnemies killed:\t\t%i\n\t\tCritical hits done:\t%i\n",player->stats->enemieskilled,player->stats->criticalhits);
	printf("\t\tConsumables found:\t\t%i\n\t\tRooms completed:\t%i\n",player->stats->consumablesfound,player->stats->roomscompleted);
	printf("\t\tEquips found:\t\t%i\n\t\tArtifacts found:\t\t%i\n",player->stats->equipsfound,player->stats->artifactsfound);

}

/**
 * Takes a Fighter pointer and prints its stats and a game over message.
 * Consumables and Artifacts are emptied before freeing the player's specials, counters, perks and stats field.
 * At last, the player pointer is freed.
 * @see Fighter
 * @see handleStats()
 * @see emptyConsumables()
 * @see emptyArtifacts()
 * @param f The Fighter pointer to free.
 * @return No return.
 */
void death(Fighter* player) {
	
	lightRed();
	handleStats(player);
	printf("\t\tLuck:\t\t%i\n",player->luck);
	red();
	printf("\n\n\tYOU DIED.\n");
	white();
	

	emptyConsumables();
	emptyArtifacts();
	//Free player special slots
	for (int i=0; i < (SPECIALSMAX + 1) ; i++) {
		free(player->specials[i]);
	}
	//Free player equipbag
	int total = player->equipsBagOccupiedSlots;
	for (int i=0; i < (total ) ; i++) {
		Equip* e = (Equip*) player->equipsBag[i];
		int perkscount = e->perksCount;
		if (perkscount > 0) {
			for (int i=0; i < perkscount; i++) {
				free(e->perks[i]);
			}
		}
		free(e);
	}
	//Free player equip slots
	for (int i=0; i < (EQUIPZONES + 1) ; i++) {
		Equipslot* s = (Equipslot*) player->equipslots[i];
		/*int perkscount = -1;
	       
		if (s->active) { perkscount = s->item->perksCount;};
		if (perkscount > 0) {
			for (int i=0; i < perkscount; i++) {
				free(s->item->perks[i]);
			}
			free(s->item);
		}*/
		free(s);
	}
	//Free player turnCounters
	for (int i=0; i < (COUNTERSMAX + 1) ; i++) {
		Turncounter* c = (Turncounter*) player->counters[i];
		free(c->desc);
		free(c);
	}
	//Free player perks
	for (int i=0; i < (PERKSMAX + 1) ; i++) {
		free(player->perks[i]);
	}
	free(player->stats);
	free(player);
}

/**
 * Takes a Enemy pointer and frees its allocated memory.
 * The counters field is freed before the enemy pointer.
 * @see Enemy
 * @param f The Fighter pointer to free.
 * @return No return.
 */
void e_death(Enemy* e) {
	
	//Free enemy special slots
	//for (int i=0; i < SPECIALSMAX + 1 ; i++) {
	//	free(player->specials[i]);
	//}
	//Free enemy turnCounters
	for (int i=0; i < (COUNTERSMAX + 1) ; i++) {
		Turncounter* c = (Turncounter*) e->counters[i];
		free(c->desc);
		free(c);
	}
	free(e);
}

/**
 * Asks the user is they want to continue and returns the choice.
 * @return int True for trying again, false otherwise.
 */
int retry() {
	lightGreen();
	printf("\n\nWant to try again?\n\n\t\t0 - No\t\t\t1 - Yes\n\n");
	white();

	char c[25];
	fgets(c, sizeof c, stdin);

	switch(c[0]) {
		case '0':
			{
				return 0;
				break;
			}
		case '1':
			{
				return 1;
				break;
			}
		default:
			return 0;
			break;
	}
}

/**
 * Takes a Fighter, a Enemy and a Path pointers (and integers for current room and enemy indexes) and asks user input to execute debug actions.
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
 * @param player The Fighter pointer at hand.
 * @param e The Enemy pointer for current enemy.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param currentEnemyNum The index of current enemy.
 * @return No return.
 */
void debug(Fighter* player, Enemy* e, Path* p, int roomIndex,int currentEnemyNum) {
	
	system("clear");

	char choice;
	printf("\tWhat do you want to do?\n\n\t[\n\t\t0\tAdd xp\t\t\t1\tGive consumable\n");
	printf("\t\t2\tReset stats\t\t3\tReset Enemy\n");
	printf("\t\t4\tAlter luck\t\t5\tPrint Perks\n");
	printf("\t\t6\tUnlock special\t\t7\tUnlock Artifact\n");
	printf("\t\t8\tPrint counters\t\t9\tGive Equip\n");
	printf("\t\t{Return}\tBack to game\t\t\t]\n");
	char ch[25];
	fgets(ch, sizeof ch, stdin);

	int c=0,n=-1;
	switch(ch[0]) {
		case '0': {
			int xp;
			do {
				printf("\nHow much?\n");
			} while ((c = scanf("%i",&xp)) != 1 || xp < 0);
			scanf("%*c");
			checkremainder(player,xp);
		}
		break;
		case '1': {
			int q=-1;
			do {
				printf("\nInsert consumable number:\n");
			} while ((c = scanf("%i",&n)) != 1 || n > CONSUMABLESMAX || n < 0 );
			scanf("%*c");
			do {
				printf("\nInsert quantity:\n");
			} while ((c = scanf("%i",&q)) != 1 && q <= 0 );
			scanf("%*c");

			consumablesBag[n].qty += q;
		}
		break;
		case '2':
			statReset(player,1);
		break;
		case '3':
			statResetEnemy(e,1);
		break;
		case '4': { 
			printf("\nCurrent luck: %i\tRL: %i\n",player->luck,p->luck);
			
			do {
				printf("\nInsert new luck:\n");
			} while ((c = scanf("%i",&n)) != 1 && n > 0 );
			scanf("%*c");
				//Copy-pasted the calc for player luck... might need a function
				p->luck = n;
				
				player->luck = (p->luck * MAXPLAYERLUCK) / MAXLUCK ;
		}
		break;
		case '5': { 
			/* Old code invoking the macro for special moves directly. Used for testing.
			 * int s = -1;
			 * printf("Insert special num:\n");
			 * scanf("%i",&s);
			 * scanf("%*c");
			 * printf("Read: %i\n",s);
			 * GET_CALLBACK(s,callback_special_t)(player,e,p,roomIndex,currentEnemyNum); 
			*/
			
			printActivePerks(player);
		}
		break;	
		case '6': {
			unlockSpecial(player);
		}
		break;
		case '7': { 
			int n=-1;
			do {
				printf("\nInsert artifact number (0<n<%i):\n",ARTIFACTSMAX + 1);
			} while ((c = scanf("%i",&n)) != 1 || n > ARTIFACTSMAX || n < 0 );

			scanf("%*c");
			artifactsBag[n].qty += 1;
			artifactsBag[n].active = 0;
			player->stats->artifactsfound += 1;
		}
		break;
		case '8': {
			printCounters((Turncounter **)player->counters);
			printf("\n------------------------------------------------------\n");
			printCounters((Turncounter **)e->counters);
		}
		break;
		case '9': {
			int q = -1;
			int c = -1;
			do {
				printf("\nInsert quantity:\n");
			} while ((c = scanf("%i",&q)) != 1 && q <= 0 );
			scanf("%*c");
			
			for (int i=0; i<q;i++) {dropEquip(player,1);};
		}
		break;
		default : 
			return;
		break;
	}	

}

/**
 * Takes a Fighter pointer and prints its stats and a quitting message, before quitting the program.
 * @see Fighter
 * @see printStats()
 * @param p The Fighter pointer at hand.
 * @return No return.
 */
void quit(Fighter* p) {
	system("clear");
	printf("\n\n\tTHANKS 4 PLAYING\n");
	printStats(p);
	printf("\n");
	exit(1);
}

/**
 * Takes an integer for room index and a Fighter and a Path pointers and handles the progression of the room.
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
 * @see debug()
 * @see handleArtifacts()
 * @see applyStatus()
 * @see applyEStatus()
 * @param index The index of current room.
 * @param player The Fighter pointer at hand.
 * @return When room is cleared, should return KILL_DONE.
 */
int room(int index, Path* p, Fighter* player) {
	
	screenTime(1.5);
	system("clear");

	printf("\n\tYou are now in room %i.\n",index);
	int enemies = (rand() % 6) + 1 ;
	
	//At least 2 enemies in every Xth room, up to 7
	if (index % BEASTROOM == 0) {
		enemies++;
	} ;
		
	printf("\tThere are %i enemies.\n",enemies);
	int fightStatus = NO_DMG;

	for (int i = 0; i < enemies ;) {
			
		fightStatus = NO_DMG;
		Enemy* e = (Enemy *) malloc (sizeof(Enemy));
		//Load up an enemy
		prepareRoomEnemy(e, player, index, enemies, i);
		 
		printSpawnMessage(e, index, i);

		//Reset player permboosts... and calc them again...
		resetPermboosts(player);
		
		//Reset artifacts state, for the ones we have unlocked 
		resetArtifactsState(); 
		//Apply artifacts 
		applyArtifacts(player, e);
		
		//Calc permboosts and turn boosts
		//Turn boosts

		//Perm boosts
		applyPermboosts(player);	

		//Print first battle stats for this enemy
		printBattleStats(player,e);
	
		//Fight preparation is done, on to turn choice
		int choice = 999;

		while (!( fightStatus == DEATH || fightStatus == KILL_DONE || choice == QUIT ) ){


			if (e->hp <= 0 ) { //Check if enemy hp got below zero during last loop
				
				fightStatus = KILL_DONE; //Set the variable that will makes us skip the loop when we continue

				if (e->beast) {
					purple();
				} else {
					green();
				}
				printf("\n\n\t%s fainted.\n",stringFromEClass(e->class));
				white();
				screenTime(1.5);
			
				//Update stats
				player->stats->enemieskilled++;
				
				//Consumable drop, guaranteed on killing a beast
				if (e->beast ||  ((rand() % 9)  - (player->luck/10)  <= 0) )  {
					dropConsumable(player);	
				}	
				
				//Equip drop, guaranteed on killing a beast
				if (e->beast ||  ((rand() % 12)  - (player->luck/10)  <= 0) )  {
					dropEquip(player,e->beast);	
				}	

				//Artifact drop (if we don't have all of them), guaranteed on killing a beast
				if ( (player->stats->artifactsfound != ARTIFACTSMAX + 1)  && (e->beast || ( (rand() % 1001)  - (player->luck/10)  <= 0 ))) {
					dropArtifact(player);	
				}	

				//Get xp and free memory from enemy
				
				//Account for harvester perk
				int harvester_perks = player->perks[HARVESTER]->innerValue;
				if (harvester_perks > 0) {
					float xpboost = harvester_perks * 0.1;
					e->xp *= (1 + xpboost);
				}
					giveXp(player,e);

					e_death(e);
				
				//Go to next battle
					i++;
					system("clear");		
					
					if (i!=enemies) {
						lightYellow();
						printf("\n%i enemies left in room %i.\n",enemies-i,index);
						white();
					}
					continue; //Check while condition again...
			} //End check for deaths 
	
			choice = getTurnChoice();
			if (choice == FIGHT) {
				fightStatus = fight(player,e);
				//Lost battle
				if (fightStatus == DEATH) {

					//Account for oracle gift perk
					int oracle_perks = player->perks[ORACLE_GIFT]->innerValue;
					if (oracle_perks > 0) {
						system("clear");
						blue();
						printf("\n\n\t\tYou get a second chance, as the prophecy said.\n");
						white();
						player->hp = round(player->totalhp / 3);


						player->perks[ORACLE_GIFT]->innerValue -= 1;
					
						e_death(e);
						break; //We go to next enemy
					}
					
					e_death(e);
					
					death(player);
					return DEATH;
				} else if (fightStatus == KILL_DONE) {
				//Win, get xp and free memory from enemy
					giveXp(player,e);

					e_death(e);
				
				//Go to next battle
					i++;

					system("clear");		
					
					if (i!=enemies) {
						lightYellow();
						printf("\n%i enemies left in room %i.\n",enemies-i,index);
						white();
					}
					fightStatus=NO_DMG;
					break;
				}
			} else if (choice == SPECIAL) {
				handleSpecials(player,e,p,index,i);
			} else if (choice == CONSUMABLE) {
				handleConsumables(player,e);
			} else if (choice == EQUIPS) {
				handleEquips(player,e,p);
			} else if (choice == PERKS) {
				printActivePerks(player);
			} else if (choice == STATS) {
				handleStats(player);
			} else if (choice == DEBUG) {
				debug(player,e,p,index,i);
			} else if (choice == ARTIFACTS) {
				handleArtifacts(player);
			} else if (choice == QUIT) {
				quit(player);
			}	
			
			//Apply status effects to enemy and player
			//By putting it here, we hope to NOT have effect applied when enemy is killed in the turn
			applyStatus(player);
			applyEStatus(e);

			//Update all the player counters
			updateCounters((Turncounter **)player->counters, 0, player, e);
			//Update all the enemy counters
			updateCounters((Turncounter **)e->counters, 1, player, e);

			printBattleStats(player,e);
				
			if (e->beast) {
				white();
			}
		} //End while current enemy
	} //End for all enemies
	return fightStatus;
}

/**
 * Takes an integer seed and returns a Path pointer.
 * The seed provided is used to set the random seed and initialise the path values.
 * @see Path
 * @see MAXLENGTH
 * @see MAXLUCK
 * @param seed An integer seed.
 * @return A Path pointer with stats.
 */
Path* randomise_path(int seed){
	Path* p = (Path*) malloc(sizeof(Path));
	srand(seed);

	p->length = (rand() % MAXLENGTH ) + 1;
	p->luck = (rand() % MAXLUCK ) + 1;
	p->prize = 15 / p->luck * (rand() % 150) + 500;

	return p;
}

/**
 * Takes a integer and a string array (possibly from main).
 * Initialises a Path pointer and a Fighter pointer, before looping for each oom in path length by calling room().
 * If all the rooms are cleared, prints a victory message and exits the program.
 * Then getParams() is called to initialise a Fighter pointer's name and class fields.
 * Takes an integer seed and frees path pointer.
 * Notably, player pointer is not freed here.
 * @see Path
 * @see Fighter
 * @see getParams()
 * @see initPlayerStats()
 * @see room()
 * @see handleStats()
 * @see printStats()
 * @param argc The number of argv values + 1 (0 is program name?).
 * @param argv Array of strings with argc - 1 values.
 * @return No return.
 */
void gameloop(int argc, char** argv){

	do {
		Path* path = randomise_path(rand());
		
		Fighter* player = (Fighter *) malloc(sizeof(Fighter));
		
		getParams(argc, argv, player);
		initPlayerStats(player,path);
		
		purple();
		printStats(player);
		white();

		int roomsDone = 0;
	
		int res = NO_DMG;
		while (roomsDone < path->length) {
			res = room(roomsDone+1,path,player);
			if (res == DEATH) {
				break;
			}
			else {
				roomsDone++;
			
				//Update stats
				player->stats->roomscompleted++;
			}

			//Flush the terminal
			system("clear");
	
			lightYellow();
			printStats(player);
			lightGreen();
			printf("\n\tYou completed room %i.\n",roomsDone);
			white();
		}
		//Got out of the loop with res not being DEATH; so i won
		if (res != DEATH) {
		system("clear");
		handleStats(player);
		printf("\n\n\tYOU WON!\n\n");
		}

		free(path);
	} while (retry());
	
	purple();
	printf("\n\n\t\tTHANKS 4 PLAYING!\n\n");
	white();	
	exit(0);
}
