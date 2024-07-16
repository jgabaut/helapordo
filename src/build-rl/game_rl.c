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
#include "game_rl.h"

callback_void_t callback_func_ptrs[SPECIALSMAX];
callback_void_t callback_artifact_ptrs[ARTIFACTSMAX];
callback_void_t callback_counter_ptrs[COUNTERSMAX];

/**
 * Shows tutorial info.
 * @see gameloop_rl()
 */
void handleTutorial(S4C_Color* palette)
{
    Rectangle rc = {0};

    //If we get to this function straight from getopts, we need to do raylib init

    /* Initialize raylib */
    if (G_DOTUTORIAL_ON == 1) {
        //TODO: prepare windowed mode
        //framesCounter++;    // Count frames
        BeginDrawing();
        ClearBackground(ColorFromS4CPalette(palette,S4C_GREY));
    }
    rc = CLITERAL(Rectangle) {
        1, 2, 20, 70
    };

    int fontSize = 20;
    const char* label = "Tutorial";;
    DrawText(label, rc.x, rc.y, fontSize, ColorFromS4CPalette(palette,S4C_CYAN));

    Color tut_color = ColorFromS4CPalette(palette, S4C_LIGHT_YELLOW);
    DrawText("You can use the arrow keys and Enter to do everything needed for the game.", 14, 3*fontSize, fontSize, tut_color);
    DrawText("Buying things from a Shop may be tricky: you have to select one, then choose Buy.\nTo select one, First go up/down to 'View Item', then press Enter,\nthen you can scroll them with left/right. Press Enter to confirm your selection,\nthen go back up to Buy.", 14, 6*fontSize, fontSize, tut_color);
    DrawText("When running in Rogue mode, you can change floors by killing a Boss.", 14, 12*fontSize, fontSize, tut_color);
    DrawText("When in floor map, you can open the menu with the \"m\" key.", 14, 15*fontSize, fontSize, tut_color);
    DrawText("Rememeber, you can't really save in Rogue mode ! Run with \"-s\" to try Story mode.", 14, 18*fontSize, fontSize, tut_color);

    DrawText("[ Press ENTER or TAP to quit ]", 14, 21*fontSize, fontSize, ColorFromS4CPalette(palette,S4C_RED));

    if (G_DOTUTORIAL_ON == 1) {
        EndDrawing();
        //TODO: update win???
    }
}

/**
 * Takes a Enemy pointer and prepares its sprite field by copying it line by line from enemies_sprites, defined in sprites.h header.
 * @see Enemy
 * @see initEnemyStats
 * @see enemies_sprites
 * @param e The Enemy pointer whose sprite field will be initialised.
 */
void setEnemySprite(Enemy *e)
{
    if (e->class < ENEMYCLASSESMAX + 1) {
        for (int i = 0; i < 8; i++) {
            strcpy(e->sprite[i], enemies_sprites[e->class][i]);
        }
    } else {
        fprintf(stderr,
                "[ERROR]    Unexpected enemyClass in setEnemySprite().\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Takes a Equip pointer and prepares its sprite field by copying it line by line from equips_sprites, defined in sprites.h header.
 * @see Equip
 * @see dropEquip
 * @see equips_sprites
 * @param e The Equip pointer whose sprite field will be initialised.
 */
void setEquipSprite(Equip *e)
{
    assert(e != NULL);
    if (e->class < EQUIPSMAX + 1) {
        for (int i = 0; i < 8; i++) {
            strcpy(e->sprite[i], equips_sprites[e->class][i]);
        }
    } else {
        log_tag("debug_log.txt", "ERROR", "%s():    Unexpected equipClass {%i}", __func__, e->class);
    }
}

/**
 * Takes a Consumable pointer and prepares its sprite field by copying it line by line from consumables_sprites, defined in sprites.h header.
 * @see Consumable
 * @see initPlayerStats
 * @see consumables_sprites
 * @param c The Consumable pointer whose sprite field will be initialised.
 */
void setConsumableSprite(Consumable *c)
{
    if (c->class < CONSUMABLESMAX + 1) {
        for (int i = 0; i < 8; i++) {
            strcpy(c->sprite[i], consumables_sprites[c->class][i]);
        }
    } else {
        fprintf(stderr,
                "[ERROR]    Unexpected consumableClass in setConsumableSprite().\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Takes a Artifact pointer and prepares its sprite field by copying it line by line from artifacts_sprites, defined in sprites.h header.
 * @see Artifact
 * @see gameloop()
 * @see artifacts_sprites
 * @param a The Artifact pointer whose sprite field will be initialised.
 */
void setArtifactSprite(Artifact* a)
{
    if (a->class < ARTIFACTSMAX + 1) {
        for (int i = 0; i < 8; i++) {
            strcpy(a->sprite[i], artifacts_sprites[a->class][i]);
        }
    } else {
        fprintf(stderr,
                "[ERROR]    Unexpected artifactClass in setArtifactSprite().\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Takes a Boss pointer and prepares its sprite field by copying it line by line from bosses_sprites, defined in sprites.h header.
 * @see Boss
 * @see initBossStats
 * @see bosses_sprites
 * @param b The Boss pointer whose sprite field will be initialised.
 */
void setBossSprite(Boss *b)
{
    if (b->class < BOSSCLASSESMAX + 1) {
        for (int i = 0; i < 8; i++) {
            strcpy(b->sprite[i], bosses_sprites[b->class][i]);
        }
    } else {
        fprintf(stderr,
                "[ERROR]    Unexpected bossclass in setBossSprite().\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Takes a Fighter pointer and prepares its sprite field by copying it line by line.
 * @see Fighter
 * @see initPlayerStats
 * @param f The Fighter pointer whose sprite field will be initialised.
 */
void setFighterSprite(Fighter *f)
{
    //TODO: this sprite is also present in misc_sprites, defined in sprites.h
    //Should follow suit as the other setter functions and grab from there, not from this local copy.
    switch (f->class) {
    default: {
        strcpy(f->sprite[0], "I    yy    I");
        strcpy(f->sprite[1], "I    yy    I");
        strcpy(f->sprite[2], "I   yyyy y I");
        strcpy(f->sprite[3], "I yy yy y  I");
        strcpy(f->sprite[4], "I    yy    I");
        strcpy(f->sprite[5], "I   y  y   I");
        strcpy(f->sprite[6], "I   y  y   I");
        strcpy(f->sprite[7], "I    y y   I");
    }
    break;

    };
}

/**
 * Takes a Equipslot pointer and prepares its sprite field by copying it line by line from equipzones_sprites, defined in sprites.h header.
 * @see Equipslot
 * @see initEquipSlots()
 * @see equipzones_sprites
 * @param s The Equipslot pointer whose sprite field will be initialised.
 */
void setEquipslotSprite(Equipslot *s)
{
    if (s->type < EQUIPZONES + 1) {
        for (int i = 0; i < 8; i++) {
            strcpy(s->sprite[i], equipzones_sprites[s->type][i]);
        }
    } else {
        fprintf(stderr,
                "[ERROR]    Unexpected Equipslot type in setEquipslotSprite().\n");
        exit(EXIT_FAILURE);
    }
}

void ToggleFullScreenWindow(int w_W, int w_H)
{
    if (!IsWindowFullscreen()) {
        int mon = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(mon), GetMonitorHeight(mon));
        ToggleFullscreen();
    } else {
        ToggleFullscreen();
        SetWindowSize(w_W, w_H);
    }
}

/**
 * Takes a Chest pointer and prepares its sprite field by copying it line by line.
 * @see Chest
 * @see initChest
 * @param c The Chest pointer whose sprite field will be initialised.
 */
void setChestSprite(Chest *c)
{

    switch (c->class) {
    case CHEST_BASE: {
        strcpy(c->sprite[0], "            ");
        strcpy(c->sprite[1], "   bbbbbbb  ");
        strcpy(c->sprite[2], "  bbcccccbb ");
        strcpy(c->sprite[3], "  bcccccccb ");
        strcpy(c->sprite[4], "  bbbbbbbbb ");
        strcpy(c->sprite[5], "  bcccrcccb ");
        strcpy(c->sprite[6], "  bcccccccb ");
        strcpy(c->sprite[7], "  bbbbbbbbb ");
    }
    break;
    case CHEST_BEAST: {
        strcpy(c->sprite[0], "            ");
        strcpy(c->sprite[1], "   rrrrrrr  ");
        strcpy(c->sprite[2], "  rryyyyyrr ");
        strcpy(c->sprite[3], "  ryyyyyyyr ");
        strcpy(c->sprite[4], "  rrrrrrrrr ");
        strcpy(c->sprite[5], "  ryyymyyyr ");
        strcpy(c->sprite[6], "  ryyyyyyyr ");
        strcpy(c->sprite[7], "  rrrrrrrrr ");
    }
    break;
    default: {
        strcpy(c->sprite[0], "I    yy    I");
        strcpy(c->sprite[1], "I    yy    I");
        strcpy(c->sprite[2], "I yyyyyyyy I");
        strcpy(c->sprite[3], "I    yy    I");
        strcpy(c->sprite[4], "I    yy    I");
        strcpy(c->sprite[5], "I   yyyy   I");
        strcpy(c->sprite[6], "I   y  y   I");
        strcpy(c->sprite[7], "I    y y   I");
    }
    break;

    };

}

// void update_GameScreen(float* scale, float gameScreenWidth, float gameScreenHeight, GameScreen* currentScreen, int* framesCounter, Floor** current_floor, int* current_x, int* current_y, int logo_sleep, bool* pause_animation, Koliseo_Temp** floor_kls, KLS_Conf temporary_kls_conf, int* current_anim_frame, Vector2* mouse, Vector2* virtualMouse, loadInfo* load_info, int* saveslot_index, char current_save_path[1000])
void update_GameScreen(Gui_State* gui_state, Floor** current_floor, Path** game_path, Fighter** player, Room** current_room, Gamestate** gamestate, int* current_x, int* current_y, int logo_sleep, bool* pause_animation, Koliseo_Temp** floor_kls, KLS_Conf temporary_kls_conf, int* current_anim_frame, loadInfo* load_info, int* saveslot_index, char current_save_path[1500], char seed[PATH_SEED_BUFSIZE+1], bool is_seeded, int* roomsDone, int* enemyTotal)
{
    assert(gui_state != NULL);
    int center_x = FLOOR_MAX_COLS / 2;
    int center_y = FLOOR_MAX_ROWS / 2;
    gui_state->scale = MIN((float)GetScreenWidth()/gui_state->gameScreenWidth, (float)GetScreenHeight()/gui_state->gameScreenHeight);

    gui_state->mouse = GetMousePosition();

    gui_state->virtualMouse = (Vector2) {
        0
    };

    gui_state->virtualMouse.x = (gui_state->mouse.x - (GetScreenWidth() - (gui_state->gameScreenWidth*(gui_state->scale)))*0.5f)/ (gui_state->scale);
    gui_state->virtualMouse.y = (gui_state->mouse.y - (GetScreenHeight() - (gui_state->gameScreenHeight* (gui_state->scale)))*0.5f)/ (gui_state->scale);
    gui_state->virtualMouse = Vector2Clamp(gui_state->virtualMouse, (Vector2) {
        0, 0
    }, (Vector2) {
        (float)gui_state->gameScreenWidth, (float)gui_state->gameScreenHeight
    } );

    if (IsKeyPressed(KEY_F) && IsKeyDown(KEY_LEFT_ALT)) {
        ToggleFullScreenWindow(gui_state->gameScreenWidth, gui_state->gameScreenHeight);
    }

    char static_path[500];
    // Set static_path value to the correct static dir path
    resolve_staticPath(static_path);

    switch(gui_state->currentScreen) {
    case LOGO: {
        // TODO: Update LOGO screen variables here!

        gui_state->framesCounter += 1;    // Count frames

        // Wait for 2 seconds (120 frames) before jumping to TITLE screen
        if (gui_state->framesCounter > logo_sleep) {
            gui_state->currentScreen = TITLE;
        }
    }
    break;
    case TITLE: {
        // TODO: Update TITLE screen variables here!

        // Press enter to change to SAVES_VIEW screen
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
            gui_state->currentScreen = SAVES_VIEW;
        }
    }
    break;
    case SAVES_VIEW: {
        // TODO: Update SAVES_VIEW screen variables here!

        switch(load_info->is_new_game) {
        case -1: { // User has to pick new (1) or load (0)
            *saveslot_index = -1; // Reset it in case we got here after a whole game
            gui_state->buttons[BUTTON_NEW_GAME].on = false;
            gui_state->buttons[BUTTON_LOAD_GAME].on = false;
            sprintf(current_save_path, "%s", ""); // Clear current save path
            for (int i=BUTTON_NEW_GAME; i < BUTTON_LOAD_GAME+1; i++) {

                if (CheckCollisionPointRec(gui_state->virtualMouse, gui_state->buttons[i].r)) {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        gui_state->buttons[i].state = BUTTON_PRESSED;
                    } else {
                        gui_state->buttons[i].state = BUTTON_HOVER;
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                        gui_state->buttons[i].on = true;
                    }
                } else {
                    gui_state->buttons[i].state = BUTTON_NORMAL;
                }
                if (gui_state->buttons[i].on) {
                    fprintf(stderr, "%s():    [EFFECT]\n", __func__);
                    //TODO: may use i to se is_new_game for now but its weak to changes in the array
                    // load_info->is_new_game = i;
                    if (i == BUTTON_NEW_GAME) { // New game is the first button
                        load_info->is_new_game = 1;
                    } else if ( i == BUTTON_LOAD_GAME) {
                        load_info->is_new_game = 0;
                    }
                }
            }
        }
        break;
        case 0: {
            // Load game, Press 1-3 to change to set the index, and then change to FLOOR_VIEW screen
            if (*saveslot_index == -1) {  // Pick saveslot
                // Reference: https://www.raylib.com/examples/textures/loader.html?name=textures_sprite_button
                gui_state->buttons[BUTTON_SAVESLOT_1].on = false;
                gui_state->buttons[BUTTON_SAVESLOT_2].on = false;
                gui_state->buttons[BUTTON_SAVESLOT_3].on = false;
                sprintf(current_save_path, "%s", ""); // Clear current save path
                for (int i=BUTTON_SAVESLOT_1; i < BUTTON_SAVESLOT_3+1; i++) {

                    if (CheckCollisionPointRec(gui_state->virtualMouse, gui_state->buttons[i].r)) {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                            gui_state->buttons[i].state = BUTTON_PRESSED;
                        } else {
                            gui_state->buttons[i].state = BUTTON_HOVER;
                        }
                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                            gui_state->buttons[i].on = true;
                        }
                    } else {
                        gui_state->buttons[i].state = BUTTON_NORMAL;
                    }
                    if (gui_state->buttons[i].on) {
                        fprintf(stderr, "%s():    [EFFECT]\n", __func__);
                        //TODO: may use i to se is_new_game for now but its weak to changes in the array
                        // load_info->is_new_game = i;
                        if (i == BUTTON_SAVESLOT_1) { // New game is the first button
                            *saveslot_index = 0;
                            sprintf(current_save_path, "%s", default_saveslots[*saveslot_index].save_path);	//Update saveslot_path value
                        } else if ( i == BUTTON_SAVESLOT_2) {
                            *saveslot_index = 1;
                            sprintf(current_save_path, "%s", default_saveslots[*saveslot_index].save_path);	//Update saveslot_path value
                        } else if ( i == BUTTON_SAVESLOT_3) {
                            *saveslot_index = 2;
                            sprintf(current_save_path, "%s", default_saveslots[*saveslot_index].save_path);	//Update saveslot_path value
                        }
                    }
                }
                /*
                if (IsKeyPressed(KEY_ONE)) {
                    *saveslot_index = 0;
                    sprintf(current_save_path, "%s", default_saveslots[*saveslot_index].save_path);	//Update saveslot_path value
                } else if (IsKeyPressed(KEY_TWO)) {
                    *saveslot_index = 1;
                    sprintf(current_save_path, "%s", default_saveslots[*saveslot_index].save_path);	//Update saveslot_path value
                } else if (IsKeyPressed(KEY_THREE)) {
                    *saveslot_index = 2;
                    sprintf(current_save_path, "%s", default_saveslots[*saveslot_index].save_path);	//Update saveslot_path value
                }
                */
                if (*saveslot_index != -1) log_tag("debug_log.txt", "DEBUG", "%s():    User picked saveslot {%i} {%s}", __func__, *saveslot_index, default_saveslots[*saveslot_index].save_path);
            } else {
                *floor_kls = kls_temp_start(temporary_kls);
                bool did_save_init = false;
                SaveHeader* current_saveHeader = prep_saveHeader(static_path, default_kls, false, &did_save_init, *saveslot_index);

                log_tag("debug_log.txt", "[DEBUG]", "Loaded Save Header version {%s}\n", current_saveHeader->game_version);
                if (*game_path == NULL) {
                    log_tag("debug_log.txt", "DEBUG", "%s():    Init for game_path", __func__);
                    *game_path = randomise_path(seed, temporary_kls, current_save_path);
                    (*game_path)->current_saveslot->index = *saveslot_index;
                    Wincon *w =
                        (Wincon *) KLS_PUSH_TYPED(default_kls, Wincon, HR_Wincon,
                                                  "Wincon", "Loady Wincon");
                    w->class = FULL_PATH;
                    initWincon(w, *game_path, w->class);
                    (*game_path)->win_condition = w;
                    log_tag("debug_log.txt", "DEBUG", "%s():    Prepared Path", __func__);
                }
                if (*player == NULL) {
                    log_tag("debug_log.txt", "DEBUG", "%s():    Init for player", __func__);
                    *player =
                        (Fighter *) KLS_PUSH_TYPED(temporary_kls, Fighter, HR_Fighter,
                                                   "Fighter", "Loady Fighter");

                    strncpy((*player)->name, "Test", strlen("Test")+1);
                    (*player)->class = Knight;
                    //getParams(argc, argv, player, path, optTot, default_kls);
                    //TODO: ensure class and name are taken before this update
                    initPlayerStats(*player, *game_path, temporary_kls);
                    log_tag("debug_log.txt", "DEBUG", "%s():    Prepared Loady Fighter", __func__);
                }
                if (*gamestate == NULL) {
                    log_tag("debug_log.txt", "DEBUG", "%s():    Init for gamestate", __func__);
                    *gamestate =
                        KLS_PUSH_TYPED(default_kls, Gamestate, HR_Gamestate, "Gamestate",
                                       "Gamestate");
#ifndef KOLISEO_HAS_REGION
                    log_tag("debug_log.txt", "[DEBUG]", "%s():    setting G_GAMESTATE", __func__);
                    G_GAMESTATE = *gamestate;
#endif
                    clock_t start_time = clock(); //TODO: get this before?
                    init_Gamestate(*gamestate, start_time, (*player)->stats, (*game_path)->win_condition, (*game_path),
                                   *player, GAMEMODE);

                    (*gamestate)->current_floor = *current_floor; // Should be NULL here

                    *current_room = KLS_PUSH_TYPED(default_kls, Room, HR_Floor, "Room", "Loady Room");
                    (*current_room)->class = BASIC;
                    (*gamestate)->current_room = *current_room;
                    // TODO Prep loading room memory

                    for (int e_idx=0; e_idx < ROOM_ENEMIES_MAX; e_idx++) {
                        log_tag("debug_log.txt", "[DEBUG]", "%s():    Preparing loading room enemy {%i}", __func__, e_idx);
                        (*current_room)->enemies[e_idx] = KLS_PUSH_TYPED(default_kls, Enemy, HR_Enemy, "Enemy", "Loading room enemy");
                        prepareRoomEnemy((*current_room)->enemies[e_idx], 1, 3, 1,
                                         *floor_kls);
                    }

                    bool force_init = false;
                    SaveHeader* current_saveHeader = prep_saveHeader(static_path, default_kls, force_init, &did_save_init, (*game_path)->current_saveslot->index);
                    log_tag("debug_log.txt", "[DEBUG]", "Loaded Save Header version {%s}", current_saveHeader->game_version);

                    bool prep_res = prep_Gamestate(*gamestate, static_path, 0, default_kls, did_save_init); //+ (idx* (sizeof(int64_t) + sizeof(SerGamestate))) , default_kls);
                    if (prep_res) {
                        log_tag("debug_log.txt", "[DEBUG]", "Done prep_Gamestate().");
                    } else {
                        log_tag("debug_log.txt", "[ERROR]", "Failed prep_Gamestate().");
                        kls_free(default_kls);
                        kls_free(temporary_kls);
                        exit(EXIT_FAILURE);
                    }

                    load_info->enemy_index = (*gamestate)->current_enemy_index;
                    log_tag("debug_log.txt", "[DEBUG]", "%s():    load_info->enemy_index: {%i}", __func__, load_info->enemy_index);
                    (*gamestate)->path->seed[PATH_SEED_BUFSIZE] = '\0';
                    memcpy(seed, (*gamestate)->path->seed, PATH_SEED_BUFSIZE);
                    seed[PATH_SEED_BUFSIZE] = '\0';
                    log_tag("debug_log.txt", "[DEBUG]",
                            "Seed after loading: [%s]", seed);

                    log_tag("debug_log.txt", "[DEBUG]", "%s():    Setting is_seeded {%s} to gamestate->is_seeded {%s}", __func__, (is_seeded ? "true" : "false"), ((*gamestate)->is_seeded ? "true" : "false"));
                    is_seeded = (*gamestate)->is_seeded;

                    //TODO: set the other load_info fields properly?
                    //
                    log_tag("debug_log.txt", "[DEBUG]", "%s():    Checking save type", __func__);
                    if ((*gamestate)->current_room != NULL) {
                        *current_room = (*gamestate)->current_room;
                        switch ((*current_room)->class) {
                        case ENEMIES: {
                            if (load_info->enemy_index < 0) {
                                log_tag("debug_log.txt", "[ERROR]", "%s():    load_info->enemy_index was <0: {%i}", __func__, load_info->enemy_index);
                                fprintf(stderr, "%s():    Failed preparing gamestate. Invalid enemy index.\n", __func__);
                                kls_free(default_kls);
                                kls_free(temporary_kls);
                                exit(EXIT_FAILURE);
                            }
                            load_info->save_type = ENEMIES_SAVE;
                        }
                        break;
                        case HOME: {
                            load_info->save_type = HOME_SAVE;
                        }
                        break;
                        case BASIC: {
                            load_info->save_type = FLOORMENU_SAVE;
                        }
                        break;
                        default: {
                            log_tag("debug_log.txt", "[ERROR]", "%s():    unexpected class for current room. {%i}", __func__, (*current_room)->class);
                            kls_free(default_kls);
                            kls_free(temporary_kls);
                            exit(EXIT_FAILURE);
                            break;
                        }
                        }

                        log_tag("debug_log.txt", "[DEBUG]", "%s():    save_type: [%s]", __func__, stringFrom_saveType(load_info->save_type));
                        log_tag("debug_log.txt", "[DEBUG]", "%s():    Setting *(load_info->ptr_to_roomindex) to {%i}.", __func__, (*gamestate)->player->stats->roomscompleted);
                        *(load_info->ptr_to_roomindex) = (*gamestate)->player->stats->roomscompleted;
                        log_tag("debug_log.txt", "[DEBUG]", "%s():    Setting *(load_info->ptr_to_roomtotalenemies) to {%i}.", __func__, (*current_room)->enemyTotal);
                        *(load_info->ptr_to_roomtotalenemies) = (*current_room)->enemyTotal;

                        if ((*current_room)->class == ENEMIES) {
                            // TODO Load && Store current enemy somewhere.
                            kls_log(default_kls, "DEBUG", "Prepping Loady Enemy: gamestate->current_room->enemies[%i]", (*gamestate)->current_enemy_index);
                            if ((*gamestate)->current_enemy_index >= (*current_room)->enemyTotal) {
                                log_tag("debug_log.txt", "[ERROR]", "%s():    gmst current_enemy_idex > gmst enemyTotal.", __func__);
                                kls_free(default_kls);
                                kls_free(temporary_kls);
                                exit(EXIT_FAILURE);
                            }
                            Enemy** loady_room_enemies = (*current_room)->enemies;
                            if (loady_room_enemies == NULL) {
                                log_tag("debug_log.txt", "[ERROR]", "%s():    Loady room_enemies is NULL.", __func__);
                                log_tag("debug_log.txt", "[ERROR]", "idx/tot: {%i/%i}", (*gamestate)->current_enemy_index, (*current_room)->enemyTotal);
                                kls_free(default_kls);
                                kls_free(temporary_kls);
                                exit(EXIT_FAILURE);
                            }

                            Enemy* loady_enemy = loady_room_enemies[(*gamestate)->current_enemy_index];
                            if (loady_enemy == NULL) {
                                log_tag("debug_log.txt", "[ERROR]", "%s():    Loady enemy_idx is NULL.", __func__);
                                log_tag("debug_log.txt", "[ERROR]", "idx/tot: {%i/%i}", (*gamestate)->current_enemy_index, (*current_room)->enemyTotal);
                                kls_free(default_kls);
                                kls_free(temporary_kls);
                                exit(EXIT_FAILURE);
                            } else {
                                //log_tag("debug_log.txt", "[DEBUG]", "%s():    Loady enemy: {%s}", __func__, stringFromEClass(loady_enemy->class));
                            }
                            load_info->loaded_enemy = loady_enemy;
                            //(Enemy *) KLS_PUSH_T_TYPED(gamestate_kls, Enemy, HR_Enemy,
                            //                         "Enemy", "Loaded Enemy");
                            //FIXME: the structs related to loaded enemy are not loaded on default_kls
                            //Update loading_room_turn_args->enemy pointer
                            //loading_room_turn_args->enemy = load_info->loaded_enemy;
                        }
                    } else {
                        log_tag("debug_log.txt", "[WARN-TURNOP]",
                                "%s():    gamestate->room was NULL. Not setting load_info's room info.", __func__);
                    }
                }
                gui_state->currentScreen = FLOOR_VIEW;
            }
        }
        break;
        case 1: {
            // New game, Press 1-3 to change to set the index, and then change to FLOOR_VIEW screen
            if (gui_state->buttons[BUTTON_NAME_TXTFIELD].label_len == 0) {
                gui_state->currentScreen = NAMEPICK_VIEW;
            } else {
                if (gui_state->buttons[BUTTON_CLASS_TXTFIELD].label_len == 0) {
                    gui_state->currentScreen = CLASSPICK_VIEW;
                } else {
                    // Reference: https://www.raylib.com/examples/textures/loader.html?name=textures_sprite_button
                    if (*saveslot_index == -1) {  // Pick saveslot
                        gui_state->buttons[BUTTON_SAVESLOT_1].on = false;
                        gui_state->buttons[BUTTON_SAVESLOT_2].on = false;
                        gui_state->buttons[BUTTON_SAVESLOT_3].on = false;
                        sprintf(current_save_path, "%s", ""); // Clear current save path
                        for (int i=BUTTON_SAVESLOT_1; i < BUTTON_SAVESLOT_3+1; i++) {

                            if (CheckCollisionPointRec(gui_state->virtualMouse, gui_state->buttons[i].r)) {
                                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                                    gui_state->buttons[i].state = BUTTON_PRESSED;
                                } else {
                                    gui_state->buttons[i].state = BUTTON_HOVER;
                                }
                                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                                    gui_state->buttons[i].on = true;
                                }
                            } else {
                                gui_state->buttons[i].state = BUTTON_NORMAL;
                            }
                            if (gui_state->buttons[i].on) {
                                fprintf(stderr, "%s():    [EFFECT]\n", __func__);
                                //TODO: may use i to se is_new_game for now but its weak to changes in the array
                                // load_info->is_new_game = i;
                                if (i == BUTTON_SAVESLOT_1) { // New game is the first button
                                    *saveslot_index = 0;
                                    sprintf(current_save_path, "%s", default_saveslots[*saveslot_index].save_path);	//Update saveslot_path value
                                } else if ( i == BUTTON_SAVESLOT_2) {
                                    *saveslot_index = 1;
                                    sprintf(current_save_path, "%s", default_saveslots[*saveslot_index].save_path);	//Update saveslot_path value
                                } else if ( i == BUTTON_SAVESLOT_3) {
                                    *saveslot_index = 2;
                                    sprintf(current_save_path, "%s", default_saveslots[*saveslot_index].save_path);	//Update saveslot_path value
                                }
                            }
                        }
                        /*
                        if (IsKeyPressed(KEY_ONE)) {
                            *saveslot_index = 0;
                            sprintf(current_save_path, "%s", default_saveslots[*saveslot_index].save_path);	//Update saveslot_path value
                        } else if (IsKeyPressed(KEY_TWO)) {
                            *saveslot_index = 1;
                            sprintf(current_save_path, "%s", default_saveslots[*saveslot_index].save_path);	//Update saveslot_path value
                        } else if (IsKeyPressed(KEY_THREE)) {
                            *saveslot_index = 2;
                            sprintf(current_save_path, "%s", default_saveslots[*saveslot_index].save_path);	//Update saveslot_path value
                        }
                        */
                        if (*saveslot_index != -1) log_tag("debug_log.txt", "DEBUG", "%s():    User picked saveslot {%i} {%s}", __func__, *saveslot_index, default_saveslots[*saveslot_index].save_path);
                    } else {
                        *floor_kls = kls_temp_start(temporary_kls);
                        bool did_save_init = false;
                        bool force_save_init = true;
                        SaveHeader* current_saveHeader = prep_saveHeader(static_path, default_kls, force_save_init, &did_save_init, *saveslot_index);

                        log_tag("debug_log.txt", "[DEBUG]", "Loaded Save Header version {%s}\n", current_saveHeader->game_version);
                        if (*game_path == NULL) {
                            log_tag("debug_log.txt", "DEBUG", "%s():    Init for game_path", __func__);
                            *game_path = randomise_path(seed, temporary_kls, current_save_path);
                            (*game_path)->current_saveslot->index = *saveslot_index;
                            Wincon *w =
                                (Wincon *) KLS_PUSH_TYPED(default_kls, Wincon, HR_Wincon,
                                                          "Wincon", "Wincon");
                            w->class = FULL_PATH;
                            initWincon(w, *game_path, w->class);
                            (*game_path)->win_condition = w;
                            log_tag("debug_log.txt", "DEBUG", "%s():    Prepared Path", __func__);
                        }
                        if (*player == NULL) {
                            log_tag("debug_log.txt", "DEBUG", "%s():    Init for player", __func__);
                            *player =
                                (Fighter *) KLS_PUSH_TYPED(temporary_kls, Fighter, HR_Fighter,
                                                           "Fighter", "Fighter");

                            Gui_Button namefield = gui_state->buttons[BUTTON_NAME_TXTFIELD];
                            if (namefield.label_len > 0) {
                                log_tag("debug_log.txt", "DEBUG", "%s():    Using {%s} for player name", __func__, namefield.label);
                                fprintf(stderr, "[DEBUG] [%s()]    Using {%s} for player name\n", __func__, namefield.label);
                                strncpy((*player)->name, namefield.label, namefield.label_len+1);
                            } else {
                                assert(false);
                                strncpy((*player)->name, "Test", strlen("Test")+1);
                            }
                            Gui_Button classfield = gui_state->buttons[BUTTON_CLASS_TXTFIELD];
                            if (classfield.label_len > 0) {
                                log_tag("debug_log.txt", "DEBUG", "%s():    Using {%s} for player class", __func__, classfield.label);
                                fprintf(stderr, "[DEBUG] [%s()]    Using {%s} for player class\n", __func__, classfield.label);
                                if (strcmp(classfield.label, "Knight") == 0) {
                                    (*player)->class = Knight;
                                } else if (strcmp(classfield.label, "Archer") == 0) {
                                    (*player)->class = Archer;
                                } else if (strcmp(classfield.label, "Mage") == 0) {
                                    (*player)->class = Mage;
                                } else if (strcmp(classfield.label, "Assassin") == 0) {
                                    (*player)->class = Assassin;
                                }
                            } else {
                                assert(false);
                                (*player)->class = Knight;
                            }
                            //getParams(argc, argv, player, path, optTot, default_kls);
                            //TODO: ensure class and name are taken before this update
                            initPlayerStats(*player, *game_path, temporary_kls);
                            log_tag("debug_log.txt", "DEBUG", "%s():    Prepared new Fighter", __func__);
                        }
                        if (*gamestate == NULL) {
                            log_tag("debug_log.txt", "DEBUG", "%s():    Init for gamestate", __func__);
                            *gamestate =
                                KLS_PUSH_TYPED(default_kls, Gamestate, HR_Gamestate, "Gamestate",
                                               "Gamestate");
#ifndef KOLISEO_HAS_REGION
                            log_tag("debug_log.txt", "[DEBUG]", "%s():    setting G_GAMESTATE", __func__);
                            G_GAMESTATE = *gamestate;
#endif
                            clock_t start_time = clock(); //TODO: get this before?
                            init_Gamestate(*gamestate, start_time, (*player)->stats, (*game_path)->win_condition, (*game_path),
                                           *player, GAMEMODE);

                            (*gamestate)->current_floor = *current_floor; // Should be NULL here
                            (*gamestate)->current_room = *current_room; // Should be NULL here
                            (*gamestate)->is_seeded = is_seeded;
                            (*gamestate)->current_enemy_index = 0;

                            bool did_save_init = false;
                            SaveHeader* current_saveHeader = prep_saveHeader(static_path, default_kls, force_save_init, &did_save_init, (*game_path)->current_saveslot->index);
                            log_tag("debug_log.txt", "[DEBUG]", "Loaded Save Header version {%s}", current_saveHeader->game_version);

                            bool prep_res = prep_Gamestate(*gamestate, static_path, 0, default_kls, did_save_init); //+ (idx* (sizeof(int64_t) + sizeof(SerGamestate))) , default_kls);
                            if (prep_res) {
                                log_tag("debug_log.txt", "[DEBUG]", "Done prep_Gamestate().");
                            } else {
                                log_tag("debug_log.txt", "[ERROR]", "Failed prep_Gamestate().");
                                kls_free(default_kls);
                                kls_free(temporary_kls);
                                exit(EXIT_FAILURE);
                            }
                        }
                        gui_state->currentScreen = FLOOR_VIEW;
                    } // End else saveslot is picked
                } // End else class is not empty
            } // End else name is not empty
        }
        break;
        default: {
            fprintf(stderr,"%s():    unexpected value for load_info->is_new_game: [%i]\n", __func__, load_info->is_new_game);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        }
    }
    break;
    case NAMEPICK_VIEW: {
        // Reference: https://www.raylib.com/examples/text/loader.html?name=text_input_box
        if (CheckCollisionPointRec(gui_state->virtualMouse, gui_state->buttons[BUTTON_NAME_TXTFIELD].r)) {
            gui_state->buttons[BUTTON_NAME_TXTFIELD].on = true;
        } else {
            gui_state->buttons[BUTTON_NAME_TXTFIELD].on = false;
        }
        if (gui_state->buttons[BUTTON_NAME_TXTFIELD].on) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (gui_state->buttons[BUTTON_NAME_TXTFIELD].label_len < 20)) {
                    size_t l = gui_state->buttons[BUTTON_NAME_TXTFIELD].label_len;
                    gui_state->buttons[BUTTON_NAME_TXTFIELD].label[l] = (char) key;
                    gui_state->buttons[BUTTON_NAME_TXTFIELD].label[l+1] = '\0';

                    gui_state->buttons[BUTTON_NAME_TXTFIELD].label_len += 1;

                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                gui_state->buttons[BUTTON_NAME_TXTFIELD].label_len -= 1;
                if (gui_state->buttons[BUTTON_NAME_TXTFIELD].label_len < 0) {
                    gui_state->buttons[BUTTON_NAME_TXTFIELD].label_len = 0;
                }
                size_t l = gui_state->buttons[BUTTON_NAME_TXTFIELD].label_len;
                gui_state->buttons[BUTTON_NAME_TXTFIELD].label[l] = '\0';

            }

            if (IsKeyPressed(KEY_ENTER)) {
                if (gui_state->buttons[BUTTON_NAME_TXTFIELD].label_len > 0) {
                    //TODO: Use the name
                    gui_state->currentScreen = SAVES_VIEW;
                }
            }
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        gui_state->framesCounter += 1;
    }
    break;
    case CLASSPICK_VIEW: {
        gui_state->buttons[BUTTON_CLASS_KNIGHT].on = false;
        gui_state->buttons[BUTTON_CLASS_ARCHER].on = false;
        gui_state->buttons[BUTTON_CLASS_MAGE].on = false;
        gui_state->buttons[BUTTON_CLASS_ASSASSIN].on = false;
        for (int i=BUTTON_CLASS_KNIGHT; i < BUTTON_CLASS_ASSASSIN+1; i++) {

            if (CheckCollisionPointRec(gui_state->virtualMouse, gui_state->buttons[i].r)) {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    gui_state->buttons[i].state = BUTTON_PRESSED;
                } else {
                    gui_state->buttons[i].state = BUTTON_HOVER;
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    gui_state->buttons[i].on = true;
                }
            } else {
                gui_state->buttons[i].state = BUTTON_NORMAL;
            }
            if (gui_state->buttons[i].on) {
                fprintf(stderr, "%s():    [EFFECT]\n", __func__);
                if (i == BUTTON_CLASS_KNIGHT) {
                    memcpy(gui_state->buttons[BUTTON_CLASS_TXTFIELD].label, "Knight", strlen("Knight")+1);
                    gui_state->buttons[BUTTON_CLASS_TXTFIELD].label_len = strlen(gui_state->buttons[BUTTON_CLASS_TXTFIELD].label);
                    gui_state->currentScreen = SAVES_VIEW;
                } else if ( i == BUTTON_CLASS_ARCHER) {
                    memcpy(gui_state->buttons[BUTTON_CLASS_TXTFIELD].label, "Archer", strlen("Archer")+1);
                    gui_state->buttons[BUTTON_CLASS_TXTFIELD].label_len = strlen(gui_state->buttons[BUTTON_CLASS_TXTFIELD].label);
                    gui_state->currentScreen = SAVES_VIEW;
                } else if ( i == BUTTON_CLASS_MAGE) {
                    memcpy(gui_state->buttons[BUTTON_CLASS_TXTFIELD].label, "Mage", strlen("Mage")+1);
                    gui_state->buttons[BUTTON_CLASS_TXTFIELD].label_len = strlen(gui_state->buttons[BUTTON_CLASS_TXTFIELD].label);
                    gui_state->currentScreen = SAVES_VIEW;
                } else if ( i == BUTTON_CLASS_ASSASSIN) {
                    memcpy(gui_state->buttons[BUTTON_CLASS_TXTFIELD].label, "Assassin", strlen("Assassin")+1);
                    gui_state->buttons[BUTTON_CLASS_TXTFIELD].label_len = strlen(gui_state->buttons[BUTTON_CLASS_TXTFIELD].label);
                    gui_state->currentScreen = SAVES_VIEW;
                }
            }
        }
    }
    break;
    case FLOOR_VIEW: {
        // TODO: Update FLOOR_VIEW screen variables here!
        if (*current_floor == NULL) {
            log_tag("debug_log.txt", "DEBUG", "%s():    Init for current_floor", __func__);
            *current_floor = (Floor *) KLS_PUSH_T_TYPED(*floor_kls, Floor,
                             HR_Floor, "Floor", "Floor");

            // Init dbg_floor
            init_floor_layout(*current_floor);

            //Set center as filled
            (*current_floor)->floor_layout[center_x][center_y] = 1;

            //Init floor rooms
            init_floor_rooms(*current_floor);

            if ((hlpd_rand() % 101) > 20) {
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Doing bsp init", __func__);
                floor_bsp_gen(*current_floor, *floor_kls, center_x, center_y);
                (*current_floor)->from_bsp = true;
            } else {
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Doing random walk init", __func__);
                //Random walk #1
                floor_random_walk(*current_floor, center_x, center_y, 100, 1);	// Perform 100 steps of random walk, reset floor_layout if needed.
                //Random walk #2
                floor_random_walk(*current_floor, center_x, center_y, 100, 0);	// Perform 100 more steps of random walk, DON'T reset floor_layout if needed.
                (*current_floor)->from_bsp = false;
            }

            //Set floor explored matrix
            load_floor_explored(*current_floor);

            //Set room types
            floor_set_room_types(*current_floor);

            if (!(*current_floor)->from_bsp) {
                log_tag("debug_log.txt", "[DEBUG]", "Putting player at center: {%i,%i}", center_x, center_y);
                *current_x = center_x;
                *current_y = center_y;
            } else {
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Finding HOME room x/y for floor, and putting player there", __func__);
                int home_room_x = -1;
                int home_room_y = -1;
                bool done_looking = false;
                for(size_t i=0; i < FLOOR_MAX_COLS && !done_looking; i++) {
                    for (size_t j=0; j < FLOOR_MAX_ROWS && !done_looking; j++) {
                        if ((*current_floor)->roomclass_layout[i][j] == HOME) {
                            log_tag("debug_log.txt", "[DEBUG]", "%s():    Found HOME room at {x:%i, y:%i}.", __func__, i, j);
                            home_room_x = i;
                            home_room_y = j;
                            done_looking = true;
                        }
                    }
                }
                if (!done_looking) {
                    log_tag("debug_log.txt", "[DEBUG]", "%s():    Could not find HOME room.", __func__);
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                }
                log_tag("debug_log.txt", "[DEBUG]", "Putting player at HOME room: {%i,%i}", home_room_x, home_room_y);
                *current_x = home_room_x;
                *current_y = home_room_y;
            }
            // Set starting room as explored already
            (*current_floor)->explored_matrix[*current_x][*current_y] = 1;
            (*gamestate)->current_floor = *current_floor;
            (*gamestate)->current_room = *current_room; // Could be NULL here
        } // End if *current_floor is NULL

        gui_state->framesCounter += 1;    // Count frames

        // Press enter to change to ENDING screen
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
            gui_state->currentScreen = ENDING;
        }

        if (IsKeyPressed(KEY_R)) {
            fprintf(stderr,"%s\n", "Regenerating current floor");
            kls_temp_end(*floor_kls);
            kls_free(temporary_kls);
            temporary_kls = kls_new_conf(KLS_DEFAULT_SIZE * 32, temporary_kls_conf);

            srand(time(NULL));
            G_RNG_ADVANCEMENTS = 0;
            if (!is_seeded) gen_random_seed(seed);

            *game_path = randomise_path(seed, temporary_kls, current_save_path);

            (*game_path)->current_saveslot->index = *saveslot_index;

            *player =
                (Fighter *) KLS_PUSH_TYPED(temporary_kls, Fighter, HR_Fighter,
                                           "Fighter", "Fighter");

            strncpy((*player)->name, "Test", strlen("Test")+1);
            (*player)->class = Knight;
            //getParams(argc, argv, player, path, optTot, default_kls);
            //TODO: ensure class and name are taken before this update
            initPlayerStats(*player, *game_path, temporary_kls);

            log_tag("debug_log.txt", "DEBUG", "%s():    Reset rng, seed, Path, Fighter", __func__);

            *floor_kls = kls_temp_start(temporary_kls);
            *current_floor =
                (Floor *) KLS_PUSH_T_TYPED(*floor_kls, Floor,
                                           HR_Floor, "Floor", "Floor");
            // Init dbg_floor
            init_floor_layout(*current_floor);

            //Set center as filled
            (*current_floor)->floor_layout[center_x][center_y] = 1;

            //Init floor rooms
            init_floor_rooms(*current_floor);

            if ((hlpd_rand() % 101) > 20) {
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Doing bsp init", __func__);
                floor_bsp_gen(*current_floor, *floor_kls, center_x, center_y);
                (*current_floor)->from_bsp = true;
            } else {
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Doing random walk init", __func__);
                //Random walk #1
                floor_random_walk(*current_floor, center_x, center_y, 100, 1);	// Perform 100 steps of random walk, reset floor_layout if needed.
                //Random walk #2
                floor_random_walk(*current_floor, center_x, center_y, 100, 0);	// Perform 100 more steps of random walk, DON'T reset floor_layout if needed.
                (*current_floor)->from_bsp = false;
            }

            //Set floor explored matrix
            load_floor_explored(*current_floor);

            //Set room types
            floor_set_room_types(*current_floor);

            if (!((*current_floor)->from_bsp)) {
                log_tag("debug_log.txt", "[DEBUG]", "Putting player at center: {%i,%i}", center_x, center_y);
                *current_x = center_x;
                *current_y = center_y;
            } else {
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Finding HOME room x/y for floor, and putting player there", __func__);
                int home_room_x = -1;
                int home_room_y = -1;
                bool done_looking = false;
                for(size_t i=0; i < FLOOR_MAX_COLS && !done_looking; i++) {
                    for (size_t j=0; j < FLOOR_MAX_ROWS && !done_looking; j++) {
                        if ((*current_floor)->roomclass_layout[i][j] == HOME) {
                            log_tag("debug_log.txt", "[DEBUG]", "%s():    Found HOME room at {x:%i, y:%i}.", __func__, i, j);
                            home_room_x = i;
                            home_room_y = j;
                            done_looking = true;
                        }
                    }
                }
                if (!done_looking) {
                    log_tag("debug_log.txt", "[DEBUG]", "%s():    Could not find HOME room.", __func__);
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                }
                log_tag("debug_log.txt", "[DEBUG]", "Putting player at HOME room: {%i,%i}", home_room_x, home_room_y);
                *current_x = home_room_x;
                *current_y = home_room_y;
            }

            // Set starting room as explored already
            (*current_floor)->explored_matrix[*current_x][*current_y] = 1;
            (*gamestate)->current_floor = *current_floor;
            (*gamestate)->current_room = *current_room; // Should be NULL here
        }
        if (IsKeyPressed(KEY_UP)) {
            step_floor(*current_floor, current_x,
                       current_y, KEY_UP);
            (*player)->floor_x = *current_x;
            (*player)->floor_y = *current_y;
            if ((*current_floor)->roomclass_layout[*current_x][*current_y] != BASIC) {
                gui_state->currentScreen = DOOR_ANIM;
                *current_anim_frame = 0;
                break;
            }
        }
        if (IsKeyPressed(KEY_DOWN)) {
            step_floor(*current_floor, current_x,
                       current_y, KEY_DOWN);
            (*player)->floor_x = *current_x;
            (*player)->floor_y = *current_y;
            if ((*current_floor)->roomclass_layout[*current_x][*current_y] != BASIC) {
                gui_state->currentScreen = DOOR_ANIM;
                *current_anim_frame = 0;
                break;
            }
        }
        if (IsKeyPressed(KEY_LEFT)) {
            step_floor(*current_floor, current_x,
                       current_y, KEY_LEFT);
            (*player)->floor_x = *current_x;
            (*player)->floor_y = *current_y;
            if ((*current_floor)->roomclass_layout[*current_x][*current_y] != BASIC) {
                gui_state->currentScreen = DOOR_ANIM;
                *current_anim_frame = 0;
                break;
            }
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            step_floor(*current_floor, current_x,
                       current_y, KEY_RIGHT);
            (*player)->floor_x = *current_x;
            (*player)->floor_y = *current_y;
            if ((*current_floor)->roomclass_layout[*current_x][*current_y] != BASIC) {
                gui_state->currentScreen = DOOR_ANIM;
                *current_anim_frame = 0;
                break;
            }
        }
    }
    break;
    case ROOM_VIEW: {
        // TODO: Update ROOM_VIEW screen variables here!
        if (*current_room == NULL) {

            log_tag("debug_log.txt", "DEBUG",
                    "Prepping Room for Rogue Gamemode. roomsDone=(%i)",
                    *roomsDone);
            *current_room =
                (Room *) KLS_PUSH_T_TYPED(*floor_kls, Room,
                                          HR_Room, "Room", "Room");

            (*current_room)->index = *roomsDone;
            //setRoomType(path, &roadFork_value, &room_type, roomsDone);

            roomClass room_type =
                (*current_floor)->
                roomclass_layout[(*player)->floor_x][(*player)->floor_y];
            log_tag("debug_log.txt", "[ROOM]",
                    "Set Room #%i type:    (%s)\n", *roomsDone,
                    stringFromRoom(room_type));

            initRoom(*current_room, *player, *roomsDone, room_type,
                     *enemyTotal, load_info, *floor_kls);
            log_tag("debug_log.txt", "[ROOM]",
                    "Init Room #%i:    (%s)\n", *roomsDone,
                    stringFromRoom(room_type));
        }

        gui_state->framesCounter += 1;    // Count frames

        if (IsKeyPressed(KEY_P)) {
            *pause_animation = !(*pause_animation);
        }

        if ((*current_room)->class == ENEMIES) {
            if (!(*pause_animation)) {
                *current_anim_frame = (gui_state->framesCounter)%60;
            }
        }

        // Press enter to change to FLOOR_VIEW screen
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
            log_tag("debug_log.txt", "DEBUG", "%s():    setting current_room to NULL", __func__);
            *current_room = NULL;
            gui_state->currentScreen = FLOOR_VIEW;
        }

    }
    break;
    case ENDING: {
        // TODO: Update ENDING screen variables here!

        // Press enter to quit the game
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
            //Reset load_info->is_new_game to -1
            log_tag("debug_log.txt", "DEBUG", "%s():    Quitting", __func__);
            fprintf(stderr, "[DEBUG] [%s()]    Quitting\n", __func__);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_SUCCESS);
        }
    }
    break;
    case DOOR_ANIM: {
        // TODO: Update DOOR_ANIM screen variables here!
        (gui_state->framesCounter) += 1;    // Count frames
        // TODO: Press enter to skip animation and go to room screen?
        if (*current_anim_frame == 59 ) { //|| IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
            gui_state->currentScreen = ROOM_VIEW;
            break;
        }
        (*current_anim_frame)++;
    }
    break;
    default: {
    }
    break;
    }
}

// void draw_GameScreen_Texture(RenderTexture2D target_txtr, GameScreen currentScreen, float gameScreenWidth, float gameScreenHeight, Vector2 mouse, Vector2 virtualMouse, int framesCounter, int fps_target, int current_anim_frame, Floor* current_floor, int current_x, int current_y, float scale, loadInfo* load_info, int saveslot_index, char current_save_path[1000])
void draw_GameScreen_Texture(RenderTexture2D target_txtr, Gui_State gui_state, int fps_target, int current_anim_frame, Floor* current_floor, Path* game_path, Fighter* player, Room* current_room, Gamestate* gamestate, int current_x, int current_y, loadInfo* load_info, int saveslot_index, char current_save_path[1500], char seed[PATH_SEED_BUFSIZE+1])
{
    BeginTextureMode(target_txtr);
    ClearBackground(RAYWHITE);
    switch(gui_state.currentScreen) {
    case LOGO: {
        // TODO: Draw LOGO screen here!
        DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
        DrawText("WIP", 20, 50, 20, RED);
        DrawText("WAIT for 2 SECONDS...", 20, 90, 20, GRAY);
#ifdef KOLISEO_HAS_TITLE
        for (int i = 0;  i <  KLS_TITLEROWS+1; i++) {
            DrawText(kls_title[i], 0, i*12, 12, BLACK);
        }
#endif // KOLISEO_HAS_TITLE
    }
    break;
    case TITLE: {
        // TODO: Draw TITLE screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, GREEN);
        DrawText(TextFormat("Default Mouse: [%i, %i]", (int)gui_state.mouse.x, (int)gui_state.mouse.y), 350, 25, 20, WHITE);
        DrawText(TextFormat("Virtual Mouse: [%i, %i]", (int)gui_state.virtualMouse.x, (int)gui_state.virtualMouse.y), 350, 55, 20, YELLOW);
        DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
        DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("Controls for FLOOR_VIEW screen", 110, 100, 20, MAROON);
        DrawText("Arrow keys to move", 110, 130, 20, MAROON);
        DrawText("PRESS R to regen floor", 110, 160, 20, MAROON);
        DrawText("PRESS P to pause animations", 110, 190, 20, MAROON);
        DrawText("PRESS Left_Alt + F to toggle fullscreen", 110, 220, 20, MAROON);
        DrawText("PRESS ENTER or TAP to go to FLOOR_VIEW SCREEN", 110, 280, 20, DARKGREEN);

        char txt[30] = {0};
        char txt_b[30] = {0};
        char txt_s4c[30] = {0};
        int txt_StartX = gui_state.gameScreenWidth * 0.4f;
        int txt_StartY = gui_state.gameScreenHeight * 0.85f;
        DrawRectangle(txt_StartX, txt_StartY, gui_state.gameScreenWidth - txt_StartX, gui_state.gameScreenHeight - txt_StartY, YELLOW);
        sprintf(txt,"Koliseo API version: %i\n", int_koliseo_version());
        DrawText(txt, txt_StartX + ( txt_StartX * 0.16), txt_StartY, 20, BLACK);
        sprintf(txt_b,"Koliseo version: %s\n", string_koliseo_version());
        DrawText(txt_b, txt_StartX + ( txt_StartX * 0.16), txt_StartY + 20, 20, BLACK);
        sprintf(txt_s4c,"s4c-animate version: %s\n", S4C_ANIMATE_VERSION );
        DrawText(txt_s4c, txt_StartX + ( txt_StartX * 0.16), txt_StartY + 40, 20, BLACK);
    }
    break;
    case SAVES_VIEW : {
        // TODO: Draw SAVES_VIEW screen here!
        switch(load_info->is_new_game) {
        case -1: { // User has to pick new (1) or load (0)
            DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, RAYWHITE);
            DrawText(TextFormat("Default Mouse: [%i, %i]", (int)gui_state.mouse.x, (int)gui_state.mouse.y), 350, 25, 20, WHITE);
            DrawText(TextFormat("Virtual Mouse: [%i, %i]", (int)gui_state.virtualMouse.x, (int)gui_state.virtualMouse.y), 350, 55, 20, YELLOW);
            DrawText("PICK NEW/LOAD GAME SCREEN", 20, 20, 40, DARKGREEN);
            DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
            for (int i=BUTTON_NEW_GAME; i < BUTTON_LOAD_GAME+1; i++) {
                DrawRectangleRec(gui_state.buttons[i].r, gui_state.buttons[i].box_color);
                DrawText(gui_state.buttons[i].label, gui_state.buttons[i].r.x + (gui_state.gameScreenWidth * 0.02f), gui_state.buttons[i].r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, gui_state.buttons[i].text_color);
            }
            //DrawText("PRESS N for new game, L to load a game.", 110, 220, 20, DARKGREEN);
        }
        break;
        case 0: {
            if (saveslot_index == -1) {  // Pick saveslot
                DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, WHITE);
                DrawText(TextFormat("Default Mouse: [%i, %i]", (int)gui_state.mouse.x, (int)gui_state.mouse.y), 350, 25, 20, WHITE);
                DrawText(TextFormat("Virtual Mouse: [%i, %i]", (int)gui_state.virtualMouse.x, (int)gui_state.virtualMouse.y), 350, 55, 20, YELLOW);
                DrawText("LOADED GAME SCREEN", 20, 20, 40, DARKGREEN);
                DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
                //DrawText("PRESS 1-3 to pick a saveslot", 110, 220, 20, DARKGREEN);
                for (int i=BUTTON_SAVESLOT_1; i < BUTTON_SAVESLOT_3 +1; i++) {
                    Gui_Button button = gui_state.buttons[i];
                    if (button.state == BUTTON_HOVER) {
                        DrawRectangleRec(button.r, RED);
                    } else {
                        DrawRectangleRec(button.r, button.box_color);
                    }
                    DrawText(button.label, button.r.x + (gui_state.gameScreenWidth * 0.02f), button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, button.text_color);
                }
            } else {
                DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, RED);
            }
        }
        break;
        case 1: {
            if (saveslot_index == -1) {  // Pick saveslot
                DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, WHITE);
                DrawText(TextFormat("Default Mouse: [%i, %i]", (int)gui_state.mouse.x, (int)gui_state.mouse.y), 350, 25, 20, WHITE);
                DrawText(TextFormat("Virtual Mouse: [%i, %i]", (int)gui_state.virtualMouse.x, (int)gui_state.virtualMouse.y), 350, 55, 20, YELLOW);
                DrawText("NEW GAME SCREEN", 20, 20, 40, DARKGREEN);
                DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
                for (int i=BUTTON_SAVESLOT_1; i < BUTTON_SAVESLOT_3 +1; i++) {
                    Gui_Button button = gui_state.buttons[i];
                    if (button.state == BUTTON_HOVER) {
                        DrawRectangleRec(button.r, RED);
                    } else {
                        DrawRectangleRec(button.r, button.box_color);
                    }
                    DrawText(button.label, button.r.x + (gui_state.gameScreenWidth * 0.02f), button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, button.text_color);
                }
                //DrawText("PRESS 1-3 to pick a saveslot", 110, 220, 20, DARKGREEN);
            } else {
                DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, RED);
            }
        }
        break;
        default: {
            fprintf(stderr,"%s():    unexpected value for load_info->is_new_game: [%i]\n", __func__, load_info->is_new_game);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
        break;
        }
    }
    break;
    case NAMEPICK_VIEW: {

        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, RAYWHITE);
        DrawText("PICK NAME SCREEN", 20, 20, 40, DARKGREEN);
        DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));

        DrawRectangleRec(gui_state.buttons[BUTTON_NAME_TXTFIELD].r, gui_state.buttons[BUTTON_NAME_TXTFIELD].box_color);

        Gui_Button namefield = gui_state.buttons[BUTTON_NAME_TXTFIELD];

        if (namefield.on) {
            DrawRectangleLines((int)namefield.r.x, (int)namefield.r.y, (int)namefield.r.width, (int)namefield.r.height, RED);
        } else {
            DrawRectangleLines((int)namefield.r.x, (int)namefield.r.y, (int)namefield.r.width, (int)namefield.r.height, DARKGRAY);
        }

        DrawText(namefield.label, (int)namefield.r.x + 5, (int) namefield.r.y + 8, 20, MAROON);

        DrawText(TextFormat("INPUT CHARS: %i/%i", namefield.label_len, 20), 315, 250, 20, DARKGRAY);

        if (namefield.on) {
            if (namefield.label_len < 20) {
                if ((gui_state.framesCounter%120) == 0) {
                    DrawText("_", (int)namefield.r.x + 8 + MeasureText(namefield.label, 20), (int) namefield.r.y + 12, 20, MAROON);
                }
            } else {
                DrawText("Press BACKSPACE to delete...", 230, 300, 20, GRAY);
            }
        }
    }
    break;
    case CLASSPICK_VIEW: {
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, RAYWHITE);
        DrawText("PICK CLASS SCREEN", 20, 20, 40, DARKGREEN);
        DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
        for (int i=BUTTON_CLASS_KNIGHT; i < BUTTON_CLASS_ASSASSIN+1; i++) {
            Gui_Button button = gui_state.buttons[i];
            if (button.state == BUTTON_HOVER) {
                DrawRectangleRec(button.r, RED);
            } else {
                DrawRectangleRec(button.r, button.box_color);
            }
            DrawText(button.label, button.r.x + (gui_state.gameScreenWidth * 0.02f), button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, button.text_color);
        }
    }
    break;
    case FLOOR_VIEW: {
        // TODO: Draw FLOOR_VIEW screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, RAYWHITE);
        DrawText("FLOOR_VIEW SCREEN", 20, 20, 40, MAROON);
        DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText(TextFormat("Current save path: {%s}", current_save_path), 110, 100, 20, MAROON);
        if (game_path != NULL) {
            DrawText(TextFormat("Current seed: {%s}", game_path->seed), 110, 130, 20, MAROON);
        }
        DrawText("Controls for FLOOR_VIEW screen", 110, 160, 20, MAROON);
        DrawText("Arrow keys to move", 110, 190, 20, MAROON);
        DrawText("PRESS R to regen floor", 110, 220, 20, MAROON);
        DrawText("PRESS ENTER or TAP to go to ENDING SCREEN", 110, 280, 20, MAROON);

        Rectangle floor_r = CLITERAL(Rectangle) {
            gui_state.gameScreenHeight *0.5f,
                                       gui_state.gameScreenWidth *0.5f,
                                       FLOOR_MAX_COLS * (gui_state.gameScreenWidth*0.01f),
                                       FLOOR_MAX_ROWS * (gui_state.gameScreenWidth*0.01f),
        };

        //DrawRectangleRec(floor_r, ColorFromS4CPalette(palette, S4C_SALMON));

        if (G_EXPERIMENTAL_ON != 1) {
            if (current_floor != NULL) {
                draw_floor_view(current_floor, current_x, current_y, gui_state.gameScreenWidth*0.01f, &floor_r);
            } else {
                log_tag("debug_log.txt", "DEBUG", "%s():    current_floor was NULL.", __func__);
            }
        } else {
            if (current_floor != NULL) {
                display_roomclass_layout(current_floor, &floor_r, gui_state.gameScreenWidth*0.01f);
            } else {
                log_tag("debug_log.txt", "DEBUG", "%s():    current_floor was NULL.", __func__);
            }
        }

        Rectangle map_r = CLITERAL(Rectangle) {
            gui_state.gameScreenHeight *0.85f,
                                       gui_state.gameScreenWidth *0.5f,
                                       FLOOR_MAX_COLS * (gui_state.gameScreenWidth*0.01f),
                                       FLOOR_MAX_ROWS * (gui_state.gameScreenWidth*0.01f),
        };


        /*
        int center_x = FLOOR_MAX_COLS / 2;
        int center_y = FLOOR_MAX_ROWS / 2;
        draw_floor_view(current_floor, center_x, center_y, sprite_w_factor, &floor_r);
        */
        //display_floor_layout(current_floor, &map_r, gameScreenWidth*0.01f);
        //display_floor_layout_with_player(current_floor, &map_r, current_x, current_y, gameScreenWidth*0.01f);
        if (current_floor != NULL) {
            display_explored_layout_with_player(current_floor, &map_r, current_x, current_y, gui_state.gameScreenWidth*0.01f);
        } else {
            log_tag("debug_log.txt", "DEBUG", "%s():    current_floor was NULL.", __func__);
        }

        //display_explored_layout(current_floor, &floor_r, sprite_w_factor);
        /*
        Rectangle en_pl_coll = GetCollisionRec(en_r,pl_r);
        Rectangle st_pl_coll = GetCollisionRec(stats_label_r,pl_r);
        Rectangle st_en_coll = GetCollisionRec(stats_label_r,en_r);
        */
        // Draw collision boxes
        //DrawRectangleRec(en_pl_coll, ColorFromS4CPalette(palette, S4C_TEAL));
        //DrawRectangleRec(st_pl_coll, ColorFromS4CPalette(palette, S4C_MAGENTA));
        //DrawRectangleRec(st_en_coll, ColorFromS4CPalette(palette, S4C_CYAN));
        /* Draw expected boxes
        Color en_c = ColorFromS4CPalette(palette, S4C_CYAN);
        Color pl_c = ColorFromS4CPalette(palette, S4C_MAGENTA);
        Color st_c = ColorFromS4CPalette(palette, S4C_TEAL);
        en_c.a = 125;
        pl_c.a = 125;
        st_c.a = 125;
        DrawRectangleRec(en_r, en_c);
        DrawRectangleRec(pl_r, pl_c);
        DrawRectangleRec(stats_label_r, st_c);
        */
    }
    break;
    case ROOM_VIEW: {
        // TODO: Draw ROOM_VIEW screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, ColorFromS4CPalette(palette,S4C_CYAN));
        DrawText(TextFormat("Default Mouse: [%i, %i]", (int)gui_state.mouse.x, (int)gui_state.mouse.y), 350, 25, 20, WHITE);
        DrawText(TextFormat("Virtual Mouse: [%i, %i]", (int)gui_state.virtualMouse.x, (int)gui_state.virtualMouse.y), 350, 55, 20, YELLOW);
        DrawText("ROOM SCREEN", 20, 20, 40, DARKGREEN);
        if (current_room != NULL) {
            DrawText(TextFormat("Room Type: {%s}", stringFromRoom(current_room->class)), 20, 80, 20, DARKGREEN);
            switch (current_room->class) {
            case ENEMIES: {
                int pl_rect_Y = gui_state.gameScreenHeight * 0.1f;
                int pl_frame_W = gui_state.gameScreenWidth * 0.2f;
                int pl_frame_H = pl_frame_W;
                int pl_rect_X = gui_state.gameScreenWidth - pl_frame_W;
                int en_rect_X = gui_state.gameScreenWidth *0.1f;
                int en_rect_Y = pl_rect_Y;
                int en_frame_W = pl_frame_W;
                int en_frame_H = pl_frame_H;
                float stats_label_W = gui_state.gameScreenWidth * 0.1f;
                float stats_label_H = stats_label_W;
                Rectangle stats_label_r = CLITERAL(Rectangle) {
                    gui_state.gameScreenWidth*0.5f - (stats_label_W/2),
                                              en_rect_Y,
                                              stats_label_W,
                                              stats_label_H
                };
                Rectangle pl_r = CLITERAL(Rectangle) {
                    pl_rect_X,
                    pl_rect_Y,
                    pl_frame_W,
                    pl_frame_H
                };
                Rectangle en_r = CLITERAL(Rectangle) {
                    en_rect_X,
                    en_rect_Y,
                    en_frame_W,
                    en_frame_H
                };
                //TODO: count time by real_clock difference from last frame
                time_t framesTime = gui_state.framesCounter / fps_target ;// GetFPS();
                struct tm* time_tm = localtime(&framesTime);
                char time_str[20] = {0};

                if (time_tm == NULL) {
                    fprintf(stderr, "%s():    time_tm was NULL.\n", __func__);
                } else {
                    strftime(time_str, 20, "Time: %M:%S", time_tm);
                    DrawText(time_str, 0, 0, 20, ColorFromS4CPalette(palette, S4C_MAGENTA));
                }
                DrawRectangleRec(stats_label_r, ColorFromS4CPalette(palette, S4C_GREY));

                fighterClass player_class = player->class;
                int pl_res = -1;
                switch (player_class) {
                case Knight: {
                    pl_res = DrawSpriteRect(knight_tapis[current_anim_frame], pl_r, 17, 17, pl_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Archer: {
                    pl_res = DrawSpriteRect(archer_drop[current_anim_frame], pl_r, 17, 17, pl_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Mage: {
                    pl_res = DrawSpriteRect(mage_spark[current_anim_frame], pl_r, 17, 17, pl_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Assassin: {
                    pl_res = DrawSpriteRect(assassin_poof[current_anim_frame], pl_r, 17, 17, pl_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                default: {
                    log_tag("debug_log.txt", "ERROR", "%s():    Unexpected player_class: {%i}", __func__, player_class);
                    fprintf(stderr, "[ERROR] [%s()]    Unexpected player_class: {%i}\n", __func__, player_class);
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                }
                break;
                }
                int en_res = -1;
                enemyClass enemy_class = current_room->enemies[gamestate->current_enemy_index]->class;
                switch (enemy_class) {
                case Mummy: {
                    en_res = DrawSpriteRect(mummy_shuffle[current_anim_frame], en_r, 17, 17, en_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Ghost: {
                    en_res = DrawSpriteRect(ghost_spell[current_anim_frame], en_r, 17, 17, en_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Zombie: {
                    en_res = DrawSpriteRect(zombie_walk[current_anim_frame], en_r, 17, 17, en_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Goblin: {
                    en_res = DrawSpriteRect(goblin_shoot[current_anim_frame], en_r, 17, 17, en_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Imp: {
                    en_res = DrawSpriteRect(imp_fireball[current_anim_frame], en_r, 17, 17, en_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Troll: {
                    en_res = DrawSpriteRect(troll_club[current_anim_frame], en_r, 17, 17, en_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Boar: {
                    en_res = DrawSpriteRect(boar_scream[current_anim_frame], en_r, 17, 17, en_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Werewolf: {
                    en_res = DrawSpriteRect(werewolf_transform[current_anim_frame], en_r, 17, 17, en_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                default: {
                    log_tag("debug_log.txt", "ERROR", "%s():    Unexpected enemy_class: {%i}", __func__, enemy_class);
                    fprintf(stderr, "[ERROR] [%s()]    Unexpected enemy_class: {%i}\n", __func__, enemy_class);
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                }
                break;
                }
                if (pl_res != 0 || en_res != 0 || CheckCollisionRecs(en_r,stats_label_r) || CheckCollisionRecs(stats_label_r,pl_r) || CheckCollisionRecs(en_r,pl_r)) {
                    DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, ColorFromS4CPalette(palette, S4C_RED));
                    DrawText("Window too small.", 20, 20, 20, RAYWHITE);
                    DrawText("Please resize.", 20, 50, 20, RAYWHITE);
                }
            }
            break;
            default: {
                log_tag("debug_log.txt", "ERROR", "%s():    Unexpected roomClass value: {%i}", __func__, current_room->class);
                if (current_room->class > 0) {
                    fprintf(stderr, "[ERROR] [%s()]    Unexpected roomClass value: {%i} {%s}\n", __func__, current_room->class, stringFromRoom(current_room->class));
                } else {
                    fprintf(stderr, "[ERROR] [%s()]    Unexpected roomClass value: {%i}\n", __func__, current_room->class);
                }

                /*
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
                */
            }
            break;
            }
        }
        DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS ENTER or TAP to go to FLOOR_VIEW SCREEN", 110, 220, 20, DARKGREEN);
        DrawText("PRESS P to pause animations", 110, 350, 20, MAROON);
        DrawText("PRESS Left_Alt + F to toggle fullscreen", 110, 390, 20, MAROON);
    }
    break;
    case ENDING: {
        // TODO: Draw ENDING screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, BLUE);
        DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS ENTER or TAP to RETURN to quit the game", 120, 220, 20, DARKBLUE);
    }
    break;
    case DOOR_ANIM: {
        // TODO: Draw DOOR_ANIM screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, ColorFromS4CPalette(palette,S4C_TEAL));
        DrawText("DOOR SCREEN", 20, 20, 40, DARKBLUE);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS ENTER or TAP to RETURN to FLOOR_VIEW SCREEN", 120, 220, 20, DARKBLUE);

        int door_frame_W = 21;
        int door_frame_H = 21;
        int door_rect_X = (gui_state.gameScreenWidth/2) - ((door_frame_W * gui_state.scale * 5.5) /2);
        int door_rect_Y = (gui_state.gameScreenHeight/2) - ((door_frame_H * gui_state.scale * 5.5) /2);
        Rectangle door_r = CLITERAL(Rectangle) {
            door_rect_X,
            door_rect_Y,
            door_frame_W * gui_state.scale * 5.5,
            door_frame_H * gui_state.scale * 5.5,
        };
        int door_res = DrawSpriteRect(enter_door[current_anim_frame], door_r, door_frame_H, door_frame_W, gui_state.scale*5.5, palette, PALETTE_S4C_H_TOTCOLORS);
        if (door_res != 0 ) {
            DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, ColorFromS4CPalette(palette, S4C_RED));
            DrawText("Window too small.", 20, 20, 20, RAYWHITE);
            DrawText("Please resize.", 20, 50, 20, RAYWHITE);
            //current_anim_frame--; // TODO: can't update the current animation frame since it's not being taken as a reference as of now.
        }
    }
    break;
    default: {
        break;
    }
    }

    EndTextureMode();
}
