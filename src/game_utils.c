#include "game_utils.h"
//Functions useful in many areas

/**
 * Allocates and prepares a turnOP_args and returns a pointer to it.
 * @see turnOP_args
 * @see turnOption_OP
 * @see turnOption
 * @see turnOP()
 * @param actor The Fighter pointer to assign to turnOP_args->actor.
 * @param path The Path pointer to assign to turnOP_args->path.
 * @param room The Room pointer to assign to turnOP_args->room.
 * @param Enemy The Enemy pointer to assign to turnOP_args->enemy.
 * @param Boss The Boss pointer to assign to turnOP_args->boss.
 * @param save_file The FILE pointer to assign to turnOP_args->save_file.
 * @param notify_win The WINDOW pointer to assign to turnOP_args->notify_win.
 * @param t_kls The Koliseo_Temp pointer to assign to turnOP_args->t_kls.
 */
turnOP_args* init_turnOP_args(Fighter* actor, Path* path, Room* room, loadInfo* load_info, Enemy* enemy, Boss* boss, FILE* save_file, WINDOW* notify_win, Koliseo_Temp* t_kls) {
	Koliseo_Temp tkls = *t_kls;
	char msg[500];
	sprintf(msg,"Allocated size %lu for new turnOP_args", sizeof(turnOP_args));
	log_tag("debug_log.txt","[TURNOP]",msg);
	kls_log("DEBUG",msg);
	turnOP_args* res = (turnOP_args*) KLS_PUSH_T_NAMED(tkls,turnOP_args,1,"turnOP_args",msg);

	res->actor = actor;
	res->path = path;
	res->room = room;
	res->load_info = load_info;
	res->enemy = enemy;
	res->boss = boss;
	res->save_file = save_file;
	res->notify_win = notify_win;
	res->t_kls = t_kls;

	return res;
}

/**
 * Takes a string and returns the corresponding saveType.
 * Will return -1 if no match is found.
 * @param s The string to convert.
 */
saveType saveTypeFrom_string(char* s) {
	int check = -1;
	char msg[500];

	for (int i = 0; i < SAVETYPE_MAX+1; i++) {
		if ((check = strcmp(s,stringFrom_saveType(i))) == 0) {
			sprintf(msg,"Matched saveType string [%s] to (%i).", s, i);
			log_tag("debug_log.txt","[DEBUG]",msg);
			return (saveType) i;
		}
	}

	sprintf(msg,"No match found for saveType string [%s].", s);
	log_tag("debug_log.txt","[ERROR]",msg);
	return -1;
}

/**
 * Logs a turnOption_OP to "$STATICDIR/OPS_log.txt".
 * @param op The turnOption_OP to log.
 */
void log_OP(turnOption_OP op) {
	char msg[500];
	sprintf(msg,"[OP_code: %i, value: %s]", op, stringFromTurnOP(op));
	log_tag(OPS_LOGFILE,"[LOG]",msg);
	log_tag("debug_log.txt","[LOG_TURNOP]",msg);
}

/**
 * Prints an ASCII escape code that makes subsequent output a thick red color.
 */
void red(void) {
	printf("\033[1;31m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light red color.
 */
void lightRed(void) {
	printf("\033[0;31m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a thick white color.
 */
void strongWhite(void) {
	printf("\033[1;37m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light white color.
 */
void white(void) {
	printf("\033[0;37m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a thick green color.
 */
void green(void) {
	printf("\033[1;32m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light green color.
 */
void lightGreen(void) {
	printf("\033[0;32m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a thick yellow color.
 */
void yellow(void) {
	printf("\033[1;33m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light yellow color.
 */
void lightYellow(void) {
	printf("\033[0;33m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a thick blue color.
 */
void blue(void) {
	printf("\033[1;34m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light blue color.
 */
void lightBlue(void) {
	printf("\033[0;34m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a thick purple color.
 */
void purple(void) {
	printf("\033[1;35m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light purple color.
 */
void lightPurple(void) {
	printf("\033[0;35m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a thick cyan color.
 */
void cyan(void) {
	printf("\033[1;36m");
}
/**
 * Prints an ASCII escape code that makes subsequent output a light cyan color.
 */
void lightCyan(void) {
	printf("\033[0;36m");
}

/**
 * Initialises color pairs for the game.
 */
void init_game_color_pairs(void) {

	// Initialize all the colors
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_BLACK, COLOR_WHITE);
	init_pair(8, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(9, COLOR_WHITE, COLOR_RED);
	init_pair(10, COLOR_WHITE, COLOR_MAGENTA);

}

/**
 * Sleeps t seconds and then flushes stdin.
 * The tcflush() function comes from termios and may not be supported on all platforms.
 * @param t A float for how many seconds to sleep.
 */
void screenTime(float t) {
	sleep(t);

	//Avoid leftovers from stdin be picked up while sleeping
	tcflush(0,TCIFLUSH);
}

/**
 * Sets the passed char array to the expected path for /static/ folder.
 * @param static_path The array that will hold path to static folder.
 */
void resolve_staticPath(char static_path[500]){
	char homedir_path[200];
	sprintf(homedir_path, "%s", getenv("HOME"));

	const char* static_folder_path_wd = "./static/";
	const char* local_install_static_folder_path = "/helapordo-local/static";
	char static_folder_path_global[500];
	sprintf(static_folder_path_global,"%s",homedir_path);
	strncat(static_folder_path_global,local_install_static_folder_path,50);
    	struct stat sb;

    	if (stat(static_folder_path_wd, &sb) == 0 && S_ISDIR(sb.st_mode)) {
		//sprintf(msg, "[DEBUG]    resolve_staticPath(): Found \"/static/\" dir in working directory (%s).\n",static_folder_path_wd);
		strcpy(static_path,static_folder_path_wd);
    	} else {
		//sprintf(msg, "[DEBUG]    resolve_staticPath(): Can't find \"/static/\" dir in working directory (%s). Will try \"%s/helapordo-local/static/\".\n", static_folder_path_wd, homedir_path);
    		if (stat(static_folder_path_global, &sb) == 0 && S_ISDIR(sb.st_mode)) {
			//sprintf(msg, "[DEBUG]    resolve_staticPath(): Found \"/static/\" dir in global directory: \"%s/helapordo-local/static/\".\n", homedir_path);
			strcpy(static_path,static_folder_path_global);
		} else {
			//sprintf(msg,"[DEBUG]    resolve_staticPath(): Can't find \"/static/\" dir in \"%s/helapordo-local/static/\". Quitting.\n", homedir_path);
			fprintf(stderr,"\n[ERROR]    Can't find static dir. Quitting.\n");
			fprintf(stderr,"\nHome dir at: (%s).\n",homedir_path);
			fprintf(stderr,"\nGlobal static dir at: (%s).\n",static_folder_path_global);
			fprintf(stderr,"\nWorking static dir at: (%s).\n",static_folder_path_wd);
			exit(EXIT_FAILURE);
		}
	}
}

/**
 * Loads the lore strings from the appropriate lore file.
 * @param lore_strings The string array to copy to.
 * @param loreKind The kind of lore, used to open the corresponding lore file.
 * @see gameloop()
 */
void loadLore(char** lore_strings, int loreKind) {


	char static_path[500];

	// Set static_path value to the correct static dir path
	resolve_staticPath(static_path);

	char lore_filename[1000];

	sprintf(lore_filename, "%s/lore/lore%i.txt", static_path, loreKind);

	FILE* lorefile = fopen(lore_filename,"r");
	if (!lorefile) {
		fprintf(stderr,"\nCan't open %s\n",lore_filename);
		exit(EXIT_FAILURE);
	}

    	char * line = NULL;
    	size_t len = 0;
    	ssize_t read;
	int i = 0;
    	while ((read = getline(&line, &len, lorefile)) != -1) {
		char msg[1000];
		sprintf(msg,"Retrieved line of length %zu:", read);
        	log_tag("debug_log.txt","[LORE-LOAD]",msg);
		sprintf(msg,"%s", line);
        	log_tag("debug_log.txt","[LORE-LOAD]",msg);
		strcpy(lore_strings[i],line);
		i++;
	}
    	fclose(lorefile);
    	if (line)
        	free(line);
}

/**
 * Takes a path pointer, a roadFork value pointer, a room type pointer, and an integer.
 * Depending on GAMEMODE (and eventually roadFork value), sets the room type pointer to a roomClass value.
 * @param roadFork_value The pointer value of the roadfork that we must use as result.
 * @param room_type The pointer value of the room type to set.
 * @param roomsDone The total of rooms completed.
 */
void setRoomType(Path* path, int* roadFork_value, roomClass* room_type, int roomsDone) {
	char msg[500];
	sprintf(msg,"room type (%i) rooms done (%i)", (int) *room_type, roomsDone);
	log_tag("debug_log.txt","[DEBUG]",msg);
	switch (GAMEMODE) {
		case Standard: {
				if ((*roadFork_value >= 0)) {
					*room_type = *roadFork_value;
					*roadFork_value = -1;
					log_tag("debug_log.txt","[TEST]","setRoomType() for ROADFORK");
				} else if ((roomsDone == 1) || (roomsDone % HOMEROOM == 0)) { //Only the first and every nth room will be a HOME room.
					//FIXME: why the hell does roomsDone need to start from 1?
					*room_type = HOME;
					log_tag("debug_log.txt","[TEST]","setRoomType() for HOME");
				} else if ( roomsDone % BOSSROOM == 0) {
					*room_type = BOSS;
				} else if (roomsDone % SHOPROOM == 0) {
					*room_type = SHOP;
				} else if (rand() % 5 == 0) {
					*room_type = TREASURE;
				} else if (rand() % 4 == 0 && (roomsDone +2 < path->length)) {
					*room_type = ROADFORK;
				} else if (*room_type == -1) {
					*room_type = ENEMIES;
				}
				if ((G_DEBUG_ROOMTYPE_ON > 0)) {
					sprintf(msg,"setRoomType(): Room debug flag asserted in standard gamemode, room type will always be equal to G_DEBUG_ROOMTYPE (%s).\n",stringFromRoom(G_DEBUG_ROOMTYPE));
					log_tag("debug_log.txt","\n[DEBUG]",msg);
					*room_type = G_DEBUG_ROOMTYPE;
				}
		}
		break;
		case Story: {
				if (*roadFork_value >= 0) { //Is this branch needed here?
					*room_type = *roadFork_value;
					*roadFork_value = -1;
				} else if ((roomsDone == 1) || (roomsDone % HOMEROOM == 0)) { //Only the first and every nth room will be a HOME room.
					//FIXME: why the hell does roomsDone need to start from 1?
					*room_type = HOME;
					log_tag("debug_log.txt","[TEST]","story mode, setRoomType() for HOME");
				} else if ( roomsDone % BOSSROOM == 0) {
					*room_type = BOSS;
				} else if (roomsDone % 4 == 0) {
					*room_type = SHOP;
				} else if (rand() % 20 == 0) {
					*room_type = TREASURE;
				} else if (*room_type == -1) {
					*room_type = ENEMIES;
				}
		}
		break;
		default: {
			fprintf(stderr,"Unexpected GAMEMODE value: %i\n",GAMEMODE);
			exit(EXIT_FAILURE);
		}
		break;
	}
	sprintf(msg,"room type (%i) rooms done (%i)", (int) *room_type, roomsDone);
	log_tag("debug_log.txt","[DEBUG]",msg);
}
/**
 * Frees the memory allocated for the passed room pointer.
 * @param room The Room pointer to free.
 * @see Room
 */
void freeRoom(Room* room) {
	char msg[200];
	int room_type = room->class;

	sprintf(msg,"Freeing room desc: (%s).", room->desc);
	log_tag("debug_log.txt","[DEBUG-FREE]",msg);
	//Free room memory
	//FIXME: do we have all room descs handled ?
	//free(room->desc);

	if (room_type == SHOP) {
		/*
		for (int i = 0 ; i < room->shop->equipsCount; i++) {
			Equip* e = room->shop->equips[i];
			if (e->perksCount > 0) {
				for (int j = 0 ; j < e->perksCount; j++) {
					Perk* p = e->perks[j];
					free(p);
					sprintf(msg,"Freeing %s room (index %i), freed Perk %i for Equip %s.", stringFromRoom(room_type), room->index, j, stringFromEquips(e->class));
        				log_tag("debug_log.txt","[FREE]",msg);
				}
        			log_tag("debug_log.txt","[FREE]","Done freeing perks.\n");
			}
			sprintf(msg,"Freeing %s room (index %i), freed Equip %s.", stringFromRoom(room_type), room->index, stringFromEquips(e->class));
			free(e);
        		log_tag("debug_log.txt","[DEBUG-FREE]",msg);
		}
		for (int i = 0 ; i < room->shop->consumablesCount; i++) {
			Consumable* c = room->shop->consumables[i];
			free(c);
			sprintf(msg,"Freeing %s room (index %i), freed Consumable %i.", stringFromRoom(room_type), room->index, i);
        		log_tag("debug_log.txt","[DEBUG-FREE]",msg);
		}
		*/
			//FIXME: remove all the commented out bs
        		log_tag("debug_log.txt","[FIXME]","Empty freeRoom branch");
	} else if (room_type == TREASURE) {
		if (room->treasure->class == TREASURE_CONSUMABLE) {

			/*
			Consumable* dele = room->treasure->consumable;
			sprintf(msg,"Freeing %s room (index %i), freed Consumable %s.\n", stringFromRoom(room_type), room->index, stringFromConsumables(dele->class));
			free(dele);
			*/
			//FIXME: remove all the commented out bs
        		log_tag("debug_log.txt","[FIXME]","Empty freeRoom branch");
		} else if (room->treasure->class == TREASURE_ARTIFACT) {
			/*
			Artifact* dele = room->treasure->artifact;
			sprintf(msg,"Freeing %s room (index %i), freed Artifact %s.\n", stringFromRoom(room_type), room->index, stringFromArtifacts(dele->class));
			free(dele);
			*/
        		log_tag("debug_log.txt","[FIXME]","Empty freeRoom branch");
		} else if (room->treasure->class == TREASURE_CHEST) {
        		log_tag("debug_log.txt","[FIXME]","freeRoom: freeing Treasure room, CHEST");
			/*
			//FIXME: freeing Treasure Chest here
			Chest* chest = room->treasure->chest;
			for (int eq_i = 0; eq_i < chest->equipsCount; eq_i++) {
				Equip* equip = chest->equips[eq_i];
				for (int p_i = 0 ; p_i < equip->perksCount; p_i++) {
					Perk* pk = equip->perks[p_i];
					free(pk);
					sprintf(msg,"Freeing %s room (index %i), CHEST: freed Perk %i for Equip %s (%i).", stringFromRoom(room_type), room->index, p_i, stringFromEquips(equip->class), eq_i);
					log_tag("debug_log.txt","[DEBUG-FREE]",msg);
				}
				sprintf(msg,"Freeing %s room (index %i), CHEST: freed Equip %s (%i).", stringFromRoom(room_type), room->index, stringFromEquips(equip->class), eq_i);
				free(equip);
				log_tag("debug_log.txt","[DEBUG-FREE]",msg);
			}

			for (int i = 0 ; i < chest->consumablesCount; i++) {
				Consumable* c = chest->consumables[i];
				free(c);
				sprintf(msg,"Freeing %s room (index %i), CHEST: freed Consumable %i.", stringFromRoom(room_type), room->index, i);
				log_tag("debug_log.txt","[DEBUG-FREE]",msg);
			}
			sprintf(msg,"Freeing %s room (index %i), freed Chest.\n", stringFromRoom(room_type), room->index);
			//free(chest);
			log_tag("debug_log.txt","[DEBUG-FREE]",msg);
			*/
        		log_tag("debug_log.txt","[FIXME]","Empty freeRoom branch");
		}
	} else if (room_type == ROADFORK) {
		/*
		Roadfork* rfk = room->roadfork;
		free(rfk);
		sprintf(msg,"Freeing %s room (index %i), freed Roadfork.\n", stringFromRoom(room_type), room->index);
		log_tag("debug_log.txt","[DEBUG-FREE]",msg);
		*/
        	log_tag("debug_log.txt","[FIXME]","Empty freeRoom branch");
	}

	sprintf(msg,"Freed %s room, index %i.\n", stringFromRoom(room->class), room->index);
	log_tag("debug_log.txt","[DEBUG-FREE]",msg);
        log_tag("debug_log.txt","[FIXME]","freeRoom(): done fake free().");
	//free(room);
}

/**
 * Prints an hardcoded title screen.
 */
void printTitle(void){
	printf("\n\n\n\n\n");
	red();
	printf("     __  __  ____  __     ______  _____   ______  ____    _____    ______\n");
	printf("    / / / / / __/ / /    / __  / / __  / / __  / / __ \\  / __  \\  / __  /\n");
	printf("   / /_/ / / /_  / /    / / / / / /_/ / / / / / / /_/ / / / /  / / / / /\n");
	printf("  / __  / / __/ / /    / /_/ / / ____/ / / / / /    _/ / / /  / / / / /\n");
	printf(" / / / / / /_  / /_   / __  / / /     / /_/ / / /\\ |  / /_/ _/ / /_/ /\n");
	printf("/_/ /_/ /____/ \\___/ /_/ /_/ /_/     /_____/ /_/ /_/ /_____/  /_____/\n");
	white();
}

/**
 * Prints version string.
 */
void printVersion(void){
	printf("%s\n", VERSION);
}

/**
 * Prints formatted version string.
 * @param progName The program name string.
 */
void printFormattedVersion(char* progName){
	printf("%s v. %s\n", progName, VERSION);
}

/**
 * Prints correct argument syntax for command line invocation.
 */
void usage(char* progname) {
	fprintf(stderr,"\nUsage:        %s [...options] [name] [class]\n", progname);
	fprintf(stderr,"Options:\n");
	fprintf(stderr,"\n    -R        Enable rogue mode\n");
	fprintf(stderr,"\n    -s        Enable story mode. Deprecated.\n");
	fprintf(stderr,"    -l        Load a game.\n");
	fprintf(stderr,"\n    -d        Enable debug mode\n");
	fprintf(stderr,"      -dr <roomType>      Enable forced room.\n");
	fprintf(stderr,"      -dE <enemyType>      Enable forced enemy.\n");
	fprintf(stderr,"\n    -h        Print this help message\n");
	fprintf(stderr,"    -T        Show a brief tutorial.\n");
	fprintf(stderr,"    -G        Enable godmode.\n");
	fprintf(stderr,"    -X        Enable experimental features.\n");
	fprintf(stderr,"    -v        Prints %s version.\n",progname);
	fprintf(stderr,"    -a        Disable autosave.\n");
	fprintf(stderr,"    -L        Enable logging.\n");
	fprintf(stderr,"    -Q        Enable fast quit.\n");
	fprintf(stderr,"    -t        Test color codes.\n");
}

/**
 * Takes a filename string, a string headear and a string message (assumes correctly formed strings).
 * Tries logging the message to the passed file if global var G_DEBUG_ON is set.
 * @param filename The filename to open.
 * @param header The string header for the message to log.
 * @param msg The string to log.
 */
void log_tag(char* filename, char* header, char* msg) {
	// Open log file if log flag is set and append to it
	if (G_LOG_ON) {
		char path_to_debug_file[500];
		char static_path[500];

		// Set static_path value to the correct static dir path
		resolve_staticPath(static_path);

		sprintf(path_to_debug_file,"%s",static_path);
		strncat(path_to_debug_file,"/",2);
		strncat(path_to_debug_file, filename, 200);

		//fprintf(stderr, "Using %s as path to debug log.\n", path_to_debug_file);

		FILE* logfile = fopen(path_to_debug_file, "a");
		if (!logfile) {
			fprintf(stderr, "Error opening log file.\n Static path: (%s) Filename : (%s).\n", static_path, filename);
			fprintf(stderr, "Path to debug file was: (%s).", path_to_debug_file);
			fprintf(stderr, "Msg was:    %s", msg);
			exit(EXIT_FAILURE);
		}
		time_t now = time(0);
		struct tm *mytime = localtime(&now);
		char timeheader[500];
		if ( strftime(timeheader, sizeof timeheader, "%X", mytime) )
		{
			fprintf(logfile,"%s %-12.12s    %s\n", timeheader, header, msg);
		}
		fclose(logfile);
	}
}

/**
 * Takes a OP_res and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see OP_res
 * @param r The OP_res.
 * @return String corresponding to the OP result.
 */
char* stringFrom_OP_res(OP_res r) {

	return opresstrings[r];
}

/**
 * Takes a saveType and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see saveType
 * @param s The saveType value.
 * @return String corresponding to the save type.
 */
char* stringFrom_saveType(saveType s) {

	return savetypestrings[s];
}

/**
 * Takes a turnOption_OP and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see turnOption_OP
 * @param t The turn choice.
 * @return String corresponding to the turn choice.
 */
char* stringFromTurnOP(turnOption_OP t) {

	return turnopstrings[t];
}

/**
 * Takes a fightResult fr and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fightResult
 * @param fr The fight result value.
 * @return String corresponding to the fight result value.
 */
char* stringFrom_fightResult(fightResult fr) {

	return fightresultstrings[fr];
}

/**
 * Takes a fighterStatus and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterStatus
 * @param s The status.
 * @return String corresponding to the status.
 */
char* stringFromStatus(fighterStatus s) {
	static char* strings[] = {
		"Normal",
		"Poison",
		"Frozen",
		"Burned",
		"Weak",
		"Strong"
	};

	return strings[s];
}

/**
 * Takes a winconClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see winconClass
 * @param w The class.
 * @return String corresponding to the class.
 */
char* stringFromWinconClass(winconClass w) {

	return winconstrings[w];
}

/**
 * Takes a fighterClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @param f The class.
 * @return String corresponding to the class.
 */
char* stringFromClass(fighterClass f) {

	return classesstrings[f];
}

/**
 * Takes a enemyClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see enemyClass
 * @param e The class.
 * @return String corresponding to the class.
 */
char* stringFromEClass(enemyClass e) {

	return classenemystrings[e];
}

/**
 * Takes a bossClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see bossClass
 * @param b The class.
 * @return String corresponding to the class.
 */
char* stringFromBossClass(bossClass b) {

	return classbossstrings[b];
}

/**
 * Takes a integer and returns the corresponding consumable string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see consumableClass
 * @param c The integer/consumableClass.
 * @return String corresponding to the consumable.
 */
char* stringFromConsumables(int c) {

	return consumablestrings[c];
}

/**
 * Takes a integer and returns the corresponding equip string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see equipClass
 * @param e The integer/equipClass.
 * @return String corresponding to the equip.
 */
char* stringFromEquips(int e) {

	return equipstrings[e];
}

/**
 * Takes a integer and returns the corresponding equipzone string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see Equipzone
 * @param z The integer/Equipzone.
 * @return String corresponding to the equipzone.
 */
char* stringFromEquipzones(int z) {

	return equipzonestrings[z];
}

/**
 * Takes a integer and returns the corresponding quality string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see quality
 * @param q The integer/quality
 * @return String corresponding to the quality.
 */
char* stringFromQuality(int q) {

	return qualitytrings[q];
}

/**
 * Takes a integer and returns the corresponding artifact string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see artifactClass
 * @param a The integer/artifactClass.
 * @return String corresponding to the artifact.
 */
char* stringFromArtifacts(int a) {

	return artifactstrings[a];
}

/**
 * Takes a fighterClass and a specialMove and returns the corresponding name string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @see specialMove
 * @param class The fighterClass.
 * @param move The specialMove.
 * @return String corresponding to the special move.
 */
char* nameStringFromSpecial(fighterClass class, specialMove move) {
	return specialsnamestrings[class][(move % (SPECIALSMAX + 1))];
}

/**
 * Takes a fighterClass and a specialMove and returns the corresponding desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @see specialMove
 * @param class The fighterClass.
 * @param move The specialMove.
 * @return String corresponding to the special move.
 */
char* descStringFromSpecial(fighterClass class, specialMove move) {
	return specialsdescstrings[class][(move % (SPECIALSMAX + 1))];
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
int costFromSpecial(fighterClass class, specialMove move) {
	return specialscosts[class][(move % (SPECIALSMAX + 1)) ];
}

/**
 * Takes a integer and returns the corresponding perk name string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see perkClass
 * @see Perk
 * @param p The integer/perkClass.
 * @return String corresponding to the perk name.
 */
char* nameStringFromPerk(int p) {

	return perksnamestrings[p];
}

/**
 * Takes a integer and returns the corresponding perk desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see perkClass
 * @see Perk
 * @param p The integer/perkClass.
 * @return String corresponding to the perk desc.
 */
char* descStringFromPerk(int p) {

	return perksdescstrings[p];
}

/**
 * Takes a integer and returns the corresponding treasure desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see treasureClass
 * @param t The integer/treasureClass.
 * @return String corresponding to the treasure desc.
 */
char* descStringFromTreasure(int t) {

	return treasuredescstrings[t];
}

/**
 * Takes a integer and returns the corresponding chest desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see chestClass
 * @param c The integer/chestClass.
 * @return String corresponding to the chest desc.
 */
char* descStringFromChest(int c) {
	return chestdescstrings[c];
}

/**
 * Takes a integer and returns the corresponding FoeParty desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see foePartyClass
 * @param c The integer/foePartyClass.
 * @return String corresponding to the FoeParty desc.
 */
char* stringFromFoePartyClass(foePartyClass fp) {
	return foepartystrings[fp];
}

/**
 * Takes a integer and returns the corresponding room name string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see roomClass
 * @param r The integer/roomClass.
 * @return String corresponding to the room name.
 */
char* stringFromRoom(roomClass r) {
	return roomnamestrings[r];
}

/**
 * Takes a integer and returns the corresponding gamemode string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see Gamemode
 * @param g The integer/Gamemode.
 * @return String corresponding to the gamemode.
 */
char* stringFromGamemode(Gamemode g) {
	return gamemodenamestrings[g];
}

/**
 * Takes a Fighter pointer and sets its name value to the string provided on stdin.
 * @param player The pointer whose name value will be set.
 */
void scanName(Fighter* player) {
	char name[50];
	int f;
	do {
		printf("\n\n\tWhat's your name?\n\n");
		f = scanf("%20s",name);
	} while (f >= 21 || f <= 0 );
	strcpy(player->name,name);
}

/**
 * Prints all the fighterClass values as integers and as strings.
 * @see fighterClass
 */
void printClasses(void) {
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
int scanClass(void) {
	int pick=-1;
	char c;
	char msg[300];
	while (pick < 0 || pick > CLASSESMAX ) {
		printf("\nPick a class: (0-%i)\n",CLASSESMAX);
		int res = scanf("%1i",&pick);
		sprintf(msg,"scanClass scanf() res was %i",res);
		log_tag("debug_log.txt","[DEBUG]",msg);
		res = scanf("%c",&c);
		sprintf(msg,"scanClass 2 scanf() res was %i",res);
		log_tag("debug_log.txt","[DEBUG]",msg);
	};
	return pick;
}

/**
 * Takes a Fighter pointer and sets the class value after asking the user for input.
 * @see fighterClass
 * @see scanClass()
 * @param player The pointer whose class value will be set.
 */
void pickClass(Fighter* player) {
	int pick=-1;
	char msg[500];
	do {
		int res = system("clear");
		sprintf(msg,"pickClass() system(\"clear\") res was (%i)",res);
		log_tag("debug_log.txt","[DEBUG]",msg);
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
 * Prints all the winconClass values as integers and as strings.
 * @see winconClass
 */
void printWincons(void) {
	printf("[");
	for (int i=0; i<=WINCON_CLASS_MAX; i++) {
		printf("\t%d)\t%s",i,stringFromWinconClass(i));
		if (i!=WINCON_CLASS_MAX) { printf("\n"); }
	};
	printf("\t]\n");
}

/**
 * Asks for an integer on stdin and returns it if it's a valid winconClass index.
 * @see winconClass
 * @see WINCON_CLASS_MAX
 * @return The selected integer, negative for unvalid choices.
 */
int scanWincon(void) {
	int pick=-1;
	char c;
	char msg[500];
	while (pick < 0 || pick > WINCON_CLASS_MAX ) {
		printf("\nPick a win condition: (0-%i)\n",WINCON_CLASS_MAX);
		int res = scanf("%1i",&pick);
		sprintf(msg,"scanWincon() scanf() res was (%i)",res);
		log_tag("debug_log.txt","[DEBUG]",msg);
		res = scanf("%c",&c);
		sprintf(msg,"scanWincon() 2 scanf() res was (%i)",res);
		log_tag("debug_log.txt","[DEBUG]",msg);
	};
	return pick;
}

/**
 * Takes a Fighter pointer and sets the name value after asking the user for input.
 * @see fighterClass
 * @see scanName()
 * @param player The pointer whose name value will be set.
 */
void pickName(Fighter* player) {
	scanName(player);
	red();
	printf("\n\n\tName: %s\n\n",player->name);
	white();
}

/**
 * Takes a Wincon pointer and sets its class after asking the user for input.
 * @see winconClass
 * @see scanWincon()
 * @param w The wincon pointer whose class value will be set.
 */
void pickWincon(Wincon* w) {
	int pick=-1;
	char msg[500];
	do {
		int res = system("clear");
		sprintf(msg,"pickWincon() system(\"clear\") res was (%i)",res);
		log_tag("debug_log.txt","[DEBUG]",msg);
		printf("\nPick a win condition.\n");
		printWincons();
		pick = scanWincon();
	} while (pick<0);

	w->class = pick;
	green();
	printf("\n\n\tWincon: %s\n\n",stringFromWinconClass(w->class));
	white();
}

/**
 * Takes two Fighter pointers, with all their fields already allocated, and copies all valaues from source to dest.
 * @see Fighter
 * @param source The Fighter pointer to be copied.
 * @param dest The Fighter pointer to copy to.
 */
void copy_fighter(Fighter* source, Fighter* dest) {

	strcpy(source->name, dest->name);
	dest->class = source->class;
	dest->hp = source->hp;
	dest->atk = source->atk;
	dest->def = source->def;
	dest->vel = source->vel;
	dest->level = source->level;
	dest->luck = source->luck;
	dest->totalxp = source->totalxp;
	dest->currentlevelxp = source->currentlevelxp;
	dest->totallevelxp = source->totallevelxp;
	dest->totalhp = source->totalhp;
	dest->status = source->status;

	dest->energy = source->energy;
	dest->totalenergy = source->totalenergy;
	for (int i = 0; i<SPECIALSMAX+1; i++) {
	  dest->specials[i] = source->specials[i];
	}

	for (int i = 0; i<COUNTERSMAX+1; i++) {
	  dest->counters[i] = source->counters[i];
	}
	dest->turnboost_atk = source->turnboost_atk;
	dest->turnboost_def = source->turnboost_def;
	dest->turnboost_vel = source->turnboost_vel;
	dest->turnboost_enr = source->turnboost_enr;

	dest->perksCount = source->perksCount;
	for (int i = 0; i<PERKSMAX+1; i++) {
	  dest->perks[i] = source->perks[i];
	}

	for (int i = 0; i<EQUIPZONES+1; i++) {
	  dest->equipslots[i] = source->equipslots[i];
	}
	for (int i = 0; i<EQUIPSBAGSIZE+1; i++) {
	  dest->equipsBag[i] = source->equipsBag[i];
	}
	for (int i = 0; i<CONSUMABLESMAX+1; i++) {
	  dest->consumablesBag[i] = source->consumablesBag[i];
	}
	for (int i = 0; i<ARTIFACTSMAX+1; i++) {
	  dest->artifactsBag[i] = source->artifactsBag[i];
	}

	dest->equipsBagOccupiedSlots = source->equipsBagOccupiedSlots;
	dest->earliestBagSlot = source->earliestBagSlot;

	dest->permboost_atk = source->permboost_atk;
	dest->permboost_def = source->permboost_def;
	dest->permboost_vel = source->permboost_vel;
	dest->permboost_enr = source->permboost_enr;

	dest->equipboost_atk = source->equipboost_atk;
	dest->equipboost_def = source->equipboost_def;
	dest->equipboost_vel = source->equipboost_vel;
	dest->equipboost_enr = source->equipboost_enr;

	dest->stats = source->stats;
	dest->balance = source->balance;
}
