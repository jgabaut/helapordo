static callback_void_t callback_func_ptrs[CLASSESMAX];

void register_callback(fighterClass class, callback_void_t ptr) {
	callback_func_ptrs[class] = ptr;
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
		"Burned",
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

void knightSpecials(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	printf("\nDODGE\n");
	screenTime(2);
}

void archerSpecials(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	printf("\nLUNGE\n");
	screenTime(2);
}

void mageSpecials(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	printf("\nPUNGE\n");
	screenTime(2);
}

void assassinSpecials(Fighter* f, Enemy* e, Path* p, int roomIndex, int enemyIndex) {
	printf("\nDUNGE\n");
	screenTime(2);
}

void setSpecials(fighterClass class){

	switch (class) {
		
		case Knight:
			REGISTER_CALLBACK(Knight, knightSpecials);	
		break;
		case Archer:
			REGISTER_CALLBACK(Archer, archerSpecials);
		break;
		case Mage: 
			REGISTER_CALLBACK(Mage, mageSpecials);
		break;
		case Assassin:
			REGISTER_CALLBACK(Assassin, assassinSpecials);
		break;
	}

}

void initPlayerStats(Fighter* player, Path* path) {
	
	player->luck = path->luck % MAXPLAYERLUCK;
	
	BaseStats* base = &basestats[player->class];

	countStats* s = (countStats*)malloc(sizeof(countStats));
	
	s->enemieskilled=0;
	s->criticalhits=0;
	s->itemsfound=0;
	s->roomscompleted=0;

	setSpecials(player->class);	
	
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
	//Triple xp for beasts
	e->xp = (e->beast) ? 3 * base->xp : base->xp;
	e->status = Normal;
}

void printStats(Fighter* f) {
	
	printf("\t%s's stats:\n\n",f->name);
	printf("\t\tClass:\t%s\n",stringFromClass(f->class));
	printf("\t\tHp:\t%i/%i\n",f->hp,f->totalhp);
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
	printf("\tWhat do you want to do?\n\n\t[\n\t\t0-2{Return}\tFight\t\t3-5\tItem\n\t\t6-8\tStats\t\t9-Others\tQuit\t\t]\n");
	char c[25];
	fgets(c, sizeof c, stdin);

	switch(c[0]) {
		case '0' ... '2':
			return FIGHT;
		break;
		case '3' ... '5':
			return ITEM;
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

	/*
	if ( (c[0] >= '0' && c[0] <= '2') || c[0] == '\n' ) { 
		return FIGHT;
	} else if (c[0] >= '3' && c[0] <=5) { 
		return ITEM;
	} else if (c[0] >= '6' && c[0] <= '8'){
		return STATS;
	} else if (c[0] >= '9' && c[0] <= 'z') {
		return QUIT;
	};
	*/
	
}

int getBoost(int lvl, int luck) {

	float boost = (lvl * 1.25F) + ((luck % 2 ) * 2);
	
	if (lvl < 2 ) {
		boost = 1.0; // Evitare conflitti
	}

	return (int) boost;
}

int getEnemyBoost(int lvl) {

	float boost = (lvl * 1.25F) ;
	
	if (lvl < 2 ) {
		boost = 0; // Evitare conflitti
	}

	return (int) boost;
}


void onLevelUp(Fighter* player) {
	int boost = getBoost(player->level,player->luck);

	BaseStats* base = &basestats[player->class];

	player->atk = ( base->atk + boost );
	player->def = ( base->def + boost );
	player->vel = ( base->vel + boost );
	player->totalhp = ( base->hp * 1.12F ) + boost;
	player->hp = player->totalhp; //Cure on level up
	
	lightCyan();
	printf("\n\n\tYour wounds were healed.\n");
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

void statReset(Fighter* player, int force) {
	if ( !force && (player->hp >= 0.5F * player->totalhp) && !(player->atk <=0 || player->def <=0 || player->vel <= 0) ) {
		return;
	}
	
	int boost = getBoost(player->level,player->luck);

	BaseStats* base = &basestats[player->class];
	if (force || player->vel <= 0.3F * (base->vel + boost) || player->atk <= 0.35F * (base->atk + boost) || player->def <= 0.18F * (base->def + boost )) {
		player->vel = base->vel + boost;
		player->atk = base->atk + boost;
		player->def = base->def + boost;
		//Reset stats
		if (!force) {
			yellow();
			printf("\n\n\t%s's stats reset.\n",player->name);
			white();
		}
	}
}

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
			if ( !(rand() % 6 == 0)) {
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

void handleItems(Fighter* f, Enemy* e) {
	system("clear");
	printf("Items:\tQuantity:\n\n");
	for (int i = 0; i < ITEMSMAX +1; i++) {
		Item* it = &bag[i];
		if (it->qty > 0) {
			printItemStats(it);
		}
	}
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

void handleStats(Fighter* player){

	system("clear");
	printStats(player);

	printf("\n\t\tEnemies killed:\t\t%i\n\t\tCritical hits done:\t%i\n",player->stats->enemieskilled,player->stats->criticalhits);
	printf("\t\tItems found:\t\t%i\n\t\tRooms completed:\t%i\n",player->stats->itemsfound,player->stats->roomscompleted);

}
void death(Fighter* player) {
	
	lightRed();
	handleStats(player);
	red();
	printf("\n\n\tYOU DIED.\n");
	white();
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
	printf("\t\t4\tAlter luck\t\t5-{Return}\tBack to game\t\t\n\t]\n");
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
			{ GET_CALLBACK(player->class,callback_special_t)(player,e,p,roomIndex,currentEnemyNum); 
			return;
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
		//Randomise enemy class
		e->class = rand() % (ENEMYCLASSESMAX + 1);

		
		//Check for spawning beast enemies
		if ((index % BEASTROOM == 0) && (i == (enemies - 1))) {
			e->beast = 1;	
		} else {
			e->beast = 0;	
		};

		initEnemyStats(e);
		if (!e->beast) {
			printf("\nRoom %i)\t\tEnemy #%i:\t%s\n",index,i+1,stringFromEClass(e->class));
		} else {
			lightCyan();
			printf("\nYou feel at discomfort.\nRoom %i)\t\t",index);
			lightRed();
			printf("Enemy #%i:\t%s\n",i+1,stringFromEClass(e->class));
			white();
		}
	
		//Enemies get 1 level for each X player levels
		if (player->level > 1) {
			e->level = round(player->level / ENEMYLVLRATIO) + 1;
			statResetEnemy(e,1);
		}
		
		printBattleStats(player,e);
		
		//Fight
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
			} else if (choice == ITEM) {
				handleItems(player,e);
			} else if (choice == STATS) {
				handleStats(player);
			} else if (choice == DEBUG) {
				debug(player,e,p,index,i);
			} else if (choice == QUIT) {
				quit(player);
			}	
			
			printBattleStats(player,e);
			//printf("\n\t%s's hp:\t%i\tYour hp:\t%i\n",stringFromEClass(e->class),e->hp,player->hp);
		
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
