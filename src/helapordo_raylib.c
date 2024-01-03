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
#include "helapordo_raylib.h"

void gameloop_rl(int argc, char** argv) {
    printf("\n\t\t\tDISCLAIMER: THIS BUILD IS STILL WIP. NO GUARANTEES ARE MADE.\n\n");
    printf("helapordo v%s\n", VERSION);
    S4C_PRINTVERSION();
    printf("koliseo v%s\n", string_koliseo_version());
    printf("raylib v%s\n", RAYLIB_VERSION);

    int screenWidth = 800;
    int screenHeight = 450;
    int logo_sleep = 120;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "helapordo raylib test");

    GameScreen currentScreen = LOGO;

    // TODO: Initialize all required variables and load all required data here!

    int framesCounter = 0;          // Useful to count frames

    SetTargetFPS(60);               // Set desired framerate (frames-per-second)
                                    //
    int current_anim_frame = 0;

    while (!WindowShouldClose()) {
        // Update
        //----------------------------------------------------------------------------------

        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        switch(currentScreen)
        {
            case LOGO:
            {
                // TODO: Update LOGO screen variables here!

                framesCounter++;    // Count frames

                // Wait for 2 seconds (120 frames) before jumping to TITLE screen
                if (framesCounter > logo_sleep)
                {
                    currentScreen = TITLE;
                }
            } break;
            case TITLE:
            {
                // TODO: Update TITLE screen variables here!

                // Press enter to change to GAMEPLAY screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!
                framesCounter++;    // Count frames
                current_anim_frame = ( current_anim_frame < 60 ? current_anim_frame +1 : 0);

                // Press enter to change to ENDING screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = ENDING;
                }
            } break;
            case ENDING:
            {
                // TODO: Update ENDING screen variables here!

                // Press enter to return to TITLE screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = TITLE;
                }
            } break;
            default: break;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            switch(currentScreen)
            {
                case LOGO:
                {
                    // TODO: Draw LOGO screen here!
                    DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                    DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);
                    for (int i = 0;  i <  KLS_TITLEROWS+1; i++) {
                        DrawText(kls_title[i], 0, i*12, 12, BLACK);
                    }
                } break;
                case TITLE:
                {
                    // TODO: Draw TITLE screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                    DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                    DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

                    char txt[30] = {0};
                    char txt_b[30] = {0};
                    char txt_s4c[30] = {0};
                    int txt_StartX = screenWidth * 0.4;
                    int txt_StartY = screenHeight * 0.65;
                    DrawRectangle(txt_StartX, txt_StartY, screenWidth - txt_StartX, screenHeight - txt_StartY, YELLOW);
                    sprintf(txt,"Koliseo API version: %i\n", int_koliseo_version());
                    DrawText(txt, txt_StartX + ( txt_StartX * 0.16) , txt_StartY + ( txt_StartY * 0.25), 24, BLACK);
                    sprintf(txt_b,"Koliseo version: %s\n", string_koliseo_version());
                    DrawText(txt_b, txt_StartX + ( txt_StartX * 0.16), txt_StartY + ( txt_StartY * 0.35), 24, BLACK);
                    sprintf(txt_s4c,"s4c-animate version: %s\n", S4C_ANIMATE_VERSION );
                    DrawText(txt_s4c, txt_StartX + ( txt_StartX * 0.16), txt_StartY + ( txt_StartY * 0.45), 24, BLACK);
                } break;
                case GAMEPLAY:
                {
                    // TODO: Draw GAMEPLAY screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, RAYWHITE);
                    DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
                    DrawSprite(mage_spark[current_anim_frame], 17, 17, 24, palette, PALETTE_S4C_H_TOTCOLORS);
                } break;
                case ENDING:
                {
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                default: break;
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();
    return;
}
