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
    if (e->class < EQUIPSMAX + 1) {
        for (int i = 0; i < 8; i++) {
            strcpy(e->sprite[i], equips_sprites[e->class][i]);
        }
    } else {
        fprintf(stderr,
                "[ERROR]    Unexpected equipClass in setEquipSprite().\n");
        exit(EXIT_FAILURE);
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

void update_GameScreen(float* scale, float gameScreenWidth, float gameScreenHeight, GameScreen* currentScreen, int* framesCounter, Floor** current_floor, int* current_x, int* current_y, int logo_sleep, bool* pause_animation, Koliseo_Temp** floor_kls, KLS_Conf temporary_kls_conf, int* current_anim_frame, Vector2* mouse, Vector2* virtualMouse)
{
    int center_x = FLOOR_MAX_COLS / 2;
    int center_y = FLOOR_MAX_ROWS / 2;
    *scale = MIN((float)GetScreenWidth()/gameScreenWidth, (float)GetScreenHeight()/gameScreenHeight);

    *mouse = GetMousePosition();

    *virtualMouse = (Vector2) {
        0
    };

    virtualMouse->x = (mouse->x - (GetScreenWidth() - (gameScreenWidth*(*scale)))*0.0f)/ (*scale);
    virtualMouse->y = (mouse->y - (GetScreenHeight() - (gameScreenHeight* (*scale)))*0.0f)/ (*scale);
    *virtualMouse = Vector2Clamp(*virtualMouse, (Vector2) {
        0, 0
    }, (Vector2) {
        (float)gameScreenWidth, (float)gameScreenHeight
    } );

    if (IsKeyPressed(KEY_F) && IsKeyDown(KEY_LEFT_ALT)) {
        ToggleFullScreenWindow(gameScreenWidth, gameScreenHeight);
    }

    switch(*currentScreen) {
    case LOGO: {
        // TODO: Update LOGO screen variables here!

        (*framesCounter)++;    // Count frames

        // Wait for 2 seconds (120 frames) before jumping to TITLE screen
        if (*framesCounter > logo_sleep) {
            *currentScreen = TITLE;
        }
    }
    break;
    case TITLE: {
        // TODO: Update TITLE screen variables here!

        // Press enter to change to FLOOR_VIEW screen
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
            *currentScreen = FLOOR_VIEW;
        }
    }
    break;
    case FLOOR_VIEW: {
        // TODO: Update FLOOR_VIEW screen variables here!
        (*framesCounter)++;    // Count frames

        // Press enter to change to ENDING screen
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
            *currentScreen = ENDING;
        }
        if (IsKeyPressed(KEY_P)) {
            *pause_animation = !(*pause_animation);
        }
        if (IsKeyPressed(KEY_R)) {
            fprintf(stderr,"%s\n", "Regenerating current floor");
            kls_temp_end(*floor_kls);
            kls_free(temporary_kls);
            temporary_kls = kls_new_conf(KLS_DEFAULT_SIZE * 32, temporary_kls_conf);
            *current_floor =
                (Floor *) KLS_PUSH_TYPED(temporary_kls, Floor,
                                         HR_Floor, "Floor", "Floor");
            // Init dbg_floor
            init_floor_layout(*current_floor);

            //Set center as filled
            (*current_floor)->floor_layout[center_x][center_y] = 1;

            //Init floor rooms
            init_floor_rooms(*current_floor);
            (*floor_kls) = kls_temp_start(temporary_kls);

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

        }
        if (IsKeyPressed(KEY_UP)) {
            step_floor(*current_floor, current_x,
                       current_y, KEY_UP);
            if ((*current_floor)->roomclass_layout[*current_x][*current_y] != BASIC) {
                *currentScreen = DOOR_ANIM;
                *current_anim_frame = 0;
                break;
            }
        }
        if (IsKeyPressed(KEY_DOWN)) {
            step_floor(*current_floor, current_x,
                       current_y, KEY_DOWN);
            if ((*current_floor)->roomclass_layout[*current_x][*current_y] != BASIC) {
                *currentScreen = DOOR_ANIM;
                *current_anim_frame = 0;
                break;
            }
        }
        if (IsKeyPressed(KEY_LEFT)) {
            step_floor(*current_floor, current_x,
                       current_y, KEY_LEFT);
            if ((*current_floor)->roomclass_layout[*current_x][*current_y] != BASIC) {
                *currentScreen = DOOR_ANIM;
                *current_anim_frame = 0;
                break;
            }
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            step_floor(*current_floor, current_x,
                       current_y, KEY_RIGHT);
            if ((*current_floor)->roomclass_layout[*current_x][*current_y] != BASIC) {
                *currentScreen = DOOR_ANIM;
                *current_anim_frame = 0;
                break;
            }
        }
        if (!(*pause_animation)) {
            *current_anim_frame = (*framesCounter)%60;
        }
    }
    break;
    case ROOM_VIEW: {
        // TODO: Update ROOM_VIEW screen variables here!

        // Press enter to change to FLOOR_VIEW screen
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
            *currentScreen = FLOOR_VIEW;
        }
    }
    break;
    case ENDING: {
        // TODO: Update ENDING screen variables here!

        // Press enter to return to TITLE screen
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
            *currentScreen = TITLE;
        }
    }
    break;
    case DOOR_ANIM: {
        // TODO: Update DOOR_ANIM screen variables here!
        (*framesCounter)++;    // Count frames
        // TODO: Press enter to skip animation and go to room screen?
        if (*current_anim_frame == 59 ) { //|| IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
            *currentScreen = ROOM_VIEW;
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

void draw_GameScreen_Texture(RenderTexture2D target_txtr, GameScreen currentScreen, float gameScreenWidth, float gameScreenHeight, Vector2 mouse, Vector2 virtualMouse, int framesCounter, int fps_target, int current_anim_frame, Floor* current_floor, int current_x, int current_y, float scale)
{
    BeginTextureMode(target_txtr);
    ClearBackground(RAYWHITE);
    switch(currentScreen) {
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
        DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, GREEN);
        DrawText(TextFormat("Default Mouse: [%i, %i]", (int)mouse.x, (int)mouse.y), 350, 25, 20, WHITE);
        DrawText(TextFormat("Virtual Mouse: [%i, %i]", (int)virtualMouse.x, (int)virtualMouse.y), 350, 55, 20, YELLOW);
        DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
        DrawText("WIP", 20, gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS ENTER or TAP to JUMP to FLOOR_VIEW SCREEN", 110, 220, 20, DARKGREEN);
        DrawText("Controls for FLOOR_VIEW screen", 110, 250, 20, MAROON);
        DrawText("Arrow keys to move", 110, 280, 20, MAROON);
        DrawText("PRESS R to regen floor", 110, 310, 20, MAROON);
        DrawText("PRESS P to pause animations", 110, 350, 20, MAROON);
        DrawText("PRESS Left_Alt + F to toggle fullscreen", 110, 390, 20, MAROON);

        char txt[30] = {0};
        char txt_b[30] = {0};
        char txt_s4c[30] = {0};
        int txt_StartX = gameScreenWidth * 0.4f;
        int txt_StartY = gameScreenHeight * 0.85f;
        DrawRectangle(txt_StartX, txt_StartY, gameScreenWidth - txt_StartX, gameScreenHeight - txt_StartY, YELLOW);
        sprintf(txt,"Koliseo API version: %i\n", int_koliseo_version());
        DrawText(txt, txt_StartX + ( txt_StartX * 0.16), txt_StartY, 20, BLACK);
        sprintf(txt_b,"Koliseo version: %s\n", string_koliseo_version());
        DrawText(txt_b, txt_StartX + ( txt_StartX * 0.16), txt_StartY + 20, 20, BLACK);
        sprintf(txt_s4c,"s4c-animate version: %s\n", S4C_ANIMATE_VERSION );
        DrawText(txt_s4c, txt_StartX + ( txt_StartX * 0.16), txt_StartY + 40, 20, BLACK);
    }
    break;
    case FLOOR_VIEW: {
        // TODO: Draw FLOOR_VIEW screen here!
        DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, RAYWHITE);
        DrawText("FLOOR_VIEW SCREEN", 20, 20, 40, MAROON);
        DrawText("WIP", 20, gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 110, 240, 20, MAROON);
        int pl_rect_Y = gameScreenHeight * 0.1f;
        int pl_frame_W = gameScreenWidth * 0.2f;
        int pl_frame_H = pl_frame_W;
        int pl_rect_X = gameScreenWidth - pl_frame_W;
        int en_rect_X = gameScreenWidth *0.1f;
        int en_rect_Y = pl_rect_Y;
        int en_frame_W = pl_frame_W;
        int en_frame_H = pl_frame_H;
        float stats_label_W = gameScreenWidth * 0.1f;
        float stats_label_H = stats_label_W;
        Rectangle stats_label_r = CLITERAL(Rectangle) {
            gameScreenWidth*0.5f - (stats_label_W/2),
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
        time_t framesTime = framesCounter / fps_target ;// GetFPS();
        struct tm* time_tm = localtime(&framesTime);
        char time_str[20] = {0};

        if (time_tm == NULL) {
            fprintf(stderr, "%s():    time_tm was NULL.\n", __func__);
        } else {
            strftime(time_str, 20, "Time: %M:%S", time_tm);
            DrawText(time_str, 0, 0, 20, ColorFromS4CPalette(palette, S4C_MAGENTA));
        }
        DrawRectangleRec(stats_label_r, ColorFromS4CPalette(palette, S4C_GREY));
        int pl_res = DrawSpriteRect(mage_spark[current_anim_frame], pl_r, 17, 17, pl_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
        int en_res = DrawSpriteRect(zombie_walk[current_anim_frame], en_r, 17, 17, en_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);

        Rectangle floor_r = CLITERAL(Rectangle) {
            gameScreenHeight *0.5f,
                             gameScreenWidth *0.5f,
                             FLOOR_MAX_COLS * (gameScreenWidth*0.01f),
                             FLOOR_MAX_ROWS * (gameScreenWidth*0.01f),
        };

        //DrawRectangleRec(floor_r, ColorFromS4CPalette(palette, S4C_SALMON));

        if (G_EXPERIMENTAL_ON != 1) {
            draw_floor_view(current_floor, current_x, current_y, gameScreenWidth*0.01f, &floor_r);
        } else {
            display_roomclass_layout(current_floor, &floor_r, gameScreenWidth*0.01f);
        }

        Rectangle map_r = CLITERAL(Rectangle) {
            gameScreenHeight *0.85f,
                             gameScreenWidth *0.5f,
                             FLOOR_MAX_COLS * (gameScreenWidth*0.01f),
                             FLOOR_MAX_ROWS * (gameScreenWidth*0.01f),
        };


        /*
        int center_x = FLOOR_MAX_COLS / 2;
        int center_y = FLOOR_MAX_ROWS / 2;
        draw_floor_view(current_floor, center_x, center_y, sprite_w_factor, &floor_r);
        */
        //display_floor_layout(current_floor, &map_r, gameScreenWidth*0.01f);
        //display_floor_layout_with_player(current_floor, &map_r, current_x, current_y, gameScreenWidth*0.01f);
        display_explored_layout_with_player(current_floor, &map_r, current_x, current_y, gameScreenWidth*0.01f);
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

        if (pl_res != 0 || en_res != 0 || CheckCollisionRecs(en_r,stats_label_r) || CheckCollisionRecs(stats_label_r,pl_r) || CheckCollisionRecs(en_r,pl_r)) {
            DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, ColorFromS4CPalette(palette, S4C_RED));
            DrawText("Window too small.", 20, 20, 20, RAYWHITE);
            DrawText("Please resize.", 20, 50, 20, RAYWHITE);
        }
    }
    break;
    case ROOM_VIEW: {
        // TODO: Draw ROOM_VIEW screen here!
        DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, ColorFromS4CPalette(palette,S4C_CYAN));
        DrawText(TextFormat("Default Mouse: [%i, %i]", (int)mouse.x, (int)mouse.y), 350, 25, 20, WHITE);
        DrawText(TextFormat("Virtual Mouse: [%i, %i]", (int)virtualMouse.x, (int)virtualMouse.y), 350, 55, 20, YELLOW);
        DrawText("ROOM SCREEN", 20, 20, 40, DARKGREEN);
        DrawText("WIP", 20, gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS ENTER or TAP to JUMP to FLOOR_VIEW SCREEN", 110, 220, 20, DARKGREEN);
        DrawText("Controls for FLOOR_VIEW screen", 110, 250, 20, MAROON);
        DrawText("Arrow keys to move", 110, 280, 20, MAROON);
        DrawText("PRESS R to regen floor", 110, 310, 20, MAROON);
        DrawText("PRESS P to pause animations", 110, 350, 20, MAROON);
        DrawText("PRESS Left_Alt + F to toggle fullscreen", 110, 390, 20, MAROON);
    }
    break;
    case ENDING: {
        // TODO: Draw ENDING screen here!
        DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, BLUE);
        DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
        DrawText("WIP", 20, gameScreenHeight - (10 * scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);
    }
    break;
    case DOOR_ANIM: {
        // TODO: Draw DOOR_ANIM screen here!
        DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, ColorFromS4CPalette(palette,S4C_TEAL));
        DrawText("DOOR SCREEN", 20, 20, 40, DARKBLUE);
        DrawText("WIP", 20, gameScreenHeight - (10 * scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS ENTER or TAP to RETURN to FLOOR_VIEW SCREEN", 120, 220, 20, DARKBLUE);

        int door_frame_W = 21;
        int door_frame_H = 21;
        int door_rect_X = (gameScreenWidth/2) - ((door_frame_W * scale * 5.5) /2);
        int door_rect_Y = (gameScreenHeight/2) - ((door_frame_H * scale * 5.5) /2);
        Rectangle door_r = CLITERAL(Rectangle) {
            door_rect_X,
            door_rect_Y,
            door_frame_W * scale * 5.5,
            door_frame_H * scale * 5.5,
        };
        int door_res = DrawSpriteRect(enter_door[current_anim_frame], door_r, door_frame_H, door_frame_W, scale*5.5, palette, PALETTE_S4C_H_TOTCOLORS);
        if (door_res != 0 ) {
            DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, ColorFromS4CPalette(palette, S4C_RED));
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
