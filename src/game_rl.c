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
#include "helapordo_raylib.h"

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
        ClearBackground(color_from_s4c_color(palette[S4C_GREY - S4C_BASE_COLOR_INDEX]));
    }
    rc = CLITERAL(Rectangle){1, 2, 20, 70};

    int fontSize = 20;
    const char* label = "Tutorial";;
    DrawText(label, rc.x, rc.y, fontSize, color_from_s4c_color(palette[S4C_CYAN - S4C_BASE_COLOR_INDEX]));

    Color tut_color = color_from_s4c_color(palette[S4C_LIGHT_YELLOW - S4C_BASE_COLOR_INDEX]);
    DrawText("You can use the arrow keys and Enter to do everything needed for the game.", 14, 3*fontSize, fontSize, tut_color);
    DrawText("Buying things from a Shop may be tricky: you have to select one, then choose Buy.\nTo select one, First go up/down to 'View Item', then press Enter,\nthen you can scroll them with left/right. Press Enter to confirm your selection,\nthen go back up to Buy.", 14, 6*fontSize, fontSize, tut_color);
    DrawText("When running in Rogue mode, you can change floors by killing a Boss.", 14, 12*fontSize, fontSize, tut_color);
    DrawText("When in floor map, you can open the menu with the \"m\" key.", 14, 15*fontSize, fontSize, tut_color);
    DrawText("Rememeber, you can't really save in Rogue mode ! Run with \"-s\" to try Story mode.", 14, 18*fontSize, fontSize, tut_color);

    DrawText("[ Press ENTER or TAP to quit ]", 14, 21*fontSize, fontSize, color_from_s4c_color(palette[S4C_RED - S4C_BASE_COLOR_INDEX]));

    if (G_DOTUTORIAL_ON == 1) {
        EndDrawing();
        //TODO: update win???
    }
}
