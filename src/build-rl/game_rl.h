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

#ifndef GAMECURSES_RL_H
#define GAMECURSES_RL_H

#include "../utils/game_utils.h"
#include "../core/sprites.h"
#include "../utils/turn_op.h"
#include "../core/game_animations.h"
#include <raymath.h> // Needed for: Vector2Clamp()

#define MIN(a, b) ((a) < (b) ? (a) : (b)) /**< Macro used to calculate the scale of render texture. */

typedef struct Gui_State {
    float scale;
    float gameScreenWidth;
    float gameScreenHeight;
    GameScreen currentScreen;
    int framesCounter;
    Vector2 mouse;
    Vector2 virtualMouse;
} Gui_State;

void handleTutorial(S4C_Color* palette);
void setEnemySprite(Enemy * e);
void setEquipSprite(Equip * e);
void setEquipslotSprite(Equipslot * s);
void setConsumableSprite(Consumable * c);
void setArtifactSprite(Artifact* a);
void setBossSprite(Boss * b);
void setFighterSprite(Fighter * f);
void setChestSprite(Chest * c);
void ToggleFullScreenWindow(int w_W, int w_H);
void update_GameScreen(Gui_State* gui_state, Floor** current_floor, Path** game_path, Fighter** player, Room** current_room, int* current_x, int* current_y, int logo_sleep, bool* pause_animation, Koliseo_Temp** floor_kls, KLS_Conf temporary_kls_conf, int* current_anim_frame, loadInfo* load_info, int* saveslot_index, char current_save_path[1000], char seed[PATH_SEED_BUFSIZE+1]);
void draw_GameScreen_Texture(RenderTexture2D target_txtr, Gui_State gui_state, int fps_target, int current_anim_frame, Floor* current_floor, Path* game_path, Fighter* player, Room* current_room, int current_x, int current_y, loadInfo* load_info, int saveslot_index, char current_save_path[1000], char seed[PATH_SEED_BUFSIZE+1]);
#endif // GAMECURSES_RL_H
