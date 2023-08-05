void red() {
	printf("\033[1;31m");
}
void white() {
	printf("\033[0;37m");
}
void green() {
	printf("\033[1;32m");
}
void yellow() {
	printf("\033[1;33m");
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

void scanName(Fighter* player) {
	char name[25];
	int f;
	do {
		printf("\nWhat's your name?\n");
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
		printf("\nPick a class.\n");
		printClasses();
		pick = scanClass();
	} while (pick<0);

	player->class = pick;
	printf("\nPicked: %s\n",stringFromClass(player->class));
}

void pickName(Fighter* player) {
	scanName(player);
	red();
	printf("\nName: %s\n",player->name);
	white();
}


void initPlayerStats(Fighter* player, Path* path) {
	player->luck = path->luck % 10;
	
	BaseStats* base = &basestats[player->class];

	player->hp = base->hp;
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

	e->hp = base->hp;
	e->atk = base->atk;
	e->def = base->def;
	e->vel = base->vel;
	e->level = base->level;
	e->xp = base->xp;
	e->status = Normal;
}

void printStats(Fighter* f) {
	
	printf("\n\t%s's stats:\n",f->name);
	printf("\tClass: %s\n",stringFromClass(f->class));
	printf("\tHp: %i\n",f->hp);
	printf("\tAtk: %i\n",f->atk);
	printf("\tDef: %i\n",f->def);
	printf("\tVel: %i\n",f->vel);
	printf("\tLvl: %i\n",f->level);
	printf("\tCurrent exp: %i\n",f->currentlevelxp);
	printf("\tStatus: %s\n\n",stringFromStatus(f->status));
}

void printEStats(Enemy* e) {
	
	printf("\n\t%s's stats:\n",stringFromEClass(e->class));
	printf("\tHp: %i\n",e->hp);
	printf("\tAtk: %i\n",e->atk);
	printf("\tDef: %i\n",e->def);
	printf("\tVel: %i\n",e->vel);
	printf("\tLvl: %i\n",e->level);
	printf("\tXp: %i\n",e->xp);
	printf("\tStatus: %s\n",stringFromStatus(e->status));
}

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
	printf("\n\n\tWhat do you want to do?\n\n\t[\n\t\t0-Return\tFight\n\t\t1-Others\tQuit\n\t]\n");
	char c[2];
	fgets(c, sizeof c, stdin);
	if ( c[0] == '0' || c[0] == '\n' ) { 
		return FIGHT;
	} else { 
		return QUIT;
	};
	
}

void onLevelUp(Fighter* player) {
	float boost = player->level * 1.05F + ((rand() % 3 ) * 2);
	player->atk += boost;
	player->def += boost;
	player->vel += boost;
}

void giveXp(Fighter* player, Enemy* e){
	int xp = e->xp;
	void checkremainder(Fighter* player, int xp) {
		int curr = player->currentlevelxp;
		int tot = player->totallevelxp;
		
		if (curr + xp >= tot) {
			player->totalxp += xp;
			if (xp !=0) printf("\n\n\t%s obtained %i xp.\n", player->name, xp);
			player->level++;
			
			yellow();
			printf("\n\n\t%s reached Lvl. %i !\n", player->name, player->level);
			white();

			//Stats gains on level up
			onLevelUp(player);	
			//Cure the player on level up
			//
			BaseStats* base = &basestats[player->class];
			player->hp = base->hp;
			
			yellow();
			printf("\n\n\tYour wounds were healed.\n");
			white();

			player->currentlevelxp=abs((tot-curr-xp));
			player->totallevelxp = tot + 2 * player->level + 1;
			checkremainder(player,0);
		} else {
			player->currentlevelxp += xp;
			player->totalxp += xp;
			if (xp!=0) printf("\n\n\t%s obtained %i xp.\n", player->name, xp);
		}
	}

	checkremainder(player, xp);
}

int fight(Fighter* player, Enemy* e){
	
	int res = 0;
	//Stat comparisons
	//
	
	int atkdelta = player->atk - e->atk - (rand() % 3 ) - 1 ; //Skewed with defender
	int defdelta = player->def - e->def + (rand() % 2 ) + 1  ; //Skewed with attacker
	int veldelta = player->vel - e->vel + (rand() % 3 ) + 1 ;

	if (veldelta >= 0) {
		
		if ( atkdelta > 2 ) {
			e->hp -= player->atk;
			res=DMG_DEALT;
		} else if ( atkdelta >= 0) {
			e->hp -= atkdelta + 1;
			res = DMG_DEALT;	
		} else {
			if ( atkdelta < -3 ) {
				player->hp -= abs(e->atk - 3);
			} else {
			       	player->hp -= abs (atkdelta);
			}
			res = DMG_TAKEN;
		}
	} else { 
		atkdelta = -atkdelta;
		if (atkdelta > 2) {
			player->hp -= e->atk;
			res = DMG_TAKEN;
		} else if ( atkdelta >= 0 ) {
			player->hp -= atkdelta +1;
			res = DMG_TAKEN;
		} else {
			if ( atkdelta < -3 ) {
				e->hp -= abs(player->atk -3);
			} else {
				e->hp -= abs (atkdelta);
			}
			res = DMG_DEALT;
		}
	}

	int yourhit = (res == DMG_DEALT ) ? 1 : 0 ;
	system("clear");
	char victim[25];
	if (!yourhit) {
		e->vel--;
		if (e->vel <=0 ) {
			EnemyBaseStats* base = &baseenemystats[e->class];
			e->vel = base->vel;
			//Reset velocità
		}
		strcpy(victim,player->name);
		red();
	} else {
		player->vel--;
		if (player->vel <=0 ) {
			BaseStats* base = &basestats[player->class];
			player->vel = base->vel;
			//Reset velocità
		}

		strcpy(victim,stringFromEClass(e->class));
		yellow();
	}
	printf("\n\n\t%s was hit.\n",victim);
	white();

	//Rolls
	//
	//Hp subtraction, check for deaths -> exit condition from loop
	//
	//
	if (e->hp <= 0 ) {
		res = KILL_DONE;
		
		green();
		printf("\n\n\t%s fainted.\n",stringFromEClass(e->class));	
		white();
	}
	if (player->hp <= 0) {
		res = DEATH;
	}
	return res;
}

void death(Fighter* player) {
	red();
	printf("\n\n\tYOU DIED.\n");
	printf("\033[0;31m");
	printStats(player);
	white();
	exit(1);
}
