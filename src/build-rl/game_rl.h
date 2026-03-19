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

typedef struct Gui_Theme {
    Color bg_color;
    Color txt_color;
} Gui_Theme;

typedef struct Gui_Button_Group {
    Gui_Button* buttons;
    int len;
    int x;
    int y;
    int cell_width;
    int cell_height;
    int cell_w_spacing;
} Gui_Button_Group;

typedef struct Gui_Button_Layout {
    Gui_Button_Group** groups;
    int len;
} Gui_Button_Layout;

typedef enum Gui_TxtField_Group_Button_Index {
    BUTTON_CLASS_TXTFIELD,
    BUTTON_NAME_TXTFIELD,
} Gui_TxtField_Group_Button_Index;

#define GUI_TXTFIELD_GROUP_BUTTONS_MAX BUTTON_NAME_TXTFIELD
#define GUI_TXTFIELD_GROUP_BOX_COLOR { 200, 200, 200, 255 }
#define GUI_TXTFIELD_GROUP_TEXT_COLOR { 0, 0, 0, 255 }

typedef enum Gui_GamePick_Group_Button_Index {
    BUTTON_NEW_GAME,
    BUTTON_LOAD_GAME,
} Gui_GamePick_Group_Button_Index;

#define GUI_GAMEPICK_GROUP_BUTTONS_MAX BUTTON_LOAD_GAME
#define GUI_GAMEPICK_GROUP_BOX_COLOR { 0, 117, 44, 255 }
#define GUI_GAMEPICK_GROUP_TEXT_COLOR { 80, 80, 80, 255 }

typedef enum Gui_ClassPick_Group_Button_Index {
    BUTTON_CLASS_KNIGHT,
    BUTTON_CLASS_MAGE,
    BUTTON_CLASS_ARCHER,
    BUTTON_CLASS_ASSASSIN
} Gui_ClassPick_Group_Button_Index;

#define GUI_CLASSPICK_GROUP_BUTTONS_MAX BUTTON_CLASS_ASSASSIN
#define GUI_CLASSPICK_GROUP_BOX_COLOR GUI_GAMEPICK_GROUP_BOX_COLOR
#define GUI_CLASSPICK_GROUP_TEXT_COLOR GUI_GAMEPICK_GROUP_TEXT_COLOR

typedef enum Gui_SaveSlotPick_Group_Button_Index {
    BUTTON_SAVESLOT_1,
    BUTTON_SAVESLOT_2,
    BUTTON_SAVESLOT_3
} Gui_SaveSlotPick_Group_Button_Index;

#define GUI_SAVESLOTPICK_GROUP_BUTTONS_MAX BUTTON_SAVESLOT_3
#define GUI_SAVESLOTPICK_GROUP_BOX_COLOR GUI_GAMEPICK_GROUP_BOX_COLOR
#define GUI_SAVESLOTPICK_GROUP_TEXT_COLOR GUI_GAMEPICK_GROUP_TEXT_COLOR

typedef enum Gui_Shop_Layout_Group_Index {
    SHOP_LAYOUT_EQUIPS_GROUP,
    SHOP_LAYOUT_CONSUMABLES_GROUP,
    SHOP_LAYOUT_OTHERS_GROUP
} Gui_Shop_Layout_Group_Index;

#define GUI_SHOP_LAYOUT_GROUPS_MAX SHOP_LAYOUT_OTHERS_GROUP

typedef enum Gui_Shop_Others_Group_Button_Index {
    BUTTON_LEAVE_SHOP,
    BUTTON_SELL_ALL
} Gui_Shop_Others_Group_Button_Index;

#define GUI_SHOP_OTHERS_GROUP_BUTTONS_MAX BUTTON_SELL_ALL
#define GUI_SHOP_OTHERS_GROUP_BOX_COLOR { 0, 117, 44, 255 }
#define GUI_SHOP_OTHERS_GROUP_TEXT_COLOR { 80, 80, 80, 255 }

typedef enum Gui_Fight_Group_Button_Index {
    BUTTON_FIGHT,
    BUTTON_SPECIAL,
    BUTTON_EQUIPS,
    BUTTON_CONSUMABLES,
    BUTTON_STATS
} Gui_Fight_Group_Button_Index;

#define GUI_FIGHT_GROUP_BUTTONS_MAX BUTTON_STATS
#define GUI_FIGHT_GROUP_BOX_COLOR GUI_GAMEPICK_GROUP_BOX_COLOR
#define GUI_FIGHT_GROUP_TEXT_COLOR GUI_GAMEPICK_GROUP_TEXT_COLOR

typedef enum Gui_Special_Group_Button_Index {
    BUTTON_SPECIAL_1,
    BUTTON_SPECIAL_2,
    BUTTON_SPECIAL_3,
    BUTTON_SPECIAL_4
} Gui_Special_Group_Button_Index;

#define GUI_SPECIAL_GROUP_BUTTONS_MAX BUTTON_SPECIAL_4
#define GUI_SPECIAL_GROUP_BOX_COLOR GUI_FIGHT_GROUP_BOX_COLOR
#define GUI_SPECIAL_GROUP_TEXT_COLOR GUI_FIGHT_GROUP_TEXT_COLOR

typedef enum Gui_Equips_Group_Button_Index {
    BUTTON_OPEN_BAG,
    BUTTON_CHECK_LOADOUT,
} Gui_Equips_Group_Button_Index;

#define GUI_EQUIPS_GROUP_BUTTONS_MAX BUTTON_CHECK_LOADOUT
#define GUI_EQUIPS_GROUP_BOX_COLOR GUI_FIGHT_GROUP_BOX_COLOR
#define GUI_EQUIPS_GROUP_TEXT_COLOR GUI_FIGHT_GROUP_TEXT_COLOR

typedef enum Gui_Treasure_Group_Button_Index {
    BUTTON_TAKE_TREASURE,
    BUTTON_LEAVE_TREASURE
} Gui_Treasure_Group_Button_Index;

#define GUI_TREASURE_GROUP_BUTTONS_MAX BUTTON_LEAVE_TREASURE
#define GUI_TREASURE_GROUP_BOX_COLOR GUI_FIGHT_GROUP_BOX_COLOR
#define GUI_TREASURE_GROUP_TEXT_COLOR GUI_FIGHT_GROUP_TEXT_COLOR

#ifdef HELAPORDO_DEBUG_ACCESS
typedef enum Gui_Debug_Group_Button_Index {
    BUTTON_DEBUG,
} Gui_Debug_Group_Button_Index;

#define GUI_DEBUG_GROUP_BUTTONS_MAX BUTTON_DEBUG
#define GUI_DEBUG_GROUP_BOX_COLOR GUI_FIGHT_GROUP_BOX_COLOR
#define GUI_DEBUG_GROUP_TEXT_COLOR GUI_FIGHT_GROUP_TEXT_COLOR

typedef enum Gui_Debug_Fighter_Layout_Group_Index {
    DEBUG_FIGHTER_LAYOUT_SPECIALSLOTS_GROUP,
    DEBUG_FIGHTER_LAYOUT_SKILLSLOTS_GROUP,
    DEBUG_FIGHTER_LAYOUT_TURNCOUNTERS_GROUP,
    DEBUG_FIGHTER_LAYOUT_PERKS_GROUP,
    DEBUG_FIGHTER_LAYOUT_EQUIPSLOTS_GROUP,
    DEBUG_FIGHTER_LAYOUT_EQUIPSBAG_GROUP,
    DEBUG_FIGHTER_LAYOUT_CONSUMABLESBAG_GROUP,
    DEBUG_FIGHTER_LAYOUT_ARTIFACTSBAG_GROUP
} Gui_Debug_Fighter_Layout_Group_Index;

#define GUI_DEBUG_FIGHTER_LAYOUT_GROUPS_MAX DEBUG_FIGHTER_LAYOUT_ARTIFACTSBAG_GROUP
#endif // HELAPORDO_DEBUG_ACCESS

typedef struct Gui_State {
    float scale;
    float gameScreenWidth;
    float gameScreenHeight;
    GameScreen currentScreen;
    int framesCounter;
    Vector2 mouse;
    Vector2 virtualMouse;
    Gui_Theme theme;
    int selectedIndex;
    Gui_Button_Group txtfield_buttons;
    Gui_Button_Group gamepick_buttons;
    Gui_Button_Group classpick_buttons;
    Gui_Button_Group saveslotpick_buttons;
    Gui_Button_Group fight_buttons;
    Gui_Button_Group special_buttons;
    Gui_Button_Group equips_buttons;
    Gui_Button_Layout shop_buttons;
    Gui_Button_Group treasure_buttons;
#ifdef HELAPORDO_DEBUG_ACCESS
    Gui_Button_Group debug_buttons;
    Gui_Button_Layout debug_fighter_buttons;
#endif // HELAPORDO_DEBUG_ACCESS
} Gui_State;

extern Gui_Button txtfield_buttons[GUI_TXTFIELD_GROUP_BUTTONS_MAX+1];
extern Gui_Button_Group txtfield_buttons_group;

extern Gui_Button gamepick_buttons[GUI_GAMEPICK_GROUP_BUTTONS_MAX+1];
extern Gui_Button_Group gamepick_buttons_group;

extern Gui_Button classpick_buttons[GUI_CLASSPICK_GROUP_BUTTONS_MAX+1];
extern Gui_Button_Group classpick_buttons_group;

extern Gui_Button saveslotpick_buttons[GUI_SAVESLOTPICK_GROUP_BUTTONS_MAX+1];
extern Gui_Button_Group saveslotpick_buttons_group;

extern Gui_Button fight_buttons[GUI_FIGHT_GROUP_BUTTONS_MAX+1];
extern Gui_Button_Group fight_buttons_group;

extern Gui_Button special_buttons[GUI_SPECIAL_GROUP_BUTTONS_MAX+1];
extern Gui_Button_Group special_buttons_group;

extern Gui_Button equips_buttons[GUI_EQUIPS_GROUP_BUTTONS_MAX+1];
extern Gui_Button_Group equips_buttons_group;

extern Gui_Button shop_equip_buttons[EQUIP_SHOP_MAX];
extern Gui_Button shop_consumable_buttons[CONSUMABLE_SHOP_MAX];
extern Gui_Button shop_other_buttons[GUI_SHOP_OTHERS_GROUP_BUTTONS_MAX+1];
extern Gui_Button_Group shop_equip_buttons_group;
extern Gui_Button_Group shop_consumable_buttons_group;
extern Gui_Button_Group shop_other_buttons_group;
extern Gui_Button_Group* shop_buttons_groups[GUI_SHOP_LAYOUT_GROUPS_MAX+1];
extern Gui_Button_Layout shop_buttons_layout;

extern Gui_Button treasure_buttons[GUI_TREASURE_GROUP_BUTTONS_MAX+1];
extern Gui_Button_Group treasure_buttons_group;

#ifdef HELAPORDO_DEBUG_ACCESS
extern Gui_Button debug_buttons[GUI_DEBUG_GROUP_BUTTONS_MAX+1];
extern Gui_Button_Group debug_buttons_group;

extern Gui_Button debug_fighter_specialslots_buttons[SPECIALSMAX+1];
extern Gui_Button debug_fighter_skillslots_buttons[FIGHTER_SKILL_SLOTS+1];
extern Gui_Button debug_fighter_counters_buttons[COUNTERSMAX+1];
extern Gui_Button debug_fighter_perks_buttons[PERKSMAX+1];
extern Gui_Button debug_fighter_equipslots_buttons[EQUIPZONES+1];
extern Gui_Button debug_fighter_equipsbag_buttons[EQUIPSBAGSIZE+1];
extern Gui_Button debug_fighter_consumablesbag_buttons[CONSUMABLESMAX+1];
extern Gui_Button debug_fighter_artifactsbag_buttons[ARTIFACTSMAX+1];
extern Gui_Button_Group debug_fighter_specialslots_group;
extern Gui_Button_Group debug_fighter_skillslots_group;
extern Gui_Button_Group debug_fighter_counters_group;
extern Gui_Button_Group debug_fighter_perks_group;
extern Gui_Button_Group debug_fighter_equipslots_group;
extern Gui_Button_Group debug_fighter_equipsbag_group;
extern Gui_Button_Group debug_fighter_consumablesbag_group;
extern Gui_Button_Group debug_fighter_artifactsbag_group;
extern Gui_Button_Group* debug_fighter_groups[GUI_DEBUG_FIGHTER_LAYOUT_GROUPS_MAX+1];
extern Gui_Button_Layout debug_fighter_buttons_layout;
#endif // HELAPORDO_DEBUG_ACCESS

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
