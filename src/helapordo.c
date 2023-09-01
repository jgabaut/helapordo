#include "helapordo.h"

/**
 * Takes a turnOption_OP and a pointer to a turnOP_args struct.
 * Logs a warning for any NULL field in the struct.
 * Performs the defined turn operation, before returning an OP_res.
 * @param op The kind of operation to do.
 * @param args Struct containing needed args for current operation. Can have some fields uninitialised, if not relevant to requested turnOP.
 * @param kls The Koliseo used for allocations.
 * @param t_kls The Koliseo_Temp used for temporary allocations.
 * @return An OP_res representing result of turn option operation.
 * @see turnOP_args
 * @see turnOption_OP
 * @see OP_res
 */
OP_res turnOP(turnOption_OP op, turnOP_args* args, Koliseo* kls, Koliseo_Temp* t_kls) {

	char msg[500];
	OP_res res = INVALID_OP;

	Fighter* actor = args->actor;
	if (actor == NULL) log_tag("debug_log.txt","[WARN]","turnOP_args->(actor) was NULL");
	Path* path = args->path;
	if (path == NULL) log_tag("debug_log.txt","[WARN]","turnOP_args->(path) was NULL");
	Room* room = args->room;
	if (room == NULL) log_tag("debug_log.txt","[WARN]","turnOP_args->(room) was NULL");
	loadInfo* load_info = args->load_info;
	if (load_info == NULL) log_tag("debug_log.txt","[WARN]","turnOP_args->(load_info) was NULL");
	Enemy* enemy = args->enemy;
	if (enemy == NULL) log_tag("debug_log.txt","[WARN]","turnOP_args->(enemy) was NULL");
	Boss* boss = args->boss;
	if (boss == NULL) log_tag("debug_log.txt","[WARN]","turnOP_args->(boss) was NULL");
	FILE* save_file = args->save_file;
	if (save_file == NULL) log_tag("debug_log.txt","[WARN]","turnOP_args->(save_file) was NULL");
	WINDOW* notify_win = args->notify_win;
	if (notify_win == NULL) log_tag("debug_log.txt","[WARN]","turnOP_args->(notify_win) was NULL");

	int isBoss = -1;
	int room_index = -1;
	int enemy_index = -1;

	//Log operation code
	log_OP(op);
	switch(op) {
		case OP_INVALID: {
			//No_OP, but unexpected
			log_tag("debug_log.txt","[ERROR]","Done turnOP() on an OP_INVALID opcode.");
		}
		break;
		case OP_NEW_GAME: {
			res = NO_OP;
		}
		break;
		case OP_LOAD_GAME: {
			if (load_info == NULL) {
				log_tag("debug_log.txt","[ERROR]","turnOP_args->(load_info) was NULL");
				//free(args);
				log_tag("debug_log.txt","[FREE]","Freed turnOP_args");
				return res;
			}
			load_info->is_new_game=0;
			res = OP_OK;
		}
		break;
		case OP_LOAD_ENEMYROOM: {
			if (load_info == NULL) {
				log_tag("debug_log.txt","[ERROR]","turnOP_args->(load_info) was NULL");
				//free(args);
				log_tag("debug_log.txt","[FREE]","Freed turnOP_args");
				return res;
			}
			load_info = args->load_info;
			if (load_info == NULL) {
				sprintf(msg,"load_info was NULL after load_info = args->load_info:  in turnOP(OP_LOAD_ENEMYROOM");
				log_tag("debug_log.txt","[ERROR]",msg);
				return res;
			}
			save_file = args->save_file;

			if (save_file == NULL) {
				log_tag("debug_log.txt","[ERROR]","turnOP_args->(save_file) was NULL");
				//free(args);
				log_tag("debug_log.txt","[FREE]","Freed turnOP_args");
				return res;
			}
			int* ptr_to_loaded_enemy_index = &(load_info->enemy_index);
			sprintf(msg,"*(ptr_to_loaded_enemy_index) == [%i]",*ptr_to_loaded_enemy_index);
			log_tag("debug_log.txt","[TURNOP]",msg);
			int* ptr_to_loaded_roomtotalenemies = (load_info->ptr_to_roomtotalenemies);
			sprintf(msg,"*(ptr_to_loaded_roomtotalenemies) == [%i]",*ptr_to_loaded_roomtotalenemies);
			log_tag("debug_log.txt","[TURNOP]",msg);
			int* ptr_to_loaded_roomindex = (load_info->ptr_to_roomindex);
			sprintf(msg,"*(ptr_to_loaded_roomindex) == [%i]",*ptr_to_loaded_roomindex);
			log_tag("debug_log.txt","[TURNOP]",msg);
			int* tot_foes = &(load_info->total_foes);
			sprintf(msg,"*(tot_foes) == [%i]",*tot_foes);
			log_tag("debug_log.txt","[TURNOP]",msg);
			int* ptr_to_done_loading = &(load_info->done_loading);
			sprintf(msg,"*(done_loading) == [%i]",*ptr_to_done_loading);
			log_tag("debug_log.txt","[TURNOP]",msg);
			res = handleLoadgame_Enemies(save_file, actor, path, load_info->loaded_enemy, ptr_to_loaded_enemy_index, ptr_to_loaded_roomtotalenemies, ptr_to_loaded_roomindex, tot_foes, ptr_to_done_loading, kls);
			//Log end of operation
			sprintf(msg,"Done operation: [%s] res: [%s (%i)]",stringFromTurnOP(op), stringFrom_OP_res(res), res);
			log_tag("debug_log.txt","[TURNOP]",msg);

			sprintf(msg,"res: [%s (%i)]", stringFrom_OP_res(res),res);
			log_tag(OPS_LOGFILE,"[RES]",msg);

			return res;
		}
		break;
		case OP_LOAD_HOMEROOM: {
			if (load_info == NULL) {
				log_tag("debug_log.txt","[ERROR]","turnOP_args->(load_info) was NULL");
				//free(args);
				log_tag("debug_log.txt","[FREE]","Freed turnOP_args");
				return res;
			}
			load_info = args->load_info;
			if (load_info == NULL) {
				sprintf(msg,"load_info was NULL after load_info = args->load_info:  in turnOP(OP_LOAD_HOMEROOM");
				log_tag("debug_log.txt","[ERROR]",msg);
				return res;
			}
			save_file = args->save_file;

			if (save_file == NULL) {
				log_tag("debug_log.txt","[ERROR]","turnOP_args->(save_file) was NULL");
				//free(args);
				log_tag("debug_log.txt","[FREE]","Freed turnOP_args");
				return res;
			}
			if (load_info->ptr_to_roomindex == NULL) {
				sprintf(msg,"load_info->ptr_to_roomindex was NULL in turnOP(OP_LOAD_HOMEROOM");
				log_tag("debug_log.txt","[ERROR]",msg);
				return res;
			}
			int* ptr_to_loaded_roomindex = (load_info->ptr_to_roomindex);
			if (ptr_to_loaded_roomindex == NULL) {
				sprintf(msg,"ptr_to_loaded_roomindex was NULL in turnOP(OP_LOAD_HOMEROOM");
				log_tag("debug_log.txt","[ERROR]",msg);
				return res;
			}
			sprintf(msg,"*(ptr_to_loaded_roomindex) == [%i]",*ptr_to_loaded_roomindex);
			log_tag("debug_log.txt","[TURNOP]",msg);

			int* ptr_to_done_loading = &(load_info->done_loading);
			sprintf(msg,"*(done_loading) == [%i]",*ptr_to_done_loading);
			log_tag("debug_log.txt","[TURNOP]",msg);
			res = handleLoadgame_Home(save_file, actor, path, ptr_to_loaded_roomindex, ptr_to_done_loading, kls);
			//Log end of operation
			sprintf(msg,"Done operation: [%s] res: [%s (%i)]",stringFromTurnOP(op), stringFrom_OP_res(res), res);
			log_tag("debug_log.txt","[TURNOP]",msg);

			sprintf(msg,"res: [%s (%i)]", stringFrom_OP_res(res),res);
			log_tag(OPS_LOGFILE,"[RES]",msg);

			return res;
		}
		break;
		case OP_FIGHT: {
			if (notify_win == NULL) {
				log_tag("debug_log.txt","[CRITICAL]","Notification WINDOW pointer was null in turnOP(OP_FIGHT)");
				exit(EXIT_FAILURE);
			}
			if (room == NULL) {
				log_tag("debug_log.txt","[CRITICAL]","Room pointer was null in turnOP(OP_FIGHT)");
				exit(EXIT_FAILURE);
			}
			room_index = room->index;
			if (room->class == ENEMIES && enemy == NULL) {
				log_tag("debug_log.txt","[ERROR]","Enemy pointer was null in turnOP(OP_FIGHT) for ENEMIES room.");
				exit(EXIT_FAILURE);
			} else if (room->class == BOSS && boss == NULL){
				log_tag("debug_log.txt","[ERROR]","Boss pointer was null in turnOP(OP_FIGHT) for BOSS room.");
				exit(EXIT_FAILURE);
			} else if (room->class != ENEMIES && room->class != BOSS) {
				sprintf(msg,"Invalid room class in turnOP(OP_FIGHT): (%s [%i])",stringFromRoom(room->class), room->class);
				log_tag("debug_log.txt","[ERROR]",msg);
				exit(EXIT_FAILURE);
			}
			switch (room->class) {
				case ENEMIES: {
					enemy_index = enemy->index;
					sprintf(msg,"Setting enemy_index to (%i) (OP_FIGHT), isBoss == 0", enemy->index);
					log_tag("debug_log.txt","[TURNOP]",msg);
					isBoss = 0;
					res = OP_res_from_fightResult(fight(actor, enemy, notify_win, kls));
				}
				break;
				case BOSS: {
					enemy_index = 0;
					sprintf(msg,"Setting enemy_index to (0) (OP_FIGHT), isBoss == 1");
					log_tag("debug_log.txt","[TURNOP]",msg);
					isBoss = 1;
					res = OP_res_from_fightResult(boss_fight(actor, boss, path, notify_win, kls));
				}
				break;
				default: {
					sprintf(msg,"Invalid room value in turnOP(OP_FIGHT): [%s (%i)]",stringFromRoom(room->class), room->class);
					log_tag("debug_log.txt","[TURNOP]",msg);
					exit(EXIT_FAILURE);

				}
			}
		}
		break;
		case OP_SPECIAL: {
			if (room == NULL) {
				log_tag("debug_log.txt","[CRITICAL]","Room pointer was null in turnOP(OP_SPECIAL)");
				exit(EXIT_FAILURE);
			}
			room_index = room->index;
			if (room->class == ENEMIES && enemy == NULL) {
				log_tag("debug_log.txt","[ERROR]","Enemy pointer was null in turnOP(OP_SPECIAL) for ENEMIES room.");
				exit(EXIT_FAILURE);
			} else if (room->class == BOSS && boss == NULL){
				log_tag("debug_log.txt","[ERROR]","Boss pointer was null in turnOP(OP_SPECIAL) for BOSS room.");
				exit(EXIT_FAILURE);
			} else if (room->class != ENEMIES && room->class != BOSS) {
				sprintf(msg,"Invalid room class in turnOP(OP_SPECIAL): (%s [%i])",stringFromRoom(room->class), room->class);
				log_tag("debug_log.txt","[ERROR]",msg);
				exit(EXIT_FAILURE);
			}
			switch (room->class) {
				case ENEMIES: {
					enemy_index = enemy->index;
					sprintf(msg,"Setting enemy_index to (%i) (OP_SPECIAL), isBoss == 0", enemy->index);
					log_tag("debug_log.txt","[TURNOP]",msg);
					isBoss = 0;
				}
				break;
				case BOSS: {
					enemy_index = 0;
					sprintf(msg,"Setting enemy_index to (0) (OP_SPECIAL), isBoss == 1");
					log_tag("debug_log.txt","[TURNOP]",msg);
					isBoss = 1;
				}
				break;
				default: {
					sprintf(msg,"Invalid room value in turnOP(OP_SPECIAL): [%s (%i)]",stringFromRoom(room->class), room->class);
					log_tag("debug_log.txt","[TURNOP]",msg);
					exit(EXIT_FAILURE);

				}
			}
			handleSpecials(actor,enemy,boss,path,room_index,enemy_index,isBoss);
			res = OP_OK;
		}
		break;
		case OP_CONSUMABLE: {
			if (room == NULL) {
				log_tag("debug_log.txt","[CRITICAL]","Room pointer was null in turnOP(OP_CONSUMABLE)");
				exit(EXIT_FAILURE);
			}
			room_index = room->index;
			if (room->class == ENEMIES && enemy == NULL) {
				log_tag("debug_log.txt","[ERROR]","Enemy pointer was null in turnOP(OP_CONSUMABLE) for ENEMIES room.");
				exit(EXIT_FAILURE);
			} else if (room->class == BOSS && boss == NULL){
				log_tag("debug_log.txt","[ERROR]","Boss pointer was null in turnOP(OP_CONSUMABLE) for BOSS room.");
				exit(EXIT_FAILURE);
			} else if (room->class != ENEMIES && room->class != BOSS) {
				sprintf(msg,"Invalid room class in turnOP(OP_CONSUMABLE): (%s [%i])",stringFromRoom(room->class), room->class);
				log_tag("debug_log.txt","[ERROR]",msg);
				exit(EXIT_FAILURE);
			}
			switch (room->class) {
				case ENEMIES: {
					enemy_index = enemy->index;
					sprintf(msg,"Setting enemy_index to (%i) (OP_CONSUMABLE), isBoss == 0", enemy->index);
					log_tag("debug_log.txt","[TURNOP]",msg);
					isBoss = 0;
				}
				break;
				case BOSS: {
					enemy_index = 0;
					sprintf(msg,"Setting enemy_index to (0) (OP_CONSUMABLE), isBoss == 1");
					log_tag("debug_log.txt","[TURNOP]",msg);
					isBoss = 1;
				}
				break;
				default: {
					sprintf(msg,"Invalid room value in turnOP(OP_CONSUMABLE): [%s (%i)]",stringFromRoom(room->class), room->class);
					log_tag("debug_log.txt","[TURNOP]",msg);
					exit(EXIT_FAILURE);

				}
			}
			handleConsumables(actor,enemy,boss,isBoss);
			res = OP_OK;
		}
		break;
		case OP_SAVE: {
			if (GAMEMODE == Rogue) {
				log_tag("debug_log.txt","[WARN]","GAMEMODE was [Rogue] in turnOP(OP_SAVE)");
				res = NO_OP;
				break;
			}
			if (save_file == NULL) {
				log_tag("debug_log.txt","[CRITICAL]","save_file pointer was null in turnOP(OP_SAVE)");
				exit(EXIT_FAILURE);
			}
			if (load_info == NULL) {
				log_tag("debug_log.txt","[ERROR]","turnOP_args->(load_info) was NULL");
				//free(args);
				log_tag("debug_log.txt","[FREE]","Freed turnOP_args");
				return res;
			}
			if (room == NULL) {
				log_tag("debug_log.txt","[CRITICAL]","Room pointer was null in turnOP(OP_SAVE)");
				exit(EXIT_FAILURE);
			}
			room_index = room->index;
			if (room->class == ENEMIES && enemy == NULL) {
				log_tag("debug_log.txt","[ERROR]","Enemy pointer was null in turnOP(OP_SAVE) for ENEMIES room.");
				exit(EXIT_FAILURE);
			} else if (room->class == BOSS && boss == NULL){
				log_tag("debug_log.txt","[ERROR]","Boss pointer was null in turnOP(OP_SAVE) for BOSS room.");
				exit(EXIT_FAILURE);
			}

			if ((room->class != ENEMIES) && (room->class != HOME)) {
				sprintf(msg,"Invalid room class in turnOP(OP_SAVE): (%s [%i])",stringFromRoom(room->class), room->class);
				log_tag("debug_log.txt","[ERROR]",msg);
				exit(EXIT_FAILURE);
			}

			switch (room->class) {
				case ENEMIES: {
					enemy_index = enemy->index;
					sprintf(msg,"Setting enemy_index to (%i) (OP_SAVE), isBoss == 0", enemy->index);
					log_tag("debug_log.txt","[TURNOP]",msg);
					isBoss = 0;
					load_info->save_type = ENEMIES_SAVE;
					sprintf(msg,"Setting save_type to ENEMIES_SAVE. [%s]", stringFrom_saveType(load_info->save_type));
					log_tag("debug_log.txt","[TURNOP]",msg);
				}
				break;
				case BOSS: {
					enemy_index = 0;
					sprintf(msg,"Setting enemy_index to (0) (OP_SAVE), isBoss == 1");
					log_tag("debug_log.txt","[TURNOP]",msg);
					isBoss = 1;
				}
				break;
				case HOME: {
					enemy_index = -1;
					sprintf(msg,"Setting enemy_index to (-1) (OP_SAVE), isBoss == -1");
					log_tag("debug_log.txt","[TURNOP]",msg);
					isBoss = -1;
					load_info->save_type = HOME_SAVE;
					sprintf(msg,"Setting save_type to HOME_SAVE. [%s]", stringFrom_saveType(load_info->save_type));
					log_tag("debug_log.txt","[TURNOP]",msg);
				}
				break;
				default: {
					sprintf(msg,"Invalid room value in turnOP(OP_SAVE): [%s (%i)]",stringFromRoom(room->class), room->class);
					log_tag("debug_log.txt","[TURNOP]",msg);
					exit(EXIT_FAILURE);

				}
			}
			switch (load_info->save_type) {
				case ENEMIES_SAVE: {
					res = handleSave_Enemies(save_file,actor,path,enemy,enemy_index,room->enemyTotal,room_index);
				}
				break;
				case HOME_SAVE: {
					res = handleSave_Home(save_file,actor,path,room_index);
				}
				break;
				default: {
					sprintf(msg,"Invalid save_type value in turnOP(OP_SAVE): (%i)", (int) load_info->save_type);
					log_tag("debug_log.txt","[TURNOP]",msg);
					exit(EXIT_FAILURE);
				}
				break;
			}
		}
		break;
		case OP_DEBUG: {
			if (room == NULL) {
				log_tag("debug_log.txt","[WARN]","Room pointer was null in turnOP(OP_DEBUG)");
				exit(EXIT_FAILURE);
			}
			room_index = room->index;
			if (room->class == ENEMIES && enemy == NULL) {
				log_tag("debug_log.txt","[ERROR]","Enemy pointer was null in turnOP(OP_DEBUG) for ENEMIES room.");
				exit(EXIT_FAILURE);
			} else if (room->class == BOSS && boss == NULL){
				log_tag("debug_log.txt","[ERROR]","Boss pointer was null in turnOP(OP_DEBUG) for BOSS room.");
				exit(EXIT_FAILURE);
			}

			switch (room->class) {
				case HOME: {
					enemy_index = -1;
					sprintf(msg,"Setting enemy_index to (%i) (OP_DEBUG), isBoss == -1", enemy_index);
					log_tag("debug_log.txt","[TURNOP]",msg);
					isBoss = -1;
					log_tag("debug_log.txt","[TURNOP]","Doing endwin() before debug_generic()");
					endwin();
					debug_generic(actor,path,room_index,kls,t_kls);
					res = OP_OK;
				}
				break;
				case ENEMIES: {
					enemy_index = enemy->index;
					sprintf(msg,"Setting enemy_index to (%i) (OP_DEBUG), isBoss == 0", enemy->index);
					log_tag("debug_log.txt","[TURNOP]",msg);
					isBoss = 0;
 					debug_enemies_room(room,actor,enemy,path,room_index,enemy_index,kls,t_kls);
					res = OP_OK;
				}
				break;
				case BOSS: {
					enemy_index = 0;
					sprintf(msg,"Setting enemy_index to (0) (OP_DEBUG), isBoss == 1");
					log_tag("debug_log.txt","[TURNOP]",msg);
					isBoss = 1;
					log_tag("debug_log.txt","[TURNOP]","Doing endwin() before debug_generic()");
					endwin();
					debug_generic(actor,path,room_index,kls,t_kls);
					res = OP_OK;
				}
				break;
				default: {
					sprintf(msg,"Invalid room value in turnOP(OP_DEBUG): [%s (%i)]",stringFromRoom(room->class), room->class);
					log_tag("debug_log.txt","[TURNOP]",msg);
					exit(EXIT_FAILURE);
				}
			}
		}
		break;
		case OP_EQUIPS: {
			if (actor == NULL || path == NULL) {
				if (actor == NULL) log_tag("debug_log.txt","[ERROR]","turnOP_args->(actor) was NULL");
				if (path == NULL) log_tag("debug_log.txt","[ERROR]","turnOP_args->(path) was NULL");
				//free(args);
				log_tag("debug_log.txt","[FREE]","Freed turnOP_args");
				return res;
			}
			handleEquips(actor,path);
			res = OP_OK;
		};
		break;
		case OP_PERKS: {
			if (actor == NULL) {
				log_tag("debug_log.txt","[ERROR]","turnOP_args->(actor) was NULL");
				//free(args);
				log_tag("debug_log.txt","[FREE]","Freed turnOP_args");
				return res;
			}
			printActivePerks(actor);
			res = OP_OK;
		}
		break;
		case OP_STATS: {
			if (actor == NULL) {
				log_tag("debug_log.txt","[ERROR]","turnOP_args->(actor) was NULL");
				//free(args);
				log_tag("debug_log.txt","[FREE]","Freed turnOP_args");
				return res;
			}
			handleStats(actor);
			res = OP_OK;
		}
		break;
		case OP_ARTIFACTS: {
			if (actor == NULL) {
				log_tag("debug_log.txt","[ERROR]","turnOP_args->(actor) was NULL");
				//free(args);
				log_tag("debug_log.txt","[FREE]","Freed turnOP_args");
				return res;
			}
			handleArtifacts(actor);
			res = OP_OK;
		}
		break;
		case OP_EXPLORE: {
			log_tag("debug_log.txt","[DEBUG]","Going to explore!");
			//The free of args should be done after the OP; in handleRoom_Home()
			res = NO_OP;
		}
		break;
		case OP_QUIT: {
			quit(actor,room,load_info,t_kls);
			//FIXME
			//We can't free the turnOP_args, can we?
			//free(args);
			log_tag("debug_log.txt","[FREE]","Freed turnOP_args");
		}
		break;
		default: {
			log_tag(OPS_LOGFILE,"[ERROR]","Invalid OP in turnOP()");
			log_tag("debug_log.txt","[ERROR]","Invalid OP in turnOP()");
		}
		break;
	}

	//Log end of operation
	sprintf(msg,"Done operation: [%s] res: [%s (%i)]",stringFromTurnOP(op), stringFrom_OP_res(res), res);
	log_tag("debug_log.txt","[TURNOP]",msg);

	sprintf(msg,"res: [%s (%i)]", stringFrom_OP_res(res),res);
	log_tag(OPS_LOGFILE,"[RES]",msg);

	return res;
}

/**
 * Takes an integer, a callback_void_t pointer function and a Fighter holding the array for the callback registration.
 * Not working as of v0.5.2.
 * Registers the pointer to the function pointer array for counter callback.
 * @see Turncounter
 * @param index An integer.
 * @param ptr A pointer to function of type callback_void_t.
 * @param f The fighter pointer holding the callback array.
 */
void register_counter_callback(int index, callback_void_t ptr, Fighter* f) {
	f->callback_counter_ptrs[index] = ptr;
}

//Status effect functions

/**
 * Takes a Fighter pointer and resets its status value to Normal.
 * @see Fighter
 * @see fighterStatus
 * @param f The pointer whose status value will be reset.
 */
void resetFighterStatus(Fighter* f) {
	f->status = Normal;
}

/**
 * Takes a Enemy pointer and resets its status value to Normal.
 * @see Enemy
 * @see fighterStatus
 * @param e The pointer whose status value will be reset.
 */
void resetEnemyStatus(Enemy* e) {
	e->status = Normal;
}

/**
 * Takes a Boss pointer and resets its status value to Normal.
 * @see Boss
 * @see fighterStatus
 * @param b The pointer whose status value will be reset.
 */
void resetBossStatus(Boss* b) {
	b->status = Normal;
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
}

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
}

/**
 * Takes a fighterStatus and returns the corresponding effect_e_fun pointer for the function relating to the status.
 * @see fighterStatus
 * @see effect_e_fun
 * @param status The fighterStatus that the caller needs to match to a effect_e_fun pointer.
 * @return The effect_e_fun pointer related to the status.
 */
effect_b_fun getStatusCounterBossFun(fighterStatus status) {
	switch (status) {
		//case Normal: {
		default: {
			return &resetBossStatus;
		}
		break;
	};
}

//Stat boost functions

/**
 * Takes a Fighter pointer and an integer, used to set the turnboost_atk value at the pointer.
 * @see Fighter
 * @see Turncounter
 * @param f The Fighter pointer whose turnboost_atk value will be set.
 * @param boost The value that will be set.
 */
void set_turnboost_atk(Fighter* f,int boost) {
	//Straight addition of the boost
	f->turnboost_atk = boost;
}

/**
 * Takes a Fighter pointer and an integer, used to set the turnboost_def value at the pointer.
 * @see Fighter
 * @see Turncounter
 * @param f The Fighter pointer whose turnboost_def value will be set.
 * @param boost The value that will be set.
 */
void set_turnboost_def(Fighter* f,int boost) {
	//Straight addition of the boost
	f->turnboost_def = boost;
}

/**
 * Takes a Fighter pointer and an integer, used to set the turnboost_vel value at the pointer.
 * @see Fighter
 * @see Turncounter
 * @param f The Fighter pointer whose turnboost_vel value will be set.
 * @param boost The value that will be set.
 */
void set_turnboost_vel(Fighter* f,int boost) {
	//Straight addition of the boost
	f->turnboost_vel = boost;
}

/**
 * Takes a Fighter pointer and an integer, used to set the turnboost_enr value at the pointer.
 * @see Fighter
 * @see Turncounter
 * @param f The Fighter pointer whose turnboost_enr value will be set.
 * @param boost The value that will be set.
 */
void set_turnboost_enr(Fighter* f,int boost) {
	//Straight addition of the boost
	f->turnboost_enr = boost;
}

/**
 * Takes a Enemy pointer and an integer, used to set the turnboost_atk value at the pointer.
 * @see Enemy
 * @see Turncounter
 * @param e The Enemy pointer whose turnboost_atk value will be set.
 * @param boost The value that will be set.
 */
void set_enemy_turnboost_atk(Enemy* e,int boost) {
	//Straight addition of the boost
	e->turnboost_atk = boost;
}

/**
 * Takes a Enemy pointer and an integer, used to set the turnboost_def value at the pointer.
 * @see Enemy
 * @see Turncounter
 * @param e The Enemy pointer whose turnboost_def value will be set.
 * @param boost The value that will be set.
 */
void set_enemy_turnboost_def(Enemy* e,int boost) {
	//Straight addition of the boost
	e->turnboost_def = boost;
}

/**
 * Takes a Enemy pointer and an integer, used to set the turnboost_vel value at the pointer.
 * @see Enemy
 * @see Turncounter
 * @param e The Enemy pointer whose turnboost_vel value will be set.
 * @param boost The value that will be set.
 */
void set_enemy_turnboost_vel(Enemy* e,int boost) {
	//Straight addition of the boost
	e->turnboost_vel = boost;
}

/**
 * Takes a Enemy pointer and an integer, used to set the turnboost_enr value at the pointer.
 * @see Enemy
 * @see Turncounter
 * @param e The Enemy pointer whose turnboost_enr value will be set.
 * @param boost The value that will be set.
 */
void set_enemy_turnboost_enr(Enemy* e,int boost) {
	//Straight addition of the boost
	e->turnboost_enr = boost;
}

/**
 * Takes a Boss pointer and an integer, used to set the turnboost_atk value at the pointer.
 * @see Boss
 * @see Turncounter
 * @param b The Boss pointer whose turnboost_atk value will be set.
 * @param boost The value that will be set.
 */
void set_boss_turnboost_atk(Boss* b,int boost) {
	//Straight addition of the boost
	b->turnboost_atk = boost;
}

/**
 * Takes a Boss pointer and an integer, used to set the turnboost_def value at the pointer.
 * @see Boss
 * @see Turncounter
 * @param b The Boss pointer whose turnboost_def value will be set.
 * @param boost The value that will be set.
 */
void set_boss_turnboost_def(Boss* b,int boost) {
	//Straight addition of the boost
	b->turnboost_def = boost;
}

/**
 * Takes a Boss pointer and an integer, used to set the turnboost_vel value at the pointer.
 * @see Boss
 * @see Turncounter
 * @param b The Boss pointer whose turnboost_vel value will be set.
 * @param boost The value that will be set.
 */
void set_boss_turnboost_vel(Boss* b,int boost) {
	//Straight addition of the boost
	b->turnboost_vel = boost;
}

/**
 * Takes a Boss pointer and an integer, used to set the turnboost_enr value at the pointer.
 * @see Boss
 * @see Turncounter
 * @param b The Boss pointer whose turnboost_enr value will be set.
 * @param boost The value that will be set.
 */
void set_boss_turnboost_enr(Boss* b,int boost) {
	//Straight addition of the boost
	b->turnboost_enr = boost;
}

/**
 * Takes a FoeParty pointer and an integer, used to set the turnboost_atk value at the pointer.
 * @see FoeParty
 * @see Turncounter
 * @param fp The FoeParty pointer whose turnboost_atk value will be set.
 * @param boost The value that will be set.
 */
void set_foeparty_turnboost_atk(FoeParty* fp,int boost) {
	//Straight addition of the boost
	fp->turnboost_atk = boost;
}

/**
 * Takes a FoeParty pointer and an integer, used to set the turnboost_def value at the pointer.
 * @see FoeParty
 * @see Turncounter
 * @param fp The FoeParty pointer whose turnboost_def value will be set.
 * @param boost The value that will be set.
 */
void set_foeparty_turnboost_def(FoeParty* fp,int boost) {
	//Straight addition of the boost
	fp->turnboost_def = boost;
}

/**
 * Takes a FoeParty pointer and an integer, used to set the turnboost_vel value at the pointer.
 * @see FoeParty
 * @see Turncounter
 * @param fp The FoeParty pointer whose turnboost_vel value will be set.
 * @param boost The value that will be set.
 */
void set_foeparty_turnboost_vel(FoeParty* fp,int boost) {
	//Straight addition of the boost
	fp->turnboost_vel = boost;
}

/**
 * Takes a FoeParty pointer and an integer, used to set the turnboost_enr value at the pointer.
 * @see FoeParty
 * @see Turncounter
 * @param fp The FoeParty pointer whose turnboost_enr value will be set.
 * @param boost The value that will be set.
 */
void set_foeparty_turnboost_enr(FoeParty* fp,int boost) {
	//Straight addition of the boost
	fp->turnboost_atk = boost;
}


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
		default: {
			log_tag("debug_log.txt","[ERROR]","Invalid stat value in getStatBoostCounterFun()");
			exit(EXIT_FAILURE);
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
		default: {
			log_tag("debug_log.txt","[ERROR]","Invalid stat value in getStatBoostCounterEnemyFun()");
			exit(EXIT_FAILURE);
		}
		break;
	};
}

/**
 * Takes a Stat and returns the corresponding boost_b_fun pointer for the function relating to the stat.
 * @see Stat
 * @see boost_b_fun
 * @param s The Stat that the caller needs to match to a boost_b_fun pointer.
 * @return The boost_b_fun pointer related to the stat.
 */
boost_b_fun getStatBoostCounterBossFun(Stat s) {

	switch (s) {
		case ATK: {
			return &set_boss_turnboost_atk;
		}
		break;
		case DEF: {
			return &set_boss_turnboost_def;
		}
		break;
		case VEL: {
			return &set_boss_turnboost_vel;
		}
		break;
		case ENR: {
			return &set_boss_turnboost_enr;
		}
		break;
		default: {
			log_tag("debug_log.txt","[ERROR]","Invalid stat value in getStatBoostCounterBossFun()");
			exit(EXIT_FAILURE);
		}
		break;
	};
}

/**
 * Takes a Stat and returns the corresponding boost_fp_fun pointer for the function relating to the stat.
 * @see Stat
 * @see boost_fp_fun
 * @param s The Stat that the caller needs to match to a boost_fp_fun pointer.
 * @return The boost_fp_fun pointer related to the stat.
 */
boost_fp_fun get_StatBoostCounter_FoeParty_Fun(Stat s) {

	switch (s) {
		case ATK: {
			return &set_foeparty_turnboost_atk;
		}
		break;
		case DEF: {
			return &set_foeparty_turnboost_def;
		}
		break;
		case VEL: {
			return &set_foeparty_turnboost_vel;
		}
		break;
		case ENR: {
			return &set_foeparty_turnboost_enr;
		}
		break;
		default: {
			log_tag("debug_log.txt","[ERROR]","Invalid stat value in getStatBoostCounter_FoeParty_Fun()");
			exit(EXIT_FAILURE);
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
 * @param kls The Koliseo used for allocation.
 */
void initPerks(Fighter* f, Koliseo* kls) {
	char msg[200];
	f->perksCount = 0;
	//Ordering of i corresponds to perksClass enum
	int total = (PERKSMAX+1);
	for (int i = 0; i < total; i++) {
		sprintf(msg,"Prepping Perk (%i)",i);
		kls_log("DEBUG",msg);
  		Perk* p = (Perk*) KLS_PUSH_NAMED(kls, Perk, 1,"Perk",msg);
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
 */
void applyEquipPerks(Equip* e, Fighter* f) {

	for (int i = 0; i < (e->perksCount); i++) {
		Perk* p = e->perks[i];

		Perk* fighterPerk = f->perks[p->class];
		fighterPerk->innerValue += 1;
	}
}

/**
 * Takes a Equip and a Fighter pointers.
 * Iterates over the equip's perks and removes them to the fighter perks.
 * @see Perk
 * @see Equip
 * @see Fighter
 * @param e An Equip pointer.
 * @param f A Fighter pointer.
 */
void removeEquipPerks(Equip* e, Fighter* f) {

	for (int i = 0; i < (e->perksCount); i++) {
		Perk* p = e->perks[i];

		Perk* fighterPerk = f->perks[p->class];
		fighterPerk->innerValue -= 1;
	}
}

/**
 * Takes a Figher pointer and prints all of its active perks formatted.
 * @see Fighter
 * @see Perk
 * @see Equip
 * @param f The Fighter pointer with perks to print.
 */
void printActivePerks(Fighter* f) {

	WINDOW *win;

	/* Initialize curses */
	//setlocale(LC_CTYPE, "it_IT.UTF-8");
	//initscr();
	clear();
	refresh();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	/* Create the windows for player stats and lifetime counters */
        win = newwin(22, 65, 1, 2);
        keypad(win, TRUE);

	/* Print a border around the windows and print a title */
        box(win, 0, 0);
	print_label(win, 1, 0, 35, "Perks", COLOR_PAIR(6));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 63);
	mvwaddch(win, 2, 64, ACS_RTEE);

	wrefresh(win);

	//attron(COLOR_PAIR(3));
	mvprintw(23, 0, "(Press q or Enter to Exit)");
	//attroff(COLOR_PAIR(3));


	int y = 4;
	int x = 2;
	//int startx = 2;
	//int width = 65;

	int empty = 1;


	int count = PERKSMAX + 1;

	for (int i = 0; i < count; i++) {
		Perk* p = f->perks[i];
		if (p->innerValue > 0) {
			empty = 0;

			wattron(win,COLOR_PAIR(4));
			mvwprintw(win, y, x, " x%i %s ", p->innerValue, nameStringFromPerk(p->class));
			wattroff(win,COLOR_PAIR(4));
			char s[250];
			sprintf(s," x%i %s", p->innerValue, nameStringFromPerk(p->class));
			int l = strlen(s);
			wattron(win,COLOR_PAIR(6));
			mvwprintw(win, y, x + l + 2, "\"%s\"",  descStringFromPerk(p->class));
			wattroff(win,COLOR_PAIR(6));
			y++;
		}
	};

	if (empty) {	//No perks are active
		wattron(win,COLOR_PAIR(6));
		mvwprintw(win, y, x , "You don't have any special power yet.");
		wattroff(win,COLOR_PAIR(6));
	}

	refresh();

	int picked = 0;
	int c = -1;
	wrefresh(win);

	while(!picked && (c = wgetch(win)) != 'q') {
		switch(c) {
			case 10: { /*Enter*/
				picked = 1;

				};
				break;
		        }
	}
	delwin(win);
	endwin();
}

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
 * @param kls The Koliseo used for allocation.
 */
void initCounters(Fighter* f, Koliseo* kls){
	//Ordering of i corresponds to counterIndexes enum
	int total = (COUNTERSMAX+1);
	char msg[500];
	for (int i = 0; i < total; i++) {
		sprintf(msg,"Prepping Turncounter (%i)",i);
		log_tag("debug_log.txt","[DEBUG]",msg);
		kls_log("DEBUG",msg);
  		Turncounter* c = (Turncounter*) KLS_PUSH_NAMED(kls, Turncounter, 1, "Turncounter", msg);

		//First, prepare counters for statuses
		if (i < STATUSMAX+1 ) {
			sprintf(msg,"Allocated size %lu for status counter: (%s)", sizeof(stringFromStatus(i)), c->desc);
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			c->desc = (char*) KLS_PUSH_NAMED(kls, char*, sizeof(stringFromStatus(i)),"Turncounter desc",msg);
			strcpy(c->desc,stringFromStatus(i));

			c->effect_fun = getStatusCounterFun(i);
			//sprintf(msg,"Status function pointer is: (%i)", (int) *(c->effect_fun));
			//log_tag("debug_log.txt","[DEBUG]",msg);
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
					sprintf(msg,"Allocated size %lu for status counter: (%s)", sizeof("ATK boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_NAMED(kls,char*,sizeof("ATK boost"),"Turncounter desc",msg);
					strcpy(c->desc,"ATK boost");

					c->boost_fun = getStatBoostCounterFun(ATK);
					c->type = CNT_ATKBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				case TURNBOOST_DEF: {
					sprintf(msg,"Allocated size %lu for status counter: (%s)", sizeof("DEF boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_NAMED(kls,char*,sizeof("DEF boost"),"Turncounter desc",msg);
					strcpy(c->desc,"DEF boost");

					c->boost_fun = getStatBoostCounterFun(DEF);
					c->type = CNT_DEFBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				case TURNBOOST_VEL: {
					sprintf(msg,"Allocated size %lu for status counter: (%s)", sizeof("VEL boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_NAMED(kls,char*,sizeof("VEL boost"),"Turncounter desc",msg);
					strcpy(c->desc,"VEL boost");

					c->boost_fun = getStatBoostCounterFun(VEL);
					c->type = CNT_VELBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				case TURNBOOST_ENR: {
					sprintf(msg,"Allocated size %lu for status counter: (%s)", sizeof("ENR boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_NAMED(kls,char*,sizeof("ENR boost"),"Turncounter desc",msg);
					strcpy(c->desc,"ENR boost");

					c->boost_fun = getStatBoostCounterFun(ENR);
					c->type = CNT_ENRBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				default: {
					printf("\n\nERROR in initCounters(): unexpected i: %i value \n\n", i);
					exit(EXIT_FAILURE);
				}
			}
		} //End else
		c->count = 0;
		c->innerValue = 0;
		f->counters[i] = (struct Turncounter*) c;
	};//End for all counters
}

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
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initECounters(Enemy* e, Koliseo_Temp* t_kls){
	Koliseo_Temp tkls = *t_kls;
	//Ordering of i corresponds to counterIndexes enum
	int total = (COUNTERSMAX+1);
	char msg[500];
	for (int i = 0; i < total; i++) {
		sprintf(msg,"Prepping enemy Turncounter (%i)",i);
		log_tag("debug_log.txt","[DEBUG]",msg);
		kls_log("DEBUG",msg);
		Turncounter* c = (Turncounter*) KLS_PUSH_T_NAMED(tkls, Turncounter, 1, "Turncounter",msg);

		//First, prepare counters for statuses
		if (i < STATUSMAX+1 ) {
			sprintf(msg,"Allocated size %lu for enemy status counter: (%s)", sizeof(stringFromStatus(i)), c->desc);
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof(stringFromStatus(i)),"Turncounter desc",msg);
			strcpy(c->desc,stringFromStatus(i));

			c->effect_e_fun = getStatusCounterEnemyFun(i);
			//sprintf(msg,"[DEBUG]    Enemy status function pointer is: (%i)", *(c->effect_e_fun));
			//log_tag("debug_log.txt","[DEBUG]",msg);
			c->type = CNT_STATUS;

		} else { //Then, counters for boosts to (all?) stats

			switch(i) {
				case TURNBOOST_ATK: {
					sprintf(msg,"Allocated size %lu for status counter: (%s)", sizeof("ATK boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof("ATK boost"),"Turncounter desc",msg);
					strcpy(c->desc,"ATK boost");

					c->boost_e_fun = getStatBoostCounterEnemyFun(ATK);
					c->type = CNT_ATKBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				case TURNBOOST_DEF: {
					sprintf(msg,"Allocated size %lu for status counter: (%s)", sizeof("DEF boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof("DEF boost"),"Turncounter desc",msg);
					strcpy(c->desc,"DEF boost");

					c->boost_e_fun = getStatBoostCounterEnemyFun(DEF);
					c->type = CNT_DEFBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				case TURNBOOST_VEL: {
					sprintf(msg,"Allocated size %lu for status counter: (%s)", sizeof("VEL boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof("VEL boost"),"Turncounter desc",msg);
					strcpy(c->desc,"VEL boost");

					c->boost_e_fun = getStatBoostCounterEnemyFun(VEL);
					c->type = CNT_VELBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				case TURNBOOST_ENR: {
					sprintf(msg,"Allocated size %lu for status counter: (%s)", sizeof("ENR boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof("ENR boost"),"Turncounter desc",msg);
					strcpy(c->desc,"ENR boost");

					c->boost_e_fun = getStatBoostCounterEnemyFun(ENR);
					c->type = CNT_ENRBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				default: {
					log_tag("debug_log.txt","[ERROR]","ERROR INITALISING TURNCOUNTER in initECounters()\n");
					exit(EXIT_FAILURE);
				}
			}
		} //End else
		c->count = 0;
		c->innerValue = 0;
		e->counters[i] = (struct Turncounter*) c;
	};
}

/**
 * Takes a Boss pointer and prepares its counters field by allocating all the Turncounter accounted in counterIndexes.
 * Counters are initialised using the ordering of counterIndexes, primarily for distinguinsing what kind of function pointer will be registered in the counter.
 * For a given Counter, only the correct pointer field is assigned and the others are left untouched.
 * @see Boss
 * @see Turncounter
 * @see Countertype
 * @see COUNTERSMAX
 * @see counterIndexes
 * @see getStatBoostCounterBossFun()
 * @see getStatusCounterBossFun()
 * @param b The Boss pointer whose counters field will be initialised.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initBCounters(Boss* b, Koliseo_Temp* t_kls){
	Koliseo_Temp tkls = *t_kls;
	//Ordering of i corresponds to counterIndexes enum
	int total = (COUNTERSMAX+1);
	char msg[500];
	for (int i = 0; i < total; i++) {
		sprintf(msg,"Prepping boss counter %i",i);
		log_tag("debug_log.txt","[DEBUG]",msg);
		kls_log("DEBUG",msg);
		Turncounter* c = (Turncounter*) KLS_PUSH_T_NAMED(tkls, Turncounter, 1, "Turncounter",msg);

		//First, prepare counters for statuses
		if (i < STATUSMAX+1 ) {
			sprintf(msg,"Allocated size %lu for boss status counter: (%s)", sizeof(stringFromStatus(i)), c->desc);
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof(stringFromStatus(i)),"Turncounter desc",msg);
			strcpy(c->desc,stringFromStatus(i));

			c->effect_b_fun = getStatusCounterBossFun(i);
			//sprintf(msg,"Boss status function pointer is: (%i)", *(c->effect_b_fun));
			//log_tag("debug_log.txt","[DEBUG]",msg);
			c->type = CNT_STATUS;

		} else { //Then, counters for boosts to (all?) stats

			switch(i) {
				case TURNBOOST_ATK: {
					sprintf(msg,"Allocated size %lu for boss stat counter: (%s)", sizeof("ATK boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof("ATK boost"),"Turncounter desc",msg);
					strcpy(c->desc,"ATK boost");

					c->boost_b_fun = getStatBoostCounterBossFun(ATK);
					c->type = CNT_ATKBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_b_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				case TURNBOOST_DEF: {
					sprintf(msg,"Allocated size %lu for boss stat counter: (%s)", sizeof("DEF boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof("DEF boost"),"Turncounter desc",msg);
					strcpy(c->desc,"DEF boost");

					c->boost_b_fun = getStatBoostCounterBossFun(DEF);
					c->type = CNT_DEFBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_b_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				case TURNBOOST_VEL: {
					sprintf(msg,"Allocated size %lu for boss stat counter: (%s)", sizeof("VEL boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof("VEL boost"),"Turncounter desc",msg);
					strcpy(c->desc,"VEL boost");

					c->boost_b_fun = getStatBoostCounterBossFun(VEL);
					c->type = CNT_VELBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_b_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				case TURNBOOST_ENR: {
					sprintf(msg,"Allocated size %lu for boss stat counter: (%s)", sizeof("ENR boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof("ENR boost"),"Turncounter desc",msg);
					strcpy(c->desc,"ENR boost");

					c->boost_b_fun = getStatBoostCounterBossFun(ENR);
					c->type = CNT_ENRBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_b_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				default: {
					log_tag("debug_log.txt","[ERROR]","ERROR INITALISING TURNCOUNTER in initBCounters()\n");
					exit(EXIT_FAILURE);
				}
			}
		} //End else
		c->count = 0;
		c->innerValue = 0;
		b->counters[i] = (struct Turncounter*) c;
	};
}

/**
 * Takes a FoeParty pointer and prepares its counters field by allocating all the Turncounter accounted in counterIndexes.
 * Counters are initialised using the ordering of counterIndexes, primarily for distinguinsing what kind of function pointer will be registered in the counter.
 * For a given Counter, only the correct pointer field is assigned and the others are left untouched.
 * @see FoeParty
 * @see Turncounter
 * @see Countertype
 * @see COUNTERSMAX
 * @see counterIndexes
 * @see get_StatBoostCounter_FoeParty_Fun()
 * @see get_StatusCounter_FoeParty_Fun()
 * @param fp The FoeParty pointer whose counters field will be initialised.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initFoePartyCounters(FoeParty* fp, Koliseo_Temp* t_kls){
	Koliseo_Temp tkls = *t_kls;
	//Ordering of i corresponds to counterIndexes enum
	int total = (COUNTERSMAX+1);
	char msg[500];
	for (int i = 0; i < total; i++) {
		sprintf(msg,"Prepping foeparty counter %i",i);
		log_tag("debug_log.txt","[DEBUG]",msg);
		kls_log("DEBUG",msg);
		Turncounter* c = (Turncounter*) KLS_PUSH_T_NAMED(tkls, Turncounter, 1, "Turncounter",msg);

		//First, prepare counters for statuses
		if (i < STATUSMAX+1 ) {
			sprintf(msg,"Allocated size %lu for foeparty status counter: (%s)", sizeof(stringFromStatus(i)), c->desc);
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof(stringFromStatus(i)),"Turncounter desc",msg);
			strcpy(c->desc,stringFromStatus(i));

			//c->effect_fp_fun = get_StatusCounter_FoeParty_Fun(i);
			//sprintf(msg,"Foeparty status function pointer is: (%i)", *(c->effect_b_fun));
			//log_tag("debug_log.txt","[DEBUG]",msg);
			c->type = CNT_STATUS;

		} else { //Then, counters for boosts to (all?) stats

			switch(i) {
				case TURNBOOST_ATK: {
					sprintf(msg,"Allocated size %lu for status counter: (%s)", sizeof("ATK boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof("ATK boost"),"Turncounter desc",msg);
					strcpy(c->desc,"ATK boost");

					c->boost_fp_fun = get_StatBoostCounter_FoeParty_Fun(ATK);
					c->type = CNT_ATKBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fp_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				case TURNBOOST_DEF: {
					sprintf(msg,"Allocated size %lu for status counter: (%s)", sizeof("DEF boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof("DEF boost"),"Turncounter desc",msg);
					strcpy(c->desc,"DEF boost");

					c->boost_fp_fun = get_StatBoostCounter_FoeParty_Fun(DEF);
					c->type = CNT_DEFBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fp_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				case TURNBOOST_VEL: {
					sprintf(msg,"Allocated size %lu for status counter: (%s)", sizeof("VEL boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof("VEL boost"),"Turncounter desc",msg);
					strcpy(c->desc,"VEL boost");

					c->boost_fp_fun = get_StatBoostCounter_FoeParty_Fun(VEL);
					c->type = CNT_VELBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fp_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				case TURNBOOST_ENR: {
					sprintf(msg,"Allocated size %lu for status counter: (%s)", sizeof("ENR boost"), c->desc);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					c->desc = (char*) KLS_PUSH_T_NAMED(tkls, char*, sizeof("ENR boost"),"Turncounter desc",msg);
					strcpy(c->desc,"ENR boost");

					c->boost_fp_fun = get_StatBoostCounter_FoeParty_Fun(ENR);
					c->type = CNT_ENRBOOST;
					//sprintf(msg,"Stat boost function pointer is: (%i)", *(c->boost_fp_fun));
					//log_tag("debug_log.txt","[DEBUG]",msg);
				}
				break;
				default: {
					log_tag("debug_log.txt","[ERROR]","Invalid counter in initFoePartyCounters");
					exit(EXIT_FAILURE);
				}
			}
		} //End else
		c->count = 0;
		c->innerValue = 0;
		fp->counters[i] = (struct Turncounter*) c;
	};
}

/**
 * Takes a Turncounter array.
 * For every Turncounter in the array, the values of count, innerVal, type and all the function pointers fields are printed.
 * @see Turncounter
 * @param counters The Turncounter array to be printed.
 */
void printCounters(Turncounter* counters[]) {
		yellow();
		printf("%-10.10s\t%-10.10s\t%-3.3s\t%-3.3s\t%-11.11s\t%-11.11s\t%-11.11s\t%-11.11s\n","Count","Desc","Val","Typ","*(eff())","*(eff_e())","*(boost())","*(boost_e())");
	for (int i = 0; i < (COUNTERSMAX + 1) ; i++) {
		Turncounter *c = counters[i];
		lightBlue();
		printf("%-10.10i\t%-10.10s\t",c->count,c->desc);
		strongWhite();
		printf("(%-3.3i)\t(%-3.3i)\t",c->innerValue,c->type);
		purple();
		//printf("[%-11.11i]\t[%-11.11i]\t",*(c->effect_fun),*(c->effect_e_fun));
		cyan();
		//printf("[%-11.11i]\t[%-11.11i]\n",*(c->boost_fun), *(c->boost_e_fun));
	};
		white();
}

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
 */
void updateCounters(Turncounter* counters[], int isEnemy, Fighter* f, Enemy* e){
	char msg[200];
	for (int i=0; i < COUNTERSMAX + 1; i++) {

		Turncounter* c = counters[i];
		if (c->count == 1) { //Counter is about to expire so we call the correct function:
			switch (c->type) {
				case CNT_STATUS: { //Callback for status counters

					if (!isEnemy) {
						(c->effect_fun)(f);
						//green();
						//printf("\t%s status returned to normal.\n",f->name);
						//white();
					} else { //Enemy function
						(c->effect_e_fun)(e);
						//lightRed();
						//printf("\t%s status returned to normal.\n",stringFromEClass(e->class));
						//white();
						//TODO
						//Display notification to win
						sprintf(msg,"Status reset for %s.\n", stringFromEClass(e->class));
						log_tag("debug_log.txt","[DEBUG-COUNTER]",msg);
					}
				}
				break;
				case CNT_ATKBOOST: case CNT_DEFBOOST: case CNT_VELBOOST: case CNT_ENRBOOST: { //Callback for stat boosts
					if (!isEnemy) {
						(c->boost_fun)(f,0); //Invoke ~setPermBoost(STAT)~ with value 0
						sprintf(msg,"Applied boost function for Fighter\n");
						log_tag("debug_log.txt","[DEBUG-COUNTER]",msg);
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
			sprintf(msg,"Advancing counter %i for %s.\n", i, (isEnemy ? "Enemy" : "Fighter"));
			log_tag("debug_log.txt","[DEBUG-COUNTER]",msg);
			c->count -= 1;
		}
	};
}

/**
 * Takes a Turncounter array, an integer, a Fighter pointer and a Boss pointer.
 * For every Turncounter in the array count value is checked, and when it's equal to 1, the function pointer relevant to the type value of the Counter is called. Depending on the isBoss input value, the function call will be for the Fighter or Boss version (and the according pointer from the input will be passed to the called function).
 * When the count value is 0 Counters are considered inactive, so when the count value is not 1, it is lowered by 1 if it's positive.
 * @see Fighter
 * @see Boss
 * @see Turncounter
 * @see Countertype
 * @see COUNTERSMAX
 * @see counterIndexes
 * @param counters The Turncounter array to be updated.
 * @param isBoss Dictates if the check is done for a Boss or a Fighter.
 * @param f The Fighter pointer whose counters field will be updated if isBoss is false.
 * @param b The Boss pointer whose counters field will be updated if isBoss is true.
 */
void updateCounters_Boss(Turncounter* counters[], int isBoss, Fighter* f, Boss* b){
	for (int i=0; i < COUNTERSMAX + 1; i++) {

		Turncounter* c = counters[i];
		if (c->count == 1) { //Counter is about to expire so we call the correct function:
			switch (c->type) {
				case CNT_STATUS: { //Callback for status counters

					//TODO
					//Add notification to window
					if (!isBoss) {
						(c->effect_fun)(f);
						//green();
						//printf("\t%s status returned to normal.\n",f->name);
						//white();
					} else { //Boss function
						(c->effect_b_fun)(b);
						//lightRed();
						//printf("\t%s status returned to normal.\n",stringFromBossClass(b->class));
						//white();
					}
				}
				break;
				case CNT_ATKBOOST: case CNT_DEFBOOST: case CNT_VELBOOST: case CNT_ENRBOOST: { //Callback for stat boosts
					if (!isBoss) {
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
}

/**
 * Takes a Turncounter pointer and an integer.
 * If the count value at the pointer is 0 (counter is inactive), the turns valueis assigned.
 * @see Turncounter
 * @param c The Turncounter whose count value will be set.
 * @param turns The value to be assigned.
 */
void setCounter(Turncounter* c,int turns) {

	if (c->count == 0) { // Counter is NOT already active
		c->count = turns;
	} else {
		//Handle counters already activ
	}
}

/**
 * Takes a Fighter pointer and prepares its specialSlot fields by allocating SPECIALSMAX slots.
 * Special slots are initialised.
 * The specific move assigned is determined using the ordering of both fighterClass values and SPECIALSMAX.
 * @see Fighter
 * @see Specialslot
 * @see SPECIALSMAX
 * @see REGISTER_CALLBACK()
 * @see costFromSpecial()
 * @see stringFromSpecial()
 * @param kls The Koliseo used for allocations.
 * @param f The Fighter pointer whose special slots will be initialised.
 */
void setSpecials(Fighter* f, Koliseo* kls){
	char movename[80];
	char movedesc[80];
	char msg[200];
	for (int i = 0; i <= SPECIALSMAX ; i++) {
		sprintf(msg,"Prepping Specialslot (%i)",i);
		kls_log("DEBUG",msg);
		Specialslot* s = (Specialslot*) KLS_PUSH_NAMED(kls,Specialslot,1,"Specialslot",msg);
		s->enabled = 0;
		s->move = i + ( f->class  * (SPECIALSMAX + 1) ) ; // Assign the i-th move offsetting by classNum * specialsMax
		s->cost = costFromSpecial(f->class,i);
		strcpy(movename,nameStringFromSpecial(f->class,i));
		strcpy(movedesc,descStringFromSpecial(f->class,i));
		//printf("DEBUG\n%i\t%s\n",(i+ (f->class * (SPECIALSMAX + 1))),stringFromSpecial(f->class,i));
		strcpy(s->name, movename);
		strcpy(s->desc, movedesc);
		f->specials[i] = s;
	};
}

/**
 * Takes a Fighter pointer and resets all of its permboost_STAT values to 0, also correctly updating the current stat values.
 * @see Fighter
 * @param f The fighter pointer whose permboosts will be reset.
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
}

/**
 * Takes a Fighter pointer and applies all of its permboost_STAT values by adding them to the current stat values.
 * @see Fighter
 * @param f The fighter pointer whose permboosts will be applied.
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
}

/**
 * Takes a Fighter pointer and Resets the active value for each Artifact in the fighter's artifactsBag array that doesn't have an always active trait.
 * At the moment, this only excludes CHAOSORB.
 * @see Artifact
 * @see artifactClass
 * @see Fighter
 */
void resetArtifactsState(Fighter* f) {
	for (int i=0; i < (ARTIFACTSMAX + 1) ; i++) {

		//if (i == CHAOSORB) { //Chaosorb never gets reset
		//	continue;
		//};

		if (f->artifactsBag[i]->qty != 0 ) { //We only reset the ones we have
			f->artifactsBag[i]->active = 0;
		};
	};
}

/**
 * Takes a Fighter, a Enemy and a Boss pointer; plus an integer indicating if effects should be applied to enemy (==0) or boss (==1=.
 * Iterates over artifacts bag and calls the inner function pointer if an artifact has positive qty and its active member is false.
 * @see Artifact
 * @param f A Fighter pointer.
 * @param e An Enemy pointer.
 * @param b The Boss pointer.
 * @param isBoss The integer defining who is receiving the effect.
 */
void applyArtifacts(Fighter* f, Enemy* e, Boss* b, int isBoss){

	for (int i=0; i < ARTIFACTSMAX + 1; i++) {
		Artifact* a = f->artifactsBag[i];
		if (a->qty != 0 && !(a->active)) { //We only apply the ones we have unlocked && aren't active already
			switch (a->class) {
				case THKSKULL: {
					artifact_thinkingskull(f,e,b, isBoss);
					       }
				break;
				case TWINKIE: {
					artifact_twinkie(f,e,b,isBoss);
					       }
				break;
				case WRISTBAND: {
					artifact_wristband(f,e,b,isBoss);
					       }
				break;
				case BOARTAIL: {
					artifact_boartail(f,e,b,isBoss);
					       }
				break;
				case CHAOSORB: {
					artifact_chaosorb(f,e,b,isBoss);
					       }
				break;
				case POWERSYPHON: {
					artifact_powersyphon(f,e,b,isBoss);
					       }
				break;
				case GIANTFOSSILE: {
					artifact_giantfossile(f,e,b,isBoss);
					       }
				break;
			};
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
 * @param kls The Koliseo used for allocations.
 */
void initEquipSlots(Fighter* f, Koliseo* kls){
	char msg[200];
	for (int i = 0; i <= EQUIPZONES ; i++) {
		sprintf(msg,"Prepping Equipslot (%i)",i);
		kls_log("DEBUG",msg);
		Equipslot* s = (Equipslot*) KLS_PUSH_NAMED(kls,Equipslot,1,"Equipslot",msg);
		s->active = 0;
		s->type = i;
		setEquipslotSprite(s);
		f->equipslots[i] = (struct Equipslot*) s;
	};
}

/**
 * Takes a Fighter pointer and prepares its consumablesBag field by allocating a Consumable for each consumableClass.
 * @see Fighter
 * @see Consumable
 * @see consumableClass
 * @see CONSUMABLESMAX
 * @param f The Fighter pointer whose consumablesBag field will be initialised.
 * @param kls The Koliseo to do allocations.
 */
void initConsumableBag(Fighter* f, Koliseo* kls) {
	char msg[200];

	for (int i = 0; i < CONSUMABLESMAX +1; i++) {
		sprintf(msg,"Prepping Consumable (%i)",i);
		kls_log("DEBUG",msg);
		Consumable* c = (Consumable*) KLS_PUSH_NAMED(kls, Consumable, 1,"Consumable",msg);
		c->class = i;

		Consumable* base = &consumablesBase[i];

		strcpy(c->name,base->name);
		strcpy(c->desc,base->desc);
		for (int j = 0; j < 8; j++) {
			strcpy(c->sprite[j],base->sprite[j]);
		}
		c->qty = 0;

		f->consumablesBag[i] = (struct Consumable*) c;
	}

}

/**
 * Takes a Fighter pointer and prepares its artifactsBag field by allocating a Artifact for each artifactClass.
 * @see Fighter
 * @see Artifact
 * @see artifactClass
 * @see ARTIFACTSMAX
 * @param kls The Koliseo used for allocations.
 * @param f The Fighter pointer whose artifactsBag field will be initialised.
 */
void initArtifactsBag(Fighter* f, Koliseo* kls) {
	char msg[200];
	for (int i = 0; i < ARTIFACTSMAX +1; i++) {
		sprintf(msg,"Prepping Artifact (%i)",i);
		kls_log("DEBUG",msg);
		Artifact* a = (Artifact*) KLS_PUSH_NAMED(kls,Artifact,1,"Artifact",msg);
		a->class = i;

		Artifact* base = &artifactsBase[i];

		strcpy(a->name,base->name);
		strcpy(a->desc,base->desc);
		for (int j = 0; j < 8; j++) {
			strcpy(a->sprite[j],base->sprite[j]);
		}
		a->qty = 0;

		f->artifactsBag[i] = (struct Artifact*) a;
	}
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
 * @param player The Fighter whose fields will be initialised.
 * @param path The Path pointer of the current game.
 * @param kls The Koliseo used for allocation.
 */
void initPlayerStats(Fighter* player, Path* path, Koliseo* kls) {
	char msg[200];

	//player luck : MAXPLAYERLUCK = path luck : MAXLUCK

	player->luck = (path->luck * MAXPLAYERLUCK) / MAXLUCK ;

	BaseStats* base = &basestats[player->class];

	sprintf(msg,"Prepping countStats");
	kls_log("DEBUG",msg);
	countStats* s = (countStats*) KLS_PUSH_NAMED(kls,countStats,1,"countStats",msg);

	s->enemieskilled=0;
	s->criticalhits=0;
	s->consumablesfound=0;
	s->equipsfound=0;
	s->artifactsfound=0;
	s->roomscompleted=0;
	s->floorscompleted=0;
	s->specialsunlocked=0;
	s->coinsfound=0;
	s->bosseskilled=0;
	s->unique_bosseskilled=0;
	for (int i = 0; i < BOSSCLASSESMAX+1; i++) {
		s->killed_bosses[i] = 0;
	}
	s->keysfound=0;

	setSpecials(player,kls);
	initCounters(player,kls);
	initPerks(player,kls);

	initConsumableBag(player,kls);
	initArtifactsBag(player,kls);

	initEquipSlots(player,kls);
	player->equipsBagOccupiedSlots = 0; //Keeps track of how many slots are occupied.
	player->earliestBagSlot = 0; //To alwasy use the array efficiently (???) I sense linked lists may be better

	player->permboost_atk = 0;
	player->permboost_def = 0;
	player->permboost_vel = 0;
	player->permboost_enr = 0;

	player->equipboost_atk = 0;
	player->equipboost_def = 0;
	player->equipboost_vel = 0;
	player->equipboost_enr = 0;

	player->balance = 0;
	player->keys_balance = 0;

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

	setFighterSprite(player);
}

/**
 * Takes one Enemy pointer and initialises the enemy fields.
 * The EnemyBaseStats pointer for the enemy's enemyClass is loaded.
 * If the beast field at the pointer is already set before this call, atk def vel and hp of the enemy will receive a multiplicative boost equal to BSTFACTOR . Also, Xp value will be multiplied by 3.
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
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initEnemyStats(Enemy* e, Koliseo_Temp* t_kls) {
	EnemyBaseStats* base = &baseenemystats[e->class];
	char msg[200];
	sprintf(msg,"Init stats for enemy (%s)",stringFromEClass(e->class));
	log_tag("debug_log.txt","[DEBUG]",msg);

	float beastf = 1;

	if (e->beast) {
		beastf = BSTFACTOR;
	}

	e->hp = round(beastf * base->hp);
	e->totalhp = round(beastf * base->totalhp);
	e->atk = round(beastf * base->atk);
	e->def = round(beastf * base->def);
	e->vel = round(beastf * base->vel);
	e->status = Normal;

	//Level should be set by caller
	//Index should be set by caller

	e->totalenergy = base->totalenergy;
	e->energy = e->totalenergy;

	//Set prize, double for beasts
	float prize = 2.8 * e->level;

	e->prize = floor((e->beast) ? 2 * prize : prize);

	initECounters(e,t_kls);

	//Triple xp for beasts
	e->xp = (e->beast) ? 3 * base->xp : base->xp;

	setEnemySprite(e);
}

/**
 * Takes one Boss pointer and initialises the boss fields.
 * The BossBaseStats pointer for the boss's bossClass is loaded.
 * If the beast field at the pointer is already set before this call, atk def vel and hp of the enemy will receive a multiplicative boost equal to BSTFACTOR . Xp value will also be multiplie by 3.
 * Notably, the level field is not set at all by this function and needs to be set by the caller.
 * All the fields common to BossBaseStats are initialised with the base value and eventual beast boost.
 * initBCounters() is called to init the counters field.
 * The status field is set to Normal.
 * @see Boss
 * @see bossClass
 * @see BossBaseStats
 * @see initBCounters()
 * @see fighterStatus
 * @param b The Boss whose fields will be initialised.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initBossStats(Boss* b, Koliseo_Temp* t_kls) {
	//Class should be set by caller
	BossBaseStats* base = &basebossstats[b->class];

	float beastf = 1;

	if (b->beast) {
		beastf = BSTFACTOR;
	}

	b->hp = round(beastf * base->hp);
	b->totalhp = round(beastf * base->totalhp);
	b->atk = round(beastf * base->atk);
	b->def = round(beastf * base->def);
	b->vel = round(beastf * base->vel);
	b->status = Normal;

	//Level should be set by caller

	b->totalenergy = base->totalenergy;
	b->energy = b->totalenergy;

	//Set prize, double for beasts
	float prize = 4.5 * b->level;

	b->prize = (b->beast) ? 2 * prize : prize;

	initBCounters(b,t_kls);

	//Triple xp for beasts
	b->xp = (b->beast) ? 3 * base->xp : base->xp;

	setBossSprite(b);
}

/**
 * Takes one FoeParty pointer and initialises the party fields.
 * The FoePartyBaseStats pointer for the FoeParty's foePartyClass is loaded.
 * All the fields common to FoePartyBaseStats are initialised with the base value.
 * initFoePartyCounters() is called to init the counters field.
 * @see FoeParty
 * @see foePartyClass
 * @see FoePartyBaseStats
 * @see initFoePartyCounters()
 * @param fp The FoeParty whose fields will be initialised.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initFoePartyStats(FoeParty* fp, Koliseo_Temp* t_kls) {
	//Class should be set by caller
	//FoePartyBaseStats* base = &basefoepartystats[fp->class];
	//TODO: add loading of more base stats not expected in prepareFoeParty()

	//Level should be set by caller

	//Zero the alive_enemies values
	for (int i=0; i< ROOM_ENEMIES_MAX+1; i++) {
		fp->alive_enemies[i] = 0;
	};
	//Zero the alive_bosses values
	for (int i=0; i< FOES_BOSSES_MAX+1; i++) {
		fp->alive_bosses[i] = 0;
	};

	//Zero the turnboost_STAT values
	fp->turnboost_atk = 0;
	fp->turnboost_def = 0;
	fp->turnboost_vel = 0;
	fp->turnboost_enr = 0;

	//Zero current enemy index
	fp->current_index = 0;

	//Size value should be set by caller
	fp->tot_alive = fp->size;

	switch (fp->class) {
		case Enemies: {
			//Set alive_enemies values
			for (int i=0; i< fp->tot_alive+1; i++) {
				fp->alive_enemies[i] = 1;
			};
		}
		break;
		case Bosses: {
			//Set alive_bosses values
			for (int i=0; i< fp->tot_alive+1; i++) {
				fp->alive_bosses[i] = 1;
			};
		}
		break;
		default: {
			fprintf(stderr,"UNEXPECTED FOEPARTY CLASS VALUE %i", fp->class);
			log_tag("debug_log.txt","[PANIC]","Invalid class value in initFoePartyStats()");
			exit(EXIT_FAILURE);
		}
		break;
	};

	//Init party counters
 	initFoePartyCounters(fp,t_kls);
}

/**
 * Takes a FoeParty pointer, a size for complete party, and an integer for the current room index, and initialises all the fields making the FoeParty ready for use in battle.
 * Calls initFoePartyStats() after setting class and level
 * @see FoeParty
 * @see initFoePartyStats()
 * @param fp The allocated FoeParty pointer to initialise.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void prepareFoeParty(FoeParty* fp, int partysize, int roomindex, Koliseo_Temp* t_kls) {
	char msg[500];

	//Class must be set by caller

	FoePartyBaseStats* base = &basefoepartystats[fp->class];

	fp->level = base->level;
	//Set size value
	fp->size = partysize;

	//FoeParty's get 1 level every 2 rooms
	//fp->level += floor(roomindex / 2) ;

	//Load foeparty stats
	initFoePartyStats(fp, t_kls);

	sprintf(msg,"Prepared FoeParty with size (%i), room #%i.", partysize, roomindex);
	log_tag("debug_log.txt","[DEBUG]",msg);
}

/**
 * Takes one integer and a bossClass and returns the boost relative to the level delta with base boss stats, as an integer.
 * The EnemyBossStats pointer for the boss's bossClass is loaded.
 * If the boost is negative, returns 0.
 * @see Boss
 * @see bossClass
 * @see BossBaseStats
 * @param lvl The level to check the boost against.
 * @param bclass The bossClass used to determine base level.
 * @return int The boost for any given stat, at the level passed as argument.
 */
int getBossBoost(int lvl, bossClass bclass) {

	BossBaseStats* base = &basebossstats[bclass];

	float boost = ((lvl - base->level) * 1.25) ;
	if (boost <= 0) {
		boost = 0;
	}

	return (int) boost;
}

/**
 * Takes a Boss pointer and an integer used to force execution.
 * If the force parameter is true, all checks are ignored.
 * If boss's hp value is at least 40% of total, and none of atk, def or vel is 0 or less, nothing happens with an early return.
 * Otherwise, getBossBoost() is called to calc the level stat boost for each stat.
 * The BossBaseStats pointer for the boss's bossClass is loaded and each one of atk, def and vel is checked accounting for level boost.
 * If none of them is below the respective treshold of 30, 30 and 20 % of total, nothing happens.
 * Otherwise, all of them are reset to full amount accounting for beast boost and level boost.
 * @see Boss
 * @see bossClass
 * @see getBossBoost()
 * @see BSTFACTOR
 * @see BossBaseStats
 * @param b The Boss pointer to check the stats for.
 * @param force The integer to bypass all checks if true.
 */
void statResetBoss(Boss* b, int force) {
	if ( !force && (b->hp >= 0.4 * b->totalhp) && !( b->atk <=0 || b->def <= 0 || b->vel <=0) ) {
		return;
	}
	int boost = getBossBoost(b->level,b->class);

	float beastf = 1 ;

	if (b->beast) {
		beastf = BSTFACTOR;
	}
	BossBaseStats* base = &basebossstats[b->class];

	if (force) { //We also update hp values
		int hpBoost = boost + round(base->level * 0.75) + (base->hp / 10 ) + ((base->def / 4) % 10) ;
		b->hp = round(beastf * (base->hp + hpBoost));
		b->totalhp = b->hp;
	}

	if (force || b->vel <= (0.3 * (base->vel + boost)) || (b->atk <= (0.3 * (base->atk + boost))) || b->def <= (0.2 * (base->def + boost))) {
		b->vel = round(beastf * (base->vel + boost));
		b->atk = round(beastf * (base->atk + boost));
		b->def = round(beastf * (base->def + boost));
		//Reset stats
		if (!force) {
			//yellow();
			//printf("\n\n\t%s's stats reset.\n",stringFromEClass(e->class));
			//white();
		}
	}
}

/**
 * Takes a Boss pointer and initialises all the fields making it ready for use in battle.
 * Calls initBossStats() after setting class and level, then forces a stat reset to update the stats
 * with the level boost.
 * @see Boss
 * @see initBossStats()
 * @param b The allocated Boss pointer to initialise.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void prepareBoss(Boss* b, Koliseo_Temp* t_kls) {

	//Randomise boss class
	b->class = rand() % (BOSSCLASSESMAX + 1);

	b->beast = 0;
	BossBaseStats* base = &basebossstats[b->class];

	b->level = base->level;

	//Enemies get 1 level every 2 rooms
	//e->level += floor(roomindex / 2) ;

	//Load boss stats
	initBossStats(b, t_kls);

	//Force load of level bonuses
	statResetBoss(b,1);

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

	float boost = ((lvl - base->level) * 1.25) ;
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
 */
void statResetEnemy(Enemy* e, int force) {
	char msg[200];
	sprintf(msg,"Call to statResetEnemy() with ($force) == (%i)", force);
	log_tag("debug_log.txt","[DEBUG]",msg);
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
		int hpBoost = floor(0.5 * boost + round(base->level * 0.75) + (base->hp / 10 ) + ((base->def / 4) % 10)) ;
		e->hp = round(beastf * (base->hp + hpBoost));
		e->totalhp = e->hp;
	}

	if (force || e->vel <= (0.3 * (base->vel + boost)) || (e->atk <= (0.3 * (base->atk + boost))) || e->def <= (0.2 * (base->def + boost))) {
		e->vel = round(beastf * (base->vel + boost));
		e->atk = round(beastf * (base->atk + boost));
		e->def = round(beastf * (base->def + boost));
		//Reset stats
		if (!force) {
			//yellow();
			//printf("\n\n\t%s's stats reset.\n",stringFromEClass(e->class));
			//white();
		}
	}
}

/**
 * Takes a Enemy and three integers denoting current room index, how many enemies are in the room and current enemy index.
 * The class field of the enemy is randomised according to ENEMYCLASSESMAX.
 * If the room index is multiple of BEASTROOM and the enemy is the last one in he room, its beast field is set to 1.
 * The EnemyBaseStats pointer for the enemy's enemyClass is loaded and the level field for the enemy is set to base level, before increasing.
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
 * @param roomindex The index of current room.
 * @param enemiesInRoom The number of enemies in current room.
 * @param enemyindex The index of current enemy.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void prepareRoomEnemy(Enemy* e, int roomindex, int enemiesInRoom, int enemyindex, Koliseo_Temp* t_kls) {
		char msg[500];

		//Randomise enemy class
		e->class = rand() % (ENEMYCLASSESMAX + 1);

		if (G_DEBUG_ENEMYTYPE_ON && ! (GAMEMODE == Story) ){ //Debug flag has a fixed enemy class when used outside of story gamemode
			sprintf(msg,"prepareRoomEnemy(): Enemy debug flag was asserted outside of story mode, will always spawn a G_DEBUG_ENEMYTYPE (%s).\n",stringFromEClass(G_DEBUG_ENEMYTYPE));
			log_tag("debug_log.txt","\n[DEBUG]",msg);
			e->class = G_DEBUG_ENEMYTYPE;
		}


		//Check for spawning beast enemies
		if ((roomindex % BEASTROOM == 0) && (enemyindex == (enemiesInRoom - 1))) {
			//TODO: better mechanic for spawning beast enemies
			if (((rand() % 5) == 0)) {
				log_tag("debug_log.txt","[DEBUG]","Setting e->beast to 1.");
			       	e->beast = 1;
			}
		} else {
			e->beast = 0;
		};


		EnemyBaseStats* base = &baseenemystats[e->class];

		e->level = base->level;

		//Enemies get 1 level every 2 rooms
		e->level += floor(roomindex / 2) ;

		//Set current enemy index
		e->index = enemyindex;

		//Load enemy stats
		initEnemyStats(e,t_kls);

		//Force load of level bonuses
		statResetEnemy(e,1);
}

/**
 * Takes a size int and a pointer to integer array, initialises all the prices, depending on stats from the Equips array pointer.
 * @see initShop()
 * @see Equip
 * @param size Size of both arrays.
 * @param equipPrices Pointer to integer array for prices.
 * @param equips Pointer to Equip pointer to influence pricing.
 */
void setEquipPrices(int size, int* equipPrices, Equip* equips[]) {

	for (int i = 0; i < size; i++) {
		int* cur_price = &equipPrices[i];
		Equip* e = equips[i];

		int baseprice = e->cost;
		int price = baseprice;

		*cur_price = price;
	}

}

/**
 * Takes a size int and a pointer to integer array, initialises all the prices.
 * @see initShop()
 * @param size Size of both arrays.
 * @param consumablePrices Pointer to integer array for prices.
 * @param consumables Pointer to Consumable pointer to influence pricing.
 */
void setConsumablePrices(int size, int* consumablePrices, Consumable** consumables) {

	for (int i = 0; i < size; i++) {
		int* cur_price = &consumablePrices[i];
		//Consumable* c = consumables[i];

		//Price evaluation
		int baseprice = 4;
		int price = baseprice + (rand() % 5) -1;

		*cur_price = price;
	}
}

/**
 * Takes one Shop pointer and initialises all the fields, depeding on stats from the Fighter pointer passed.
 * @see Shop
 * @param s The Shop whose fields will be initialised.
 * @param indexWeight The integer for room index weight.
 * @param player The Fighter player to influence item generation.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initShop(Shop* s, int indexWeight, Fighter* player, Koliseo_Temp* t_kls) {

	Koliseo_Temp tkls = *t_kls;
	char msg[200];

	int equipsCount = (rand() % EQUIP_SHOP_MAX )+1;

	if (equipsCount != 0 ) {

		for (int equip_index = 0; equip_index < equipsCount; equip_index++) {
			int curr = (rand() % (EQUIPSMAX+1));
			sprintf(msg,"Prepping Equip (%i/%i) for Shop", equip_index, equipsCount);
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			Equip* e = (Equip*) KLS_PUSH_T_NAMED(tkls,Equip,1,"Equip",msg);

			//Randomise quality
			quality q = rand() % (QUALITIESMAX + 1);

			//Get the base item and copy the stats to the current equip
			Equip* base = &equips[curr];

			e->class = base->class;
			e->type = base->type;
			e->qual = q;

			setEquipSprite(e);
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
				int chance = 35;

				if (q == Good) {
					chance *= 1.5;
				}

				if ( (rand() % 100) < chance ) {

					e->perksCount += 1;
					sprintf(msg,"Prepping Perk (%i) for Shop Equip (%i/%i)", i, equip_index, equipsCount);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					Perk* p = (Perk*) KLS_PUSH_T_NAMED(tkls,Perk,1,"Perk",msg);
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

			//Calc cost value

			float cost = 3;

			cost += 4.5 * (e->qual +1);
			cost += 7.5 * (e->perksCount);

			cost += 10.2 * (e->level);

			e->cost = floor(cost);

			s->equips[equip_index] = e;
			s->equipsCount++;


		}//End for all equips
	}
	s->equipsCount = equipsCount;
	setEquipPrices(s->equipsCount, s->equipPrices, s->equips);

	int uniqueConsumablesCount = (rand() % SINGLE_CONSUMABLE_SHOP_MAX)+1;
	int uniques = 0;
	//int not_uniques = 0;
	if (uniqueConsumablesCount != 0 ) {
		int already_rolled[CONSUMABLESMAX+1];
		for (int i=0; i < (CONSUMABLESMAX+1); i++) {
			already_rolled[i] = 0;
		}
		int cons_prepared = 0;
		while (cons_prepared < uniqueConsumablesCount) {

			for (int i=0; i < CONSUMABLESMAX+1 ; i++) {
				if (cons_prepared < uniqueConsumablesCount) {
					int curr = rand() % (CONSUMABLESMAX+1);
					if (!(already_rolled[curr])) {
						sprintf(msg,"Prepping Consumable (%i/%i) for Shop", cons_prepared, uniqueConsumablesCount);
						log_tag("debug_log.txt","[DEBUG]",msg);
						kls_log("DEBUG",msg);
						Consumable *cur = (Consumable*) KLS_PUSH_T_NAMED(tkls,Consumable,1,"Consumable",msg);
						cur->class = curr;
						already_rolled[curr] = 1;
						if (uniqueConsumablesCount - cons_prepared > 0) {
							cur->qty = (rand() % 4)+1;
						} else {
							cur->qty = 1;
							//cur->qty = (rand() % (consumablesCount - cons_index)) +1;
						}
						setConsumableSprite(cur);
						//not_uniques += cur->qty;
						cons_prepared ++; //= cur->qty;
						s->consumables[uniques] = cur;
						uniques++;
					}
				}
			};
		}
	}
	s->consumablesCount = uniqueConsumablesCount;
	setConsumablePrices(s->consumablesCount, s->consumablePrices, s->consumables);

	s->itemCount = s->consumablesCount + s->equipsCount;
}

/**
 * Takes a Chest and Fighter pointers and initialises all the fields in chest based on chest class and fighter stats.
 * @see Chest
 * @param c The allocated Chest pointer with already set class to initialise.
 * @param f The Fighter pointer with stats.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initChest(Chest* c, Fighter* f, Koliseo_Temp* t_kls) {

	Koliseo_Temp tkls = *t_kls;
	char msg[200];

	setChestSprite(c);

	strcpy(c->desc,descStringFromChest(c->class));
	int cons_cnt, equip_cnt;

	switch (c->class) {
		case CHEST_BASE: {
			cons_cnt = rand() % (CHEST_CONSUMABLES_MAX -1);
			equip_cnt = rand() % (CHEST_EQUIPS_MAX -1);
		}
		break;
		case CHEST_BEAST: {
			cons_cnt = ( rand() % (CHEST_CONSUMABLES_MAX) ) +1;
			equip_cnt = ( rand() % (CHEST_EQUIPS_MAX) ) +1;

		}
		break;
		default: {
			sprintf(msg,"%i is not a valid chest class.\n",c->class);
			log_tag("debug_log.txt","[ERROR]",msg);
			exit(EXIT_FAILURE);
		}
		break;

	}

	c->consumablesCount = cons_cnt;
	c->equipsCount = equip_cnt;

	if (c->consumablesCount > 0) {
		for (int i = 0; i < c->consumablesCount; i++) {
			sprintf(msg,"Prepping Consumable (%i/%i) for Chest", i, c->consumablesCount);
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			Consumable* cns = (Consumable*) KLS_PUSH_T_NAMED(tkls,Consumable,1,"Consumable",msg);
			int drop = rand() % (CONSUMABLESMAX + 1);

			cns->class = drop;

			Consumable* base = &consumablesBase[drop];

			strcpy(cns->name,base->name);
			strcpy(cns->desc,base->desc);
			for (int j = 0; j < 8; j++) {
				strcpy(cns->sprite[j],base->sprite[j]);
			}
			cns->qty = 1;

			c->consumables[i] = cns;
		}
	}

	if (c->equipsCount > 0) {
		for (int i = 0; i < c->equipsCount; i++) {

			//Select a basic item from the list
			int drop = rand() % (EQUIPSMAX + 1);
			//Randomise quality
			quality q = rand() % (QUALITIESMAX + 1);

			//Prepare the item
			sprintf(msg,"Prepping Equip (%i/%i) for Chest", i, c->equipsCount);
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			Equip* e = (Equip*) KLS_PUSH_T_NAMED(tkls,Equip,1,"Equip",msg);

			//Get the base item and copy the stats to the drop
			Equip* base = &equips[drop];

			e->class = base->class;
			e->type = base->type;
			e->qual = q;

			setEquipSprite(e);
			strcpy(e->name,base->name);
			strcpy(e->desc,base->desc);

			e->qty = 1;
			e->equipped = 0;

			e->perksCount = 0;

			//Calc drop level
			e->level = base->level + round(f->level / EQUIPLVLBOOSTRATIO);

			//Chance for better leveled item
			if ( ( rand() % 8 ) - (f->luck / 10) <= 0) { //Should use a defined constant
				e->level += 1; //At least a simple +1
				if ( ( rand() % 25 ) - (f->luck / 10) <= 0) { //Should use a defined constant
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

			for (int j = 0; j < (EQUIPPERKSMAX); j++) {
				int chance = 20;

				if (q == Good) {
					chance *= 1.5;
				}

				if ( (rand() % 100) < chance || (c->class == CHEST_BEAST && e->perksCount == 0)) {

					e->perksCount += 1;

					sprintf(msg,"Prepping Perk (%i/%i) for Equip (%i/%i) for Chest", j, e->perksCount, i, c->equipsCount);
					log_tag("debug_log.txt","[DEBUG]",msg);
					kls_log("DEBUG",msg);
					Perk* p = (Perk*) KLS_PUSH_T_NAMED(tkls,Perk,1,"Perk",msg);
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

			//Calc cost value

			float cost = 5;

			cost += 2.5 * (e->qual);
			cost += 3.5 * (e->perksCount);

			cost += 7.2 * (e->level);

			e->cost = floor(cost);

			c->equips[i] = e;
		} //End for all equips
	}//End if equipsCount > 0
}

/**
 * Takes a Chest and Fighter pointers and initialises all the fields in chest making it ready for use in battle, using fighter stats to influence init.
 * Calls initChest() after setting class.
 * @see Chest
 * @see initChest()
 * @param c The allocated Chest pointer to initialise.
 * @param f The Fighter pointer to influence item generation.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void prepareChest(Chest* c, Fighter* f, Koliseo_Temp* t_kls) {

	//Init chest class
	int drop = (rand() % 100 ) +1;

	if (drop > 70) {
		c->class = CHEST_BASE;
	} else {
		c->class = CHEST_BEAST;
	}

	//Load Chest stats
	initChest(c,f,t_kls);

}

/**
 * Takes a Treasure and Fighter pointers and initialises all the treasure fields based on its class and fighter's stats.
 * @see Treasure
 * @param t The allocated Treasure pointer with already set class to initialise.
 * @param f The Fighter pointer to influence item generation.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void initTreasure(Treasure* t, Fighter* f, Koliseo_Temp* t_kls) {
	char msg[200];

	Koliseo_Temp tkls = *t_kls;

	strcpy(t->desc,descStringFromTreasure(t->class));

	switch(t->class) {
		case TREASURE_CHEST: {

			    sprintf(msg,"Allocated %lu for Treasure [Chest]:",sizeof(Chest));
			    log_tag("debug_log.txt","[DEBUG]",msg);
			    kls_log("DEBUG",msg);
			    Chest* c = (Chest*) KLS_PUSH_T_NAMED(tkls,Chest,1,"Chest",msg);
			    prepareChest(c,f,t_kls);
			    t->chest = c;

			    }
		break;
		case TREASURE_CONSUMABLE: {
			sprintf(msg,"Allocated %lu for Treasure [Consumable]:",sizeof(Consumable));
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			Consumable* cns = (Consumable*) KLS_PUSH_T_NAMED(tkls,Consumable,1,"Consumable",msg);
			int drop = rand() % (CONSUMABLESMAX + 1);

			cns->class = drop;

			Consumable* base = (Consumable*) f->consumablesBag[drop];

			strcpy(cns->name,base->name);
			strcpy(cns->desc,base->desc);
			for (int j = 0; j < 8; j++) {
				strcpy(cns->sprite[j],base->sprite[j]);
			}
			cns->qty = 1;

			t->consumable = cns;
			}
		break;
		case TREASURE_ARTIFACT: {
			sprintf(msg,"Allocated %lu for Treasure [Artifact]:",sizeof(Artifact));
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			Artifact* a = (Artifact*) KLS_PUSH_T_NAMED(tkls,Artifact,1,"Artifact",msg);
			int drop = -1;
			do {
				drop = rand() % (ARTIFACTSMAX + 1);
			} while (f->artifactsBag[drop]->qty > 0);

			a->class = drop;

			Artifact* base = f->artifactsBag[drop];

			strcpy(a->name,base->name);
			strcpy(a->desc,base->desc);
			for (int j = 0; j < 8; j++) {
				strcpy(a->sprite[j],base->sprite[j]);
			}
			a->qty = 0;

			t->artifact = a;

			    }
		break;
	}
}

/**
 * Takes a Treasure and Fighter pointer and initialises all the treasure fields making it ready for use in battle, based on the fighter stats.
 * Calls initTreasure() after setting class.
 * @see Treasure
 * @see initTreasure()
 * @param t The allocated Treasure pointer to initialise.
 * @param f The Fighter pointer to influence item generation.
 * @param t_kls The Koliseo_Temp used for allocations.
 */
void prepareTreasure(Treasure* t, Fighter* f, Koliseo_Temp* t_kls) {

	//Init treasure class

	int roll = (rand() % 100) +1;

	if (roll > 70) {
		t->class = TREASURE_CHEST;
	} else if (roll > 50) {
		t->class = TREASURE_ARTIFACT;
	} else {
		t->class = TREASURE_CONSUMABLE;
	}

	//Load Treasure stats
	initTreasure(t,f, t_kls);

}

/**
 * Takes a Roadfork pointer and initialises all the fields making it ready for use in battle.
 * Calls initRoadfork() after setting class.
 * @see Roadfork
 * @see initRoadfork()
 * @param r The allocated Roadfork pointer to initialise.
 */
void prepareRoadfork(Roadfork* r) {

	//Randomise options
	r->options[0] = rand() % (ROADFORK_OPTIONS_MAX + 1);
	int previous = r->options[0];
	int new = -1;

	do {
		new = rand() % (ROADFORK_OPTIONS_MAX + 1);

	} while (new == previous);

	previous = new;
	r->options[1] = new;

}

/**
 * Takes a Fighter pointer and prints most of its values formatted.
 * @see Fighter
 * @see stringFromClass()
 * @see stringFromStatus()
 * @param f The Fighter pointer with stats to print.
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
 * Takes a Boss pointer and returns its xp gain as sum of xp field value and level.
 * @see Boss
 * @param b The Boss pointer.
 * @return int The xp gain.
 */
int getBossXpGain(Boss* b) {

	int xp = b->xp + b->level;
	return xp;
}

/**
 * Takes a Consumable pointer and prints most of its values formatted.
 * @see Consumable
 * @param c The Consumable pointer with stats to print.
 */
void printConsumablesStats(Consumable* c) {
	printf("  (%i)\t%i\t%s\t\"%s\"\n",c->class,c->qty,c->name,c->desc);
}

/**
 * Takes a Artifact pointer and prints most of its values formatted.
 * @see Artifact
 * @param a The Artifact pointer with stats to print.
 */
void printArtifactStats(Artifact* a) {
	printf("  (%i)\t\t",a->class);
	purple();
	printf("%s\t",a->name);
	yellow();
	printf("\"%s\"\n",a->desc);
	white();
}

/**
 * Takes a quality value and calls the respective color function without actually printing text.
 * @see quality
 * @param q The quality value we want to set text color for.
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
}

/**
 * Takes a Equip pointer and prints most of its values formatted.
 * The beginning of the format assumes the caller prepended "(x" on the output before calling, where x is the index of the equip.
 * @see Equip
 * @see stringFromQuality()
 * @param e The Equip pointer with stats to print.
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
}

/**
 * Takes a Enemy pointer and two integers for current room index and current enemy index, and prints the spawn message formatted.
 * @see Enemy
 * @see stringFromEClass()
 * @param e The Enemy pointer to print.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 */
void printSpawnMessage(Enemy* e, int roomIndex, int enemyIndex) {
	char msg[500];
	if (!e->beast) {
		//TODO:
		//Where to print a windowed spawn message?
		sprintf(msg, "Room %i)\t\t%s\t\tEnemy #%i",roomIndex,stringFromEClass(e->class),enemyIndex+1);
		log_tag("debug_log.txt","[ROOM]",msg);
	} else {
		//lightBlue();
		//printf("\nYou feel at discomfort.\n\nRoom %i)\t\t",roomIndex);
		//lightRed();
		//printf("Enemy #%i:\t%s",enemyIndex+1,stringFromEClass(e->class));
		//white();
	}
}


/**
 * Takes a Wincon and a Path pointers and a winconClass and initialises the passed Wincon.
 * @see Wincon
 * @see Path
 * @see winconClass
 * @see WINCON_CLASS_MAX
 * @param w The Wincon pointer to initialise.
 * @param p The Path to use to initialise Wincon.
 * @param class The winconClass to use to initialise.
 */
void initWincon(Wincon* w, Path* p, winconClass class) {

	w->class = class;

	switch (w->class) {
		case ALL_BOSSES: {
			w->current_val = 0;
			w->target_val = BOSSCLASSESMAX+1;

		};
		break;
		case ALL_ARTIFACTS: {
			w->current_val = 0;
			w->target_val = ARTIFACTSMAX+1;

		};
		break;
		case FULL_PATH: {
			w->current_val = 0;
			w->target_val = p->length;
		};
		break;
		default: {
			fprintf(stderr,"\nUnexpected Wincon Class %i\n",w->class);
			w->class = -1;
			w->current_val = 0;
			w->target_val = 1;
		}

	}

}

/**
 * Takes a integer and a string array (possibly from main), a Fighter pointer and a Path pointer to set some values.
 * Depending on argc value, the arguments in argv may be used instead of calling the functions to get user input.
 * @see Fighter
 * @see pickName()
 * @see pickClass()
 * @see pickWincon()
 * @param argc The number of argv values + 1 (0 is program name?).
 * @param argv Array of strings with argc - 1 values.
 * @param player The Fighter of which we set name and class.
 * @param path The Path pointer used for the game.
 * @param kls The Koliseo used for allocation.
 */
void getParams(int argc, char** argv, Fighter* player, Path* path, int optTot, Koliseo* kls) {
	char msg[200];

	int argTot = argc - optTot;
	if (argTot == 0) {
		pickName(player);
		pickClass(player);
		sprintf(msg,"Prepping Wincon");
		kls_log("DEBUG",msg);
  		Wincon* w = (Wincon*) KLS_PUSH_NAMED(kls, Wincon, 1,"Wincon",msg);
		if (GAMEMODE == Story) {
			//Path length must be already initialised before getting here.
			initWincon(w,path,FULL_PATH);
		} else if (GAMEMODE == Rogue) {
			//Path length must be already initialised before getting here.
			initWincon(w,path,ALL_ARTIFACTS);
		} else {
			pickWincon(w);
			initWincon(w,path,w->class);
		}
		path->win_condition = w;
	} else if (argTot == 1 || argTot == 2){
		char read_name[25];
		int i = 0, len=0;
		len = strlen(argv[optTot]);
		if (len < 20) {
			for (; i<len; i++) {
				read_name[i]=argv[optTot][i];
			}
			read_name[i]='\0';
			strcpy(player->name,read_name);
		} else {
			pickName(player);
		}
	} else {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	if ( argTot == 1 ) {
		pickClass(player);
		sprintf(msg,"Prepping Wincon");
		kls_log("DEBUG",msg);
  		Wincon* w = (Wincon*) KLS_PUSH_NAMED(kls, Wincon, 1,"Wincon",msg);
		if (GAMEMODE == Story) {
			//Path length must be already initialised before getting here.
			initWincon(w,path,FULL_PATH);
		} else if (GAMEMODE == Rogue) {
			//Path length must be already initialised before getting here.
			initWincon(w,path,ALL_ARTIFACTS);
		} else {
			pickWincon(w);
			initWincon(w,path,w->class);
		}
		path->win_condition = w;
	}

	if ( argTot == 2 ) {
		int c= -1;
		int i = 0;
		for (i=0; i <= CLASSESMAX ; i++ ) {
			if (strcmp(argv[optTot+1],classesstrings[i]) == 0){
				c=1;
				player->class=i;
				break;
			}
		}
		if (c < 0 ) {
			pickClass(player);
		}
		sprintf(msg,"Prepping Wincon");
		kls_log("DEBUG",msg);
  		Wincon* w = (Wincon*) KLS_PUSH_NAMED(kls, Wincon, 1,"Wincon",msg);
		if (GAMEMODE == Story) {
			//Path length must be already initialised before getting here.
			initWincon(w,path,FULL_PATH);
		} else if (GAMEMODE == Rogue) {
			//TODO: what do we set as path length? Number of floors?
			//Path length must be already initialised before getting here.
			initWincon(w,path,ALL_ARTIFACTS);
		} else {
			pickWincon(w);
			initWincon(w,path,w->class);
		}
		path->win_condition = w;
	}
}

/**
 * Returns the chosen option as a turnOption.
 * @param ch A string representing the turn choice.
 * @return The chosen turnOption value representing turn action.
 */
turnOption getTurnChoice(char* ch) {
	int comp = 999;
	char msg[500];

	sprintf(msg,"Turnchoice string was (%s)",ch);
	log_tag("debug_log.txt","[TURNCHOICE]",msg);
	turnOption pick = INVALID;

	while (pick == INVALID) {
		if ((comp = strcmp(ch, "Fight")) == 0) {
			pick = FIGHT;
		} else if ((comp = strcmp(ch, "New game")) == 0) {
			pick = NEW_GAME;
		} else if ((comp = strcmp(ch, "Load save")) == 0) {
			pick = LOAD_GAME;
		} else if ((comp = strcmp(ch, "Special")) == 0) {
			pick = SPECIAL;
		} else if ((comp = strcmp(ch, "Consumables")) == 0) {
			pick = CONSUMABLE;
		} else if ((comp = strcmp(ch, "Artifacts")) == 0) {
			pick = ARTIFACTS;
		} else if ((comp = strcmp(ch, "Equips")) == 0) {
			pick = EQUIPS;
		} else if ((comp = strcmp(ch, "Perks")) == 0) {
			pick = PERKS;
		} else if ((comp = strcmp(ch, "Stats")) == 0) {
			pick = STATS;
		} else if ((comp = strcmp(ch, "Save")) == 0) {
			pick = SAVE;
		} else if ((comp = strcmp(ch, "Debug")) == 0) {
			pick = DEBUG;
		} else if ((comp = strcmp(ch, "Quit")) == 0) {
			pick = QUIT;
		} else if ((comp = strcmp(ch, "Explore")) == 0) {
			pick = EXPLORE;
		} else if ((comp = strcmp(ch, "Tutorial")) == 0) {
			pick = TUTORIAL;
		} else if ((comp = strcmp(ch, "Close")) == 0) {
			pick = CLOSE_MENU;
		} else {
			pick = INVALID;
		}
	}

	sprintf(msg,"Pick was: (%i)",pick);
	log_tag("debug_log.txt","[TURNOPTION]",msg);

	if (pick == INVALID) {
		fprintf(stderr,"Error: unexpected turn choice value");
		log_tag("debug_log.txt","[ERROR]","Unexpected turn choice in getTurnChoice(), quitting");
		exit(EXIT_FAILURE);
	}
	return pick;
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
 */
void unlockSpecial(Fighter* f) {

	//Thanks to u/skeeto for the suggestions.
	ITEM *my_items[SPECIALSMAX+2] = {0};
	MENU *my_menu;
        WINDOW *my_menu_win;
	WINDOW *display_win;

        int n_choices = 0;
	int selection = -1;
	int currentIndexed = -1;
	ITEM* cur;

	/* Initialize curses */
	//setlocale(LC_CTYPE, "it_IT.UTF-8");
	//initscr();
	clear();
	refresh();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);


	init_game_color_pairs();

	/* Create menu items */
	for (int i = 0; i < SPECIALSMAX + 1; i++) {
		if (! (f->specials[i]->enabled) ) {
			my_items[n_choices++] = new_item(nameStringFromSpecial(f->class,i), "  ");
		}
	}

	/* Create menu */
	my_menu = new_menu(my_items);

	/* Set description off */
	menu_opts_off(my_menu,O_SHOWDESC);

	/* Create the window to be associated with the menu */
        my_menu_win = newwin(18, 28, 2, 2);
        keypad(my_menu_win, TRUE);

        /* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 12, 26, 4, 2));
	set_menu_format(my_menu, 12, 1);

	/* Set menu mark to the string " > " */
        set_menu_mark(my_menu, " > ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_label(my_menu_win, 1, 0, 20, "New move unlocked", COLOR_PAIR(3));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 26);
	mvwaddch(my_menu_win, 2, 27, ACS_RTEE);

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	//attron(COLOR_PAIR(2));
	//mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scoll down or up a page of items");
	mvprintw(23, 0, "Select a new special move to learn.");
	//attroff(COLOR_PAIR(2));
	//refresh();

	refresh();

	/* Prepare selection display window*/
	display_win = newwin(18, 40, 3, 32);
        box(display_win, 0, 0);
	//Update selected window
	cur = current_item(my_menu);
	for (int i = 0; i < SPECIALSMAX +1; i++) {
		int check = -1;
		if ((check = strcmp(nameStringFromSpecial(f->class,i), item_name(cur))) == 0 ) {
		currentIndexed = i;
		break;
		}
	}
	mvwprintw(display_win, 2, 2, "%s", descStringFromSpecial(f->class,f->specials[currentIndexed]->move));
	mvwprintw(display_win, 4, 2, "Energy Cost %i", f->specials[currentIndexed]->cost);
        wrefresh(my_menu_win);
        wrefresh(display_win);

	int picked = 0;
	int c;

	while(!picked && (c = wgetch(my_menu_win))) {
		switch(c) {
			case KEY_DOWN: {
				menu_driver(my_menu, REQ_DOWN_ITEM);
				cur = current_item(my_menu);
				//Update selected window
				for (int i = 0; i < SPECIALSMAX +1; i++) {
					int check = -1;
					if ((check = strcmp(nameStringFromSpecial(f->class,i), item_name(cur))) == 0 ) {
						currentIndexed = i;
						break;
					}
				}
				}
				break;
			case KEY_UP: {
			        menu_driver(my_menu, REQ_UP_ITEM);
				cur = current_item(my_menu);
				for (int i = 0; i < SPECIALSMAX +1; i++) {
					int check = -1;
					if ((check = strcmp(nameStringFromSpecial(f->class,i), item_name(cur))) == 0 ) {
						currentIndexed = i;
						break;
					}
				}
				}
			        break;
			case KEY_NPAGE: {
				menu_driver(my_menu, REQ_SCR_DPAGE);
				cur = current_item(my_menu);
				//Update selected window
				for (int i = 0; i < SPECIALSMAX +1; i++) {
					int check = -1;
					if ((check = strcmp(nameStringFromSpecial(f->class,i), item_name(cur))) == 0 ) {
						currentIndexed = i;
						break;
					}
				}
				}
				break;
			case KEY_PPAGE: {
			        menu_driver(my_menu, REQ_SCR_UPAGE);
				cur = current_item(my_menu);
				for (int i = 0; i < SPECIALSMAX +1; i++) {
					int check = -1;
					if ((check = strcmp(nameStringFromSpecial(f->class,i), item_name(cur))) == 0 ) {
						currentIndexed = i;
						break;
					}
				}
				}
			        break;
			case 10: { /*Enter*/
				picked = 1;
				cur = current_item(my_menu);
				for (int i = 0; i < SPECIALSMAX +1; i++) {
					int check = -1;
					if ((check = strcmp(nameStringFromSpecial(f->class,i), item_name(cur))) == 0 ) {
						selection = i;
						break;
				}

				pos_menu_cursor(my_menu);
				refresh();
				};
				break;
		        }
		}
		wclear(display_win);
		wrefresh(display_win);
        	box(display_win, 0, 0);
		mvwprintw(display_win, 2, 2, "%s", descStringFromSpecial(f->class,f->specials[currentIndexed]->move));
		mvwprintw(display_win, 4, 2, "Energy Cost %i", f->specials[currentIndexed]->cost);
                wrefresh(my_menu_win);
                wrefresh(display_win);
		refresh();
	}
	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        free_menu(my_menu);
	for(int k = 0; k <= n_choices; k++) {
		free_item(my_items[k]);
        }
	delwin(my_menu_win);
	delwin(display_win);
	endwin();

	int num = selection;

	if (num < SPECIALSMAX + 1) {	//Check if selected move number is lower than the maximum
		Specialslot* selected = f->specials[num];

		//Check if the selected move is NOT enabled
		if (! (selected->enabled) ) {
			//Enable the move
			selected->enabled = 1;
		}
	}
	f->stats->specialsunlocked += 1;
}

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

	log_tag("debug_log.txt","[LEVELUP]","Player leveled up.");

	/*
	lightCyan();
	printf("\n\n\tYour wounds were healed.");
	printf("\n\tYour energy is replenished.");
	white();
	*/
}

/**
 * Takes a Fighter pointer and the amount of xp to add.
 * Current level xp is managed by this function, including how much xp is needed to level up again. The values are stored in the fighter struct.
 * Thresholds for each level are checked and eventually onLevelUp() is called, with recursion on this function after.
 * @see Fighter
 * @see onLevelUp()
 * @param player The Fighter pointer that gets xp.
 * @param xp The amount of xp.
 */
void checkremainder(Fighter* player, int xp) {
	int curr = player->currentlevelxp;
	int tot = player->totallevelxp;

	//TODO: Print notifications to a passed WINDOW

	if (curr + xp >= tot) {
		player->totalxp += xp;
		//if (xp !=0) printf("\n\t%s obtained %i xp.", player->name, xp);
		player->level++;

		//cyan();
		//printf("\n\t%s reached Lvl. %i !", player->name, player->level);
		//white();

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
			//printf("\n\t%s obtained %i xp.", player->name, xp);
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
 */
void giveXp(Fighter* player, Enemy* e){


	int xp = getEnemyXpGain(e);

	checkremainder(player, xp);
}

/**
 * Takes a Fighter and a Boss pointers and handles xp gain by fighter.
 * @see Fighter
 * @see Boss
 * @see getBossXpGain()
 * @see checkremainder()
 * @param player The Fighter pointer that gets xp.
 * @param b The Boss pointer that gives xp.
 */
void giveXp_Boss(Fighter* player, Boss* b){


	int xp = getBossXpGain(b);

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
 * @param player The Fighter pointer to check the stats for.
 * @param force The integer to bypass all checks if true.
 */
void statReset(Fighter* player, int force) {
	char msg[200];
	sprintf(msg,"Call to statReset() with ($force) == (%i)", force);
	log_tag("debug_log.txt","[DEBUG]",msg);
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
			//yellow();
			//printf("\n\n\t%s's stats reset.\n",player->name);
			//white();
		}
	}
}

/**
 * Takes a Fighter pointer value and adds a random Consumable to consumablesBag.
 * @see Fighter
 * @see Consumable
 * @see consumableClass
 * @see CONSUMABLESMAX
 * @see stringFromConsumables()
 * @param player The Fighter pointer at hand.
 * @return int Returns the enum value of the drop as an integer.
 */
int dropConsumable(Fighter* player) {
	int drop = rand() % (CONSUMABLESMAX + 1);

	//Special drop chances. Maybe a function for this?
	if (drop == Powergem) {
		if ( rand() % 3 == 0 ) {
			drop = rand() % (CONSUMABLESMAX + 1);
		}
	}
	// Powergem has 33% chance to be rerolled

	Consumable* c = (Consumable*) player->consumablesBag[drop];
	c->qty++;

	//Update stats
	player->stats->consumablesfound++;

	return drop;
}

/**
 * Takes a Fighter pointer value and an integer indicating if the drop was from a beast enemy, and adds a random Equip to the fighter's equipsBag.
 * Prints notifications to the passed WINDOW pointer.
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
 * @param player The Fighter pointer at hand.
 * @param beast The integer for drops coming from a beast kill if true.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 */
void dropEquip(Fighter* player, int beast, WINDOW* notify_win, Koliseo* kls) {

	char msg[200];

	//Select a basic item from the list
	int drop = rand() % (EQUIPSMAX + 1);
	//Randomise quality
	quality q = rand() % (QUALITIESMAX + 1);

	//Prepare the item
	sprintf(msg,"Prepping dropped Equip");
	kls_log("DEBUG",msg);
	Equip* e = (Equip*) KLS_PUSH_NAMED(kls,Equip,1,"Equip",msg);

	//Get the base item and copy the stats to the drop
	Equip* base = &equips[drop];

	e->class = base->class;
	e->type = base->type;
	e->qual = q;

	setEquipSprite(e);
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

			sprintf(msg,"Prepping Perk (%i) for dropped Equip)", e->perksCount);
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			Perk* p = (Perk*) KLS_PUSH_NAMED(kls,Perk,1,"Perk",msg);
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

	//Calc cost value

	float cost = 5;

	cost += 2.5 * (e->qual);
	cost += 3.5 * (e->perksCount);

	cost += 7.2 * (e->level);

	e->cost = floor(cost);

	wattron(notify_win,COLOR_PAIR(6));
	sprintf(msg,"You found %s %s!",stringFromQuality(q),stringFromEquips(drop));
	display_notification(notify_win,msg,800);
	wattroff(notify_win,COLOR_PAIR(6));
	sprintf(msg,"Found Equip:    %s.", stringFromEquips(drop));
	log_tag("debug_log.txt","[DEBUG-DROPS]",msg);


	if (player->equipsBagOccupiedSlots >= EQUIPSBAGSIZE) { //Handle full bag by asking player if we throw something away
		//FIXME: The handling of full bag event is a mess as it does not support curses.
		lightRed();
		printf("\tYour bag is full. Want to throw something away?\n");
		white();
		log_tag("debug_log.txt","[DEBUG-EQUIPS]","Bag full, need to make space.\n");

		for (int i = 0; i < player->equipsBagOccupiedSlots ; i++) {
			Equip* current = (Equip*)player->equipsBag[i];
			if (current->equipped) { green(); printf("ON  "); white(); };

			printf("(%i",i); //This starts lines with the item index.
			printEquipStats(current);
		} ;

		int selected = -1;
		int c = -1;
		Equip* s = NULL;

		while (selected < 0 || selected >= player->equipsBagOccupiedSlots || c != 1) {

			c = scanf("%i",&selected);
			int res = scanf("%*c");
			sprintf(msg,"dropEquip() scanf() res was (%i)",res);
			log_tag("debug_log.txt","[DEBUG]",msg);

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
			int res = scanf("%*c");
			sprintf(msg,"dropEquip() scanf() res was (%i)",res);
			log_tag("debug_log.txt","[DEBUG]",msg);
		}

		if (n != 0) { //Abort deletion, drop will not be awared.

			/*
			int perkscount = e->perksCount;
			if (perkscount > 0) {
				for (int i=0; i < perkscount; i++) {
					free(e->perks[i]);
				}
			}
			free(e);
			*/
			log_tag("debug_log.txt","[DEBUG-EQUIPS]","Equip was not taken.\n");
			return;
		};

		Equip* toDelete = (Equip*) player->equipsBag[selected];
		int perkscount = toDelete->perksCount;
		/*
		if (perkscount > 0) {
			for (int i=0; i < perkscount; i++) {
				free(toDelete->perks[i]);
			}
		}
		*/
		sprintf(msg,"Equip %s (%i Perks) was taken by deleting %s.\n", stringFromEquips(e->class), perkscount, stringFromEquips(toDelete->class));
		log_tag("debug_log.txt","[DEBUG-EQUIPS]",msg);
		/*
		free(toDelete);
		*/

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
}

/**
 * Takes a Fighter pointer value and adds a random Artifact to artifactsBag.
 * The Artifact is randomised according to ARTIFACTSMAX until one which was not found yet drops.
 * Special Equip functions are also set up here.
 * @see Fighter
 * @see Artifact
 * @see artifactClass
 * @see stringFromArtifacts()
 * @param player The Fighter pointer at hand.
 * @return int Returns the enum value of the drop as an integer.
 */
int dropArtifact(Fighter* player) {
	int drop = 0;
	do {
		drop = rand() % (ARTIFACTSMAX + 1);
	} while (player->artifactsBag[drop]->qty != 0 ); //We reroll to get one we don't have

	player->artifactsBag[drop]->qty++;

	//Update stats
	player->stats->artifactsfound++;

	return drop;
}

/**
 * Takes a Fighter and a Enemy pointers and compares their stats to determine who gets damaged and returns the fightStatus value.
 * Prints notifications to the passed WINDOW pointer.
 * On enemy death, there's a chance to call dropConsumable, dropEquip or dropArtifact (guaranteed for beast enemies).
 * @see Fighter
 * @see Enemy
 * @see statReset()
 * @see statResetEnemy()
 * @see stringFromEClass()
 * @see dropConsumable()
 * @see dropEquip()
 * @see dropArtifact()
 * @param player The Fighter pointer at hand.
 * @param e The Enemy pointer at hand.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 * @see display_notification()
 */
int fight(Fighter* player, Enemy* e, WINDOW* notify_win, Koliseo* kls) {

	fightResult res = FIGHTRES_NO_DMG;
	char msg[200];
	//Stat comparisons
	//

	int atkdelta = (player->atk + player->equipboost_atk) - e->atk - (rand() % 3 ) - 1 ; //Skewed with defender
	int defdelta = (player->def + player->equipboost_def) - e->def + (rand() % 2 ) + 1  ; //Skewed with attacker
	int veldelta = (player->vel + player->equipboost_vel) - e->vel + (rand() % 3 ) + 1 ;

	int atkOnPlayer = e->atk - (player->def + player->equipboost_def + (player->vel / 6)) ;
	int atkOnEnemy = (player->atk + player->equipboost_atk)- (e->def + (e->vel / 6)) ;

	if (G_GODMODE_ON == 1) {
		log_tag("debug_log.txt","[DEBUG]","fight():  G_GODMODE_ON == 1");
		atkdelta = 100;
		defdelta = 100;
		veldelta = 100;
		atkOnPlayer = 1;
		atkOnEnemy = 100;
	}

	int damageDealt = -1;
	sprintf(msg,"atkdelta %i",atkdelta);
	log_tag("debug_log.txt","[DEBUG-FIGHT]",msg);
	sprintf(msg,"defdelta %i",defdelta);
	log_tag("debug_log.txt","[DEBUG-FIGHT]",msg);
	sprintf(msg,"veldelta %i",veldelta);
	log_tag("debug_log.txt","[DEBUG-FIGHT]",msg);
	sprintf(msg,"atkOnEnemy %i",atkOnEnemy);
	log_tag("debug_log.txt","[DEBUG-FIGHT]",msg);
	sprintf(msg,"atkOnPlayer %i\n",atkOnPlayer);
	log_tag("debug_log.txt","[DEBUG-FIGHT]",msg);

	if (veldelta >= 0) {

		if ( atkOnEnemy > 3 ) {
			damageDealt = atkOnEnemy;
			e->hp -= damageDealt > 0 ? damageDealt : 1;
			res = FIGHTRES_DMG_DEALT;
			log_tag("debug_log.txt","[FIGHT]","Fight result A WIN (faster, great atk).\n");
		} else if ( atkOnEnemy >= 0) {
			damageDealt = abs(atkOnEnemy - atkdelta);
			e->hp -= damageDealt > 0 ? damageDealt : 1;
			res = FIGHTRES_DMG_DEALT;
			log_tag("debug_log.txt","[FIGHT]","Fight result B WIN (faster, ok atk).\n");
		} else {
			if ( atkOnEnemy > -3 ) {
				damageDealt = fabsf(atkOnPlayer - 0.75F * (player->vel + player->equipboost_vel) );
			log_tag("debug_log.txt","[FIGHT]","Fight result C1 LOST (faster, atk > -3).\n");
			} else {
			       	damageDealt = abs(atkOnPlayer - 1 );
			log_tag("debug_log.txt","[FIGHT]","Fight result C2 LOST (faster, atk < -3).\n");
			}
			player->hp -= damageDealt > 0 ? damageDealt : 1;
			res = FIGHTRES_DMG_TAKEN;
		}
	} else {
		atkdelta = -atkdelta;
		if (atkOnPlayer > 3) {
			damageDealt = atkOnPlayer;
			player->hp -= damageDealt > 0 ? damageDealt : 1;
			res = FIGHTRES_DMG_TAKEN;
			log_tag("debug_log.txt","[FIGHT]","Fight result D LOST (slower, great enemy atk).\n");
		} else if ( atkOnPlayer >= 0 ) {
			damageDealt = abs( atkOnPlayer - atkdelta);
			player->hp -= damageDealt > 0 ? damageDealt : 1;
			res = FIGHTRES_DMG_TAKEN;
			log_tag("debug_log.txt","[FIGHT]","Fight result E LOST (slower, ok enemy atk).\n");
		} else {
			if ( atkOnPlayer > -3 ) {
				damageDealt = fabsf(atkOnEnemy - 0.75F * e->vel );
			log_tag("debug_log.txt","[FIGHT]","Fight result F1 WIN (slower, enemy atk > -3).\n");
			} else {
				damageDealt = abs (atkOnEnemy - 1 );
			log_tag("debug_log.txt","[FIGHT]","Fight result F2 WIN (slower, enemy atk < -3).\n");
			}
			e->hp -= damageDealt > 0 ? damageDealt : 1;
			res = FIGHTRES_DMG_DEALT;
		}
	}
	sprintf(msg,"damageCalc %i\n", damageDealt);
	log_tag("debug_log.txt","[FIGHT]",msg);

	int yourhit = (res == FIGHTRES_DMG_DEALT ) ? 1 : 0 ;
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
			int recovery = floor(damageDealt * (0.1 * vampire_perks));
			player->hp += recovery;
			sprintf(msg,"Vampirism proc for +%i HP.\n",recovery);
			log_tag("debug_log.txt","[PERKS]",msg);
			if (player->hp >= player->totalhp) { player->hp = player->totalhp;};
		}

		//Account for burn on touch perk
		int hotbody_perks = player->perks[HOT_BODY]->innerValue;
		if (hotbody_perks > 0) {
			int burnchance = 11 - hotbody_perks;
			if (rand() % burnchance == 0) {
				e->status = Burned; //Set status to Burned. May need change to manage multiple statuses active at once
				setCounter((Turncounter *)e->counters[Burned],2); //Give 2 turns of Burned status
				sprintf(msg,"Hotbody proc on 1/%i chance.\n",burnchance);
				log_tag("debug_log.txt","[PERKS]",msg);
			}
		}

		//Account for poison on touch perk. Order of checks with hot_body perk may cause issues?
		int biohazard_perks = player->perks[BIOHAZARD]->innerValue;
		if (biohazard_perks > 0) {
			int poisonchance = 11 - biohazard_perks;
			if (rand() % poisonchance == 0) {
				e->status = Poison; //Set status to Poison. May need change to manage multiple statuses active at once
				setCounter((Turncounter *)e->counters[POISON],2); //Give 2 turns of Poison status
				sprintf(msg,"Biohazard proc on 1/%i chance.\n",poisonchance);
				log_tag("debug_log.txt","[PERKS]",msg);
			}
		}

		//Check if someone earned a stat reset after the fight
		statResetEnemy(e,0);
		statReset(player,0);

		strcpy(victim,stringFromEClass(e->class));
	}

	int color = -1;
	if (yourhit) {
		color = 6;
	} else {
		color = 1;
	}

	sprintf(msg,"%s was hit.    (%i DMG)",victim,damageDealt > 0 ? damageDealt : 1);
	wattron(notify_win, COLOR_PAIR(color));
	display_notification(notify_win,msg,500);
	wattroff(notify_win, COLOR_PAIR(color));

	//Rolls
	//
	//Critical hit roll

	//Account for critboost_chance perks
	int critboost_value = 1.5 * player->perks[CRITBOOST_CHANCE]->innerValue;
	int critMax =  round(10.0 - floor(player->luck/5) - (critboost_value));

	int critRes = (rand() % critMax);

	if (res == FIGHTRES_DMG_DEALT && (critRes <= 0)) {

		//Account for critboost_dmg perks
		int dmgboost_perks = player->perks[CRITBOOST_DMG]->innerValue;
		damageDealt *= (0.30 + (0.12* dmgboost_perks));
		e->hp -= (damageDealt > 0 ? damageDealt : 1);
		sprintf(msg,"Critical hit for %i dmg, proc on 1/%i chance.", damageDealt, critMax);
		log_tag("debug_log.txt","[FIGHT]",msg);
		sprintf(msg,"Critical hit, critboost was %i.\n", critboost_value);
		log_tag("debug_log.txt","[PERKS]",msg);

		sprintf(msg,"A critical hit!    (%i DMG)",damageDealt > 0 ? damageDealt : 1);
		wattron(notify_win,COLOR_PAIR(8));
		display_notification(notify_win,msg,500);
		wattroff(notify_win,COLOR_PAIR(8));
		//Update stats
		player->stats->criticalhits++;
	}
	//Check for deaths -> exit condition from loop
	//
	//
	//
	if (e->hp <= 0 ) {
		res = FIGHTRES_KILL_DONE;

		//Account for runic circle perk
		int runic_perks = player->perks[RUNIC_MAGNET]->innerValue;
		if (runic_perks > 0) {
			int recovery = round(0.51 * runic_perks);
			player->energy += recovery;
			sprintf(msg,"Runicmagnet proc for %i energy.\n",recovery);
			log_tag("debug_log.txt","[PERKS]",msg);
		}
		if (e->beast) {
			color = 8;
		} else {
			color = 2;
		}
		wattron(notify_win,COLOR_PAIR(color));
		sprintf(msg, "%s fainted.",stringFromEClass(e->class));
		display_notification(notify_win,msg,500);
		wattroff(notify_win,COLOR_PAIR(color));

		sprintf(msg,"Killed  %s.", stringFromEClass(e->class));
		log_tag("debug_log.txt","[FIGHT]",msg);

		//Update stats
		player->stats->enemieskilled++;
	} else {
		//Apply status effects to enemy
		if (e->status != Normal) {
			applyEStatus(notify_win, e);
			sprintf(msg,"Applied  %s to %s.", stringFromStatus(e->status),stringFromEClass(e->class));
			log_tag("debug_log.txt","[STATUS]",msg);
		}
	}


	if (player->hp <= 0) {
		res = FIGHTRES_DEATH;
	} else {
		//Apply status effects to player
		if (player->status != Normal) {
			applyStatus(notify_win, player);
		}
	}

	//Consumable drop, guaranteed on killing a beast
	if (res == FIGHTRES_KILL_DONE && (e->beast || ( (rand() % 9)  - (player->luck/10)  <= 0 ))) {
		int drop = dropConsumable(player);
		sprintf(msg, "You found a %s!",stringFromConsumables(drop));
		wattron(notify_win,COLOR_PAIR(4));
		display_notification(notify_win,msg,500);
		wattroff(notify_win,COLOR_PAIR(4));
		sprintf(msg,"Found Consumable:    %s.", stringFromConsumables(drop));
		log_tag("debug_log.txt","[DROPS]",msg);
	}


	//Artifact drop (if we don't have all of them), guaranteed on killing a beast
	if ( (player->stats->artifactsfound != ARTIFACTSMAX + 1)  && res == FIGHTRES_KILL_DONE && (e->beast || ( (rand() % 1001)  - (player->luck/10)  <= 0 ))) {
		int artifact_drop = dropArtifact(player);
		sprintf(msg, "You found a %s!",stringFromArtifacts(artifact_drop));
		wattron(notify_win,COLOR_PAIR(8));
		display_notification(notify_win,msg,500);
		wattroff(notify_win,COLOR_PAIR(8));
		sprintf(msg,"Found Artifact:    %s.", stringFromArtifacts(artifact_drop));
		log_tag("debug_log.txt","[DROPS]",msg);
		if (!e->beast) 	log_tag("debug_log.txt","[.1%% CHANCE]","\nNORMAL ENEMY DROPPED ARTIFACT! 0.1%% chance??\n");
	}


	//Equip drop, guaranteed on killing a beast
	if (res == FIGHTRES_KILL_DONE && (e->beast || ( (rand() % 15)  - (player->luck/10)  <= 0 ))) {
		dropEquip(player,e->beast,notify_win,kls);
	}
	return res;
}

/**
 * Takes a Fighter, a Boss and a Path pointers and compares fighters stats to determine who gets damaged and returns the fightStatus value.
 * Prints notifications to the passed WINDOW pointer.
 * On boss death, we call dropConsumable, dropEquip and dropArtifact.
 * @see Fighter
 * @see Boss
 * @see statReset()
 * @see statResetBoss()
 * @see stringFromBossClass()
 * @see dropConsumable()
 * @see dropEquip()
 * @see dropArtifact()
 * @param player The Fighter pointer at hand.
 * @param b The Enemy pointer at hand.
 * @param p The Path pointer for the game.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 * @see display_notification()
 */
int boss_fight(Fighter* player, Boss* b, Path* p, WINDOW* notify_win, Koliseo* kls){

	fightResult res = FIGHTRES_NO_DMG;
	//Stat comparisons
	//
	char msg[200];

	int atkdelta = (player->atk + player->equipboost_atk) - b->atk - (rand() % 3 ) - 1 ; //Skewed with defender
	int defdelta = (player->def + player->equipboost_def) - b->def + (rand() % 2 ) + 1  ; //Skewed with attacker
	int veldelta = (player->vel + player->equipboost_vel) - b->vel + (rand() % 3 ) + 1 ;

	int atkOnPlayer = b->atk - (player->def + player->equipboost_def + (player->vel / 6)) ;
	int atkOnEnemy = (player->atk + player->equipboost_atk)- (b->def + (b->vel / 6)) ;
	if (G_GODMODE_ON == 1) {
		log_tag("debug_log.txt","[DEBUG]","boss_fight():  G_GODMODE_ON == 1");
		atkdelta = 100;
		defdelta = 100;
		veldelta = 100;
		atkOnPlayer = 1;
		atkOnEnemy = 100;
	}
	sprintf(msg,"atkdelta %i",atkdelta);
	log_tag("debug_log.txt","[FIGHT-BOSS]",msg);
	sprintf(msg,"defdelta %i",defdelta);
	log_tag("debug_log.txt","[FIGHT-BOSS]",msg);
	sprintf(msg,"veldelta %i",veldelta);
	log_tag("debug_log.txt","[FIGHT-BOSS]",msg);
	sprintf(msg,"atkOnEnemy %i",atkOnEnemy);
	log_tag("debug_log.txt","[FIGHT-BOSS]",msg);
	sprintf(msg,"atkOnPlayer %i\n",atkOnPlayer);
	log_tag("debug_log.txt","[FIGHT-BOSS]",msg);

	int damageDealt = -1;

	if (veldelta >= 0) {

		if ( atkOnEnemy > 3 ) {
			damageDealt = atkOnEnemy;
			b->hp -= damageDealt > 0 ? damageDealt : 1;
			res = FIGHTRES_DMG_DEALT;
			log_tag("debug_log.txt","[FIGHT]","Fight result A WIN (faster, great atk).\n");
		} else if ( atkOnEnemy >= 0) {
			damageDealt = abs(atkOnEnemy - atkdelta);
			b->hp -= damageDealt > 0 ? damageDealt : 1;
			res = FIGHTRES_DMG_DEALT;
			log_tag("debug_log.txt","[FIGHT]","Fight result B WIN (faster, ok atk).\n");
		} else {
			if ( atkOnEnemy < -3 ) {
				damageDealt = fabsf(atkOnPlayer - 0.75F * (player->vel + player->equipboost_vel) );
				log_tag("debug_log.txt","[FIGHT]","Fight result C1 LOST (faster, atk < -3).\n");
			} else {
			       	damageDealt = abs (atkOnPlayer - 1 );
				log_tag("debug_log.txt","[FIGHT]","Fight result C2 LOST (faster, atk > -3).\n");
			}
			player->hp -= damageDealt > 0 ? damageDealt : 1;
			res = FIGHTRES_DMG_TAKEN;
		}
	} else {
		atkdelta = -atkdelta;
		if (atkOnPlayer > 3) {
			damageDealt = atkOnPlayer;
			player->hp -= damageDealt > 0 ? damageDealt : 1;
			res = FIGHTRES_DMG_TAKEN;
			log_tag("debug_log.txt","[FIGHT]","Fight result D LOST (slower, great enemy atk.\n");
		} else if ( atkOnPlayer >= 0 ) {
			damageDealt = abs( atkOnPlayer - atkdelta);
			player->hp -= damageDealt > 0 ? damageDealt : 1;
			res = FIGHTRES_DMG_TAKEN;
			log_tag("debug_log.txt","[FIGHT]","Fight result E LOST (slower, ok enemy atk.\n");
		} else {
			if ( atkOnPlayer < -3 ) {
				damageDealt = fabsf(atkOnEnemy - 0.75F * b->vel );
				log_tag("debug_log.txt","[FIGHT]","Fight result F1 WIN (slower, enemy atk < -3.\n");
			} else {
				damageDealt = abs (atkOnEnemy - 1 );
				log_tag("debug_log.txt","[FIGHT]","Fight result F2 WIN (slower, enemy atk > -3.\n");
			}
			b->hp -= damageDealt > 0 ? damageDealt : 1;
			res = FIGHTRES_DMG_DEALT;
		}
	}

	sprintf(msg,"damageCalc %i\n", damageDealt);
	log_tag("debug_log.txt","[FIGHT]",msg);

	int yourhit = (res == FIGHTRES_DMG_DEALT ) ? 1 : 0 ;
	char victim[25];

	if (!yourhit) {

		b->vel--;
		b->atk--;
		b->def -= 2;

		//Check if someone earned a stat reset after the fight
		statReset(player,0);
		statResetBoss(b,0);

		strcpy(victim,player->name);
	} else {

		player->vel--;
		player->atk--;
		player->def -= 2;

		//Account for vampirism perk
		int vampire_perks = player->perks[VAMPIRISM]->innerValue;
		if (vampire_perks > 0) {
			int recovery = floor(damageDealt * (0.1 * vampire_perks));
			player->hp += recovery;
			sprintf(msg,"Vampirism proc for +%i HP.\n",recovery);
			log_tag("debug_log.txt","[PERKS]",msg);
			if (player->hp >= player->totalhp) { player->hp = player->totalhp;};
		}

		//Account for burn on touch perk
		int hotbody_perks = player->perks[HOT_BODY]->innerValue;
		if (hotbody_perks > 0) {
			int burnchance = 11 - hotbody_perks;
			if (rand() % burnchance == 0) {
				b->status = Burned; //Set status to Burned. May need change to manage multiple statuses active at once
				setCounter((Turncounter *)b->counters[Burned],2); //Give 2 turns of Burned status
				sprintf(msg,"Hotbody proc on 1/%i chance.\n",burnchance);
				log_tag("debug_log.txt","[PERKS]",msg);
			}
		}

		//Account for poison on touch perk. Order of checks with hot_body perk may cause issues?
		int biohazard_perks = player->perks[BIOHAZARD]->innerValue;
		if (biohazard_perks > 0) {
			int poisonchance = 11 - biohazard_perks;
			if (rand() % poisonchance == 0) {
				b->status = Poison; //Set status to Poison. May need change to manage multiple statuses active at once
				setCounter((Turncounter *)b->counters[POISON],2); //Give 2 turns of Poison status
				sprintf(msg,"Biohazard proc on 1/%i chance.\n",poisonchance);
				log_tag("debug_log.txt","[PERKS]",msg);
			}
		}

		//Check if someone earned a stat reset after the fight
		statResetBoss(b,0);
		statReset(player,0);

		strcpy(victim,stringFromBossClass(b->class));
	}

	int color = -1;
	if (yourhit) {
		color = 6;
	} else {
		color = 1;
	}

	wattron(notify_win, COLOR_PAIR(color));
	sprintf(msg,"%s was hit.    (%i DMG)", victim, damageDealt > 0 ? damageDealt : 1);
	display_notification(notify_win, msg, 500);
	wattroff(notify_win, COLOR_PAIR(color));

	//Rolls
	//
	//Critical hit roll

	//Account for critboost_chance perks
	int critMax =  round(10.0 - floor(player->luck/5) - (1.5* player->perks[CRITBOOST_CHANCE]->innerValue)  ) ;

	int critRes = (rand() % critMax);

	if (res == FIGHTRES_DMG_DEALT && (critRes <= 0)) {

		//Account for critboost_dmg perks
		int dmgboost_perks = player->perks[CRITBOOST_DMG]->innerValue;
		damageDealt *= (0.30 + (0.12* dmgboost_perks));
		b->hp -= damageDealt > 0 ? damageDealt : 1;
		wattron(notify_win,COLOR_PAIR(8));
		sprintf(msg,"A critical hit!    (%i DMG)", damageDealt > 0 ? damageDealt : 1);
		display_notification(notify_win,msg,500);
		wattroff(notify_win,COLOR_PAIR(8));
		sprintf(msg,"Critical hit for %i dmg, proc on 1/%i chance.\n", damageDealt, critMax);
		log_tag("debug_log.txt","[FIGHT-BOSS]",msg);

		//Update stats
		player->stats->criticalhits++;
	}
	//Check for deaths -> exit condition from loop
	//
	//
	//
	if (b->hp <= 0 ) {
		res = FIGHTRES_KILL_DONE;

		//Account for runic circle perk
		int runic_perks = player->perks[RUNIC_MAGNET]->innerValue;
		if (runic_perks > 0) {
			int recovery = round(0.51 * runic_perks);
			player->energy += recovery;
			sprintf(msg,"Runicmagnet proc for %i energy.\n",recovery);
			log_tag("debug_log.txt","[PERKS]",msg);
		}
		if (b->beast) {
			color = 8;
		} else {
			color = 2;
		}
		wattron(notify_win,COLOR_PAIR(color));
		sprintf(msg, "%s fainted.",stringFromBossClass(b->class));
		display_notification(notify_win, msg, 500);
		wattroff(notify_win,COLOR_PAIR(color));
		sprintf(msg,"Killed  %s.", stringFromBossClass(b->class));
		log_tag("debug_log.txt","[FIGHT-BOSS]",msg);

		//Update stats
		player->stats->bosseskilled++;

		//Check if we killed this boss before
		if (player->stats->killed_bosses[b->class] == 0) {
			//Update stats
			player->stats->unique_bosseskilled++;
			player->stats->killed_bosses[b->class] += 1;
			//Check if we have to update the wincon value
			if (p->win_condition->class == ALL_BOSSES) {
				p->win_condition->current_val++;
			}
			sprintf(msg,"Killed new boss  %s.\n", stringFromBossClass(b->class));
			log_tag("debug_log.txt","[FIGHT-BOSS]",msg);
		}
	} else {
		//Apply status effects to boss
		if (b->status != Normal) {
			applyBStatus(notify_win, b);
			sprintf(msg,"Applied  %s to %s.", stringFromStatus(b->status),stringFromBossClass(b->class));
			log_tag("debug_log.txt","[STATUS]",msg);
		}
	}


	if (player->hp <= 0) {
		res = FIGHTRES_DEATH;
	} else {
		//Apply status effects to player
		if (player->status != Normal) {
			applyStatus(notify_win, player);
		}
	}

	//Consumable drop, guaranteed on killing a beast
	if (res == FIGHTRES_KILL_DONE) {
		int drop = dropConsumable(player);
		wattron(notify_win,COLOR_PAIR(4));
		sprintf(msg, "You found a %s!",stringFromConsumables(drop));
		display_notification(notify_win,msg,500);
		wattroff(notify_win,COLOR_PAIR(4));
		sprintf(msg,"Found Consumable:    %s.", stringFromConsumables(drop));
		log_tag("debug_log.txt","[DROPS]",msg);
	}


	//Artifact drop (if we don't have all of them)
	if (res == FIGHTRES_KILL_DONE && (player->stats->artifactsfound != ARTIFACTSMAX + 1)) {
		int artifact_drop = dropArtifact(player);
		wattron(notify_win,COLOR_PAIR(8));
		sprintf(msg, "You found a %s!",stringFromArtifacts(artifact_drop));
		display_notification(notify_win, msg, 500);
		wattroff(notify_win,COLOR_PAIR(8));
		sprintf(msg,"Found Artifact:    %s.", stringFromArtifacts(artifact_drop));
		log_tag("debug_log.txt","[DROPS]",msg);
	}

	//Equip drop
	if (res == FIGHTRES_KILL_DONE) {
		//We give 1 to obtain the better equip generation used for beasts
		dropEquip(player,1,notify_win,kls);
	}

	return res;
}

/**
 * Takes a Fighter, an Enemy and a Boss pointers, a string denoting the consumableClass and an int for use on enemy or boss.
 * If qty value for the Consumable is 0, we have an early return. Otherise effect is applied and qty is decreased.
 * @see Fighter
 * @see Enemy
 * @see Boss
 * @see Consumable
 * @see consumableClass
 * @param f The Fighter pointer at hand.
 * @param e The Enemy pointer at hand.
 * @param b The Boss pointer at hand.
 * @param string The string value of consumable to use.
 * @param isBoss The mode of use (on boss if == 1)
 */
void useConsumable(Fighter* f, Enemy* e, Boss* b, char* string, int isBoss){
	//Mountain of ifs? Where are my damn maps

	int i=0;
	int check;
	for (int j=0; j < CONSUMABLESMAX+1; j++) {
		if ((check = strcmp(consumablestrings[j],string)) == 0) {
			i = j;
		}
	};

	Consumable* c = (Consumable*) f->consumablesBag[i];

	if (c->qty == 0) {
		return;
	}
	if (c->class == Potion) {
		f->hp=f->totalhp;
		//green();
		//printf("\n\n\n\tYour health was restored.\n\n");
		//white();
	} else if (c->class == Rock) {
		if (isBoss) {
			b->def -= 8;
			b->hp -= 2;
			//printf("\n\n\n\t%s got smacked.",stringFromBossClass(b->class));
		} else {
			e->def -= 8;
			e->hp -= 2;
			//printf("\n\n\n\t%s got smacked.",stringFromEClass(e->class));
		}
		//red();
		//printf("\t-8 DEF and -2 HP.\n\n");
		//white();
	} else if (c->class == Bread) {
		f->vel -= 1 ;
		f->def += 6 ;
		f->atk += 4;
		//printf("\n\n\n\tYou ate elvish bread.");
		//green();
		//printf("\t+6 DEF , +4 ATK, ");
		//yellow();
		//printf("-1 VEL\n\n");
		//white();
	} else if (c->class == Coffee) {
		f->vel += 6 ;
		f->atk += 2 ;
		f->def -= 2 ;
		//printf("\n\n\n\tYou drank hot coffee.");
		//green();
		//printf("\t+6 SPE , +2 ATK, ");
		//yellow();
		//printf("-2 DEF\n\n");
		//white();
	} else if (c->class == Powergem) {
		//printf("\n\n\n\tYou became wiser.");
		//green();
		//printf("\t+25 XP\n\n");
		//white();
		checkremainder(f,25);
	} else if (c->class == Magicbean) {
		//printf("\n\n\n\tYou aren't tired anymore.");
		//green();
		//printf("\tEnergy was restored\n\n");
		//white();
		f->energy = f->totalenergy; //Refill energy
	} else {
		return;
	}

	c->qty--;
}

/**
 * Takes a Fighter pointer and an integer denoting the consumableClass and returns the respective qty value from player's consumablesBag at the provided index.
 * @see Fighter
 * @see Consumable
 * @see consumableClass
 * @param f The Fighter pointer.
 * @param n The consumableClass value.
 * @return int The qty value in consumablesBag for selected consumableClass.
 */
int getConsumableQty(Fighter* f, int n) {
	Consumable* c = (Consumable*) f->consumablesBag[n];
	return c->qty;
}

/**
 * Sets the qty value to 0 for all Consumable in f's consumablesBag with positive qty.
 * @see Consumable
 * @see consumableClass
 * @see CONSUMABLESMAX
 * @see getConsumableQty()
 * @param f The Fighter pointer at hand.
 */
void emptyConsumables(Fighter* f) {
	for (int i=0; i < CONSUMABLESMAX + 1; i++) {
		if (getConsumableQty(f,i) > 0 ) {
			Consumable* c = (Consumable*) f->consumablesBag[i];
			c->qty = 0;
		}
	};
}

/**
 * Takes a Fighter pointer and sets the qty value to 0 and the active flag to false for all Artifacts in the fighter's artifactsBag with positive qty.
 * @see Artifact
 * @see artifactClass
 * @see ARTIFACTSMAX
 * @param f The Fighter pointer at hand.
 */
void emptyArtifacts(Fighter* f) {
	for (int i=0; i < ARTIFACTSMAX + 1; i++) {
		if (f->artifactsBag[i]->qty > 0 ) {
			f->artifactsBag[i]->qty = 0;
			f->artifactsBag[i]->active = 0;
		}
	};
}

/**
 * Takes a Fighter pointer and, for all Equip in equipsBag field with positive qty, sets qty to 0 and frees the Equip pointer.
 * @see Fighter
 * @see Equip
 * @param player The fighter pointer whose equipsbag will be emptied.
 */
void emptyEquips(Fighter* player) {
	//We REALLY need to be sure the items are in successive cells
	//Still thinking linked lists would be better than an array
	int occupied = player->equipsBagOccupiedSlots;

	for (int i=0; i < occupied ; i++) {

		Equip* e = (Equip*) player->equipsBag[i];
		if (e->qty > 0 ) {
			//free(e);
			e->qty = 0;
			log_tag("debug_log.txt","[FIXME]","emptyEquips():  how do I empty them semantically?");
		}
	};
}

/**
 * Takes a FILE pointer to which the gamestate from the passed args will be saved.
 * Fighter, Path and an integer for room index.
 * Saves the state of the game.
 * @see Fighter
 * @see Path
 * @see gameloop()
 * @param file The FILE pointer struct to write to.
 * @param f The player struct.
 * @param p The path struct.
 * @param roomIndex Current room index.
 */
OP_res handleSave_Home(FILE* file, Fighter* f, Path*p, int roomIndex) {
	const char version[] = "v0.1.5";
	//FILE *file = fopen("save.txt", "w");
	char msg[200];
	sprintf(msg,"Saving with version %s", version);
	log_tag("debug_log.txt","[DEBUG]",msg);

	if (file == NULL)
	{
		fprintf(stderr,"[ERROR]    Can't open save file!\n");
		return OP_CANNOT_OPEN_SAVEFILE;
	}

	/* File version printing */
	fprintf(file, "%s\n", version);

	/* Save type printing */
	fprintf(file, "%s\n", stringFrom_saveType(HOME_SAVE));

	/* Gamemode printing */
	fprintf(file, "%i# gamemode\n", GAMEMODE);

	/* Fighter printing */
	fprintf(file, "Fighter{\n");
	fprintf(file, "%s# name\n", f->name);
	fprintf(file, "%s# class\n", stringFromClass(f->class));
	fprintf(file, "%i# hp\n", f->hp);
	fprintf(file, "%i# totalhp\n", f->totalhp);
	fprintf(file, "%i# atk\n", f->atk);
	fprintf(file, "%i# def\n", f->def);
	fprintf(file, "%i# vel\n", f->vel);
	fprintf(file, "%i# level\n", f->level);
	fprintf(file, "%i# luck\n", f->luck);
	fprintf(file, "%i# totxp\n", f->totalxp);
	fprintf(file, "%i# currlevxp\n", f->currentlevelxp);
	fprintf(file, "%i# totlevxp\n", f->totallevelxp);
	fprintf(file, "%i# energy\n", f->energy);
	fprintf(file, "%i# totenergy\n", f->totalenergy);
	fprintf(file, "%i# coinbalance\n", f->balance);
	fprintf(file, "%s# status\n", stringFromStatus(f->status));
	fprintf(file, "    Specials{\n");
	for (int i = 0; i < SPECIALSMAX +1; i++) {
		fprintf(file, "%i# %i_special_enabled_flag\n",f->specials[i]->enabled, i);
	}
	fprintf(file, "    },\n");
	fprintf(file, "    Counters{\n");
	for (int i = 0; i < COUNTERSMAX +1; i++) {
		fprintf(file, "%i# innervalue\n",f->counters[i]->innerValue);
		fprintf(file, "%i# count\n",f->counters[i]->count);
	}
	fprintf(file, "    },\n");
	fprintf(file, "%i# tb_atk\n",f->turnboost_atk);
	fprintf(file, "%i# tb_def\n",f->turnboost_def);
	fprintf(file, "%i# tb_vel\n",f->turnboost_vel);
	fprintf(file, "%i# tb_enr\n",f->turnboost_enr);
	fprintf(file, "%i# eqpbag_occupied_slots\n", f->equipsBagOccupiedSlots);
	fprintf(file, "%i# earliestbagslot\n", f->earliestBagSlot);
	fprintf(file, "    EquipsBag{\n");
	for (int i = 0; i < f->equipsBagOccupiedSlots; i++) {
		Equip* eq = f->equipsBag[i];
		fprintf(file, "        Equip_%i{\n",i);

		fprintf(file, "%s# equipclass\n",stringFromEquips(eq->class));
		fprintf(file, "%s# equipzone\n",stringFromEquipzones(eq->type));
		fprintf(file, "%i# qty\n",eq->qty);
		fprintf(file, "%i# equipped\n",eq->equipped);
		fprintf(file, "%i# lvl\n",eq->level);
		fprintf(file, "%i# atk\n",eq->atk);
		fprintf(file, "%i# def\n",eq->def);
		fprintf(file, "%i# vel\n",eq->vel);
		fprintf(file, "%i# enr\n",eq->enr);
		fprintf(file, "%i# bonus\n",eq->bonus);
		fprintf(file, "%i# perkscount\n",eq->perksCount);
		fprintf(file, "%i# cost\n",eq->cost);
		fprintf(file, "%s# quality\n",stringFromQuality(eq->type));
		fprintf(file, "            Perks{\n");
		for (int j = 0; j < eq->perksCount; j++) {
			Perk* pk = eq->perks[j];
			fprintf(file, "%i# class\n",pk->class);
			fprintf(file, "%i# innervalue\n",pk->innerValue);
		}
		fprintf(file, "            }\n");
		fprintf(file, "        }\n");
	}
	fprintf(file, "    }\n");
	fprintf(file, "%i# pbst_atk\n", f->permboost_atk);
	fprintf(file, "%i# pbst_def\n", f->permboost_def);
	fprintf(file, "%i# pbst_vel\n", f->permboost_vel);
	fprintf(file, "%i# pbst_enr\n", f->permboost_enr);
	fprintf(file, "%i# ebst_atk\n", f->equipboost_atk);
	fprintf(file, "%i# ebst_atk\n", f->equipboost_def);
	fprintf(file, "%i# ebst_atk\n", f->equipboost_vel);
	fprintf(file, "%i# ebst_atk\n", f->equipboost_enr);
	fprintf(file, "    Stats{\n");
	fprintf(file, "%i# enemieskilled\n", f->stats->enemieskilled);
	fprintf(file, "%i# consumablesfound\n", f->stats->consumablesfound);
	fprintf(file, "%i# equipsfound\n", f->stats->equipsfound);
	fprintf(file, "%i# artifactsfound\n", f->stats->artifactsfound);
	fprintf(file, "%i# crits\n", f->stats->criticalhits);
	fprintf(file, "%i# roomsdone\n", f->stats->roomscompleted);
	fprintf(file, "%i# specialsunlocked\n", f->stats->specialsunlocked);
	fprintf(file, "%i# coinsfound\n", f->stats->coinsfound);
	fprintf(file, "%i# bosseskilled\n", f->stats->bosseskilled);
	fprintf(file, "%i# uniquebosseskilled\n", f->stats->unique_bosseskilled);
	fprintf(file, "        Bosses_killed{\n");
	for (int b_i = 0; b_i < BOSSCLASSESMAX+1; b_i++) {
	fprintf(file, "%i# boss_%iwas_%s_killed\n", f->stats->killed_bosses[b_i], b_i, f->stats->killed_bosses[b_i] ? "" : "not");
	}
	fprintf(file, "        },\n");
	fprintf(file, "%i# keysfound\n", f->stats->keysfound);
	fprintf(file, "    },\n");
	fprintf(file, "},\n");

	/* Path and current room indexes printing */
	fprintf(file, "Path{\n");
	fprintf(file, "%i# luck\n", p->luck);
	fprintf(file, "%i# length\n", p->length);
	fprintf(file, "%i# prize\n", p->prize);
	fprintf(file, "%i# lorecounter\n", (p->loreCounter)-1); //We want to see the last message again when the game is loaded. This might need investigation. TODO
	fprintf(file, "    Wincon{\n");
	fprintf(file, "%i# winconclass\n", p->win_condition->class);
	fprintf(file, "%i# current_val\n", p->win_condition->current_val);
	fprintf(file, "%i# target_val\n", p->win_condition->target_val);
	fprintf(file, "    },\n");
	fprintf(file, "},\n");
	fprintf(file, "%i# roomindex\n", roomIndex);

	/* Consumables bag printing */
	fprintf(file, "ConsumableBag{\n");
	for (int i = 0; i < CONSUMABLESMAX +1; i++) {
		Consumable * cn = (Consumable*) f->consumablesBag[i];
		fprintf(file, "%i# qty_for_consumable_%i\n",cn->qty,i);
	}
	fprintf(file, "},\n");

	/* Artifacts bag printing */
	fprintf(file, "ArtifactsBag{\n");
	for (int i = 0; i < ARTIFACTSMAX +1; i++) {
		Artifact * a = f->artifactsBag[i];
		fprintf(file, "%i# qty_for_artifact_%i\n",a->qty,i);
	}
	fprintf(file, "},\n");

	log_tag("debug_log.txt","[DEBUG]","Done saving");

	return OP_OK;
}

/**
 * Takes a FILE pointer to which the gamestate from the passed args will be saved.
 * Fighter, Path and Enemy pointers, and integers for room / path progression indexes.
 * Saves the state of the game.
 * @see Fighter
 * @see Enemy
 * @see Path
 * @see room()
 * @see gameloop()
 * @param file The FILE pointer struct to write to.
 * @param f The player struct.
 * @param p The path struct.
 * @param e The current enemy struct.
 * @param enemyIndex Index in current room for current enemy.
 * @param roomTotalEnemies Total enemies for current room.
 * @param roomIndex Current room index.
 */
OP_res handleSave_Enemies(FILE* file, Fighter* f, Path*p, Enemy* e, int enemyIndex, int roomTotalEnemies, int roomIndex) {
	const char version[] = "v0.1.5";
	//FILE *file = fopen("save.txt", "w");
	char msg[200];
	sprintf(msg,"Saving with version %s", version);
	log_tag("debug_log.txt","[DEBUG]",msg);

	if (file == NULL)
	{
		fprintf(stderr,"[ERROR]    Can't open save file!\n");
		return OP_CANNOT_OPEN_SAVEFILE;
	}

	/* File version printing */
	fprintf(file, "%s\n", version);

	/* Save type printing */
	fprintf(file, "%s\n", stringFrom_saveType(ENEMIES_SAVE));

	/* Gamemode printing */
	fprintf(file, "%i# gamemode\n", GAMEMODE);

	/* Fighter printing */
	fprintf(file, "Fighter{\n");
	fprintf(file, "%s# name\n", f->name);
	fprintf(file, "%s# class\n", stringFromClass(f->class));
	fprintf(file, "%i# hp\n", f->hp);
	fprintf(file, "%i# totalhp\n", f->totalhp);
	fprintf(file, "%i# atk\n", f->atk);
	fprintf(file, "%i# def\n", f->def);
	fprintf(file, "%i# vel\n", f->vel);
	fprintf(file, "%i# level\n", f->level);
	fprintf(file, "%i# luck\n", f->luck);
	fprintf(file, "%i# totxp\n", f->totalxp);
	fprintf(file, "%i# currlevxp\n", f->currentlevelxp);
	fprintf(file, "%i# totlevxp\n", f->totallevelxp);
	fprintf(file, "%i# energy\n", f->energy);
	fprintf(file, "%i# totenergy\n", f->totalenergy);
	fprintf(file, "%i# coinbalance\n", f->balance);
	fprintf(file, "%s# status\n", stringFromStatus(f->status));
	fprintf(file, "    Specials{\n");
	for (int i = 0; i < SPECIALSMAX +1; i++) {
		fprintf(file, "%i# %i_special_enabled_flag\n",f->specials[i]->enabled, i);
	}
	fprintf(file, "    },\n");
	fprintf(file, "    Counters{\n");
	for (int i = 0; i < COUNTERSMAX +1; i++) {
		fprintf(file, "%i# innervalue\n",f->counters[i]->innerValue);
		fprintf(file, "%i# count\n",f->counters[i]->count);
	}
	fprintf(file, "    },\n");
	fprintf(file, "%i# tb_atk\n",f->turnboost_atk);
	fprintf(file, "%i# tb_def\n",f->turnboost_def);
	fprintf(file, "%i# tb_vel\n",f->turnboost_vel);
	fprintf(file, "%i# tb_enr\n",f->turnboost_enr);
	fprintf(file, "%i# eqpbag_occupied_slots\n", f->equipsBagOccupiedSlots);
	fprintf(file, "%i# earliestbagslot\n", f->earliestBagSlot);
	fprintf(file, "    EquipsBag{\n");
	for (int i = 0; i < f->equipsBagOccupiedSlots; i++) {
		Equip* eq = f->equipsBag[i];
		fprintf(file, "        Equip_%i{\n",i);

		fprintf(file, "%s# equipclass\n",stringFromEquips(eq->class));
		fprintf(file, "%s# equipzone\n",stringFromEquipzones(eq->type));
		fprintf(file, "%i# qty\n",eq->qty);
		fprintf(file, "%i# equipped\n",eq->equipped);
		fprintf(file, "%i# lvl\n",eq->level);
		fprintf(file, "%i# atk\n",eq->atk);
		fprintf(file, "%i# def\n",eq->def);
		fprintf(file, "%i# vel\n",eq->vel);
		fprintf(file, "%i# enr\n",eq->enr);
		fprintf(file, "%i# bonus\n",eq->bonus);
		fprintf(file, "%i# perkscount\n",eq->perksCount);
		fprintf(file, "%i# cost\n",eq->cost);
		fprintf(file, "%s# quality\n",stringFromQuality(eq->type));
		fprintf(file, "            Perks{\n");
		for (int j = 0; j < eq->perksCount; j++) {
			Perk* pk = eq->perks[j];
			fprintf(file, "%i# class\n",pk->class);
			fprintf(file, "%i# innervalue\n",pk->innerValue);
		}
		fprintf(file, "            }\n");
		fprintf(file, "        }\n");
	}
	fprintf(file, "    }\n");
	fprintf(file, "%i# pbst_atk\n", f->permboost_atk);
	fprintf(file, "%i# pbst_def\n", f->permboost_def);
	fprintf(file, "%i# pbst_vel\n", f->permboost_vel);
	fprintf(file, "%i# pbst_enr\n", f->permboost_enr);
	fprintf(file, "%i# ebst_atk\n", f->equipboost_atk);
	fprintf(file, "%i# ebst_atk\n", f->equipboost_def);
	fprintf(file, "%i# ebst_atk\n", f->equipboost_vel);
	fprintf(file, "%i# ebst_atk\n", f->equipboost_enr);
	fprintf(file, "    Stats{\n");
	fprintf(file, "%i# enemieskilled\n", f->stats->enemieskilled);
	fprintf(file, "%i# consumablesfound\n", f->stats->consumablesfound);
	fprintf(file, "%i# equipsfound\n", f->stats->equipsfound);
	fprintf(file, "%i# artifactsfound\n", f->stats->artifactsfound);
	fprintf(file, "%i# crits\n", f->stats->criticalhits);
	fprintf(file, "%i# roomsdone\n", f->stats->roomscompleted);
	fprintf(file, "%i# specialsunlocked\n", f->stats->specialsunlocked);
	fprintf(file, "%i# coinsfound\n", f->stats->coinsfound);
	fprintf(file, "%i# bosseskilled\n", f->stats->bosseskilled);
	fprintf(file, "%i# uniquebosseskilled\n", f->stats->unique_bosseskilled);
	fprintf(file, "        Bosses_killed{\n");
	for (int b_i = 0; b_i < BOSSCLASSESMAX+1; b_i++) {
	fprintf(file, "%i# boss_%iwas_%s_killed\n", f->stats->killed_bosses[b_i], b_i, f->stats->killed_bosses[b_i] ? "" : "not");
	}
	fprintf(file, "        },\n");
	fprintf(file, "%i# keysfound\n", f->stats->keysfound);
	fprintf(file, "    },\n");
	fprintf(file, "},\n");

	/* Current enemy printing */
	fprintf(file, "Enemy{\n");
	fprintf(file, "%s# class\n", stringFromEClass(e->class));
	fprintf(file, "%i# hp\n", e->hp);
	fprintf(file, "%i# totalhp\n", e->totalhp);
	fprintf(file, "%i# atk\n", e->atk);
	fprintf(file, "%i# def\n", e->def);
	fprintf(file, "%i# vel\n", e->vel);
	fprintf(file, "%i# level\n", e->level);
	fprintf(file, "%i# xp\n", e->xp);
	fprintf(file, "%i# energy\n", e->energy);
	fprintf(file, "%i# totenergy\n", e->totalenergy);
	fprintf(file, "%i# beast\n", e->beast);
	fprintf(file, "%i# prize\n", e->prize);
	fprintf(file, "%s# status\n", stringFromStatus(e->status));
	fprintf(file, "    Counters{\n");
	for (int i = 0; i < COUNTERSMAX +1; i++) {
		fprintf(file, "%i# innervalue\n",e->counters[i]->innerValue);
		fprintf(file, "%i# count\n",e->counters[i]->count);
	}
	fprintf(file, "    },\n");
	fprintf(file, "},\n");

	/* Path and current room indexes printing */
	fprintf(file, "Path{\n");
	fprintf(file, "%i# luck\n", p->luck);
	fprintf(file, "%i# length\n", p->length);
	fprintf(file, "%i# prize\n", p->prize);
	fprintf(file, "%i# lorecounter\n", (p->loreCounter)-1); //We want to see the last message again when the game is loaded. This might need investigation. TODO
	fprintf(file, "    Wincon{\n");
	fprintf(file, "%i# winconclass\n", p->win_condition->class);
	fprintf(file, "%i# current_val\n", p->win_condition->current_val);
	fprintf(file, "%i# target_val\n", p->win_condition->target_val);
	fprintf(file, "    },\n");
	fprintf(file, "},\n");
	fprintf(file, "%i# enemyindex\n", enemyIndex);
	fprintf(file, "%i# roomtotalenemies\n", roomTotalEnemies);
	fprintf(file, "%i# roomindex\n", roomIndex);

	/* Consumables bag printing */
	fprintf(file, "ConsumableBag{\n");
	for (int i = 0; i < CONSUMABLESMAX +1; i++) {
		Consumable * cn = (Consumable*) f->consumablesBag[i];
		fprintf(file, "%i# qty_for_consumable_%i\n",cn->qty,i);
	}
	fprintf(file, "},\n");

	/* Artifacts bag printing */
	fprintf(file, "ArtifactsBag{\n");
	for (int i = 0; i < ARTIFACTSMAX +1; i++) {
		Artifact * a = f->artifactsBag[i];
		fprintf(file, "%i# qty_for_artifact_%i\n",a->qty,i);
	}
	fprintf(file, "},\n");

	log_tag("debug_log.txt","[DEBUG]","Done saving");

	return OP_OK;
}

/**
 * Takes a FILE pointer from which the save type will be loaded.
 * Returns the read savetype value or -1 if an invalid value is found.
 * @see saveType
 * @see gameloop()
 * @param file The FILE pointer to load from.
 * @return The saveType read from the file.
 */
saveType read_saveType(FILE* file) {
	log_tag("debug_log.txt","[DEBUG]","Reading save type from text file.");

	if (file == NULL)
	{
		endwin();
		printf("Error with file while trying to read save type!\n");
		return -1;
	}
	char buf[500];
	const char version[] = "v0.1.5";

	char msg[1000];

	int scanres = -1;
	/* File version scanning */
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in read_saveType(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while reading save type.");
		exit(EXIT_FAILURE);
	}

	int check = -1;
	if (!((check = strcmp(buf,version)) == 0)) {
		sprintf(msg,"Failed save format version check. Quitting.\n");
		log_tag("debug_log.txt","[LOAD-ERROR]",msg);
		fprintf(stderr,"[ERROR]    File version mismatch on save type reading.\n");
		exit(EXIT_FAILURE);
	};
	sprintf(msg,"Read save format version: (%s).\n",buf);
	log_tag("debug_log.txt","[LOAD]",msg);

	/* Save type scanning */
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in read_saveType(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while reading save type.");
		exit(EXIT_FAILURE);
	}


	saveType result = saveTypeFrom_string(buf);

	if (result == -1) {
		sprintf(msg,"read_saveType():  Failed save type reading. Read string Was [%s]. Quitting.\n",buf);
		log_tag("debug_log.txt","[LOAD-ERROR]",msg);
		fprintf(stderr,"[ERROR]    Save type version error.\n");
		exit(EXIT_FAILURE);

	};

	sprintf(msg,"Loaded save type: (%s) --> [%s].\n", buf, stringFrom_saveType(result));
	log_tag("debug_log.txt","[LOAD]",msg);

	log_tag("debug_log.txt","[LOAD]","Doing rewind(save_file) before returning from read_saveType()!");
	rewind(file);
	return result;
}

/**
 * Takes a FILE pointer from which the gamestate will be loaded.
 * Gets the pointers to relevant game structures, sets all the values by reading the save file.
 * Fighter, Path pointers, and an integer for room index.
 * Saves the state of the game.
 * @see Fighter
 * @see Path
 * @see gameloop()
 * @param file The FILE pointer to load from.
 * @param f The player struct.
 * @param p The path struct.
 * @param roomIndex Current room index.
 * @param done_loading Flag to check if loading ended.
 * @param kls The Koliseo used for allocations.
 * @return The OP_res corresponding to loading result.
 */
OP_res handleLoadgame_Home(FILE* file, Fighter* f, Path* p, int* roomIndex, int* done_loading, Koliseo* kls) {

	//printf("\nLoading game...\n");
	log_tag("debug_log.txt","[LOAD]","Starting loading from text file.");

//	FILE* file = fopen("save.txt", "r");
	if (file == NULL)
	{
		printf("Error with file while trying to load!\n");
		return OP_CANNOT_OPEN_SAVEFILE;
	}
	char buf[500];
	char comment[300];
	int num_value = -1;
	const char version[] = "v0.1.5";

	char msg[1000];

	int scanres = -1;
	/* File version scanning */
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	int check = -1;
	if (!((check = strcmp(buf,version)) == 0)) {
		sprintf(msg,"Failed save format version check. Was [%s]. Quitting.\n", buf);
		log_tag("debug_log.txt","[LOAD-ERROR]",msg);
		fprintf(stderr,"[ERROR]    File version mismatch on load.\n");
		exit(EXIT_FAILURE);
	};
	sprintf(msg,"Loaded save format version: (%s).\n",buf);
	log_tag("debug_log.txt","[LOAD]",msg);

	/* Save type scanning */
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	check = -1;
	if (!((check = strcmp(buf,stringFrom_saveType(HOME_SAVE))) == 0)) {
		sprintf(msg,"handleLoadGame_Home():  Failed save type check, was [%s]. Quitting.\n",buf);
		log_tag("debug_log.txt","[LOAD-ERROR]",msg);
		fprintf(stderr,"[ERROR]    Save type version mismatch on load.\n");
		exit(EXIT_FAILURE);
	};
	sprintf(msg,"Loaded save type: (%s).\n",buf);
	log_tag("debug_log.txt","[LOAD]",msg);

	/* Gamemode scanning */
	scanres = fscanf(file, "%200[^#]# %s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.\n", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	GAMEMODE = num_value;

	/* Fighter scanning */
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	strcpy(f->name,buf);


	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	for (int i = 0; i < CLASSESMAX +1; i++) {
		int check = -1;
		if ((check = strcmp(buf, stringFromClass(i))) == 0) {
			f->class = i;
			break;
		}
	};

	//We have to reload the specials callbacks after setting fighter class
	setSpecials(f,kls);

	//HP
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->hp = num_value;
	//TotalHP
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->totalhp = num_value;
	//Atk
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->atk = num_value;
	//Def
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	sscanf(buf, "%3i", &num_value);
	f->def = num_value;
	//Vel
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->vel = num_value;
	//Level
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->level = num_value;
	//Luck
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->luck = num_value;
	//Totxp
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	f->totalxp = num_value;
	//Current level xp
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	f->currentlevelxp = num_value;
	//Totallevelxp
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	f->totallevelxp = num_value;
	//Energy
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->energy = num_value;
	//Totenergy
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->totalenergy = num_value;
	//Coin balance
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	f->balance = num_value;

	//Status
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	for (int i = 0; i < STATUSMAX +1; i++) {
		int check = -1;
		if ((check = strcmp(buf,stringFromStatus(i))) == 0) {
			f->status = i;
			break;
		}
	};
	//Specials
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < SPECIALSMAX +1; i++) {
		//Enabled flag
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		f->specials[i]->enabled = num_value;
	}
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	//Fighter counters
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < COUNTERSMAX +1; i++) {
		//innerValue
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		f->counters[i]->innerValue = num_value;
		//count
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		f->counters[i]->count = num_value;
	}
	scanres = fscanf(file, "%200s\n", buf);

	//turnboost_atk
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->turnboost_atk = num_value;
	//turnboost_def
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->turnboost_def = num_value;
	//turnboost_vel
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->turnboost_vel = num_value;
	//turnboost_enr
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->turnboost_enr = num_value;

	//equipsbag occupied slots
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->equipsBagOccupiedSlots = num_value;
	//equipsbag earliest slot
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->earliestBagSlot = num_value;

	//Equipsbag
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < f->equipsBagOccupiedSlots; i++) {
		sprintf(msg,"Loadgame_Home(): Prepping Equip (%i)",i);
		log_tag("debug_log.txt","[DEBUG]",msg);
		kls_log("DEBUG",msg);
		Equip* eq = KLS_PUSH_NAMED(kls,Equip,1,"Equip",msg);
		scanres = fscanf(file, "%200s\n", buf); //Skip equip_i heading
		if (scanres != 1) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}


		//equipClass
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		for (int j = 0; j < EQUIPSMAX +1; j++) {
			int check = -1;
			if ((check = strcmp(buf,stringFromEquips(j))) == 0) {
				eq->class = j;
				break;
			}
		};
		//equipzoneClass
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		for (int j = 0; j < EQUIPZONES +1; j++) {
			int check = -1;
			if ((check = strcmp(buf,stringFromEquipzones(j))) == 0) {
				eq->type = j;
				break;
			}
		};

		//qty
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->qty = num_value;
		//equipped
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->equipped = num_value;
		//level
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->level = num_value;
		//atk
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->atk = num_value;
		//def
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->def = num_value;
		//vel
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->vel = num_value;
		//enr
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->enr = num_value;
		//bonus
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->bonus = num_value;
		//perksCount
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->perksCount = num_value;
		//cost
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%8i", &num_value);
		eq->cost = num_value;

		//Quality
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		for (int j = 0; j < QUALITIESMAX +1; j++) {
			int check = -1;
			if ((check = strcmp(buf,stringFromQuality(j))) == 0) {
				eq->qual = j;
				break;
			}
		};

		//perks
		scanres = fscanf(file, "%200s\n", buf); //Skip perks heading
		if (scanres != 1) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		for (int j = 0; j < eq->perksCount ; j++) {
			sprintf(msg,"Loadgame_Home(): Prepping Perk (%i)",j);
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			Perk* pk = (Perk*) KLS_PUSH_NAMED(kls,Perk,1,"Perk",msg);
			//perkClass
			scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
			if (scanres != 2) {
				sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
				log_tag("debug_log.txt","[DEBUG]",msg);
				fprintf(stderr,"Error while loading game.");
				exit(EXIT_FAILURE);
			}

			sprintf(msg,"Loaded %s: %s.", comment, buf);
			log_tag("debug_log.txt","[LOAD]",msg);
			sscanf(buf, "%3i", &num_value);
			pk->class = num_value;
			char* name = nameStringFromPerk(pk->class);
			char* desc = descStringFromPerk(pk->class);
			//p->name = name; //(char*)malloc(sizeof(name));
			strcpy(pk->name,name);
			//p->desc = (char*)malloc(sizeof(desc));
			strcpy(pk->desc,desc);
			pk->innerValue = 0;
			eq->perks[j] = pk;

			//innerValue
			scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
			if (scanres != 2) {
				sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
				log_tag("debug_log.txt","[DEBUG]",msg);
				fprintf(stderr,"Error while loading game.");
				exit(EXIT_FAILURE);
			}

			sprintf(msg,"Loaded %s: %s.", comment, buf);
			log_tag("debug_log.txt","[LOAD]",msg);
			sscanf(buf, "%3i", &num_value);
			pk->innerValue = num_value;
		}

		scanres = fscanf(file, "%200s\n", buf); //Skip closing perks breacket
		if (scanres != 1) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		scanres = fscanf(file, "%200s\n", buf); //Skip closing current equip bracket
		if (scanres != 1) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		//Get the base item and copy the stats to the loading item
		Equip* base = &equips[eq->class];
		setEquipSprite(eq);
		strcpy(eq->name, base->name);
		strcpy(eq->desc, base->desc);

		if (eq->equipped) {
			Equipslot* slot = f->equipslots[eq->type];
			//We equip the item
			slot->item = eq;
			applyEquipPerks(eq, f);
			slot->active = 1;
		}
		f->equipsBag[i] = eq;
	} //End for Equips bag

	scanres = fscanf(file,"%200s\n", buf); //Skip closing equipsbag bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}


	//Fighter boosts
	//permboost_atk
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->permboost_atk = num_value;
	//permboost_def
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->permboost_def = num_value;
	//permboost_vel
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->permboost_vel = num_value;
	//permboost_enr
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->permboost_enr = num_value;
	//equipboost_atk
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->equipboost_atk = num_value;
	//equipboost_def
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->equipboost_def = num_value;
	//equipboost_vel
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->equipboost_vel = num_value;
	//equipboost_enr
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->equipboost_enr = num_value;

	//Stats
	scanres = fscanf(file, "%200s\n", buf); //Skip Stats bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	//enemieskilled
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->enemieskilled = num_value;
	//consumablesfound
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->consumablesfound = num_value;
	//equipsfound
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->equipsfound = num_value;
	//artifactsfound
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->artifactsfound = num_value;
	//crits
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->criticalhits = num_value;
	//roomsdone
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->roomscompleted = num_value;
	//specials unlocked
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->specialsunlocked = num_value;
	//coinsfound
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	f->stats->coinsfound = num_value;
	//bosseskilled
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->bosseskilled = num_value;

	//Unique boss kills
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->unique_bosseskilled = num_value;

	scanres = fscanf(file, "%200s\n", buf); //Skip unique bosses bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	for (int boss_index = 0; boss_index < BOSSCLASSESMAX +1; boss_index++) {
		//boss %i was killed
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		f->stats->killed_bosses[boss_index] = num_value;
	}
	scanres = fscanf(file, "%200s\n", buf); //Skip unique bosses closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	//keysfound
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->keysfound = num_value;
	scanres = fscanf(file, "%200s\n", buf); //Skip Stats closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	scanres = fscanf(file, "%200s\n", buf); //Skip Fighter closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	log_tag("debug_log.txt","[LOAD]","Done loading Fighter.");

	//Path
	scanres = fscanf(file, "%200s\n", buf); //Skip Path bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	//Luck
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	p->luck = num_value;
	//Length
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	p->length = num_value;
	//Prize
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	p->prize = num_value;
	//loreCounter
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	p->loreCounter = num_value;


	//Wincon
	scanres = fscanf(file, "%200s\n", buf); //Skip Wincon bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	//winconClass
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	p->win_condition->class = num_value;
	//current_val
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	p->win_condition->current_val = num_value;
	//target_val
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	p->win_condition->target_val = num_value;

	scanres = fscanf(file, "%200s\n", buf); //Skip Wincon closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	log_tag("debug_log.txt","[LOAD]","Done loading Wincon.");

	scanres = fscanf(file, "%200s\n", buf); //Skip Path closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	log_tag("debug_log.txt","[LOAD]","Done loading Path.");

	//roomindex
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	*roomIndex = num_value;

	log_tag("debug_log.txt","[LOAD]","Done loading room info.");

	//Consumables bag
	scanres = fscanf(file, "%200s\n", buf); //Skip Consumablebag bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	initConsumableBag(f, kls);
	for (int i = 0; i < CONSUMABLESMAX +1; i++) {
		Consumable * cn = (Consumable*) f->consumablesBag[i];
		//qty
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		cn->qty = num_value;
	}
	scanres = fscanf(file, "%200s\n", buf); //Skip Consumablebag closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	log_tag("debug_log.txt","[LOAD]","Done loading Consumable bag.");

	//Artifacts bag
	scanres = fscanf(file, "%200s\n", buf); //Skip Artifactsbag bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < ARTIFACTSMAX +1; i++) {
		Artifact * a = f->artifactsBag[i];
		//qty
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		a->qty = num_value;
	}
	scanres = fscanf(file, "%200s\n", buf); //Skip Artifactsbag closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadgame_Home(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	log_tag("debug_log.txt","[LOAD]","Done loading Artifacts bag.");

	fclose(file);
	sprintf(msg,"Done loading from text file!\n");
	log_tag("debug_log.txt","[LOAD]",msg);
	//printf("[INFO]    Done loading!\n");
	//TODO:
	//Notification win for load status

	return OP_OK;
}

/**
 * Takes a FILE pointer from which the gamestate will be loaded.
 * Gets the pointers to relevant game structures, sets all the values by reading the save file.
 * Fighter, Path and Enemy pointers, and integers for room / path progression indexes.
 * Saves the state of the game.
 * @see Fighter
 * @see Enemy
 * @see Path
 * @see gameloop()
 * @param file The FILE pointer to load from.
 * @param f The player struct.
 * @param p The path struct.
 * @param e The current enemy struct.
 * @param enemyIndex Index in current room for current enemy.
 * @param roomTotalEnemies Total enemies for current room.
 * @param roomIndex Current room index.
 * @param total_foes Total foes in current room.
 * @param done_loading Flag to check if loading ended.
 * @param kls The Koliseo used for allocations.
 * @return The OP_res corresponding to loading result.
 */
OP_res handleLoadgame_Enemies(FILE* file, Fighter* f, Path* p, Enemy* e, int* enemyIndex, int* roomTotalEnemies, int* roomIndex, int* total_foes, int* done_loading, Koliseo* kls) {

	//printf("\nLoading game...\n");
	log_tag("debug_log.txt","[LOAD]","Starting loading from text file.");

//	FILE* file = fopen("save.txt", "r");
	if (file == NULL)
	{
		printf("Error with file while trying to load!\n");
		return OP_CANNOT_OPEN_SAVEFILE;
	}
	char buf[500];
	char comment[300];
	int num_value = -1;
	const char version[] = "v0.1.5";

	char msg[1000];

	int scanres = -1;
	/* File version scanning */
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	int check = -1;
	if (!((check = strcmp(buf,version)) == 0)) {
		sprintf(msg,"Failed save format version check. Was [%s]. Quitting.\n", buf);
		log_tag("debug_log.txt","[LOAD-ERROR]",msg);
		fprintf(stderr,"[ERROR]    File version mismatch on load.\n");
		exit(EXIT_FAILURE);
	};
	sprintf(msg,"Loaded save format version: (%s).\n",buf);
	log_tag("debug_log.txt","[LOAD]",msg);

	/* Save type scanning */
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	check = -1;
	if (!((check = strcmp(buf,stringFrom_saveType(ENEMIES_SAVE))) == 0)) {
		sprintf(msg,"handleLoadGame_Enemies():  Failed save type check, was [%s]. Quitting.\n",buf);
		log_tag("debug_log.txt","[LOAD-ERROR]",msg);
		fprintf(stderr,"[ERROR]    Save type version mismatch on load.\n");
		exit(EXIT_FAILURE);
	};
	sprintf(msg,"Loaded save type: (%s).\n",buf);
	log_tag("debug_log.txt","[LOAD]",msg);

	/* Gamemode scanning */
	scanres = fscanf(file, "%200[^#]# %s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.\n", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	GAMEMODE = num_value;

	/* Fighter scanning */
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	strcpy(f->name,buf);


	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	for (int i = 0; i < CLASSESMAX +1; i++) {
		int check = -1;
		if ((check = strcmp(buf, stringFromClass(i))) == 0) {
			f->class = i;
			break;
		}
	};

	//We have to reload the specials callbacks after setting fighter class
	setSpecials(f,kls);

	//HP
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->hp = num_value;
	//TotalHP
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->totalhp = num_value;
	//Atk
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->atk = num_value;
	//Def
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	sscanf(buf, "%3i", &num_value);
	f->def = num_value;
	//Vel
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->vel = num_value;
	//Level
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->level = num_value;
	//Luck
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->luck = num_value;
	//Totxp
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	f->totalxp = num_value;
	//Current level xp
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	f->currentlevelxp = num_value;
	//Totallevelxp
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	f->totallevelxp = num_value;
	//Energy
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->energy = num_value;
	//Totenergy
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->totalenergy = num_value;
	//Coin balance
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	f->balance = num_value;

	//Status
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	for (int i = 0; i < STATUSMAX +1; i++) {
		int check = -1;
		if ((check = strcmp(buf,stringFromStatus(i))) == 0) {
			f->status = i;
			break;
		}
	};
	//Specials
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < SPECIALSMAX +1; i++) {
		//Enabled flag
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		f->specials[i]->enabled = num_value;
	}
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	//Fighter counters
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < COUNTERSMAX +1; i++) {
		//innerValue
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		f->counters[i]->innerValue = num_value;
		//count
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		f->counters[i]->count = num_value;
	}
	scanres = fscanf(file, "%200s\n", buf);

	//turnboost_atk
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->turnboost_atk = num_value;
	//turnboost_def
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->turnboost_def = num_value;
	//turnboost_vel
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->turnboost_vel = num_value;
	//turnboost_enr
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->turnboost_enr = num_value;

	//equipsbag occupied slots
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->equipsBagOccupiedSlots = num_value;
	//equipsbag earliest slot
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->earliestBagSlot = num_value;

	//Equipsbag
	scanres = fscanf(file, "%200s\n", buf);
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < f->equipsBagOccupiedSlots; i++) {
		sprintf(msg,"Loadgame_Enemies(): Prepping Equip (%i)",i);
		log_tag("debug_log.txt","[DEBUG]",msg);
		kls_log("DEBUG",msg);
		Equip* eq = KLS_PUSH_NAMED(kls,Equip,1,"Equip",msg);
		scanres = fscanf(file, "%200s\n", buf); //Skip equip_i heading
		if (scanres != 1) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}


		//equipClass
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		for (int j = 0; j < EQUIPSMAX +1; j++) {
			int check = -1;
			if ((check = strcmp(buf,stringFromEquips(j))) == 0) {
				eq->class = j;
				break;
			}
		};
		//equipzoneClass
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		for (int j = 0; j < EQUIPZONES +1; j++) {
			int check = -1;
			if ((check = strcmp(buf,stringFromEquipzones(j))) == 0) {
				eq->type = j;
				break;
			}
		};

		//qty
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->qty = num_value;
		//equipped
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->equipped = num_value;
		//level
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->level = num_value;
		//atk
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->atk = num_value;
		//def
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->def = num_value;
		//vel
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->vel = num_value;
		//enr
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->enr = num_value;
		//bonus
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->bonus = num_value;
		//perksCount
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		eq->perksCount = num_value;
		//cost
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%8i", &num_value);
		eq->cost = num_value;

		//Quality
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		for (int j = 0; j < QUALITIESMAX +1; j++) {
			int check = -1;
			if ((check = strcmp(buf,stringFromQuality(j))) == 0) {
				eq->qual = j;
				break;
			}
		};

		//perks
		scanres = fscanf(file, "%200s\n", buf); //Skip perks heading
		if (scanres != 1) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}
		for (int j = 0; j < eq->perksCount ; j++) {
			sprintf(msg,"Loadgame_Enemies(): Prepping Perk (%i/%i)",j,eq->perksCount);
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			Perk* pk = KLS_PUSH_NAMED(kls,Perk,1,"Perk",msg);
			//perkClass
			scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
			if (scanres != 2) {
				sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
				log_tag("debug_log.txt","[DEBUG]",msg);
				fprintf(stderr,"Error while loading game.");
				exit(EXIT_FAILURE);
			}

			sprintf(msg,"Loaded %s: %s.", comment, buf);
			log_tag("debug_log.txt","[LOAD]",msg);
			sscanf(buf, "%3i", &num_value);
			pk->class = num_value;
			char* name = nameStringFromPerk(pk->class);
			char* desc = descStringFromPerk(pk->class);
			//p->name = name; //(char*)malloc(sizeof(name));
			strcpy(pk->name,name);
			//p->desc = (char*)malloc(sizeof(desc));
			strcpy(pk->desc,desc);
			pk->innerValue = 0;
			eq->perks[j] = pk;

			//innerValue
			scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
			if (scanres != 2) {
				sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
				log_tag("debug_log.txt","[DEBUG]",msg);
				fprintf(stderr,"Error while loading game.");
				exit(EXIT_FAILURE);
			}

			sprintf(msg,"Loaded %s: %s.", comment, buf);
			log_tag("debug_log.txt","[LOAD]",msg);
			sscanf(buf, "%3i", &num_value);
			pk->innerValue = num_value;
		}

		scanres = fscanf(file, "%200s\n", buf); //Skip closing perks breacket
		if (scanres != 1) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		scanres = fscanf(file, "%200s\n", buf); //Skip closing current equip bracket
		if (scanres != 1) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		//Get the base item and copy the stats to the loading item
		Equip* base = &equips[eq->class];
		setEquipSprite(eq);
		strcpy(eq->name, base->name);
		strcpy(eq->desc, base->desc);

		if (eq->equipped) {
			Equipslot* slot = f->equipslots[eq->type];
			//We equip the item
			slot->item = eq;
			applyEquipPerks(eq, f);
			slot->active = 1;
		}
		f->equipsBag[i] = eq;
	} //End for Equips bag

	scanres = fscanf(file,"%200s\n", buf); //Skip closing equipsbag bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}


	//Fighter boosts
	//permboost_atk
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->permboost_atk = num_value;
	//permboost_def
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->permboost_def = num_value;
	//permboost_vel
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->permboost_vel = num_value;
	//permboost_enr
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->permboost_enr = num_value;
	//equipboost_atk
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->equipboost_atk = num_value;
	//equipboost_def
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->equipboost_def = num_value;
	//equipboost_vel
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->equipboost_vel = num_value;
	//equipboost_enr
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->equipboost_enr = num_value;

	//Stats
	scanres = fscanf(file, "%200s\n", buf); //Skip Stats bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	//enemieskilled
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->enemieskilled = num_value;
	//consumablesfound
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->consumablesfound = num_value;
	//equipsfound
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->equipsfound = num_value;
	//artifactsfound
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->artifactsfound = num_value;
	//crits
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->criticalhits = num_value;
	//roomsdone
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->roomscompleted = num_value;
	//specials unlocked
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->specialsunlocked = num_value;
	//coinsfound
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	f->stats->coinsfound = num_value;
	//bosseskilled
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->bosseskilled = num_value;

	//Unique boss kills
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->unique_bosseskilled = num_value;

	scanres = fscanf(file, "%200s\n", buf); //Skip unique bosses bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	for (int boss_index = 0; boss_index < BOSSCLASSESMAX +1; boss_index++) {
		//boss %i was killed
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		f->stats->killed_bosses[boss_index] = num_value;
	}
	scanres = fscanf(file, "%200s\n", buf); //Skip unique bosses closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	//keysfound
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	f->stats->keysfound = num_value;
	scanres = fscanf(file, "%200s\n", buf); //Skip Stats closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	scanres = fscanf(file, "%200s\n", buf); //Skip Fighter closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	log_tag("debug_log.txt","[LOAD]","Done loading Fighter.");

	//Current enemy
	scanres = fscanf(file, "%200s\n", buf); //Skip Enemy bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	//Enemy class
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	scanres = sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	for (int i = 0; i < ENEMYCLASSESMAX +1; i++) {
		int check = -1;
		if ((check = strcmp(buf, stringFromEClass(i))) == 0) {
			e->class = i;
			break;
		};
	}
	//We have to reload the sprite after setting enemy class
	setEnemySprite(e);

	//hp
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	e->hp = num_value;
	//totalhp
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	e->totalhp = num_value;
	//atk
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	e->atk = num_value;
	//def
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	e->def = num_value;
	//vel
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	e->vel = num_value;
	//level
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	e->level = num_value;
	//xp
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	e->xp = num_value;
	//energy
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	e->energy = num_value;
	//totalenergy
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	e->totalenergy = num_value;
	//beast value
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	e->beast = num_value;
	//prize
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	e->prize = num_value;

	//status
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	for (int i = 0; i < STATUSMAX +1; i++) {
		int check = -1;
		if ((check = strcmp(buf, stringFromStatus(i))) == 0) {
			e->status = i;
			break;
		}
	};

	//Enemy counters
	scanres = fscanf(file, "%200s\n", buf); //Skip Enemy counters bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < COUNTERSMAX +1; i++) {
		//innerValue
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		e->counters[i]->innerValue = num_value;
		//count
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		e->counters[i]->count = num_value;
	}
	scanres = fscanf(file, "%200s\n", buf); //Skip Enemy counters closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	scanres = fscanf(file, "%200s\n", buf); //Skip Enemy closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	log_tag("debug_log.txt","[LOAD]","Done loading current Enemy.");

	//Path
	scanres = fscanf(file, "%200s\n", buf); //Skip Path bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	//Luck
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	p->luck = num_value;
	//Length
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	p->length = num_value;
	//Prize
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	p->prize = num_value;
	//loreCounter
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	p->loreCounter = num_value;


	//Wincon
	scanres = fscanf(file, "%200s\n", buf); //Skip Wincon bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	//winconClass
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	p->win_condition->class = num_value;
	//current_val
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	p->win_condition->current_val = num_value;
	//target_val
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	p->win_condition->target_val = num_value;

	scanres = fscanf(file, "%200s\n", buf); //Skip Wincon closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	log_tag("debug_log.txt","[LOAD]","Done loading Wincon.");

	scanres = fscanf(file, "%200s\n", buf); //Skip Path closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	log_tag("debug_log.txt","[LOAD]","Done loading Path.");

	//enemyindex
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	*enemyIndex = num_value;
	//roomtotalenemies
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%3i", &num_value);
	*roomTotalEnemies = num_value;
	*total_foes = num_value;
	sprintf(msg,"Also Loaded %s: %s. Additional assignment to pointer for foes, see handleLoadGame(_Enemies)", "total_foes", buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	//roomindex
	scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
	if (scanres != 2) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	sprintf(msg,"Loaded %s: %s.", comment, buf);
	log_tag("debug_log.txt","[LOAD]",msg);
	sscanf(buf, "%8i", &num_value);
	*roomIndex = num_value;

	log_tag("debug_log.txt","[LOAD]","Done loading room info.");

	//Consumables bag
	scanres = fscanf(file, "%200s\n", buf); //Skip Consumablebag bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	initConsumableBag(f, kls);
	for (int i = 0; i < CONSUMABLESMAX +1; i++) {
		Consumable * cn = (Consumable*) f->consumablesBag[i];
		//qty
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		cn->qty = num_value;
	}
	scanres = fscanf(file, "%200s\n", buf); //Skip Consumablebag closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	log_tag("debug_log.txt","[LOAD]","Done loading Consumable bag.");

	//Artifacts bag
	scanres = fscanf(file, "%200s\n", buf); //Skip Artifactsbag bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < ARTIFACTSMAX +1; i++) {
		Artifact * a = f->artifactsBag[i];
		//qty
		scanres = fscanf(file, "%200[^#]# %200s\n", buf, comment);
		if (scanres != 2) {
			sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",2,scanres);
			log_tag("debug_log.txt","[DEBUG]",msg);
			fprintf(stderr,"Error while loading game.");
			exit(EXIT_FAILURE);
		}

		sprintf(msg,"Loaded %s: %s.", comment, buf);
		log_tag("debug_log.txt","[LOAD]",msg);
		sscanf(buf, "%3i", &num_value);
		a->qty = num_value;
	}
	scanres = fscanf(file, "%200s\n", buf); //Skip Artifactsbag closing bracket
	if (scanres != 1) {
		sprintf(msg,"Bad fscanf() result in handleLoadGame_Enemies(), expected [%i] was (%i)",1,scanres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		fprintf(stderr,"Error while loading game.");
		exit(EXIT_FAILURE);
	}

	log_tag("debug_log.txt","[LOAD]","Done loading Artifacts bag.");

	fclose(file);
	sprintf(msg,"Done loading from text file!\n");
	log_tag("debug_log.txt","[LOAD]",msg);
	//printf("[INFO]    Done loading!\n");
	//TODO:
	//Notification win for load status

	return OP_OK;
}

/**
 * Takes a Fighter and loadInfo pointers and prints fighter stats and a game over message.
 * Consumables and Artifacts are emptied before freeing the player's specials, counters, perks and stats field.
 * At last, the player pointer is freed.
 * @see Fighter
 * @see handleStats()
 * @see emptyConsumables()
 * @see emptyArtifacts()
 * @param player The Fighter pointer to free.
 * @param load_info The loadInfo pointer to free.
 */
void death(Fighter* player, loadInfo* load_info) {

	//FIXME:
	//dropping out of the Koliseo scope might render stat pointer invalid.
	//handleStats(player);

	//Free default kls
	kls_free(default_kls);
	kls_log("DEBUG","Freed default KLS");
	log_tag("debug_log.txt","[DEBUG-KLS]","Freed default KLS");

	//Free temporary kls
	kls_free(temporary_kls);
	kls_log("DEBUG","Freed temporary KLS");
	log_tag("debug_log.txt","[DEBUG-KLS]","Freed temporary KLS");

	/*
	free(load_info);
	sprintf(msg,"Freed loadInfo.\n");
	log_tag("debug_log.txt","[FREE]",msg);
	*/

	//emptyConsumables(player);
	//emptyArtifacts(player);
	/*
	//Free player special slots
	for (int i=0; i < (SPECIALSMAX + 1) ; i++) {
		free(player->specials[i]);
		sprintf(msg,"Freed player special %i.",i);
		log_tag("debug_log.txt","[FREE]",msg);
	}
	log_tag("debug_log.txt","[FREE]","Done.\n");
	*/

	/*
	//Free player equipbag
	int total = player->equipsBagOccupiedSlots;
	for (int i=0; i < (total ) ; i++) {
		Equip* e = (Equip*) player->equipsBag[i];
		int perkscount = e->perksCount;
		if (perkscount > 0) {
			for (int j=0; j < perkscount; j++) {
				free(e->perks[j]);
				sprintf(msg,"Freed equip %i perk %i.", i, j);
				log_tag("debug_log.txt","[FREE]",msg);
			}
		}
		free(e);
		sprintf(msg,"Freed equip %i.\n", i);
		log_tag("debug_log.txt","[FREE]",msg);
	}
	log_tag("debug_log.txt","[FREE]","Done.\n");
	*/

	/*
	//Free player consumablebag
	int cons_total = CONSUMABLESMAX+1;
	for (int i=0; i < cons_total ; i++) {
		Consumable* c = (Consumable*) player->consumablesBag[i];
		sprintf(msg,"Freed consumable %i.", i);
		log_tag("debug_log.txt","[FREE]",msg);
		free(c);
	}
	log_tag("debug_log.txt","[FREE]","Done.\n");
	*/

	/*
	//Free player equip slots
	for (int i=0; i < (EQUIPZONES + 1) ; i++) {
		Equipslot* s = (Equipslot*) player->equipslots[i];

		 int perkscount = -1;

		if (s->active) { perkscount = s->item->perksCount;};
		if (perkscount > 0) {
			for (int i=0; i < perkscount; i++) {
				free(s->item->perks[i]);
			}
			free(s->item);
		}

		free(s);
		sprintf(msg,"Freed equipslot %i.", i);
		log_tag("debug_log.txt","[FREE]",msg);
	}
	log_tag("debug_log.txt","[FREE]","Done.\n");
	*/

	/*
	//Free player artifactsbag
	int art_total = CONSUMABLESMAX+1;
	for (int i=0; i < art_total ; i++) {
		Artifact* a = (Artifact*) player->artifactsBag[i];
		free(a);
		sprintf(msg,"Freed artifact %i.", i);
		log_tag("debug_log.txt","[FREE]",msg);
	}
	log_tag("debug_log.txt","[FREE]","Done.\n");
	*/

	/*
	//Free player turnCounters
	for (int i=0; i < (COUNTERSMAX + 1) ; i++) {
		Turncounter* c = (Turncounter*) player->counters[i];
		free(c->desc);
		sprintf(msg,"Freed turncounter %i desc.", i);
		log_tag("debug_log.txt","[FREE]",msg);
		free(c);
		sprintf(msg,"Freed turncounter %i.\n", i);
		log_tag("debug_log.txt","[FREE]",msg);
	}
	log_tag("debug_log.txt","[FREE]","Done.\n");
	*/

	/*
	//Free player perks
	for (int i=0; i < (PERKSMAX + 1) ; i++) {
		free(player->perks[i]);
		sprintf(msg,"Freed player perk %i.", i);
		log_tag("debug_log.txt","[FREE]",msg);
	}
	log_tag("debug_log.txt","[FREE]","Done.\n");
	*/

	//free(player->stats);
	//log_tag("debug_log.txt","[FREE]","Freed player stats.\n");
}

/**
 * Takes a Enemy pointer and frees its allocated memory.
 * The counters field is freed before the enemy pointer.
 * @see Enemy
 * @param e The Enemy pointer to free.
 */
void e_death(Enemy* e) {

	char msg[200];

	//Free enemy special slots
	//for (int i=0; i < SPECIALSMAX + 1 ; i++) {
	//	free(player->specials[i]);
	//}

	/*
	//Free enemy turnCounters
	for (int i=0; i < (COUNTERSMAX + 1) ; i++) {
		Turncounter* c = (Turncounter*) e->counters[i];
		sprintf(msg,"Freed enemy turncounter %i desc:    %s.",i, c->desc);
		log_tag("debug_log.txt","[FREE]",msg);
		if (c->desc == NULL) {
			log_tag("debug_log.txt","[ERROR]", "Enemy turncounter desc was null.\n");
		} else {
			char* desc_to_free = c->desc;
			free(desc_to_free);
		}
		free(c);
		sprintf(msg,"Freed enemy turncounter %i.\n",i);
		log_tag("debug_log.txt","[FREE]",msg);
	}
	*/

	sprintf(msg,"Freeing enemy %s\n",stringFromEClass(e->class));
	log_tag("debug_log.txt","[FREE]",msg);
	//free(e);
}

/**
 * Takes a Boss pointer and frees its allocated memory.
 * The counters field is freed before the boss pointer.
 * @see Boss
 * @param b The Boss pointer to free.
 */
void b_death(Boss* b) {

	log_tag("debug_log.txt","[DEBUG]","b_death():  I'm only freeing boss pointer.");

	//Free boss special slots
	//for (int i=0; i < SPECIALSMAX + 1 ; i++) {
	//	free(player->specials[i]);
	//}
	//Free boss turnCounters
	/*
	for (int i=0; i < (COUNTERSMAX + 1) ; i++) {
		Turncounter* c = (Turncounter*) b->counters[i];
		sprintf(msg,"Freed boss turncounter %i desc:    %s.",i, c->desc);
		log_tag("debug_log.txt","[FREE]",msg);
		free(c->desc);
		free(c);
		sprintf(msg,"Freed boss turncounter %i.\n",i);
		log_tag("debug_log.txt","[FREE]",msg);
	}
	*/
	//free(b);
}

/**
 * Asks the user is they want to continue and returns the choice.
 * @return int True for trying again, false otherwise.
 */
int retry(void) {
	lightGreen();
	printf("\n\nYou died. Want to try again?\n\n\t\t0 - No\t\t\t1 - Yes\n\n");
	white();

	char c[25];
	char msg[500];
	char* res = fgets(c, sizeof c, stdin);
	sprintf(msg,"retry() fgets() result was (%s)",res);
	log_tag("debug_log.txt","[DEBUG]",msg);

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
 * Takes a Fighter and a Path pointers (and an integer for current room index) and asks user input to execute debug actions.
 * @see Fighter
 * @see Path
 * @see statReset()
 * @see GET_CALLBACK()
 * @see unlock_special()
 * @param player The Fighter pointer at hand.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param The Koliseo to debug.
 * @param The Koliseo_Temp used for allocations.
 */
void debug_generic(Fighter* player, Path* p, int roomIndex, Koliseo* kls, Koliseo_Temp* t_kls) {

	Koliseo_Temp tkls = *t_kls;

	char msg[200];
	char ch[25];
	int picked_debug_proc = 0;
	struct utsname uts;
	uname(&uts);
	sprintf(msg,"debug_generic() loaded utsname using uname().\n");
	log_tag("debug_log.txt","[DEBUG]",msg);
	sprintf(msg,"System is %s\n",uts.sysname);
	log_tag("debug_log.txt","[DEBUG]",msg);
	sprintf(msg,"OS Release is %s\n",uts.release);
	log_tag("debug_log.txt","[DEBUG]",msg);
	sprintf(msg,"OS Version is %s\n",uts.version);
	log_tag("debug_log.txt","[DEBUG]",msg);
	sprintf(msg,"Machine is %s\n",uts.machine);
	log_tag("debug_log.txt","[DEBUG]",msg);

	int res = system("clear");
	sprintf(msg,"debug_generic() system(\"clear\") res was (%i)",res);
	log_tag("debug_log.txt","[DEBUG]",msg);


	int c=0,n=-1;
	while (!picked_debug_proc) {
		int res = system("clear");
		sprintf(msg,"debug_generic() 2 system(\"clear\") res was (%i)",res);
		log_tag("debug_log.txt","[DEBUG]",msg);
		printf("\n\
	What do you want to do? ('q' to quit)\n\
		[\n\
			'0':  Give xp points\t'1':  Give consumable\n\
			'2':  Reset stats\t'3':  Alter luck\n\
			'4':  Give coins\t'5':  Unlock special\n\
			'6':  Unlock Artifact\t'7':   Print turnCounter\n\
			's':  Sprites slideshow\t'd':  Dump debug symbols\n\
			'g':  Toggle godmode\t'A':  Toggle autosave\n\
			'L':  Toggle logging\t'F':  Try Floor functionality\n\
			'Q':  Toggle fast quit\t'K': Log passed kls state to debug log file.\n\
			'T': Log global temporary_kls state to debug log file.\n\
			{Return}  Process your input line.\t'q':  Quit\n\
		]\n\n\
	[%s@debug-func]$ ",player->name);

		char* fgetsres = fgets(ch, sizeof ch, stdin);
		sprintf(msg,"debug_generic() fgets() res was (%s)", fgetsres);
		log_tag("debug_log.txt","[DEBUG]",msg);
	switch(ch[0]) {
		case '0': {
			picked_debug_proc = 1;
			int xp;
			do {
				printf("\nHow much?\n");
			} while ((c = scanf("%i",&xp)) != 1 || xp < 0);
			int res = scanf("%*c");
			sprintf(msg,"debug_generic() scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			checkremainder(player,xp);
		}
		break;
		case '1': {
			picked_debug_proc = 1;
			int q=-1;
			do {
				printf("\nInsert consumable number:\n");
			} while ((c = scanf("%i",&n)) != 1 || n > CONSUMABLESMAX || n < 0 );
			int res = scanf("%*c");
			sprintf(msg,"debug_generic() scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			do {
				printf("\nInsert quantity:\n");
			} while ((c = scanf("%i",&q)) != 1 && q <= 0 );
			res = scanf("%*c");
			sprintf(msg,"debug_generic() 2 scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			Consumable* c = (Consumable*) player->consumablesBag[n];
			c->qty += q;
			player->stats->consumablesfound += q;
		}
		break;
		case '2': {
			picked_debug_proc = 1;
			statReset(player,1);
		}
		break;
		case '3': {
			picked_debug_proc = 1;
			printf("\nCurrent luck: %i\tRL: %i\n",player->luck,p->luck);

			do {
				printf("\nInsert new luck:\n");
			} while ((c = scanf("%i",&n)) != 1 && n > 0 );
			int res = scanf("%*c");
			sprintf(msg,"debug_generic() 3 scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
				//FIXME:
				//Copy-pasted the calc for player luck... might need a function
				p->luck = n;

				player->luck = (p->luck * MAXPLAYERLUCK) / MAXLUCK ;
		}
		break;
		case '4': {
			picked_debug_proc = 1;
			/* Old code invoking the macro for special moves directly. Used for testing.
			 * int s = -1;
			 * printf("Insert special num:\n");
			 * scanf("%i",&s);
			 * scanf("%*c");
			 * printf("Read: %i\n",s);
			 * GET_CALLBACK(s,callback_special_t)(player,e,p,roomIndex,currentEnemyNum);
			*/

			//printActivePerks(player);
			int c;
			int n=-1;
			do {
				printf("\nInsert coin number (0<n<100):\n");
			} while ((c = scanf("%i",&n)) < 0 || c > 3 || n < 0 || n > 100 );
			int res = scanf("%*c");
			sprintf(msg,"debug_generic() 4 scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			player->balance += n;
		}
		break;
		case '5': {
			picked_debug_proc = 1;
			unlockSpecial(player);
		}
		break;
		case '6': {
			picked_debug_proc = 1;
			int n=-1;
			do {
				printf("\nInsert artifact number (0<n<%i):\n",ARTIFACTSMAX + 1);
			} while ((c = scanf("%i",&n)) != 1 || n > ARTIFACTSMAX || n < 0 );

			int res = scanf("%*c");
			sprintf(msg,"debug_generic() scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			player->artifactsBag[n]->qty += 1;
			player->artifactsBag[n]->active = 0;
			player->stats->artifactsfound += 1;
		}
		break;
		case '7': {
			picked_debug_proc = 1;
			int res = system("clear");
			sprintf(msg,"debug_generic() 3 system(\"clear\") res was (%i)",res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			printf("\nPlayer Counters:\n");
			printCounters((Turncounter **)player->counters);
			printf("\nPress Enter to resume game");
			res = scanf("%*c");
			sprintf(msg,"debug_generic() 5 scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
		}
		break;
		case 'd': {
			picked_debug_proc = 1;
			printf("\nVERSION:    %s\n",VERSION);
			printf("\nSystem:    %s\n",uts.sysname);
			printf("\nOS Release:    %s\n",uts.release);
			printf("\nOS Version:    %s\n",uts.version);
			printf("\nMachine:    %s\n",uts.machine);
			printf("\nGAMEMODE:    %s\n",stringFromGamemode(GAMEMODE));
			printf("\nPath->current_saveslot->save_path:    %s\n",p->current_saveslot->save_path);
			printf("\nGS_AUTOSAVE_ON:    %i\n",GS_AUTOSAVE_ON);
			printf("\nG_FASTQUIT_ON:    %i\n",G_FASTQUIT_ON);
			printf("\nG_DEBUG_ON:    %i\n",G_DEBUG_ON);
			printf("\nG_LOG_ON:    %i\n",G_LOG_ON);
			printf("\nG_GODMODE_ON:    %i\n",G_GODMODE_ON);
			printf("\nG_EXPERIMENTAL_ON:    %i\n",G_EXPERIMENTAL_ON);
			printf("\nG_DEBUG_ROOMTYPE_ON:    %i\n",G_DEBUG_ROOMTYPE_ON);
			if (G_DEBUG_ROOMTYPE_ON == 1) {
				printf("\nG_DEBUG_ROOMTYPE:    %i\n",G_DEBUG_ROOMTYPE);
			}
			printf("\nG_DEBUG_ENEMYTYPE_ON:    %i\n",G_DEBUG_ENEMYTYPE_ON);
			if (G_DEBUG_ENEMYTYPE_ON == 1) {
				printf("\nG_DEBUG_ENEMYTYPE:    %i\n",G_DEBUG_ENEMYTYPE);
			}
			printf("\nPress Enter to resume game.\n");
			int res = scanf("%*c");
			sprintf(msg,"debug_generic() 7 scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
		}
		break;
		case 'F': {
			sprintf(msg,"Trying out Floor functionality.");
			log_tag("debug_log.txt","[DEBUG]",msg);
			// Declare dbg_floor
			sprintf(msg,"Pushing dbg_floor to tkls.");
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			Floor* dbg_floor = (Floor*) KLS_PUSH_T_NAMED(tkls,Floor,1,"Floor",msg);
			// Start the random walk from the center of the dungeon
    			int center_x = FLOOR_MAX_COLS / 2;
    			int center_y = FLOOR_MAX_ROWS / 2;
			// Init dbg_floor
			init_floor_layout(dbg_floor);
			//Set center as filled
    			dbg_floor->floor_layout[center_x][center_y] = 1;

			init_floor_rooms(dbg_floor);

    			floor_random_walk(dbg_floor, center_x, center_y, 100, 1); // Perform 100 steps of random walk, reload floor_layout if needed
			load_floor_explored(dbg_floor);

			floor_set_room_types(dbg_floor);

			/*
    			debug_print_floor_layout(dbg_floor);

			printf("\nPress Enter to see room layout.\n");
			int res = scanf("%*c");
			sprintf(msg,"debug_generic() 7 scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			*/
    			debug_print_roomclass_layout(dbg_floor,stdout);
			printf("\nPress Enter to return to game.\n");
			res = scanf("%*c");
			sprintf(msg,"debug_generic() 8 scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			//free(dbg_floor);
		}
		break;
		case 'A': {
			GS_AUTOSAVE_ON = (GS_AUTOSAVE_ON == 1 ? 0 : 1);
			sprintf(msg,"Toggled G_AUTOSAVE_ON, new value: (%i)", GS_AUTOSAVE_ON);
			log_tag("debug_log.txt","[DEBUG]",msg);
		}
		break;
		case 'T': {
			char path_to_kls_file[600];
			char static_path[500];
			// Set static_path value to the correct static dir path
			resolve_staticPath(static_path);

			//Append to "kls_log.txt"
			sprintf(path_to_kls_file,"%s/%s",static_path,"debug_log.txt");
			FILE* kls_file = NULL;
			kls_file = fopen(path_to_kls_file, "a");
			if (kls_file == NULL) {
				fprintf(stderr,"debug_generic():  failed opening debug logfile.\n");
				exit(EXIT_FAILURE);
			}
			if (kls == NULL) {
				fprintf(stderr,"debug_generic():  kls was NULL.\n");
				exit(EXIT_FAILURE);
			}
			fprintf(kls_file,"--BEGIN debug of temporary_kls--\n");
			print_kls_2file(kls_file,temporary_kls);
			kls_showList_toFile(kls_reverse(temporary_kls->regs),kls_file);
			kls_usageReport_toFile(temporary_kls,kls_file);
			fprintf(kls_file,"--END debug of temporary_kls--\n\n");
			WINDOW* win = NULL;
			/* Initialize curses */
			clear();
			refresh();
			start_color();
			cbreak();
			noecho();
			keypad(stdscr, TRUE);
			win = newwin(20, 50, 1, 2);
			keypad(win, TRUE);
			wclear(win);
			wrefresh(win);
			kls_showList_toWin(temporary_kls,win);
			delwin(win);
			endwin();

			fclose(kls_file);
		}
		break;
		case 'K': {
			char path_to_kls_file[600];
			char static_path[500];
			// Set static_path value to the correct static dir path
			resolve_staticPath(static_path);

			//Append to "kls_log.txt"
			sprintf(path_to_kls_file,"%s/%s",static_path,"debug_log.txt");
			FILE* kls_file = NULL;
			kls_file = fopen(path_to_kls_file, "a");
			if (kls_file == NULL) {
				fprintf(stderr,"debug_generic():  failed opening debug logfile.\n");
				exit(EXIT_FAILURE);
			}
			if (kls == NULL) {
				fprintf(stderr,"debug_generic():  kls was NULL.\n");
				exit(EXIT_FAILURE);
			}
			fprintf(kls_file,"--BEGIN debug of passed kls--\n");
			print_kls_2file(kls_file,kls);
			kls_showList_toFile(kls_reverse(kls->regs),kls_file);
			kls_usageReport_toFile(kls,kls_file);
			fprintf(kls_file,"--END debug of passed kls--\n\n");
			WINDOW* win = NULL;
			/* Initialize curses */
			clear();
			refresh();
			start_color();
			cbreak();
			noecho();
			keypad(stdscr, TRUE);
			win = newwin(20, 50, 1, 2);
			keypad(win, TRUE);
			wclear(win);
			wrefresh(win);
			kls_showList_toWin(kls,win);
			delwin(win);
			endwin();

			fclose(kls_file);
		}
		break;
		case 'Q': {
			G_FASTQUIT_ON = (G_FASTQUIT_ON == 1 ? 0 : 1);
			sprintf(msg,"Toggled G_FASTQUIT_ON, new value: (%i)", G_FASTQUIT_ON);
			log_tag("debug_log.txt","[DEBUG]",msg);
		}
		break;
		case 'g': {
			G_GODMODE_ON = (G_GODMODE_ON == 1 ? 0 : 1);
			sprintf(msg,"Toggled G_GODMODE_ON, new value: (%i)", G_GODMODE_ON);
			log_tag("debug_log.txt","[DEBUG]",msg);
		}
		break;
		case 'L': {
			G_LOG_ON = (G_LOG_ON == 1 ? 0 : 1);
			sprintf(msg,"Toggled G_LOG_ON, new value: (%i)", G_LOG_ON);
			log_tag("debug_log.txt","[DEBUG]",msg);
		}
		case 's': { //Query sprites slideshow
			picked_debug_proc = 1;
			int res = system("clear");
			sprintf(msg,"debug_generic() 4 system(\"clear\") res was (%i)",res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			int comm_len = -1;
			int picked_comm = 0;
			char comm[15] = "empty";
			char* desc = NULL;
			int sprite_total = -1;
			int tot_cons = CONSUMABLESMAX+1;
			int tot_enemy = ENEMYCLASSESMAX+1;
			int tot_boss = BOSSCLASSESMAX+1;
			int tot_art = ARTIFACTSMAX+1;
			int tot_eq = EQUIPSMAX+1;
			int tot_eqzones = EQUIPZONES+1;
			int tot_misc = MISC_SPRITES_MAX+1;
			int allsprites =
				tot_cons +
				tot_enemy +
				tot_boss +
				tot_art +
				tot_eq +
				tot_eqzones +
				tot_misc;

			printf("\nConsumables sprites: \t%d\
				\nArtifacts sprites: \t%d\
				\nEquips sprites: \t%d\
				\nEquip zone sprites: \t%d\
				\nEnemies sprites: \t%d\
				\nBosses sprites: \t%d\
				\nOther misc sprites: \t%d\n", tot_cons, tot_art, tot_eq, tot_eqzones, tot_enemy, tot_boss, tot_misc);
			printf("--------------------------");
			printf("\nTotal sprites: \t\t%d\n",allsprites);
			printf("\nInsert sprites query, q to quit: (q|boss|cons|equip|eqzone|artf|enemy|misc):\n\n[%s@spriteshow]$ ",player->name);
			while (!picked_comm && (comm_len = scanf("%10s",comm)) > -1) {
				int res = scanf("%*c");
				sprintf(msg,"debug_generic() 7 scanf() res was (%i)", res);
				log_tag("debug_log.txt","[DEBUG]",msg);
				if (strcmp(comm,"q") == 0) {
					return;
				} else if (strcmp(comm,"boss") == 0) {
					picked_comm = 1;
					sprite_total = BOSSCLASSESMAX+1;
				} else if (strcmp(comm,"cons") == 0) {
					picked_comm = 1;
					sprite_total = CONSUMABLESMAX+1;
				} else if (strcmp(comm,"equip") == 0) {
					picked_comm = 1;
					sprite_total = EQUIPSMAX+1;
				} else if (strcmp(comm,"eqzone") == 0) {
					picked_comm = 1;
					sprite_total = EQUIPZONES+1;
				} else if (strcmp(comm,"artf") == 0) {
					picked_comm = 1;
					sprite_total = ARTIFACTSMAX+1;
				} else if (strcmp(comm,"enemy") == 0) {
					picked_comm = 1;
					sprite_total = ENEMYCLASSESMAX+1;
				} else if (strcmp(comm,"misc") == 0) {
					picked_comm = 1;
					sprite_total = MISC_SPRITES_MAX+1;
				} else {
					printf("\nInsert sprites query, q to quit: (q|boss|cons|equip|eqzone|artf|enemy|misc):\n\n[%s@spriteshow]$ ",player->name);
				}
			}
			int sprite_count = -1;
			int c = -1;
			char s[20];
			int startx = 0;
			int x,y;
			WINDOW* spriteshow_win;
			/* Initialize curses */
			//setlocale(LC_CTYPE, "it_IT.UTF-8");
			//initscr();
			start_color();
			clear();
			refresh();
			cbreak();
			noecho();
			keypad(stdscr, TRUE);
			spriteshow_win = newwin(19, 19, 2, 2);
			keypad(spriteshow_win, TRUE);
			for (sprite_count=0; sprite_count < sprite_total; sprite_count++) {
				if (strcmp(comm,"boss") == 0) {
					desc = stringFromBossClass(sprite_count);
				} else if (strcmp(comm,"cons") == 0) {
					desc = stringFromConsumables(sprite_count);
				} else if (strcmp(comm,"equip") == 0) {
					desc = stringFromEquips(sprite_count);
				} else if (strcmp(comm,"eqzone") == 0) {
					desc = stringFromEquipzones(sprite_count);
				} else if (strcmp(comm,"artf") == 0) {
					desc = stringFromArtifacts(sprite_count);
				} else if (strcmp(comm,"enemy") == 0) {
					desc = stringFromEClass(sprite_count);
				} else if (strcmp(comm,"misc") == 0) {
					desc = "Misc";
				}
				wclear(spriteshow_win);
				wrefresh(spriteshow_win);
				wclear(stdscr);
				wrefresh(stdscr);
				sprintf(msg,"Showing sprite n.%d, class is: %s\n", sprite_count, desc);
				log_tag("debug_log.txt","[DEBUG]",msg);
				mvwprintw(stdscr, 5, 20, "Sprite for: \'%s\'", desc);
				mvwprintw(stdscr, 7, 20, "(%i/%i)", sprite_count+1, sprite_total);
				refresh();
				for (int i = 0; i < 8; i++) {
					if (strcmp(comm,"boss") == 0) {
						strcpy(s, bosses_sprites[sprite_count][i]);
					} else if (strcmp(comm,"cons") == 0) {
						strcpy(s, consumables_sprites[sprite_count][i]);
					} else if (strcmp(comm,"equip") == 0) {
						strcpy(s, equips_sprites[sprite_count][i]);
					} else if (strcmp(comm,"eqzone") == 0) {
						strcpy(s, equipzones_sprites[sprite_count][i]);
					} else if (strcmp(comm,"artf") == 0) {
						strcpy(s, artifacts_sprites[sprite_count][i]);
					} else if (strcmp(comm,"enemy") == 0) {
						strcpy(s, enemies_sprites[sprite_count][i]);
					} else if (strcmp(comm,"misc") == 0) {
						strcpy(s, misc_sprites[sprite_count][i]);
					}
					//sprintf(msg,"[DEBUG]    Copied string: \'%s\'\n", s);
					//debug_log("debug_log.txt",msg);

					for (int j = 0; j < 13; j++) {
						x = startx + 1 + j;
						y = 1 + i ;

						print_encoded_char(spriteshow_win, y, x, s[j]);
					}//End for line print
				}//End for sprite print

				wrefresh(spriteshow_win);
				//printf("\nPress Enter for next, q to quit.\n");
				int picked = 0;
				int go_previous = 0;
				int quit_show = 0;
				while (!picked && ((c = wgetch(spriteshow_win)) != KEY_F(1))) {
					if (c == 10 || c == KEY_RIGHT) { //Enter, right
							sprintf(msg,"Going to next sprite.\n");
							log_tag("debug_log.txt","[DEBUG]",msg);
							wclear(spriteshow_win);
							wrefresh(spriteshow_win);
							picked = 1;
							continue; //Go to next sprite
					} else if (c == 'q') { //Quit
						sprintf(msg,"Stopping query sprite show.\n");
						log_tag("debug_log.txt","[DEBUG]",msg);
						picked = 1;
						quit_show = 1;
					} else if (c == KEY_LEFT ) { //Go to previous sprite
						go_previous = 1;
						sprintf(msg,"Going to previous sprite.\n");
						log_tag("debug_log.txt","[DEBUG]",msg);
						picked = 1;
					} else { //Unexpected char
						sprintf(msg,"Wrong operation. Continuing with next sprite.\n");
						log_tag("debug_log.txt","\033[1;31m[ERROR]",msg);
						picked = 1;
						continue; //We still go to next sprite
					}
				} //End while wait for user input
				if (go_previous) {
					sprite_count--;
					sprite_count--;
					if (sprite_count < -1) sprite_count = -1;
					go_previous=0;
				}
				if (quit_show) {
					break;
				}
			} //End for all sprites
			sprintf(msg,"End of sprite show.\n");
			log_tag("debug_log.txt","[DEBUG]",msg);

			delwin(spriteshow_win);
			endwin();
		}
		break;
		case 'q': {
			return;
		}
		default : { //Echo the passed char and ask for one more.
			char cmd[50];
			sprintf(cmd,"\necho \"%c\\n\"\n%c\n\n",ch[0],ch[0]);
			printf("%s",cmd);
			screenTime(1);
		}
		break;
	} //Close switch on ch[0]
	} //Close while !picked_debug_proc

	res = system("clear");
	sprintf(msg,"debug_generic() final system(\"clear\") res was (%i)",res);
	log_tag("debug_log.txt","[DEBUG]",msg);
}

/**
 * Takes a Room, a Fighter, a Enemy and a Path pointers (and integers for current room and enemy indexes) and asks user input to execute debug actions.
 * @see Room
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
 * @param kls The Koliseo used for allocations.
 * @param t_kls The Koliseo_Temp used for temporary allocations.
 */
void debug_enemies_room(Room* room, Fighter* player, Enemy* e, Path* p, int roomIndex,int currentEnemyNum, Koliseo* kls, Koliseo_Temp* t_kls) {

	char msg[200];
	char ch[25];
	int picked_debug_proc = 0;
	struct utsname uts;
	uname(&uts);
	sprintf(msg,"debug_enemies_room() loaded utsname using uname().\n");
	log_tag("debug_log.txt","[DEBUG]",msg);
	sprintf(msg,"System is %s\n",uts.sysname);
	log_tag("debug_log.txt","[DEBUG]",msg);
	sprintf(msg,"OS Release is %s\n",uts.release);
	log_tag("debug_log.txt","[DEBUG]",msg);
	sprintf(msg,"OS Version is %s\n",uts.version);
	log_tag("debug_log.txt","[DEBUG]",msg);
	sprintf(msg,"Machine is %s\n",uts.machine);
	log_tag("debug_log.txt","[DEBUG]",msg);

	int res = system("clear");
	sprintf(msg,"debug_enemies_room() system(\"clear\") res was (%i)",res);
	log_tag("debug_log.txt","[DEBUG]",msg);


	int c=0,n=-1;
	while (!picked_debug_proc) {
		int res = system("clear");
		sprintf(msg,"debug_enemies_room() 2 system(\"clear\") res was (%i)",res);
		log_tag("debug_log.txt","[DEBUG]",msg);
		printf("\n\
	What do you want to do? ('q' to quit)\n\
		[\n\
			'0':  Give xp points\t'1':  Give consumable\n\
			'2':  Reset stats\t'3':  Reset Enemy\n\
			'4':  Alter luck\t'5':  Give coins\n\
			'6':  Unlock special\t'7':  Unlock Artifact\n\
			'8':  Print counters\t'9':  Give Equip\n\
			's':  Sprites slideshow\t'd': Dump debug symbols\n\
			'f':  Show foes info\t'g': Toggle godmode\n\
			'A':  Toggle autosave\t'Q': Toggle fast quit\n\
			'L':  Toggle logging\t'K': Log passed Koliseo info\n\
			'T': Log global temporary_kls Koliseo info\n\
			'q': Quit\t{Return}  Process your input line.\n\
		]\n\n\
	[%s@debug-func]$ ",player->name);

		char* fgetsres = fgets(ch, sizeof ch, stdin);
		sprintf(msg,"debug_enemies_room() fgets() res was (%s)", fgetsres);
		log_tag("debug_log.txt","[DEBUG]",msg);
	switch(ch[0]) {
		case '0': {
			picked_debug_proc = 1;
			int xp;
			do {
				printf("\nHow much?\n");
			} while ((c = scanf("%i",&xp)) != 1 || xp < 0);
			int res = scanf("%*c");
			sprintf(msg,"debug_enemies_room() scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			checkremainder(player,xp);
		}
		break;
		case '1': {
			picked_debug_proc = 1;
			int q=-1;
			do {
				printf("\nInsert consumable number:\n");
			} while ((c = scanf("%i",&n)) != 1 || n > CONSUMABLESMAX || n < 0 );
			int res = scanf("%*c");
			sprintf(msg,"debug_enemies_room() scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			do {
				printf("\nInsert quantity:\n");
			} while ((c = scanf("%i",&q)) != 1 && q <= 0 );
			res = scanf("%*c");
			sprintf(msg,"debug_enemies_room() 2 scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			Consumable* c = (Consumable*) player->consumablesBag[n];
			c->qty += q;
			player->stats->consumablesfound += q;
		}
		break;
		case '2': {
			picked_debug_proc = 1;
			statReset(player,1);
		}
		break;
		case '3': {
			picked_debug_proc = 1;
			statResetEnemy(e,1);
		}
		break;
		case '4': {
			picked_debug_proc = 1;
			printf("\nCurrent luck: %i\tRL: %i\n",player->luck,p->luck);

			do {
				printf("\nInsert new luck:\n");
			} while ((c = scanf("%i",&n)) != 1 && n > 0 );
			int res = scanf("%*c");
			sprintf(msg,"debug_enemies_room() 3 scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
				//FIXME:
				//Copy-pasted the calc for player luck... might need a function
				p->luck = n;

				player->luck = (p->luck * MAXPLAYERLUCK) / MAXLUCK ;
		}
		break;
		case '5': {
			picked_debug_proc = 1;
			/* Old code invoking the macro for special moves directly. Used for testing.
			 * int s = -1;
			 * printf("Insert special num:\n");
			 * scanf("%i",&s);
			 * scanf("%*c");
			 * printf("Read: %i\n",s);
			 * GET_CALLBACK(s,callback_special_t)(player,e,p,roomIndex,currentEnemyNum);
			*/

			//printActivePerks(player);
			int c;
			int n=-1;
			do {
				printf("\nInsert coin number (0<n<100):\n");
			} while ((c = scanf("%i",&n)) < 0 || c > 3 || n < 0 || n > 100 );
			int res = scanf("%*c");
			sprintf(msg,"debug_enemies_room() 4 scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			player->balance += n;
		}
		break;
		case '6': {
			picked_debug_proc = 1;
			unlockSpecial(player);
		}
		break;
		case '7': {
			picked_debug_proc = 1;
			int n=-1;
			do {
				printf("\nInsert artifact number (0<n<%i):\n",ARTIFACTSMAX + 1);
			} while ((c = scanf("%i",&n)) != 1 || n > ARTIFACTSMAX || n < 0 );

			int res = scanf("%*c");
			sprintf(msg,"debug_enemies_room() scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			player->artifactsBag[n]->qty += 1;
			player->artifactsBag[n]->active = 0;
			player->stats->artifactsfound += 1;
		}
		break;
		case '8': {
			picked_debug_proc = 1;
			int res = system("clear");
			sprintf(msg,"debug_enemies_room() 3 system(\"clear\") res was (%i)",res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			printf("\nPlayer Counters:\n");
			printCounters((Turncounter **)player->counters);
			printf("\nEnemy (%s) Counters:\n",stringFromEClass(e->class));
			printCounters((Turncounter **)e->counters);
			printf("\nPress Enter to resume game");
			res = scanf("%*c");
			sprintf(msg,"debug_enemies_room() 5 scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
		}
		break;
		case '9': {
			picked_debug_proc = 1;
			int q = -1;
			int c = -1;
			do {
				printf("\nInsert quantity:\n");
			} while ((c = scanf("%i",&q)) != 1 && q <= 0 );
			int res = scanf("%*c");
			sprintf(msg,"debug_enemies_room() 6 scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);

			//TODO: handle dropping an equip with a new notification window
			//for (int i=0; i<q;i++) {dropEquip(player,1,notify_win);};
			log_tag("debug_log.txt","[DEBUG]","TODO: not working ATM.");
		}
		break;
		case 'd': {
			picked_debug_proc = 1;
			printf("\nVERSION:    %s\n",VERSION);
			printf("\nSystem:    %s\n",uts.sysname);
			printf("\nOS Release:    %s\n",uts.release);
			printf("\nOS Version:    %s\n",uts.version);
			printf("\nMachine:    %s\n",uts.machine);
			printf("\nGAMEMODE:    %s\n",stringFromGamemode(GAMEMODE));
			printf("\nPath->current_saveslot->save_path:    %s\n",p->current_saveslot->save_path);
			printf("\nGS_AUTOSAVE_ON:    %i\n",GS_AUTOSAVE_ON);
			printf("\nG_FASTQUIT_ON:    %i\n",G_FASTQUIT_ON);
			printf("\nG_DEBUG_ON:    %i\n",G_DEBUG_ON);
			printf("\nG_LOG_ON:    %i\n",G_LOG_ON);
			printf("\nG_GODMODE_ON:    %i\n",G_GODMODE_ON);
			printf("\nG_EXPERIMENTAL_ON:    %i\n",G_EXPERIMENTAL_ON);
			printf("\nG_DEBUG_ROOMTYPE_ON:    %i\n",G_DEBUG_ROOMTYPE_ON);
			if (G_DEBUG_ROOMTYPE_ON == 1) {
				printf("\nG_DEBUG_ROOMTYPE:    %i\n",G_DEBUG_ROOMTYPE);
			}
			printf("\nG_DEBUG_ENEMYTYPE_ON:    %i\n",G_DEBUG_ENEMYTYPE_ON);
			if (G_DEBUG_ENEMYTYPE_ON == 1) {
				printf("\nG_DEBUG_ENEMYTYPE:    %i\n",G_DEBUG_ENEMYTYPE);
			}
			printf("\nPress Enter to resume game.\n");
			int res = scanf("%*c");
			sprintf(msg,"debug_enemies_room() 7 scanf() res was (%i)", res);
			log_tag("debug_log.txt","[DEBUG]",msg);
		}
		break;
		case 'f': {
			clear();
			refresh();
			debug_printFoeParty(room->foes);
		}
		break;
		case 'T': {
			char path_to_kls_file[600];
			char static_path[500];
			// Set static_path value to the correct static dir path
			resolve_staticPath(static_path);

			//Append to "kls_log.txt"
			sprintf(path_to_kls_file,"%s/%s",static_path,"debug_log.txt");
			FILE* kls_file = NULL;
			kls_file = fopen(path_to_kls_file, "a");
			if (kls_file == NULL) {
				fprintf(stderr,"debug_generic():  failed opening debug logfile.\n");
				exit(EXIT_FAILURE);
			}
			if (kls == NULL) {
				fprintf(stderr,"debug_generic():  kls was NULL.\n");
				exit(EXIT_FAILURE);
			}
			fprintf(kls_file,"--BEGIN debug of temporary_kls--\n");
			print_kls_2file(kls_file,temporary_kls);
			kls_showList_toFile(kls_reverse(temporary_kls->regs),kls_file);
			kls_usageReport_toFile(temporary_kls,kls_file);
			fprintf(kls_file,"--END debug of temporary_kls--\n\n");
			WINDOW* win = NULL;
			/* Initialize curses */
			clear();
			refresh();
			start_color();
			cbreak();
			noecho();
			keypad(stdscr, TRUE);
			win = newwin(20, 50, 1, 2);
			keypad(win, TRUE);
			wclear(win);
			wrefresh(win);
			kls_showList_toWin(temporary_kls,win);
			delwin(win);
			endwin();

			fclose(kls_file);
		}
		break;
		case 'K': {
			char path_to_kls_file[600];
			char static_path[500];
			// Set static_path value to the correct static dir path
			resolve_staticPath(static_path);

			//Append to "kls_log.txt"
			sprintf(path_to_kls_file,"%s/%s",static_path,"debug_log.txt");
			FILE* kls_file = NULL;
			kls_file = fopen(path_to_kls_file, "a");
			if (kls_file == NULL) {
				fprintf(stderr,"debug_generic():  failed opening debug logfile.\n");
				exit(EXIT_FAILURE);
			}
			if (kls == NULL) {
				fprintf(stderr,"debug_generic():  kls was NULL.\n");
				exit(EXIT_FAILURE);
			}
			fprintf(kls_file,"--BEGIN debug of passed kls--\n");
			print_kls_2file(kls_file,kls);
			kls_showList_toFile(kls_reverse(kls->regs),kls_file);
			kls_usageReport_toFile(kls,kls_file);
			fprintf(kls_file,"--END debug of passed kls--\n\n");
			WINDOW* win = NULL;
			/* Initialize curses */
			clear();
			refresh();
			start_color();
			cbreak();
			noecho();
			keypad(stdscr, TRUE);
			win = newwin(20, 50, 1, 2);
			keypad(win, TRUE);
			wclear(win);
			wrefresh(win);
			kls_showList_toWin(kls,win);
			delwin(win);
			endwin();
			fclose(kls_file);
		}
		break;
		case 'A': {
			GS_AUTOSAVE_ON = (GS_AUTOSAVE_ON == 1 ? 0 : 1);
			sprintf(msg,"Toggled G_AUTOSAVE_ON, new value: (%i)", GS_AUTOSAVE_ON);
			log_tag("debug_log.txt","[DEBUG]",msg);
		}
		break;
		case 'Q': {
			G_FASTQUIT_ON = (G_FASTQUIT_ON == 1 ? 0 : 1);
			sprintf(msg,"Toggled G_FASTQUIT_ON, new value: (%i)", G_FASTQUIT_ON);
			log_tag("debug_log.txt","[DEBUG]",msg);
		}
		break;
		case 'L': {
			G_LOG_ON = (G_LOG_ON == 1 ? 0 : 1);
			sprintf(msg,"Toggled G_LOG_ON, new value: (%i)", G_LOG_ON);
			log_tag("debug_log.txt","[DEBUG]",msg);
		}
		break;
		case 'g': {
			G_GODMODE_ON = (G_GODMODE_ON == 1 ? 0 : 1);
			sprintf(msg,"Toggled G_GODMODE_ON, new value: (%i)", G_GODMODE_ON);
			log_tag("debug_log.txt","[DEBUG]",msg);
		}
		break;
		case 's': { //Query sprites slideshow
			picked_debug_proc = 1;
			int res = system("clear");
			sprintf(msg,"debug_enemies_room() 4 system(\"clear\") res was (%i)",res);
			log_tag("debug_log.txt","[DEBUG]",msg);
			int comm_len = -1;
			int picked_comm = 0;
			char comm[15] = "empty";
			char* desc = NULL;
			int sprite_total = -1;
			int tot_cons = CONSUMABLESMAX+1;
			int tot_enemy = ENEMYCLASSESMAX+1;
			int tot_boss = BOSSCLASSESMAX+1;
			int tot_art = ARTIFACTSMAX+1;
			int tot_eq = EQUIPSMAX+1;
			int tot_eqzones = EQUIPZONES+1;
			int tot_misc = MISC_SPRITES_MAX+1;
			int allsprites =
				tot_cons +
				tot_enemy +
				tot_boss +
				tot_art +
				tot_eq +
				tot_eqzones +
				tot_misc;

			printf("\nConsumables sprites: \t%d\
				\nArtifacts sprites: \t%d\
				\nEquips sprites: \t%d\
				\nEquip zone sprites: \t%d\
				\nEnemies sprites: \t%d\
				\nBosses sprites: \t%d\
				\nOther misc sprites: \t%d\n", tot_cons, tot_art, tot_eq, tot_eqzones, tot_enemy, tot_boss, tot_misc);
			printf("--------------------------");
			printf("\nTotal sprites: \t\t%d\n",allsprites);
			printf("\nInsert sprites query, q to quit: (q|boss|cons|equip|eqzone|artf|enemy|misc):\n\n[%s@spriteshow]$ ",player->name);
			while (!picked_comm && (comm_len = scanf("%10s",comm)) > -1) {
				int res = scanf("%*c");
				sprintf(msg,"debug_enemies_room() 7 scanf() res was (%i)", res);
				log_tag("debug_log.txt","[DEBUG]",msg);
				if (strcmp(comm,"q") == 0) {
					return;
				} else if (strcmp(comm,"boss") == 0) {
					picked_comm = 1;
					sprite_total = BOSSCLASSESMAX+1;
				} else if (strcmp(comm,"cons") == 0) {
					picked_comm = 1;
					sprite_total = CONSUMABLESMAX+1;
				} else if (strcmp(comm,"equip") == 0) {
					picked_comm = 1;
					sprite_total = EQUIPSMAX+1;
				} else if (strcmp(comm,"eqzone") == 0) {
					picked_comm = 1;
					sprite_total = EQUIPZONES+1;
				} else if (strcmp(comm,"artf") == 0) {
					picked_comm = 1;
					sprite_total = ARTIFACTSMAX+1;
				} else if (strcmp(comm,"enemy") == 0) {
					picked_comm = 1;
					sprite_total = ENEMYCLASSESMAX+1;
				} else if (strcmp(comm,"misc") == 0) {
					picked_comm = 1;
					sprite_total = MISC_SPRITES_MAX+1;
				} else {
					printf("\nInsert sprites query, q to quit: (q|boss|cons|equip|eqzone|artf|enemy|misc):\n\n[%s@spriteshow]$ ",player->name);
				}
			}
			int sprite_count = -1;
			int c = -1;
			char s[20];
			int startx = 0;
			int x,y;
			WINDOW* spriteshow_win;
			/* Initialize curses */
			//setlocale(LC_CTYPE, "it_IT.UTF-8");
			//initscr();
			start_color();
			clear();
			refresh();
			cbreak();
			noecho();
			keypad(stdscr, TRUE);
			spriteshow_win = newwin(19, 19, 2, 2);
			keypad(spriteshow_win, TRUE);
			for (sprite_count=0; sprite_count < sprite_total; sprite_count++) {
				if (strcmp(comm,"boss") == 0) {
					desc = stringFromBossClass(sprite_count);
				} else if (strcmp(comm,"cons") == 0) {
					desc = stringFromConsumables(sprite_count);
				} else if (strcmp(comm,"equip") == 0) {
					desc = stringFromEquips(sprite_count);
				} else if (strcmp(comm,"eqzone") == 0) {
					desc = stringFromEquipzones(sprite_count);
				} else if (strcmp(comm,"artf") == 0) {
					desc = stringFromArtifacts(sprite_count);
				} else if (strcmp(comm,"enemy") == 0) {
					desc = stringFromEClass(sprite_count);
				} else if (strcmp(comm,"misc") == 0) {
					desc = "Misc";
				}
				wclear(spriteshow_win);
				wrefresh(spriteshow_win);
				wclear(stdscr);
				wrefresh(stdscr);
				sprintf(msg,"Showing sprite n.%d, class is: %s\n", sprite_count, desc);
				log_tag("debug_log.txt","[DEBUG]",msg);
				mvwprintw(stdscr, 5, 20, "Sprite for: \'%s\'", desc);
				mvwprintw(stdscr, 7, 20, "(%i/%i)", sprite_count+1, sprite_total);
				refresh();
				for (int i = 0; i < 8; i++) {
					if (strcmp(comm,"boss") == 0) {
						strcpy(s, bosses_sprites[sprite_count][i]);
					} else if (strcmp(comm,"cons") == 0) {
						strcpy(s, consumables_sprites[sprite_count][i]);
					} else if (strcmp(comm,"equip") == 0) {
						strcpy(s, equips_sprites[sprite_count][i]);
					} else if (strcmp(comm,"eqzone") == 0) {
						strcpy(s, equipzones_sprites[sprite_count][i]);
					} else if (strcmp(comm,"artf") == 0) {
						strcpy(s, artifacts_sprites[sprite_count][i]);
					} else if (strcmp(comm,"enemy") == 0) {
						strcpy(s, enemies_sprites[sprite_count][i]);
					} else if (strcmp(comm,"misc") == 0) {
						strcpy(s, misc_sprites[sprite_count][i]);
					}
					//sprintf(msg,"[DEBUG]    Copied string: \'%s\'\n", s);
					//debug_log("debug_log.txt",msg);

					for (int j = 0; j < 13; j++) {
						x = startx + 1 + j;
						y = 1 + i ;

						print_encoded_char(spriteshow_win, y, x, s[j]);
					}//End for line print
				}//End for sprite print

				wrefresh(spriteshow_win);
				//printf("\nPress Enter for next, q to quit.\n");
				int picked = 0;
				int go_previous = 0;
				int quit_show = 0;
				while (!picked && ((c = wgetch(spriteshow_win)) != KEY_F(1))) {
					if (c == 10 || c == KEY_RIGHT) { //Enter, right
							sprintf(msg,"Going to next sprite.\n");
							log_tag("debug_log.txt","[DEBUG]",msg);
							wclear(spriteshow_win);
							wrefresh(spriteshow_win);
							picked = 1;
							continue; //Go to next sprite
					} else if (c == 'q') { //Quit
						sprintf(msg,"Stopping query sprite show.\n");
						log_tag("debug_log.txt","[DEBUG]",msg);
						picked = 1;
						quit_show = 1;
					} else if (c == KEY_LEFT ) { //Go to previous sprite
						go_previous = 1;
						sprintf(msg,"Going to previous sprite.\n");
						log_tag("debug_log.txt","[DEBUG]",msg);
						picked = 1;
					} else { //Unexpected char
						sprintf(msg,"Wrong operation. Continuing with next sprite.\n");
						log_tag("debug_log.txt","\033[1;31m[ERROR]",msg);
						picked = 1;
						continue; //We still go to next sprite
					}
				} //End while wait for user input
				if (go_previous) {
					sprite_count--;
					sprite_count--;
					if (sprite_count < -1) sprite_count = -1;
					go_previous=0;
				}
				if (quit_show) {
					break;
				}
			} //End for all sprites
			sprintf(msg,"End of sprite show.\n");
			log_tag("debug_log.txt","[DEBUG]",msg);

			delwin(spriteshow_win);
			endwin();
		}
		break;
		case 'q': {
			return;
		}
		default : { //Echo the passed char and ask for one more.
			char cmd[50];
			sprintf(cmd,"\necho \"%c\\n\"\n%c\n\n",ch[0],ch[0]);
			printf("%s",cmd);
			screenTime(1);
		}
		break;
	} //Close switch on ch[0]
	} //Close while !picked_debug_proc

}

/**
 * Takes a Fighter, a Room and a loadInfo pointers, and prints fighter stats and a quitting message, before quitting the program and freeing Room.
 *
 * @see Fighter
 * @see printStats()
 * @see death()
 * @param p The Fighter pointer at hand.
 * @param room The Room pointer at hand.
 * @param load_info The loadInfo pointer at hand.
 * @param t_kls The Koliseo_Temp to end if possible.
 */
void quit(Fighter* p, Room* room, loadInfo* load_info, Koliseo_Temp* t_kls) {
	char msg[500];
	endwin();
	int res = system("reset");
	sprintf(msg,"quit() system(\"reset\") res was (%i)",res);
	log_tag("debug_log.txt","[DEBUG]",msg);
	//printf("\n\n\tTHANKS 4 PLAYING\n");
	//FIXME
	//dropping out of the Koliseo scope might render stat pointer invalid.
	//Can't we print stats and clear the kls?
	//printStats(p);
	//printf("\n");
	sprintf(msg,"Resetting Koliseo_Temp from: (%li)",t_kls->kls->offset);
	kls_log("DEBUG",msg);
	kls_temp_end(*t_kls);
	sprintf(msg,"Koliseo now at: (%li)",t_kls->kls->offset);
	kls_log("DEBUG",msg);
	death(p,load_info);
	//FIXME:
	//Calling this segfaults?
	//freeRoom(room);
	log_tag("debug_log.txt","[DEBUG]","Quitting program.");
	exit(EXIT_SUCCESS);
}

/**
 * Takes a Fighter pointer and deleted all the equips not in use, granting a payment to the Fighter balance.
 * @see Shop
 * @see handleRoom_Shop()
 * @see Fighter
 * @param f The Fighter pointer at hand.
 */
void sell_all_equips(Fighter* f, Koliseo_Temp* t_kls) {
	Koliseo_Temp tkls = *t_kls;
	char msg[200];

	Equip* saved_equips[EQUIPZONES+1];
	int saved_count = 0;

	for (int i = 0; i < EQUIPZONES+1; i++) {
		if (f->equipslots[i]->active) {
			sprintf(msg,"sell_all_equips(): Prepping Equip to save f->equipslot[%i]",i);
			log_tag("debug_log.txt","[DEBUG]",msg);
			kls_log("DEBUG",msg);
			Equip* saved = (Equip*) KLS_PUSH_T_NAMED(tkls,Equip,1,"Equip",msg);
			Equip* to_save = f->equipslots[i]->item;

			saved->class = to_save->class;
			saved->type = to_save->type;
			strcpy(saved->name,to_save->name);
			strcpy(saved->desc,to_save->desc);
			saved->qty = to_save->qty;
			saved->equipped = 0 ; //Will be set after when re-equipped
			saved->level = to_save->level ;
			saved->atk = to_save->atk ;
			saved->def = to_save->def ;
			saved->vel = to_save->vel ;
			saved->enr = to_save->enr ;
			saved->bonus = to_save->bonus ;
			saved->perksCount = 0; //Will be set during perks copy
			saved->qual = to_save->qual ;
			saved->equip_fun = to_save->equip_fun ;

			for (int j = 0; j < to_save->perksCount; j++) {
				sprintf(msg,"sell_all_equips(): Prepping Perk (%i) to save f->equipslot[%i]", j, i);
				log_tag("debug_log.txt","[DEBUG]",msg);
				kls_log("DEBUG",msg);
				Perk* save_pk = (Perk*) KLS_PUSH_T_NAMED(tkls,Perk,1,"Perk",msg);
				save_pk->class = to_save->perks[j]->class;
				strcpy(save_pk->name, to_save->perks[j]->name);
				strcpy(save_pk->desc, to_save->perks[j]->desc);
				save_pk->innerValue = to_save->perks[j]->innerValue;
				saved->perks[saved->perksCount] = save_pk;
				saved->perksCount++;
			}

			for (int j = 0 ; j < 8; j++) {
				strcpy(saved->sprite[j],to_save->sprite[j]);
			}

			saved_equips[saved_count] = saved;
			saved_count++;
		}
	}

	int deleted_count = 0;
	int pay = 0;

	for (int i = 0; i < f->equipsBagOccupiedSlots; i++) {
		Equip* toDel = f->equipsBag[i];
		pay += toDel->cost/2;
		//int perksTot = toDel->perksCount;
		/*
		for (int j = 0; j < perksTot; j++) {
			Perk* pk = toDel->perks[j];
			free(pk);
		}
		*/
		//FIXME: are we deleting this correctly?
		//free(toDel);
		deleted_count++;
	}


	f->equipsBagOccupiedSlots -= deleted_count;

	for (int i = 0; i < saved_count; i++) {
		f->equipsBag[i] = saved_equips[i];
		f->equipslots[i]->item = saved_equips[i];
		saved_equips[i]->equipped = 1;
		f->equipsBagOccupiedSlots ++;
	}
	f->earliestBagSlot = f->equipsBagOccupiedSlots;

	f->balance += pay;

}

/**
 * Takes a Room pointer and a Fighter pointer, displays a choice for the next room and sets it at *roadFork_value.
 * @see Roadfork
 * @see Fighter
 * @param room The pointer to current room.
 * @param roadFork_value The pointer to the value for next room's class.
 * @param roomsDone The total of rooms completed.
 * @param path The Path pointer.
 * @param f The Fighter pointer at hand.
 * @return When room is exited, should return NO_DMG.
 */
int handleRoom_Roadfork(Room* room, int* roadFork_value, int roomsDone, Path* path, Fighter* f) {
	//Strings for turn menu choices
	char choices[ROADFORK_OPTIONS_MAX+1][80] = {
		"One",
		"Two"
	};
	//TODO: the choices array had a '(char) NULL' value which was giving a obvious warning about pointer to integer casting.
	//Was it needed as a sentinel value??


	char msg[200];

	for (int i = 0; i < 2; i++) {
		char c[80];
		switch (room->roadfork->options[i]) {
			case ROADFORK_BOSS: {
				strcpy(c,"Boss Room");
					    }
			break;
			case ROADFORK_SHOP: {
				strcpy(c,"Shop Room");
					    }
			break;
			case ROADFORK_TREASURE: {
				strcpy(c,"Treasure Room");
					    }
			break;
			case ROADFORK_ENEMIES: {
				strcpy(c,"Enemies");
					    }
			break;
			case ROADFORK_ROADFORK: {
				strcpy(c,"Roadfork");
					    }
			break;
		}
		strcpy(choices[i],c);
	}
	sprintf(msg,"Entered Roadfork with %s and %s.\n", choices[0],choices[1]);
	log_tag("debug_log.txt","[ROADFORK]",msg);

	ITEM **my_items;
	MENU *my_menu;
        WINDOW *my_menu_win;
        int n_choices, c;

	/* Initialize curses */
	//setlocale(LC_CTYPE, "it_IT.UTF-8");
	//initscr();
	start_color();
	clear();
	refresh();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	init_game_color_pairs();

	/* Create items */
	n_choices = ARRAY_SIZE(choices);
	sprintf(msg,"n_choices size was: (%i)\n", n_choices);
	log_tag("debug_log.txt","[ROADFORK]",msg);
	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	for(int k = 0; k < n_choices; k++) {
		my_items[k] = new_item(choices[k], choices[k]);
		/* Set the user pointer */
		//set_item_userptr(my_items[i]);
	}

	/* Create menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Set description off */
	menu_opts_off(my_menu,O_SHOWDESC);

        /* Create the window to be associated with the menu */
        my_menu_win = newwin(12, 28, 1, 1);
        keypad(my_menu_win, TRUE);

        /* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 4, 26, 3, 2));
	set_menu_format(my_menu, 3, 1);

	/* Set menu mark to the string " >  " */
        set_menu_mark(my_menu, " >  ");

	char label[25];
	sprintf(label,"Road Fork");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_label(my_menu_win, 1, 0, 28, label, COLOR_PAIR(8));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 26);
	mvwaddch(my_menu_win, 2, 27, ACS_RTEE);

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	/* Create the items window */
        //win = newwin(22, 54, 1, 23);
        //keypad(win, TRUE);
	//box(win, 0, 0);

	//print_label(win, 1, 0, 54, "Option", COLOR_PAIR(6));
	//mvwaddch(win, 2, 0, ACS_LTEE);
	//mvwhline(win, 2, 1, ACS_HLINE, 52);
	//mvwaddch(win, 2, 53, ACS_RTEE);

	attron(COLOR_PAIR(3));
	mvprintw(20, 2, "Arrows to move");
	mvprintw(21, 2, "(q to Exit)");
	attroff(COLOR_PAIR(3));
	refresh();

	int end_room = 0;
	int check = -1;

	while(!end_room && (c = wgetch(my_menu_win)) != KEY_F(1)) {
		switch(c) {
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
			        menu_driver(my_menu, REQ_UP_ITEM);
			        break;
			case KEY_NPAGE:
				menu_driver(my_menu, REQ_SCR_DPAGE);
				break;
			case KEY_PPAGE:
			        menu_driver(my_menu, REQ_SCR_UPAGE);
			        break;
			case 10: // Enter
				{
					ITEM *cur;

				        cur = current_item(my_menu);
					sprintf(msg,"Picked %s.\n", item_name(cur));
					log_tag("debug_log.txt","[ROADFORK]",msg);
					if ((check = strcmp("Boss Room",item_name(cur))) == 0) {
						*roadFork_value = BOSS;
						end_room = 1;
					} else if ( (check = strcmp("Shop Room",item_name(cur)) == 0) ) {
						*roadFork_value = SHOP;
						end_room = 1;
					} else if ( (check = strcmp("Treasure Room",item_name(cur)) == 0) ) {
						*roadFork_value = TREASURE;
						end_room = 1;
					} else if ( (check = strcmp("Enemies",item_name(cur)) == 0) ) {
						*roadFork_value = ENEMIES;
						end_room = 1;
					} else if ( (check = strcmp("Roadfork",item_name(cur)) == 0) ) {
						*roadFork_value = ROADFORK;
						end_room = 1;
					}
				        pos_menu_cursor(my_menu);
					refresh();
			        };
			        break;
		}
                wrefresh(my_menu_win);
	}
	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        free_menu(my_menu);
        for(int k = 0; k < n_choices; k++) {
		free_item(my_items[k]);
	}
	endwin();

	return FIGHTRES_NO_DMG;
}

/**
 * Takes an integer seed and returns a Path pointer.
 * The seed provided is used to set the random seed and initialise the path values.
 * @see Path
 * @see MAXLENGTH
 * @see MAXLUCK
 * @param seed An integer seed.
 * @param kls The Koliseo used for allocation.
 * @param current_saveslot The Saveslot used to init the Path.
 * @return A Path pointer with stats.
 */
Path* randomise_path(int seed, Koliseo* kls, const char* path_to_savefile){
	char msg[200];
	sprintf(msg,"Prepping Path");
	kls_log("DEBUG",msg);
	Path* p = (Path*) KLS_PUSH_NAMED(kls, Path, 1, "Path", msg);
	srand(seed);
	sprintf(msg,"Prepping Saveslot");
	kls_log("DEBUG",msg);
	sprintf(msg,"save_path: [%s]",path_to_savefile);
	Saveslot* save = (Saveslot*) KLS_PUSH_NAMED(kls, Saveslot, 1, "Saveslot", msg);
	sprintf(msg,"Seed: %i",seed);
	strcpy(save->name,msg);
	sprintf(msg,"%s",path_to_savefile);
	strcpy(save->save_path,msg);
	p->current_saveslot = save;
	sprintf(msg,"Prepped Saveslot:  path->current_saveslot->save_path == [%s]",p->current_saveslot->save_path);
	kls_log("DEBUG",msg);
	log_tag("debug_log.txt","[SAVESLOT]",msg);

	switch(GAMEMODE) {
		case Standard: {
			p->length = (rand() % MAXLENGTH ) + 1;
			p->luck = (rand() % MAXLUCK ) + 1;
			p->prize = 15 / p->luck * (rand() % 150) + 500;
		}
		break;
		case Story: {
			p->length = 41;
			p->luck = (rand() % MAXLUCK ) + 1;
			p->prize = 15 / p->luck * (rand() % 150) + 500;
		}
		break;
		case Rogue: {
			p->length = 1;
			p->luck = (rand() % MAXLUCK ) + 1;
			p->prize = 15 / p->luck * (rand() % 150) + 500;
		}
		break;
		default: {
			fprintf(stderr,"\nUnexpected GAMEMODE value %i.\n",GAMEMODE);
			exit(EXIT_FAILURE);
		}
	}
	return p;
}

/**
 * Takes a integer and a string array (possibly from main).
 * Initialises a Path pointer and a Fighter pointer, before looping for each oom in path length by calling the correct room function.
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
 */
void gameloop(int argc, char** argv){

  char* whoami; // This will reference argv[0] at basename, it's the same string in memory, just starting later
  //Init default_kls
  default_kls = kls_new(KLS_DEFAULT_SIZE*8);
  temporary_kls = kls_new(KLS_DEFAULT_SIZE*8);

  (whoami = strrchr(argv[0], '/')) ? ++whoami : (whoami = argv[0]);

  char* kls_progname = (char*) KLS_PUSH_NAMED(default_kls, char*, sizeof(whoami),"progname",whoami);
  strcpy(kls_progname,whoami);

	do {
		char msg[1500]; //This has a big scope.
		FILE *debug_file = NULL;
		FILE *OPS_debug_file = NULL;
		// Parse command-line options
		int option;
		loadInfo* load_info = (loadInfo*) KLS_PUSH_NAMED(default_kls, loadInfo, 1, "loadInfo","loadInfo");

		load_info->is_new_game = 1; //By default we do a new game
		load_info->enemy_index = -1;
		load_info->total_foes = -1;
		load_info->save_type = -1;
		int loaded_roomtotalenemies = -1;
		int loaded_roomindex = -1;
		load_info->ptr_to_roomtotalenemies = &loaded_roomtotalenemies;
		load_info->ptr_to_roomindex = &loaded_roomindex;

		while ((option = getopt(argc, argv, "f:r:E:tTGRXQLlvdhsa")) != -1) {
			switch (option) {
				case 'd': {
					G_DEBUG_ON += 1;
					G_LOG_ON = 1;
				}
				break;
				case 'r': {
					G_DEBUG_ROOMTYPE_ON += 1;
				}
				break;
				case 'E': {
					G_DEBUG_ENEMYTYPE_ON += 1;
				}
				break;
				case 'L': {
					G_LOG_ON = 1;
				}
				break;
				case 'l': {
					load_info->is_new_game = 0;
				}
				break;
				case 'G': {
					G_GODMODE_ON = 1;
				}
				break;
				case 'Q': {
					G_FASTQUIT_ON = 1;
				}
				break;
				case 'X': {
					G_EXPERIMENTAL_ON = 1;
				}
				break;
				case 'a': {
					GS_AUTOSAVE_ON = 0;
				}
				break;
				case 's': {
					GAMEMODE = Story;
				}
				break;
				case 'R': {
					GAMEMODE = Rogue;
				}
				break;
				case 'f': {
					//filename = optarg;
				}
				break;
				case 'h': {
					usage(whoami);
					exit(EXIT_SUCCESS);
				}
				break;
				case 'T': {
					handleTutorial();
					usage(whoami);
					exit(EXIT_SUCCESS);
				}
				break;
				case 't': {
					//Test all colors
					printFormattedVersion(whoami);
					printf("Using:\n");
					printf("  \'animate\' :\n    s4c/animate.h    ");
					S4C_ECHOVERSION();
					printf("[DEBUG]    Testing terminal color capabilities.\n");
					screenTime(1);
					display_colorpairs();
					exit(EXIT_SUCCESS);
				}
				break;
				case 'v': {
					printVersion();
					/*
					printf("Using:\n");
					printf("  \'animate\' :\n    s4c/animate.h    ");
					S4C_ECHOVERSION();
					printf("\n  \'anvil\' :\n");
					int status = system("echo \"    $( anvil -vv 2>/dev/null ) \"");
					int exitcode = status / 256;
					if (exitcode != 0) {
						printf("\033[1;31m[DEBUG]\e[0m    \"anvil -vv\" failed.\n\n    Maybe amboso is not installed globally?\n");
						exit(exitcode);
					}
					exit(exitcode);
					*/
					exit(EXIT_SUCCESS);
				}
				break;
				case '?': {
					fprintf(stderr,"Invalid option: %c\n Check your arguments.\n", option);
					usage(whoami);
					// Handle invalid options
					exit(EXIT_FAILURE);
				}
				break;
				default: {
					// Should never get here
					fprintf(stderr,"Invalid option: %c\n, bad usage.\n", option);
					exit(EXIT_FAILURE);
				}
				break;
			}
		}
		// Open log file if log flag is set and reset it
		if (G_LOG_ON == 1) {
			KOLISEO_DEBUG = 1;
			char path_to_debug_file[600];
			char path_to_kls_debug_file[600];
			char path_to_OPS_debug_file[600];
			char static_path[500];
			// Set static_path value to the correct static dir path
			resolve_staticPath(static_path);

			//Truncate "debug_log.txt"
			sprintf(path_to_debug_file,"%s/%s",static_path,"debug_log.txt");
			debug_file = fopen(path_to_debug_file, "w");
			if (!debug_file) {
				endwin(); //TODO: Can/should we check if we have to do this only in curses mode?
				fprintf(stderr,"[ERROR]    Can't open debug logfile (%s/debug_log.txt).\n", static_path);
				exit(EXIT_FAILURE);
			}
			if (KOLISEO_DEBUG == 1) {
				sprintf(path_to_kls_debug_file,"%s/%s",static_path,"kls_debug_log.txt");
				KOLISEO_DEBUG_FP = fopen(path_to_kls_debug_file,"w");
				if (!KOLISEO_DEBUG_FP) {
					endwin(); //TODO: Can/should we check if we have to do this only in curses mode?
					fprintf(stderr,"[ERROR]    Can't open kls debug logfile (%s/kls_debug_log.txt).\n", static_path);
					exit(EXIT_FAILURE);
				}
				fprintf(KOLISEO_DEBUG_FP,"[DEBUGLOG]    --Debugging KLS to kls_debug_log.txt--  \n");
			}
			fprintf(debug_file,"[DEBUGLOG]    --New game--  \n");
			fprintf(debug_file,"[DEBUG]    --Default kls debug info:--  \n");
  			print_kls_2file(debug_file,default_kls);
			fprintf(debug_file,"[DEBUG]    --Temporary kls debug info:--  \n");
  			print_kls_2file(debug_file,temporary_kls);
			fprintf(debug_file,"[DEBUG]    --Closing header for new game.--  \n");
			fclose(debug_file);

			//Lay debug info
			sprintf(msg,"G_DEBUG_ON == (%i)",G_DEBUG_ON);
			log_tag("debug_log.txt","[DEBUG]",msg);
			sprintf(msg,"kls_progname == (%s)",kls_progname);
			log_tag("debug_log.txt","[DEBUG]",msg);
			sprintf(msg,"G_LOG_ON == (%i)",G_LOG_ON);
			log_tag("debug_log.txt","[DEBUG]",msg);
			log_tag("debug_log.txt","[DEBUG]","small DEBUG FLAG ASSERTED");
  			sprintf(msg,"[Current position in default_kls] [pos: %li]\n",kls_get_pos(default_kls));
			log_tag("debug_log.txt","[DEBUG]",msg);

			//Truncate OPS_LOGFILE
			sprintf(path_to_OPS_debug_file,"%s/%s",static_path,OPS_LOGFILE);
			OPS_debug_file = fopen(path_to_OPS_debug_file, "w");
			if (!OPS_debug_file) {
				endwin(); //TODO: Can/should we check if we have to do this only in curses mode?
				fprintf(stderr,"[ERROR]    Can't open OPS logfile (%s/%s).\n", static_path, OPS_LOGFILE);
				exit(EXIT_FAILURE);
			}
			fprintf(OPS_debug_file,"[OPLOG]    --New game--  \n");
			fclose(OPS_debug_file);
			sprintf(msg,"Truncated [%s]",OPS_LOGFILE);
			log_tag("debug_log.txt","[DEBUG]",msg);
    		}
		if (G_DEBUG_ENEMYTYPE_ON == 1) {
			char msg[200];
			sprintf(msg,"G_DEBUG_ENEMYTYPE_ON == (%i)",G_DEBUG_ENEMYTYPE_ON);
			log_tag("debug_log.txt","[DEBUG]",msg);
			log_tag("debug_log.txt","[DEBUG]","ENEMY DEBUG FLAG ASSERTED");
			if ((G_DEBUG_ON > 0)) {
				G_DEBUG_ON += 1;
				sprintf(msg,"G_DEBUG_ON == (%i)",G_DEBUG_ON);
				log_tag("debug_log.txt","[DEBUG]",msg);
				sprintf(msg,"Forcing enemy type: (%s)",optarg);
				log_tag("debug_log.txt","[DEBUG]",msg);
				int setenemy_debug = 0;
				for (int ec=0; ec<ENEMYCLASSESMAX && (setenemy_debug == 0); ec++) {
						sprintf(msg,"Checking optarg for -E: (%s)",stringFromEClass(ec));
						log_tag("debug_log.txt","[DEBUG]",msg);
					if ((strcmp(optarg,stringFromEClass(ec)) == 0)) {
						sprintf(msg,"Match on optarg (%s), setting G_DEBUG_ENEMYTYPE to (%i).",stringFromEClass(ec),ec);
						log_tag("debug_log.txt","[DEBUG]",msg);
						G_DEBUG_ENEMYTYPE = ec;
						setenemy_debug=1;
					}
				}
				if (setenemy_debug == 0) {
					log_tag("debug_log.txt","[ERROR]","Invalid optarg for -E flag.\n");
					fprintf(stderr,"[ERROR]    Incorrect -E \"enemyType\" arg.\n");
					exit(EXIT_FAILURE);
				};
			}
		}
		if (G_DEBUG_ROOMTYPE_ON == 1) {
			sprintf(msg,"G_DEBUG_ROOMTYPE_ON == (%i)",G_DEBUG_ROOMTYPE_ON);
			log_tag("debug_log.txt","[DEBUG]",msg);
			log_tag("debug_log.txt","[DEBUG]","ROOM DEBUG FLAG ASSERTED");
			if ((G_DEBUG_ON > 0)) {
				G_DEBUG_ON += 1;
				sprintf(msg,"G_DEBUG_ON == (%i)",G_DEBUG_ON);
				log_tag("debug_log.txt","[DEBUG]",msg);
				sprintf(msg,"Forcing room type: optarg was (%s)", optarg);
				log_tag("debug_log.txt","[DEBUG]",msg);
				int setroom_debug = 0;
				for (int rc=0; (rc<ROOM_CLASS_MAX +1) && (setroom_debug == 0); rc++) {
						sprintf(msg,"Checking optarg (%s) for -R: (%s)", optarg, stringFromRoom(rc));
						log_tag("debug_log.txt","[DEBUG]",msg);
					if ((strcmp(optarg,stringFromRoom(rc)) == 0)) {
						sprintf(msg,"Match on optarg (%s), setting G_DEBUG_ROOMTYPE to (%i).",stringFromRoom(rc),rc);
						log_tag("debug_log.txt","[DEBUG]",msg);
						G_DEBUG_ROOMTYPE = rc;
						setroom_debug=1;
					}
				}
				if (setroom_debug == 0) {
					log_tag("debug_log.txt","[ERROR]","Invalid optarg for -R flag.\n");
					fprintf(stderr,"[ERROR]    Incorrect -R \"roomType\" arg.\n");
					exit(EXIT_FAILURE);
				};
			}

		}
		log_tag("debug_log.txt","[DEBUG]","Done getopt.\n");

		// Clear screen and print title, wait for user to press enter
		int clearres = system("clear");
		sprintf(msg,"gameloop() system(\"clear\") res was (%i)",clearres);
		log_tag("debug_log.txt","[DEBUG]",msg);
		printTitle();
		char c;
		yellow();
		printf("\n\n\n\n\t\t\tPRESS ENTER TO START\n\n");
		white();

		if (G_DEBUG_ON) {
			lightCyan();
			printf("\t\t\t\t\t\t\t\tDEBUG ON\n");
			white();
		}
		printf("\t\t\t\t\t\t");
                printFormattedVersion(whoami);
		int scanfres = scanf("%c",&c);
		sprintf(msg,"gameloop() scanf() res was (%i)", scanfres);
		log_tag("debug_log.txt","[DEBUG]",msg);






		// Parse positional arguments
		//for (int i = optind; i < argc; i++) {
		// Handle positional arguments
		Path* path = NULL;
		Fighter* player = NULL;

		clock_t start_time = clock(), diff_time;
		char load_msg[100];

		// Prepare the fighter frames
		char fighter_sprites[CLASSESMAX+1][MAXFRAMES][MAXROWS][MAXCOLS];

		char static_path[500];

		// Set static_path value to the correct static dir path
		resolve_staticPath(static_path);

		/*
		 * Legacy code for loading animations from an s4c-file.
		char fighter_filename[600];
		FILE* fighter_sprite_file;
		*/
		for (int i = 0; i< CLASSESMAX+1; i++) {

		      int n_load_frames = 60;
		      int n_load_rows = 17;
		      int n_load_cols = 17;

			switch (i) {
				case Knight: {
				        copy_animation(knight_tapis,fighter_sprites[i],n_load_frames,n_load_rows,n_load_cols);
				}
				break;
				case Mage: {
				        copy_animation(mage_spark,fighter_sprites[i],n_load_frames,n_load_rows,n_load_cols);
				}
				break;
				case Archer: {
				        copy_animation(archer_drop,fighter_sprites[i],n_load_frames,n_load_rows,n_load_cols);
				}
				break;
				case Assassin: {
				        copy_animation(assassin_poof,fighter_sprites[i],n_load_frames,n_load_rows,n_load_cols);
				}
				break;
				default: {
					sprintf(msg,"Unexpected fighterclass index while loading animation for class (%i): [%s]",i,stringFromClass(i));
					log_tag("debug_log.txt","[ERROR]",msg);
					exit(EXIT_FAILURE);
				}
				break;
			}

			/*
			 * Legacy code for loading animations from an s4c-file
			fighter_sprite_file = fopen(fighter_filename, "r");
			if (!fighter_sprite_file) {
				fprintf(stderr,"Error opening animation file at (%s).\n",fighter_filename);
				fprintf(stderr,"Static path was (%s).\n",static_path);
				exit(EXIT_FAILURE);
			}
			int fighter_loadCheck = load_sprites(fighter_sprites[i], fighter_sprite_file, 17, 17);
			sprintf(load_msg,"Loaded animation for %s, load result was %i.", stringFromClass(i), fighter_loadCheck);
			log_tag("debug_log.txt","[PREP]",load_msg);
			*/

		      sprintf(msg,"Copied animation from default matrix vector for: [%s] with dimensions: [%i][%i][%i].",stringFromClass(i),n_load_frames,n_load_rows,n_load_cols);
		      log_tag("debug_log.txt","[PREP]",msg);

			//Massive log of all loaded lines
			/*
		      	for (int k=0; k<MAXFRAMES+1; k++) {
				sprintf(load_msg,"[DEBUG]    frame %i:", k);
				debug_log("debug_log.txt",load_msg);
				for (int z=0; z<MAXROWS+1;z++) {
					sprintf(load_msg,"[DEBUG] %i  ( %s ).", z, fighter_sprites[i][k][z]);
					debug_log("debug_log.txt",load_msg);
				}
		      	}
			*/
		}

                //Try preloading all enemy animations
		char enemy_sprites[ENEMYCLASSESMAX+1][MAXFRAMES][MAXROWS][MAXCOLS];
		for (int i = 0; i < ( ENEMYCLASSESMAX +1); i++) {
		     /* Legacy code to load animations from an s4c-file
		      char static_path[500];

		      // Set static_path value to the correct static dir path
		      resolve_staticPath(static_path);

		      char enemy_filename[600];
		      FILE* enemy_sprite_file;
		      */

		      int n_load_frames = 60;
		      int n_load_rows = 17;
		      int n_load_cols = 17;
		      // Prepare the enemy frames
		      switch (i) {
			      case Imp: {
				      copy_animation(imp_fireball,enemy_sprites[i],n_load_frames,n_load_rows,n_load_cols);
			      }
			      break;
			      case Zombie: {
				      copy_animation(zombie_walk,enemy_sprites[i],n_load_frames,n_load_rows,n_load_cols);
			      }
			      break;
			      case Troll: {
				      copy_animation(troll_club,enemy_sprites[i],n_load_frames,n_load_rows,n_load_cols);
			      }
			      break;
			      case Ghost: {
				      copy_animation(ghost_spell,enemy_sprites[i],n_load_frames,n_load_rows,n_load_cols);
			      }
			      break;
			      case Boar: {
				      copy_animation(boar_scream,enemy_sprites[i],n_load_frames,n_load_rows,n_load_cols);
			      }
			      break;
			      case Mummy: {
				      copy_animation(mummy_shuffle,enemy_sprites[i],n_load_frames,n_load_rows,n_load_cols);
			      }
			      break;
			      case Goblin: {
				      copy_animation(goblin_shoot,enemy_sprites[i],n_load_frames,n_load_rows,n_load_cols);
			      }
			      break;
			      case Werewolf: {
				      copy_animation(werewolf_transform,enemy_sprites[i],n_load_frames,n_load_rows,n_load_cols);
			      }
			      break;
			      default: {
					sprintf(msg,"Unexpected enemy index while loading animation for enemy (%i): [%s]",i,stringFromEClass(i));
					log_tag("debug_log.txt","[ERROR]",msg);
					exit(EXIT_FAILURE);
			      }
		      }

		      /* Legacy code loading the animations from an s4c-file
			enemy_sprite_file = fopen(enemy_filename, "r");
			if (!enemy_sprite_file) {
			      fprintf(stderr,"Error opening animation file at (%s/%s).\n",static_path,enemy_filename);
			      exit(EXIT_FAILURE);
			}
			int enemy_loadCheck = load_sprites(enemy_sprites[i], enemy_sprite_file, 17, 17);
			sprintf(load_msg,"Loaded animation for %s, load result was %i.", stringFromEClass(i), enemy_loadCheck);
			log_tag("debug_log.txt","[PREP]",load_msg);
		       */

		      sprintf(msg,"Copied animation from default matrix vector for: [%s] with dimensions: [%i][%i][%i].",stringFromEClass(i),n_load_frames,n_load_rows,n_load_cols);
		      log_tag("debug_log.txt","[PREP]",msg);

			//Massive log of all loaded lines
			/*
		      for (int k=0; k<MAXFRAMES+1; k++) {
				sprintf(load_msg,"[DEBUG]    frame %i:", k);
				debug_log("debug_log.txt",load_msg);
				for (int z=0; z<MAXROWS+1;z++) {
					sprintf(load_msg,"[DEBUG] %i  ( %s ).", z, enemy_sprites[i][k][z]);
					debug_log("debug_log.txt",load_msg);
				}
		      }
			*/
		}
                //Try preloading all boss animations
		char boss_sprites[BOSSCLASSESMAX+1][MAXFRAMES][MAXROWS][MAXCOLS];
		for (int i = 0; i < ( BOSSCLASSESMAX +1); i++) {
		      int n_load_frames = 60;
		      int n_load_rows = 17;
		      int n_load_cols = 17;

		      /*
		       * Legacy code for loading animations from an s4c-file.
		      char static_path[500];

		      // Set static_path value to the correct static dir path
		      resolve_staticPath(static_path);

		      char boss_filename[600];
		      FILE* boss_sprite_file;
		      */

		      // Prepare the boss frames
		      switch (i) {
			      case Blue_Troll: {
				      copy_animation(bluetroll_wonder,boss_sprites[i],n_load_frames,n_load_rows,n_load_cols);
			      }
			      break;
			      case Headless_Ninja: {
				      copy_animation(headlessninja_throw,boss_sprites[i],n_load_frames,n_load_rows,n_load_cols);
			      }
			      break;
			      case Crawling_Dude: {
				      copy_animation(crawlingdude_crawl,boss_sprites[i],n_load_frames,n_load_rows,n_load_cols);
			      }
			      break;
			      case Sr_Warthog: {
				      copy_animation(srwarthog_square,boss_sprites[i],n_load_frames,n_load_rows,n_load_cols);
			      }
			      break;
			      case Doppelganger: {
				      copy_animation(knight_tapis,boss_sprites[i],n_load_frames,n_load_rows,n_load_cols);
			      }
			      break;
			      default: {
				      sprintf(msg,"Unexpected boss index while loading animation for boss (%i): [%s]",i,stringFromBossClass(i));
				      log_tag("debug_log.txt","[ERROR]",msg);
				      exit(EXIT_FAILURE);
			      }
		      }
		      /*
		       * Legacy code for loading animations from an s4c-file.
		      boss_sprite_file = fopen(boss_filename, "r");
		      if (!boss_sprite_file) {
			      fprintf(stderr,"Error opening animation file at (%s/%s).\n",static_path,boss_filename);
			      exit(EXIT_FAILURE);
		      }
		      int boss_loadCheck = load_sprites(boss_sprites[i], boss_sprite_file, 17, 17);
		      sprintf(load_msg,"Loaded animation for %s, load result was %i.", stringFromBossClass(i), boss_loadCheck);
		      log_tag("debug_log.txt","[PREP]",load_msg);
		      */

		      sprintf(msg,"Copied animation from default matrix vector for: [%s] with dimensions: [%i][%i][%i].",stringFromBossClass(i),n_load_frames,n_load_rows,n_load_cols);
		      log_tag("debug_log.txt","[PREP]",msg);

			//Massive log of all loaded lines
			/*
		      for (int k=0; k<MAXFRAMES+1; k++) {
				sprintf(load_msg,"[DEBUG]    frame %i:", k);
				debug_log("debug_log.txt",load_msg);
				for (int z=0; z<MAXROWS+1;z++) {
					sprintf(load_msg,"[DEBUG] %i  ( %s ).", z, enemy_sprites[i][k][z]);
					debug_log("debug_log.txt",load_msg);
				}
		      }
			*/
		}//End loading boss animations
		//End of loading animations
		clock_t animationload_time = clock() - start_time;
		double time_spent_loading_animations = (double) animationload_time / CLOCKS_PER_SEC;
		sprintf(load_msg,"Animation loading took %0.7f seconds.\n",time_spent_loading_animations);
		log_tag("debug_log.txt","[DEBUG]",load_msg);

		initscr();
		// TODO
		//Select load or new game
		ITEM **savepick_items;
		MENU *savepick_menu;
        	WINDOW *savepick_menu_win;
        	WINDOW *savepick_side_win;
		char current_save_path[300]; //Will hold picked path

		Koliseo_Temp savepick_kls = kls_temp_start(temporary_kls);

		//Declare turnOP_args
		Room* fakeroom = NULL;
		Enemy* fakeenemy = NULL;
		Boss* fakeboss = NULL;
		FILE* fakesavefile = NULL;
		WINDOW* fakenotifywin = NULL;
		turnOP_args* savepick_turn_args = init_turnOP_args(player, path, fakeroom, load_info, fakeenemy, fakeboss, fakesavefile, fakenotifywin, &savepick_kls);
 		char *savepick_choices[] = {
			"New game",
			"Load save",
			"Tutorial",
			"Quit",
			(char *)NULL,
		};
		int savepick_n_choices = ARRAY_SIZE(savepick_choices);
		//FIXME: remove magic numbers
		turnOption savepick_choice = 999;

		/* Create menu items */
		savepick_items = (ITEM **)calloc(savepick_n_choices,sizeof(ITEM *));
		for (int i = 0; i < savepick_n_choices; i++) {
			savepick_items[i] = new_item(savepick_choices[i],savepick_choices[i]);
		}
		savepick_items[savepick_n_choices-1] = (ITEM*) NULL;

		/* Create menu */
		savepick_menu = new_menu((ITEM **)savepick_items);

		/* Set description off */
		menu_opts_off(savepick_menu,O_SHOWDESC);

		/* Create the window to be associated with the menu */
		savepick_menu_win = newwin(11, 16, 5, 35);
		keypad(savepick_menu_win, TRUE);

		/* Set main window and sub window */
		set_menu_win(savepick_menu, savepick_menu_win);
		set_menu_sub(savepick_menu, derwin(savepick_menu_win, 4, 14, 4, 1));
		set_menu_format(savepick_menu, 4, 1);

		/* Set menu mark to the string " >  " */
		set_menu_mark(savepick_menu, " >  ");

		/* Print a border around main menu window*/
		box(savepick_menu_win, 0, 0);
		print_label(savepick_menu_win, 1, 0, 16, "Select save", COLOR_PAIR(6));
		mvwaddch(savepick_menu_win, 2, 0, ACS_LTEE);
		mvwhline(savepick_menu_win, 2, 1, ACS_HLINE, 16);
		mvwaddch(savepick_menu_win, 2, 15, ACS_RTEE);

		/* Post the menu */
		post_menu(savepick_menu);
		wrefresh(savepick_menu_win);

		//Handle side window for welcome info
		savepick_side_win = newwin(12, 32, 2, 2);
		scrollok(savepick_side_win,TRUE);
		wprintw(savepick_side_win,"  \nhelapordo v%s",VERSION);
		wprintw(savepick_side_win,"  \n  using: s4c-animate v%s",S4C_ANIMATE_VERSION);
		wprintw(savepick_side_win,"  \n  using: koliseo v%s",KOLISEO_API_VERSION_STRING);
		wprintw(savepick_side_win,"  \nBuilt with: amboso v%s",ANVIL__API_LEVEL__STRING);
		//wprintw(savepick_side_win,"  \n  %s",get_ANVIL__VERSION__DESC__());
		wrefresh(savepick_side_win);
		refresh();

		int savepick_picked = 0;

		/*
		//We set the colors to use s4c's palette file...
		FILE* palette_file;
		char path_to_palette[600];
		char palette_name[50] = "palette.gpl";
		*/
		int pickchar = -1;

		/*
		// Set static_path value to the correct static dir path
		resolve_staticPath(static_path);

		sprintf(path_to_palette,"%s/%s",static_path,palette_name);

		palette_file = fopen(path_to_palette, "r");

		init_s4c_color_pairs(palette_file);
		*/

		for (int i = 0; i < PALETTE_S4C_H_TOTCOLORS; i++) {
			init_s4c_color_pair(&palette[i],9+i);
		}

		while ( !savepick_picked && (pickchar = wgetch(savepick_menu_win)) != KEY_F(1) ) {
			switch(pickchar) {
				case KEY_DOWN: {
					menu_driver(savepick_menu, REQ_DOWN_ITEM);
				}
				break;
				case KEY_UP: {
					menu_driver(savepick_menu, REQ_UP_ITEM);
				}
				break;
				case KEY_LEFT: { /*Left option pick*/
					ITEM *cur;
					cur = current_item(savepick_menu);
					savepick_choice = getTurnChoice((char*)item_name(cur));
					sprintf(msg,"Left on choice: [ %s ] value (%i)",item_name(cur),savepick_choice);
					log_tag("debug_log.txt","[DEBUG]",msg);
					if (savepick_choice == NEW_GAME) {
						log_tag("debug_log.txt","[DEBUG]","Should do something");
					}
				}
				break;
				case KEY_RIGHT: { /*Right option pick*/
					ITEM *cur;
					cur = current_item(savepick_menu);
					savepick_choice = getTurnChoice((char*)item_name(cur));
					sprintf(msg,"Right on choice: [ %s ] value (%i)",item_name(cur),savepick_choice);
					log_tag("debug_log.txt","[DEBUG]",msg);
					if (savepick_choice == NEW_GAME) {
						log_tag("debug_log.txt","[DEBUG]","Should do something");
					}
				}
				break;
				case KEY_NPAGE: {
					menu_driver(savepick_menu, REQ_SCR_DPAGE);
				}
				break;
				case KEY_PPAGE: {
					menu_driver(savepick_menu, REQ_SCR_UPAGE);
				}
				break;
				case 10: /* Enter */
				{
					savepick_picked = 1;
					ITEM *cur;

					//move(18,47);
					//clrtoeol();
					cur = current_item(savepick_menu);
					//mvprintw(18, 47, "Item selected is : %s", item_name(cur));
					savepick_choice = getTurnChoice((char*)item_name(cur));
					pos_menu_cursor(savepick_menu);
					refresh();
				}
				break;
				case 'q':
				{
					log_tag("debug_log.txt","[DEBUG]","Player used q to quit from savepick menu.");
					//TODO: take some variable to disable quick quitting with q
					savepick_picked = 1;
					savepick_choice = getTurnChoice("Quit");
					pos_menu_cursor(savepick_menu);
					refresh();
				}
				break;
				default: {
					break;
				}
			}
			wrefresh(savepick_menu_win);
			if (savepick_choice == NEW_GAME) {
				int picked_saveslot_index = get_saveslot_index();
				sprintf(msg,"Saveslot index picked: [%i]",picked_saveslot_index);
				log_tag("debug_log.txt","[DEBUG]",msg);
				sprintf(current_save_path,"%s",default_saveslots[picked_saveslot_index].save_path); //Update saveslot_path value
				//TODO
				//Get picked_slot with a curses menu.
				//int picked_slot = handle_pickSave();
				//sprintf(current_save_path,default_saveslots[picked_slot].save_path);
				//TODO
				//By default we expect the user to press new game, no action needed?
				log_tag("debug_log.txt","[DEBUG]","Running new game from savepick menu");
				turnOP(OP_NEW_GAME,savepick_turn_args, default_kls, &savepick_kls);
			} else if (savepick_choice == LOAD_GAME) {
				int picked_saveslot_index = get_saveslot_index();
				sprintf(msg,"Saveslot index picked: [%i]",picked_saveslot_index);
				log_tag("debug_log.txt","[DEBUG]",msg);
				sprintf(current_save_path,"%s",default_saveslots[picked_saveslot_index].save_path); //Update saveslot_path value
				//TODO
				//Get picked_slot with a curses menu.
				//int picked_slot = handle_pickSave();
				//sprintf(current_save_path,default_saveslots[picked_slot].save_path);
				//ATM we expect a single save.
				//Setting this to 0 is the only thing we expect here, the actual load is done later.
				load_info->is_new_game = 0;
				sprintf(msg,"Set load value: load_info->is_new_game == (%i)", load_info->is_new_game);
				log_tag("debug_log.txt","[DEBUG]",msg);
				turnOP(OP_LOAD_GAME,savepick_turn_args, default_kls, &savepick_kls);
				//TODO
				//Select which game to load, by preparing the necessary handles to code below (correct savefile/name, for now)
			} else if (savepick_choice == QUIT) {
				//TODO
				//We can quit, I guess.
				sprintf(msg,"Savepick menu: doing exit(%i)",EXIT_SUCCESS);
				log_tag("debug_log.txt","[DEBUG]",msg);
				// Unpost menu and free all the memory taken up
				unpost_menu(savepick_menu);
				free_menu(savepick_menu);
				log_tag("debug_log.txt","[FREE]","Freed savepick menu");
				for(int k = 0; k < savepick_n_choices; k++) {
					free_item(savepick_items[k]);
					sprintf(msg,"Freed %i savepick menu item",k);
					log_tag("debug_log.txt","[FREE]",msg);
				}

				delwin(savepick_menu_win);
				endwin();
				exit(EXIT_SUCCESS);
			} else if (savepick_choice == TUTORIAL) {
				//TODO: show tutorial
				log_tag("debug_log.txt","[DEBUG]","Doing tutorial.");
				handleTutorial();
				exit(EXIT_SUCCESS);
			}
		}//End while !savepick_picked

		//Free turnOP_args
		//free(savepick_turn_args);

		// Unpost menu and free all the memory taken up
		unpost_menu(savepick_menu);
		free_menu(savepick_menu);
		log_tag("debug_log.txt","[FREE]","Freed savepick menu");
		for(int k = 0; k < savepick_n_choices; k++) {
			free_item(savepick_items[k]);
			sprintf(msg,"Freed %i savepick menu item",k);
			log_tag("debug_log.txt","[FREE]",msg);
		}

		delwin(savepick_menu_win);
		endwin();
		log_tag("debug_log.txt","[DEBUG]","Ended window mode for savepick menu");

		kls_temp_end(savepick_kls);

		//Flush the terminal
		int clrres = system("clear");
		sprintf(msg,"gameloop() system(\"clear\") after savepick res was (%i)",clrres);
		log_tag("debug_log.txt","[DEBUG]",msg);

		//By now, we expect load_info->is_new_game to be set to 0 or 1.
		sprintf(msg,"  Checking is_new_game:  load_info->is_new_game == (%i)",load_info->is_new_game);
		log_tag("debug_log.txt","[DEBUG]",msg);

		Koliseo_Temp gamestate_kls = kls_temp_start(temporary_kls);

		if (load_info->is_new_game) {// We prepare path and fighter
			path = randomise_path(rand(), default_kls, current_save_path);
			path->loreCounter = -1;

			sprintf(msg,"Prepping Fighter");
			kls_log("DEBUG",msg);
  			player = (Fighter*) KLS_PUSH_NAMED(default_kls, Fighter, 1, "Fighter","Fighter");

			int optTot = optind;

			getParams(argc, argv, player, path, optTot, default_kls);
			initPlayerStats(player,path,default_kls);
		} else { //Handle loading of gamestate


			//Declar turnOP_args
			Room* fakeroom = NULL;
			Enemy* fakeenemy = NULL;
			Boss* fakeboss = NULL;
			FILE* fakesavefile = NULL;
			WINDOW* fakenotifywin = NULL;
			turnOP_args* loading_room_turn_args = init_turnOP_args(player, path, fakeroom, load_info, fakeenemy, fakeboss, fakesavefile, fakenotifywin, &gamestate_kls);
			FILE* save_file;
			char path_to_savefile[1000];
			char static_path[500];
			char savefile_name[300];

			//Copy current_save_path
			sprintf(savefile_name,"%s",current_save_path);

			// Set static_path value to the correct static dir path
			resolve_staticPath(static_path);

			sprintf(path_to_savefile,"%s/%s",static_path,savefile_name);

			save_file = fopen(path_to_savefile, "r");
			if (!save_file) {
				//User error
				fprintf(stderr,"[ERROR]    Can't open savefile for loading game.\n");
				fprintf(stderr,"[ERROR]    Expected at path [%s].\n",path_to_savefile);
				//Debug error
				sprintf(msg,"Could not load savefile at (%s)",path_to_savefile);
				log_tag("debug_log.txt","[ERROR]",msg);
				exit(EXIT_FAILURE);
			}

			//Update loading_room_turn_args->save_file pointer
			loading_room_turn_args->save_file = save_file;
			sprintf(msg,"Assigned loading_room_turn_args->save_file: path [%s]",path_to_savefile);
			log_tag("debug_log.txt","[TURNOP]",msg);

			//Read save type
			//
			saveType loaded_save_type = -1;
			loaded_save_type = read_saveType(save_file);
			sprintf(msg,"Read saveType, was [%s].",stringFrom_saveType(loaded_save_type));
			log_tag("debug_log.txt","[TURNOP]",msg);

			if ( loaded_save_type == -1 ) {
				log_tag("debug_log.txt","[ERROR]","Failed setting loaded_save_type. Quitting.");
				fprintf(stderr,"[ERROR]    Failed setting a save type.");

				exit(EXIT_FAILURE);
			}

			load_info->save_type = loaded_save_type;

			sprintf(msg,"Assigned load_info->save_type: [%s]", stringFrom_saveType(load_info->save_type));
			log_tag("debug_log.txt","[TURNOP]",msg);


			path = randomise_path(rand(), default_kls, current_save_path);
			sprintf(msg,"Prepping Loady Fighter");
			kls_log("DEBUG",msg);
  			player = (Fighter*) KLS_PUSH_NAMED(default_kls, Fighter, 1, "Fighter","Loady Fighter");
			player->class = Knight;

			strcpy(player->name,"Loady");

			//Update loading_room_turn_args->actor pointer
			loading_room_turn_args->actor = player;
			sprintf(msg,"Assigned Fighter [%s]. loading_room_turn_args->actor->name: [%s]",player->name, loading_room_turn_args->actor->name);
			log_tag("debug_log.txt","[TURNOP]",msg);

			sprintf(msg,"Prepping Loady Wincon");
			kls_log("DEBUG",msg);
			Wincon* w = (Wincon*) KLS_PUSH_NAMED(default_kls, Wincon, 1,"Wincon","Loady Wincon");
			w->class = FULL_PATH;
			initWincon(w,path,w->class);
			initPlayerStats(player,path,default_kls);
			path->win_condition = w;

			if (load_info->save_type == ENEMIES_SAVE) {

				sprintf(msg,"Prepping Loady Enemy");
				kls_log("DEBUG",msg);
				load_info->loaded_enemy = (Enemy*) KLS_PUSH_NAMED(default_kls, Enemy, 1, "Enemy","Loaded Enemy");
				//FIXME: the structs related to loaded enemy are not loaded on default_kls
				prepareRoomEnemy(load_info->loaded_enemy, 1, 3, 1, &gamestate_kls);

				//Update loading_room_turn_args->enemy pointer
				loading_room_turn_args->enemy = load_info->loaded_enemy;
				sprintf(msg,"Assigned load_info->loaded_enemy->class == [%s]. loading_room_turn_args->loaded_enemy->class == [%s]",stringFromEClass(load_info->loaded_enemy->class), stringFromEClass(loading_room_turn_args->enemy->class));
				log_tag("debug_log.txt","[TURNOP]",msg);
			}
			//Update loading_room_turn_args->path pointer
			loading_room_turn_args->path = path;
			sprintf(msg,"Assigned loading_room_turn_args->path == [path] (len %i)",path->length);
			log_tag("debug_log.txt","[TURNOP]",msg);


			switch (load_info->save_type) {
				case ENEMIES_SAVE: {
					log_tag("debug_log.txt","[TURNOP]","Doing OP_LOAD_ENEMYROOM.");
					//int* loadinfo_totfoes = &(load_info->total_foes);
					//FIXME: the structs related to loaded enemy are not loaded on default_kls
					OP_res load_op_result = turnOP(OP_LOAD_ENEMYROOM,loading_room_turn_args, default_kls, &gamestate_kls);
					sprintf(msg,"OP_LOAD_ENEMYROOM:  result was [%s].",stringFrom_OP_res(load_op_result));
					log_tag("debug_log.txt","[TURNOP]",msg);
					sprintf(msg,"Freed loading_room_turn_args. Load result was [%s].",stringFrom_OP_res(load_op_result));
					log_tag("debug_log.txt","[FREE]",msg);
					//free(loading_room_turn_args);
				}
				break;
				case HOME_SAVE: {
					log_tag("debug_log.txt","[TURNOP]","Doing OP_LOAD_HOMEROOM.");
					//int* loadinfo_totfoes = &(load_info->total_foes);
					//FIXME: the structs related to loaded enemy are not loaded on default_kls
					OP_res load_op_result = turnOP(OP_LOAD_HOMEROOM,loading_room_turn_args, default_kls, &gamestate_kls);
					sprintf(msg,"OP_LOAD_HOMEROOM:  result was [%s].",stringFrom_OP_res(load_op_result));
					log_tag("debug_log.txt","[TURNOP]",msg);
					sprintf(msg,"Freed loading_room_turn_args. Load result was [%s].",stringFrom_OP_res(load_op_result));
					log_tag("debug_log.txt","[FREE]",msg);
					load_info->done_loading = 1;
					log_tag("debug_log.txt","[PREP]","Set load_info->done_loading to 1.");
					//free(loading_room_turn_args);
				}
				break;
				default: {
					sprintf(msg,"gameloop():  Unexpected save type, value was (%i).",(int) load_info->save_type);
					log_tag("debug_log.txt","[ERROR]",msg);
					exit(EXIT_FAILURE);
				}
				break;
			}

			//e_death(loaded_enemy);
			//death(player);
			//exit(0);
		}

		/*
		 * TODO
		 * Remove me
		 * Legacy code to load lores from a text file.
		for (int i=0; i<5; i++) {
			sprintf(msg,"Prepping lore (%i)",i);
			kls_log("DEBUG",msg);
			lore_strings[i] = (char*) KLS_PUSH_NAMED(default_kls, char, 300, "Lore", msg);
		}
		 */

		int* loreCounter = &(path->loreCounter);
		sprintf(msg,"loreCounter == (%i)",*loreCounter);
		log_tag("debug_log.txt","[DEBUG]",msg);

		if (GAMEMODE == Story) {

			/*
			 * TODO
			 * Remove me
			 * Legacy code to load lores from a text file.
			int loreKind = 0; //rand() % LORES_MAX;
			 */

			if (load_info->is_new_game) {
				sprintf(msg,"loreCounter was (%i), setting it to 0.",*loreCounter);
				log_tag("debug_log.txt","[FIXME]",msg);
				*loreCounter = 0; //We must set the counter before going forward
				//FIXME:
				//loreCounter should not start from 0 again.
			}

			/*
			 * TODO
			 *Remove me
			 *Legacy code for loading lores from a text file.
			 loadLore(lore_strings,loreKind);
			 */

		} else {
			sprintf(msg,"GAMEMODE is not Story. Value was: (%i)",GAMEMODE);
			log_tag("debug_log.txt","[WARN]",msg);
		}

		//Set consumables sprites
		for (int i = 0; i < CONSUMABLESMAX+1; i++) {
			setConsumableSprite((Consumable*)player->consumablesBag[i]);
			char msg[100];
			sprintf(msg,"Set sprite for %s", stringFromConsumables(i));
			log_tag("debug_log.txt","[PREP]",msg);
		}
		log_tag("debug_log.txt","[DEBUG-PREP]","Done setting sprites for Consumables.\n");
		//Set artifact sprites
		for (int i = 0; i < ARTIFACTSMAX+1; i++) {
			setArtifactSprite(player->artifactsBag[i]);
			char msg[100];
			sprintf(msg,"Set sprite for %s", stringFromArtifacts(i));
			log_tag("debug_log.txt","[PREP]",msg);
		}
		log_tag("debug_log.txt","[PREP]","Done setting sprites for Artifacts.\n");
		//Set base equips sprites...
		for (int i = 0; i < EQUIPSMAX+1; i++) {
			setEquipSprite(&equips[i]);
			char msg[100];
			sprintf(msg,"Set sprite for %s", stringFromEquips(i));
			log_tag("debug_log.txt","[PREP]",msg);
		}
		log_tag("debug_log.txt","[PREP]","Done setting sprites for Equips.\n");

		if (load_info->is_new_game) {
			log_tag("debug_log.txt","[PREP]","New game from scratch.\n");
		} else {
			log_tag("debug_log.txt","[PREP]","New game from loading.\n");
		}
		sprintf(msg,"Name:    %s",player->name);
		log_tag("debug_log.txt","[DEBUG]",msg);
		sprintf(msg,"Class:    %s",stringFromClass(player->class));
		log_tag("debug_log.txt","[DEBUG]",msg);
		sprintf(msg,"Gamemode:    %s",stringFromGamemode(GAMEMODE));
		log_tag("debug_log.txt","[DEBUG]",msg);

		//purple();
		//printStats(player);
		//white();

		int roomsDone = load_info->is_new_game ? 1 : loaded_roomindex;
		OP_res res = OP_RES_NO_DMG;
		int roadFork_value = -1; //0 may be used as a value, so

		Wincon* win_con = path->win_condition;
		sprintf(msg,"Wincon:    %s\n",stringFromWinconClass(win_con->class));
		log_tag("debug_log.txt","[DEBUG]",msg);

		//int refresh_artifact_wincon = 0;
		if (load_info->save_type == ENEMIES_SAVE) {
			load_info->done_loading = 0;
			log_tag("debug_log.txt","[DEBUG-PREP]","Set load_info->done_loading to 0.");
		}
		log_tag("debug_log.txt","[DEBUG-PREP]","Prepping done.\n");
		log_tag("debug_log.txt","[DEBUG]","Starting wincon loop.\n");

                diff_time = clock() - start_time;
                int time_spent = diff_time *1000 / CLOCKS_PER_SEC;
		//sprintf(msg,"[DEBUG]    Prep took %0.7f seconds.\n",time_spent);
		sprintf(msg,"Prep took %d s, %d ms.\n",time_spent/1000,time_spent%1000);
		log_tag("debug_log.txt","[DEBUG]",msg);

		if (GAMEMODE == Story || GAMEMODE == Standard) {

			//Loop till wincon reached

			while (win_con->current_val < win_con->target_val) {

				//Flush the terminal
				int loop_clrres = system("clear");
				sprintf(msg,"gameloop() system(\"clear\") in wincon loop, res: (%i)",loop_clrres);
				log_tag("debug_log.txt","[DEBUG]",msg);

				//Check if we have to update the wincon value
				if (path->win_condition->class == ALL_ARTIFACTS) {
					path->win_condition->current_val = player->stats->artifactsfound;
					//Are we forced to do one more room?
				}

				int enemyTotal = -1;
				roomClass room_type = -1;

				if (!(load_info->is_new_game) && !(load_info->done_loading) && (load_info->save_type == ENEMIES_SAVE)) {
					enemyTotal = loaded_roomtotalenemies;
				}

				sprintf(msg,"Prepping Room for Story Gamemode. roomsDone=(%i)",roomsDone);
				kls_log("DEBUG",msg);
				Room* current_room = (Room*) KLS_PUSH_T_NAMED(gamestate_kls,Room,1,"Room","Story Room");

				current_room->index = roomsDone;
				setRoomType(path, &roadFork_value, &room_type, roomsDone);
				sprintf(msg,"Set Room #%i type:    (%s)\n", roomsDone, stringFromRoom(room_type));
				log_tag("debug_log.txt","[ROOM]",msg);

				initRoom(current_room, player, roomsDone, room_type, enemyTotal, load_info, &gamestate_kls);
				sprintf(msg,"Init Room #%i:    (%s)\n", roomsDone, stringFromRoom(room_type));
				log_tag("debug_log.txt","[ROOM]",msg);


				//Check if we need to display a story prompt
				if (GAMEMODE == Story && (roomsDone == 1 || room_type == BOSS)) {
					displayLore(lore_strings,*loreCounter);
					(*loreCounter)++;
				}

				//Play room animation

				/*
				FILE* palette_file;
				char path_to_palette[600];
				char static_path[500];
				char palette_name[50] = "palette.gpl" ;

				// Set static_path value to the correct static dir path
				resolve_staticPath(static_path);


				sprintf(path_to_palette,"%s/%s",static_path,palette_name);

				palette_file = fopen(path_to_palette, "r");
				if (palette_file == NULL) {
					fprintf(stderr, "Error: could not open palette file (%s/%s).\n",static_path, palette_name);
					exit(EXIT_FAILURE);
				}
				*/

				WINDOW* door_win;
				//setlocale(LC_CTYPE, "it_IT.UTF-8");
				//initscr();
				clear();
				refresh();
				start_color();
				int colorCheck = has_colors();

				if (colorCheck == FALSE ) {
					fprintf(stderr,"Terminal can't use colors, abort.\n");
					exit(S4C_ERR_TERMCOLOR);
				}

				colorCheck = can_change_color();

				if (colorCheck == FALSE ) {
					fprintf(stderr,"Terminal can't change colors, abort.\n");
					exit(S4C_ERR_TERMCHANGECOLOR);
				}
				cbreak();
				noecho();
				keypad(stdscr, TRUE);

				// Initialize all the colors using the palette file we opened at the start
				//init_s4c_color_pairs(palette_file);

				for (int i = 0; i < PALETTE_S4C_H_TOTCOLORS; i++) {
					init_s4c_color_pair(&palette[i],9+i);
				}

				int reps = 1;
				int frametime = 27;
				int num_frames = 60;
				int frame_height = 22;
				int frame_width = 22;
				door_win = newwin(frame_height + 1, frame_width + 1, 0, 25);

				char door_sprites[MAXFRAMES][MAXROWS][MAXCOLS];

				copy_animation(enter_door,door_sprites,num_frames,frame_height,frame_width);

				sprintf(msg,"Copied animation from matrix vector for enter_door with dimensions: [%i][%i][%i].",num_frames,frame_height,frame_width);
		      		log_tag("debug_log.txt","[PREP]",msg);

				/*
				 * TODO
				 * Remove me
				 * Legacy code for loading animation from an s4c-file.
				 *
				// Set static_path value to the correct static dir path
				resolve_staticPath(static_path);

				char door_file_path[600];

				sprintf(door_file_path,"%s/animations/enter_door.txt",static_path);

				FILE* door_file = fopen(door_file_path,"r");
				if (!door_file) {
					fprintf(stderr,"[ERROR]    Can't open enter_door file.\n");
					exit(EXIT_FAILURE);
				}
				int loadCheck = load_sprites(door_sprites, door_file, frame_height-1, frame_width-1);

				// Check for possible loadCheck() errors and in this case we return early if we couldn't load
				if (loadCheck < 0) {
					endwin();
					switch (loadCheck) {
						case S4C_ERR_FILEVERSION: {
							fprintf(stderr,"S4C_ERR_FILEVERSION : Failed file version check.\n");
						}
						break;
						case S4C_ERR_LOADSPRITES: {
							fprintf(stderr,"S4C_ERR_LOADSPRITES : Failed loading the sprites.\n");
						}
						break;
					}
					exit(loadCheck);
				}
				*/

				// We make sure we have the background correcly set up and expect animate_sprites to refresh it
				wclear(door_win);
				wrefresh(door_win);

				int result = animate_sprites_at_coords(door_sprites, door_win, reps, frametime, num_frames, frame_height, frame_width, 0 , 0);
				sprintf(msg,"animate() result was (%i)", result);
				log_tag("debug_log.txt","[DEBUG]",msg);
				wclear(door_win);
				wrefresh(door_win);
				delwin(door_win);
				endwin();

				if (current_room->class == HOME) {
					res = handleRoom_Home(current_room, roomsDone, path, player, load_info, fighter_sprites,default_kls,&gamestate_kls);
				} else if (current_room->class == ENEMIES) {
					res = handleRoom_Enemies(current_room, roomsDone, path, player, load_info, enemy_sprites, fighter_sprites,default_kls,&gamestate_kls);
				} else if (current_room->class == SHOP) {
					//FIXME: does shop require usage of gameloop kls?
					res = handleRoom_Shop(current_room, roomsDone, path, player, default_kls, &gamestate_kls);
				} else if (current_room->class == BOSS) {
					res = handleRoom_Boss(current_room, roomsDone, path, player, load_info, boss_sprites, fighter_sprites, default_kls,&gamestate_kls);
				} else if (current_room->class == TREASURE) {
					res = handleRoom_Treasure(current_room, roomsDone, path, player, default_kls, &gamestate_kls);
				} else if (current_room->class == ROADFORK) {
					res = handleRoom_Roadfork(current_room, &roadFork_value, roomsDone, path, player);
				} else {
					sprintf(msg,"Unexpected current_room->class value: [%i] [%s]",current_room->class,stringFromRoom(current_room->class));
					log_tag("debug_log.txt","[ERROR]",msg);
					//freeRoom(current_room);
					log_tag("debug_log.txt","[ERROR]","Freed current room, quitting program.");
					exit(EXIT_FAILURE);
				}

				if (res == OP_RES_DEATH) {
					log_tag("debug_log.txt","[DEBUG]","Room resulted in DEATH.\n");
					//Free room memory
					//freeRoom(current_room);
					break;
				}
				else {
					//Flush the terminal
					int clrres = system("clear");
					sprintf(msg,"gameloop() system(\"clear\") res was (%i)",clrres);
					log_tag("debug_log.txt","[DEBUG]",msg);

					if (roadFork_value > 0) {
						lightYellow();
						printStats(player);
						lightGreen();
						printf("\n\tYou completed room %i.\n",roomsDone);
						white();
					}
					roomsDone++;

					//Update stats
					player->stats->roomscompleted++;

					//Check if we need to update the win condition
					if (win_con->class == FULL_PATH) {
						win_con->current_val++;
					}

					//Free room memory
					//freeRoom(current_room);
					// Reset gamestate_kls
					kls_temp_end(gamestate_kls);
					gamestate_kls = kls_temp_start(temporary_kls);
				}
			}// Win condition loop

			// Clear default_kls
			//kls_clear(default_kls);

			//Got out of the loop with res not being DEATH; so i won
			if (res != OP_RES_DEATH) { //I guess player and enemy were freed already?
				int clrres = system("clear");
				sprintf(msg,"gameloop() 2 system(\"clear\") res was (%i)",clrres);
				log_tag("debug_log.txt","[DEBUG]",msg);
				handleStats(player);
				printf("\n\n\tYOU WON!\n\n");
				log_tag("debug_log.txt","[DEBUG]","Game won.\n");
			}

			/*
			//Free lore strings if they were loaded
			if (GAMEMODE == Story) {
				for (int i=0; i<5; i++) {
					char msg[1000];
					sprintf(msg,"Freed lore string %i",i);
					log_tag("debug_log.txt","[FREE]",msg);
					sprintf(msg,"%s",lore_strings[i]);
					log_tag("debug_log.txt","[FREE]",msg);
					free(lore_strings[i]);
				}
			}
			*/
			//free(path->win_condition);
			//free(path);
			log_tag("debug_log.txt","[DEBUG]","End of wincon loop.\n");

		} else { //Gamemode is not Story or Standard
			sprintf(msg,"Gamemode was [%i]",GAMEMODE);
			log_tag("debug_log.txt","[DEBUG]",msg);

			if (GAMEMODE == Rogue) {
				log_tag("debug_log.txt","[DEBUG]","Doing a Rogue run.");
				FILE* palette_file;
				char path_to_palette[600];
				char static_path[500];
				char palette_name[50] = "palette.gpl" ;

				// Set static_path value to the correct static dir path
				resolve_staticPath(static_path);

				sprintf(path_to_palette,"%s/%s",static_path,palette_name);

				palette_file = fopen(path_to_palette, "r");
				if (palette_file == NULL) {
					fprintf(stderr, "Error: could not open palette file (%s/%s).\n",static_path, palette_name);
					exit(EXIT_FAILURE);
				}

				WINDOW* floor_win;
				//TODO: store if we have done initsrc() or endwin(). Am sure we can get this from ncurses with some MACRO
				//initscr();
				clear();
				refresh();
				start_color();

				int colorCheck = has_colors();

				if (colorCheck == FALSE ) {
					fprintf(stderr,"Terminal can't use colors, abort.\n");
					exit(S4C_ERR_TERMCOLOR);
				}

				colorCheck = can_change_color();

				if (colorCheck == FALSE ) {
					fprintf(stderr,"Terminal can't change colors, abort.\n");
					exit(S4C_ERR_TERMCHANGECOLOR);
				}
				cbreak();
				noecho();
				keypad(stdscr, TRUE);

				// Initialize all the colors using the palette file we opened at the start
				//init_s4c_color_pairs(palette_file);

				for (int i = 0; i < PALETTE_S4C_H_TOTCOLORS; i++) {
					init_s4c_color_pair(&palette[i],9+i);
				}

				cbreak();
				noecho();
				keypad(stdscr, TRUE);

				// Create the window
				floor_win = newwin(28, 30, 1, 2);
				wclear(floor_win);
				wrefresh(floor_win);
				keypad(floor_win, TRUE);

				/* Print a border around the windows and print a title */
				box(floor_win, 0, 0);
				wrefresh(floor_win);
				refresh();
				int res = -1;
				char msg[500];

				sprintf(msg,"Prepping current_floor.");
				log_tag("debug_log.txt","[DEBUG]",msg);
				kls_log("DEBUG",msg);
				Floor* current_floor = (Floor*) KLS_PUSH_T_NAMED(gamestate_kls,Floor,1,"Floor","Floor");
				// Start the random walk from the center of the dungeon
				int center_x = FLOOR_MAX_COLS / 2;
				int center_y = FLOOR_MAX_ROWS / 2;

				// Init dbg_floor
				init_floor_layout(current_floor);

				//Set center as filled
				current_floor->floor_layout[center_x][center_y] = 1;

				//Init floor rooms
				init_floor_rooms(current_floor);

				//Random walk #1
				floor_random_walk(current_floor, center_x, center_y, 100, 1); // Perform 100 steps of random walk, reset floor_layout if needed.
				//Random walk #2
				floor_random_walk(current_floor, center_x, center_y, 100, 0); // Perform 100 more steps of random walk, DON'T reset floor_layout if needed.

				//Set floor explored matrix
				load_floor_explored(current_floor);

				//Set room types
				floor_set_room_types(current_floor);

				int current_x = center_x;
				int current_y = center_y;

				//TODO: handle finishing all floors
				path->length = MAX_ROGUE_FLOORS;

				int floors_done = 0;

				//Loop till wincon reached

				//while (dbg_floor->explored_area*1.0 < (dbg_floor->area*0.8)) {
				while (win_con->current_val < win_con->target_val) {

					//Check if we have to update the wincon value
					if (path->win_condition->class == ALL_ARTIFACTS) {
						path->win_condition->current_val = player->stats->artifactsfound;
						//Are we forced to do one more room?
					}

					int enemyTotal = -1;
					roomClass room_type = -1;

					if (!(load_info->is_new_game) && !(load_info->done_loading) && (load_info->save_type == ENEMIES_SAVE)) {
						enemyTotal = loaded_roomtotalenemies;
					}

					Room* current_room = NULL;

					//Check if current room needs to be played
					if (current_floor->roomclass_layout[current_x][current_y] != BASIC) {
						sprintf(msg,"Prepping Room for Rogue Gamemode. roomsDone=(%i)",roomsDone);
						kls_log("DEBUG",msg);
						current_room = (Room*) KLS_PUSH_T_NAMED(gamestate_kls,Room,1,"Room",msg);

						current_room->index = roomsDone;
						//setRoomType(path, &roadFork_value, &room_type, roomsDone);

						room_type = current_floor->roomclass_layout[current_x][current_y];
						sprintf(msg,"Set Room #%i type:    (%s)\n", roomsDone, stringFromRoom(room_type));
						log_tag("debug_log.txt","[ROOM]",msg);

						initRoom(current_room, player, roomsDone, room_type, enemyTotal, load_info, &gamestate_kls);
						sprintf(msg,"Init Room #%i:    (%s)\n", roomsDone, stringFromRoom(room_type));
						log_tag("debug_log.txt","[ROOM]",msg);


						/*
						//Check if we need to display a story prompt
						if (GAMEMODE == Story && (roomsDone == 1 || room_type == BOSS)) {
							displayLore(lore_strings,*loreCounter);
							(*loreCounter)++;
						}
						*/

						//Play room animation

						WINDOW* door_win;
						//setlocale(LC_CTYPE, "it_IT.UTF-8");
						//initscr();
						clear();
						refresh();
						start_color();

						int reps = 1;
						int frametime = 27;
						int num_frames = 60;
						int frame_height = 22;
						int frame_width = 22;
						door_win = newwin(frame_height + 1, frame_width + 1, 0, 25);

						char door_sprites[MAXFRAMES][MAXROWS][MAXCOLS];

						copy_animation(enter_door,door_sprites,num_frames,frame_height,frame_width);

						sprintf(msg,"Copied animation from matrix vector for enter_door with dimensions: [%i][%i][%i].",num_frames,frame_height,frame_width);
		      				log_tag("debug_log.txt","[PREP]",msg);

						/*
						 * TODO
						 * Remove me
						 * Legacy code for loading animation from an s4c-file.
						 *
						// Set static_path value to the correct static dir path

						// Set static_path value to the correct static dir path
						resolve_staticPath(static_path);

						char door_file_path[600];

						sprintf(door_file_path,"%s/animations/enter_door.txt",static_path);

						FILE* door_file = fopen(door_file_path,"r");
						if (!door_file) {
							fprintf(stderr,"[ERROR]    Can't open enter_door file.\n");
							exit(EXIT_FAILURE);
						}
						int loadCheck = load_sprites(door_sprites, door_file, frame_height-1, frame_width-1);

						// Check for possible loadCheck() errors and in this case we return early if we couldn't load
						if (loadCheck < 0) {
							endwin();
							switch (loadCheck) {
								case S4C_ERR_FILEVERSION: {
									fprintf(stderr,"S4C_ERR_FILEVERSION : Failed file version check.\n");
								}
								break;
								case S4C_ERR_LOADSPRITES: {
									fprintf(stderr,"S4C_ERR_LOADSPRITES : Failed loading the sprites.\n");
								}
								break;
							}
							exit(loadCheck);
						}
						*/

						// We make sure we have the background correcly set up and expect animate_sprites to refresh it
						wclear(door_win);
						wrefresh(door_win);

						int result = animate_sprites_at_coords(door_sprites, door_win, reps, frametime, num_frames, frame_height, frame_width, 0 , 0);
						sprintf(msg,"animate() result was (%i)", result);
						log_tag("debug_log.txt","[DEBUG]",msg);
						wclear(door_win);
						wrefresh(door_win);
						delwin(door_win);
						endwin();

						if (current_room->class == HOME) {
							res = handleRoom_Home(current_room, roomsDone, path, player, load_info, fighter_sprites, default_kls, &gamestate_kls);
						} else if (current_room->class == ENEMIES) {
							res = handleRoom_Enemies(current_room, roomsDone, path, player, load_info, enemy_sprites, fighter_sprites, default_kls, &gamestate_kls);
						} else if (current_room->class == SHOP) {
							res = handleRoom_Shop(current_room, roomsDone, path, player, default_kls, &gamestate_kls);
						} else if (current_room->class == BOSS) {
							res = handleRoom_Boss(current_room, roomsDone, path, player, load_info, boss_sprites, fighter_sprites, default_kls, &gamestate_kls);
						} else if (current_room->class == TREASURE) {
							res = handleRoom_Treasure(current_room, roomsDone, path, player, default_kls, &gamestate_kls);
						} else if (current_room->class == ROADFORK) {
							res = handleRoom_Roadfork(current_room, &roadFork_value, roomsDone, path, player);
						} else {
							sprintf(msg,"Unexpected current_room->class value: [%i] [%s]",current_room->class,stringFromRoom(current_room->class));
							log_tag("debug_log.txt","[ERROR]",msg);
							//freeRoom(current_room);
							log_tag("debug_log.txt","[ERROR]","Freed current room, quitting program.");
							exit(EXIT_FAILURE);
						}

						if (res == OP_RES_DEATH) {
							log_tag("debug_log.txt","[DEBUG]","Room resulted in DEATH.\n");
							//Free room memory
							//freeRoom(current_room);
							break;
						} else {
							//Flush the terminal
							int clrres = system("clear");
							sprintf(msg,"gameloop() system(\"clear\") res was (%i)",clrres);
							log_tag("debug_log.txt","[DEBUG]",msg);

							if (roadFork_value > 0) {
								lightYellow();
								printStats(player);
								lightGreen();
								printf("\n\tYou completed room %i.\n",roomsDone);
								white();
							}
							roomsDone++;

							//Update stats
							player->stats->roomscompleted++;

							//Free room memory
							//freeRoom(current_room);

							//Update floor's roomclass layout for finished rooms which should not be replayed
							switch(current_floor->roomclass_layout[current_x][current_y]) {
								case ENEMIES: {
									current_floor->roomclass_layout[current_x][current_y] = BASIC;
									// Reset gamestate_kls
									kls_temp_end(gamestate_kls);
									gamestate_kls = kls_temp_start(temporary_kls);
								}
								break;
								case BOSS: {
									current_floor->roomclass_layout[current_x][current_y] = BASIC;
									floors_done++;
									sprintf(msg,"Floors done: [%i]", floors_done);
									log_tag("debug_log.txt","[DEBUG]",msg);
									//Check if we need to update the win condition
									if (win_con->class == FULL_PATH) {
										win_con->current_val++;
									}

									//Regenerate floor
									log_tag("debug_log.txt","[DEBUG]","Beaten a boss, regenerating current floor.");
									// Init
									init_floor_layout(current_floor);
									//Set center as filled
									current_floor->floor_layout[center_x][center_y] = 1;
									//Init floor rooms
									init_floor_rooms(current_floor);
									//Random walk #1
									floor_random_walk(current_floor, center_x, center_y, 100, 1); // Perform 100 steps of random walk, reset floor_layout if needed.
									//Random walk #2
									floor_random_walk(current_floor, center_x, center_y, 100, 0); // Perform 100 more steps of random walk, DON'T reset floor_layout if needed.
									//Set floor explored matrix
									load_floor_explored(current_floor);
									//Set room types
									floor_set_room_types(current_floor);

									//Center current coords
									current_x = center_x;
									current_y = center_y;

									// Reset gamestate_kls
									kls_temp_end(gamestate_kls);
									gamestate_kls = kls_temp_start(temporary_kls);

									continue; //Check win condition for loop

								}
								break;
								case SHOP: {
									current_floor->roomclass_layout[current_x][current_y] = BASIC;
									// Reset gamestate_kls
									kls_temp_end(gamestate_kls);
									gamestate_kls = kls_temp_start(temporary_kls);
								}
								break;
								case TREASURE: {
									current_floor->roomclass_layout[current_x][current_y] = BASIC;
									// Reset gamestate_kls
									kls_temp_end(gamestate_kls);
									gamestate_kls = kls_temp_start(temporary_kls);
								}
								break;
								case HOME: {
									//We leave it available
									log_tag("debug_log.txt","[DEBUG]","Skipping reset of roomclass for HOME room");
									// Reset gamestate_kls
									kls_temp_end(gamestate_kls);
									gamestate_kls = kls_temp_start(temporary_kls);
								}
								break;
								default: {
									sprintf(msg,"Unexpected roomclass value in Rogue loop: [%i] [%s]",current_floor->roomclass_layout[current_x][current_y],stringFromRoom(current_floor->roomclass_layout[current_x][current_y]));
									log_tag("debug_log.txt","[ERROR]",msg);
									exit(EXIT_FAILURE);
								}
								break;
							}
						}
					} else {
						sprintf(msg,"Current room class was [%s] (val: %i), not playable.",stringFromRoom(current_floor->roomclass_layout[current_x][current_y]),current_floor->roomclass_layout[current_x][current_y]);
						log_tag("debug_log.txt","[DEBUG]",msg);
					}

					//Draw current FOV
					draw_floor_view(current_floor, current_x, current_y, floor_win);
					//Take a step and update screen
					move_update(current_floor, &current_x, &current_y, floor_win, path, player, current_room, load_info, default_kls, &gamestate_kls);
				}// Win condition loop

				kls_temp_end(gamestate_kls);
				// Clear default_kls
				//kls_clear(default_kls);

				 //Got out of the loop with res not being DEATH; so i won
				 if (res != OP_RES_DEATH) { //I guess player and enemy were freed already?
					 int clrres = system("clear");
					 sprintf(msg,"gameloop() 2 system(\"clear\") res was (%i)",clrres);
					 log_tag("debug_log.txt","[DEBUG]",msg);
					 handleStats(player);
					 printf("\n\n\tYOU WON!\n\n");
					 log_tag("debug_log.txt","[DEBUG]","Game won.\n");
				 } else {
					 int clrres = system("clear");
					 sprintf(msg,"gameloop() 3 system(\"clear\") res was (%i)",clrres);
					 log_tag("debug_log.txt","[DEBUG]",msg);
					 printf("\n\n\tYOU DIED.\n\n");
					 log_tag("debug_log.txt","[DEBUG]","Game lost.\n");
				 }

				 //Free lore strings if they were loaded
				 if (GAMEMODE == Story) {
					 for (int i=0; i<5; i++) {
						 char msg[1000];
						 sprintf(msg,"Freed lore string %i",i);
						 log_tag("debug_log.txt","[FREE]",msg);
						 sprintf(msg,"%s",lore_strings[i]);
						 log_tag("debug_log.txt","[FREE]",msg);
						 //free(lore_strings[i]);
					 }
				 }
				 //free(path->win_condition);
				 //free(path);
				 log_tag("debug_log.txt","[DEBUG]","End of wincon loop.\n");

				//free(current_floor);

				endwin();
			} else {
				log_tag("debug_log.txt","[ERROR]","Error in gameloop().");
				exit(EXIT_FAILURE);
			}
		}
		kls_temp_end(gamestate_kls);
	} while (retry());

	purple();
	printf("\n\n\t\tTHANKS 4 PLAYING!\n\n");
	white();
	log_tag("debug_log.txt","[DEBUG]","End of program.\n");
	exit(0);
}
