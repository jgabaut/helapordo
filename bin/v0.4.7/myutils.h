static callback_void_t callback_func_ptrs[SPECIALSMAX];
static callback_void_t callback_counter_ptrs[COUNTERSMAX];

void register_callback(specialMove move, callback_void_t ptr) {
	callback_func_ptrs[move] = ptr;
}

void register_counter_callback(int index, callback_void_t ptr) {
	callback_counter_ptrs[index] = ptr;
}

void red() {
	printf("\033[1;31m");
}
void lightRed() {
	printf("\033[0;31m");
}
void white() {
	printf("\033[0;37m");
}
void green() {
	printf("\033[1;32m");
}
void lightGreen() {
	printf("\033[0;32m");
}
void yellow() {
	printf("\033[1;33m");
}
void lightYellow() {
	printf("\033[0;33m");
}
void blue() {
	printf("\033[1;34m");
}
void lightBlue() {
	printf("\033[0;34m");
}
void purple() {
	printf("\033[1;35m");
}
void lightPurple() {
	printf("\033[0;35m");
}
void cyan() {
	printf("\033[1;36m");
}
void lightCyan() {
	printf("\033[0;36m");
}

void screenTime(float t) {
	sleep(t);

	//Avoid leftovers from stdin be picked up while sleeping
	tcflush(0,TCIFLUSH);
}

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

void printVersion(){

	printf("\t\t\t\t\t\t\tVersion %s\n",VERSION);
}

void usage() {
	perror("\nUsage:\t./battlesim [name] [class]");
	exit(1);
}

static inline char* stringFromStatus(fighterStatus s) {
	static const char* strings[] = {
		"Normal",
		"Poison",
		"Frozen",
		"Burn",
		"Weak",
		"Strong"
	};

	return strings[s];
}

static inline char* stringFromClass(fighterClass f) {

	return classesstrings[f];
}


static inline char* stringFromEClass(enemyClass e) {

	return classenemystrings[e];
};

static inline char* stringFromItems(int i) {

	return itemstrings[i];
}

static inline char* stringFromArtifacts(int i) {

	return artifactstrings[i];
}

static inline char* stringFromSpecial(fighterClass class, specialMove move) {
	return specialsstrings[class][(move % (SPECIALSMAX + 1))];
}

static inline int costFromSpecial(fighterClass class, specialMove move) {
	return specialscosts[class][(move % (SPECIALSMAX + 1)) ];
}

void scanName(Fighter* player) {
	char name[25];
	int f;
	do {
		printf("\n\n\tWhat's your name?\n\n");
		f = scanf("%s",name);
	} while (f >= 24 || f <= 0 );
	strcpy(player->name,name);
}


void printClasses() {
	printf("[");
	for (int i=0; i<=CLASSESMAX; i++) {
		printf("\t%d)\t%s",i,stringFromClass(i));
		if (i!=CLASSESMAX) { printf("\n"); }
	};
	printf("\t]\n");
}

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

void pickName(Fighter* player) {
	scanName(player);
	red();
	printf("\n\n\tName: %s\n\n",player->name);
	white();
}

void knightSpecial_Slash(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	
	printf("\t\t%s strikes %s.\n\n",f->name,stringFromEClass(e->class));
	
	e->hp -= f->atk * 0.5; //Dmg the enemy
	f->energy -= costFromSpecial(f->class,KSlash);        //Reduce fighter energy
	lightGreen();
	printf("\t\t%s was deeply hurt.\n\n",stringFromEClass(e->class));
	white();
	screenTime(2);
}

void knightSpecial_Cover(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	printf("\t\t%s uses his shield to cover.\n\n",f->name);

	f->def += 10;	//Raise fighter def
	f->energy -= costFromSpecial(f->class,KCover);	//Reduce fighter energy
	
	lightGreen();
	printf("\t\t+10 DEF\n\n");
	white();
	screenTime(1);
}

void knightSpecial_Armordrop(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if ( f->def >= 11 ) {
		printf("\t\t%s drops his armor to strike faster.\n\n",f->name);
		f->def -= 10;
		f->vel += 10; 
		
		f->energy -= costFromSpecial(f->class,KArmordrop);        //Reduce fighter energy
		
		lightGreen();
		printf("\t\t+10VEL\t");
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

void archerSpecial_Poisonshot(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if (e->hp > 1) { //Check if enemy has some hp
		if (e->hp >= 4) {
			e->hp -= 3;
		}
		
		printf("\t\t%s aims a poisoned arrow at %s's!\n",f->name,stringFromEClass(e->class));
		
		e->status = Poison; //Set status to Poison. May need change to manage multiple statuses active at once
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

void mageSpecial_Powerup(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	printf("\t\t%s channels its energy.\n",f->name);
	f->atk += 5;

	f->energy -= costFromSpecial(f->class,MPowerup);        //Reduce fighter energy

	lightGreen();
	printf("\t\t+5 ATK\t\n\n");
	white();
	screenTime(1);
}

void mageSpecial_Spellstrike(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	if (e->hp > 1 ) { //Check if enemy has some hp
		if (e->hp >= 6 ) {
			e->hp -= 5; //Deal damage	
		} else if (e->hp > 1) {
			e->hp = 1; //Set enemy hp to 1
		}
		
		printf("\t\t%s casts a spell on %s!\n",f->name,stringFromEClass(e->class));

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
			}
		break;
		case Archer:
			{
				REGISTER_CALLBACK(AHeadshot, archerSpecial_Headshot);
				REGISTER_CALLBACK(AQuivercheck, archerSpecial_Quivercheck);
				REGISTER_CALLBACK(APoisonshot, archerSpecial_Poisonshot);
			}
		break;
		case Mage: 
			{
				REGISTER_CALLBACK(MFatewarp, mageSpecial_Fatewarp);
				REGISTER_CALLBACK(MPowerup, mageSpecial_Powerup);
				REGISTER_CALLBACK(MSpellstrike, mageSpecial_Spellstrike);
			}
		break;
		case Assassin:
			{
				REGISTER_CALLBACK(XGrimdagger, assassinSpecial_Grimdagger);
				REGISTER_CALLBACK(XLeechknife, assassinSpecial_Leechknife);
				REGISTER_CALLBACK(XDisguise, assassinSpecial_Disguise);
			}
		break;
	}

}

//Shoulde return the address of callback function for a given fighterStatus
/*(callback_void_t*) getCounterStatusCallback(fighterStatus status) {
	switch () {
		case Normal:
			{
				return &normalStatusCallback;
			}
		break;
		case Poison:
			{
				return &poisonStatusCallback;
			}
		break;
		case Burned:
			{
				return &burnedStatusCallback;
			}
		break;
		case Frozen:
			{
				return &frozenStatusCallback;
			}
		break;
		case Weak:
			{
				return &weakStatusCallback;
			}
		break;
		case Strong:
			{
				return &normalStatusCallback;
			}
		break;
	}
}*/


//Status effect functions
void resetFighterStatus(Fighter* f) {
	f->status = Normal;
}


effect_fun getStatusCounterFun(fighterStatus status) {
	switch (status) {
		//case Normal: {
		default: {
			return &resetFighterStatus;
		}
		break;
	};
};

//Stat boost functions
void set_turnboost_atk(Fighter* f,int boost) {
	//Straight addition of the boost
	f->turnboost_atk = boost;
};
void set_turnboost_def(Fighter* f,int boost) {
	//Straight addition of the boost
	f->turnboost_def = boost;
};
void set_turnboost_vel(Fighter* f,int boost) {
	//Straight addition of the boost
	f->turnboost_vel = boost;
};
void set_turnboost_enr(Fighter* f,int boost) {
	//Straight addition of the boost
	f->turnboost_enr = boost;
};

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

void initCounters(Fighter* f){
		
	int j = 0 ; //Counter for statuses
		    //
	int statsboostsLoaded = 0; //Step through all stat boosts counter in one if

	for (int i = 0; i < COUNTERSMAX; i++) {
		Turncounter* c = (Turncounter*)malloc(sizeof(Turncounter));
		
		//First, prepare counters for statuses
		if (j < STATUSMAX) {
			c->desc = (char*)malloc(sizeof(stringFromStatus(j)));
			strcpy(c->desc,stringFromStatus(j));

			c->effect_fun = getStatusCounterFun(f->status);
			c->innerValue = 0;
			c->type = CNT_STATUS;
			//Register function for the specific status counter
			//
			//REGISTER_COUNTER_CALLBACK(i,resetFighterStatus);
			//
			//Why does uncommenting this cause problems to special moves callback?
			//More specific, ONLY MAGE MOVES
			
			j++;
			//debug
			//printf("%s",stringFromStatus(j));
		} else if (!statsboostsLoaded) { //Then, counters for boosts to (all?) stats
			
			//We prepare all of them in one go, before going to the next section
			c->desc = (char*)malloc(sizeof("ATK boost"));
			strcpy(c->desc,"ATK boost");
			c->innerValue = 0;
			c->boost_fun = getStatBoostCounterFun(ATK);
			c->type = CNT_ATKBOOST;

			c->count = 0;
			f->counters[i] = c;
			i++;	

			c->desc = (char*)malloc(sizeof("DEF boost"));
			strcpy(c->desc,"DEF boost");
			c->innerValue = 0;
			c->boost_fun = getStatBoostCounterFun(DEF);
			c->type = CNT_DEFBOOST;

			c->count = 0;
			f->counters[i] = c;
			i++;

			c->desc = (char*)malloc(sizeof("VEL boost"));
			strcpy(c->desc,"VEL boost");
			c->innerValue = 0;
			c->boost_fun = getStatBoostCounterFun(VEL);
			c->type = CNT_VELBOOST;

			//In reality, the last counter is added to the f->counters array a bit later
			statsboostsLoaded = 1;
		}
	
		c->count = 0;

		f->counters[i] = c;
	};
};	

void updateCounters(Fighter* f){
	for (int i=0; i < COUNTERSMAX; i++) {

		Turncounter* c; //= (Turncounter*)malloc(sizeof(Turncounter));
		c = f->counters[i]; 
		if (c->count == 1) { //Counter is about to expire so we call the correct function:
			switch (c->type) {
				case CNT_STATUS: { //Callback for status counters
					(c->effect_fun)(f);
				}
				break;
				case CNT_ATKBOOST: case CNT_DEFBOOST: case CNT_VELBOOST: case CNT_ENRBOOST: { //Callback for stat boosts
					(c->boost_fun)(f,0);
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

void artifact_thinkingskull(Fighter* f,Enemy* e) {
	if (!(artifactsBag[THKSKULL].active)) { //Gives permanent +4 DEF to f
		f->permboost_def += 4;
		artifactsBag[THKSKULL].active = 1;
	}
};

void artifact_twinkie(Fighter* f,Enemy* e) {
	if (!(artifactsBag[TWINKIE].active)) { //Zombies give double exp
		if (e->class == Zombie ) {
			e->xp *= 2;
		}
		artifactsBag[TWINKIE].active = 1;
	}
};

void artifact_wristband(Fighter* f,Enemy* e) {
	if (!(artifactsBag[WRISTBAND].active)) { //Gives permanent +2 ENR to f
		f->permboost_enr += 2;
		artifactsBag[WRISTBAND].active = 1;
	}
};

void artifact_boartail(Fighter* f,Enemy* e) {
	if (!(artifactsBag[BOARTAIL].active)) { //Boars get slowed down
		if (e->class == Boar ) {
			e->vel /= 1.5;
		}
		artifactsBag[BOARTAIL].active = 1;
	}
};
	
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
		}
	};
};

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

void resetArtifactsState() {
	for (int i=0; i < ARTIFACTSMAX + 1; i++) {
		if (artifactsBag[i].qty != 0 ) { //We only reset the ones we have
			artifactsBag[i].active = 0;
		};
	};
};	

void applyArtifacts(Fighter* f, Enemy* e){
	for (int i=0; i < ARTIFACTSMAX + 1; i++) {
		if (artifactsBag[i].qty != 0 && !(artifactsBag[i].active)) { //We only apply the ones we have unlocked && aren't active already
			artifact_fun fun;
			fun = artifactsBag[i].artifact_fun;
			(fun)(f,e);
		};
	};
}

void initPlayerStats(Fighter* player, Path* path) {
	
	player->luck = path->luck % MAXPLAYERLUCK;
	
	BaseStats* base = &basestats[player->class];

	countStats* s = (countStats*)malloc(sizeof(countStats));
	
	s->enemieskilled=0;
	s->criticalhits=0;
	s->itemsfound=0;
	s->artifactsfound=0;
	s->roomscompleted=0;
	s->specialsunlocked=0;

	setSpecials(player);	
	initCounters(player);	
	
	initArtifactsFun();
	
	player->permboost_atk = 0;
	player->permboost_def = 0;
	player->permboost_vel = 0;
	player->permboost_enr = 0;

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
	e->level = base->level;
	
	e->totalenergy = base->totalenergy;
	e->energy = e->totalenergy;

	//Triple xp for beasts
	e->xp = (e->beast) ? 3 * base->xp : base->xp;
	e->status = Normal;
}

int getEnemyBoost(int lvl) {

	float boost = (lvl * 1.25F) ;
	
	if (lvl < 2 ) {
		boost = 0; // Evitare conflitti
	}

	return (int) boost;
}
		
void statResetEnemy(Enemy* e, int force) {
	if ( !force && (e->hp >= 0.4F * e->totalhp) && !( e->atk <=0 || e->def <= 0 || e->vel <=0) ) {
		return;
	}	
	int boost = getEnemyBoost(e->level); 	
	
	float beastf = 1 ;

	if (e->beast) {
		beastf = BSTFACTOR;
	}
	EnemyBaseStats* base = &baseenemystats[e->class];
	if (force || e->vel <= (0.3F * (base->vel + boost)) || (e->atk <= (0.3F * (base->atk + boost))) || e->def <= (0.2F * (base->def + boost))) {
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

void prepareRoomEnemy(Enemy* e, Fighter* player, int roomindex, int enemiesInRoom, int enemyindex) {

		//Randomise enemy class
		e->class = rand() % (ENEMYCLASSESMAX + 1);

		
		//Check for spawning beast enemies
		if ((roomindex % BEASTROOM == 0) && (enemyindex == (enemiesInRoom - 1))) {
			e->beast = 1;	
		} else {
			e->beast = 0;	
		};

		//Load enemy stats
		initEnemyStats(e);
		
		if (!e->beast) {
			printf("\nRoom %i)\t\tEnemy #%i:\t%s\n",roomindex,enemyindex+1,stringFromEClass(e->class));
		} else {
			lightCyan();
			printf("\nYou feel at discomfort.\nRoom %i)\t\t",roomindex);
			lightRed();
			printf("Enemy #%i:\t%s\n",enemyindex+1,stringFromEClass(e->class));
			white();
		}
	
		//Enemies get 1 level for each X player levels
		if (player->level > 1) {
			e->level = round(player->level / ENEMYLVLRATIO) + 1;
			statResetEnemy(e,1);
		}
}

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

int getEnemyXpGain(Enemy* e) {
	
	int xp = e->xp + e->level;
	return xp;
}

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
	printf("\t\t\t%i/%i\n",f->energy,f->totalenergy);
	white();
	printf("Atk:\t\t%i\t\t\t%i\n",e->atk, f->atk);
	printf("Def:\t\t%i\t\t\t%i\n",e->def, f->def);
	printf("Vel:\t\t%i\t\t\t%i\n",e->vel,f->vel);
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

void printItemStats(Item* it) {
	printf("  (%i)\t%i\t%s\t\"%s\"\n",it->class,it->qty,it->name,it->desc);	
};

void printArtifactStats(Artifact* a) {
	printf("  (%i)\t\t",a->class);
	purple();
	printf("%s\t",a->name);
	yellow();
	printf("\"%s\"\n",a->desc);	
	white();
};

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

int getTurnChoice() {
	char choice;
	printf("\tWhat do you want to do?\n\n\t[\n\t\t0-1{Return}\tFight\t\t2\tSpecial Attack\n");
	printf("\t\t3-4\t\tItem\t\t5\tArtifacts\n");
	printf("\t\t6-8\tStats\t\t9-Others\tQuit\t\t\t\t]\n");
	char c[25];
	fgets(c, sizeof c, stdin);

	switch(c[0]) {
		case '0' ... '1':
			return FIGHT;
		break;
		case '2':
			return SPECIAL;
		case '3' ... '4':
			return ITEM;
		break;
		case '5':
			return ARTIFACTS;
		break;
		case '6' ... '8':
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

int getBoost(int lvl, int luck) {

	float boost = (lvl * 1.25F) + ((luck % 2 ) * 2);
	
	if (lvl < 2 ) {
		boost = 1.0; // Evitare conflitti
	}

	return (int) boost;
}

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

void onLevelUp(Fighter* player) {
	int boost = getBoost(player->level,player->luck);

	BaseStats* base = &basestats[player->class];

	player->atk = ( base->atk + boost );
	player->def = ( base->def + boost );
	player->vel = ( base->vel + boost );
	player->totalhp = ( base->hp * 1.12F ) + boost;

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

void giveXp(Fighter* player, Enemy* e){
	
	
	int xp = getEnemyXpGain(e);

	checkremainder(player, xp);
}

void statReset(Fighter* player, int force) {
	if ( !force && (player->hp >= 0.5F * player->totalhp) && !(player->atk <=0 || player->def <=0 || player->vel <= 0) ) {
		return;
	}
	
	int boost = getBoost(player->level,player->luck);

	BaseStats* base = &basestats[player->class];
	if (force || player->vel <= 0.3F * (base->vel + boost) || player->atk <= 0.35F * (base->atk + boost) || player->def <= 0.18F * (base->def + boost )) {
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

void printStatusText(fighterStatus status, char* subject) {
	switch (status) {
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

void applyStatus(Fighter* f){
	
	switch(f->status) {
		case Normal:
			{
				break;
			}
		break;
		case Poison: 
			{
				lightRed();
				printStatusText(Poison,f->name);
				white();
			}
		break;
		case Burned:

		break;
		case Frozen:

		break;
		case Weak:

		break;
		case Strong:

		break;
	}	
};

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
		case Burned: { //ATM it's just the same as Poison.
			if (e->xp >= 4) {
				e->hp -= 3;
			} else {
				e->hp = 1; //Will this be a problem for kills in the enemy loop?
			}
			lightGreen();
			printStatusText(Burned,stringFromEClass(e->class));
			white();
		}

		break;
		case Frozen:

		break;
		case Weak:

		break;
		case Strong:

		break;
	}	
};


int fight(Fighter* player, Enemy* e){

	system("clear");
	
	int res = 0;
	//Stat comparisons
	//
	
	int atkdelta = player->atk - e->atk - (rand() % 3 ) - 1 ; //Skewed with defender
	int defdelta = player->def - e->def + (rand() % 2 ) + 1  ; //Skewed with attacker
	int veldelta = player->vel - e->vel + (rand() % 3 ) + 1 ;

	int atkOnPlayer = e->atk - (player->def + (player->vel / 6)) ;
	int atkOnEnemy = player->atk - (e->def + (e->vel / 6)) ;

	if (veldelta >= 0) {
		
		if ( atkOnEnemy > 3 ) {
			e->hp -= atkOnEnemy;
			res=DMG_DEALT;
		} else if ( atkOnEnemy >= 0) {
			e->hp -= abs(atkOnEnemy - atkdelta);
			res = DMG_DEALT;	
		} else {
			if ( atkOnEnemy < -3 ) {
				player->hp -= abs(atkOnPlayer - 0.75F * player->vel );
			} else {
			       	player->hp -= abs (atkOnPlayer - 1 );
			}
			res = DMG_TAKEN;
		}
	} else { 
		atkdelta = -atkdelta;
		if (atkOnPlayer > 3) {
			player->hp -= atkOnPlayer;
			res = DMG_TAKEN;
		} else if ( atkOnPlayer >= 0 ) {
			player->hp -= abs( atkOnPlayer - atkdelta);
			res = DMG_TAKEN;
		} else {
			if ( atkOnPlayer < -3 ) {
				e->hp -= abs(atkOnEnemy - 0.75F * e->vel );
			} else {
				e->hp -= abs (atkOnEnemy - 1 );
			}
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
	
		statReset(player,0);
		statResetEnemy(e,0);	
	
		strcpy(victim,player->name);
	} else {
		player->vel--;
		player->atk--;
		player->def -= 2;
		
		statResetEnemy(e,0);	
		statReset(player,0);
		
		strcpy(victim,stringFromEClass(e->class));
	}
	
	if (yourhit) {
		yellow();
	} else {
		red();
	}

	printf("\n\n\t%s was hit.\n",victim);
	white();

	//Rolls
	//
	//Critical hit
	if (res == DMG_DEALT && ((rand() % (15 - (player->luck/5) ) ) == 0) ) {
		lightPurple();
		printf("\n\n\tA critical hit!\n");
		white();
		e->hp -= player->atk * 0.35F;

		//Update stats
		player->stats->criticalhits++;
	}
	//Check for deaths -> exit condition from loop
	//
	//
	//
	if (e->hp <= 0 ) {
		res = KILL_DONE;
		
		if (e->beast) {
			purple();
		} else {
			green();
		}
			printf("\n\n\t%s fainted.\n",stringFromEClass(e->class));
		white();
		screenTime(2);
		
		//Update stats
		player->stats->enemieskilled++;
	}
	if (player->hp <= 0) {
		res = DEATH;
	}

	//Item drop, guaranteed on killing a beast
	if (res == KILL_DONE && (e->beast || ( (rand() % 9)  - (player->luck/10)  <= 0 ))) {
		int drop = rand() % (ITEMSMAX + 1);
	
		//Special drop chances. Maybe a function for this?
		while (drop == Powergem) {
			if ( !(rand() % 2 == 0)) {
				drop = rand() % (ITEMSMAX + 1);
			}
		}
			
		cyan();
		printf("\n\n\tYou found a %s!\n",stringFromItems(drop));
		white();
		bag[drop].qty++;
		
		//Update stats
		player->stats->itemsfound++;
		
		screenTime(1);
	}	
	
	//Artifact drop (if we don't have all of them), guaranteed on killing a beast
	if ( (player->stats->artifactsfound != ARTIFACTSMAX + 1)  && res == KILL_DONE && (e->beast || ( (rand() % 1001)  - (player->luck/10)  <= 0 ))) {
		int drop = 0;
		do {
			drop = rand() % (ARTIFACTSMAX + 1);
		} while (artifactsBag[drop].qty != 0 ); //If we happen to drop one we already have, we reroll up to 10 times
	
		purple();
		printf("\n\n\tYou found a %s!\n",stringFromArtifacts(drop));
		white();
		artifactsBag[drop].qty++;
		
		//Update stats
		player->stats->artifactsfound++;
		
		screenTime(2);
	}	

	return res;
}

void useItem(Fighter* f, Enemy* e, int i){
	//Mountain of ifs? Where are my damn maps
	
	if (bag[i].qty == 0) {
		return;
	}	
	if (bag[i].class == Potion) {
		f->hp=f->totalhp;
		green();
		printf("\n\n\n\tYour health was restored.\n\n");
		white();
	} else if (bag[i].class == Rock) {
		e->def -= 8;
		e->hp -= 2;
		printf("\n\n\n\t%s got smacked.",stringFromEClass(e->class));
		red();
		printf("\t-8 DEF and -2 HP.\n\n");
		white();
	} else if (bag[i].class == Bread) {
		f->vel -= 1 ;
		f->def += 6 ;
		f->atk += 4;
		printf("\n\n\n\tYou ate elvish bread.");
		green();
		printf("\t+6 DEF , +4 ATK, ");
		yellow();
		printf("-1 VEL\n\n");
		white();
	} else if (bag[i].class == Coffee) {
		f->vel += 6 ;
		f->atk += 2 ;
		f->def -= 2 ;
		printf("\n\n\n\tYou drank hot coffee.");
		green();
		printf("\t+6 SPE , +2 ATK, ");
		yellow();
		printf("-2 DEF\n\n");
		white();
	} else if (bag[i].class == Powergem) {
		printf("\n\n\n\tYou became wiser.");
		green();
		printf("\t+25 XP\n\n");
		white();
		checkremainder(f,25);	
	} else {
		return;
	}

	bag[i].qty--;
};

int getItemQty(int n) {
	return bag[n].qty;	
}

void emptyItems() {
	for (int i=0; i < ITEMSMAX + 1; i++) {
		if (getItemQty(i) > 0 ) {
			bag[i].qty = 0;
		}
	};
}

void handleItems(Fighter* f, Enemy* e) {
	system("clear");
	
	printf("\tItems:\tQuantity:\n\n");
	int empty = 1; //To print something when you have no items

	for (int i = 0; i < ITEMSMAX +1; i++) {
		Item* it = &bag[i];
		if (it->qty > 0) {
			printItemStats(it);
			if (empty) { empty = 0; }; //Bag is not empty
		}
	}

	if (empty) {
		lightYellow();
		printf("\t\t\tYou have no items now.\n\n");
		white();
		screenTime(1);
		system("clear");
	} else {
		int num = -1;
		printf("\n\n\tInsert item number or {Return} to exit:\n\n\t");
		//scanf("%i",&num);
	
		char c[25];
		fgets(c, sizeof c, stdin);
		if (c[0]=='\n') {
			return;
		}

		//Converting the first char into an integer...
		num = c[0] - '0';

		useItem(f,e,num);
	}
}

void emptyArtifacts() {
	for (int i=0; i < ARTIFACTSMAX + 1; i++) {
		if (artifactsBag[i].qty > 0 ) {
			artifactsBag[i].qty = 0;
			artifactsBag[i].active = 0;
		}
	};
}

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
		
		//Debug
		//printf("Selected:\t%s\t%i\n",stringFromSpecial(f->class,selected->move),selected->move);
		//screenTime(2);	
		
		//Check if the selected move is enabled AND if user has enough energy
		if (selected->enabled && selected->cost <= f->energy) {
			
			//callback to the move
			GET_CALLBACK(selected->move,callback_special_t)(f,e,p,roomIndex,enemyIndex); 
		}
	}
}

void handleStats(Fighter* player){

	system("clear");
	printStats(player);

	printf("\n\t\tEnemies killed:\t\t%i\n\t\tCritical hits done:\t%i\n",player->stats->enemieskilled,player->stats->criticalhits);
	printf("\t\tItems found:\t\t%i\n\t\tRooms completed:\t%i\n",player->stats->itemsfound,player->stats->roomscompleted);
	printf("\t\tArtifacts found:\t\t%i\n",player->stats->artifactsfound);

}
void death(Fighter* player) {
	
	lightRed();
	handleStats(player);
	red();
	printf("\n\n\tYOU DIED.\n");
	white();
	

	emptyItems();
	emptyArtifacts();
	//Free player special slots
	for (int i=0; i < SPECIALSMAX + 1 ; i++) {
		free(player->specials[i]);
	}
	//Free player turnCounters
	//for (int i=0; i < COUNTERSMAX ; i++) {
	//	free(player->counters[i]);
	//}
	free(player->stats);
	free(player);
}

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

void debug(Fighter* player, Enemy* e, Path* p, int roomIndex,int currentEnemyNum) {
	
	system("clear");

	char choice;
	printf("\tWhat do you want to do?\n\n\t[\n\t\t0\tAdd xp\t\t\t1\tGive item\n");
	printf("\t\t2\tReset stats\t\t3\tReset Enemy\n");
	printf("\t\t4\tAlter luck\t\t5\tGET_CALLBACK\n");
	printf("\t\t6\tUnlock special\t\t7\tUnlock Artifact\n");
	printf("\t\t8-{Return}\tBack to game\t\t\t]\n");
	char ch[25];
	fgets(ch, sizeof ch, stdin);

	int c=0,n=-1;
	switch(ch[0]) {
		case '0':
			{ int xp;
			do {
				printf("\nHow much?\n");
			} while ((c = scanf("%i",&xp)) != 1 || xp < 0);
			checkremainder(player,xp);
			}
		break;
		case '1':
			{ int q=-1;
			do {
				printf("\nInsert item number:\n");
			} while ((c = scanf("%i",&n)) != 1 || n > ITEMSMAX || n < 0 );
			do {
				printf("\nInsert quantity:\n");
			} while ((c = scanf("%i",&q)) != 1 && q <= 0 );

			bag[n].qty += q;
			}
		break;
		case '2':
			statReset(player,1);
		break;
		case '3':
			statResetEnemy(e,1);
		break;
		case '4':
			{ 
				printf("\nCurrent luck: %i\tRL: %i\n",player->luck,p->luck);
				
				do {
					printf("\nInsert new luck:\n");
				} while ((c = scanf("%i",&n)) != 1 && n > 0 );
			
				//Copy-pasted the calc for player luck... might need a function
				p->luck = n;

				player->luck = p->luck % MAXPLAYERLUCK;
			}
		break;
		case '5':
			{ 
				int s = -1;
				printf("Insert special num:\n");
				scanf("%i",&s);
				printf("Read: %i\n",s);
				GET_CALLBACK(s,callback_special_t)(player,e,p,roomIndex,currentEnemyNum); 
			
			}
		break;	
		case '6':
			{
				unlockSpecial(player);
			}
		break;
		case '7':
			{ int n=-1;
			do {
				printf("\nInsert artifact number (0<n<%i):\n",ARTIFACTSMAX + 1);
			} while ((c = scanf("%i",&n)) != 1 || n > ARTIFACTSMAX || n < 0 );

			artifactsBag[n].qty += 1;
			artifactsBag[n].active = 0;
			}
		break;
		default : 
			return;
		break;
	}	

}

void quit(Fighter* p) {
	system("clear");
	printf("\n\n\tTHANKS 4 PLAYING\n");
	printStats(p);
	printf("\n");
	exit(1);
}

int room(int index, Path* p, Fighter* player) {
	
	screenTime(2);
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
	
		Enemy* e = (Enemy *) malloc (sizeof(Enemy));
		//Load up an enemy
		prepareRoomEnemy(e, player, index, enemies, i);
		
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

			choice = getTurnChoice();
			if (choice == FIGHT) {
				fightStatus = fight(player,e);
				//Lost battle
				if (fightStatus == DEATH) {
					
					free(e);
					
					death(player);
					return DEATH;
				} else if (fightStatus == KILL_DONE) {
				//Win, get xp and free memory from enemy
					giveXp(player,e);

					free(e);
				
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
			} else if (choice == ITEM) {
				handleItems(player,e);
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
			updateCounters(player);

			printBattleStats(player,e);
				
			if (e->beast) {
				white();
			}
		}
	}
	return fightStatus;
}

Path* randomise_path(int seed){
	Path* p = (Path*) malloc(sizeof(Path));
	srand(seed);

	p->length = (rand() % MAXLENGTH ) + 1;
	p->luck = (rand() % MAXLUCK ) + 1;
	p->prize = 15 / p->luck * (rand() % 150) + 500;

	return p;
}

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
