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
#include "turn_op.h"

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
#ifdef HELAPORDO_RAYLIB_BUILD
    log_tag("debug_log.txt", "[DEBUG]",
            "Logging isBoss: {%i}", isBoss);
#endif
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
#ifdef HELAPORDO_CURSES_BUILD
        handleSpecials(actor, enemy, boss, path, room_index, enemy_index,
                       isBoss);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
        assert(0 && "Implement handleSpecials() for raylib build\n");
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
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
#ifdef HELAPORDO_CURSES_BUILD
        handleConsumables(actor, enemy, boss, isBoss);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
        assert(0 && "Implement handleConsumables() for raylib build\n");
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
        res = OP_OK;
    }
    break;
    case OP_SAVE: {
        if (G_EXPERIMENTAL_ON == 1) {
            log_tag("debug_log.txt", "[DEBUG]", "%s():    G_RNG_ADVANCEMENTS == {%" PRId64 "}", __func__, G_RNG_ADVANCEMENTS);
            bool did_saveheader_init = false;
            char static_path[500];
            // Set static_path value to the correct static dir path
            resolve_staticPath(static_path);
            SaveHeader* current_saveHeader = prep_saveHeader(static_path, kls, true, &did_saveheader_init, path->current_saveslot->index);
            log_tag("debug_log.txt", "[DEBUG]", "Loaded Save Header version {%s}\n", current_saveHeader->game_version);
            bool prep_res = prep_Gamestate(gmst, static_path, 0, default_kls, true); //+ (idx* (sizeof(int64_t) + sizeof(SerGamestate))) , default_kls);
            if (prep_res) {
                log_tag("debug_log.txt", "[DEBUG]", "Done prep_Gamestate().");
            } else {
                log_tag("debug_log.txt", "[ERROR]", "Failed prep_Gamestate().");
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
        } else {
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Can't save without experimental flag.", __func__);
        }
        res = NO_OP; //TODO Collect save result for caller?
        break;
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
#ifdef HELAPORDO_CURSES_BUILD
            log_tag("debug_log.txt", "[TURNOP]",
                    "Doing endwin() before debug_generic()");
            endwin();
            debug_generic(gmst, actor, path, room_index, kls, t_kls);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
            assert(0 && "Implement debug_generic() for raylib build.\n");
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
            res = OP_OK;
        }
        break;
        case ENEMIES: {
            enemy_index = enemy->index;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (%i) (OP_DEBUG), isBoss == 0",
                    enemy->index);
            isBoss = 0;
#ifdef HELAPORDO_CURSES_BUILD
            debug_enemies_room(gmst, room, actor, enemy, path,
                               room_index, enemy_index, kls, t_kls);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
            assert(0 && "Implement debug_enemies_room() for raylib build.\n");
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
            res = OP_OK;
        }
        break;
        case BOSS: {
            enemy_index = 0;
            log_tag("debug_log.txt", "[TURNOP]",
                    "Setting enemy_index to (0) (OP_DEBUG), isBoss == 1");
            isBoss = 1;
#ifdef HELAPORDO_CURSES_BUILD
            log_tag("debug_log.txt", "[TURNOP]",
                    "Doing endwin() before debug_generic()");
            endwin();
            debug_generic(gmst, actor, path, room_index, kls, t_kls);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
            assert(0 && "Implement debug_generic() for raylib build.\n");
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
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
#ifdef HELAPORDO_CURSES_BUILD
        handleEquips(actor, path);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
        assert(0 && "Implement handleEquips() for raylib build\n");
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
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
#ifdef HELAPORDO_CURSES_BUILD
        printActivePerks(actor);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
        assert(0 && "Implement printActivePerks() for raylib build\n");
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
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
#ifdef HELAPORDO_CURSES_BUILD
        handleStats(actor);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
        assert(0 && "Implement handleStats() for raylib build\n");
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
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
#ifdef HELAPORDO_CURSES_BUILD
        handleArtifacts(actor);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
        assert(0 && "Implement handleArtifacts() for raylib build\n");
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
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
    case OP_CHANGE_OPTIONS: {
        if (gmst == NULL) {
            log_tag("debug_log.txt", "[WARN]",
                    "Gamestate pointer was null in turnOP(OP_CHANGE_OPTIONS)");
            exit(EXIT_FAILURE);
        }
        if (gmst->options == NULL) {
            log_tag("debug_log.txt", "[WARN]",
                    "GameOptions pointer was null in turnOP(OP_CHANGE_OPTIONS)");
            exit(EXIT_FAILURE);
        }
        log_tag("debug_log.txt", "[DEBUG]", "OP_CHANGE_OPTIONS");
        handleGameOptions(gmst->options);
        res = OP_OK;
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
