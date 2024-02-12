// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2022-2024 jgabaut

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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
OP_res turnOP(turnOption_OP op, turnOP_args *args, Koliseo *kls,
              Koliseo_Temp *t_kls)
{

    OP_res res = INVALID_OP;

    Gamestate *gmst = args->gmst;
    if (gmst == NULL)
        log_tag("debug_log.txt", "[WARN]", "turnOP_args->(gmst) was NULL");
    Fighter *actor = args->actor;
    if (actor == NULL)
        log_tag("debug_log.txt", "[WARN]", "turnOP_args->(actor) was NULL");
    Path *path = args->path;
    if (path == NULL)
        log_tag("debug_log.txt", "[WARN]", "turnOP_args->(path) was NULL");
    Room *room = args->room;
    if (room == NULL)
        log_tag("debug_log.txt", "[WARN]", "turnOP_args->(room) was NULL");
    loadInfo *load_info = args->load_info;
    if (load_info == NULL)
        log_tag("debug_log.txt", "[WARN]", "turnOP_args->(load_info) was NULL");
    Enemy *enemy = args->enemy;
    if (enemy == NULL)
        log_tag("debug_log.txt", "[WARN]", "turnOP_args->(enemy) was NULL");
    Boss *boss = args->boss;
    if (boss == NULL)
        log_tag("debug_log.txt", "[WARN]", "turnOP_args->(boss) was NULL");
    FILE *save_file = args->save_file;
    if (save_file == NULL)
        log_tag("debug_log.txt", "[WARN]", "turnOP_args->(save_file) was NULL");
#ifdef HELAPORDO_CURSES_BUILD
    WINDOW *notify_win = args->notify_win;
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
    Rectangle *notify_win = args->notify_win;
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
    if (notify_win == NULL)
        log_tag("debug_log.txt", "[WARN]",
                "turnOP_args->(notify_win) was NULL");
    foeTurnOption_OP foe_op = args->foe_op;
    if (foe_op == FOE_OP_INVALID) {
        log_tag("debug_log.txt", "[WARN]",
                "turnOP_args->(foe_op) was FOE_OP_INVALID");
    } else if (foe_op < 0 || foe_op > FOETURNOP_MAX) {
        log_tag("debug_log.txt", "[ERROR]",
                "turnOP_args->(foe_op) was invalid: [%i]", foe_op);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_SUCCESS);
    }
    skillType skill = args->picked_skill;
    if (skill < 0 || skill >= SKILLSTOTAL) {
        log_tag("debug_log.txt", "[WARN]",
                "turnOP_args->(picked_skill) was invalid: [%i]", skill);
    }

    int isBoss = -1;
    int room_index = -1;
    int enemy_index = -1;

    //Log operation code
    log_OP(op);
    switch (op) {
    case OP_INVALID: {
        //No_OP, but unexpected
        log_tag("debug_log.txt", "[ERROR]",
                "Done turnOP() on an OP_INVALID opcode.");
    }
    break;
    case OP_NEW_GAME: {
        res = NO_OP;
    }
    break;
    case OP_LOAD_GAME: {
        if (load_info == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "turnOP_args->(load_info) was NULL");
            //free(args);
            log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
            return res;
        }
        load_info->is_new_game = 0;
        res = OP_OK;
    }
    break;
    case OP_LOAD_ENEMYROOM: {
        if (load_info == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "turnOP_args->(load_info) was NULL");
            //free(args);
            log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
            return res;
        }
        load_info = args->load_info;
        if (load_info == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "load_info was NULL after load_info = args->load_info:  in turnOP(OP_LOAD_ENEMYROOM");
            return res;
        }
        save_file = args->save_file;

        if (save_file == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "turnOP_args->(save_file) was NULL");
            //free(args);
            log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
            return res;
        }
        int *ptr_to_loaded_enemy_index = &(load_info->enemy_index);
        log_tag("debug_log.txt", "[TURNOP]",
                "*(ptr_to_loaded_enemy_index) == [%i]",
                *ptr_to_loaded_enemy_index);
        int *ptr_to_loaded_roomtotalenemies =
            (load_info->ptr_to_roomtotalenemies);
        log_tag("debug_log.txt", "[TURNOP]",
                "*(ptr_to_loaded_roomtotalenemies) == [%i]",
                *ptr_to_loaded_roomtotalenemies);
        int *ptr_to_loaded_roomindex = (load_info->ptr_to_roomindex);
        log_tag("debug_log.txt", "[TURNOP]",
                "*(ptr_to_loaded_roomindex) == [%i]",
                *ptr_to_loaded_roomindex);
        int *tot_foes = &(load_info->total_foes);
        log_tag("debug_log.txt", "[TURNOP]", "*(tot_foes) == [%i]",
                *tot_foes);
        int *ptr_to_done_loading = &(load_info->done_loading);
        log_tag("debug_log.txt", "[TURNOP]", "*(done_loading) == [%i]",
                *ptr_to_done_loading);
        res =
            handleLoadgame_Enemies(save_file, actor, path,
                                   load_info->loaded_enemy,
                                   ptr_to_loaded_enemy_index,
                                   ptr_to_loaded_roomtotalenemies,
                                   ptr_to_loaded_roomindex, tot_foes,
                                   ptr_to_done_loading, kls);
        //Log end of operation
        log_tag("debug_log.txt", "[TURNOP]",
                "Done operation: [%s] res: [%s (%i)]", stringFromTurnOP(op),
                stringFrom_OP_res(res), res);

        log_tag(OPS_LOGFILE, "[RES]", "res: [%s (%i)]",
                stringFrom_OP_res(res), res);

        return res;
    }
    break;
    case OP_LOAD_HOMEROOM: {
        if (load_info == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "turnOP_args->(load_info) was NULL");
            //free(args);
            log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
            return res;
        }
        load_info = args->load_info;
        if (load_info == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "load_info was NULL after load_info = args->load_info:  in turnOP(OP_LOAD_HOMEROOM");
            return res;
        }
        save_file = args->save_file;

        if (save_file == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "turnOP_args->(save_file) was NULL");
            //free(args);
            log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
            return res;
        }
        if (load_info->ptr_to_roomindex == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "load_info->ptr_to_roomindex was NULL in turnOP(OP_LOAD_HOMEROOM");
            return res;
        }
        int *ptr_to_loaded_roomindex = (load_info->ptr_to_roomindex);
        if (ptr_to_loaded_roomindex == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "ptr_to_loaded_roomindex was NULL in turnOP(OP_LOAD_HOMEROOM");
            return res;
        }
        log_tag("debug_log.txt", "[TURNOP]",
                "*(ptr_to_loaded_roomindex) == [%i]",
                *ptr_to_loaded_roomindex);

        int *ptr_to_done_loading = &(load_info->done_loading);
        log_tag("debug_log.txt", "[TURNOP]", "*(done_loading) == [%i]",
                *ptr_to_done_loading);
        res =
            handleLoadgame_Home(save_file, actor, path,
                                ptr_to_loaded_roomindex,
                                ptr_to_done_loading, kls);
        //Log end of operation
        log_tag("debug_log.txt", "[TURNOP]",
                "Done operation: [%s] res: [%s (%i)]", stringFromTurnOP(op),
                stringFrom_OP_res(res), res);

        log_tag(OPS_LOGFILE, "[RES]", "res: [%s (%i)]",
                stringFrom_OP_res(res), res);

        return res;
    }
    break;
    case OP_FIGHT: {
        if (notify_win == NULL) {
            log_tag("debug_log.txt", "[CRITICAL]",
                    "Notification WINDOW pointer was null in turnOP(OP_FIGHT)");
            exit(EXIT_FAILURE);
        }
        if (room == NULL) {
            log_tag("debug_log.txt", "[CRITICAL]",
                    "Room pointer was null in turnOP(OP_FIGHT)");
            exit(EXIT_FAILURE);
        }
        if (foe_op == FOE_OP_INVALID) {
            log_tag("debug_log.txt", "[CRITICAL]",
                    "foe_op was FOE_OP_INVALID in turnOP(OP_FIGHT)");
            exit(EXIT_FAILURE);
        } else if (foe_op < 0 || foe_op > FOETURNOP_MAX) {
            log_tag("debug_log.txt", "[CRITICAL]",
                    "foe_op was invalid in turnOP(OP_FIGHT): [%i]", foe_op);
            exit(EXIT_FAILURE);
        }
        room_index = room->index;
        if (room->class == ENEMIES && enemy == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Enemy pointer was null in turnOP(OP_FIGHT) for ENEMIES room.");
            exit(EXIT_FAILURE);
        } else if (room->class == BOSS && boss == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Boss pointer was null in turnOP(OP_FIGHT) for BOSS room.");
            exit(EXIT_FAILURE);
        } else if (room->class != ENEMIES && room->class != BOSS) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Invalid room class in turnOP(OP_FIGHT): (%s [%i])",
                    stringFromRoom(room->class), room->class);
            exit(EXIT_FAILURE);
        }
        switch (room->class) {
        case ENEMIES: {
            enemy_index = enemy->index;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (%i) (OP_FIGHT), isBoss == 0",
                    enemy->index);
            isBoss = 0;
            res =
                OP_res_from_fightResult(defer_fight_enemy
                                        (actor, enemy, foe_op,
                                         notify_win, kls));
        }
        break;
        case BOSS: {
            enemy_index = 0;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (0) (OP_FIGHT), isBoss == 1");
            isBoss = 1;
            res =
                OP_res_from_fightResult(defer_fight_boss
                                        (actor, boss, path, foe_op,
                                         notify_win, kls));
        }
        break;
        default: {
            log_tag("debug_log.txt", "[TURNOP]",
                    "Invalid room value in turnOP(OP_FIGHT): [%s (%i)]",
                    stringFromRoom(room->class), room->class);
            exit(EXIT_FAILURE);

        }
        break;
        }
    }
    break;
    case OP_SPECIAL: {
        if (room == NULL) {
            log_tag("debug_log.txt", "[CRITICAL]",
                    "Room pointer was null in turnOP(OP_SPECIAL)");
            exit(EXIT_FAILURE);
        }
        room_index = room->index;
        if (room->class == ENEMIES && enemy == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Enemy pointer was null in turnOP(OP_SPECIAL) for ENEMIES room.");
            exit(EXIT_FAILURE);
        } else if (room->class == BOSS && boss == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Boss pointer was null in turnOP(OP_SPECIAL) for BOSS room.");
            exit(EXIT_FAILURE);
        } else if (room->class != ENEMIES && room->class != BOSS) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Invalid room class in turnOP(OP_SPECIAL): (%s [%i])",
                    stringFromRoom(room->class), room->class);
            exit(EXIT_FAILURE);
        }
        switch (room->class) {
        case ENEMIES: {
            enemy_index = enemy->index;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (%i) (OP_SPECIAL), isBoss == 0",
                    enemy->index);
            isBoss = 0;
        }
        break;
        case BOSS: {
            enemy_index = 0;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (0) (OP_SPECIAL), isBoss == 1");
            isBoss = 1;
        }
        break;
        default: {
            log_tag("debug_log.txt", "[TURNOP]",
                    "Invalid room value in turnOP(OP_SPECIAL): [%s (%i)]",
                    stringFromRoom(room->class), room->class);
            exit(EXIT_FAILURE);

        }
        }
        handleSpecials(actor, enemy, boss, path, room_index, enemy_index,
                       isBoss);
        res = OP_OK;
    }
    break;
    case OP_CONSUMABLE: {
        if (room == NULL) {
            log_tag("debug_log.txt", "[CRITICAL]",
                    "Room pointer was null in turnOP(OP_CONSUMABLE)");
            exit(EXIT_FAILURE);
        }
        room_index = room->index;
        if (room->class == ENEMIES && enemy == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Enemy pointer was null in turnOP(OP_CONSUMABLE) for ENEMIES room.");
            exit(EXIT_FAILURE);
        } else if (room->class == BOSS && boss == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Boss pointer was null in turnOP(OP_CONSUMABLE) for BOSS room.");
            exit(EXIT_FAILURE);
        } else if (room->class != ENEMIES && room->class != BOSS) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Invalid room class in turnOP(OP_CONSUMABLE): (%s [%i])",
                    stringFromRoom(room->class), room->class);
            exit(EXIT_FAILURE);
        }
        switch (room->class) {
        case ENEMIES: {
            enemy_index = enemy->index;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (%i) (OP_CONSUMABLE), isBoss == 0",
                    enemy->index);
            isBoss = 0;
        }
        break;
        case BOSS: {
            enemy_index = 0;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (0) (OP_CONSUMABLE), isBoss == 1");
            isBoss = 1;
        }
        break;
        default: {
            log_tag("debug_log.txt", "[TURNOP]",
                    "Invalid room value in turnOP(OP_CONSUMABLE): [%s (%i)]",
                    stringFromRoom(room->class), room->class);
            exit(EXIT_FAILURE);

        }
        }
        handleConsumables(actor, enemy, boss, isBoss);
        res = OP_OK;
    }
    break;
    case OP_SAVE: {
        if (GAMEMODE == Rogue) {
            log_tag("debug_log.txt", "[WARN]",
                    "GAMEMODE was [Rogue] in turnOP(OP_SAVE)");
            res = NO_OP;
            break;
        }
        if (save_file == NULL) {
            log_tag("debug_log.txt", "[CRITICAL]",
                    "save_file pointer was null in turnOP(OP_SAVE)");
            exit(EXIT_FAILURE);
        }
        if (load_info == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "turnOP_args->(load_info) was NULL");
            //free(args);
            log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
            return res;
        }
        if (room == NULL) {
            log_tag("debug_log.txt", "[CRITICAL]",
                    "Room pointer was null in turnOP(OP_SAVE)");
            exit(EXIT_FAILURE);
        }
        room_index = room->index;
        if (room->class == ENEMIES && enemy == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Enemy pointer was null in turnOP(OP_SAVE) for ENEMIES room.");
            exit(EXIT_FAILURE);
        } else if (room->class == BOSS && boss == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Boss pointer was null in turnOP(OP_SAVE) for BOSS room.");
            exit(EXIT_FAILURE);
        }

        if ((room->class != ENEMIES) && (room->class != HOME)) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Invalid room class in turnOP(OP_SAVE): (%s [%i])",
                    stringFromRoom(room->class), room->class);
            exit(EXIT_FAILURE);
        }

        switch (room->class) {
        case ENEMIES: {
            enemy_index = enemy->index;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (%i) (OP_SAVE), isBoss == 0",
                    enemy->index);
            isBoss = 0;
            load_info->save_type = ENEMIES_SAVE;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting save_type to ENEMIES_SAVE. [%s]",
                    stringFrom_saveType(load_info->save_type));
        }
        break;
        case BOSS: {
            enemy_index = 0;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (0) (OP_SAVE), isBoss == 1");
            isBoss = 1;
        }
        break;
        case HOME: {
            enemy_index = -1;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (-1) (OP_SAVE), isBoss == -1");
            isBoss = -1;
            load_info->save_type = HOME_SAVE;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting save_type to HOME_SAVE. [%s]",
                    stringFrom_saveType(load_info->save_type));
        }
        break;
        default: {
            log_tag("debug_log.txt", "[TURNOP]",
                    "Invalid room value in turnOP(OP_SAVE): [%s (%i)]",
                    stringFromRoom(room->class), room->class);
            exit(EXIT_FAILURE);

        }
        }
        switch (load_info->save_type) {
        case ENEMIES_SAVE: {
            res =
                handleSave_Enemies(save_file, actor, path, enemy,
                                   enemy_index, room->enemyTotal,
                                   room_index);
        }
        break;
        case HOME_SAVE: {
            res = handleSave_Home(save_file, actor, path, room_index);
        }
        break;
        default: {
            log_tag("debug_log.txt", "[TURNOP]",
                    "Invalid save_type value in turnOP(OP_SAVE): (%i)",
                    (int)load_info->save_type);
            exit(EXIT_FAILURE);
        }
        break;
        }
    }
    break;
    case OP_DEBUG: {
        if (gmst == NULL) {
            log_tag("debug_log.txt", "[WARN]",
                    "Gamestate pointer was null in turnOP(OP_DEBUG)");
            exit(EXIT_FAILURE);
        }
        if (room == NULL) {
            log_tag("debug_log.txt", "[WARN]",
                    "Room pointer was null in turnOP(OP_DEBUG)");
            exit(EXIT_FAILURE);
        }
        room_index = room->index;
        if (room->class == ENEMIES && enemy == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Enemy pointer was null in turnOP(OP_DEBUG) for ENEMIES room.");
            exit(EXIT_FAILURE);
        } else if (room->class == BOSS && boss == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Boss pointer was null in turnOP(OP_DEBUG) for BOSS room.");
            exit(EXIT_FAILURE);
        }

        switch (room->class) {
        case HOME: {
            enemy_index = -1;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (%i) (OP_DEBUG), isBoss == -1",
                    enemy_index);
            isBoss = -1;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Doing endwin() before debug_generic()");
            endwin();
            debug_generic(gmst, actor, path, room_index, kls, t_kls);
            res = OP_OK;
        }
        break;
        case ENEMIES: {
            enemy_index = enemy->index;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (%i) (OP_DEBUG), isBoss == 0",
                    enemy->index);
            isBoss = 0;
            debug_enemies_room(gmst, room, actor, enemy, path,
                               room_index, enemy_index, kls, t_kls);
            res = OP_OK;
        }
        break;
        case BOSS: {
            enemy_index = 0;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (0) (OP_DEBUG), isBoss == 1");
            isBoss = 1;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Doing endwin() before debug_generic()");
            endwin();
            debug_generic(gmst, actor, path, room_index, kls, t_kls);
            res = OP_OK;
        }
        break;
        default: {
            log_tag("debug_log.txt", "[TURNOP]",
                    "Invalid room value in turnOP(OP_DEBUG): [%s (%i)]",
                    stringFromRoom(room->class), room->class);
            exit(EXIT_FAILURE);
        }
        }
    }
    break;
    case OP_EQUIPS: {
        if (actor == NULL || path == NULL) {
            if (actor == NULL)
                log_tag("debug_log.txt", "[ERROR]",
                        "turnOP_args->(actor) was NULL");
            if (path == NULL)
                log_tag("debug_log.txt", "[ERROR]",
                        "turnOP_args->(path) was NULL");
            //free(args);
            log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
            return res;
        }
        handleEquips(actor, path);
        res = OP_OK;
    };
    break;
    case OP_PERKS: {
        if (actor == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "turnOP_args->(actor) was NULL");
            //free(args);
            log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
            return res;
        }
        printActivePerks(actor);
        res = OP_OK;
    }
    break;
    case OP_STATS: {
        if (actor == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "turnOP_args->(actor) was NULL");
            //free(args);
            log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
            return res;
        }
        handleStats(actor);
        res = OP_OK;
    }
    break;
    case OP_ARTIFACTS: {
        if (actor == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "turnOP_args->(actor) was NULL");
            //free(args);
            log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
            return res;
        }
        handleArtifacts(actor);
        res = OP_OK;
    }
    break;
    case OP_EXPLORE: {
        log_tag("debug_log.txt", "[DEBUG]", "Going to explore!");
        //The free of args should be done after the OP; in handleRoom_Home()
        res = NO_OP;
    }
    break;
    case OP_QUIT: {
        quit(actor, room, load_info, t_kls);
        //FIXME
        //We can't free the turnOP_args, can we?
        //free(args);
        log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
    }
    break;
    case OP_SKILL: {

        log_tag("debug_log.txt", "[DEBUG]", "OP_SKILL");

        res = NO_OP;

        if (notify_win == NULL) {
            log_tag("debug_log.txt", "[CRITICAL]",
                    "Notification WINDOW pointer was null in turnOP(OP_SKILL)");
            exit(EXIT_FAILURE);
        }
        if (room == NULL) {
            log_tag("debug_log.txt", "[CRITICAL]",
                    "Room pointer was null in turnOP(OP_SKILL)");
            exit(EXIT_FAILURE);
        }
        if (foe_op == FOE_OP_INVALID) {
            log_tag("debug_log.txt", "[CRITICAL]",
                    "foe_op was FOE_OP_INVALID in turnOP(OP_SKILL)");
            exit(EXIT_FAILURE);
        } else if (foe_op < 0 || foe_op > FOETURNOP_MAX) {
            log_tag("debug_log.txt", "[CRITICAL]",
                    "foe_op was invalid in turnOP(OP_SKILL): [%i]", foe_op);
            exit(EXIT_FAILURE);
        }
        if (skill < 0 || skill > SKILLSTOTAL) {
            log_tag("debug_log.txt", "[CRITICAL]",
                    "skill was invalid in turnOP(OP_SKILL): [%i]", skill);
            exit(EXIT_FAILURE);
        }
        room_index = room->index;
        if (room->class == ENEMIES && enemy == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Enemy pointer was null in turnOP(OP_SKILL) for ENEMIES room.");
            exit(EXIT_FAILURE);
        } else if (room->class == BOSS && boss == NULL) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Boss pointer was null in turnOP(OP_SKILL) for BOSS room.");
            exit(EXIT_FAILURE);
        } else if (room->class != ENEMIES && room->class != BOSS) {
            log_tag("debug_log.txt", "[ERROR]",
                    "Invalid room class in turnOP(OP_SKILL): (%s [%i])",
                    stringFromRoom(room->class), room->class);
            exit(EXIT_FAILURE);
        }
        switch (room->class) {
        case ENEMIES: {
            enemy_index = enemy->index;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (%i) (OP_SKILL), isBoss == 0",
                    enemy->index);
            isBoss = 0;
            //TODO
            //Implement the missing function to wrap skill usage and foe op
            res = OP_res_from_fightResult(defer_skill_enemy(actor, enemy, skill, foe_op, notify_win, kls));
        }
        break;
        case BOSS: {
            enemy_index = 0;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (0) (OP_SKILL), isBoss == 1");
            isBoss = 1;
            //TODO
            //Implement the missing function to wrap skill usage and foe op
            res = OP_res_from_fightResult(defer_skill_boss(actor, boss, skill, path, foe_op, notify_win, kls));
        }
        break;
        default: {
            log_tag("debug_log.txt", "[TURNOP]",
                    "Invalid room value in turnOP(OP_FIGHT): [%s (%i)]",
                    stringFromRoom(room->class), room->class);
            exit(EXIT_FAILURE);

        }
        break;
        }
    }
    break;
    default: {
        log_tag(OPS_LOGFILE, "[ERROR]", "Invalid OP in turnOP()");
        log_tag("debug_log.txt", "[ERROR]", "Invalid OP in turnOP()");
    }
    break;
    }

    //Log end of operation
    log_tag("debug_log.txt", "[TURNOP]", "Done operation: [%s] res: [%s (%i)]",
            stringFromTurnOP(op), stringFrom_OP_res(res), res);

    log_tag(OPS_LOGFILE, "[RES]", "res: [%s (%i)]", stringFrom_OP_res(res),
            res);

    return res;
}

fightResult do_Skill(Fighter * player, Enemy * e, skillType picked_skill, WINDOW * notify_win, Koliseo * kls)
{

    return FIGHTRES_NO_DMG;
}

fightResult do_Skill_boss(Fighter * player, Boss * b, skillType picked_skill, Path * path, WINDOW * notify_win, Koliseo * kls)
{

    return FIGHTRES_NO_DMG;
}

//TODO Drop dead code
/*
 * Takes an integer, a callback_void_t pointer function and a Fighter holding the array for the callback registration.
 * Not working as of v0.5.2.
 * Registers the pointer to the function pointer array for counter callback.
 * @see Turncounter
 * @param index An integer.
 * @param ptr A pointer to function of type callback_void_t.
 * @param f The fighter pointer holding the callback array.
 *
void register_counter_callback(int index, callback_void_t ptr, Fighter* f) {
	f->callback_counter_ptrs[index] = ptr;
}
*/

/**
 * Takes a Equip and a Fighter pointers.
 * Iterates over the equip's perks and removes them to the fighter perks.
 * @see Perk
 * @see Equip
 * @see Fighter
 * @param e An Equip pointer.
 * @param f A Fighter pointer.
 */
void removeEquipPerks(Equip *e, Fighter *f)
{

    for (int i = 0; i < (e->perksCount); i++) {
        Perk *p = e->perks[i];

        Perk *fighterPerk = f->perks[p->class];
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
void printActivePerks(Fighter *f)
{

    WINDOW *win;

    /* Initialize curses */
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
    print_label(win, 1, 0, 35, "Perks", COLOR_PAIR(S4C_BRIGHT_YELLOW));
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
        Perk *p = f->perks[i];
        if (p->innerValue > 0) {
            empty = 0;

            wattron(win, COLOR_PAIR(S4C_CYAN));
            mvwprintw(win, y, x, " x%i %s ", p->innerValue,
                      nameStringFromPerk(p->class));
            wattroff(win, COLOR_PAIR(S4C_CYAN));
            char s[250];
            sprintf(s, " x%i %s", p->innerValue, nameStringFromPerk(p->class));
            int l = strlen(s);
            wattron(win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
            mvwprintw(win, y, x + l + 2, "\"%s\"",
                      descStringFromPerk(p->class));
            wattroff(win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
            y++;
        }
    };

    if (empty) {		//No perks are active
        wattron(win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
        mvwprintw(win, y, x, "You don't have any special power yet.");
        wattroff(win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
    }

    refresh();

    int picked = 0;
    int c = -1;
    wrefresh(win);

    while (!picked && (c = wgetch(win)) != 'q') {
        switch (c) {
        case 10: {		/*Enter */
            picked = 1;

        };
        break;
        }
    }
    delwin(win);
    endwin();
}

//Counter functions


//TODO update this to actually handle different types of counters and reliably print them, maybe to a ncurses window
/**
 * Takes a Turncounter array.
 * For every Turncounter in the array, the values of count, innerVal, type and all the function pointers fields are printed.
 * @see Turncounter
 * @param counters The Turncounter array to be printed.
 */
void printCounters(Turncounter *counters[])
{
    yellow();
    printf
    ("%-10.10s\t%-10.10s\t%-3.3s\t%-3.3s\t%-11.11s\t%-11.11s\t%-11.11s\t%-11.11s\n",
     "Count", "Desc", "Val", "Typ", "*(eff())", "*(eff_e())", "*(boost())",
     "*(boost_e())");
    for (int i = 0; i < (COUNTERSMAX + 1); i++) {
        Turncounter *c = counters[i];
        lightBlue();
        printf("%-10.10i\t%-10.10s\t", c->count, c->desc);
        strongWhite();
        printf("(%-3.3i)\t(%-3.3i)\t", c->innerValue, c->type);
        purple();
        //printf("[%-11.11i]\t[%-11.11i]\t",*(c->effect_fun),*(c->effect_e_fun));
        cyan();
        //printf("[%-11.11i]\t[%-11.11i]\n",*(c->boost_fun), *(c->boost_e_fun));
    };
    white();
}

/**
 * Takes a Turncounter pointer and an integer.
 * If the count value at the pointer is 0 (counter is inactive), the turns valueis assigned.
 * @see Turncounter
 * @param c The Turncounter whose count value will be set.
 * @param turns The value to be assigned.
 */
void setCounter(Turncounter *c, int turns)
{

    if (c->count == 0) {	// Counter is NOT already active
        c->count = turns;
    } else {
        //Handle counters already activ
    }
}

/**
 * Takes a Fighter pointer and prints most of its values formatted.
 * @see Fighter
 * @see stringFromClass()
 * @see stringFromStatus()
 * @param f The Fighter pointer with stats to print.
 */
void printStats(Fighter *f)
{

    printf("\t%s's stats:\n\n", f->name);
    printf("\t\tClass:\t%s\n", stringFromClass(f->class));
    printf("\t\tHp:\t%i/%i\n", f->hp, f->totalhp);
    printf("\t\tEnergy:\t%i/%i\n", f->energy, f->totalenergy);
    printf("\t\tAtk:\t%i\n", f->atk);
    printf("\t\tDef:\t%i\n", f->def);
    printf("\t\tVel:\t%i\n", f->vel);
    printf("\t\tLvl:\t%i\n", f->level);
    printf("\t\tCurrent Level exp:\t%i/%i\n", f->currentlevelxp,
           f->totallevelxp);
    printf("\t\tTotal exp:\t%i\n", f->totalxp);
    printf("\t\tStatus:\t%s\n", stringFromStatus(f->status));
}

/**
 * Takes a Enemy pointer and prints most of its values formatted.
 * @see Enemy
 * @see stringFromEClass()
 * @see stringFromStatus()
 * @param e The Enemy pointer with stats to print.
 */
void printEStats(Enemy *e)
{
    if (e->beast) {
        lightRed();
    }
    printf("\t%s's stats:\n", stringFromEClass(e->class));
    if (e->beast) {
        white();
    }
    printf("\tHp:\t%i/%i\n", e->hp, e->totalhp);
    printf("\tEnergy:\t%i/%i\n", e->energy, e->totalenergy);
    printf("\tAtk:\t%i\n", e->atk);
    printf("\tDef:\t%i\n", e->def);
    printf("\tVel:\t%i\n", e->vel);
    printf("\tLvl:\t%i\n", e->level);
    printf("\tXp:\t%i\n", e->xp);
    printf("\tStatus:\t%s\n", stringFromStatus(e->status));
}

/**
 * Takes a Consumable pointer and prints most of its values formatted.
 * @see Consumable
 * @param c The Consumable pointer with stats to print.
 */
void printConsumablesStats(Consumable *c)
{
    printf("  (%i)\t%i\t%s\t\"%s\"\n", c->class, c->qty, c->name, c->desc);
}

/**
 * Takes a Artifact pointer and prints most of its values formatted.
 * @see Artifact
 * @param a The Artifact pointer with stats to print.
 */
void printArtifactStats(Artifact *a)
{
    printf("  (%i)\t\t", a->class);
    purple();
    printf("%s\t", a->name);
    yellow();
    printf("\"%s\"\n", a->desc);
    white();
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
void initWincon(Wincon *w, Path *p, winconClass class)
{

    w->class = class;

    switch (w->class) {
    case ALL_BOSSES: {
        w->current_val = 0;
        w->target_val = BOSSCLASSESMAX + 1;

    };
    break;
    case ALL_ARTIFACTS: {
        w->current_val = 0;
        w->target_val = ARTIFACTSMAX + 1;

    };
    break;
    case FULL_PATH: {
        w->current_val = 0;
        w->target_val = p->length;
    };
    break;
    default: {
        fprintf(stderr, "\nUnexpected Wincon Class %i\n", w->class);
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
void getParams(int argc, char **argv, Fighter *player, Path *path, int optTot,
               Koliseo *kls)
{

    int argTot = argc - optTot;
    if (argTot == 0) {
        pickName(player);
        pickClass(player);
        kls_log(kls, "DEBUG", "Prepping Wincon");
        Wincon *w =
            (Wincon *) KLS_PUSH_TYPED(kls, Wincon, HR_Wincon, "Wincon",
                                      "Wincon");
        if (GAMEMODE == Story) {
            //Path length must be already initialised before getting here.
            initWincon(w, path, FULL_PATH);
        } else if (GAMEMODE == Rogue) {
            //Path length must be already initialised before getting here.
            initWincon(w, path, ALL_ARTIFACTS);
        } else {
            pickWincon(w);
            initWincon(w, path, w->class);
        }
        path->win_condition = w;
    } else if (argTot == 1 || argTot == 2) {
        char read_name[25];
        int i = 0, len = 0;
        len = strlen(argv[optTot]);
        if (len < 20) {
            for (; i < len; i++) {
                read_name[i] = argv[optTot][i];
            }
            read_name[i] = '\0';
            strcpy(player->name, read_name);
        } else {
            pickName(player);
        }
    } else {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argTot == 1) {
        pickClass(player);
        kls_log(kls, "DEBUG", "Prepping Wincon");
        Wincon *w =
            (Wincon *) KLS_PUSH_TYPED(kls, Wincon, HR_Wincon, "Wincon",
                                      "Wincon");
        if (GAMEMODE == Story) {
            //Path length must be already initialised before getting here.
            initWincon(w, path, FULL_PATH);
        } else if (GAMEMODE == Rogue) {
            //Path length must be already initialised before getting here.
            initWincon(w, path, ALL_ARTIFACTS);
        } else {
            pickWincon(w);
            initWincon(w, path, w->class);
        }
        path->win_condition = w;
    }

    if (argTot == 2) {
        int c = -1;
        int i = 0;
        for (i = 0; i <= CLASSESMAX; i++) {
            if (strcmp(argv[optTot + 1], classesstrings[i]) == 0) {
                c = 1;
                player->class = i;
                break;
            }
        }
        if (c < 0) {
            pickClass(player);
        }
        kls_log(kls, "DEBUG", "Prepping Wincon");
        Wincon *w =
            (Wincon *) KLS_PUSH_TYPED(kls, Wincon, HR_Wincon, "Wincon",
                                      "Wincon");
        if (GAMEMODE == Story) {
            //Path length must be already initialised before getting here.
            initWincon(w, path, FULL_PATH);
        } else if (GAMEMODE == Rogue) {
            //TODO: what do we set as path length? Number of floors?
            //Path length must be already initialised before getting here.
            initWincon(w, path, ALL_ARTIFACTS);
        } else {
            pickWincon(w);
            initWincon(w, path, w->class);
        }
        path->win_condition = w;
    }
}

/**
 * Returns a randomised pick as foeTurnOption.
 * @param e Pointer to the enemy to pick for.
 * @param f Pointer to the target fighter.
 * @return The chosen foeturnOption value representing turn action.
 */
foeTurnOption enemyTurnPick(Enemy *e, Fighter *f)
{
    if (e == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "enemyTurnPick():  Enemy was NULL.");
        exit(EXIT_FAILURE);
    }
    if (f == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "enemyTurnPick():  Fighter was NULL.");
        exit(EXIT_FAILURE);
    }
    foeTurnOption pick = FOE_INVALID;

    while (pick == FOE_INVALID) {
        int rn = rand() % 101;
        /*
           if (rn > 80) {
           //TODO
           //pick = FOE_SPECIAL;
           pick = FOE_IDLE;

           } else
         */
        if (rn > 40) {
            pick = FOE_FIGHT;
        } else {
            pick = FOE_IDLE;
        }
    }

    log_tag("debug_log.txt", "[FOETURNOPTION]", "Pick was: ( %i ) [ %s ]", pick,
            stringFromFoeTurnOP(foeTurnOP_from_foeTurnOption(pick)));

    if (pick == FOE_INVALID) {
        fprintf(stderr, "Error: unexpected turn choice value");
        log_tag("debug_log.txt", "[ERROR]",
                "Unexpected turn choice in enemyTurnPick(), quitting");
        exit(EXIT_FAILURE);
    }
    return pick;
}

/**
 * Returns a randomised pick as foeTurnOption.
 * @param b Pointer to the boss to pick for.
 * @param f Pointer to the target fighter.
 * @return The chosen foeturnOption value representing turn action.
 */
foeTurnOption bossTurnPick(Boss *b, Fighter *f)
{
    if (b == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "bossTurnPick():  Boss was NULL.");
        exit(EXIT_FAILURE);
    }
    if (f == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "bossTurnPick():  Fighter was NULL.");
        exit(EXIT_FAILURE);
    }
    foeTurnOption pick = FOE_INVALID;

    while (pick == FOE_INVALID) {
        int rn = rand() % 101;
        /*
           if (rn > 80) {
           //TODO
           //pick = FOE_SPECIAL;
           pick = FOE_IDLE;
           } else
         */
        if (rn > 40) {
            pick = FOE_FIGHT;
        } else {
            pick = FOE_IDLE;
        }
    }

    log_tag("debug_log.txt", "[FOETURNOPTION]", "Pick was: ( %i ) [ %s ]", pick,
            stringFromFoeTurnOP(foeTurnOP_from_foeTurnOption(pick)));

    if (pick == FOE_INVALID) {
        fprintf(stderr, "Error: unexpected turn choice value");
        log_tag("debug_log.txt", "[ERROR]",
                "Unexpected turn choice in enemyTurnPick(), quitting");
        exit(EXIT_FAILURE);
    }
    return pick;
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
void statReset(Fighter *player, int force)
{
    log_tag("debug_log.txt", "[DEBUG]",
            "Call to statReset() with ($force) == (%i)", force);
    if (!force && (player->hp >= 0.5 * player->totalhp)
        && !(player->atk <= 0 || player->def <= 0 || player->vel <= 0)) {
        return;
    }

    int boost = getBoost(player->level, player->luck);

    BaseStats *base = &basestats[player->class];
    if (force || player->vel <= 0.3 * (base->vel + boost)
        || player->atk <= 0.35 * (base->atk + boost)
        || player->def <= 0.18 * (base->def + boost)) {
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
int fight(Fighter *player, Enemy *e, WINDOW *notify_win, Koliseo *kls)
{

    fightResult res = FIGHTRES_NO_DMG;
    char msg[200];
    //Stat comparisons
    //

    int atkdelta = (player->atk + player->equipboost_atk) - e->atk - (rand() % 3) - 1;	//Skewed with defender
    int defdelta = (player->def + player->equipboost_def) - e->def + (rand() % 2) + 1;	//Skewed with attacker
    int veldelta =
        (player->vel + player->equipboost_vel) - e->vel + (rand() % 3) + 1;

    int atkOnPlayer =
        e->atk - (player->def + player->equipboost_def + (player->vel / 6));
    int atkOnEnemy =
        (player->atk + player->equipboost_atk) - (e->def + (e->vel / 6));

    if (G_GODMODE_ON == 1) {
        log_tag("debug_log.txt", "[DEBUG]", "fight():  G_GODMODE_ON == 1");
        atkdelta = 100;
        defdelta = 100;
        veldelta = 100;
        atkOnPlayer = 1;
        atkOnEnemy = 100;
    }

    int damageDealt = -1;
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkdelta %i", atkdelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "defdelta %i", defdelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "veldelta %i", veldelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkOnEnemy %i", atkOnEnemy);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkOnPlayer %i", atkOnPlayer);

    if (veldelta >= 0) {

        if (atkOnEnemy > 3) {
            damageDealt = atkOnEnemy;
            e->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result A WIN (faster, great atk).");
        } else if (atkOnEnemy >= 0) {
            damageDealt = abs(atkOnEnemy - atkdelta);
            e->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result B WIN (faster, ok atk).");
        } else {
            if (atkOnEnemy > -3) {
                damageDealt =
                    fabsf(atkOnPlayer -
                          0.75F * (player->vel + player->equipboost_vel));
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result C1 LOST (faster, atk > -3).");
            } else {
                damageDealt = abs(atkOnPlayer - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result C2 LOST (faster, atk < -3).");
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
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result D LOST (slower, great enemy atk).");
        } else if (atkOnPlayer >= 0) {
            damageDealt = abs(atkOnPlayer - atkdelta);
            player->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result E LOST (slower, ok enemy atk).");
        } else {
            if (atkOnPlayer > -3) {
                damageDealt = fabsf(atkOnEnemy - 0.75F * e->vel);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result F1 WIN (slower, enemy atk > -3).");
            } else {
                damageDealt = abs(atkOnEnemy - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result F2 WIN (slower, enemy atk < -3).");
            }
            e->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
        }
    }
    log_tag("debug_log.txt", "[FIGHT]", "damageCalc %i", damageDealt);

    int yourhit = (res == FIGHTRES_DMG_DEALT) ? 1 : 0;
    char victim[25];

    if (!yourhit) {

        e->vel--;
        e->atk--;
        e->def -= 2;

        //Check if someone earned a stat reset after the fight
        statReset(player, 0);
        statResetEnemy(e, 0);

        strcpy(victim, player->name);
    } else {

        player->vel--;
        player->atk--;
        player->def -= 2;

        //Account for vampirism perk
        int vampire_perks = player->perks[VAMPIRISM]->innerValue;
        if (vampire_perks > 0) {
            int recovery = floor(damageDealt * (0.1 * vampire_perks));
            player->hp += recovery;
            log_tag("debug_log.txt", "[PERKS]", "Vampirism proc for +%i HP.",
                    recovery);
            if (player->hp >= player->totalhp) {
                player->hp = player->totalhp;
            };
        }
        //Account for burn on touch perk
        int hotbody_perks = player->perks[HOT_BODY]->innerValue;
        if (hotbody_perks > 0) {
            int burnchance = 11 - hotbody_perks;
            if (rand() % burnchance == 0) {
                //TODO
                //Handle multiple statuses
                e->status = Burned;	//Set status to Burned. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) e->counters[Burned], 2);	//Give 2 turns of Burned status
                log_tag("debug_log.txt", "[PERKS]",
                        "Hotbody proc on 1/%i chance.", burnchance);
            }
        }
        //Account for poison on touch perk. Order of checks with hot_body perk may cause issues?
        int biohazard_perks = player->perks[BIOHAZARD]->innerValue;
        if (biohazard_perks > 0) {
            int poisonchance = 11 - biohazard_perks;
            if (rand() % poisonchance == 0) {
                e->status = Poison;	//Set status to Poison. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) e->counters[POISON], 2);	//Give 2 turns of Poison status
                log_tag("debug_log.txt", "[PERKS]",
                        "Biohazard proc on 1/%i chance.", poisonchance);
            }
        }
        //Check if someone earned a stat reset after the fight
        statResetEnemy(e, 0);
        statReset(player, 0);

        strcpy(victim, stringFromEClass(e->class));
    }

    int color = -1;
    if (yourhit) {
        color = S4C_WHITE;
    } else {
        color = S4C_RED;
    }

    sprintf(msg, "%s was hit.    (%i DMG)", victim,
            damageDealt > 0 ? damageDealt : 1);
    wattron(notify_win, COLOR_PAIR(color));
    display_notification(notify_win, msg, 500);
    wattroff(notify_win, COLOR_PAIR(color));

    //Rolls
    //
    //Critical hit roll

    //Account for critboost_chance perks
    int critboost_value = 1.5 * player->perks[CRITBOOST_CHANCE]->innerValue;
    int critMax = round(10.0 - floor(player->luck / 5) - (critboost_value));

    int critRes = (rand() % critMax);

    if (res == FIGHTRES_DMG_DEALT && (critRes <= 0)) {

        //Account for critboost_dmg perks
        int dmgboost_perks = player->perks[CRITBOOST_DMG]->innerValue;
        damageDealt *= (0.30 + (0.12 * dmgboost_perks));
        e->hp -= (damageDealt > 0 ? damageDealt : 1);
        log_tag("debug_log.txt", "[FIGHT]",
                "Critical hit for %i dmg, proc on 1/%i chance.", damageDealt,
                critMax);
        log_tag("debug_log.txt", "[PERKS]", "Critical hit, critboost was %i.",
                critboost_value);

        sprintf(msg, "A critical hit!    (%i DMG)",
                damageDealt > 0 ? damageDealt : 1);
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        //Update stats
        player->stats->criticalhits++;
    }
    //Check for deaths -> exit condition from loop
    //
    //
    //
    if (e->hp <= 0) {
        res = FIGHTRES_KILL_DONE;

        //Account for runic circle perk
        int runic_perks = player->perks[RUNIC_MAGNET]->innerValue;
        if (runic_perks > 0) {
            int recovery = round(0.51 * runic_perks);
            player->energy += recovery;
            log_tag("debug_log.txt", "[PERKS]",
                    "Runicmagnet proc for %i energy.", recovery);
        }
        if (e->beast) {
            color = S4C_MAGENTA;
        } else {
            color = S4C_RED;
        }
        wattron(notify_win, COLOR_PAIR(color));
        sprintf(msg, "%s fainted.", stringFromEClass(e->class));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(color));

        log_tag("debug_log.txt", "[FIGHT]", "Killed  %s.",
                stringFromEClass(e->class));

        //Update stats
        player->stats->enemieskilled++;
    } else {
        //Apply status effects to enemy
        if (e->status != Normal) {
            applyEStatus(notify_win, e);
            log_tag("debug_log.txt", "[STATUS]", "Applied  %s to %s.",
                    stringFromStatus(e->status), stringFromEClass(e->class));
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
    if (res == FIGHTRES_KILL_DONE
        && (e->beast || ((rand() % 9) - (player->luck / 10) <= 0))) {
        int drop = dropConsumable(player);
        sprintf(msg, "You found a %s!", stringFromConsumables(drop));
        wattron(notify_win, COLOR_PAIR(S4C_CYAN));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_CYAN));
        log_tag("debug_log.txt", "[DROPS]", "Found Consumable:    %s.",
                stringFromConsumables(drop));
    }

    //Artifact drop (if we don't have all of them), guaranteed on killing a beast
    if ((player->stats->artifactsfound != ARTIFACTSMAX + 1)
        && res == FIGHTRES_KILL_DONE && (e->beast
                                         ||
                                         ((rand() % ENEMY_ARTIFACTDROP_CHANCE) -
                                          (player->luck / 10) <= 0))) {
        int artifact_drop = dropArtifact(player);
        sprintf(msg, "You found a %s!", stringFromArtifacts(artifact_drop));
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        log_tag("debug_log.txt", "[DROPS]", "Found Artifact:    %s.",
                stringFromArtifacts(artifact_drop));
        if (!e->beast)
            log_tag("debug_log.txt", "[.1%% CHANCE]",
                    "\nNORMAL ENEMY DROPPED ARTIFACT! 0.1%% chance??\n");
    }

    //Equip drop, guaranteed on killing a beast
    if (res == FIGHTRES_KILL_DONE
        && (e->beast || ((rand() % 15) - (player->luck / 10) <= 0))) {
        dropEquip(player, e->beast, notify_win, kls);
    }
    return res;
}

/**
 * Takes an Enemy and a Fighter pointer and compares their stats to determine who gets damaged and returns the fightStatus value.
 * Prints notifications to the passed WINDOW pointer.
 * On enemy death, there's a chance to call dropConsumable, dropEquip or dropArtifact (guaranteed for beast enemies).
 * NOTE:  that the return values are always from the POV of the Fighter: FIGHTRES_DMG_DEALT means the Enemy was damaged!
 * @see defer_fight_enemy()
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
int enemy_attack(Enemy *e, Fighter *target, WINDOW *notify_win, Koliseo *kls)
{
    //Implementation similar to fight(), as a base idea
    //Should return fightResult values, while keeping the perspective on the Fighter, as in:
    //
    //      FIGHTRES_DEATH means the Fighter died
    //      FIGHTRES_KILL_DONE means the Enemy died
    //      FIGHTRES_DMG_DEALT means the Fighter inflicted damage
    //      FIGHRES_DMG_TAKEN means the Fighter received damage

    fightResult res = FIGHTRES_NO_DMG;
    char msg[200];
    //Stat comparisons
    //

    int atkdelta = (e->atk + e->turnboost_atk) - (target->atk + target->equipboost_atk + target->turnboost_atk - (rand() % 3)) - 1;	//Skewed with defender
    int defdelta = (e->def + e->turnboost_def) - (target->def + target->equipboost_def + target->turnboost_def) + (rand() % 2) + 1;	//Skewed with attacker
    int veldelta =
        (e->vel + e->turnboost_vel) - (target->vel + target->equipboost_vel +
                                       target->turnboost_vel) + (rand() % 3) +
        1;

    int atkOnPlayer =
        (e->atk + e->turnboost_atk) - (target->def + target->equipboost_def +
                                       target->turnboost_def +
                                       (target->vel / 6));
    int atkOnEnemy =
        (target->atk + target->equipboost_atk + target->turnboost_atk) -
        (e->def + e->turnboost_def + (e->vel / 6));

    if (G_GODMODE_ON == 1) {
        log_tag("debug_log.txt", "[DEBUG]", "[%s]:  G_GODMODE_ON == 1",
                __func__);
        atkdelta = -100;
        defdelta = -100;
        veldelta = -100;
        atkOnPlayer = 1;
        atkOnEnemy = 100;
    }

    int damageDealt = -1;
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkdelta %i", atkdelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "defdelta %i", defdelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "veldelta %i", veldelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkOnEnemy %i", atkOnEnemy);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkOnPlayer %i", atkOnPlayer);

    if (veldelta >= 0) {	//Enemy has a non-negative veldelta
        if (atkOnPlayer > 3) {
            damageDealt = atkOnPlayer;
            target->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "[%s]:  Fight result D LOST (slower, great enemy atk).",
                    __func__);
        } else if (atkOnPlayer >= 0) {
            damageDealt = abs(atkOnPlayer - atkdelta);
            target->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "[%s]:  Fight result E LOST (slower, ok enemy atk).",
                    __func__);
        } else {
            if (atkOnPlayer > -3) {
                damageDealt = fabsf(atkOnEnemy - 0.75F * e->vel);
                log_tag("debug_log.txt", "[FIGHT]",
                        "[%s]:  Fight result F1 WIN (slower, enemy atk > -3).",
                        __func__);
            } else {
                damageDealt = abs(atkOnEnemy - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "[%s]:  Fight result F2 WIN (slower, enemy atk < -3).",
                        __func__);
            }
            e->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
        }
    } else {			//Enemy veldelta is not strictly positive
        atkdelta = -atkdelta;
        if (atkOnEnemy > 3) {
            damageDealt = atkOnEnemy;
            e->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "[%s]:  Fight result A WIN (faster, great atk).", __func__);
        } else if (atkOnEnemy >= 0) {
            damageDealt = abs(atkOnEnemy - atkdelta);
            e->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "[%s]:  Fight result B WIN (faster, ok atk).", __func__);
        } else {
            if (atkOnEnemy > -3) {
                damageDealt =
                    fabsf(atkOnPlayer -
                          0.75F * (target->vel + target->equipboost_vel));
                log_tag("debug_log.txt", "[FIGHT]",
                        "[%s]:  Fight result C1 LOST (faster, atk > -3).",
                        __func__);
            } else {
                damageDealt = abs(atkOnPlayer - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "[%s]:  Fight result C2 LOST (faster, atk < -3).",
                        __func__);
            }
            target->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
        }
    }
    log_tag("debug_log.txt", "[FIGHT]", "[%s]:  damageCalc %i", __func__,
            damageDealt);

    int playerhit = (res == FIGHTRES_DMG_DEALT) ? 1 : 0;
    char victim[25];

    if (!playerhit) {

        e->vel--;
        e->atk--;
        e->def -= 2;

        //Check if someone earned a stat reset after the fight
        statReset(target, 0);
        statResetEnemy(e, 0);

        strcpy(victim, target->name);
    } else {

        target->vel--;
        target->atk--;
        target->def -= 2;

        //Account for vampirism perk
        int vampire_perks = target->perks[VAMPIRISM]->innerValue;
        if (vampire_perks > 0) {
            int recovery = floor(damageDealt * (0.1 * vampire_perks));
            target->hp += recovery;
            log_tag("debug_log.txt", "[PERKS]", "Vampirism proc for +%i HP.",
                    recovery);
            if (target->hp >= target->totalhp) {
                target->hp = target->totalhp;
            };
        }
        //Account for burn on touch perk
        int hotbody_perks = target->perks[HOT_BODY]->innerValue;
        if (hotbody_perks > 0) {
            int burnchance = 11 - hotbody_perks;
            if (rand() % burnchance == 0) {
                //TODO
                //Handle multiple statuses
                e->status = Burned;	//Set status to Burned. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) e->counters[Burned], 2);	//Give 2 turns of Burned status
                log_tag("debug_log.txt", "[PERKS]",
                        "Hotbody proc on 1/%i chance.", burnchance);
            }
        }
        //Account for poison on touch perk. Order of checks with hot_body perk may cause issues?
        int biohazard_perks = target->perks[BIOHAZARD]->innerValue;
        if (biohazard_perks > 0) {
            int poisonchance = 11 - biohazard_perks;
            if (rand() % poisonchance == 0) {
                e->status = Poison;	//Set status to Poison. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) e->counters[POISON], 2);	//Give 2 turns of Poison status
                log_tag("debug_log.txt", "[PERKS]",
                        "Biohazard proc on 1/%i chance.", poisonchance);
            }
        }
        //Check if someone earned a stat reset after the fight
        statResetEnemy(e, 0);
        statReset(target, 0);

        strcpy(victim, stringFromEClass(e->class));
    }

    int color = -1;
    if (playerhit) {
        color = S4C_WHITE;
    } else {
        color = S4C_RED;
    }

    sprintf(msg, "%s was hit.    (%i DMG)", victim,
            damageDealt > 0 ? damageDealt : 1);
    wattron(notify_win, COLOR_PAIR(color));
    display_notification(notify_win, msg, 500);
    wattroff(notify_win, COLOR_PAIR(color));

    //Rolls
    //
    //Critical hit roll

    //Account for critboost_chance perks
    int critboost_value = 1.5 * target->perks[CRITBOOST_CHANCE]->innerValue;
    int critMax = round(10.0 - floor(target->luck / 5) - (critboost_value));

    int critRes = (rand() % critMax);

    if (res == FIGHTRES_DMG_DEALT && (critRes <= 0)) {

        //Account for critboost_dmg perks
        int dmgboost_perks = target->perks[CRITBOOST_DMG]->innerValue;
        damageDealt *= (0.30 + (0.12 * dmgboost_perks));
        e->hp -= (damageDealt > 0 ? damageDealt : 1);
        log_tag("debug_log.txt", "[FIGHT]",
                "Critical hit for %i dmg, proc on 1/%i chance.", damageDealt,
                critMax);
        log_tag("debug_log.txt", "[PERKS]", "Critical hit, critboost was %i.",
                critboost_value);

        sprintf(msg, "A critical hit!    (%i DMG)",
                damageDealt > 0 ? damageDealt : 1);
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        //Update stats
        target->stats->criticalhits++;
    }
    //Check for deaths -> exit condition from loop
    //
    //
    //
    if (e->hp <= 0) {
        res = FIGHTRES_KILL_DONE;

        //Account for runic circle perk
        int runic_perks = target->perks[RUNIC_MAGNET]->innerValue;
        if (runic_perks > 0) {
            int recovery = round(0.51 * runic_perks);
            target->energy += recovery;
            log_tag("debug_log.txt", "[PERKS]",
                    "Runicmagnet proc for %i energy.", recovery);
        }
        if (e->beast) {
            color = S4C_MAGENTA;
        } else {
            color = S4C_RED;
        }
        wattron(notify_win, COLOR_PAIR(color));
        sprintf(msg, "%s fainted.", stringFromEClass(e->class));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(color));

        log_tag("debug_log.txt", "[FIGHT]", "Killed  %s.",
                stringFromEClass(e->class));

        //Update stats
        target->stats->enemieskilled++;
    } else {
        //Apply status effects to enemy
        if (e->status != Normal) {
            applyEStatus(notify_win, e);
            log_tag("debug_log.txt", "[STATUS]", "Applied  %s to %s.",
                    stringFromStatus(e->status), stringFromEClass(e->class));
        }
    }

    if (target->hp <= 0) {
        log_tag("debug_log.txt", "[DEBUG]", "[%s]:  Target died. Hp: (%i)",
                __func__, target->hp);
        res = FIGHTRES_DEATH;
    } else {
        //Apply status effects to target
        if (target->status != Normal) {
            applyStatus(notify_win, target);
        }
    }

    //Consumable drop, guaranteed on killing a beast
    if (res == FIGHTRES_KILL_DONE
        && (e->beast || ((rand() % 9) - (target->luck / 10) <= 0))) {
        int drop = dropConsumable(target);
        sprintf(msg, "You found a %s!", stringFromConsumables(drop));
        wattron(notify_win, COLOR_PAIR(S4C_CYAN));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_CYAN));
        log_tag("debug_log.txt", "[DROPS]", "Found Consumable:    %s.",
                stringFromConsumables(drop));
    }

    //Artifact drop (if we don't have all of them), guaranteed on killing a beast
    if ((target->stats->artifactsfound != ARTIFACTSMAX + 1)
        && res == FIGHTRES_KILL_DONE && (e->beast
                                         ||
                                         ((rand() % ENEMY_ARTIFACTDROP_CHANCE) -
                                          (target->luck / 10) <= 0))) {
        int artifact_drop = dropArtifact(target);
        sprintf(msg, "You found a %s!", stringFromArtifacts(artifact_drop));
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        log_tag("debug_log.txt", "[DROPS]", "Found Artifact:    %s.",
                stringFromArtifacts(artifact_drop));
        if (!e->beast)
            log_tag("debug_log.txt", "[.1%% CHANCE]",
                    "\nNORMAL ENEMY DROPPED ARTIFACT! 0.1%% chance??\n");
    }

    //Equip drop, guaranteed on killing a beast
    if (res == FIGHTRES_KILL_DONE
        && (e->beast || ((rand() % 15) - (target->luck / 10) <= 0))) {
        dropEquip(target, e->beast, notify_win, kls);
    }
    return res;
}

/**
 * Takes a Fighter and a Enemy pointers and calls fight().
 * @see Fighter
 * @see Enemy
 * @see fight()
 * @param player The Fighter pointer at hand.
 * @param e The Enemy pointer at hand.
 * @param foe_op The foeTurnOption_OP for the foe.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 */
int defer_fight_enemy(Fighter *player, Enemy *e, foeTurnOption_OP foe_op,
                      WINDOW *notify_win, Koliseo *kls)
{
    char msg[200];
    //FIXME
    //Is it okay to return just one result, when having 2 interactions that could go differently?
    //
    //Use FIGHTRES_CLASH as needed, to indicate both sides were damaged at some point.
    fightResult res = FIGHTRES_NO_DMG;

    int player_goes_first = (player->vel >= e->vel ? 1 : 0);

    int first_act_res = FIGHTRES_NO_DMG;

    if (player_goes_first) {

        res = fight(player, e, notify_win, kls);

        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);
        first_act_res = res;

        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            switch (foe_op) {
            case FOE_OP_INVALID: {
                log_tag("debug_log.txt", "[ERROR]",
                        "foe_op was FOE_OP_INVALID in [%s]: [%i]", __func__,
                        foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            case FOE_OP_IDLE: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } was idle.", __func__,
                        stringFromEClass(e->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is loafing around.",
                        stringFromEClass(e->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));
            }
            break;
            case FOE_OP_FIGHT: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } wants to fight.", __func__,
                        stringFromEClass(e->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is angry!", stringFromEClass(e->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));

                res = enemy_attack(e, player, notify_win, kls);
            }
            break;
            case FOE_OP_SPECIAL: {
                log_tag("debug_log.txt", "[TODO]",
                        "[%s()]:  Foe { %s } wants to use a special.",
                        __func__, stringFromEClass(e->class));
                //TODO
                //Implement enemy special function
                //res = enemy_attack_special(e,player,notify_win,kls);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected foeTurnOption_OP in [%s()]: [%i]",
                        __func__, foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            }			// End foe_op switch

            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);
            if (res == FIGHTRES_DEATH || res == FIGHTRES_KILL_DONE) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was not a clash...", __func__);
            } else if ((res == FIGHTRES_DMG_TAKEN
                        && first_act_res == FIGHTRES_DMG_DEALT)
                       || (res == FIGHTRES_DMG_DEALT
                           && first_act_res == FIGHTRES_DMG_TAKEN)) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was a clash!", __func__);
                res = FIGHTRES_CLASH;
            }

            return res;
        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    } else {
        //Foe acts first
        switch (foe_op) {
        case FOE_OP_INVALID: {
            log_tag("debug_log.txt", "[ERROR]",
                    "foe_op was FOE_OP_INVALID in [%s]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        case FOE_OP_IDLE: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } was idle.", __func__,
                    stringFromEClass(e->class));
            wattron(notify_win, COLOR_PAIR(S4C_GREY));
            sprintf(msg, "%s is loafing around.",
                    stringFromEClass(e->class));
            display_notification(notify_win, msg, 500);
            wattroff(notify_win, COLOR_PAIR(S4C_GREY));
        }
        break;
        case FOE_OP_FIGHT: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } wants to fight.", __func__,
                    stringFromEClass(e->class));
            res = enemy_attack(e, player, notify_win, kls);
        }
        break;
        case FOE_OP_SPECIAL: {
            log_tag("debug_log.txt", "[TODO]",
                    "[%s()]:  Foe { %s } wants to use a special.", __func__,
                    stringFromEClass(e->class));
            //TODO
            //Implement enemy special function
            //res = enemy_attack_special(e,player,notify_win,kls);
        }
        break;
        default: {
            log_tag("debug_log.txt", "[ERROR]",
                    "Unexpected foeTurnOption_OP in [%s()]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        }			// End foe_op switch

        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);
        first_act_res = res;

        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            res = fight(player, e, notify_win, kls);

            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);
            if (res == FIGHTRES_DEATH || res == FIGHTRES_KILL_DONE) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was not a clash...", __func__);
            } else if ((res == FIGHTRES_DMG_TAKEN
                        && first_act_res == FIGHTRES_DMG_DEALT)
                       || (res == FIGHTRES_DMG_DEALT
                           && first_act_res == FIGHTRES_DMG_TAKEN)) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was a clash!", __func__);
                res = FIGHTRES_CLASH;
            }

            return res;
        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    }
    return res;
}

/**
 * Takes a Fighter and a Enemy pointers and calls do_Skill().
 * @see Fighter
 * @see Enemy
 * @see do_Skill()
 * @param player The Fighter pointer at hand.
 * @param e The Enemy pointer at hand.
 * @param picked_skill The picked skill by Fighter.
 * @param foe_op The foeTurnOption_OP for the foe.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 */
int defer_skill_enemy(Fighter *player, Enemy *e, skillType picked_skill, foeTurnOption_OP foe_op,
                      WINDOW *notify_win, Koliseo *kls)
{
    char msg[200];
    //FIXME
    //Is it okay to return just one result, when having 2 interactions that could go differently?
    //
    //Use FIGHTRES_CLASH as needed, to indicate both sides were damaged at some point.
    fightResult res = FIGHTRES_NO_DMG;

    int player_goes_first = (player->vel >= e->vel ? 1 : 0);

    int first_act_res = FIGHTRES_NO_DMG;

    if (player_goes_first) {

        res = do_Skill(player, e, picked_skill, notify_win, kls);

        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);
        first_act_res = res;

        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            switch (foe_op) {
            case FOE_OP_INVALID: {
                log_tag("debug_log.txt", "[ERROR]",
                        "foe_op was FOE_OP_INVALID in [%s]: [%i]", __func__,
                        foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            case FOE_OP_IDLE: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } was idle.", __func__,
                        stringFromEClass(e->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is loafing around.",
                        stringFromEClass(e->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));
            }
            break;
            case FOE_OP_FIGHT: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } wants to fight.", __func__,
                        stringFromEClass(e->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is angry!", stringFromEClass(e->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));

                res = enemy_attack(e, player, notify_win, kls);
            }
            break;
            case FOE_OP_SPECIAL: {
                log_tag("debug_log.txt", "[TODO]",
                        "[%s()]:  Foe { %s } wants to use a special.",
                        __func__, stringFromEClass(e->class));
                //TODO
                //Implement enemy special function
                //res = enemy_attack_special(e,player,notify_win,kls);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected foeTurnOption_OP in [%s()]: [%i]",
                        __func__, foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            }			// End foe_op switch

            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);
            if (res == FIGHTRES_DEATH || res == FIGHTRES_KILL_DONE) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was not a clash...", __func__);
            } else if ((res == FIGHTRES_DMG_TAKEN
                        && first_act_res == FIGHTRES_DMG_DEALT)
                       || (res == FIGHTRES_DMG_DEALT
                           && first_act_res == FIGHTRES_DMG_TAKEN)) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was a clash!", __func__);
                res = FIGHTRES_CLASH;
            }

            return res;
        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    } else {
        //Foe acts first
        switch (foe_op) {
        case FOE_OP_INVALID: {
            log_tag("debug_log.txt", "[ERROR]",
                    "foe_op was FOE_OP_INVALID in [%s]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        case FOE_OP_IDLE: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } was idle.", __func__,
                    stringFromEClass(e->class));
            wattron(notify_win, COLOR_PAIR(S4C_GREY));
            sprintf(msg, "%s is loafing around.",
                    stringFromEClass(e->class));
            display_notification(notify_win, msg, 500);
            wattroff(notify_win, COLOR_PAIR(S4C_GREY));
        }
        break;
        case FOE_OP_FIGHT: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } wants to fight.", __func__,
                    stringFromEClass(e->class));
            res = enemy_attack(e, player, notify_win, kls);
        }
        break;
        case FOE_OP_SPECIAL: {
            log_tag("debug_log.txt", "[TODO]",
                    "[%s()]:  Foe { %s } wants to use a special.", __func__,
                    stringFromEClass(e->class));
            //TODO
            //Implement enemy special function
            //res = enemy_attack_special(e,player,notify_win,kls);
        }
        break;
        default: {
            log_tag("debug_log.txt", "[ERROR]",
                    "Unexpected foeTurnOption_OP in [%s()]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        }			// End foe_op switch

        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);
        first_act_res = res;

        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            res = do_Skill(player, e, picked_skill, notify_win, kls);

            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);
            if (res == FIGHTRES_DEATH || res == FIGHTRES_KILL_DONE) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was not a clash...", __func__);
            } else if ((res == FIGHTRES_DMG_TAKEN
                        && first_act_res == FIGHTRES_DMG_DEALT)
                       || (res == FIGHTRES_DMG_DEALT
                           && first_act_res == FIGHTRES_DMG_TAKEN)) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "[%s()]: Deferred fight was a clash!", __func__);
                res = FIGHTRES_CLASH;
            }

            return res;
        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
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
int boss_fight(Fighter *player, Boss *b, Path *p, WINDOW *notify_win,
               Koliseo *kls)
{

    fightResult res = FIGHTRES_NO_DMG;
    //Stat comparisons
    //
    char msg[200];

    int atkdelta = (player->atk + player->equipboost_atk) - b->atk - (rand() % 3) - 1;	//Skewed with defender
    int defdelta = (player->def + player->equipboost_def) - b->def + (rand() % 2) + 1;	//Skewed with attacker
    int veldelta =
        (player->vel + player->equipboost_vel) - b->vel + (rand() % 3) + 1;

    int atkOnPlayer =
        b->atk - (player->def + player->equipboost_def + (player->vel / 6));
    int atkOnEnemy =
        (player->atk + player->equipboost_atk) - (b->def + (b->vel / 6));
    if (G_GODMODE_ON == 1) {
        log_tag("debug_log.txt", "[DEBUG]", "boss_fight():  G_GODMODE_ON == 1");
        atkdelta = 100;
        defdelta = 100;
        veldelta = 100;
        atkOnPlayer = 1;
        atkOnEnemy = 100;
    }
    sprintf(msg, "atkdelta %i", atkdelta);
    log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);
    sprintf(msg, "defdelta %i", defdelta);
    log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);
    sprintf(msg, "veldelta %i", veldelta);
    log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);
    sprintf(msg, "atkOnEnemy %i", atkOnEnemy);
    log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);
    sprintf(msg, "atkOnPlayer %i\n", atkOnPlayer);
    log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);

    int damageDealt = -1;

    if (veldelta >= 0) {

        if (atkOnEnemy > 3) {
            damageDealt = atkOnEnemy;
            b->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result A WIN (faster, great atk).\n");
        } else if (atkOnEnemy >= 0) {
            damageDealt = abs(atkOnEnemy - atkdelta);
            b->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result B WIN (faster, ok atk).\n");
        } else {
            if (atkOnEnemy < -3) {
                damageDealt =
                    fabsf(atkOnPlayer -
                          0.75F * (player->vel + player->equipboost_vel));
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result C1 LOST (faster, atk < -3).\n");
            } else {
                damageDealt = abs(atkOnPlayer - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result C2 LOST (faster, atk > -3).\n");
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
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result D LOST (slower, great enemy atk.\n");
        } else if (atkOnPlayer >= 0) {
            damageDealt = abs(atkOnPlayer - atkdelta);
            player->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result E LOST (slower, ok enemy atk.\n");
        } else {
            if (atkOnPlayer < -3) {
                damageDealt = fabsf(atkOnEnemy - 0.75F * b->vel);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result F1 WIN (slower, enemy atk < -3.\n");
            } else {
                damageDealt = abs(atkOnEnemy - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result F2 WIN (slower, enemy atk > -3.\n");
            }
            b->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
        }
    }

    log_tag("debug_log.txt", "[FIGHT]", "damageCalc %i", damageDealt);

    int yourhit = (res == FIGHTRES_DMG_DEALT) ? 1 : 0;
    char victim[25];

    if (!yourhit) {

        b->vel--;
        b->atk--;
        b->def -= 2;

        //Check if someone earned a stat reset after the fight
        statReset(player, 0);
        statResetBoss(b, 0);

        strcpy(victim, player->name);
    } else {

        player->vel--;
        player->atk--;
        player->def -= 2;

        //Account for vampirism perk
        int vampire_perks = player->perks[VAMPIRISM]->innerValue;
        if (vampire_perks > 0) {
            int recovery = floor(damageDealt * (0.1 * vampire_perks));
            player->hp += recovery;
            sprintf(msg, "Vampirism proc for +%i HP.\n", recovery);
            log_tag("debug_log.txt", "[PERKS]", msg);
            if (player->hp >= player->totalhp) {
                player->hp = player->totalhp;
            };
        }
        //Account for burn on touch perk
        int hotbody_perks = player->perks[HOT_BODY]->innerValue;
        if (hotbody_perks > 0) {
            int burnchance = 11 - hotbody_perks;
            if (rand() % burnchance == 0) {
                b->status = Burned;	//Set status to Burned. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) b->counters[Burned], 2);	//Give 2 turns of Burned status
                sprintf(msg, "Hotbody proc on 1/%i chance.\n", burnchance);
                log_tag("debug_log.txt", "[PERKS]", msg);
            }
        }
        //Account for poison on touch perk. Order of checks with hot_body perk may cause issues?
        int biohazard_perks = player->perks[BIOHAZARD]->innerValue;
        if (biohazard_perks > 0) {
            int poisonchance = 11 - biohazard_perks;
            if (rand() % poisonchance == 0) {
                b->status = Poison;	//Set status to Poison. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) b->counters[POISON], 2);	//Give 2 turns of Poison status
                sprintf(msg, "Biohazard proc on 1/%i chance.\n", poisonchance);
                log_tag("debug_log.txt", "[PERKS]", msg);
            }
        }
        //Check if someone earned a stat reset after the fight
        statResetBoss(b, 0);
        statReset(player, 0);

        strcpy(victim, stringFromBossClass(b->class));
    }

    int color = -1;
    if (yourhit) {
        color = S4C_WHITE;
    } else {
        color = S4C_RED;
    }

    wattron(notify_win, COLOR_PAIR(color));
    sprintf(msg, "%s was hit.    (%i DMG)", victim,
            damageDealt > 0 ? damageDealt : 1);
    display_notification(notify_win, msg, 500);
    wattroff(notify_win, COLOR_PAIR(color));

    //Rolls
    //
    //Critical hit roll

    //Account for critboost_chance perks
    int critMax =
        round(10.0 - floor(player->luck / 5) -
              (1.5 * player->perks[CRITBOOST_CHANCE]->innerValue));

    int critRes = (rand() % critMax);

    if (res == FIGHTRES_DMG_DEALT && (critRes <= 0)) {

        //Account for critboost_dmg perks
        int dmgboost_perks = player->perks[CRITBOOST_DMG]->innerValue;
        damageDealt *= (0.30 + (0.12 * dmgboost_perks));
        b->hp -= damageDealt > 0 ? damageDealt : 1;
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        sprintf(msg, "A critical hit!    (%i DMG)",
                damageDealt > 0 ? damageDealt : 1);
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        sprintf(msg, "Critical hit for %i dmg, proc on 1/%i chance.\n",
                damageDealt, critMax);
        log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);

        //Update stats
        player->stats->criticalhits++;
    }
    //Check for deaths -> exit condition from loop
    //
    //
    //
    if (b->hp <= 0) {
        res = FIGHTRES_KILL_DONE;

        //Account for runic circle perk
        int runic_perks = player->perks[RUNIC_MAGNET]->innerValue;
        if (runic_perks > 0) {
            int recovery = round(0.51 * runic_perks);
            player->energy += recovery;
            sprintf(msg, "Runicmagnet proc for %i energy.\n", recovery);
            log_tag("debug_log.txt", "[PERKS]", msg);
        }
        if (b->beast) {
            color = S4C_MAGENTA;
        } else {
            color = S4C_RED;
        }
        wattron(notify_win, COLOR_PAIR(color));
        sprintf(msg, "%s fainted.", stringFromBossClass(b->class));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(color));
        sprintf(msg, "Killed  %s.", stringFromBossClass(b->class));
        log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);

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
            sprintf(msg, "Killed new boss  %s.\n",
                    stringFromBossClass(b->class));
            log_tag("debug_log.txt", "[FIGHT-BOSS]", msg);
        }
    } else {
        //Apply status effects to boss
        if (b->status != Normal) {
            applyBStatus(notify_win, b);
            sprintf(msg, "Applied  %s to %s.", stringFromStatus(b->status),
                    stringFromBossClass(b->class));
            log_tag("debug_log.txt", "[STATUS]", msg);
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
        wattron(notify_win, COLOR_PAIR(S4C_CYAN));
        sprintf(msg, "You found a %s!", stringFromConsumables(drop));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_CYAN));
        sprintf(msg, "Found Consumable:    %s.", stringFromConsumables(drop));
        log_tag("debug_log.txt", "[DROPS]", msg);
    }

    //Artifact drop (if we don't have all of them)
    if (res == FIGHTRES_KILL_DONE
        && (player->stats->artifactsfound != ARTIFACTSMAX + 1)) {
        int artifact_drop = dropArtifact(player);
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        sprintf(msg, "You found a %s!", stringFromArtifacts(artifact_drop));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        sprintf(msg, "Found Artifact:    %s.",
                stringFromArtifacts(artifact_drop));
        log_tag("debug_log.txt", "[DROPS]", msg);
    }
    //Equip drop
    if (res == FIGHTRES_KILL_DONE) {
        //We give 1 to obtain the better equip generation used for beasts
        dropEquip(player, 1, notify_win, kls);
    }

    return res;
}

int boss_attack(Boss *b, Fighter *target, Path *p, WINDOW *notify_win,
                Koliseo *kls)
{

    //TODO
    //Implementation similar to boss_fight(), as a base idea
    //Should return fightResult values, while keeping the perspective on the Fighter, as in:
    //
    //      FIGHTRES_DEATH means the Fighter died
    //      FIGHTRES_KILL_DONE means the Boss died
    //      FIGHTRES_DMG_DEALT means the Fighter inflicted damage
    //      FIGHRES_DMG_TAKEN means the Fighter received damage
    fightResult res = FIGHTRES_NO_DMG;
    //Stat comparisons
    //
    char msg[200];

    int atkdelta = (b->atk + b->turnboost_atk) - (target->atk + target->equipboost_atk + target->turnboost_atk - (rand() % 3)) - 1;	//Skewed with defender
    int defdelta = (b->def + b->turnboost_def) - (target->def + target->equipboost_def + target->turnboost_def) + (rand() % 2) + 1;	//Skewed with attacker
    int veldelta =
        (b->vel + b->turnboost_vel) - (target->vel + target->equipboost_vel +
                                       target->turnboost_vel) + (rand() % 3) +
        1;

    int atkOnPlayer =
        (b->atk + b->turnboost_atk) - (target->def + target->equipboost_def +
                                       target->turnboost_def +
                                       (target->vel / 6));
    int atkOnEnemy =
        (target->atk + target->equipboost_atk + target->turnboost_atk) -
        (b->def + b->turnboost_def + (b->vel / 6));
    if (G_GODMODE_ON == 1) {
        log_tag("debug_log.txt", "[DEBUG]", "[%s]:  G_GODMODE_ON == 1",
                __func__);
        atkdelta = -100;
        defdelta = -100;
        veldelta = -100;
        atkOnPlayer = 1;
        atkOnEnemy = 100;
    }
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkdelta %i", atkdelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "defdelta %i", defdelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "veldelta %i", veldelta);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkOnEnemy %i", atkOnEnemy);
    log_tag("debug_log.txt", "[DEBUG-FIGHT]", "atkOnPlayer %i", atkOnPlayer);

    int damageDealt = -1;

    if (veldelta >= 0) {	//Boss was faster
        if (atkOnPlayer > 3) {
            damageDealt = atkOnPlayer;
            target->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result D LOST (slower, great enemy atk.");
        } else if (atkOnPlayer >= 0) {
            damageDealt = abs(atkOnPlayer - atkdelta);
            target->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result E LOST (slower, ok enemy atk.");
        } else {
            if (atkOnPlayer < -3) {
                damageDealt = fabsf(atkOnEnemy - 0.75F * b->vel);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result F1 WIN (slower, enemy atk < -3.");
            } else {
                damageDealt = abs(atkOnEnemy - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result F2 WIN (slower, enemy atk > -3.");
            }
            b->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
        }
    } else {			//Target was faster
        atkdelta = -atkdelta;
        if (atkOnEnemy > 3) {
            damageDealt = atkOnEnemy;
            b->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result A WIN (faster, great atk).");
        } else if (atkOnEnemy >= 0) {
            damageDealt = abs(atkOnEnemy - atkdelta);
            b->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_DEALT;
            log_tag("debug_log.txt", "[FIGHT]",
                    "Fight result B WIN (faster, ok atk).");
        } else {
            if (atkOnEnemy < -3) {
                damageDealt =
                    fabsf(atkOnPlayer -
                          0.75F * (target->vel + target->equipboost_vel));
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result C1 LOST (faster, atk < -3).");
            } else {
                damageDealt = abs(atkOnPlayer - 1);
                log_tag("debug_log.txt", "[FIGHT]",
                        "Fight result C2 LOST (faster, atk > -3).");
            }
            target->hp -= damageDealt > 0 ? damageDealt : 1;
            res = FIGHTRES_DMG_TAKEN;
        }
    }

    log_tag("debug_log.txt", "[FIGHT]", "damageCalc %i", damageDealt);

    int yourhit = (res == FIGHTRES_DMG_DEALT) ? 1 : 0;
    char victim[25];

    if (!yourhit) {

        b->vel--;
        b->atk--;
        b->def -= 2;

        //Check if someone earned a stat reset after the fight
        statReset(target, 0);
        statResetBoss(b, 0);

        strcpy(victim, target->name);
    } else {

        target->vel--;
        target->atk--;
        target->def -= 2;

        //Account for vampirism perk
        int vampire_perks = target->perks[VAMPIRISM]->innerValue;
        if (vampire_perks > 0) {
            int recovery = floor(damageDealt * (0.1 * vampire_perks));
            target->hp += recovery;
            log_tag("debug_log.txt", "[PERKS]", "Vampirism proc for +%i HP.",
                    recovery);
            if (target->hp >= target->totalhp) {
                target->hp = target->totalhp;
            };
        }
        //Account for burn on touch perk
        int hotbody_perks = target->perks[HOT_BODY]->innerValue;
        if (hotbody_perks > 0) {
            int burnchance = 11 - hotbody_perks;
            if (rand() % burnchance == 0) {
                b->status = Burned;	//Set status to Burned. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) b->counters[Burned], 2);	//Give 2 turns of Burned status
                log_tag("debug_log.txt", "[PERKS]",
                        "Hotbody proc on 1/%i chance.", burnchance);
            }
        }
        //Account for poison on touch perk. Order of checks with hot_body perk may cause issues?
        int biohazard_perks = target->perks[BIOHAZARD]->innerValue;
        if (biohazard_perks > 0) {
            int poisonchance = 11 - biohazard_perks;
            if (rand() % poisonchance == 0) {
                b->status = Poison;	//Set status to Poison. May need change to manage multiple statuses active at once
                setCounter((Turncounter *) b->counters[POISON], 2);	//Give 2 turns of Poison status
                log_tag("debug_log.txt", "[PERKS]",
                        "Biohazard proc on 1/%i chance.", poisonchance);
            }
        }
        //Check if someone earned a stat reset after the fight
        statResetBoss(b, 0);
        statReset(target, 0);

        strcpy(victim, stringFromBossClass(b->class));
    }

    int color = -1;
    if (yourhit) {
        color = S4C_WHITE;
    } else {
        color = S4C_RED;
    }

    wattron(notify_win, COLOR_PAIR(color));
    sprintf(msg, "%s was hit.    (%i DMG)", victim,
            damageDealt > 0 ? damageDealt : 1);
    display_notification(notify_win, msg, 500);
    wattroff(notify_win, COLOR_PAIR(color));

    //Rolls
    //
    //Critical hit roll

    //Account for critboost_chance perks
    int critMax =
        round(10.0 - floor(target->luck / 5) -
              (1.5 * target->perks[CRITBOOST_CHANCE]->innerValue));

    int critRes = (rand() % critMax);

    if (res == FIGHTRES_DMG_DEALT && (critRes <= 0)) {

        //Account for critboost_dmg perks
        int dmgboost_perks = target->perks[CRITBOOST_DMG]->innerValue;
        damageDealt *= (0.30 + (0.12 * dmgboost_perks));
        b->hp -= damageDealt > 0 ? damageDealt : 1;
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        sprintf(msg, "A critical hit!    (%i DMG)",
                damageDealt > 0 ? damageDealt : 1);
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        log_tag("debug_log.txt", "[FIGHT-BOSS]",
                "Critical hit for %i dmg, proc on 1/%i chance.", damageDealt,
                critMax);

        //Update stats
        target->stats->criticalhits++;
    }
    //Check for deaths -> exit condition from loop
    //
    //
    //
    if (b->hp <= 0) {
        res = FIGHTRES_KILL_DONE;

        //Account for runic circle perk
        int runic_perks = target->perks[RUNIC_MAGNET]->innerValue;
        if (runic_perks > 0) {
            int recovery = round(0.51 * runic_perks);
            target->energy += recovery;
            log_tag("debug_log.txt", "[PERKS]",
                    "Runicmagnet proc for %i energy.", recovery);
        }
        if (b->beast) {
            color = S4C_MAGENTA;
        } else {
            color = S4C_RED;
        }
        wattron(notify_win, COLOR_PAIR(color));
        sprintf(msg, "%s fainted.", stringFromBossClass(b->class));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(color));
        log_tag("debug_log.txt", "[FIGHT-BOSS]", "Killed  %s.",
                stringFromBossClass(b->class));

        //Update stats
        target->stats->bosseskilled++;

        //Check if we killed this boss before
        if (target->stats->killed_bosses[b->class] == 0) {
            //Update stats
            target->stats->unique_bosseskilled++;
            target->stats->killed_bosses[b->class] += 1;
            //Check if we have to update the wincon value
            if (p->win_condition->class == ALL_BOSSES) {
                p->win_condition->current_val++;
            }
            log_tag("debug_log.txt", "[FIGHT-BOSS]", "Killed new boss  %s.",
                    stringFromBossClass(b->class));
        }
    } else {
        //Apply status effects to boss
        if (b->status != Normal) {
            applyBStatus(notify_win, b);
            log_tag("debug_log.txt", "[STATUS]", "Applied  %s to %s.",
                    stringFromStatus(b->status), stringFromBossClass(b->class));
        }
    }

    if (target->hp <= 0) {
        res = FIGHTRES_DEATH;
    } else {
        //Apply status effects to target
        if (target->status != Normal) {
            applyStatus(notify_win, target);
        }
    }

    //Consumable drop, guaranteed on killing a beast
    if (res == FIGHTRES_KILL_DONE) {
        int drop = dropConsumable(target);
        wattron(notify_win, COLOR_PAIR(S4C_CYAN));
        sprintf(msg, "You found a %s!", stringFromConsumables(drop));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_CYAN));
        log_tag("debug_log.txt", "[DROPS]", "Found Consumable:    %s.",
                stringFromConsumables(drop));
    }

    //Artifact drop (if we don't have all of them)
    if (res == FIGHTRES_KILL_DONE
        && (target->stats->artifactsfound != ARTIFACTSMAX + 1)) {
        int artifact_drop = dropArtifact(target);
        wattron(notify_win, COLOR_PAIR(S4C_MAGENTA));
        sprintf(msg, "You found a %s!", stringFromArtifacts(artifact_drop));
        display_notification(notify_win, msg, 500);
        wattroff(notify_win, COLOR_PAIR(S4C_MAGENTA));
        log_tag("debug_log.txt", "[DROPS]", "Found Artifact:    %s.",
                stringFromArtifacts(artifact_drop));
    }
    //Equip drop
    if (res == FIGHTRES_KILL_DONE) {
        //We give 1 to obtain the better equip generation used for beasts
        dropEquip(target, 1, notify_win, kls);
    }

    return res;
}

/**
 * Takes a Fighter and a Boss pointers and calls boss_fight().
 * @see Fighter
 * @see Boss
 * @see boss_fight()
 * @param player The Fighter pointer at hand.
 * @param b The Boss pointer at hand.
 * @param foe_op The foeTurnOption_OP for the foe.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 */
int defer_fight_boss(Fighter *player, Boss *b, Path *p, foeTurnOption_OP foe_op,
                     WINDOW *notify_win, Koliseo *kls)
{
    char msg[200];
    //FIXME
    //Is it okay to return just one result, when having 2 interactions that could go differently?
    //
    //Use FIGHTRES_CLASH as needed, to indicate both sides were damaged at some point.
    fightResult res = FIGHTRES_NO_DMG;

    int player_goes_first = (player->vel >= b->vel ? 1 : 0);

    if (player_goes_first) {
        res = boss_fight(player, b, p, notify_win, kls);
        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);
        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            switch (foe_op) {
            case FOE_OP_INVALID: {
                log_tag("debug_log.txt", "[ERROR]",
                        "foe_op was FOE_OP_INVALID in [%s]: [%i]", __func__,
                        foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            case FOE_OP_IDLE: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } was idle.", __func__,
                        stringFromBossClass(b->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is loafing around.",
                        stringFromBossClass(b->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));
            }
            break;
            case FOE_OP_FIGHT: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } wants to fight.", __func__,
                        stringFromBossClass(b->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is angry!", stringFromBossClass(b->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));
                res = boss_attack(b, player, p, notify_win, kls);
            }
            break;
            case FOE_OP_SPECIAL: {
                log_tag("debug_log.txt", "[TODO]",
                        "[%s()]:  Foe { %s } wants to use a special.",
                        __func__, stringFromBossClass(b->class));
                //TODO
                //Implement boss special function
                //res = boss_attack_special(b,player,p,notify_win,kls);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected foeTurnOption_OP in [%s()]: [%i]",
                        __func__, foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            }			// End foe_op switch

            //TODO
            //Check second turn act res?
            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);

            return res;
        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    } else {
        //Foe acts first
        switch (foe_op) {
        case FOE_OP_INVALID: {
            log_tag("debug_log.txt", "[ERROR]",
                    "foe_op was FOE_OP_INVALID in [%s()]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        case FOE_OP_IDLE: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } was idle.", __func__,
                    stringFromBossClass(b->class));
            wattron(notify_win, COLOR_PAIR(S4C_GREY));
            sprintf(msg, "%s is loafing around.",
                    stringFromBossClass(b->class));
            display_notification(notify_win, msg, 500);
            wattroff(notify_win, COLOR_PAIR(S4C_GREY));
        }
        break;
        case FOE_OP_FIGHT: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } wants to fight.", __func__,
                    stringFromBossClass(b->class));
            wattron(notify_win, COLOR_PAIR(S4C_GREY));
            sprintf(msg, "%s is angry!", stringFromBossClass(b->class));
            display_notification(notify_win, msg, 500);
            wattroff(notify_win, COLOR_PAIR(S4C_GREY));
            res = boss_attack(b, player, p, notify_win, kls);
        }
        break;
        case FOE_OP_SPECIAL: {
            log_tag("debug_log.txt", "[TODO]",
                    "[%s()]:  Foe { %s } wants to use a special.", __func__,
                    stringFromBossClass(b->class));
            //TODO
            //Implement boss special function
            //res = boss_attack_special(b,player,p,notify_win,kls);
        }
        break;
        default: {
            log_tag("debug_log.txt", "[ERROR]",
                    "Unexpected foeTurnOption_OP in [%s()]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        }			// End foe_op switch

        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);

        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            res = boss_fight(player, b, p, notify_win, kls);

            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);
            //TODO
            //Check second turn act res?
            return res;

        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    }

    return res;
}

/**
 * Takes a Fighter and a Boss pointers and calls do_Skill_boss().
 * @see Fighter
 * @see Boss
 * @see do_Skill_boss()
 * @param player The Fighter pointer at hand.
 * @param b The Boss pointer at hand.
 * @param picked_skill The skill picked by Fighter.
 * @param foe_op The foeTurnOption_OP for the foe.
 * @param notify_win The WINDOW pointer to call display_notification() on.
 * @param kls The Koliseo used for allocations.
 */
int defer_skill_boss(Fighter *player, Boss *b, skillType picked_skill, Path *p, foeTurnOption_OP foe_op,
                     WINDOW *notify_win, Koliseo *kls)
{
    char msg[200];
    //FIXME
    //Is it okay to return just one result, when having 2 interactions that could go differently?
    //
    //Use FIGHTRES_CLASH as needed, to indicate both sides were damaged at some point.
    fightResult res = FIGHTRES_NO_DMG;

    int player_goes_first = (player->vel >= b->vel ? 1 : 0);

    if (player_goes_first) {
        res = do_Skill_boss(player, b, picked_skill, p, notify_win, kls);
        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);
        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            switch (foe_op) {
            case FOE_OP_INVALID: {
                log_tag("debug_log.txt", "[ERROR]",
                        "foe_op was FOE_OP_INVALID in [%s]: [%i]", __func__,
                        foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            case FOE_OP_IDLE: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } was idle.", __func__,
                        stringFromBossClass(b->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is loafing around.",
                        stringFromBossClass(b->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));
            }
            break;
            case FOE_OP_FIGHT: {
                log_tag("debug_log.txt", "[DEFER]",
                        "[%s()]:  Foe { %s } wants to fight.", __func__,
                        stringFromBossClass(b->class));
                wattron(notify_win, COLOR_PAIR(S4C_GREY));
                sprintf(msg, "%s is angry!", stringFromBossClass(b->class));
                display_notification(notify_win, msg, 500);
                wattroff(notify_win, COLOR_PAIR(S4C_GREY));
                res = boss_attack(b, player, p, notify_win, kls);
            }
            break;
            case FOE_OP_SPECIAL: {
                log_tag("debug_log.txt", "[TODO]",
                        "[%s()]:  Foe { %s } wants to use a special.",
                        __func__, stringFromBossClass(b->class));
                //TODO
                //Implement boss special function
                //res = boss_attack_special(b,player,p,notify_win,kls);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected foeTurnOption_OP in [%s()]: [%i]",
                        __func__, foe_op);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            }			// End foe_op switch

            //TODO
            //Check second turn act res?
            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);

            return res;
        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
    } else {
        //Foe acts first
        switch (foe_op) {
        case FOE_OP_INVALID: {
            log_tag("debug_log.txt", "[ERROR]",
                    "foe_op was FOE_OP_INVALID in [%s()]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        case FOE_OP_IDLE: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } was idle.", __func__,
                    stringFromBossClass(b->class));
            wattron(notify_win, COLOR_PAIR(S4C_GREY));
            sprintf(msg, "%s is loafing around.",
                    stringFromBossClass(b->class));
            display_notification(notify_win, msg, 500);
            wattroff(notify_win, COLOR_PAIR(S4C_GREY));
        }
        break;
        case FOE_OP_FIGHT: {
            log_tag("debug_log.txt", "[DEFER]",
                    "[%s()]:  Foe { %s } wants to fight.", __func__,
                    stringFromBossClass(b->class));
            wattron(notify_win, COLOR_PAIR(S4C_GREY));
            sprintf(msg, "%s is angry!", stringFromBossClass(b->class));
            display_notification(notify_win, msg, 500);
            wattroff(notify_win, COLOR_PAIR(S4C_GREY));
            res = boss_attack(b, player, p, notify_win, kls);
        }
        break;
        case FOE_OP_SPECIAL: {
            log_tag("debug_log.txt", "[TODO]",
                    "[%s()]:  Foe { %s } wants to use a special.", __func__,
                    stringFromBossClass(b->class));
            //TODO
            //Implement boss special function
            //res = boss_attack_special(b,player,p,notify_win,kls);
        }
        break;
        default: {
            log_tag("debug_log.txt", "[ERROR]",
                    "Unexpected foeTurnOption_OP in [%s()]: [%i]", __func__,
                    foe_op);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        }			// End foe_op switch

        //Check res and apply second action if needed
        log_tag("debug_log.txt", "[DEBUG]",
                "[%s()]: First act res was [%s]: [%i]", __func__,
                stringFrom_fightResult(res), res);

        if (res != FIGHTRES_DEATH && res != FIGHTRES_KILL_DONE) {
            res = do_Skill_boss(player, b, picked_skill, p, notify_win, kls);

            log_tag("debug_log.txt", "[DEBUG]",
                    "[%s()]: Second act res was [%s]: [%i]", __func__,
                    stringFrom_fightResult(res), res);
            //TODO
            //Check second turn act res?
            return res;

        } else if (res == FIGHTRES_DEATH) {
            return res;
        } else if (res == FIGHTRES_KILL_DONE) {
            return res;
        }
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
void useConsumable(Fighter *f, Enemy *e, Boss *b, char *string, int isBoss)
{
    //Mountain of ifs? Where are my damn maps

    int i = 0;
    int check;
    for (int j = 0; j < CONSUMABLESMAX + 1; j++) {
        if ((check = strcmp(consumablestrings[j], string)) == 0) {
            i = j;
        }
    };

    Consumable *c = (Consumable *) f->consumablesBag[i];

    if (c->qty == 0) {
        return;
    }
    if (c->class == Potion) {
        f->hp = f->totalhp;
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
        f->vel -= 1;
        f->def += 6;
        f->atk += 4;
        //printf("\n\n\n\tYou ate elvish bread.");
        //green();
        //printf("\t+6 DEF , +4 ATK, ");
        //yellow();
        //printf("-1 VEL\n\n");
        //white();
    } else if (c->class == Coffee) {
        f->vel += 6;
        f->atk += 2;
        f->def -= 2;
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
        checkremainder(f, 25);
    } else if (c->class == Magicbean) {
        //printf("\n\n\n\tYou aren't tired anymore.");
        //green();
        //printf("\tEnergy was restored\n\n");
        //white();
        f->energy = f->totalenergy;	//Refill energy
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
int getConsumableQty(Fighter *f, int n)
{
    Consumable *c = (Consumable *) f->consumablesBag[n];
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
void emptyConsumables(Fighter *f)
{
    for (int i = 0; i < CONSUMABLESMAX + 1; i++) {
        if (getConsumableQty(f, i) > 0) {
            Consumable *c = (Consumable *) f->consumablesBag[i];
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
void emptyArtifacts(Fighter *f)
{
    for (int i = 0; i < ARTIFACTSMAX + 1; i++) {
        if (f->artifactsBag[i]->qty > 0) {
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
void emptyEquips(Fighter *player)
{
    //We REALLY need to be sure the items are in successive cells
    //Still thinking linked lists would be better than an array
    int occupied = player->equipsBagOccupiedSlots;

    for (int i = 0; i < occupied; i++) {

        Equip *e = (Equip *) player->equipsBag[i];
        if (e->qty > 0) {
            //free(e);
            e->qty = 0;
            log_tag("debug_log.txt", "[FIXME]",
                    "emptyEquips():  how do I empty them semantically?");
        }
    };
}

/**
 * Asks the user is they want to continue and returns the choice.
 * @return int True for trying again, false otherwise.
 */
int retry(void)
{
    lightGreen();
    printf("\n\nYou died. Want to try again?\n\n\t\t[type no / yes]\n\n");
    white();
    char c[25] = { 0 };
    if (fgets(c, sizeof(c), stdin) != NULL) {
        log_tag("debug_log.txt", "[DEBUG]", "Read input for %s().", __func__);
        if (c[strlen(c) - 1] == '\n') {
            c[strlen(c) - 1] = '\0';
        }

        for (char *ptr = c; *ptr; ++ptr) {
            *ptr = tolower(*ptr);
        }

        if (c[0] == '\0' || strcmp(c, "no") == 0) {
            log_tag("debug_log.txt", "[DEBUG]", "%s(): input was no.",
                    __func__);
            return 0;
        } else if (strcmp(c, "yes") == 0) {
            log_tag("debug_log.txt", "[DEBUG]", "%s(): input was yes.",
                    __func__);
            return 1;
        } else {
            log_tag("debug_log.txt", "[DEBUG]",
                    "%s(): Invalid input, defaulting to 0.", __func__);

            return 0;
        }
    } else {
        log_tag("debug_log.txt", "[DEBUG]", "Failed reading input for %s.",
                __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
}

/**
 * Takes a Fighter and a Path pointers (and an integer for current room index) and asks user input to execute debug actions.
 * @see Fighter
 * @see Path
 * @see statReset()
 * @see GET_CALLBACK()
 * @see unlock_special()
 * @param gmst The Gamestate pointer.
 * @param player The Fighter pointer at hand.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param The Koliseo to debug.
 * @param The Koliseo_Temp used for allocations.
 */
void debug_generic(Gamestate *gmst, Fighter *player, Path *p, int roomIndex,
                   Koliseo *kls, Koliseo_Temp *t_kls)
{

    char msg[200];
    char ch[25];
    int picked_debug_proc = 0;
#ifndef _WIN32
    struct utsname uts;
    uname(&uts);
    sprintf(msg, "debug_generic() loaded utsname using uname().\n");
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "System is %s\n", uts.sysname);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "OS Release is %s\n", uts.release);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "OS Version is %s\n", uts.version);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "Machine is %s\n", uts.machine);
    log_tag("debug_log.txt", "[DEBUG]", msg);
#endif

    int res = system("clear");
    sprintf(msg, "debug_generic() system(\"clear\") res was (%i)", res);
    log_tag("debug_log.txt", "[DEBUG]", msg);

    int c = 0, n = -1;
    while (!picked_debug_proc) {
        int res = system("clear");
        sprintf(msg, "debug_generic() 2 system(\"clear\") res was (%i)", res);
        log_tag("debug_log.txt", "[DEBUG]", msg);
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
			'Q':  Toggle fast quit\t'k': Log passed kls state to debug log file.\n\
			't': Log global temporary_kls state to debug log file.\n\
			'K': Log usage stats for passed kls to debug log file.\n\
			'T': Log usage stats for temporary_kls to debug log file.\n\
			'G': Log Gamestate to debug log file.\n\
			{Return}  Process your input line.\t'q':  Quit\n\
		]\n\n\
	[%s@debug-func]$ ", player->name);

        char *fgetsres = fgets(ch, sizeof ch, stdin);
        sprintf(msg, "debug_generic() fgets() res was (%s)", fgetsres);
        log_tag("debug_log.txt", "[DEBUG]", msg);
        switch (ch[0]) {
        case '0': {
            picked_debug_proc = 1;
            int xp;
            do {
                printf("\nHow much?\n");
            } while ((c = scanf("%i", &xp)) != 1 || xp < 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_generic() scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            checkremainder(player, xp);
        }
        break;
        case '1': {
            picked_debug_proc = 1;
            int q = -1;
            do {
                printf("\nInsert consumable number:\n");
            } while ((c = scanf("%i", &n)) != 1 || n > CONSUMABLESMAX
                     || n < 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_generic() scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            do {
                printf("\nInsert quantity:\n");
            } while ((c = scanf("%i", &q)) != 1 && q <= 0);
            res = scanf("%*c");
            sprintf(msg, "debug_generic() 2 scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            Consumable *c = (Consumable *) player->consumablesBag[n];
            c->qty += q;
            player->stats->consumablesfound += q;
        }
        break;
        case '2': {
            picked_debug_proc = 1;
            statReset(player, 1);
        }
        break;
        case '3': {
            picked_debug_proc = 1;
            printf("\nCurrent luck: %i\tRL: %i\n", player->luck, p->luck);

            do {
                printf("\nInsert new luck:\n");
            } while ((c = scanf("%i", &n)) != 1 && n > 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_generic() 3 scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            //FIXME:
            //Copy-pasted the calc for player luck... might need a function
            p->luck = n;

            player->luck = (p->luck * MAXPLAYERLUCK) / MAXLUCK;
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
            int n = -1;
            do {
                printf("\nInsert coin number (0<n<100):\n");
            } while ((c = scanf("%i", &n)) < 0 || c > 3 || n < 0
                     || n > 100);
            int res = scanf("%*c");
            sprintf(msg, "debug_generic() 4 scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
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
            int n = -1;
            do {
                printf("\nInsert artifact number (0<n<%i):\n",
                       ARTIFACTSMAX + 1);
            } while ((c = scanf("%i", &n)) != 1 || n > ARTIFACTSMAX
                     || n < 0);

            int res = scanf("%*c");
            sprintf(msg, "debug_generic() scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            player->artifactsBag[n]->qty += 1;
            player->artifactsBag[n]->active = 0;
            player->stats->artifactsfound += 1;
        }
        break;
        case '7': {
            picked_debug_proc = 1;
            int res = system("clear");
            sprintf(msg, "debug_generic() 3 system(\"clear\") res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            printf("\nPlayer Counters:\n");
            printCounters((Turncounter **) player->counters);
            printf("\nPress Enter to resume game");
            res = scanf("%*c");
            sprintf(msg, "debug_generic() 5 scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'd': {
            picked_debug_proc = 1;
            printf("\nVERSION:    %s\n", VERSION);
#ifndef _WIN32
            printf("\nSystem:    %s\n", uts.sysname);
            printf("\nOS Release:    %s\n", uts.release);
            printf("\nOS Version:    %s\n", uts.version);
            printf("\nMachine:    %s\n", uts.machine);
#endif
            printf("\nGAMEMODE:    %s\n", stringFromGamemode(GAMEMODE));
            printf("\nPath->current_saveslot->save_path:    %s\n",
                   p->current_saveslot->save_path);
            printf("\nGS_AUTOSAVE_ON:    %i\n", GS_AUTOSAVE_ON);
            printf("\nG_FASTQUIT_ON:    %i\n", G_FASTQUIT_ON);
            printf("\nG_DEBUG_ON:    %i\n", G_DEBUG_ON);
            printf("\nG_LOG_ON:    %i\n", G_LOG_ON);
            printf("\nG_GODMODE_ON:    %i\n", G_GODMODE_ON);
            printf("\nG_EXPERIMENTAL_ON:    %i\n", G_EXPERIMENTAL_ON);
            printf("\nG_DEBUG_ROOMTYPE_ON:    %i\n", G_DEBUG_ROOMTYPE_ON);
            if (G_DEBUG_ROOMTYPE_ON == 1) {
                printf("\nG_DEBUG_ROOMTYPE:    %i\n", G_DEBUG_ROOMTYPE);
            }
            printf("\nG_DEBUG_ENEMYTYPE_ON:    %i\n", G_DEBUG_ENEMYTYPE_ON);
            if (G_DEBUG_ENEMYTYPE_ON == 1) {
                printf("\nG_DEBUG_ENEMYTYPE:    %i\n", G_DEBUG_ENEMYTYPE);
            }
            printf("\nPress Enter to resume game.\n");
            int res = scanf("%*c");
            sprintf(msg, "debug_generic() 7 scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'F': {
            sprintf(msg, "Trying out Floor functionality.");
            log_tag("debug_log.txt", "[DEBUG]", msg);
            // Declare dbg_floor
            sprintf(msg, "Pushing dbg_floor to tkls.");
            log_tag("debug_log.txt", "[DEBUG]", msg);
            kls_log(kls, "DEBUG", msg);
            Floor *dbg_floor =
                (Floor *) KLS_PUSH_T_TYPED(t_kls, Floor, HR_Floor,
                                           "Floor", msg);
            // Start the random walk from the center of the dungeon
            int center_x = FLOOR_MAX_COLS / 2;
            int center_y = FLOOR_MAX_ROWS / 2;
            // Init dbg_floor
            init_floor_layout(dbg_floor);
            //Set center as filled
            dbg_floor->floor_layout[center_x][center_y] = 1;

            init_floor_rooms(dbg_floor);

            floor_random_walk(dbg_floor, center_x, center_y, 100, 1);	// Perform 100 steps of random walk, reload floor_layout if needed
            load_floor_explored(dbg_floor);

            floor_set_room_types(dbg_floor);

            /*
               debug_print_floor_layout(dbg_floor);

               printf("\nPress Enter to see room layout.\n");
               int res = scanf("%*c");
               sprintf(msg,"debug_generic() 7 scanf() res was (%i)", res);
               log_tag("debug_log.txt","[DEBUG]",msg);
             */
            debug_print_roomclass_layout(dbg_floor, stdout);
            printf("\nPress Enter to return to game.\n");
            res = scanf("%*c");
            sprintf(msg, "debug_generic() 8 scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            //free(dbg_floor);
        }
        break;
        case 'A': {
            GS_AUTOSAVE_ON = (GS_AUTOSAVE_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_AUTOSAVE_ON, new value: (%i)",
                    GS_AUTOSAVE_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'G': {
            log_tag("debug_log.txt", "[DEBUG]", "Logging Gamestate");
            dbg_Gamestate(gmst);
            log_tag("debug_log.txt", "[DEBUG]", "Done logging Gamestate");
        }
        break;
        case 'T': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }

            fprintf(kls_file, "--BEGIN debug of temporary_kls--\n");
            for (int i = HR_Path; i < HLP_MAX_INDEX + 99; i++) {
                ptrdiff_t usage = kls_type_usage(i, temporary_kls);
#ifndef _WIN32
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %li }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#else
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %lli }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#endif
            }
            fprintf(kls_file, "--END debug of temporary_kls--\n\n");

            fclose(kls_file);
        }
        break;
        case 't': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }
            fprintf(kls_file, "--BEGIN debug of temporary_kls--\n");
            print_kls_2file(kls_file, temporary_kls);
            kls_showList_toFile(temporary_kls->regs, kls_file);
            kls_usageReport_toFile(temporary_kls, kls_file);
            fprintf(kls_file, "--END debug of temporary_kls--\n\n");
            WINDOW *win = NULL;
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
            kls_showList_toWin(temporary_kls, win);
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
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }

            fprintf(kls_file, "--BEGIN debug of passed kls--\n");
            for (int i = HR_Path; i < HLP_MAX_INDEX + 99; i++) {
                ptrdiff_t usage = kls_type_usage(i, kls);
#ifndef _WIN32
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %li }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#else
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %lli }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#endif
            }
            fprintf(kls_file, "--END debug of passed kls--\n\n");

            fclose(kls_file);
        }
        break;
        case 'k': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }
            fprintf(kls_file, "--BEGIN debug of passed kls--\n");
            print_kls_2file(kls_file, kls);
            kls_showList_toFile(kls->regs, kls_file);
            kls_usageReport_toFile(kls, kls_file);
            fprintf(kls_file, "--END debug of passed kls--\n\n");
            WINDOW *win = NULL;
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
            kls_showList_toWin(kls, win);
            delwin(win);
            endwin();

            fclose(kls_file);
        }
        break;
        case 'Q': {
            G_FASTQUIT_ON = (G_FASTQUIT_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_FASTQUIT_ON, new value: (%i)",
                    G_FASTQUIT_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'g': {
            G_GODMODE_ON = (G_GODMODE_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_GODMODE_ON, new value: (%i)",
                    G_GODMODE_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'L': {
            G_LOG_ON = (G_LOG_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_LOG_ON, new value: (%i)", G_LOG_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        case 's': {		//Query sprites slideshow
            picked_debug_proc = 1;
            int res = system("clear");
            sprintf(msg, "debug_generic() 4 system(\"clear\") res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            int comm_len = -1;
            int picked_comm = 0;
            char comm[15] = "empty";
            char *desc = NULL;
            int sprite_total = -1;
            int tot_cons = CONSUMABLESMAX + 1;
            int tot_enemy = ENEMYCLASSESMAX + 1;
            int tot_boss = BOSSCLASSESMAX + 1;
            int tot_art = ARTIFACTSMAX + 1;
            int tot_eq = EQUIPSMAX + 1;
            int tot_eqzones = EQUIPZONES + 1;
            int tot_misc = MISC_SPRITES_MAX + 1;
            int allsprites =
                tot_cons +
                tot_enemy +
                tot_boss + tot_art + tot_eq + tot_eqzones + tot_misc;

            printf("\nConsumables sprites: \t%d\
\nArtifacts sprites: \t%d\
\nEquips sprites: \t%d\
\nEquip zone sprites: \t%d\
\nEnemies sprites: \t%d\
\nBosses sprites: \t%d\
\nOther misc sprites: \t%d\n", tot_cons, tot_art, tot_eq, tot_eqzones, tot_enemy, tot_boss, tot_misc);
            printf("--------------------------");
            printf("\nTotal sprites: \t\t%d\n", allsprites);
            printf
            ("\nInsert sprites query, q to quit: (q|boss|cons|equip|eqzone|artf|enemy|misc):\n\n[%s@spriteshow]$ ",
             player->name);
            while (!picked_comm && (comm_len = scanf("%10s", comm)) > -1) {
                int res = scanf("%*c");
                sprintf(msg, "debug_generic() 7 scanf() res was (%i)", res);
                log_tag("debug_log.txt", "[DEBUG]", msg);
                if (strcmp(comm, "q") == 0) {
                    return;
                } else if (strcmp(comm, "boss") == 0) {
                    picked_comm = 1;
                    sprite_total = BOSSCLASSESMAX + 1;
                } else if (strcmp(comm, "cons") == 0) {
                    picked_comm = 1;
                    sprite_total = CONSUMABLESMAX + 1;
                } else if (strcmp(comm, "equip") == 0) {
                    picked_comm = 1;
                    sprite_total = EQUIPSMAX + 1;
                } else if (strcmp(comm, "eqzone") == 0) {
                    picked_comm = 1;
                    sprite_total = EQUIPZONES + 1;
                } else if (strcmp(comm, "artf") == 0) {
                    picked_comm = 1;
                    sprite_total = ARTIFACTSMAX + 1;
                } else if (strcmp(comm, "enemy") == 0) {
                    picked_comm = 1;
                    sprite_total = ENEMYCLASSESMAX + 1;
                } else if (strcmp(comm, "misc") == 0) {
                    picked_comm = 1;
                    sprite_total = MISC_SPRITES_MAX + 1;
                } else {
                    printf
                    ("\nInsert sprites query, q to quit: (q|boss|cons|equip|eqzone|artf|enemy|misc):\n\n[%s@spriteshow]$ ",
                     player->name);
                }
            }
            int sprite_count = -1;
            int c = -1;
            char s[20];
            int startx = 0;
            int x, y;
            WINDOW *spriteshow_win;
            /* Initialize curses */
            //initscr();
            start_color();
            clear();
            refresh();
            cbreak();
            noecho();
            keypad(stdscr, TRUE);
            spriteshow_win = newwin(19, 19, 2, 2);
            keypad(spriteshow_win, TRUE);
            for (sprite_count = 0; sprite_count < sprite_total;
                 sprite_count++) {
                if (strcmp(comm, "boss") == 0) {
                    desc = stringFromBossClass(sprite_count);
                } else if (strcmp(comm, "cons") == 0) {
                    desc = stringFromConsumables(sprite_count);
                } else if (strcmp(comm, "equip") == 0) {
                    desc = stringFromEquips(sprite_count);
                } else if (strcmp(comm, "eqzone") == 0) {
                    desc = stringFromEquipzones(sprite_count);
                } else if (strcmp(comm, "artf") == 0) {
                    desc = stringFromArtifacts(sprite_count);
                } else if (strcmp(comm, "enemy") == 0) {
                    desc = stringFromEClass(sprite_count);
                } else if (strcmp(comm, "misc") == 0) {
                    desc = "Misc";
                }
                wclear(spriteshow_win);
                wrefresh(spriteshow_win);
                wclear(stdscr);
                wrefresh(stdscr);
                sprintf(msg, "Showing sprite n.%d, class is: %s\n",
                        sprite_count, desc);
                log_tag("debug_log.txt", "[DEBUG]", msg);
                mvwprintw(stdscr, 5, 20, "Sprite for: \'%s\'", desc);
                mvwprintw(stdscr, 7, 20, "(%i/%i)", sprite_count + 1,
                          sprite_total);
                refresh();
                for (int i = 0; i < 8; i++) {
                    if (strcmp(comm, "boss") == 0) {
                        strcpy(s, bosses_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "cons") == 0) {
                        strcpy(s, consumables_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "equip") == 0) {
                        strcpy(s, equips_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "eqzone") == 0) {
                        strcpy(s, equipzones_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "artf") == 0) {
                        strcpy(s, artifacts_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "enemy") == 0) {
                        strcpy(s, enemies_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "misc") == 0) {
                        strcpy(s, misc_sprites[sprite_count][i]);
                    }
                    //sprintf(msg,"[DEBUG]    Copied string: \'%s\'\n", s);
                    //debug_log("debug_log.txt",msg);

                    for (int j = 0; j < 13; j++) {
                        x = startx + 1 + j;
                        y = 1 + i;

                        print_encoded_char(spriteshow_win, y, x, s[j]);
                    }	//End for line print
                }		//End for sprite print

                wrefresh(spriteshow_win);
                //printf("\nPress Enter for next, q to quit.\n");
                int picked = 0;
                int go_previous = 0;
                int quit_show = 0;
                while (!picked
                       && ((c = wgetch(spriteshow_win)) != KEY_F(1))) {
                    if (c == 10 || c == KEY_RIGHT) {	//Enter, right
                        sprintf(msg, "Going to next sprite.\n");
                        log_tag("debug_log.txt", "[DEBUG]", msg);
                        wclear(spriteshow_win);
                        wrefresh(spriteshow_win);
                        picked = 1;
                        continue;	//Go to next sprite
                    } else if (c == 'q') {	//Quit
                        sprintf(msg, "Stopping query sprite show.\n");
                        log_tag("debug_log.txt", "[DEBUG]", msg);
                        picked = 1;
                        quit_show = 1;
                    } else if (c == KEY_LEFT) {	//Go to previous sprite
                        go_previous = 1;
                        sprintf(msg, "Going to previous sprite.\n");
                        log_tag("debug_log.txt", "[DEBUG]", msg);
                        picked = 1;
                    } else {	//Unexpected char
                        sprintf(msg,
                                "Wrong operation. Continuing with next sprite.\n");
                        log_tag("debug_log.txt", "\033[1;31m[ERROR]", msg);
                        picked = 1;
                        continue;	//We still go to next sprite
                    }
                }		//End while wait for user input
                if (go_previous) {
                    sprite_count--;
                    sprite_count--;
                    if (sprite_count < -1)
                        sprite_count = -1;
                    go_previous = 0;
                }
                if (quit_show) {
                    break;
                }
            }		//End for all sprites
            sprintf(msg, "End of sprite show.\n");
            log_tag("debug_log.txt", "[DEBUG]", msg);

            delwin(spriteshow_win);
            endwin();
        }
        break;
        case 'q': {
            return;
        }
        default: {		//Echo the passed char and ask for one more.
            char cmd[50];
            sprintf(cmd, "\necho \"%c\\n\"\n%c\n\n", ch[0], ch[0]);
            printf("%s", cmd);
            napms(500);
        }
        break;
        }			//Close switch on ch[0]
    }				//Close while !picked_debug_proc

    res = system("clear");
    sprintf(msg, "debug_generic() final system(\"clear\") res was (%i)", res);
    log_tag("debug_log.txt", "[DEBUG]", msg);
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
 * @param gmst The Gamestate pointer.
 * @param player The Fighter pointer at hand.
 * @param e The Enemy pointer for current enemy.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param currentEnemyNum The index of current enemy.
 * @param kls The Koliseo used for allocations.
 * @param t_kls The Koliseo_Temp used for temporary allocations.
 */
void debug_enemies_room(Gamestate *gmst, Room *room, Fighter *player, Enemy *e,
                        Path *p, int roomIndex, int currentEnemyNum,
                        Koliseo *kls, Koliseo_Temp *t_kls)
{

    char msg[200];
    char ch[25];
    int picked_debug_proc = 0;
#ifndef _WIN32
    struct utsname uts;
    uname(&uts);
    sprintf(msg, "debug_enemies_room() loaded utsname using uname().\n");
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "System is %s\n", uts.sysname);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "OS Release is %s\n", uts.release);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "OS Version is %s\n", uts.version);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    sprintf(msg, "Machine is %s\n", uts.machine);
    log_tag("debug_log.txt", "[DEBUG]", msg);
#endif

    int res = system("clear");
    sprintf(msg, "debug_enemies_room() system(\"clear\") res was (%i)", res);
    log_tag("debug_log.txt", "[DEBUG]", msg);

    int c = 0, n = -1;
    while (!picked_debug_proc) {
        int res = system("clear");
        sprintf(msg, "debug_enemies_room() 2 system(\"clear\") res was (%i)",
                res);
        log_tag("debug_log.txt", "[DEBUG]", msg);
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
			'L':  Toggle logging\t'k': Log passed Koliseo info\n\
			't': Log global temporary_kls Koliseo info\n\
			'K': Log usage stats for passed kls to debug log file.\n\
			'T': Log usage stats for temporary_kls to debug log file.\n\
			'G': Log Gamestate to debug log file.\n\
			'q': Quit\t{Return}  Process your input line.\n\
		]\n\n\
	[%s@debug-func]$ ", player->name);

        char *fgetsres = fgets(ch, sizeof ch, stdin);
        sprintf(msg, "debug_enemies_room() fgets() res was (%s)", fgetsres);
        log_tag("debug_log.txt", "[DEBUG]", msg);
        switch (ch[0]) {
        case '0': {
            picked_debug_proc = 1;
            int xp;
            do {
                printf("\nHow much?\n");
            } while ((c = scanf("%i", &xp)) != 1 || xp < 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            checkremainder(player, xp);
        }
        break;
        case '1': {
            picked_debug_proc = 1;
            int q = -1;
            do {
                printf("\nInsert consumable number:\n");
            } while ((c = scanf("%i", &n)) != 1 || n > CONSUMABLESMAX
                     || n < 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            do {
                printf("\nInsert quantity:\n");
            } while ((c = scanf("%i", &q)) != 1 && q <= 0);
            res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() 2 scanf() res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            Consumable *c = (Consumable *) player->consumablesBag[n];
            c->qty += q;
            player->stats->consumablesfound += q;
        }
        break;
        case '2': {
            picked_debug_proc = 1;
            statReset(player, 1);
        }
        break;
        case '3': {
            picked_debug_proc = 1;
            statResetEnemy(e, 1);
        }
        break;
        case '4': {
            picked_debug_proc = 1;
            printf("\nCurrent luck: %i\tRL: %i\n", player->luck, p->luck);

            do {
                printf("\nInsert new luck:\n");
            } while ((c = scanf("%i", &n)) != 1 && n > 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() 3 scanf() res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            //FIXME:
            //Copy-pasted the calc for player luck... might need a function
            p->luck = n;

            player->luck = (p->luck * MAXPLAYERLUCK) / MAXLUCK;
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
            int n = -1;
            do {
                printf("\nInsert coin number (0<n<100):\n");
            } while ((c = scanf("%i", &n)) < 0 || c > 3 || n < 0
                     || n > 100);
            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() 4 scanf() res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
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
            int n = -1;
            do {
                printf("\nInsert artifact number (0<n<%i):\n",
                       ARTIFACTSMAX + 1);
            } while ((c = scanf("%i", &n)) != 1 || n > ARTIFACTSMAX
                     || n < 0);

            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() scanf() res was (%i)", res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            player->artifactsBag[n]->qty += 1;
            player->artifactsBag[n]->active = 0;
            player->stats->artifactsfound += 1;
        }
        break;
        case '8': {
            picked_debug_proc = 1;
            int res = system("clear");
            sprintf(msg,
                    "debug_enemies_room() 3 system(\"clear\") res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            printf("\nPlayer Counters:\n");
            printCounters((Turncounter **) player->counters);
            printf("\nEnemy (%s) Counters:\n", stringFromEClass(e->class));
            printCounters((Turncounter **) e->counters);
            printf("\nPress Enter to resume game");
            res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() 5 scanf() res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case '9': {
            picked_debug_proc = 1;
            int q = -1;
            int c = -1;
            do {
                printf("\nInsert quantity:\n");
            } while ((c = scanf("%i", &q)) != 1 && q <= 0);
            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() 6 scanf() res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);

            //TODO: handle dropping an equip with a new notification window
            //for (int i=0; i<q;i++) {dropEquip(player,1,notify_win);};
            log_tag("debug_log.txt", "[DEBUG]", "TODO: not working ATM.");
        }
        break;
        case 'd': {
            picked_debug_proc = 1;
            printf("\nVERSION:    %s\n", VERSION);
#ifndef _WIN32
            printf("\nSystem:    %s\n", uts.sysname);
            printf("\nOS Release:    %s\n", uts.release);
            printf("\nOS Version:    %s\n", uts.version);
            printf("\nMachine:    %s\n", uts.machine);
#endif
            printf("\nGAMEMODE:    %s\n", stringFromGamemode(GAMEMODE));
            printf("\nPath->current_saveslot->save_path:    %s\n",
                   p->current_saveslot->save_path);
            printf("\nGS_AUTOSAVE_ON:    %i\n", GS_AUTOSAVE_ON);
            printf("\nG_FASTQUIT_ON:    %i\n", G_FASTQUIT_ON);
            printf("\nG_DEBUG_ON:    %i\n", G_DEBUG_ON);
            printf("\nG_LOG_ON:    %i\n", G_LOG_ON);
            printf("\nG_GODMODE_ON:    %i\n", G_GODMODE_ON);
            printf("\nG_EXPERIMENTAL_ON:    %i\n", G_EXPERIMENTAL_ON);
            printf("\nG_DEBUG_ROOMTYPE_ON:    %i\n", G_DEBUG_ROOMTYPE_ON);
            if (G_DEBUG_ROOMTYPE_ON == 1) {
                printf("\nG_DEBUG_ROOMTYPE:    %i\n", G_DEBUG_ROOMTYPE);
            }
            printf("\nG_DEBUG_ENEMYTYPE_ON:    %i\n", G_DEBUG_ENEMYTYPE_ON);
            if (G_DEBUG_ENEMYTYPE_ON == 1) {
                printf("\nG_DEBUG_ENEMYTYPE:    %i\n", G_DEBUG_ENEMYTYPE);
            }
            printf("\nPress Enter to resume game.\n");
            int res = scanf("%*c");
            sprintf(msg, "debug_enemies_room() 7 scanf() res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'f': {
            clear();
            refresh();
            debug_printFoeParty(room->foes);
        }
        break;
        case 'G': {
            log_tag("debug_log.txt", "[DEBUG]", "Logging Gamestate");
            dbg_Gamestate(gmst);
            log_tag("debug_log.txt", "[DEBUG]", "Done logging Gamestate");
        }
        break;
        case 'T': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }

            fprintf(kls_file, "--BEGIN debug of temporary_kls--\n");
            for (int i = HR_Path; i < HLP_MAX_INDEX + 99; i++) {
                ptrdiff_t usage = kls_type_usage(i, temporary_kls);
#ifndef _WIN32
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %li }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#else
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %lli }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#endif
            }
            fprintf(kls_file, "--END debug of temporary_kls--\n\n");

            fclose(kls_file);
        }
        break;
        case 't': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }
            fprintf(kls_file, "--BEGIN debug of temporary_kls--\n");
            print_kls_2file(kls_file, temporary_kls);
            kls_showList_toFile(temporary_kls->regs, kls_file);
            kls_usageReport_toFile(temporary_kls, kls_file);
            fprintf(kls_file, "--END debug of temporary_kls--\n\n");
            WINDOW *win = NULL;
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
            kls_showList_toWin(temporary_kls, win);
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
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }

            fprintf(kls_file, "--BEGIN debug of passed kls--\n");
            for (int i = HR_Path; i < HLP_MAX_INDEX + 99; i++) {
                ptrdiff_t usage = kls_type_usage(i, kls);
#ifndef _WIN32
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %li }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#else
                fprintf(kls_file,
                        "Usage for HLP_Region_Type { %s } [Index: %i]  {Size: %lli }\n",
                        stringFrom_HLP_Region_Type(i - 101 +
                                                   KLS_REGIONTYPE_MAX), i,
                        usage);
#endif
            }
            fprintf(kls_file, "--END debug of passed kls--\n\n");

            fclose(kls_file);
        }
        break;
        case 'k': {
            char path_to_kls_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Append to "kls_log.txt"
            sprintf(path_to_kls_file, "%s/%s", static_path,
                    "debug_log.txt");
            FILE *kls_file = NULL;
            kls_file = fopen(path_to_kls_file, "a");
            if (kls_file == NULL) {
                fprintf(stderr,
                        "debug_generic():  failed opening debug logfile.\n");
                exit(EXIT_FAILURE);
            }
            if (kls == NULL) {
                fprintf(stderr, "debug_generic():  kls was NULL.\n");
                exit(EXIT_FAILURE);
            }
            fprintf(kls_file, "--BEGIN debug of passed kls--\n");
            print_kls_2file(kls_file, kls);
            kls_showList_toFile(kls->regs, kls_file);
            kls_usageReport_toFile(kls, kls_file);
            fprintf(kls_file, "--END debug of passed kls--\n\n");
            WINDOW *win = NULL;
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
            kls_showList_toWin(kls, win);
            delwin(win);
            endwin();
            fclose(kls_file);
        }
        break;
        case 'A': {
            GS_AUTOSAVE_ON = (GS_AUTOSAVE_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_AUTOSAVE_ON, new value: (%i)",
                    GS_AUTOSAVE_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'Q': {
            G_FASTQUIT_ON = (G_FASTQUIT_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_FASTQUIT_ON, new value: (%i)",
                    G_FASTQUIT_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'L': {
            G_LOG_ON = (G_LOG_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_LOG_ON, new value: (%i)", G_LOG_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 'g': {
            G_GODMODE_ON = (G_GODMODE_ON == 1 ? 0 : 1);
            sprintf(msg, "Toggled G_GODMODE_ON, new value: (%i)",
                    G_GODMODE_ON);
            log_tag("debug_log.txt", "[DEBUG]", msg);
        }
        break;
        case 's': {		//Query sprites slideshow
            picked_debug_proc = 1;
            int res = system("clear");
            sprintf(msg,
                    "debug_enemies_room() 4 system(\"clear\") res was (%i)",
                    res);
            log_tag("debug_log.txt", "[DEBUG]", msg);
            int comm_len = -1;
            int picked_comm = 0;
            char comm[15] = "empty";
            char *desc = NULL;
            int sprite_total = -1;
            int tot_cons = CONSUMABLESMAX + 1;
            int tot_enemy = ENEMYCLASSESMAX + 1;
            int tot_boss = BOSSCLASSESMAX + 1;
            int tot_art = ARTIFACTSMAX + 1;
            int tot_eq = EQUIPSMAX + 1;
            int tot_eqzones = EQUIPZONES + 1;
            int tot_misc = MISC_SPRITES_MAX + 1;
            int allsprites =
                tot_cons +
                tot_enemy +
                tot_boss + tot_art + tot_eq + tot_eqzones + tot_misc;

            printf("\nConsumables sprites: \t%d\
\nArtifacts sprites: \t%d\
\nEquips sprites: \t%d\
\nEquip zone sprites: \t%d\
\nEnemies sprites: \t%d\
\nBosses sprites: \t%d\
\nOther misc sprites: \t%d\n", tot_cons, tot_art, tot_eq, tot_eqzones, tot_enemy, tot_boss, tot_misc);
            printf("--------------------------");
            printf("\nTotal sprites: \t\t%d\n", allsprites);
            printf
            ("\nInsert sprites query, q to quit: (q|boss|cons|equip|eqzone|artf|enemy|misc):\n\n[%s@spriteshow]$ ",
             player->name);
            while (!picked_comm && (comm_len = scanf("%10s", comm)) > -1) {
                int res = scanf("%*c");
                sprintf(msg, "debug_enemies_room() 7 scanf() res was (%i)",
                        res);
                log_tag("debug_log.txt", "[DEBUG]", msg);
                if (strcmp(comm, "q") == 0) {
                    return;
                } else if (strcmp(comm, "boss") == 0) {
                    picked_comm = 1;
                    sprite_total = BOSSCLASSESMAX + 1;
                } else if (strcmp(comm, "cons") == 0) {
                    picked_comm = 1;
                    sprite_total = CONSUMABLESMAX + 1;
                } else if (strcmp(comm, "equip") == 0) {
                    picked_comm = 1;
                    sprite_total = EQUIPSMAX + 1;
                } else if (strcmp(comm, "eqzone") == 0) {
                    picked_comm = 1;
                    sprite_total = EQUIPZONES + 1;
                } else if (strcmp(comm, "artf") == 0) {
                    picked_comm = 1;
                    sprite_total = ARTIFACTSMAX + 1;
                } else if (strcmp(comm, "enemy") == 0) {
                    picked_comm = 1;
                    sprite_total = ENEMYCLASSESMAX + 1;
                } else if (strcmp(comm, "misc") == 0) {
                    picked_comm = 1;
                    sprite_total = MISC_SPRITES_MAX + 1;
                } else {
                    printf
                    ("\nInsert sprites query, q to quit: (q|boss|cons|equip|eqzone|artf|enemy|misc):\n\n[%s@spriteshow]$ ",
                     player->name);
                }
            }
            int sprite_count = -1;
            int c = -1;
            char s[20];
            int startx = 0;
            int x, y;
            WINDOW *spriteshow_win;
            /* Initialize curses */
            //initscr();
            start_color();
            clear();
            refresh();
            cbreak();
            noecho();
            keypad(stdscr, TRUE);
            spriteshow_win = newwin(19, 19, 2, 2);
            keypad(spriteshow_win, TRUE);
            for (sprite_count = 0; sprite_count < sprite_total;
                 sprite_count++) {
                if (strcmp(comm, "boss") == 0) {
                    desc = stringFromBossClass(sprite_count);
                } else if (strcmp(comm, "cons") == 0) {
                    desc = stringFromConsumables(sprite_count);
                } else if (strcmp(comm, "equip") == 0) {
                    desc = stringFromEquips(sprite_count);
                } else if (strcmp(comm, "eqzone") == 0) {
                    desc = stringFromEquipzones(sprite_count);
                } else if (strcmp(comm, "artf") == 0) {
                    desc = stringFromArtifacts(sprite_count);
                } else if (strcmp(comm, "enemy") == 0) {
                    desc = stringFromEClass(sprite_count);
                } else if (strcmp(comm, "misc") == 0) {
                    desc = "Misc";
                }
                wclear(spriteshow_win);
                wrefresh(spriteshow_win);
                wclear(stdscr);
                wrefresh(stdscr);
                sprintf(msg, "Showing sprite n.%d, class is: %s\n",
                        sprite_count, desc);
                log_tag("debug_log.txt", "[DEBUG]", msg);
                mvwprintw(stdscr, 5, 20, "Sprite for: \'%s\'", desc);
                mvwprintw(stdscr, 7, 20, "(%i/%i)", sprite_count + 1,
                          sprite_total);
                refresh();
                for (int i = 0; i < 8; i++) {
                    if (strcmp(comm, "boss") == 0) {
                        strcpy(s, bosses_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "cons") == 0) {
                        strcpy(s, consumables_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "equip") == 0) {
                        strcpy(s, equips_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "eqzone") == 0) {
                        strcpy(s, equipzones_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "artf") == 0) {
                        strcpy(s, artifacts_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "enemy") == 0) {
                        strcpy(s, enemies_sprites[sprite_count][i]);
                    } else if (strcmp(comm, "misc") == 0) {
                        strcpy(s, misc_sprites[sprite_count][i]);
                    }
                    //sprintf(msg,"[DEBUG]    Copied string: \'%s\'\n", s);
                    //debug_log("debug_log.txt",msg);

                    for (int j = 0; j < 13; j++) {
                        x = startx + 1 + j;
                        y = 1 + i;

                        print_encoded_char(spriteshow_win, y, x, s[j]);
                    }	//End for line print
                }		//End for sprite print

                wrefresh(spriteshow_win);
                //printf("\nPress Enter for next, q to quit.\n");
                int picked = 0;
                int go_previous = 0;
                int quit_show = 0;
                while (!picked
                       && ((c = wgetch(spriteshow_win)) != KEY_F(1))) {
                    if (c == 10 || c == KEY_RIGHT) {	//Enter, right
                        sprintf(msg, "Going to next sprite.\n");
                        log_tag("debug_log.txt", "[DEBUG]", msg);
                        wclear(spriteshow_win);
                        wrefresh(spriteshow_win);
                        picked = 1;
                        continue;	//Go to next sprite
                    } else if (c == 'q') {	//Quit
                        sprintf(msg, "Stopping query sprite show.\n");
                        log_tag("debug_log.txt", "[DEBUG]", msg);
                        picked = 1;
                        quit_show = 1;
                    } else if (c == KEY_LEFT) {	//Go to previous sprite
                        go_previous = 1;
                        sprintf(msg, "Going to previous sprite.\n");
                        log_tag("debug_log.txt", "[DEBUG]", msg);
                        picked = 1;
                    } else {	//Unexpected char
                        sprintf(msg,
                                "Wrong operation. Continuing with next sprite.\n");
                        log_tag("debug_log.txt", "\033[1;31m[ERROR]", msg);
                        picked = 1;
                        continue;	//We still go to next sprite
                    }
                }		//End while wait for user input
                if (go_previous) {
                    sprite_count--;
                    sprite_count--;
                    if (sprite_count < -1)
                        sprite_count = -1;
                    go_previous = 0;
                }
                if (quit_show) {
                    break;
                }
            }		//End for all sprites
            sprintf(msg, "End of sprite show.\n");
            log_tag("debug_log.txt", "[DEBUG]", msg);

            delwin(spriteshow_win);
            endwin();
        }
        break;
        case 'q': {
            return;
        }
        default: {		//Echo the passed char and ask for one more.
            char cmd[50];
            sprintf(cmd, "\necho \"%c\\n\"\n%c\n\n", ch[0], ch[0]);
            printf("%s", cmd);
            napms(500);
        }
        break;
        }			//Close switch on ch[0]
    }				//Close while !picked_debug_proc

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
void quit(Fighter *p, Room *room, loadInfo *load_info, Koliseo_Temp *t_kls)
{
    char msg[500];
    Koliseo *kls = t_kls->kls;
    endwin();
    int res = system("reset");
    sprintf(msg, "quit() system(\"reset\") res was (%i)", res);
    log_tag("debug_log.txt", "[DEBUG]", msg);
    //printf("\n\n\tTHANKS 4 PLAYING\n");
    //FIXME
    //dropping out of the Koliseo scope might render stat pointer invalid.
    //Can't we print stats and clear the kls?
    //printStats(p);
    //printf("\n");
#ifndef _WIN32
    sprintf(msg, "Resetting Koliseo_Temp from: (%li)", t_kls->kls->offset);
#else
    sprintf(msg, "Resetting Koliseo_Temp from: (%lli)", t_kls->kls->offset);
#endif
    kls_log(kls, "DEBUG", msg);
    death(p, load_info);
    //FIXME
    //I think we should free those?
    //May cause a crash?
    //kls_free(default_kls);
    //kls_free(temporary_kls);
    //FIXME:
    //Calling this segfaults?
    //freeRoom(room);
    log_tag("debug_log.txt", "[DEBUG]", "Quitting program.");
    exit(EXIT_SUCCESS);
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
int handleRoom_Roadfork(Room *room, int *roadFork_value, int roomsDone,
                        Path *path, Fighter *f)
{
    //Strings for turn menu choices
    char choices[ROADFORK_OPTIONS_MAX + 1][80] = {
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
            strcpy(c, "Boss Room");
        }
        break;
        case ROADFORK_SHOP: {
            strcpy(c, "Shop Room");
        }
        break;
        case ROADFORK_TREASURE: {
            strcpy(c, "Treasure Room");
        }
        break;
        case ROADFORK_ENEMIES: {
            strcpy(c, "Enemies");
        }
        break;
        case ROADFORK_ROADFORK: {
            strcpy(c, "Roadfork");
        }
        break;
        }
        strcpy(choices[i], c);
    }
    sprintf(msg, "Entered Roadfork with %s and %s.\n", choices[0], choices[1]);
    log_tag("debug_log.txt", "[ROADFORK]", msg);

    ITEM **my_items;
    MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices, c;

    /* Initialize curses */
    //initscr();
    start_color();
    clear();
    refresh();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    /* Create items */
    n_choices = ARRAY_SIZE(choices);
    sprintf(msg, "n_choices size was: (%i)\n", n_choices);
    log_tag("debug_log.txt", "[ROADFORK]", msg);
    my_items = (ITEM **) calloc(n_choices, sizeof(ITEM *));
    for (int k = 0; k < n_choices; k++) {
        my_items[k] = new_item(choices[k], choices[k]);
        /* Set the user pointer */
        //set_item_userptr(my_items[i]);
    }

    /* Create menu */
    my_menu = new_menu((ITEM **) my_items);

    /* Set description off */
    menu_opts_off(my_menu, O_SHOWDESC);

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
    sprintf(label, "Road Fork");

    /* Print a border around the main window and print a title */
    box(my_menu_win, 0, 0);
    print_label(my_menu_win, 1, 0, 28, label, COLOR_PAIR(S4C_MAGENTA));
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

    attron(COLOR_PAIR(S4C_CYAN));
    mvprintw(20, 2, "Arrows to move");
    mvprintw(21, 2, "(q to Exit)");
    attroff(COLOR_PAIR(S4C_CYAN));
    refresh();

    int end_room = 0;
    int check = -1;

    while (!end_room && (c = wgetch(my_menu_win)) != KEY_F(1)) {
        switch (c) {
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
        case 10: {	// Enter
            ITEM *cur;

            cur = current_item(my_menu);
            sprintf(msg, "Picked %s.\n", item_name(cur));
            log_tag("debug_log.txt", "[ROADFORK]", msg);
            if ((check = strcmp("Boss Room", item_name(cur))) == 0) {
                *roadFork_value = BOSS;
                end_room = 1;
            } else if ((check = strcmp("Shop Room", item_name(cur)) == 0)) {
                *roadFork_value = SHOP;
                end_room = 1;
            } else if ((check =
                            strcmp("Treasure Room", item_name(cur)) == 0)) {
                *roadFork_value = TREASURE;
                end_room = 1;
            } else if ((check = strcmp("Enemies", item_name(cur)) == 0)) {
                *roadFork_value = ENEMIES;
                end_room = 1;
            } else if ((check = strcmp("Roadfork", item_name(cur)) == 0)) {
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
    for (int k = 0; k < n_choices; k++) {
        free_item(my_items[k]);
    }
    endwin();

    //FIXME
    //Why are we relying on this?
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
Path *randomise_path(int seed, Koliseo *kls, const char *path_to_savefile)
{
    char msg[200];
    sprintf(msg, "Prepping Path");
    kls_log(kls, "DEBUG", msg);
    Path *p = (Path *) KLS_PUSH_TYPED(kls, Path, HR_Path, "Path", msg);
    srand(seed);
    sprintf(msg, "Prepping Saveslot");
    kls_log(kls, "DEBUG", msg);
    sprintf(msg, "save_path: [%s]", path_to_savefile);
    Saveslot *save =
        (Saveslot *) KLS_PUSH_TYPED(kls, Saveslot, HR_Saveslot, "Saveslot",
                                    msg);
    sprintf(msg, "Seed: %i", seed);
    strcpy(save->name, msg);
    sprintf(msg, "%s", path_to_savefile);
    strcpy(save->save_path, msg);
    p->current_saveslot = save;
    kls_log(kls, "DEBUG",
            "Prepped Saveslot:  path->current_saveslot->save_path == [%s]",
            p->current_saveslot->save_path);
    log_tag("debug_log.txt", "[SAVESLOT]",
            "Prepped Saveslot:  path->current_saveslot->save_path == [%s]",
            p->current_saveslot->save_path);

    switch (GAMEMODE) {
    case Standard: {
        p->length = (rand() % MAXLENGTH) + 1;
        p->luck = (rand() % MAXLUCK) + 1;
        p->prize = 15 / p->luck * (rand() % 150) + 500;
    }
    break;
    case Story: {
        p->length = 41;
        p->luck = (rand() % MAXLUCK) + 1;
        p->prize = 15 / p->luck * (rand() % 150) + 500;
    }
    break;
    case Rogue: {
        p->length = 1;
        p->luck = (rand() % MAXLUCK) + 1;
        p->prize = 15 / p->luck * (rand() % 150) + 500;
    }
    break;
    default: {
        fprintf(stderr, "\nUnexpected GAMEMODE value %i.\n", GAMEMODE);
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
void gameloop(int argc, char **argv)
{

    char *whoami;		// This will reference argv[0] at basename, it's the same string in memory, just starting later
    char path_to_kls_debug_file[600];
    char static_path[500];
    // Set static_path value to the correct static dir path
    resolve_staticPath(static_path);

    //Truncate "debug_log.txt"
    sprintf(path_to_kls_debug_file, "%s/%s", static_path, "kls_debug_log.txt");
    KLS_Conf default_kls_conf = {
        .kls_autoset_regions = 1,
        .kls_autoset_temp_regions = 1,
        .kls_verbose_lvl = 1,
        .kls_log_filepath = path_to_kls_debug_file,
        .kls_reglist_kls_size = KLS_DEFAULT_SIZE * 16,
        .kls_reglist_alloc_backend = KLS_REGLIST_ALLOC_KLS_BASIC,
    };
    KLS_Conf temporary_kls_conf = {
        .kls_autoset_regions = 1,
        .kls_autoset_temp_regions = 1,
        .kls_verbose_lvl = 0,
        .kls_log_fp = stderr,
        .kls_reglist_kls_size = KLS_DEFAULT_SIZE * 16,
        .kls_reglist_alloc_backend = KLS_REGLIST_ALLOC_KLS_BASIC,
    };

    do {
        //Init default_kls
        default_kls = kls_new_conf(KLS_DEFAULT_SIZE * 16, default_kls_conf);
        temporary_kls = kls_new_conf(KLS_DEFAULT_SIZE * 32, temporary_kls_conf);

#ifndef _WIN32
        (whoami = strrchr(argv[0], '/')) ? ++whoami : (whoami = argv[0]);
#else
        (whoami = strrchr(argv[0], '\\')) ? ++whoami : (whoami = argv[0]);
#endif

        char *kls_progname =
            (char *)KLS_PUSH_ARR_TYPED(default_kls, char *, sizeof(whoami),
                                       KLS_None, "progname", whoami);
        strcpy(kls_progname, whoami);

#ifndef HELAPORDO_DEBUG_LOG
#else
        FILE *debug_file = NULL;
        FILE *OPS_debug_file = NULL;
#endif
        // Parse command-line options
        int option;
        loadInfo *load_info =
            (loadInfo *) KLS_PUSH_TYPED(default_kls, loadInfo, HR_loadInfo,
                                        "loadInfo", "loadInfo");

        load_info->is_new_game = 1;	//By default we do a new game
        load_info->enemy_index = -1;
        load_info->total_foes = -1;
        load_info->save_type = -1;
        int loaded_roomtotalenemies = -1;
        int loaded_roomindex = -1;
        load_info->ptr_to_roomtotalenemies = &loaded_roomtotalenemies;
        load_info->ptr_to_roomindex = &loaded_roomindex;

        while ((option = getopt(argc, argv, "f:r:E:tTGRXQLlvdhsaV")) != -1) {
            switch (option) {
            case 'd': {
#ifndef HELAPORDO_DEBUG_ACCESS
#else
                G_DEBUG_ON += 1;
                G_LOG_ON = 1;
#endif
            }
            break;
            case 'r': {
                G_DEBUG_ROOMTYPE_ON += 1;
                G_DEBUG_ROOMTYPE_ARG = optarg;
            }
            break;
            case 'E': {
                G_DEBUG_ENEMYTYPE_ON += 1;
                G_DEBUG_ENEMYTYPE_ARG = optarg;
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
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            }
            break;
            case 'T': {
                G_DOTUTORIAL_ON = 1;
                handleTutorial();
                usage(whoami);
                kls_free(default_kls);
                kls_free(temporary_kls);
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
                napms(200);
                display_colorpairs();
                napms(200);
                WINDOW *test_win;
                initscr();
                start_color();
                for (int i = 0; i < PALETTE_S4C_H_TOTCOLORS; i++) {
                    init_s4c_color_pair(&palette[i], 9 + i);
                }
                clear();
                refresh();
                cbreak();
                noecho();
                test_win = newwin(9, 7, 1, 1);
                keypad(test_win, TRUE);
                box(test_win, 0, 0);

                refresh();

                test_game_color_pairs(test_win, 5);

                napms(200);
                delwin(test_win);
                endwin();
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            }
            break;
            case 'V': {
                printf("helapordo build: %s\n", helapordo_build_string);
                hlpd_dbg_features();
                printf("  using: s4c-animate v%s\n", S4C_ANIMATE_VERSION);
                s4c_dbg_features();
                printf("  using: koliseo v%s\n", string_koliseo_version());
                kls_dbg_features();
                printf("  using: ncurses v%s\n", NCURSES_VERSION);
#ifdef ANVIL__helapordo__
#ifndef INVIL__helapordo__HEADER__
                printf("  Built with: amboso v%s\n",
                       ANVIL__API_LEVEL__STRING);
#else
                printf("  Built with: invil v%s\n",
                       INVIL__VERSION__STRING);
                printf("Version Info: %.8s\n",
                       get_ANVIL__VERSION__DESC__());
                printf("Last commit: %s", get_INVIL__COMMIT__DESC__());
                const char* anvil_date = get_ANVIL__VERSION__DATE__();
                char* anvil_date_end;
#ifndef _WIN32
                time_t anvil_build_time = strtol(anvil_date, &anvil_date_end, 10);
#else
                time_t anvil_build_time = strtoll(anvil_date, &anvil_date_end, 10);
#endif //_WIN32

                if (anvil_date_end == anvil_date) {
                    //TODO: error
                } else {
                    char build_time_buff[20] = {0};
                    struct tm* build_time_tm = localtime(&anvil_build_time);

                    if (build_time_tm == NULL) {
                        //TODO: error
                    } else {
                        strftime(build_time_buff, 20, "%Y-%m-%d %H:%M:%S", build_time_tm);
                        printf("\nDate: %s\n", build_time_buff);
                    }
                }
                const char* headergen_date = get_INVIL__HEADERGEN__TIME__();
                char* headergen_date_end;
#ifndef _WIN32
                time_t headergen_time = strtol(headergen_date, &headergen_date_end, 10);
#else
                time_t headergen_time = strtoll(headergen_date, &headergen_date_end, 10);
#endif //_WIN32

                if (headergen_date_end == headergen_date) {
                    //TODO: error
                } else {
                    char headergen_time_buff[20] = {0};
                    struct tm* headergen_time_tm = localtime(&headergen_time);

                    if (headergen_time_tm == NULL) {
                        //TODO: error
                    } else {
                        strftime(headergen_time_buff, 20, "%Y-%m-%d %H:%M:%S", headergen_time_tm);
                        printf("Anvil Gen Date: %s\n", headergen_time_buff);
                    }
                }
#endif // INVIL__helapordo__HEADER__
#else
                printf("  Built without anvil\n");
#endif // ANVIL__helapordo__
                kls_free(default_kls);
                kls_free(temporary_kls);
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
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            }
            break;
            case '?': {
                fprintf(stderr,
                        "Invalid option: %c\n Check your arguments.\n",
                        option);
                usage(whoami);
                // Handle invalid options
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            default: {
                // Should never get here
                fprintf(stderr, "Invalid option: %c\n, bad usage.\n",
                        option);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            }
        }

#ifndef HELAPORDO_DEBUG_LOG
#else
        // Open log file if log flag is set and reset it
        if (G_LOG_ON == 1) {
            char path_to_debug_file[600];
            char path_to_OPS_debug_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Truncate "debug_log.txt"
            sprintf(path_to_debug_file, "%s/%s", static_path, "debug_log.txt");
            debug_file = fopen(path_to_debug_file, "w");
            if (!debug_file) {
                endwin();	//TODO: Can/should we check if we have to do this only in curses mode?
                fprintf(stderr,
                        "[ERROR]    Can't open debug logfile (%s/debug_log.txt).\n",
                        static_path);
                exit(EXIT_FAILURE);
            }
            fprintf(debug_file, "[DEBUGLOG]    --New game--  \n");
            if (NCURSES_VERSION_MAJOR < EXPECTED_NCURSES_VERSION_MAJOR
                && NCURSES_VERSION_MINOR < EXPECTED_NCURSES_VERSION_MINOR
                && NCURSES_VERSION_PATCH < EXPECTED_NCURSES_VERSION_PATCH) {
                fprintf(debug_file,
                        "[WARN]    ncurses version is lower than expected {%s: %i.%i.%i} < {%i.%i.%i}\n",
                        NCURSES_VERSION, NCURSES_VERSION_MAJOR,
                        NCURSES_VERSION_MINOR, NCURSES_VERSION_PATCH,
                        EXPECTED_NCURSES_VERSION_MAJOR,
                        EXPECTED_NCURSES_VERSION_MINOR,
                        EXPECTED_NCURSES_VERSION_PATCH);
            }
            fprintf(debug_file, "[DEBUG]    --Default kls debug info:--  \n");
            print_kls_2file(debug_file, default_kls);
            fprintf(debug_file, "[DEBUG]    --Temporary kls debug info:--  \n");
            print_kls_2file(debug_file, temporary_kls);
            fprintf(debug_file,
                    "[DEBUG]    --Closing header for new game.--  \n");
            fclose(debug_file);

            //Lay debug info
            log_tag("debug_log.txt", "[DEBUG]", "G_DEBUG_ON == (%i)",
                    G_DEBUG_ON);
            log_tag("debug_log.txt", "[DEBUG]", "kls_progname == (%s)",
                    kls_progname);
            log_tag("debug_log.txt", "[DEBUG]", "G_LOG_ON == (%i)", G_LOG_ON);
            log_tag("debug_log.txt", "[DEBUG]", "small DEBUG FLAG ASSERTED");
            log_tag("debug_log.txt", "[DEBUG]",
                    "[Current position in default_kls] [pos: %li]\n",
                    kls_get_pos(default_kls));

            //Truncate OPS_LOGFILE
            sprintf(path_to_OPS_debug_file, "%s/%s", static_path, OPS_LOGFILE);
            OPS_debug_file = fopen(path_to_OPS_debug_file, "w");
            if (!OPS_debug_file) {
                endwin();	//TODO: Can/should we check if we have to do this only in curses mode?
                fprintf(stderr, "[ERROR]    Can't open OPS logfile (%s/%s).\n",
                        static_path, OPS_LOGFILE);
                exit(EXIT_FAILURE);
            }
            fprintf(OPS_debug_file, "[OPLOG]    --New game--  \n");
            fclose(OPS_debug_file);
            log_tag("debug_log.txt", "[DEBUG]", "Truncated [%s]", OPS_LOGFILE);
        }
#endif

        if (G_DEBUG_ENEMYTYPE_ON == 1) {
            log_tag("debug_log.txt", "[DEBUG]", "G_DEBUG_ENEMYTYPE_ON == (%i)",
                    G_DEBUG_ENEMYTYPE_ON);
            log_tag("debug_log.txt", "[DEBUG]", "ENEMY DEBUG FLAG ASSERTED");
            if ((G_DEBUG_ON > 0)) {
                G_DEBUG_ON += 1;
                log_tag("debug_log.txt", "[DEBUG]", "G_DEBUG_ON == (%i)",
                        G_DEBUG_ON);
                log_tag("debug_log.txt", "[DEBUG]", "Forcing enemy type: (%s)",
                        G_DEBUG_ENEMYTYPE_ARG);
                int setenemy_debug = 0;
                for (int ec = 0; ec < ENEMYCLASSESMAX && (setenemy_debug == 0);
                     ec++) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Checking optarg for -E: (%s)",
                            stringFromEClass(ec));
                    if ((strcmp(G_DEBUG_ENEMYTYPE_ARG, stringFromEClass(ec)) ==
                         0)) {
                        log_tag("debug_log.txt", "[DEBUG]",
                                "Match on optarg (%s), setting G_DEBUG_ENEMYTYPE to (%i).",
                                stringFromEClass(ec), ec);
                        G_DEBUG_ENEMYTYPE = ec;
                        setenemy_debug = 1;
                    }
                }
                if (setenemy_debug == 0) {
                    log_tag("debug_log.txt", "[ERROR]",
                            "Invalid optarg for -E flag: {%s}.\n",
                            G_DEBUG_ENEMYTYPE_ARG);
                    fprintf(stderr,
                            "[ERROR]    Incorrect -E \"enemyType\" arg: {%s}.\n",
                            G_DEBUG_ENEMYTYPE_ARG);
                    fprintf(stderr, "[ERROR]    Run \"%s -h\" for help.\n",
                            kls_progname);
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                };
            }
        }
        if (G_DEBUG_ROOMTYPE_ON == 1) {
            log_tag("debug_log.txt", "[DEBUG]", "G_DEBUG_ROOMTYPE_ON == (%i)",
                    G_DEBUG_ROOMTYPE_ON);
            log_tag("debug_log.txt", "[DEBUG]", "ROOM DEBUG FLAG ASSERTED");
            if ((G_DEBUG_ON > 0)) {
                G_DEBUG_ON += 1;
                log_tag("debug_log.txt", "[DEBUG]", "G_DEBUG_ON == (%i)",
                        G_DEBUG_ON);
                log_tag("debug_log.txt", "[DEBUG]",
                        "Forcing room type: optarg was (%s)",
                        G_DEBUG_ROOMTYPE_ARG);
                int setroom_debug = 0;
                for (int rc = 0;
                     (rc < ROOM_CLASS_MAX + 1) && (setroom_debug == 0); rc++) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Checking optarg (%s) for -R: (%s)", optarg,
                            stringFromRoom(rc));
                    if ((strcmp(G_DEBUG_ROOMTYPE_ARG, stringFromRoom(rc)) == 0)) {
                        log_tag("debug_log.txt", "[DEBUG]",
                                "Match on optarg (%s), setting G_DEBUG_ROOMTYPE to (%i).",
                                stringFromRoom(rc), rc);
                        G_DEBUG_ROOMTYPE = rc;
                        setroom_debug = 1;
                    }
                }
                if (setroom_debug == 0) {
                    log_tag("debug_log.txt", "[ERROR]",
                            "Invalid optarg for -R flag: {%s}.",
                            G_DEBUG_ROOMTYPE_ARG);
                    fprintf(stderr,
                            "[ERROR]    Incorrect -R \"roomType\" arg: {%s}.\n",
                            G_DEBUG_ROOMTYPE_ARG);
                    fprintf(stderr, "[ERROR]    Run \"%s -h\" for help.\n",
                            kls_progname);
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                };
            }

        }
        log_tag("debug_log.txt", "[DEBUG]", "Done getopt.");

        // Clear screen and print title, wait for user to press enter
        int clearres = system("clear");
        log_tag("debug_log.txt", "[DEBUG]",
                "gameloop() system(\"clear\") res was (%i)", clearres);
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
        printf("\t\t\t\t\t\t\tncurses build\n");
        printf("\t\t\t\t\t\t");
        printFormattedVersion(whoami);
        int scanfres = scanf("%c", &c);
        log_tag("debug_log.txt", "[DEBUG]", "gameloop() scanf() res was (%i)",
                scanfres);

        // Parse positional arguments
        //for (int i = optind; i < argc; i++) {
        // Handle positional arguments
        Path *path = NULL;
        Fighter *player = NULL;

        clock_t start_time = clock(), diff_time;

        // Prepare the fighter frames
        char fighter_sprites[CLASSESMAX + 1][MAXFRAMES][MAXROWS][MAXCOLS];

        char static_path[500];

        // Set static_path value to the correct static dir path
        resolve_staticPath(static_path);

        /*
         * Legacy code for loading animations from an s4c-file.
         char fighter_filename[600];
         FILE* fighter_sprite_file;
         */
        for (int i = 0; i < CLASSESMAX + 1; i++) {

            int n_load_frames = 60;
            int n_load_rows = 17;
            int n_load_cols = 17;

            switch (i) {
            case Knight: {
                s4c_copy_animation(knight_tapis, fighter_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Mage: {
                s4c_copy_animation(mage_spark, fighter_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Archer: {
                s4c_copy_animation(archer_drop, fighter_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Assassin: {
                s4c_copy_animation(assassin_poof, fighter_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected fighterclass index while loading animation for class (%i): [%s]",
                        i, stringFromClass(i));
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

            log_tag("debug_log.txt", "[PREP]",
                    "Copied animation from default matrix vector for: [%s] with dimensions: [%i][%i][%i].",
                    stringFromClass(i), n_load_frames, n_load_rows,
                    n_load_cols);

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
        char enemy_sprites[ENEMYCLASSESMAX + 1][MAXFRAMES][MAXROWS][MAXCOLS];
        for (int i = 0; i < (ENEMYCLASSESMAX + 1); i++) {
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
                s4c_copy_animation(imp_fireball, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Zombie: {
                s4c_copy_animation(zombie_walk, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Troll: {
                s4c_copy_animation(troll_club, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Ghost: {
                s4c_copy_animation(ghost_spell, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Boar: {
                s4c_copy_animation(boar_scream, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Mummy: {
                s4c_copy_animation(mummy_shuffle, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Goblin: {
                s4c_copy_animation(goblin_shoot, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Werewolf: {
                s4c_copy_animation(werewolf_transform, enemy_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected enemy index while loading animation for enemy (%i): [%s]",
                        i, stringFromEClass(i));
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

            log_tag("debug_log.txt", "[PREP]",
                    "Copied animation from default matrix vector for: [%s] with dimensions: [%i][%i][%i].",
                    stringFromEClass(i), n_load_frames, n_load_rows,
                    n_load_cols);

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
        char boss_sprites[BOSSCLASSESMAX + 1][MAXFRAMES][MAXROWS][MAXCOLS];
        for (int i = 0; i < (BOSSCLASSESMAX + 1); i++) {
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
                s4c_copy_animation(bluetroll_wonder, boss_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Headless_Ninja: {
                s4c_copy_animation(headlessninja_throw, boss_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Crawling_Dude: {
                s4c_copy_animation(crawlingdude_crawl, boss_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Sr_Warthog: {
                s4c_copy_animation(srwarthog_square, boss_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            case Doppelganger: {
                s4c_copy_animation(knight_tapis, boss_sprites[i],
                                   n_load_frames, n_load_rows, n_load_cols);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "Unexpected boss index while loading animation for boss (%i): [%s]",
                        i, stringFromBossClass(i));
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

            log_tag("debug_log.txt", "[PREP]",
                    "Copied animation from default matrix vector for: [%s] with dimensions: [%i][%i][%i].",
                    stringFromBossClass(i), n_load_frames, n_load_rows,
                    n_load_cols);

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
        }			//End loading boss animations
        //End of loading animations
        clock_t animationload_time = clock() - start_time;
        double time_spent_loading_animations =
            (double)animationload_time / CLOCKS_PER_SEC;
        log_tag("debug_log.txt", "[DEBUG]",
                "Animation loading took %0.7f seconds.",
                time_spent_loading_animations);

        WINDOW* screen = initscr();
        bool screen_is_big_enough = false;
        int screen_rows = 0;
        int screen_cols = 0;
        //TODO: factor this out to ensure the check is done for on any gien screen draw.
        while (!screen_is_big_enough) {
            getmaxyx(screen, screen_rows, screen_cols);
            if (screen_rows >= HLPD_MIN_SCREEN_ROWS && screen_cols >= HLPD_MIN_SCREEN_COLS) {
                screen_is_big_enough = true;
                clear();
                refresh();
            } else if (screen_rows >= HLPD_DEFAULT_SCREEN_ROWS && screen_cols >= HLPD_MIN_SCREEN_COLS) {
                mvwprintw(screen, 0, 0, "%s", "Current screen is too small to see battle notifications.");
                mvwprintw(screen, 1, 0, "%s", "Enlarge vertically to fit it.");
                refresh();
            } else {
                mvwprintw(screen, 0, 0, "%s", "Screen too small, please resize.");
                refresh();
            }
        }
        GameScreen* gamescreen = (GameScreen*) KLS_PUSH_TYPED(default_kls, GameScreen, HR_Gamescreen, "Main GameScreen", "Wrap stdscr");
        gamescreen->colors = COLORS;
        gamescreen->color_pairs = COLOR_PAIRS;
        gamescreen->cols = COLS;
        gamescreen->rows = LINES;
        gamescreen->escape_delay = ESCDELAY;
        gamescreen->tabsize = TABSIZE;

        gamescreen->win = screen;
        ITEM **savepick_items;
        MENU *savepick_menu;
        WINDOW *savepick_menu_win;
        WINDOW *savepick_side_win;
        char current_save_path[300];	//Will hold picked path

        Koliseo_Temp *savepick_kls = kls_temp_start(temporary_kls);

        //Declare turnOP_args
        Room *fakeroom = NULL;
        Enemy *fakeenemy = NULL;
        Boss *fakeboss = NULL;
        FILE *fakesavefile = NULL;
        WINDOW *fakenotifywin = NULL;
        Gamestate *fakegmst = NULL;
        foeTurnOption_OP fake_foe_op = FOE_OP_INVALID;
        skillType fake_skill = -1;
        turnOP_args *savepick_turn_args =
            init_turnOP_args(fakegmst, player, path, fakeroom, load_info,
                             fakeenemy, fakeboss, fakesavefile, fakenotifywin,
                             savepick_kls, fake_foe_op, fake_skill);
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
        savepick_items = (ITEM **) calloc(savepick_n_choices, sizeof(ITEM *));
        for (int i = 0; i < savepick_n_choices; i++) {
            savepick_items[i] =
                new_item(savepick_choices[i], savepick_choices[i]);
        }
        savepick_items[savepick_n_choices - 1] = (ITEM *) NULL;

        /* Create menu */
        savepick_menu = new_menu((ITEM **) savepick_items);

        /* Set description off */
        menu_opts_off(savepick_menu, O_SHOWDESC);

        /* Create the window to be associated with the menu */
        savepick_menu_win = newwin(11, 16, 5, 35);
        keypad(savepick_menu_win, TRUE);

        /* Set main window and sub window */
        set_menu_win(savepick_menu, savepick_menu_win);
        set_menu_sub(savepick_menu, derwin(savepick_menu_win, 4, 14, 4, 1));
        set_menu_format(savepick_menu, 4, 1);

        /* Set menu mark to the string " >  " */
        set_menu_mark(savepick_menu, " >  ");

        /* Print a border around main menu window */
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
        scrollok(savepick_side_win, TRUE);
        wprintw(savepick_side_win, "  \nhelapordo");
        wprintw(savepick_side_win, "  \n  build: %s", helapordo_build_string);
        wprintw(savepick_side_win, "  \n  using: s4c-animate v%s",
                S4C_ANIMATE_VERSION);
        wprintw(savepick_side_win, "  \n  using: koliseo v%s",
                KOLISEO_API_VERSION_STRING);
        wprintw(savepick_side_win, "  \n  using: ncurses v%s", NCURSES_VERSION);
#ifdef ANVIL__helapordo__
#ifndef INVIL__helapordo__HEADER__
        wprintw(savepick_side_win, "  \nBuilt with: amboso v%s",
                ANVIL__API_LEVEL__STRING);
#else
        wprintw(savepick_side_win, "  \nBuilt with: invil v%s",
                INVIL__VERSION__STRING);
        wprintw(savepick_side_win, "  \nVersion Info: %.8s",
                get_ANVIL__VERSION__DESC__());
        const char* anvil_date = get_ANVIL__VERSION__DATE__();
        char* anvil_date_end;
#ifndef _WIN32
        time_t anvil_build_time = strtol(anvil_date, &anvil_date_end, 10);
#else
        time_t anvil_build_time = strtoll(anvil_date, &anvil_date_end, 10);
#endif //_WIN32

        if (anvil_date_end == anvil_date) {
            log_tag("debug_log.txt", "ERROR", "anvil date was invalid");
        } else {
            char build_time_buff[20] = {0};
            struct tm* build_time_tm = localtime(&anvil_build_time);

            if (build_time_tm == NULL) {
                log_tag("debug_log.txt", "ERROR", "localtime() failed");
            } else {
                strftime(build_time_buff, 20, "%Y-%m-%d %H:%M:%S", build_time_tm);
                wprintw(savepick_side_win, "  \nDate: %s", build_time_buff);
            }
        }
#endif // INVIL__helapordo__HEADER__
#else
        wprintw(savepick_side_win, "  \nBuilt without anvil");
#endif // ANVIL__helapordo__
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
            init_s4c_color_pair(&palette[i], 9 + i);
        }
        log_tag("debug_log.txt","[DEBUG]","%s():    Updating gamescreen->colors and colorpairs after init_s4c_color_pair() loop.", __func__);
        gamescreen->colors = COLORS;
        gamescreen->color_pairs = COLOR_PAIRS;

        while (!savepick_picked
               && (pickchar = wgetch(savepick_menu_win)) != KEY_F(1)) {
            switch (pickchar) {
            case KEY_DOWN: {
                menu_driver(savepick_menu, REQ_DOWN_ITEM);
            }
            break;
            case KEY_UP: {
                menu_driver(savepick_menu, REQ_UP_ITEM);
            }
            break;
            case KEY_LEFT: {	/*Left option pick */
                ITEM *cur;
                cur = current_item(savepick_menu);
                savepick_choice = getTurnChoice((char *)item_name(cur));
                log_tag("debug_log.txt", "[DEBUG]",
                        "Left on choice: [ %s ] value (%i)", item_name(cur),
                        savepick_choice);
                if (savepick_choice == NEW_GAME) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Should do something");
                }
            }
            break;
            case KEY_RIGHT: {	/*Right option pick */
                ITEM *cur;
                cur = current_item(savepick_menu);
                savepick_choice = getTurnChoice((char *)item_name(cur));
                log_tag("debug_log.txt", "[DEBUG]",
                        "Right on choice: [ %s ] value (%i)",
                        item_name(cur), savepick_choice);
                if (savepick_choice == NEW_GAME) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Should do something");
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
            case 10: {	/* Enter */
                savepick_picked = 1;
                ITEM *cur;

                //move(18,47);
                //clrtoeol();
                cur = current_item(savepick_menu);
                //mvprintw(18, 47, "Item selected is : %s", item_name(cur));
                savepick_choice = getTurnChoice((char *)item_name(cur));
                pos_menu_cursor(savepick_menu);
                refresh();
            }
            break;
            case 'q': {
                log_tag("debug_log.txt", "[DEBUG]",
                        "Player used q to quit from savepick menu.");
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
                log_tag("debug_log.txt", "[DEBUG]",
                        "Saveslot index picked: [%i]", picked_saveslot_index);
                sprintf(current_save_path, "%s", default_saveslots[picked_saveslot_index].save_path);	//Update saveslot_path value
                //TODO
                //Get picked_slot with a curses menu.
                //int picked_slot = handle_pickSave();
                //sprintf(current_save_path,default_saveslots[picked_slot].save_path);
                //TODO
                //By default we expect the user to press new game, no action needed?
                log_tag("debug_log.txt", "[DEBUG]",
                        "Running new game from savepick menu");
                turnOP(OP_NEW_GAME, savepick_turn_args, default_kls,
                       savepick_kls);
            } else if (savepick_choice == LOAD_GAME) {
                int picked_saveslot_index = get_saveslot_index();
                log_tag("debug_log.txt", "[DEBUG]",
                        "Saveslot index picked: [%i]", picked_saveslot_index);
                sprintf(current_save_path, "%s", default_saveslots[picked_saveslot_index].save_path);	//Update saveslot_path value
                //TODO
                //Get picked_slot with a curses menu.
                //int picked_slot = handle_pickSave();
                //sprintf(current_save_path,default_saveslots[picked_slot].save_path);
                //ATM we expect a single save.
                //Setting this to 0 is the only thing we expect here, the actual load is done later.
                load_info->is_new_game = 0;
                log_tag("debug_log.txt", "[DEBUG]",
                        "Set load value: load_info->is_new_game == (%i)",
                        load_info->is_new_game);
                turnOP(OP_LOAD_GAME, savepick_turn_args, default_kls,
                       savepick_kls);
                //TODO
                //Select which game to load, by preparing the necessary handles to code below (correct savefile/name, for now)
            } else if (savepick_choice == QUIT) {
                //TODO
                //We can quit, I guess.
                log_tag("debug_log.txt", "[DEBUG]",
                        "Savepick menu: doing exit(%i)", EXIT_SUCCESS);
                // Unpost menu and free all the memory taken up
                unpost_menu(savepick_menu);
                free_menu(savepick_menu);
                log_tag("debug_log.txt", "[FREE]", "Freed savepick menu");
                for (int k = 0; k < savepick_n_choices; k++) {
                    free_item(savepick_items[k]);
                    log_tag("debug_log.txt", "[FREE]",
                            "Freed %i savepick menu item", k);
                }

                delwin(savepick_menu_win);
                endwin();
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            } else if (savepick_choice == TUTORIAL) {
                log_tag("debug_log.txt", "[DEBUG]", "Doing tutorial.");
                handleTutorial();
                exit(EXIT_SUCCESS);
            }
        }			//End while !savepick_picked

        //Free turnOP_args
        //free(savepick_turn_args);

        // Unpost menu and free all the memory taken up
        unpost_menu(savepick_menu);
        free_menu(savepick_menu);
        log_tag("debug_log.txt", "[FREE]", "Freed savepick menu");
        for (int k = 0; k < savepick_n_choices; k++) {
            free_item(savepick_items[k]);
            log_tag("debug_log.txt", "[FREE]", "Freed %i savepick menu item",
                    k);
        }

        delwin(savepick_menu_win);
        endwin();
        log_tag("debug_log.txt", "[DEBUG]",
                "Ended window mode for savepick menu");

        kls_temp_end(savepick_kls);

        //Flush the terminal
        int clrres = system("clear");
        log_tag("debug_log.txt", "[DEBUG]",
                "gameloop() system(\"clear\") after savepick res was (%i)",
                clrres);

        //By now, we expect load_info->is_new_game to be set to 0 or 1.
        log_tag("debug_log.txt", "[DEBUG]",
                "  Checking is_new_game:  load_info->is_new_game == (%i)",
                load_info->is_new_game);

        Koliseo_Temp *gamestate_kls = kls_temp_start(temporary_kls);

        if (load_info->is_new_game) {	// We prepare path and fighter
            path = randomise_path(rand(), default_kls, current_save_path);
            path->loreCounter = -1;

            kls_log(default_kls, "DEBUG", "Prepping Fighter");
            player =
                (Fighter *) KLS_PUSH_TYPED(default_kls, Fighter, HR_Fighter,
                                           "Fighter", "Fighter");

            int optTot = optind;

            getParams(argc, argv, player, path, optTot, default_kls);
            initPlayerStats(player, path, default_kls);
        } else {		//Handle loading of gamestate

            //Declar turnOP_args
            Room *fakeroom = NULL;
            Enemy *fakeenemy = NULL;
            Boss *fakeboss = NULL;
            FILE *fakesavefile = NULL;
            WINDOW *fakenotifywin = NULL;
            Gamestate *fakegmst = NULL;
            foeTurnOption_OP fake_foe_op = FOE_OP_INVALID;
            skillType fake_skill = -1;
            turnOP_args *loading_room_turn_args =
                init_turnOP_args(fakegmst, player, path, fakeroom, load_info,
                                 fakeenemy, fakeboss, fakesavefile,
                                 fakenotifywin, gamestate_kls, fake_foe_op,
                                 fake_skill);
            FILE *save_file;
            char path_to_savefile[1000];
            char static_path[500];
            char savefile_name[300];

            //Copy current_save_path
            sprintf(savefile_name, "%s", current_save_path);

            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            sprintf(path_to_savefile, "%s/%s", static_path, savefile_name);

            save_file = fopen(path_to_savefile, "r");
            if (!save_file) {
                //User error
                fprintf(stderr,
                        "[ERROR]    Can't open savefile for loading game.\n");
                fprintf(stderr, "[ERROR]    Expected at path [%s].\n",
                        path_to_savefile);
                //Debug error
                log_tag("debug_log.txt", "[ERROR]",
                        "Could not load savefile at (%s)", path_to_savefile);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            //Update loading_room_turn_args->save_file pointer
            loading_room_turn_args->save_file = save_file;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Assigned loading_room_turn_args->save_file: path [%s]",
                    path_to_savefile);

            //Read save type
            //
            saveType loaded_save_type = -1;
            loaded_save_type = read_saveType(save_file);
            log_tag("debug_log.txt", "[TURNOP]", "Read saveType, was [%s].",
                    stringFrom_saveType(loaded_save_type));

            if (loaded_save_type == -1) {
                log_tag("debug_log.txt", "[ERROR]",
                        "Failed setting loaded_save_type. Quitting.");
                fprintf(stderr, "[ERROR]    Failed setting a save type.");

                exit(EXIT_FAILURE);
            }

            load_info->save_type = loaded_save_type;

            log_tag("debug_log.txt", "[TURNOP]",
                    "Assigned load_info->save_type: [%s]",
                    stringFrom_saveType(load_info->save_type));

            path = randomise_path(rand(), default_kls, current_save_path);
            kls_log(default_kls, "DEBUG", "Prepping Loady Fighter");
            player =
                (Fighter *) KLS_PUSH_TYPED(default_kls, Fighter, HR_Fighter,
                                           "Fighter", "Loady Fighter");
            player->class = Knight;

            strcpy(player->name, "Loady");

            //Update loading_room_turn_args->actor pointer
            loading_room_turn_args->actor = player;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Assigned Fighter [%s]. loading_room_turn_args->actor->name: [%s]",
                    player->name, loading_room_turn_args->actor->name);

            kls_log(default_kls, "DEBUG", "Prepping Loady Wincon");
            Wincon *w =
                (Wincon *) KLS_PUSH_TYPED(default_kls, Wincon, HR_Wincon,
                                          "Wincon", "Loady Wincon");
            w->class = FULL_PATH;
            initWincon(w, path, w->class);
            initPlayerStats(player, path, default_kls);
            path->win_condition = w;

            if (load_info->save_type == ENEMIES_SAVE) {

                kls_log(default_kls, "DEBUG", "Prepping Loady Enemy");
                load_info->loaded_enemy =
                    (Enemy *) KLS_PUSH_TYPED(default_kls, Enemy, HR_Enemy,
                                             "Enemy", "Loaded Enemy");
                //FIXME: the structs related to loaded enemy are not loaded on default_kls
                prepareRoomEnemy(load_info->loaded_enemy, 1, 3, 1,
                                 gamestate_kls);

                //Update loading_room_turn_args->enemy pointer
                loading_room_turn_args->enemy = load_info->loaded_enemy;
                log_tag("debug_log.txt", "[TURNOP]",
                        "Assigned load_info->loaded_enemy->class == [%s]. loading_room_turn_args->loaded_enemy->class == [%s]",
                        stringFromEClass(load_info->loaded_enemy->class),
                        stringFromEClass(loading_room_turn_args->enemy->class));
            }
            //Update loading_room_turn_args->path pointer
            loading_room_turn_args->path = path;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Assigned loading_room_turn_args->path == [path] (len %i)",
                    path->length);

            switch (load_info->save_type) {
            case ENEMIES_SAVE: {
                log_tag("debug_log.txt", "[TURNOP]",
                        "Doing OP_LOAD_ENEMYROOM.");
                //int* loadinfo_totfoes = &(load_info->total_foes);
                //FIXME: the structs related to loaded enemy are not loaded on default_kls
                OP_res load_op_result =
                    turnOP(OP_LOAD_ENEMYROOM, loading_room_turn_args,
                           default_kls, gamestate_kls);
                log_tag("debug_log.txt", "[TURNOP]",
                        "OP_LOAD_ENEMYROOM:  result was [%s].",
                        stringFrom_OP_res(load_op_result));
                log_tag("debug_log.txt", "[FREE]",
                        "Freed loading_room_turn_args. Load result was [%s].",
                        stringFrom_OP_res(load_op_result));
                //free(loading_room_turn_args);
            }
            break;
            case HOME_SAVE: {
                log_tag("debug_log.txt", "[TURNOP]",
                        "Doing OP_LOAD_HOMEROOM.");
                //int* loadinfo_totfoes = &(load_info->total_foes);
                //FIXME: the structs related to loaded enemy are not loaded on default_kls
                OP_res load_op_result =
                    turnOP(OP_LOAD_HOMEROOM, loading_room_turn_args,
                           default_kls, gamestate_kls);
                log_tag("debug_log.txt", "[TURNOP]",
                        "OP_LOAD_HOMEROOM:  result was [%s].",
                        stringFrom_OP_res(load_op_result));
                //log_tag("debug_log.txt","[FREE]","Freed loading_room_turn_args. Load result was [%s].",stringFrom_OP_res(load_op_result));
                load_info->done_loading = 1;
                log_tag("debug_log.txt", "[PREP]",
                        "Set load_info->done_loading to 1.");
                //free(loading_room_turn_args);
            }
            break;
            default: {
                log_tag("debug_log.txt", "[ERROR]",
                        "gameloop():  Unexpected save type, value was (%i).",
                        (int)load_info->save_type);
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

        int *loreCounter = &(path->loreCounter);
        log_tag("debug_log.txt", "[DEBUG]", "loreCounter == (%i)",
                *loreCounter);

        if (GAMEMODE == Story) {

            /*
             * TODO
             * Remove me
             * Legacy code to load lores from a text file.
             int loreKind = 0; //rand() % LORES_MAX;
             */

            if (load_info->is_new_game) {
                log_tag("debug_log.txt", "[FIXME]",
                        "loreCounter was (%i), setting it to 0.", *loreCounter);
                *loreCounter = 0;	//We must set the counter before going forward
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
            log_tag("debug_log.txt", "[WARN]",
                    "GAMEMODE is not Story. Value was: (%i)", GAMEMODE);
        }

        //Set consumables sprites
        for (int i = 0; i < CONSUMABLESMAX + 1; i++) {
            setConsumableSprite((Consumable *) player->consumablesBag[i]);
            log_tag("debug_log.txt", "[PREP]", "Set sprite for %s",
                    stringFromConsumables(i));
        }
        log_tag("debug_log.txt", "[DEBUG-PREP]",
                "Done setting sprites for Consumables.");
        //Set artifact sprites
        for (int i = 0; i < ARTIFACTSMAX + 1; i++) {
            setArtifactSprite(player->artifactsBag[i]);
            log_tag("debug_log.txt", "[PREP]", "Set sprite for %s",
                    stringFromArtifacts(i));
        }
        log_tag("debug_log.txt", "[PREP]",
                "Done setting sprites for Artifacts.");
        //Set base equips sprites...
        for (int i = 0; i < EQUIPSMAX + 1; i++) {
            setEquipSprite(&equips[i]);
            log_tag("debug_log.txt", "[PREP]", "Set sprite for %s",
                    stringFromEquips(i));
        }
        log_tag("debug_log.txt", "[PREP]", "Done setting sprites for Equips.");

        if (load_info->is_new_game) {
            log_tag("debug_log.txt", "[PREP]", "New game from scratch.");
        } else {
            log_tag("debug_log.txt", "[PREP]", "New game from loading.");
        }
        log_tag("debug_log.txt", "[DEBUG]", "Name:    %s", player->name);
        log_tag("debug_log.txt", "[DEBUG]", "Class:    %s",
                stringFromClass(player->class));
        log_tag("debug_log.txt", "[DEBUG]", "Gamemode:    %s",
                stringFromGamemode(GAMEMODE));

        //purple();
        //printStats(player);
        //white();

        int roomsDone = load_info->is_new_game ? 1 : loaded_roomindex;
        OP_res res = OP_RES_NO_DMG;
        int roadFork_value = -1;	//0 may be used as a value, so

        Wincon *win_con = path->win_condition;
        log_tag("debug_log.txt", "[DEBUG]", "Wincon:    %s\n",
                stringFromWinconClass(win_con->class));

        //int refresh_artifact_wincon = 0;
        if (load_info->save_type == ENEMIES_SAVE) {
            load_info->done_loading = 0;
            log_tag("debug_log.txt", "[DEBUG-PREP]",
                    "Set load_info->done_loading to 0.");
        }
        log_tag("debug_log.txt", "[DEBUG-PREP]", "Prepping done.\n");
        log_tag("debug_log.txt", "[DEBUG]", "Starting wincon loop.\n");

        diff_time = clock() - start_time;
        int time_spent = diff_time * 1000 / CLOCKS_PER_SEC;
        //sprintf(msg,"[DEBUG]    Prep took %0.7f seconds.\n",time_spent);
        log_tag("debug_log.txt", "[DEBUG]", "Prep took %d s, %d ms.",
                time_spent / 1000, time_spent % 1000);

        Gamestate *gamestate =
            KLS_PUSH_TYPED(default_kls, Gamestate, HR_Gamestate, "Gamestate",
                           "Gamestate");
        init_Gamestate(gamestate, start_time, player->stats, path->win_condition, path,
                       player, GAMEMODE, gamescreen);
        if (gamestate->gamemode == Rogue) {
            //Note: different lifetime than gamestate
            //NO. The update_gamestate call is instead performed later.
            //Floor* current_floor = KLS_PUSH_T_TYPED(gamestate_kls,Floor,1,HR_Floor,"Floor","Init Curr floor");
            //NO. We pass NULL now.
            update_Gamestate(gamestate, 1, HOME, roomsDone, -1, NULL);
        } else {
            update_Gamestate(gamestate, 1, HOME, roomsDone, -1, NULL);
        }
        log_tag("debug_log.txt", "[DEBUG]", "Initialised Gamestate.");
        dbg_Gamestate(gamestate);

        if (GAMEMODE == Story || GAMEMODE == Standard) {

            //Loop till wincon reached

            while (win_con->current_val < win_con->target_val) {

                //Flush the terminal
                int loop_clrres = system("clear");
                log_tag("debug_log.txt", "[DEBUG]",
                        "gameloop() system(\"clear\") in wincon loop, res: (%i)",
                        loop_clrres);

                //Check if we have to update the wincon value
                if (path->win_condition->class == ALL_ARTIFACTS) {
                    path->win_condition->current_val =
                        player->stats->artifactsfound;
                    //Are we forced to do one more room?
                }

                int enemyTotal = -1;
                roomClass room_type = -1;

                if (!(load_info->is_new_game) && !(load_info->done_loading)
                    && (load_info->save_type == ENEMIES_SAVE)) {
                    enemyTotal = loaded_roomtotalenemies;
                }

                kls_log(temporary_kls, "DEBUG",
                        "Prepping Room for Story Gamemode. roomsDone=(%i)",
                        roomsDone);
                Room *current_room =
                    (Room *) KLS_PUSH_T_TYPED(gamestate_kls, Room, HR_Room,
                                              "Room", "Story Room");

                current_room->index = roomsDone;
                setRoomType(path, &roadFork_value, &room_type, roomsDone);
                log_tag("debug_log.txt", "[ROOM]",
                        "Set Room #%i type:    (%s)\n", roomsDone,
                        stringFromRoom(room_type));

                initRoom(current_room, player, roomsDone, room_type, enemyTotal,
                         load_info, gamestate_kls);
                log_tag("debug_log.txt", "[ROOM]", "Init Room #%i:    (%s)\n",
                        roomsDone, stringFromRoom(room_type));

                start_color();
                int colorCheck = has_colors();

                if (colorCheck == FALSE) {
                    fprintf(stderr, "Terminal can't use colors, abort.\n");
                    exit(S4C_ERR_TERMCOLOR);
                }

                colorCheck = can_change_color();

                if (colorCheck == FALSE) {
                    fprintf(stderr, "Terminal can't change colors, abort.\n");
                    exit(S4C_ERR_TERMCHANGECOLOR);
                }
                for (int i = 0; i < PALETTE_S4C_H_TOTCOLORS; i++) {
                    init_s4c_color_pair(&palette[i], 9 + i);
                }
                cbreak();
                noecho();
                keypad(stdscr, TRUE);

                //Check if we need to display a story prompt
                if (GAMEMODE == Story && (roomsDone == 1 || room_type == BOSS)) {
                    displayLore(lore_strings, *loreCounter);
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

                WINDOW *door_win;
                //initscr();
                clear();
                refresh();

                int reps = 1;
                int frametime = 27;
                int num_frames = 60;
                int frame_height = 22;
                int frame_width = 22;
                door_win = newwin(frame_height + 1, frame_width + 1, 0, 25);

                char door_sprites[MAXFRAMES][MAXROWS][MAXCOLS];

                s4c_copy_animation(enter_door, door_sprites, num_frames,
                                   frame_height, frame_width);

                log_tag("debug_log.txt", "[PREP]",
                        "Copied animation from matrix vector for enter_door with dimensions: [%i][%i][%i].",
                        num_frames, frame_height, frame_width);

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

                int result =
                    s4c_animate_sprites_at_coords(door_sprites, door_win, reps,
                                                  frametime, num_frames,
                                                  frame_height, frame_width, 0,
                                                  0);
                log_tag("debug_log.txt", "[DEBUG]", "animate() result was (%i)",
                        result);
                wclear(door_win);
                wrefresh(door_win);
                delwin(door_win);
                endwin();

                update_Gamestate(gamestate, 1, current_room->class, roomsDone,
                                 -1, NULL);

                if (current_room->class == HOME) {
                    res =
                        handleRoom_Home(gamestate, current_room, roomsDone,
                                        path, player, load_info,
                                        fighter_sprites, default_kls,
                                        gamestate_kls);
                } else if (current_room->class == ENEMIES) {
                    res =
                        handleRoom_Enemies(gamestate, current_room, roomsDone,
                                           path, player, load_info,
                                           enemy_sprites, fighter_sprites,
                                           default_kls, gamestate_kls);
                } else if (current_room->class == SHOP) {
                    //FIXME: does shop require usage of gameloop kls?
                    res =
                        handleRoom_Shop(current_room, roomsDone, path, player,
                                        default_kls, gamestate_kls);
                } else if (current_room->class == BOSS) {
                    res =
                        handleRoom_Boss(gamestate, current_room, roomsDone,
                                        path, player, load_info, boss_sprites,
                                        fighter_sprites, default_kls,
                                        gamestate_kls);
                } else if (current_room->class == TREASURE) {
                    res =
                        handleRoom_Treasure(current_room, roomsDone, path,
                                            player, default_kls, gamestate_kls);
                } else if (current_room->class == ROADFORK) {
                    res =
                        handleRoom_Roadfork(current_room, &roadFork_value,
                                            roomsDone, path, player);
                } else {
                    log_tag("debug_log.txt", "[ERROR]",
                            "Unexpected current_room->class value: [%i] [%s]",
                            current_room->class,
                            stringFromRoom(current_room->class));
                    //freeRoom(current_room);
                    log_tag("debug_log.txt", "[ERROR]",
                            "Freed current room, quitting program.");
                    exit(EXIT_FAILURE);
                }

                if (res == OP_RES_DEATH) {
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Room resulted in DEATH.\n");
                    //Free room memory
                    //freeRoom(current_room);
                    break;
                } else {
                    //Flush the terminal
                    int clrres = system("clear");
                    log_tag("debug_log.txt", "[DEBUG]",
                            "gameloop() system(\"clear\") res was (%i)",
                            clrres);

                    if (roadFork_value > 0) {
                        //TODO
                        //What is this?
                        lightYellow();
                        printStats(player);
                        lightGreen();
                        log_tag("debug_log.txt", "[ROADFORK?]",
                                "You completed room %i.", roomsDone);
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
            }			// Win condition loop

            // Clear default_kls
            //kls_clear(default_kls);

            //Got out of the loop with res not being DEATH; so i won
            if (res != OP_RES_DEATH) {	//I guess player and enemy were freed already?
                int clrres = system("clear");
                log_tag("debug_log.txt", "[DEBUG]",
                        "gameloop() 2 system(\"clear\") res was (%i)", clrres);
                handleStats(player);
                printf("\n\n\tYOU WON!\n\n");
                log_tag("debug_log.txt", "[DEBUG]", "Game won.");
                //Free default kls
                kls_log(default_kls, "DEBUG", "Freeing default KLS");
                kls_free(default_kls);
                log_tag("debug_log.txt", "[DEBUG-KLS]", "Freed default KLS");

                //Free temporary kls
                kls_log(temporary_kls, "DEBUG", "Freeing temporary KLS");
                kls_free(temporary_kls);
                log_tag("debug_log.txt", "[DEBUG-KLS]", "Freed temporary KLS");
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
            log_tag("debug_log.txt", "[DEBUG]", "End of wincon loop.\n");

        } else {		//Gamemode is not Story or Standard
            log_tag("debug_log.txt", "[DEBUG]", "Gamemode was [%i]", GAMEMODE);

            if (GAMEMODE == Rogue) {
                log_tag("debug_log.txt", "[DEBUG]", "Doing a Rogue run.");
                char static_path[500];

                // Set static_path value to the correct static dir path
                resolve_staticPath(static_path);

                /**
                 * Legacy code for reading palette.gpl. Was it ever needed at runtime?
                 * TODO check which tags may use the palette.gpl at runtime (other than trying to read it pointlessly).
                 *
                char path_to_palette[600];
                FILE* palette_file = NULL;
                char palette_name[50] = "palette.gpl";
                sprintf(path_to_palette, "%s/%s", static_path, palette_name);
                palette_file = fopen(path_to_palette, "r");
                if (palette_file == NULL) {
                    fprintf(stderr,
                            "Error: could not open palette file (%s/%s).\n",
                            static_path, palette_name);
                    exit(EXIT_FAILURE);
                }
                */

                WINDOW *floor_win;
                //TODO: store if we have done initsrc() or endwin(). Am sure we can get this from ncurses with some MACRO
                //initscr();
                clear();
                refresh();
                start_color();

                int colorCheck = has_colors();

                if (colorCheck == FALSE) {
                    fprintf(stderr, "Terminal can't use colors, abort.\n");
                    exit(S4C_ERR_TERMCOLOR);
                }

                colorCheck = can_change_color();

                if (colorCheck == FALSE) {
                    fprintf(stderr, "Terminal can't change colors, abort.\n");
                    exit(S4C_ERR_TERMCHANGECOLOR);
                }
                for (int i = 0; i < PALETTE_S4C_H_TOTCOLORS; i++) {
                    init_s4c_color_pair(&palette[i], 9 + i);
                }
                cbreak();
                noecho();
                keypad(stdscr, TRUE);

                cbreak();
                noecho();
                keypad(stdscr, TRUE);

                // Create the window
                floor_win = newwin(23, 24, 1, 2);
                wclear(floor_win);
                wrefresh(floor_win);
                keypad(floor_win, TRUE);

                /* Print a border around the windows and print a title */
                box(floor_win, 0, 0);
                wrefresh(floor_win);
                refresh();
                int res = -1;
                char msg[500];

                log_tag("debug_log.txt", "[DEBUG]", "Prepping current_floor.");
                kls_log(default_kls, "DEBUG", "Prepping current_floor.");
                Floor *current_floor =
                    (Floor *) KLS_PUSH_T_TYPED(gamestate_kls, Floor,
                                               HR_Floor, "Floor", "Floor");
                update_Gamestate(gamestate, 1, HOME, roomsDone, -1,
                                 current_floor);
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
                floor_random_walk(current_floor, center_x, center_y, 100, 1);	// Perform 100 steps of random walk, reset floor_layout if needed.
                //Random walk #2
                floor_random_walk(current_floor, center_x, center_y, 100, 0);	// Perform 100 more steps of random walk, DON'T reset floor_layout if needed.

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
                        path->win_condition->current_val =
                            player->stats->artifactsfound;
                        //Are we forced to do one more room?
                    }

                    int enemyTotal = -1;
                    roomClass room_type = -1;

                    if (!(load_info->is_new_game) && !(load_info->done_loading)
                        && (load_info->save_type == ENEMIES_SAVE)) {
                        enemyTotal = loaded_roomtotalenemies;
                    }

                    Room *current_room = NULL;

                    //Check if current room needs to be played
                    if (current_floor->roomclass_layout[current_x][current_y] !=
                        BASIC) {
                        kls_log(temporary_kls, "DEBUG",
                                "Prepping Room for Rogue Gamemode. roomsDone=(%i)",
                                roomsDone);
                        current_room =
                            (Room *) KLS_PUSH_T_TYPED(gamestate_kls, Room,
                                                      HR_Room, "Room", msg);

                        current_room->index = roomsDone;
                        //setRoomType(path, &roadFork_value, &room_type, roomsDone);

                        room_type =
                            current_floor->
                            roomclass_layout[current_x][current_y];
                        log_tag("debug_log.txt", "[ROOM]",
                                "Set Room #%i type:    (%s)\n", roomsDone,
                                stringFromRoom(room_type));

                        initRoom(current_room, player, roomsDone, room_type,
                                 enemyTotal, load_info, gamestate_kls);
                        log_tag("debug_log.txt", "[ROOM]",
                                "Init Room #%i:    (%s)\n", roomsDone,
                                stringFromRoom(room_type));

                        /*
                           //Check if we need to display a story prompt
                           if (GAMEMODE == Story && (roomsDone == 1 || room_type == BOSS)) {
                           displayLore(lore_strings,*loreCounter);
                           (*loreCounter)++;
                           }
                         */

                        //Play room animation

                        WINDOW *door_win;
                        //initscr();
                        clear();
                        refresh();
                        start_color();

                        int reps = 1;
                        int frametime = 27;
                        int num_frames = 60;
                        int frame_height = 22;
                        int frame_width = 22;
                        door_win =
                            newwin(frame_height + 1, frame_width + 1, 0, 25);

                        char door_sprites[MAXFRAMES][MAXROWS][MAXCOLS];

                        s4c_copy_animation(enter_door, door_sprites, num_frames,
                                           frame_height, frame_width);

                        log_tag("debug_log.txt", "[PREP]",
                                "Copied animation from matrix vector for enter_door with dimensions: [%i][%i][%i].",
                                num_frames, frame_height, frame_width);

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

                        int result =
                            s4c_animate_sprites_at_coords(door_sprites,
                                                          door_win, reps,
                                                          frametime, num_frames,
                                                          frame_height,
                                                          frame_width, 0, 0);
                        log_tag("debug_log.txt", "[DEBUG]",
                                "animate() result was (%i)", result);
                        wclear(door_win);
                        wrefresh(door_win);
                        delwin(door_win);
                        endwin();

                        update_Gamestate(gamestate, 1, current_room->class,
                                         current_room->index, -1,
                                         current_floor);

                        if (current_room->class == HOME) {
                            res =
                                handleRoom_Home(gamestate, current_room,
                                                roomsDone, path, player,
                                                load_info, fighter_sprites,
                                                default_kls, gamestate_kls);
                        } else if (current_room->class == ENEMIES) {
                            res =
                                handleRoom_Enemies(gamestate, current_room,
                                                   roomsDone, path, player,
                                                   load_info, enemy_sprites,
                                                   fighter_sprites, default_kls,
                                                   gamestate_kls);
                        } else if (current_room->class == SHOP) {
                            res =
                                handleRoom_Shop(current_room, roomsDone, path,
                                                player, default_kls,
                                                gamestate_kls);
                        } else if (current_room->class == BOSS) {
                            res =
                                handleRoom_Boss(gamestate, current_room,
                                                roomsDone, path, player,
                                                load_info, boss_sprites,
                                                fighter_sprites, default_kls,
                                                gamestate_kls);
                        } else if (current_room->class == TREASURE) {
                            res =
                                handleRoom_Treasure(current_room, roomsDone,
                                                    path, player, default_kls,
                                                    gamestate_kls);
                        } else if (current_room->class == ROADFORK) {
                            res =
                                handleRoom_Roadfork(current_room,
                                                    &roadFork_value, roomsDone,
                                                    path, player);
                        } else {
                            sprintf(msg,
                                    "Unexpected current_room->class value: [%i] [%s]",
                                    current_room->class,
                                    stringFromRoom(current_room->class));
                            log_tag("debug_log.txt", "[ERROR]", msg);
                            //freeRoom(current_room);
                            log_tag("debug_log.txt", "[ERROR]",
                                    "Freed current room, quitting program.");
                            exit(EXIT_FAILURE);
                        }

                        if (res == OP_RES_DEATH) {
                            log_tag("debug_log.txt", "[DEBUG]",
                                    "Room resulted in DEATH.");
                            //Free room memory
                            //freeRoom(current_room);
                            break;
                        } else {
                            //Flush the terminal
                            int clrres = system("clear");
                            log_tag("debug_log.txt", "[DEBUG]",
                                    "gameloop() system(\"clear\") res was (%i)",
                                    clrres);

                            if (roadFork_value > 0) {
                                //lightYellow();
                                //TODO
                                //What is this?
                                printStats(player);
                                //lightGreen();
                                log_tag("debug_logx.txt", "[ROADFORK?]",
                                        "You completed room %i.", roomsDone);
                                //white();
                            }
                            roomsDone++;

                            //Update stats
                            player->stats->roomscompleted++;

                            //Free room memory
                            //freeRoom(current_room);

                            //Update floor's roomclass layout for finished rooms which should not be replayed
                            switch (current_floor->
                                    roomclass_layout[current_x][current_y]) {
                            case ENEMIES: {
                                current_floor->
                                roomclass_layout[current_x][current_y] =
                                    BASIC;
                            }
                            break;
                            case BOSS: {
                                current_floor->
                                roomclass_layout[current_x][current_y] =
                                    BASIC;
                                floors_done++;
                                player->stats->floorscompleted++;
                                log_tag("debug_log.txt", "[DEBUG]",
                                        "Floors done: [%i]", floors_done);
                                //Check if we need to update the win condition
                                if (win_con->class == FULL_PATH) {
                                    win_con->current_val++;
                                }
                                // Reset gamestate_kls
                                kls_temp_end(gamestate_kls);
                                gamestate_kls =
                                    kls_temp_start(temporary_kls);

                                current_floor =
                                    (Floor *)
                                    KLS_PUSH_T_TYPED(gamestate_kls, Floor,
                                                     HR_Floor, "Floor",
                                                     "Floor");
                                update_Gamestate(gamestate, 1, HOME,
                                                 roomsDone, -1,
                                                 current_floor);

                                //Regenerate floor
                                log_tag("debug_log.txt", "[DEBUG]",
                                        "Beaten a boss, regenerating current floor.");
                                // Init
                                init_floor_layout(current_floor);
                                //Set center as filled
                                current_floor->
                                floor_layout[center_x][center_y] = 1;
                                //Init floor rooms
                                init_floor_rooms(current_floor);
                                //Random walk #1
                                floor_random_walk(current_floor, center_x, center_y, 100, 1);	// Perform 100 steps of random walk, reset floor_layout if needed.
                                //Random walk #2
                                floor_random_walk(current_floor, center_x, center_y, 100, 0);	// Perform 100 more steps of random walk, DON'T reset floor_layout if needed.
                                //Set floor explored matrix
                                load_floor_explored(current_floor);
                                //Set room types
                                floor_set_room_types(current_floor);

                                //Center current coords
                                current_x = center_x;
                                current_y = center_y;
                                continue;	//Check win condition for loop

                            }
                            break;
                            case SHOP: {
                                current_floor->
                                roomclass_layout[current_x][current_y] =
                                    BASIC;
                            }
                            break;
                            case TREASURE: {
                                current_floor->
                                roomclass_layout[current_x][current_y] =
                                    BASIC;
                            }
                            break;
                            case HOME: {
                                //We leave it available
                                log_tag("debug_log.txt", "[DEBUG]",
                                        "Skipping reset of roomclass for HOME room");
                            }
                            break;
                            default: {
                                log_tag("debug_log.txt", "[ERROR]",
                                        "Unexpected roomclass value in Rogue loop: [%i] [%s]",
                                        current_floor->
                                        roomclass_layout[current_x]
                                        [current_y],
                                        stringFromRoom(current_floor->
                                                       roomclass_layout
                                                       [current_x]
                                                       [current_y]));
                                kls_free(default_kls);
                                kls_free(temporary_kls);
                                exit(EXIT_FAILURE);
                            }
                            break;
                            }
                        }
                    } else {
                        log_tag("debug_log.txt", "[DEBUG]",
                                "Current room class was [%s] (val: %i), not playable.",
                                stringFromRoom(current_floor->
                                               roomclass_layout[current_x]
                                               [current_y]),
                                current_floor->
                                roomclass_layout[current_x][current_y]);
                    }

                    //Draw current FOV
                    draw_floor_view(current_floor, current_x, current_y,
                                    floor_win);
                    //Take a step and update screen
                    move_update(gamestate, current_floor, &current_x,
                                &current_y, floor_win, path, player,
                                current_room, load_info, default_kls,
                                gamestate_kls);
                }		// Win condition loop

                //FIXME: do we need this?
                //kls_temp_end(gamestate_kls);
                // Clear default_kls
                //kls_clear(default_kls);

                //Got out of the loop with res not being DEATH; so i won
                if (res != OP_RES_DEATH) {	//I guess player and enemy were freed already?
                    int clrres = system("clear");
                    //TODO
                    //What is this?
                    log_tag("debug_log.txt", "[DEBUG]",
                            "gameloop() 2 system(\"clear\") res was (%i)",
                            clrres);
                    handleStats(player);
                    printf("\n\n\tYOU WON!\n\n");
                    log_tag("debug_log.txt", "[DEBUG]", "Game won.");
                    //Free default kls
                    kls_log(default_kls, "DEBUG", "Freeing default KLS");
                    kls_free(default_kls);
                    log_tag("debug_log.txt", "[DEBUG-KLS]",
                            "Freed default KLS");

                    //Free temporary kls
                    kls_log(temporary_kls, "DEBUG", "Freeing temporary KLS");
                    kls_free(temporary_kls);
                    log_tag("debug_log.txt", "[DEBUG-KLS]",
                            "Freed temporary KLS");
                } else {
                    //TODO
                    //What is this?
                    int clrres = system("clear");
                    log_tag("debug_log.txt", "[DEBUG]",
                            "gameloop() 3 system(\"clear\") res was (%i)",
                            clrres);
                    printf("\n\n\tYOU DIED.\n\n");
                    log_tag("debug_log.txt", "[DEBUG]", "Game lost.");
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
                   //free(lore_strings[i]);
                   }
                   }
                 */

                //free(path->win_condition);
                //free(path);
                log_tag("debug_log.txt", "[DEBUG]", "End of wincon loop.");

                //free(current_floor);

                endwin();
            } else {
                log_tag("debug_log.txt", "[ERROR]", "Error in gameloop().");
                exit(EXIT_FAILURE);
            }
        }
        //kls_temp_end(gamestate_kls);
    } while (retry());

    //TODO
    //What is this?
    purple();
    printf("\n\n\t\tTHANKS 4 PLAYING!\n\n");
    white();
    log_tag("debug_log.txt", "[DEBUG]", "End of program.");
    exit(EXIT_SUCCESS);
}

#ifdef _WIN32
/**
 * Takes a integer and a string array (possibly from main).
 * @param argc The number of argv values + 1 (0 is program name?).
 * @param argv Array of strings with argc - 1 values.
 */
void gameloop_Win(int argc, char **argv)
{
    char *whoami;
    char path_to_kls_debug_file[600];
    char static_path[500];
    // Set static_path value to the correct static dir path
    resolve_staticPath(static_path);

    //Truncate "debug_log.txt"
    sprintf(path_to_kls_debug_file, "%s\\%s", static_path, "kls_debug_log.txt");
    KLS_Conf default_kls_conf = {
        .kls_autoset_regions = 1,
        .kls_autoset_temp_regions = 1,
        .kls_verbose_lvl = 1,
        .kls_log_filepath = path_to_kls_debug_file,
        .kls_reglist_kls_size = KLS_DEFAULT_SIZE * 16,
        .kls_reglist_alloc_backend = KLS_REGLIST_ALLOC_KLS_BASIC,
    };
    KLS_Conf temporary_kls_conf = {
        .kls_autoset_regions = 1,
        .kls_autoset_temp_regions = 1,
        .kls_verbose_lvl = 0,
        .kls_log_fp = stderr,
        .kls_reglist_kls_size = KLS_DEFAULT_SIZE * 16,
        .kls_reglist_alloc_backend = KLS_REGLIST_ALLOC_KLS_BASIC,
    };

    (whoami = strrchr(argv[0], '\\')) ? ++whoami : (whoami = argv[0]);
    do {
        default_kls = kls_new_conf(KLS_DEFAULT_SIZE * 8, default_kls_conf);
        temporary_kls = kls_new_conf(KLS_DEFAULT_SIZE * 8, temporary_kls_conf);
        char *kls_progname =
            (char *)KLS_PUSH_ARR_TYPED(default_kls, char *, sizeof(whoami),
                                       KLS_None, "progname", whoami);
        strcpy(kls_progname, whoami);
#ifndef HELAPORDO_DEBUG_LOG
#else
        FILE *debug_file = NULL;
        FILE *OPS_debug_file = NULL;
#endif
        // Parse command-line options
        int option;
        loadInfo *load_info =
            (loadInfo *) KLS_PUSH_TYPED(default_kls, loadInfo, HR_loadInfo,
                                        "loadInfo", "loadInfo");

        load_info->is_new_game = 1;	//By default we do a new game
        load_info->enemy_index = -1;
        load_info->total_foes = -1;
        load_info->save_type = -1;
        int loaded_roomtotalenemies = -1;
        int loaded_roomindex = -1;
        load_info->ptr_to_roomtotalenemies = &loaded_roomtotalenemies;
        load_info->ptr_to_roomindex = &loaded_roomindex;

        while ((option = getopt(argc, argv, "r:E:tTGRXQLlvdhsaV")) != -1) {
            switch (option) {
            case 'd': {
#ifndef HELAPORDO_DEBUG_ACCESS
#else
                G_DEBUG_ON += 1;
                G_LOG_ON = 1;
#endif
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
            break;
            case 'h': {
                usage(whoami);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            }
            break;
            case 'T': {
                G_DOTUTORIAL_ON = 1;
                handleTutorial();
                usage(whoami);
                kls_free(default_kls);
                kls_free(temporary_kls);
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
                napms(800);
                //TODO Win term color test?
                //display_colorpairs();
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            }
            break;
            case 'V': {
                printf("helapordo build: %s\n", helapordo_build_string);
                hlpd_dbg_features();
                printf("  using: s4c-animate v%s\n", S4C_ANIMATE_VERSION);
                s4c_dbg_features();
                printf("  using: koliseo v%s\n", string_koliseo_version());
                kls_dbg_features();
                printf("  using: ncurses v%s\n", NCURSES_VERSION);
#ifdef ANVIL__helapordo__
#ifndef INVIL__helapordo__HEADER__
                printf("  Built with: amboso v%s\n",
                       ANVIL__API_LEVEL__STRING);
#else
                printf("  Built with: invil v%s\n",
                       INVIL__VERSION__STRING);
                printf("Version Info: %.8s\n",
                       get_ANVIL__VERSION__DESC__());
                printf("Last commit: %s", get_INVIL__COMMIT__DESC__());
                const char* anvil_date = get_ANVIL__VERSION__DATE__();
                char* anvil_date_end;
#ifndef _WIN32
                time_t anvil_build_time = strtol(anvil_date, &anvil_date_end, 10);
#else
                time_t anvil_build_time = strtoll(anvil_date, &anvil_date_end, 10);
#endif //_WIN32

                if (anvil_date_end == anvil_date) {
                    //TODO: error
                } else {
                    char build_time_buff[20] = {0};
                    struct tm* build_time_tm = localtime(&anvil_build_time);

                    if (build_time_tm == NULL) {
                        //TODO: error
                    } else {
                        strftime(build_time_buff, 20, "%Y-%m-%d %H:%M:%S", build_time_tm);
                        printf("\nDate: %s\n", build_time_buff);
                    }
                }
#endif // INVIL__helapordo__HEADER__
#else
                printf("  Built without anvil\n");
#endif // ANVIL__helapordo__
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            }
            break;
            case 'v': {
                printVersion();
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_SUCCESS);
            }
            case '?': {
                fprintf(stderr,
                        "Invalid option: %c\n Check your arguments.\n",
                        option);
                usage(whoami);
                // Handle invalid options
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            default: {
                // Should never get here
                fprintf(stderr, "Invalid option: %c\n, bad usage.\n",
                        option);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            break;
            }
        }

#ifndef HELAPORDO_DEBUG_LOG
#else
        // Open log file if log flag is set and reset it
        if (G_LOG_ON == 1) {
            char path_to_debug_file[600];
            char path_to_OPS_debug_file[600];
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);

            //Truncate "debug_log.txt"
            sprintf(path_to_debug_file, "%s\\%s", static_path, "debug_log.txt");
            debug_file = fopen(path_to_debug_file, "w");
            if (!debug_file) {
                endwin();	//TODO: Can/should we check if we have to do this only in curses mode?
                fprintf(stderr,
                        "[ERROR]    Can't open debug logfile (%s\\debug_log.txt).\n",
                        static_path);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            fprintf(debug_file, "[DEBUGLOG]    --New game--  \n");
            fprintf(debug_file, "[DEBUG]    --Default kls debug info:--  \n");
            print_kls_2file(debug_file, default_kls);
            fprintf(debug_file, "[DEBUG]    --Temporary kls debug info:--  \n");
            print_kls_2file(debug_file, temporary_kls);
            fprintf(debug_file,
                    "[DEBUG]    --Closing header for new game.--  \n");
            fclose(debug_file);

            //Lay debug info
            log_tag("debug_log.txt", "[DEBUG]", "G_DEBUG_ON == (%i)",
                    G_DEBUG_ON);
            //FIXME we should have same behaviour as gameloop(), and actually log kls_progname...
            //Doesn't matter for now, kls_progname is declared later
            log_tag("debug_log.txt", "[DEBUG]", "whoami == (%s)", whoami);
            log_tag("debug_log.txt", "[DEBUG]", "G_LOG_ON == (%i)", G_LOG_ON);
            log_tag("debug_log.txt", "[DEBUG]", "small DEBUG FLAG ASSERTED");
            log_tag("debug_log.txt", "[DEBUG]",
                    "[Current position in default_kls] [pos: %lli]\n",
                    kls_get_pos(default_kls));

            //Truncate OPS_LOGFILE
            sprintf(path_to_OPS_debug_file, "%s\\%s", static_path, OPS_LOGFILE);
            OPS_debug_file = fopen(path_to_OPS_debug_file, "w");
            if (!OPS_debug_file) {
                endwin();	//TODO: Can/should we check if we have to do this only in curses mode?
                fprintf(stderr, "[ERROR]    Can't open OPS logfile (%s\\%s).\n",
                        static_path, OPS_LOGFILE);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
            fprintf(OPS_debug_file, "[OPLOG]    --New game--  \n");
            fclose(OPS_debug_file);
            log_tag("debug_log.txt", "[DEBUG]", "Truncated [%s]", OPS_LOGFILE);
            log_Win_EnvVars();
            log_tag("debug_log.txt", "[WIN32-DEBUG]", "Printing title.");
        }
#endif
        printTitle();
        printf("\n\n\n\n\t\t\t\tSTART\n\n");
        if (G_DEBUG_ON) {
            printf("\t\t\t\t\t\t\t\tDEBUG ON\n");
        }
        printf("\t\t\t\t\t\t");
        printFormattedVersion(whoami);
        printf("\n\nThe Windows build of \"helapordo\" is very much WIP.\n\n");
        printf("\n  Press Enter to proceed.\n");
        scanf("%*c");
        system("cls");
        printGlobVars();
        printWin_EnvVars();
        printf("\n\n  Press Enter to demo a minimal rogue floor.\n");
        printf("  Quit with Ctrl+C, or explore enough of the map.\n\n");
        printf("  You move with the arrow keys.\n\n");
        scanf("%*c");
        test_floors();
        kls_free(temporary_kls);
        kls_free(default_kls);
    } while (retry());

    //TODO
    //What is this?
    printf("\n\n\t\tTHANKS 4 PLAYING!\n\n");
    log_tag("debug_log.txt", "[DEBUG]", "End of program.");
    kls_free(default_kls);
    kls_free(temporary_kls);
    exit(0);
}
#endif				//End gameloop_Win()
