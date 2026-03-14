// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2022-2026 jgabaut

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
#include "../utils/rooms.h"
#include <raymath.h> // Needed for: Vector2Clamp()

#define MIN(a, b) ((a) < (b) ? (a) : (b)) /**< Macro used to calculate the scale of render texture. */

typedef enum Gui_Button_State {
    BUTTON_NORMAL,
    BUTTON_HOVER,
    BUTTON_PRESSED
} Gui_Button_State;

typedef struct Gui_Button {
    Rectangle r;
    bool on;
    Gui_Button_State state;
    char label[50];
    size_t label_len;
    Color box_color;
    Color text_color;
} Gui_Button;

typedef enum Gui_Button_Idx {
    BUTTON_NEW_GAME = 0,
    BUTTON_LOAD_GAME,
    BUTTON_SAVESLOT_1,
    BUTTON_SAVESLOT_2,
    BUTTON_SAVESLOT_3,
    BUTTON_CLASS_TXTFIELD,
    BUTTON_CLASS_KNIGHT,
    BUTTON_CLASS_ARCHER,
    BUTTON_CLASS_MAGE,
    BUTTON_CLASS_ASSASSIN,
    BUTTON_NAME_TXTFIELD,
    BUTTON_FIGHT,
    BUTTON_SPECIAL,
    BUTTON_EQUIPS,
    BUTTON_CONSUMABLES,
    BUTTON_OPEN_BAG,
    BUTTON_CHECK_LOADOUT,
    BUTTON_SPECIAL_1,
    BUTTON_SPECIAL_2,
    BUTTON_SPECIAL_3,
    BUTTON_SPECIAL_4,
    BUTTON_SPECIAL_UNLOCK_1,
    BUTTON_SPECIAL_UNLOCK_2,
    BUTTON_SPECIAL_UNLOCK_3,
    BUTTON_SPECIAL_UNLOCK_4,
    BUTTON_TAKE_TREASURE,
    BUTTON_LEAVE_TREASURE,
    BUTTON_FLOOR_DEBUG,
} Gui_Button_Idx;

#define GUI_BUTTONS_MAX BUTTON_FLOOR_DEBUG

typedef struct Gui_Theme {
    Color bg_color;
    Color txt_color;
} Gui_Theme;

typedef struct Gui_Button_Row {
    Gui_Button* buttons;
    int len;
    int x;
    int y;
    int cell_width;
    int cell_height;
    int cell_w_spacing;
} Gui_Button_Row;

typedef struct Gui_Button_Layout {
    Gui_Button_Row** rows;
    int len;
} Gui_Button_Layout;

typedef enum Gui_Shop_Layout_Row_Index {
    SHOP_LAYOUT_EQUIPS_ROW,
    SHOP_LAYOUT_CONSUMABLES_ROW
} Gui_Shop_Layout_Row_Index;

#define GUI_SHOP_LAYOUT_ROWS_MAX SHOP_LAYOUT_CONSUMABLES_ROW

typedef enum Gui_Fight_Layout_Button_Index {
    LAYOUT_BUTTON_FIGHT,
    LAYOUT_BUTTON_SPECIAL,
    LAYOUT_BUTTON_EQUIPS,
    LAYOUT_BUTTON_CONSUMABLES
} Gui_Fight_Layout_Button_Index;

#define GUI_FIGHT_LAYOUT_BUTTONS_MAX LAYOUT_BUTTON_CONSUMABLES

typedef struct Gui_State {
    float scale;
    float gameScreenWidth;
    float gameScreenHeight;
    GameScreen currentScreen;
    int framesCounter;
    Vector2 mouse;
    Vector2 virtualMouse;
    Gui_Button buttons[GUI_BUTTONS_MAX+1];
    Gui_Theme theme;
    int selectedIndex;
    Gui_Button_Layout shop_buttons;
    Gui_Button_Layout fight_buttons;
} Gui_State;

extern Gui_Button shop_equip_buttons[EQUIP_SHOP_MAX];
extern Gui_Button shop_consumable_buttons[CONSUMABLE_SHOP_MAX];
extern Gui_Button_Row shop_equip_buttons_row;
extern Gui_Button_Row shop_consumable_buttons_row;
extern Gui_Button_Row* shop_buttons_rows[GUI_SHOP_LAYOUT_ROWS_MAX+1];
extern Gui_Button_Layout shop_buttons_layout;

extern Gui_Button fight_buttons[GUI_FIGHT_LAYOUT_BUTTONS_MAX+1];
extern Gui_Button_Row fight_buttons_row;
extern Gui_Button_Row* fight_buttons_rows[1];
extern Gui_Button_Layout fight_buttons_layout;

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
void hlpd_draw_notifications(RingaBuf* rb_notifications, Rectangle notifications_rect);
void update_GameScreen(Gui_State* gui_state, Floor** current_floor, Path** game_path, Fighter** player, Room** current_room, Gamestate** gamestate, RingaBuf* rb_notifications, int* current_x, int* current_y, int logo_sleep, bool* pause_animation, Koliseo_Temp** floor_kls, KLS_Conf temporary_kls_conf, int* current_anim_frame, loadInfo* load_info, int* saveslot_index, char current_save_path[1500], char seed[PATH_SEED_BUFSIZE+1], bool is_seeded, int* roomsDone, int* enemyTotal);
void draw_GameScreen_Texture(RenderTexture2D target_txtr, Gui_State gui_state, int fps_target, int current_anim_frame, Floor* current_floor, Path* game_path, Fighter* player, Room* current_room, Gamestate* gamestate, RingaBuf* rb_notifications, int current_x, int current_y, loadInfo* load_info, int saveslot_index, char current_save_path[1500], char seed[PATH_SEED_BUFSIZE+1]);
void fight_Special(specialMove move, Rectangle *w, Fighter *f, Enemy *e, Boss *b,
                   Path *p, int roomIndex, int enemyIndex, int isBoss);
#endif // GAMECURSES_RL_H
