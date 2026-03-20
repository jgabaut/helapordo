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
#include "game_rl.h"

callback_void_t callback_func_ptrs[SPECIALSMAX];
callback_void_t callback_artifact_ptrs[ARTIFACTSMAX];
callback_void_t callback_counter_ptrs[COUNTERSMAX];

Gui_Button txtfield_buttons[GUI_TXTFIELD_GROUP_BUTTONS_MAX+1] = {
    [BUTTON_NAME_TXTFIELD] = {
        .r = {.x = 100, .y = 100, .width = 200, .height = 100},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "",
        .label_len = 0,
        .box_color = GUI_TXTFIELD_GROUP_BOX_COLOR,
        .text_color = GUI_TXTFIELD_GROUP_TEXT_COLOR,
    },
    [BUTTON_CLASS_TXTFIELD] = {
        .r = {.x = 100, .y = 100, .width = 200, .height = 100},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "",
        .label_len = 0,
        .box_color = GUI_TXTFIELD_GROUP_BOX_COLOR,
        .text_color = GUI_TXTFIELD_GROUP_TEXT_COLOR,
    }
};
Gui_Button_Group txtfield_buttons_group = {
    .buttons = &(txtfield_buttons[0]),
    .len = ARRAY_SIZE(txtfield_buttons),
};

Gui_Button gamepick_buttons[GUI_GAMEPICK_GROUP_BUTTONS_MAX+1] = {
    [BUTTON_NEW_GAME] = {
        .r = {.x = 100, .y = 100, .width = 150, .height = 80},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "New Game",
        .label_len = ARRAY_SIZE("New Game")-1,
        .box_color = GUI_GAMEPICK_GROUP_BOX_COLOR,
        .text_color = GUI_GAMEPICK_GROUP_TEXT_COLOR,
    },
    [BUTTON_LOAD_GAME] = {
        .r = {.x = 300, .y = 100, .width = 150, .height = 80},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Load Game",
        .label_len = ARRAY_SIZE("Load Game")-1,
        .box_color = GUI_GAMEPICK_GROUP_BOX_COLOR,
        .text_color = GUI_GAMEPICK_GROUP_TEXT_COLOR,
    }
};
Gui_Button_Group gamepick_buttons_group = {
    .buttons = &(gamepick_buttons[0]),
    .len = ARRAY_SIZE(gamepick_buttons),
};

Gui_Button classpick_buttons[GUI_CLASSPICK_GROUP_BUTTONS_MAX+1] = {
    [BUTTON_CLASS_KNIGHT] = {
        .r = {.x = 50, .y = 100, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Knight",
        .label_len = ARRAY_SIZE("Knight")-1,
        .box_color = GUI_CLASSPICK_GROUP_BOX_COLOR,
        .text_color = GUI_CLASSPICK_GROUP_TEXT_COLOR,
    },
    [BUTTON_CLASS_MAGE] = {
        .r = {.x = 160, .y = 100, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Mage",
        .label_len = ARRAY_SIZE("Mage")-1,
        .box_color = GUI_CLASSPICK_GROUP_BOX_COLOR,
        .text_color = GUI_CLASSPICK_GROUP_TEXT_COLOR,
    },
    [BUTTON_CLASS_ARCHER] = {
        .r = {.x = 270, .y = 100, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Archer",
        .label_len = ARRAY_SIZE("Archer")-1,
        .box_color = GUI_CLASSPICK_GROUP_BOX_COLOR,
        .text_color = GUI_CLASSPICK_GROUP_TEXT_COLOR,
    },
    [BUTTON_CLASS_ASSASSIN] = {
        .r = {.x = 380, .y = 100, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Assassin",
        .label_len = ARRAY_SIZE("Assassin")-1,
        .box_color = GUI_CLASSPICK_GROUP_BOX_COLOR,
        .text_color = GUI_CLASSPICK_GROUP_TEXT_COLOR,
    }
};
Gui_Button_Group classpick_buttons_group = {
    .buttons = &(classpick_buttons[0]),
    .len = ARRAY_SIZE(classpick_buttons),
};

Gui_Button saveslotpick_buttons[GUI_SAVESLOTPICK_GROUP_BUTTONS_MAX+1] = {
    [BUTTON_SAVESLOT_1] = {
        .r = {.x = 100, .y = 100, .width = 100, .height = 80},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Slot 1",
        .label_len = ARRAY_SIZE("Slot 1")-1,
        .box_color = GUI_SAVESLOTPICK_GROUP_BOX_COLOR,
        .text_color = GUI_SAVESLOTPICK_GROUP_TEXT_COLOR,
    },
    [BUTTON_SAVESLOT_2] = {
        .r = {.x = 210, .y = 100, .width = 100, .height = 80},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Slot 2",
        .label_len = ARRAY_SIZE("Slot 2")-1,
        .box_color = GUI_SAVESLOTPICK_GROUP_BOX_COLOR,
        .text_color = GUI_SAVESLOTPICK_GROUP_TEXT_COLOR,
    },
    [BUTTON_SAVESLOT_3] = {
        .r = {.x = 320, .y = 100, .width = 100, .height = 80},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Slot 3",
        .label_len = ARRAY_SIZE("Slot 3")-1,
        .box_color = GUI_SAVESLOTPICK_GROUP_BOX_COLOR,
        .text_color = GUI_SAVESLOTPICK_GROUP_TEXT_COLOR,
    }
};
Gui_Button_Group saveslotpick_buttons_group = {
    .buttons = &(saveslotpick_buttons[0]),
    .len = ARRAY_SIZE(saveslotpick_buttons),
};

Gui_Button fight_buttons[GUI_FIGHT_GROUP_BUTTONS_MAX+1] = {
    [BUTTON_FIGHT] = {
        .r = {.x = 64, .y = 250, .width = 80, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Fight",
        .label_len = ARRAY_SIZE("Fight")-1,
        .box_color = GUI_FIGHT_GROUP_BOX_COLOR,
        .text_color = GUI_FIGHT_GROUP_TEXT_COLOR,
    },
    [BUTTON_SPECIAL] = {
        .r = {.x = 159, .y = 250, .width = 80, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Special",
        .label_len = ARRAY_SIZE("Special")-1,
        .box_color = GUI_FIGHT_GROUP_BOX_COLOR,
        .text_color = GUI_FIGHT_GROUP_TEXT_COLOR,
    },
    [BUTTON_EQUIPS] = {
        .r = {.x = 254, .y = 250, .width = 80, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Equips",
        .label_len = ARRAY_SIZE("Equips")-1,
        .box_color = GUI_FIGHT_GROUP_BOX_COLOR,
        .text_color = GUI_FIGHT_GROUP_TEXT_COLOR,
    },
    [BUTTON_CONSUMABLES] = {
        .r = {.x = 349, .y = 250, .width = 130, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Consumables",
        .label_len = ARRAY_SIZE("Consumables")-1,
        .box_color = GUI_FIGHT_GROUP_BOX_COLOR,
        .text_color = GUI_FIGHT_GROUP_TEXT_COLOR,
    },
    [BUTTON_STATS] = {
        .r = {.x = 494, .y = 250, .width = 80, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Stats",
        .label_len = ARRAY_SIZE("Stats")-1,
        .box_color = GUI_FIGHT_GROUP_BOX_COLOR,
        .text_color = GUI_FIGHT_GROUP_TEXT_COLOR,
    }
};
Gui_Button_Group fight_buttons_group = {
    .buttons = &(fight_buttons[0]),
    .len = GUI_FIGHT_GROUP_BUTTONS_MAX+1,
};

Gui_Button special_buttons[GUI_SPECIAL_GROUP_BUTTONS_MAX+1] = {
    [BUTTON_SPECIAL_1] = {
        .r = {.x = 60, .y = 250, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "1",
        .label_len = ARRAY_SIZE("1")-1,
        .box_color = GUI_SPECIAL_GROUP_BOX_COLOR,
        .text_color = GUI_SPECIAL_GROUP_TEXT_COLOR,
    },
    [BUTTON_SPECIAL_2] = {
        .r = {.x = 170, .y = 250, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "2",
        .label_len = ARRAY_SIZE("2")-1,
        .box_color = GUI_SPECIAL_GROUP_BOX_COLOR,
        .text_color = GUI_SPECIAL_GROUP_TEXT_COLOR,
    },
    [BUTTON_SPECIAL_3] = {
        .r = {.x = 280, .y = 250, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "3",
        .label_len = ARRAY_SIZE("3")-1,
        .box_color = GUI_SPECIAL_GROUP_BOX_COLOR,
        .text_color = GUI_SPECIAL_GROUP_TEXT_COLOR,
    },
    [BUTTON_SPECIAL_4] = {
        .r = {.x = 390, .y = 250, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "4",
        .label_len = ARRAY_SIZE("4")-1,
        .box_color = GUI_SPECIAL_GROUP_BOX_COLOR,
        .text_color = GUI_SPECIAL_GROUP_TEXT_COLOR,
    }
};
Gui_Button_Group special_buttons_group = {
    .buttons = &(special_buttons[0]),
    .len = ARRAY_SIZE(special_buttons),
};
Gui_Button equips_buttons[GUI_EQUIPS_GROUP_BUTTONS_MAX+1] = {
    [BUTTON_OPEN_BAG] = {
        .r = {.x = 60, .y = 125, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Open Bag",
        .label_len = ARRAY_SIZE("Open Bag")-1,
        .box_color = GUI_EQUIPS_GROUP_BOX_COLOR,
        .text_color = GUI_EQUIPS_GROUP_TEXT_COLOR,
    },
    [BUTTON_CHECK_LOADOUT] = {
        .r = {.x = 60, .y = 195, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Loadout",
        .label_len = ARRAY_SIZE("Loadout")-1,
        .box_color = GUI_EQUIPS_GROUP_BOX_COLOR,
        .text_color = GUI_EQUIPS_GROUP_TEXT_COLOR,
    },
};
Gui_Button_Group equips_buttons_group = {
    .buttons = &(equips_buttons[0]),
    .len = ARRAY_SIZE(equips_buttons),
};

Gui_Button shop_equip_buttons[EQUIP_SHOP_MAX] = {0};
Gui_Button shop_consumable_buttons[CONSUMABLE_SHOP_MAX] = {0};
Gui_Button shop_other_buttons[GUI_SHOP_OTHERS_GROUP_BUTTONS_MAX+1] = {
    [BUTTON_LEAVE_SHOP] = {
        .label = "Leave",
        .label_len = ARRAY_SIZE("Leave")-1,
        .box_color = GUI_SHOP_OTHERS_GROUP_BOX_COLOR,
        .text_color = GUI_SHOP_OTHERS_GROUP_TEXT_COLOR,
    },
    [BUTTON_SELL_ALL] = {
        .label = "Sell all",
        .label_len = ARRAY_SIZE("Sell all")-1,
        .box_color = GUI_SHOP_OTHERS_GROUP_BOX_COLOR,
        .text_color = GUI_SHOP_OTHERS_GROUP_TEXT_COLOR,
    }
};
Gui_Button_Group shop_equip_buttons_group = {
    .buttons = &(shop_equip_buttons[0]),
    .len = EQUIP_SHOP_MAX
};
Gui_Button_Group shop_consumable_buttons_group = {
    .buttons = &(shop_consumable_buttons[0]),
    .len = CONSUMABLE_SHOP_MAX
};
Gui_Button_Group shop_other_buttons_group = {
    .buttons = &(shop_other_buttons[0]),
    .len = 2,
};
Gui_Button_Group* shop_buttons_groups[GUI_SHOP_LAYOUT_GROUPS_MAX+1] = {
    &shop_equip_buttons_group,
    &shop_consumable_buttons_group,
    &shop_other_buttons_group
};
Gui_Button_Layout shop_buttons_layout = {
    .groups = shop_buttons_groups,
    .len = GUI_SHOP_LAYOUT_GROUPS_MAX+1
};

Gui_Button treasure_buttons[GUI_TREASURE_GROUP_BUTTONS_MAX+1] = {
    [BUTTON_TAKE_TREASURE] = {
        .r = {.x = 60, .y = 250, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Take",
        .label_len = ARRAY_SIZE("Take")-1,
        .box_color = GUI_TREASURE_GROUP_BOX_COLOR,
        .text_color = GUI_TREASURE_GROUP_TEXT_COLOR,
    },
    [BUTTON_LEAVE_TREASURE] = {
        .r = {.x = 170, .y = 250, .width = 100, .height = 50},
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Leave",
        .label_len = ARRAY_SIZE("Leave")-1,
        .box_color = GUI_TREASURE_GROUP_BOX_COLOR,
        .text_color = GUI_TREASURE_GROUP_TEXT_COLOR,
    }
};
Gui_Button_Group treasure_buttons_group = {
    .buttons = &(treasure_buttons[0]),
    .len = ARRAY_SIZE(treasure_buttons),
};

Gui_Button floor_buttons[GUI_FLOOR_GROUP_BUTTONS_MAX+1] = {
    [BUTTON_ARTIFACTS] = {
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Artifacts",
        .label_len = ARRAY_SIZE("Artifacts")-1,
        .box_color = GUI_FLOOR_GROUP_BOX_COLOR,
        .text_color = GUI_FLOOR_GROUP_TEXT_COLOR,
    }
};
Gui_Button_Group floor_buttons_group = {
    .buttons = &(floor_buttons[0]),
    .len = ARRAY_SIZE(floor_buttons),
};

#ifdef HELAPORDO_DEBUG_ACCESS
Gui_Button debug_buttons[GUI_DEBUG_GROUP_BUTTONS_MAX+1] = {
    [BUTTON_DEBUG] = {
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Debug",
        .label_len = ARRAY_SIZE("Debug")-1,
        .box_color = GUI_DEBUG_GROUP_BOX_COLOR,
        .text_color = GUI_DEBUG_GROUP_TEXT_COLOR,
    },
    [BUTTON_CYCLE_DEBUG_LAYOUT] = {
        .on = false,
        .state = BUTTON_NORMAL,
        .label = "Cycle",
        .label_len = ARRAY_SIZE("Cycle")-1,
        .box_color = GUI_DEBUG_GROUP_BOX_COLOR,
        .text_color = GUI_DEBUG_GROUP_TEXT_COLOR,
    },
};
Gui_Button_Group debug_buttons_group = {
    .buttons = &(debug_buttons[0]),
    .len = ARRAY_SIZE(debug_buttons),
};

Gui_Button debug_fighter_specialslots_buttons[SPECIALSMAX+1] = {0};
Gui_Button debug_fighter_skillslots_buttons[FIGHTER_SKILL_SLOTS+1] = {0};
Gui_Button debug_fighter_counters_buttons[COUNTERSMAX+1] = {0};
Gui_Button debug_fighter_perks_buttons[PERKSMAX+1] = {0};
Gui_Button debug_fighter_equipslots_buttons[EQUIPZONES+1] = {0};
Gui_Button debug_fighter_equipsbag_buttons[EQUIPSBAGSIZE+1] = {0};
Gui_Button debug_fighter_consumablesbag_buttons[CONSUMABLESMAX+1] = {0};
Gui_Button debug_fighter_artifactsbag_buttons[ARTIFACTSMAX+1] = {0};
Gui_Button_Group debug_fighter_specialslots_group = {
    .buttons = &(debug_fighter_specialslots_buttons[0]),
    .len = ARRAY_SIZE(debug_fighter_specialslots_buttons),
};
Gui_Button_Group debug_fighter_skillslots_group = {
    .buttons = &(debug_fighter_skillslots_buttons[0]),
    .len = ARRAY_SIZE(debug_fighter_skillslots_buttons),
};
Gui_Button_Group debug_fighter_counters_group = {
    .buttons = &(debug_fighter_counters_buttons[0]),
    .len = ARRAY_SIZE(debug_fighter_counters_buttons),
};
Gui_Button_Group debug_fighter_perks_group = {
    .buttons = &(debug_fighter_perks_buttons[0]),
    .len = ARRAY_SIZE(debug_fighter_perks_buttons),
};
Gui_Button_Group debug_fighter_equipslots_group = {
    .buttons = &(debug_fighter_equipslots_buttons[0]),
    .len = ARRAY_SIZE(debug_fighter_equipslots_buttons),
};
Gui_Button_Group debug_fighter_equipsbag_group = {
    .buttons = &(debug_fighter_equipsbag_buttons[0]),
    .len = ARRAY_SIZE(debug_fighter_equipsbag_buttons),
};
Gui_Button_Group debug_fighter_consumablesbag_group = {
    .buttons = &(debug_fighter_consumablesbag_buttons[0]),
    .len = ARRAY_SIZE(debug_fighter_consumablesbag_buttons),
};
Gui_Button_Group debug_fighter_artifactsbag_group = {
    .buttons = &(debug_fighter_artifactsbag_buttons[0]),
    .len = ARRAY_SIZE(debug_fighter_artifactsbag_buttons),
};
Gui_Button_Group* debug_fighter_groups[GUI_DEBUG_FIGHTER_LAYOUT_GROUPS_MAX+1] = {
    &debug_fighter_specialslots_group,
    &debug_fighter_skillslots_group,
    &debug_fighter_counters_group,
    &debug_fighter_perks_group,
    &debug_fighter_equipslots_group,
    &debug_fighter_equipsbag_group,
    &debug_fighter_consumablesbag_group,
    &debug_fighter_artifactsbag_group
};
Gui_Button_Layout debug_fighter_buttons_layout = {
    .groups = &(debug_fighter_groups[0]),
    .len = ARRAY_SIZE(debug_fighter_groups),
};
#endif // HELAPORDO_DEBUG_ACCESS

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
    DrawText("You can change floors by killing a Boss.", 14, 12*fontSize, fontSize, tut_color);
    DrawText("When in floor map, you can open the menu with the \"m\" key.", 14, 15*fontSize, fontSize, tut_color);
    DrawText("By default, the game autosaves when you get to an Home room.", 14, 18*fontSize, fontSize, tut_color);

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

/**
 * Takes a RingaBuf to take the Notification from, and a Rectangle to draw them to.
 * @param rb_notifications The RingaBuf holding the Notification
 * @param notifications_rect The Rectangle used to draw the notifications
 * @see Notification
 * @see enqueue_notification()
 */
void hlpd_draw_notifications(RingaBuf* rb_notifications, Rectangle notifications_rect)
{
    Notification* newest_notif = NULL;
    //Notification* oldest_notif = NULL;

    if (!(rb_isfull(*rb_notifications))) {
        if (rb_get_head(*rb_notifications) != 0) {
            //oldest_notif = (Notification*) &(rb_notifications->data[0]);
            int32_t head = rb_get_head(*rb_notifications);
            //size_t newest_idx = ((head/sizeof(Notification)) -1);
            bool getelem_success = true;
            //newest_notif = (Notification*) rb_getelem_by_offset(*rb_notifications, head - sizeof(Notification));
            //newest_notif = (Notification*) rb_getelem_by_index(*rb_notifications, newest_idx, &getelem_success);
            newest_notif = (Notification*) rb_getelem_newest(*rb_notifications, &getelem_success);
            (void) newest_notif;
            assert(getelem_success);
            //if (!newest_notif->displayed) {
            //wclear(notifications_win);
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Checking up from 0 to head: { %" PRIu32 " }", __func__, head);
            for (int i = 0; i < (head / sizeof(Notification)); i++) {
                //Notification* read_notif = (Notification*) rb_getelem_by_offset(*rb_notifications, i * sizeof(Notification));
                Notification* read_notif = (Notification*) rb_getelem_by_index(*rb_notifications, i, &getelem_success);
                assert(getelem_success);
                log_tag("debug_log.txt", "[DEBUG]", "%s():    0->H [%i] Displaying notification {%s} Color: [%" PRId8 "]", __func__, i, read_notif->buf, read_notif->color);
                Color notif_color = ColorFromS4CPalette(palette, read_notif->color);
                int txt_height = notifications_rect.height * 0.125f;
                DrawText(TextFormat(" %s", read_notif->buf), notifications_rect.x, notifications_rect.y + ((i+1)* txt_height), txt_height, notif_color);
                read_notif->displayed = true;
            }
            //}
        } else {
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Notification ring is empty.", __func__);
        }
    } else {
        int32_t head = rb_get_head(*rb_notifications);
        size_t capacity = rb_get_capacity(*rb_notifications);
        //size_t newest_idx = ( head == 0 ? (NOTIFICATIONS_RINGBUFFER_SIZE-1) : ((head/sizeof(Notification)) -1));
        bool getelem_success = true;
        //size_t newest_offset = (head == 0 ? ((NOTIFICATIONS_RINGBUFFER_SIZE-1)* sizeof(Notification)) : (head - sizeof(Notification)));
        //newest_notif = (Notification*) rb_getelem_by_offset(*rb_notifications, newest_offset);
        //newest_notif = (Notification*) rb_getelem_by_index(*rb_notifications, newest_idx, &getelem_success);
        newest_notif = (Notification*) rb_getelem_newest(*rb_notifications, &getelem_success);
        (void) newest_notif;
        assert(getelem_success);
        //oldest_notif = (Notification*) &(rb_notifications->data[(rb_notifications->head)]);
        int current_idx = 0;
        //if (!newest_notif->displayed) {
        //wclear(notifications_win);
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Checking up from head+1 { %" PRIu32 " } to size { %" PRIu32 " }, then from 0 to head.", __func__, (head / sizeof(Notification)) +1, capacity / sizeof(Notification));
        for (size_t i = (head / sizeof(Notification)) +1; i < (capacity / sizeof(Notification)); i++) {
            //Notification* read_notif = (Notification*) rb_getelem_by_offset(*rb_notifications, i * sizeof(Notification));
            Notification* read_notif = (Notification*) rb_getelem_by_index(*rb_notifications, i, &getelem_success);
            assert(getelem_success);
#ifndef _WIN32
            log_tag("debug_log.txt", "[DEBUG]", "%s():    H+1->S [%li] Displaying notification {%s} Color: [%" PRId8 "]", __func__, i, read_notif->buf, read_notif->color);
#else
            log_tag("debug_log.txt", "[DEBUG]", "%s():    H+1->S [%lli] Displaying notification {%s} Color: [%" PRId8 "]", __func__, i, read_notif->buf, read_notif->color);
#endif
            Color notif_color = ColorFromS4CPalette(palette, read_notif->color);
            int txt_height = notifications_rect.height * 0.125f;
            DrawText(TextFormat(" %s", read_notif->buf), notifications_rect.x, notifications_rect.y + ((current_idx+1)* txt_height), txt_height, notif_color);
            read_notif->displayed = true;
            current_idx++;
        }
        for (size_t i = 0; i < (head / sizeof(Notification)); i++) {
            //Notification* read_notif = (Notification*) rb_getelem_by_offset(*rb_notifications, i * sizeof(Notification));
            Notification* read_notif = (Notification*) rb_getelem_by_index(*rb_notifications, i, &getelem_success);
            assert(getelem_success);
#ifndef _WIN32
            log_tag("debug_log.txt", "[DEBUG]", "%s():    0->H [%li] Displaying notification {%s} Color: [%" PRId8 "]", __func__, i, read_notif->buf, read_notif->color);
#else
            log_tag("debug_log.txt", "[DEBUG]", "%s():    0->H [%lli] Displaying notification {%s} Color: [%" PRId8 "]", __func__, i, read_notif->buf, read_notif->color);
#endif
            Color notif_color = ColorFromS4CPalette(palette, read_notif->color);
            int txt_height = notifications_rect.height * 0.125f;
            DrawText(TextFormat(" %s", read_notif->buf), notifications_rect.x, notifications_rect.y + ((current_idx+1)* txt_height), txt_height, notif_color);
            read_notif->displayed = true;
            current_idx++;
        }
        //}
    }
}

// void update_GameScreen(float* scale, float gameScreenWidth, float gameScreenHeight, GameScreen* currentScreen, int* framesCounter, Floor** current_floor, int* current_x, int* current_y, int logo_sleep, bool* pause_animation, Koliseo_Temp** floor_kls, KLS_Conf temporary_kls_conf, int* current_anim_frame, Vector2* mouse, Vector2* virtualMouse, loadInfo* load_info, int* saveslot_index, char current_save_path[1000])
void update_GameScreen(Gui_State* gui_state, Floor** current_floor, Path** game_path, Fighter** player, Room** current_room, Gamestate** gamestate, RingaBuf* rb_notifications, int* current_x, int* current_y, int logo_sleep, bool* pause_animation, Koliseo_Temp** floor_kls, KLS_Conf temporary_kls_conf, int* current_anim_frame, loadInfo* load_info, int* saveslot_index, char current_save_path[1500], char seed[PATH_SEED_BUFSIZE+1], bool is_seeded, int* roomsDone, int* enemyTotal)
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
            Gui_Button_Group group = gui_state->gamepick_buttons;
            for (Gui_GamePick_Group_Button_Index i = 0; i < group.len; i++) {
                group.buttons[i].on = false;
            }
            sprintf(current_save_path, "%s", ""); // Clear current save path
            for (Gui_GamePick_Group_Button_Index i = 0; i < group.len; i++) {

                if (CheckCollisionPointRec(gui_state->virtualMouse, group.buttons[i].r)) {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        group.buttons[i].state = BUTTON_PRESSED;
                    } else {
                        group.buttons[i].state = BUTTON_HOVER;
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                        group.buttons[i].on = true;
                    }
                } else {
                    group.buttons[i].state = BUTTON_NORMAL;
                }
                if (group.buttons[i].on) {
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
                Gui_Button_Group group = gui_state->saveslotpick_buttons;
                for (Gui_SaveSlotPick_Group_Button_Index i = 0; i < group.len; i++) {
                    group.buttons[i].on = false;
                }
                sprintf(current_save_path, "%s", ""); // Clear current save path
                for (Gui_SaveSlotPick_Group_Button_Index i = 0; i < group.len; i++) {

                    if (CheckCollisionPointRec(gui_state->virtualMouse, group.buttons[i].r)) {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                            group.buttons[i].state = BUTTON_PRESSED;
                        } else {
                            group.buttons[i].state = BUTTON_HOVER;
                        }
                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                            group.buttons[i].on = true;
                        }
                    } else {
                        group.buttons[i].state = BUTTON_NORMAL;
                    }
                    if (group.buttons[i].on) {
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
                    *game_path = randomise_path(seed, default_kls, current_save_path);
                    (*game_path)->current_saveslot->index = *saveslot_index;
                    (*game_path)->length = MAX_ROGUE_FLOORS;
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
                        (Fighter *) KLS_PUSH_TYPED(default_kls, Fighter, HR_Fighter,
                                                   "Fighter", "Loady Fighter");

                    strncpy((*player)->name, "Test", strlen("Test")+1);
                    (*player)->class = Knight;
                    //getParams(argc, argv, player, path, optTot, default_kls);
                    //TODO: ensure class and name are taken before this update
                    initPlayerStats(*player, *game_path, default_kls);
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

                    *current_floor = KLS_PUSH_TYPED(default_kls, Floor, HR_Floor, "Floor", "Loady Floor");
                    (*gamestate)->current_floor = *current_floor;

                    *current_room = KLS_PUSH_TYPED(default_kls, Room, HR_Room, "Room", "Loady Room");
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
                    update_Equipslots(*player);

                    *current_x = (*player)->floor_x;
                    *current_y = (*player)->floor_y;

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
                        } else {
                            load_info->done_loading = 1;
                            log_tag("debug_log.txt", "[WARN-TURNOP]",
                                    "%s():    current room was not ENEMIES. Nullying current room.", __func__);
                            *current_room = NULL;
                            (*gamestate)->current_room = *current_room;
                            load_info->done_loading = 1;
                        }
                    } else {
                        log_tag("debug_log.txt", "[WARN-TURNOP]",
                                "%s():    gamestate->room was NULL. Not setting load_info's room info.", __func__);
                        load_info->done_loading = 1;
                    }
                }
                gui_state->currentScreen = FLOOR_VIEW;
            }
        }
        break;
        case 1: {
            // New game, Press 1-3 to change to set the index, and then change to FLOOR_VIEW screen
            if (gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].label_len == 0) {
                gui_state->currentScreen = NAMEPICK_VIEW;
            } else {
                if (gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD].label_len == 0) {
                    gui_state->currentScreen = CLASSPICK_VIEW;
                } else {
                    // Reference: https://www.raylib.com/examples/textures/loader.html?name=textures_sprite_button
                    if (*saveslot_index == -1) {  // Pick saveslot
                        Gui_Button_Group group = gui_state->saveslotpick_buttons;
                        for (Gui_SaveSlotPick_Group_Button_Index i = 0; i < group.len; i++) {
                            group.buttons[i].on = false;
                        }
                        sprintf(current_save_path, "%s", ""); // Clear current save path
                        for (Gui_SaveSlotPick_Group_Button_Index i = 0; i < group.len; i++) {

                            if (CheckCollisionPointRec(gui_state->virtualMouse, group.buttons[i].r)) {
                                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                                    group.buttons[i].state = BUTTON_PRESSED;
                                } else {
                                    group.buttons[i].state = BUTTON_HOVER;
                                }
                                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                                    group.buttons[i].on = true;
                                }
                            } else {
                                group.buttons[i].state = BUTTON_NORMAL;
                            }
                            if (group.buttons[i].on) {
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
                            *game_path = randomise_path(seed, default_kls, current_save_path);
                            (*game_path)->current_saveslot->index = *saveslot_index;
                            (*game_path)->length = MAX_ROGUE_FLOORS;
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
                                (Fighter *) KLS_PUSH_TYPED(default_kls, Fighter, HR_Fighter,
                                                           "Fighter", "Fighter");

                            Gui_Button namefield = gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD];
                            if (namefield.label_len > 0) {
                                log_tag("debug_log.txt", "DEBUG", "%s():    Using {%s} for player name", __func__, namefield.label);
                                fprintf(stderr, "[DEBUG] [%s()]    Using {%s} for player name\n", __func__, namefield.label);
                                strncpy((*player)->name, namefield.label, namefield.label_len+1);
                            } else {
                                assert(false);
                                strncpy((*player)->name, "Test", strlen("Test")+1);
                            }
                            Gui_Button classfield = gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD];
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
                            initPlayerStats(*player, *game_path, default_kls);
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
        if (CheckCollisionPointRec(gui_state->virtualMouse, gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].r)) {
            gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].on = true;
        } else {
            gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].on = false;
        }
        if (gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].on) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].label_len < 20)) {
                    size_t l = gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].label_len;
                    gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].label[l] = (char) key;
                    gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].label[l+1] = '\0';

                    gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].label_len += 1;

                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].label_len -= 1;
                if (gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].label_len < 0) {
                    gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].label_len = 0;
                }
                size_t l = gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].label_len;
                gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].label[l] = '\0';

            }

            if (IsKeyPressed(KEY_ENTER)) {
                if (gui_state->txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].label_len > 0) {
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
        Gui_Button_Group group = gui_state->classpick_buttons;
        for (Gui_ClassPick_Group_Button_Index i = 0; i < group.len; i++) {
            group.buttons[i].on = false;
        }
        for (Gui_ClassPick_Group_Button_Index i = 0; i < group.len; i++) {

            if (CheckCollisionPointRec(gui_state->virtualMouse, group.buttons[i].r)) {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    group.buttons[i].state = BUTTON_PRESSED;
                } else {
                    group.buttons[i].state = BUTTON_HOVER;
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    group.buttons[i].on = true;
                }
            } else {
                group.buttons[i].state = BUTTON_NORMAL;
            }
            if (group.buttons[i].on) {
                fprintf(stderr, "%s():    [EFFECT]\n", __func__);
                if (i == BUTTON_CLASS_KNIGHT) {
                    memcpy(gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD].label, "Knight", strlen("Knight")+1);
                    gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD].label_len = strlen(gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD].label);
                    gui_state->currentScreen = SAVES_VIEW;
                } else if ( i == BUTTON_CLASS_ARCHER) {
                    memcpy(gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD].label, "Archer", strlen("Archer")+1);
                    gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD].label_len = strlen(gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD].label);
                    gui_state->currentScreen = SAVES_VIEW;
                } else if ( i == BUTTON_CLASS_MAGE) {
                    memcpy(gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD].label, "Mage", strlen("Mage")+1);
                    gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD].label_len = strlen(gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD].label);
                    gui_state->currentScreen = SAVES_VIEW;
                } else if ( i == BUTTON_CLASS_ASSASSIN) {
                    memcpy(gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD].label, "Assassin", strlen("Assassin")+1);
                    gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD].label_len = strlen(gui_state->txtfield_buttons.buttons[BUTTON_CLASS_TXTFIELD].label);
                    gui_state->currentScreen = SAVES_VIEW;
                }
            }
        }
        gui_state->framesCounter += 1;
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
            // Set player floor x/y coords
            (*player)->floor_x = *current_x;
            (*player)->floor_y = *current_y;

            // Autosave
            {
                Enemy *dummy_enemy = NULL;
                Boss *dummy_boss = NULL;
                FILE *dummy_savefile = NULL;
                Rectangle rect = {0};
                RingaBuf *dummy_rb = NULL;
                foeTurnOption_OP dummy_foe_op = FOE_OP_INVALID;
                skillType dummy_skill_pick = -1;
                //Declare turnOP_args
                turnOP_args *args =
                    init_turnOP_args(*gamestate, *player, *game_path, *current_room, load_info, dummy_enemy,
                                     dummy_boss, dummy_savefile, &rect, *floor_kls,
                                     dummy_foe_op, dummy_skill_pick, dummy_rb);

                log_tag("debug_log.txt", "[DEBUG]", "%s():    Skipping preparing autosave file path", __func__);
                turnOP(turnOP_from_turnOption(SAVE), args, default_kls, *floor_kls);
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Done autosave.", __func__);
                log_tag("debug_log.txt", "[DEBUG]", "%s():    G_RNG_ADVANCEMENTS == {%" PRId64 "}", __func__, G_RNG_ADVANCEMENTS);
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Seed: {%s}", __func__, (*game_path)->seed);
            }
        } // End if *current_floor is NULL

        gui_state->framesCounter += 1;    // Count frames

#ifdef HELAPORDO_DEBUG_ACCESS
        if (G_DEBUG_ON == 1) {
            gui_state->debug_buttons.buttons[BUTTON_DEBUG].on = false;

            int debug_button_w = gui_state->gameScreenWidth*0.2f;
            int debug_button_h = gui_state->gameScreenHeight*0.1f;
            int debug_button_x = gui_state->gameScreenWidth - debug_button_w;
            int debug_button_y = (gui_state->gameScreenHeight - debug_button_h)/2;
            Rectangle debug_button_r = {
                .x = debug_button_x,
                .y = debug_button_y,
                .width = debug_button_w,
                .height = debug_button_h,
            };
            Gui_Button* button = &(gui_state->debug_buttons.buttons[BUTTON_DEBUG]);
            button->r = debug_button_r;
            if (CheckCollisionPointRec(gui_state->virtualMouse, button->r)) {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    button->state = BUTTON_PRESSED;
                } else {
                    button->state = BUTTON_HOVER;
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    button->on = true;
                }
            } else {
                button->state = BUTTON_NORMAL;
            }
            if (button->on) {
                fprintf(stderr, "%s():    [EFFECT]\n", __func__);
                gui_state->currentScreen = DEBUG_VIEW;
                break;
            }
        }

        if (IsKeyPressed(KEY_R)) {
            fprintf(stderr,"%s\n", "Regenerating current floor");
            Wincon* w = (*game_path)->win_condition;
            int path_length = (*game_path)->length;
            const char* player_name = (*player)->name;
            const int player_name_len = strlen(player_name);
            fighterClass player_class = (*player)->class;
            kls_temp_end(*floor_kls);
            kls_free(temporary_kls);
            temporary_kls = kls_new_conf(KLS_DEFAULT_SIZE * 32, temporary_kls_conf);

            srand(time(NULL));
            G_RNG_ADVANCEMENTS = 0;
            if (!is_seeded) gen_random_seed(seed);

            *game_path = randomise_path(seed, temporary_kls, current_save_path);

            (*game_path)->current_saveslot->index = *saveslot_index;
            (*game_path)->win_condition = w;
            (*game_path)->length = path_length;

            *player =
                (Fighter *) KLS_PUSH_TYPED(temporary_kls, Fighter, HR_Fighter,
                                           "Fighter", "Fighter");

            strncpy((*player)->name, player_name, player_name_len);
            (*player)->name[player_name_len] = '\0';
            (*player)->class = player_class;
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
            // Set player floor x/y coords
            (*player)->floor_x = *current_x;
            (*player)->floor_y = *current_y;

            // Autosave
            {
                Enemy *dummy_enemy = NULL;
                Boss *dummy_boss = NULL;
                FILE *dummy_savefile = NULL;
                Rectangle rect = {0};
                RingaBuf *dummy_rb = NULL;
                foeTurnOption_OP dummy_foe_op = FOE_OP_INVALID;
                skillType dummy_skill_pick = -1;
                //Declare turnOP_args
                turnOP_args *args =
                    init_turnOP_args(*gamestate, *player, *game_path, *current_room, load_info, dummy_enemy,
                                     dummy_boss, dummy_savefile, &rect, *floor_kls,
                                     dummy_foe_op, dummy_skill_pick, dummy_rb);

                log_tag("debug_log.txt", "[DEBUG]", "%s():    Skipping preparing autosave file path", __func__);
                turnOP(turnOP_from_turnOption(SAVE), args, default_kls, *floor_kls);
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Done autosave.", __func__);
                log_tag("debug_log.txt", "[DEBUG]", "%s():    G_RNG_ADVANCEMENTS == {%" PRId64 "}", __func__, G_RNG_ADVANCEMENTS);
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Seed: {%s}", __func__, (*game_path)->seed);
            }
        } // End of debug floor regen
#endif // HELAPORDO_DEBUG_ACCESS

        Gui_Button_Group group = gui_state->floor_buttons;
        for (Gui_Floor_Group_Button_Index i = 0; i < group.len; i++) {
            group.buttons[i].on = false;
            switch (i) {
            case BUTTON_ARTIFACTS: {
                int bt_w = gui_state->gameScreenWidth*0.2f;
                int bt_h = gui_state->gameScreenHeight*0.1f;
                group.buttons[i].r = (Rectangle) {
                    .x = gui_state->gameScreenWidth - (bt_w*2),
                    .y = (gui_state->gameScreenHeight - bt_h)*0.5f,
                    .width = bt_w,
                    .height = bt_h
                };
            }
            break;
            }
        }
        for (Gui_Floor_Group_Button_Index i = 0; i < group.len; i++) {
            if (CheckCollisionPointRec(gui_state->virtualMouse, group.buttons[i].r)) {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    group.buttons[i].state = BUTTON_PRESSED;
                } else {
                    group.buttons[i].state = BUTTON_HOVER;
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    group.buttons[i].on = true;
                }
            } else {
                group.buttons[i].state = BUTTON_NORMAL;
            }
            if (group.buttons[i].on) {
                fprintf(stderr, "%s():    [EFFECT]\n", __func__);
                switch (i) {
                case BUTTON_ARTIFACTS: {
                    gui_state->currentScreen = ARTIFACTS_VIEW;
                }
                break;
                }
            }
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

        if ((*current_room)->class == BASIC) {
            gui_state->currentScreen = FLOOR_VIEW;
        }

        if ((*current_room)->class == ENEMIES) {
            Gui_Button_Group group = gui_state->fight_buttons;
            for (Gui_Fight_Group_Button_Index i = 0; i < group.len; i++) {
                group.buttons[i].on = false;
            }
            if (!(*pause_animation)) {
                *current_anim_frame = (gui_state->framesCounter)%60;
            }

            for (Gui_Fight_Group_Button_Index i = 0; i < group.len; i++) {

                if (CheckCollisionPointRec(gui_state->virtualMouse, group.buttons[i].r)) {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        group.buttons[i].state = BUTTON_PRESSED;
                    } else {
                        group.buttons[i].state = BUTTON_HOVER;
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                        group.buttons[i].on = true;
                    }
                } else {
                    group.buttons[i].state = BUTTON_NORMAL;
                }
                if (group.buttons[i].on) {
                    fprintf(stderr, "%s():    [EFFECT]\n", __func__);
                    if (i == BUTTON_FIGHT) {
                        Boss *dummy_boss = NULL;
                        Enemy* enemy = (*current_room)->enemies[(*gamestate)->current_enemy_index];
                        FILE *args_save_file = NULL;
                        foeTurnOption_OP dummy_foe_op = FOE_OP_INVALID;
                        skillType dummy_picked_skill = -1;

                        Rectangle rb_r = CLITERAL(Rectangle) {
                            gui_state->gameScreenWidth*0.1f,
                                      gui_state->gameScreenHeight*0.65f,
                                      gui_state->gameScreenWidth*0.8f,
                                      gui_state->gameScreenHeight*0.3f,
                        };

                        //Declare turnOP_args
                        turnOP_args *args =
                            init_turnOP_args(*gamestate, *player, *game_path, *current_room, load_info, enemy,
                                             dummy_boss, args_save_file, &rb_r, *floor_kls,
                                             dummy_foe_op, dummy_picked_skill, rb_notifications);
                        args->foe_op =
                            foeTurnOP_from_foeTurnOption(enemyTurnPick(enemy, *player));
                        log_tag("debug_log.txt", "[FOETURN]", "foePick was: [ %s ]",
                                stringFromFoeTurnOP(args->foe_op));

                        OP_res fightStatus = turnOP(OP_FIGHT, args, default_kls, *floor_kls);
                        //Lost battle
                        if (fightStatus == OP_RES_DEATH) {
                            //Account for oracle gift perk
                            int oracle_perks = (*player)->perks[ORACLE_GIFT]->innerValue;
                            if (oracle_perks > 0) {
                                (*player)->hp = round((*player)->totalhp / 3);

                                (*player)->perks[ORACLE_GIFT]->innerValue -= 1;

                                e_death(enemy);
                                log_tag("debug_log.txt", "[DEBUG-ROOM-PERKS]",
                                        "Oraclegift proc.");

                                if ((*current_room)->foes->tot_alive > 0) {
                                    //Go to next battle
                                    //Display current party info
                                    //display_printFoeParty(room->foes);
                                    (*gamestate)->current_enemy_index++;
                                    fightStatus = OP_RES_NO_DMG;
                                    log_tag("debug_log.txt", "[ROOM]",
                                            "Onto next enemy, %i left.", (*current_room)->foes->tot_alive - (*current_room)->foes->current_index);
                                } else {
                                    log_tag("debug_log.txt", "DEBUG", "%s():    setting current_room to NULL", __func__);
                                    *current_room = NULL;
                                    (*current_floor)->roomclass_layout[*current_x][*current_y] = BASIC;
                                    (*gamestate)->current_enemy_index = 0;
                                    gui_state->currentScreen = FLOOR_VIEW;
                                    break;
                                }
                                break;	//We go to next enemy
                            }

                            e_death(enemy);
                            int player_luck = (*player)->luck;
                            log_tag("debug_log.txt", "[DEBUG]", "Player luck was [%i]",
                                    player_luck);
                            death((*player), load_info);
                            //free(room->foes);

                            //printf("\t\tLuck:  %i Path luck:  %i\n",player_luck,p->luck);
                            //red();
                            //printf("\n\n\tYOU DIED.\n");
                            //white();
                            //free(args);
                            log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
                            *current_room = NULL;
                            gui_state->currentScreen = ENDING;
                            break;
                        } else if (fightStatus == OP_RES_KILL_DONE) {
                            //Give coins

                            (*player)->balance += enemy->prize;
                            (*player)->stats->coinsfound += enemy->prize;

                            char msg[50];
                            sprintf(msg, "You found +%i coins.", enemy->prize);
                            enqueue_notification(msg, 500, S4C_BRIGHT_YELLOW, rb_notifications);

                            //Win, get xp and free memory from enemy
                            int special_unlock = giveXp((*player), enemy);

                            e_death(enemy);

                            (*current_room)->foes->alive_enemies[(*current_room)->foes->current_index] = 0;
                            (*current_room)->foes->current_index++;
                            (*current_room)->foes->tot_alive--;

                            if ((*current_room)->foes->tot_alive > 0) {
                                //Go to next battle
                                //Display current party info
                                //display_printFoeParty(room->foes);
                                (*gamestate)->current_enemy_index++;
                                fightStatus = OP_RES_NO_DMG;
                                log_tag("debug_log.txt", "[ROOM]",
                                        "Onto next enemy, %i left.", (*current_room)->foes->tot_alive - (*current_room)->foes->current_index);
                            } else {
                                log_tag("debug_log.txt", "DEBUG", "%s():    setting current_room to NULL", __func__);
                                *current_room = NULL;
                                (*current_floor)->roomclass_layout[*current_x][*current_y] = BASIC;
                                (*gamestate)->current_enemy_index = 0;
                                (*roomsDone) += 1;
                                (*player)->stats->roomscompleted += 1;
                                gui_state->currentScreen = FLOOR_VIEW;
                                if (special_unlock == 1) {
                                    gui_state->currentScreen = UNLOCK_SPECIAL_VIEW;
                                }
                                break;
                            }

                            if (special_unlock == 1) {
                                gui_state->currentScreen = UNLOCK_SPECIAL_VIEW;
                            }
                            break;
                        }

                        // End of fight button
                    } else if (i == BUTTON_SPECIAL) {
                        if ((*player)->stats->specialsunlocked > 0) {
                            gui_state->currentScreen = PICK_SPECIAL_VIEW;
                        }
                    } else if (i == BUTTON_EQUIPS) {
                        gui_state->currentScreen = EQUIPS_VIEW;
                    } else if (i == BUTTON_CONSUMABLES) {
                        gui_state->currentScreen = CONSUMABLES_VIEW;
                    } else if (i == BUTTON_STATS) {
                        gui_state->currentScreen = STATS_VIEW;
                    }
                }
            }
        } else if ((*current_room)->class == BOSS) {
            Gui_Button_Group group = gui_state->fight_buttons;
            for (Gui_Fight_Group_Button_Index i = 0; i < group.len; i++) {
                group.buttons[i].on = false;
            }
            if (!(*pause_animation)) {
                *current_anim_frame = (gui_state->framesCounter)%60;
            }

            for (Gui_Fight_Group_Button_Index i = 0; i < group.len; i++) {

                if (CheckCollisionPointRec(gui_state->virtualMouse, group.buttons[i].r)) {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        group.buttons[i].state = BUTTON_PRESSED;
                    } else {
                        group.buttons[i].state = BUTTON_HOVER;
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                        group.buttons[i].on = true;
                    }
                } else {
                    group.buttons[i].state = BUTTON_NORMAL;
                }
                if (group.buttons[i].on) {
                    fprintf(stderr, "%s():    [EFFECT]\n", __func__);
                    //TODO: may use i to se is_new_game for now but its weak to changes in the array
                    // load_info->is_new_game = i;
                    if (i == BUTTON_FIGHT) { // New game is the first button
                        Boss *boss = (*current_room)->boss;
                        Enemy* dummy_enemy = NULL;
                        FILE *args_save_file = NULL;
                        foeTurnOption_OP dummy_foe_op = FOE_OP_INVALID;
                        skillType dummy_picked_skill = -1;

                        Rectangle rb_r = CLITERAL(Rectangle) {
                            gui_state->gameScreenWidth*0.1f,
                                      gui_state->gameScreenHeight*0.65f,
                                      gui_state->gameScreenWidth*0.8f,
                                      gui_state->gameScreenHeight*0.3f,
                        };

                        //Declare turnOP_args
                        turnOP_args *args =
                            init_turnOP_args(*gamestate, *player, *game_path, *current_room, load_info, dummy_enemy,
                                             boss, args_save_file, &rb_r, *floor_kls,
                                             dummy_foe_op, dummy_picked_skill, rb_notifications);

                        args->foe_op =
                            foeTurnOP_from_foeTurnOption(bossTurnPick(boss, *player));
                        log_tag("debug_log.txt", "[FOETURN]", "foePick was: [ %s ]",
                                stringFromFoeTurnOP(args->foe_op));

                        OP_res fightStatus = turnOP(OP_FIGHT, args, default_kls, *floor_kls);
                        //Lost battle
                        if (fightStatus == OP_RES_DEATH) {
                            //Account for oracle gift perk
                            int oracle_perks = (*player)->perks[ORACLE_GIFT]->innerValue;
                            if (oracle_perks > 0) {
                                (*player)->hp = round((*player)->totalhp / 3);

                                (*player)->perks[ORACLE_GIFT]->innerValue -= 1;

                                b_death(boss);
                                log_tag("debug_log.txt", "[DEBUG-ROOM-PERKS]",
                                        "Oraclegift proc.");

                                log_tag("debug_log.txt", "DEBUG", "%s():    setting current_room to NULL", __func__);
                                *current_room = NULL;
                                (*current_floor)->roomclass_layout[*current_x][*current_y] = BASIC;
                                (*gamestate)->current_enemy_index = 0;
                                gui_state->currentScreen = FLOOR_VIEW;
                                break;
                            }

                            b_death(boss);
                            int player_luck = (*player)->luck;
                            log_tag("debug_log.txt", "[DEBUG]", "Player luck was [%i]",
                                    player_luck);
                            death((*player), load_info);
                            //free(room->foes);

                            //printf("\t\tLuck:  %i Path luck:  %i\n",player_luck,p->luck);
                            //red();
                            //printf("\n\n\tYOU DIED.\n");
                            //white();
                            //free(args);
                            log_tag("debug_log.txt", "[FREE]", "Freed turnOP_args");
                            *current_room = NULL;
                            gui_state->currentScreen = ENDING;
                            break;
                        } else if (fightStatus == OP_RES_KILL_DONE) {
                            //Give coins

                            (*player)->balance += boss->prize;
                            (*player)->stats->coinsfound += boss->prize;

                            char msg[50];
                            sprintf(msg, "You found +%i coins.", boss->prize);
                            enqueue_notification(msg, 500, S4C_BRIGHT_YELLOW, rb_notifications);

                            //Consumable drop
                            int consDrop = dropConsumable(*player);
                            log_tag("debug_log.txt", "[DEBUG]", "consDrop was (%i)", consDrop);

                            //Artifact drop (if we don't have all of them)
                            if (((*player)->stats->artifactsfound != ARTIFACTSMAX + 1)) {
                                int artifactDrop = dropArtifact(*player);
                                log_tag("debug_log.txt", "[DEBUG]", "artifactDrop was (%i)",
                                        artifactDrop);
                            }
                            //Equip drop
                            dropEquip(*player, boss->beast, default_kls, rb_notifications);

                            //Account for harvester perk
                            int harvester_perks = (*player)->perks[HARVESTER]->innerValue;
                            if (harvester_perks > 0) {
                                float xpboost = harvester_perks * 0.1;
                                boss->xp *= (1 + xpboost);
                            }

                            //Give key
                            (*player)->keys_balance += 1;
                            (*player)->stats->keysfound += 1;

                            enqueue_notification("You found a key. May be useful.", 800, S4C_MAGENTA, rb_notifications);


                            //Win, get xp and free memory from enemy
                            int special_unlock = giveXp_Boss((*player), boss);

                            b_death(boss);

                            log_tag("debug_log.txt", "DEBUG", "%s():    setting current_room to NULL", __func__);
                            *current_room = NULL;
                            (*current_floor)->roomclass_layout[*current_x][*current_y] = BASIC;
                            (*gamestate)->current_enemy_index = 0;
                            (*roomsDone) += 1;
                            (*player)->stats->roomscompleted += 1;
                            gui_state->currentScreen = FLOOR_VIEW;
                            (*current_floor)->
                            roomclass_layout[(*player)->floor_x][(*player)->floor_y] =
                                BASIC;
                            (*player)->stats->floorscompleted++;
                            log_tag("debug_log.txt", "[DEBUG]",
                                    "Floors done: [%i]", (*player)->stats->floorscompleted);
                            //Check if we need to update the win condition
                            if ((*game_path)->win_condition->class == FULL_PATH) {
                                (*game_path)->win_condition->current_val++;
                                if ((*game_path)->win_condition->current_val >= (*game_path)->win_condition->target_val) {
                                    log_tag("debug_log.txt", "[DEBUG]",
                                            "Game won: [%i/%i]", (*game_path)->win_condition->current_val, (*game_path)->win_condition->target_val);
                                    gui_state->game_won = true;
                                    gui_state->currentScreen = ENDING;
                                    return;
                                }
                            }
                            // Reset floor_kls
                            kls_temp_end(*floor_kls);
                            *floor_kls =
                                kls_temp_start(temporary_kls);

                            (*current_floor) =
                                (Floor *)
                                KLS_PUSH_T_TYPED(*floor_kls, Floor,
                                                 HR_Floor, "Floor",
                                                 "Floor");
                            /*
                            update_Gamestate(gamestate, 1, HOME,
                                             roomsDone, -1,
                                             current_floor, NULL, &game_options); // Passing NULL for current_room
                            */

                            //Regenerate floor
                            log_tag("debug_log.txt", "[DEBUG]",
                                    "Beaten a boss, regenerating current floor.");
                            // Init
                            init_floor_layout(*current_floor);
                            //Set center as filled
                            (*current_floor)->
                            floor_layout[center_x][center_y] = 1;
                            //Init floor rooms
                            init_floor_rooms(*current_floor);
                            if ((hlpd_rand() % 101) > 20) {
                                log_tag("debug_log.txt", "[DEBUG]", "%s():    Doing bsp init", __func__);
                                BSP_Room* bsp_tree = floor_bsp_gen(*current_floor, *floor_kls, center_x, center_y);
                                dbg_BSP_Room(bsp_tree);
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
                                (*player)->floor_x = center_x;
                                (*player)->floor_y = center_y;
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
                                (*player)->floor_x = home_room_x;
                                (*player)->floor_y = home_room_y;
                            }
                            *current_x = (*player)->floor_x;
                            *current_y = (*player)->floor_y;
                            if (special_unlock == 1) {
                                gui_state->currentScreen = UNLOCK_SPECIAL_VIEW;
                            }
                            break;
                        }

                        // End of fight button
                    } else if (i == BUTTON_SPECIAL) {
                        if ((*player)->stats->specialsunlocked > 0) {
                            gui_state->currentScreen = PICK_SPECIAL_VIEW;
                        }
                    } else if (i == BUTTON_EQUIPS) {
                        gui_state->currentScreen = EQUIPS_VIEW;
                    } else if (i == BUTTON_CONSUMABLES) {
                        gui_state->currentScreen = CONSUMABLES_VIEW;
                    } else if (i == BUTTON_STATS) {
                        gui_state->currentScreen = STATS_VIEW;
                    }
                }
            }
        } else if ((*current_room)->class == TREASURE) {
            Gui_Button_Group group = gui_state->treasure_buttons;
            for (Gui_Treasure_Group_Button_Index i = 0; i < group.len; i++) {
                group.buttons[i].on = false;
            }

            for (Gui_Treasure_Group_Button_Index i = 0; i < group.len; i++) {

                if (CheckCollisionPointRec(gui_state->virtualMouse, group.buttons[i].r)) {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        group.buttons[i].state = BUTTON_PRESSED;
                    } else {
                        group.buttons[i].state = BUTTON_HOVER;
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                        group.buttons[i].on = true;
                    }
                } else {
                    group.buttons[i].state = BUTTON_NORMAL;
                }
                if (group.buttons[i].on) {
                    fprintf(stderr, "%s():    [EFFECT]\n", __func__);
                    if (i == BUTTON_LEAVE_TREASURE) {
                        log_tag("debug_log.txt", "DEBUG", "%s():    Leaving treasure behind.", __func__);
                        log_tag("debug_log.txt", "DEBUG", "%s():    setting current_room to NULL", __func__);
                        *current_room = NULL;
                        (*current_floor)->roomclass_layout[*current_x][*current_y] = BASIC;
                        gui_state->currentScreen = FLOOR_VIEW;
                    } else if (i == BUTTON_TAKE_TREASURE) {
                        switch ((*current_room)->treasure->class) {
                        case TREASURE_CHEST: {
                            if ((*player)->keys_balance > 0) {
                                log_tag("debug_log.txt", "[TREASURE]",
                                        "Opened chest in Treasure room, index %i.\n",
                                        (*current_room)->index);
                                (*player)->keys_balance--;
                                open_chest(rb_notifications, (*current_room)->treasure->chest, *player, default_kls, *floor_kls);
                                *current_room = NULL;
                                (*current_floor)->roomclass_layout[*current_x][*current_y] = BASIC;
                                gui_state->currentScreen = CHEST_ANIM;
                                *current_anim_frame = 0;
                            } else {
                                //TODO: display this in some way
                                //mvwprintw(win, 18, 5, "You don't have any key.");
                                log_tag("debug_log.txt", "[TREASURE]",
                                        "Tried Opening a chest in Treasure room with no keys, index %i.\n",
                                        (*current_room)->index);
                            }
                        }
                        break;
                        case TREASURE_CONSUMABLE: {
                            Consumable *bagged =
                                (Consumable *) (*player)->consumablesBag[(*current_room)->treasure->
                                    consumable->class];
                            bagged->qty += (*current_room)->treasure->consumable->qty;
                            (*player)->stats->consumablesfound++;
                            char msg[100];
                            sprintf(msg, "You found a %s!", stringFromConsumables(bagged->class));
                            enqueue_notification(msg, 500, S4C_BRIGHT_GREEN, rb_notifications);
                            log_tag("debug_log.txt", "DEBUG", "%s():    setting current_room to NULL", __func__);
                            *current_room = NULL;
                            (*current_floor)->roomclass_layout[*current_x][*current_y] = BASIC;
                            gui_state->currentScreen = FLOOR_VIEW;
                        }
                        break;
                        case TREASURE_ARTIFACT: {
                            Artifact *bagged =
                                (Artifact *) (*player)->artifactsBag[(*current_room)->treasure->artifact->
                                    class];
                            bagged->qty += 1;
                            (*player)->stats->artifactsfound++;
                            char msg[100];
                            sprintf(msg, "You found a %s!", stringFromArtifacts(bagged->class));
                            enqueue_notification(msg, 500, S4C_CYAN, rb_notifications);
                            log_tag("debug_log.txt", "DEBUG", "%s():    setting current_room to NULL", __func__);
                            *current_room = NULL;
                            (*current_floor)->roomclass_layout[*current_x][*current_y] = BASIC;
                            gui_state->currentScreen = FLOOR_VIEW;
                        }
                        break;
                        }
                    }
                }
            }
        } else if ((*current_room)->class == SHOP) {
            Shop* shop = (*current_room)->shop;
            Gui_Button_Group* equips_group = gui_state->shop_buttons.groups[SHOP_LAYOUT_EQUIPS_GROUP];
            equips_group->cell_width = ((int)(gui_state->gameScreenWidth * 0.175f) / 12) * 12;
            equips_group->cell_height = ((int)(gui_state->gameScreenHeight * 0.175f) / 8) * 8;
            equips_group->cell_w_spacing = 10;
            int equip_cells = shop->equipsCount;
            equips_group->len = equip_cells;
            int equips_r_width = (equips_group->cell_width + equips_group->cell_w_spacing) * equip_cells;
            equips_group->x = (gui_state->gameScreenWidth - equips_r_width)/2;
            equips_group->y = 50;

            Gui_Button_Group* consumables_group = gui_state->shop_buttons.groups[SHOP_LAYOUT_CONSUMABLES_GROUP];
            consumables_group->cell_width = equips_group->cell_width;
            consumables_group->cell_height = equips_group->cell_height;
            consumables_group->cell_w_spacing = equips_group->cell_w_spacing;
            int consumable_cells = shop->consumablesCount;
            consumables_group->len = consumable_cells;
            int consumables_r_width = (consumables_group->cell_width + consumables_group->cell_w_spacing) * consumable_cells;
            int consumables_r_h_spacing = equips_group->cell_height*2;
            consumables_group->x = (gui_state->gameScreenWidth - consumables_r_width)/2;
            consumables_group->y = equips_group->y + equips_group->cell_height + consumables_r_h_spacing;

            Gui_Button_Group* others_group = gui_state->shop_buttons.groups[SHOP_LAYOUT_OTHERS_GROUP];
            others_group->cell_width = 80;
            others_group->cell_height = 50;
            others_group->cell_w_spacing = 10;
            others_group->x = 20;
            others_group->y = 200;

            for (Gui_Shop_Layout_Group_Index i = 0; i < gui_state->shop_buttons.len; i++) {
                Gui_Button_Group* group = gui_state->shop_buttons.groups[i];
                for (int j = 0; j < group->len; j++) {
                    Rectangle cell = {
                        .x = group->x + (j*group->cell_width) + (j*group->cell_w_spacing),
                        .y = group->y,
                        .width = group->cell_width,
                        .height = group->cell_height,
                    };
                    group->buttons[j].r = cell;
                    //TODO: Detect mouse in cells
                    if (CheckCollisionPointRec(gui_state->virtualMouse, cell)) {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                            group->buttons[j].state = BUTTON_PRESSED;
                        } else {
                            group->buttons[j].state = BUTTON_HOVER;
                        }
                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                            group->buttons[j].on = true;
                        }
                    } else {
                        group->buttons[j].state = BUTTON_NORMAL;
                    }
                    if (group->buttons[j].on) {
                        switch (i) {
                        case SHOP_LAYOUT_EQUIPS_GROUP: {
                            if ((*player)->equipsBagOccupiedSlots >= EQUIPSBAGSIZE) {
                                // TODO: Handle full bag by asking player if we throw something away
                                log_tag("debug_log.txt", "[DEBUG]", "%s():    Player equip bag was full. Not buying.", __func__);
                                return;
                            }
                            Equip* equip = shop->equips[j];
                            int price = shop->equipPrices[j];
                            if ((*player)->balance >= price) {
                                int slotnum = (*player)->equipsBagOccupiedSlots;
                                //We create a deep copy of the equip so we can free the shop without worrying about the memory sharing with the bag.
                                log_tag("debug_log.txt", "[SHOP]",
                                        "Buying Equip %s, deep copy stuff.",
                                        stringFromEquips(equip->class));
                                //TODO
                                //Should use a function to avoid refactoring more points when changing Equip generation.
                                log_tag("debug_log.txt", "[SHOP]",
                                        "Prepping Equip for purchase, push to raw default_kls.");
                                kls_log(default_kls, "DEBUG",
                                        "Prepping Equip for purchase, push to raw default_kls.");
                                Equip *saved =
                                    (Equip *) KLS_PUSH_TYPED(default_kls, Equip,
                                                             HR_Equip, "Equip",
                                                             "Equip");
                                Equip *to_save = equip;

                                saved->class = to_save->class;
                                saved->type = to_save->type;
                                strcpy(saved->name, to_save->name);
                                strcpy(saved->desc, to_save->desc);
                                saved->qty = to_save->qty;
                                saved->equipped = 0;
                                saved->level = to_save->level;
                                saved->atk = to_save->atk;
                                saved->def = to_save->def;
                                saved->vel = to_save->vel;
                                saved->enr = to_save->enr;
                                saved->bonus = to_save->bonus;
                                saved->perksCount = 0;	//Will be set during perks copy
                                saved->qual = to_save->qual;
                                saved->equip_fun = to_save->equip_fun;

                                for (int k = 0; k < to_save->perksCount; k++) {
                                    log_tag("debug_log.txt", "[SHOP]",
                                            "Prepping Perk (%i/%i) for Equip purchase, push to raw default_kls.",
                                            k, to_save->perksCount);
                                    kls_log(default_kls, "DEBUG",
                                            "Prepping Perk (%i/%i) for Equip purchase, push to raw default_kls.",
                                            k, to_save->perksCount);
                                    Perk *save_pk =
                                        (Perk *) KLS_PUSH_TYPED(default_kls, Perk,
                                                                HR_Perk, "Perk",
                                                                "Perk");
                                    save_pk->class = to_save->perks[k]->class;
                                    strcpy(save_pk->name, to_save->perks[k]->name);
                                    strcpy(save_pk->desc, to_save->perks[k]->desc);
                                    save_pk->innerValue =
                                        to_save->perks[k]->innerValue;
                                    saved->perks[saved->perksCount] = save_pk;
                                    saved->perksCount++;
                                }

                                for (int k = 0; k < 8; k++) {
                                    strcpy(saved->sprite[k], to_save->sprite[k]);
                                }

                                (*player)->equipsBag[slotnum] = saved;
                                (*player)->equipsBagOccupiedSlots++;
                                (*player)->earliestBagSlot = (*player)->equipsBagOccupiedSlots;
                                (*player)->balance -= price;
                                *current_room = NULL;
                                (*current_floor)->roomclass_layout[*current_x][*current_y] = BASIC;
                                gui_state->currentScreen = FLOOR_VIEW;
                                return; // End of update step
                            } else {
                                //TODO
                                //PRINT NOT ENOUGH MONEY
                                log_tag("debug_log.txt", "[SHOP]",
                                        "Buying Equip %s, TODO: Print NOT ENOUGH MONEY.\n",
                                        stringFromEquips(equip->class));
                            }
                        }
                        break;
                        case SHOP_LAYOUT_CONSUMABLES_GROUP: {
                            Consumable* c = shop->consumables[j];
                            int price =
                                shop->consumablePrices[j];
                            int qty = c->qty;
                            if ((*player)->balance >= price * qty) {
                                log_tag("debug_log.txt", "[SHOP]",
                                        "Buying x%i of Consumable %s.\n", qty,
                                        stringFromConsumables(c->
                                                              class));
                                Consumable *bagged =
                                    (Consumable *) (*player)->
                                    consumablesBag[c->class];
                                bagged->qty += qty;
                                (*player)->balance -= (qty * price);
                                *current_room = NULL;
                                (*current_floor)->roomclass_layout[*current_x][*current_y] = BASIC;
                                gui_state->currentScreen = FLOOR_VIEW;
                                return; // End of update step
                            } else {
                                //TODO
                                //PRINT NOT ENOUGH MONEY
                                log_tag("debug_log.txt", "[SHOP]",
                                        "Buying Consumable %s, TODO: Print NOT ENOUGH MONEY.\n",
                                        stringFromConsumables(c->
                                                              class));
                            }
                        }
                        break;
                        case SHOP_LAYOUT_OTHERS_GROUP: {
                            switch (j) {
                            case BUTTON_LEAVE_SHOP: {
                                log_tag("debug_log.txt", "[SHOP]",
                                        "Leaving shop");
                                *current_room = NULL;
                                (*current_floor)->roomclass_layout[*current_x][*current_y] = BASIC;
                                gui_state->currentScreen = FLOOR_VIEW;
                                return; // End of update step
                            }
                            break;
                            case BUTTON_SELL_ALL: {
                                log_tag("debug_log.txt", "[SHOP]",
                                        "Selling all Equips");
                                sell_all_equips(*player);
                                *current_room = NULL;
                                (*current_floor)->roomclass_layout[*current_x][*current_y] = BASIC;
                                gui_state->currentScreen = FLOOR_VIEW;
                                return; // End of update step
                            }
                            break;
                            default: {
                            }
                            break;
                            }
                        }
                        break;
                        }
                        group->buttons[j].on = false;
                    }
                }
            }
        } else if ((*current_room)->class == HOME) {
            // Press Q to change to FLOOR_VIEW screen
            if (IsKeyPressed(KEY_Q)) {
                log_tag("debug_log.txt", "DEBUG", "%s():    setting current_room to NULL", __func__);
                *current_room = NULL;
                gui_state->currentScreen = FLOOR_VIEW;
            }
        } else {
            // Press enter to change to FLOOR_VIEW screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                log_tag("debug_log.txt", "DEBUG", "%s():    setting current_room to NULL", __func__);
                *current_room = NULL;
                gui_state->currentScreen = FLOOR_VIEW;
            }
        }

    }
    break;
    case PICK_SPECIAL_VIEW: {
        // TODO: Update PICK_SPECIAL_VIEW screen variables here!
        Gui_Button_Group group = gui_state->special_buttons;
        for (Gui_Special_Group_Button_Index i = 0; i < group.len; i++) {
            group.buttons[i].on = false;
        }
        for (Gui_Special_Group_Button_Index i = 0; i < group.len; i++) {
            if (((*player)->specials[i]->enabled)) {
                if (CheckCollisionPointRec(gui_state->virtualMouse, group.buttons[i].r)) {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        group.buttons[i].state = BUTTON_PRESSED;
                    } else {
                        group.buttons[i].state = BUTTON_HOVER;
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                        group.buttons[i].on = true;
                    }
                } else {
                    group.buttons[i].state = BUTTON_NORMAL;
                }
                if (group.buttons[i].on) {
                    fprintf(stderr, "%s():    [EFFECT]\n", __func__);
                    Rectangle special_notice_r = CLITERAL(Rectangle) {
                        gui_state->gameScreenHeight *0.5f,
                                  gui_state->gameScreenWidth *0.5f,
                                  (gui_state->gameScreenHeight*0.3f),
                                  (gui_state->gameScreenWidth*0.3f),
                    };
                    Enemy* enemy = (*current_room)->enemies[(*gamestate)->current_enemy_index];
                    Boss* boss = (*current_room)->boss;
                    int enemyIndex = (*gamestate)->current_enemy_index;
                    if ((*player)->specials[i]->cost <= (*player)->energy + (*player)->equipboost_enr) {
                        fight_Special((*player)->specials[i]->move, &special_notice_r, *player, enemy, boss,
                                      *game_path, *roomsDone, enemyIndex, (*current_room)->class == BOSS);
                    }
                    gui_state->currentScreen = ROOM_VIEW;
                }
            }
        }
    }
    break;
    case UNLOCK_SPECIAL_VIEW: {
        // TODO: Update UNLOCK_SPECIAL_VIEW screen variables here!
        Gui_Button_Group group = gui_state->special_buttons;
        for (Gui_Special_Group_Button_Index i = 0; i < group.len; i++) {
            group.buttons[i].on = false;
        }
        for (Gui_Special_Group_Button_Index i = 0; i < group.len; i++) {
            if (!((*player)->specials[i]->enabled)) {
                if (CheckCollisionPointRec(gui_state->virtualMouse, group.buttons[i].r)) {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        group.buttons[i].state = BUTTON_PRESSED;
                    } else {
                        group.buttons[i].state = BUTTON_HOVER;
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                        group.buttons[i].on = true;
                    }
                } else {
                    group.buttons[i].state = BUTTON_NORMAL;
                }
                if (group.buttons[i].on) {
                    fprintf(stderr, "%s():    [EFFECT]\n", __func__);
                    Specialslot *selected = (*player)->specials[i];

                    //Check if the selected move is NOT enabled
                    if (!(selected->enabled)) {
                        //Enable the move
                        selected->enabled = 1;
                    }
                    (*player)->stats->specialsunlocked += 1;
                    if (*current_room != NULL) {
                        gui_state->currentScreen = ROOM_VIEW;
                    } else {
                        gui_state->currentScreen = FLOOR_VIEW;
                    }
                }
            }
        }
    }
    break;
    case EQUIPS_VIEW: {
        if (IsKeyPressed(KEY_Q)) {
            gui_state->currentScreen = ROOM_VIEW;
        }
        // TODO: Update EQUIPS_VIEW screen variables here!

        Gui_Button_Group group = gui_state->equips_buttons;
        for (Gui_Equips_Group_Button_Index i = 0; i < group.len; i++) {
            group.buttons[i].on = false;
        }
        for (Gui_Equips_Group_Button_Index i = 0; i < group.len; i++) {
            if (CheckCollisionPointRec(gui_state->virtualMouse, group.buttons[i].r)) {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    group.buttons[i].state = BUTTON_PRESSED;
                } else {
                    group.buttons[i].state = BUTTON_HOVER;
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    group.buttons[i].on = true;
                }
            } else {
                group.buttons[i].state = BUTTON_NORMAL;
            }
            if (group.buttons[i].on) {
                fprintf(stderr, "%s():    [EFFECT]\n", __func__);
                //Specialslot *selected = (*player)->specials[i - BUTTON_SPECIAL_UNLOCK_1];

                //Check if the selected move is NOT enabled
                //if (!(selected->enabled)) {
                //Enable the move
                //   selected->enabled = 1;
                //}
                //(*player)->stats->specialsunlocked += 1;
                if (i == BUTTON_OPEN_BAG) {
                    gui_state->currentScreen = OPEN_BAG_VIEW;
                } else if (i == BUTTON_CHECK_LOADOUT) {
                    gui_state->currentScreen = CHECK_LOADOUT_VIEW;
                }
            }
        }
    }
    break;
    case OPEN_BAG_VIEW: {
        // Press q to change to SAVES_VIEW screen
        if (IsKeyPressed(KEY_Q)) {
            gui_state->currentScreen = EQUIPS_VIEW;
            gui_state->selectedIndex = 0;
        }
        if (gui_state->selectedIndex >= (*player)->equipsBagOccupiedSlots) {
            gui_state->selectedIndex = 0;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            gui_state->selectedIndex += 1;
        }
        if (IsKeyPressed(KEY_UP)) {
            if (gui_state->selectedIndex > 0) gui_state->selectedIndex -= 1;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            //Retrieve item info
            Equip *e = (Equip *) (*player)->equipsBag[gui_state->selectedIndex];
            Equipslot *slot = (Equipslot *) (*player)->equipslots[e->type];
            if (slot->active == 1) {
                //We reset status for equipped item
                slot->item->equipped = 0;

                removeEquipPerks(slot->item, *player);

                //We adjust total boost removing current values
                (*player)->equipboost_atk -= slot->item->atk;
                (*player)->equipboost_def -= slot->item->def;
                (*player)->equipboost_vel -= slot->item->vel;
                (*player)->equipboost_enr -= slot->item->enr;
            };

            //We equip the item
            slot->item = e;
            slot->item->equipped = 1;

            applyEquipPerks(slot->item, *player);
            slot->active = 1;

            //Apply the new item stats
            (*player)->equipboost_atk += slot->item->atk;
            (*player)->equipboost_def += slot->item->def;
            (*player)->equipboost_vel += slot->item->vel;
            (*player)->equipboost_enr += slot->item->enr;
        }
    }
    break;
    case CHECK_LOADOUT_VIEW: {
        // Press q to change to EQUIPS_VIEW screen
        if (IsKeyPressed(KEY_Q)) {
            gui_state->currentScreen = EQUIPS_VIEW;
        }
    }
    break;
    case CONSUMABLES_VIEW: {
        // Press q to change to ROOM_VIEW screen
        if (IsKeyPressed(KEY_Q)) {
            gui_state->currentScreen = ROOM_VIEW;
            gui_state->selectedIndex = 0;
        }
        if (gui_state->selectedIndex >= CONSUMABLESMAX+1) {
            gui_state->selectedIndex = 0;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            gui_state->selectedIndex = (gui_state->selectedIndex + 1) % (CONSUMABLESMAX+1);
        }
        if (IsKeyPressed(KEY_UP)) {
            if (gui_state->selectedIndex > 0) gui_state->selectedIndex -= 1;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            Consumable* c = (*player)->consumablesBag[gui_state->selectedIndex];
            if (c->qty > 0) {
                Enemy* enemy = (*current_room)->enemies[(*gamestate)->current_enemy_index];
                Boss* boss = (*current_room)->boss;
                bool isBoss = false;
                if (boss) isBoss = true;
                useConsumable(*player, enemy, boss, consumablestrings[gui_state->selectedIndex], isBoss);
            }
        }
    }
    break;
    case STATS_VIEW: {
        // TODO: Update STATS_VIEW screen variables here!
        if (IsKeyPressed(KEY_Q)) {
            gui_state->currentScreen = (*current_room ? ROOM_VIEW : FLOOR_VIEW);
        }
    }
    break;
    case ARTIFACTS_VIEW: {
        // TODO: Update ARTIFACTS_VIEW screen variables here!
        if (IsKeyPressed(KEY_Q)) {
            gui_state->currentScreen = (*current_room ? ROOM_VIEW : FLOOR_VIEW);
            gui_state->selectedIndex = 0;
        }
        if (gui_state->selectedIndex >= ARTIFACTSMAX+1) {
            gui_state->selectedIndex = 0;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            gui_state->selectedIndex = (gui_state->selectedIndex + 1) % (ARTIFACTSMAX+1);
        }
        if (IsKeyPressed(KEY_UP)) {
            if (gui_state->selectedIndex > 0) gui_state->selectedIndex -= 1;
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
            if ((*current_floor)->roomclass_layout[*current_x][*current_y] == HOME) {
                log_tag("debug_log.txt", "[DEBUG]", "Doing autosave.");

                Enemy *dummy_enemy = NULL;
                Boss *dummy_boss = NULL;
                FILE *dummy_savefile = NULL;
                Rectangle rect = {0};
                RingaBuf *dummy_rb = NULL;
                foeTurnOption_OP dummy_foe_op = FOE_OP_INVALID;
                skillType dummy_skill_pick = -1;
                //Declare turnOP_args
                turnOP_args *args =
                    init_turnOP_args(*gamestate, *player, *game_path, *current_room, load_info, dummy_enemy,
                                     dummy_boss, dummy_savefile, &rect, *floor_kls,
                                     dummy_foe_op, dummy_skill_pick, dummy_rb);

                log_tag("debug_log.txt", "[DEBUG]", "%s():    Skipping preparing autosave file path", __func__);
                turnOP(turnOP_from_turnOption(SAVE), args, default_kls, *floor_kls);
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Done autosave.", __func__);
                log_tag("debug_log.txt", "[DEBUG]", "%s():    G_RNG_ADVANCEMENTS == {%" PRId64 "}", __func__, G_RNG_ADVANCEMENTS);
                log_tag("debug_log.txt", "[DEBUG]", "%s():    Seed: {%s}", __func__, (*game_path)->seed);
            }
            break;
        }
        (*current_anim_frame)++;
    }
    break;
    case CHEST_ANIM: {
        // TODO: Update CHEST_ANIM screen variables here!
        (gui_state->framesCounter) += 1;    // Count frames
        // TODO: Press enter to skip animation and go to room screen?
        if (*current_anim_frame == 24 ) { //|| IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
            gui_state->currentScreen = FLOOR_VIEW;
            break;
        }
        (*current_anim_frame)++;
    }
    break;
#ifdef HELAPORDO_DEBUG_ACCESS
    case DEBUG_VIEW: {
        // Press Enter to change to FLOOR_VIEW screen
        if (IsKeyPressed(KEY_ENTER)) {
            gui_state->currentScreen = FLOOR_VIEW;
        }
        int cycle_button_w = gui_state->gameScreenWidth * 0.2f;
        int cycle_button_h = gui_state->gameScreenWidth * 0.05f;
        int cycle_button_x = gui_state->gameScreenWidth - cycle_button_w;
        int cycle_button_y = (gui_state->gameScreenHeight - cycle_button_h) * 0.5f;
        Rectangle cycle_button_r = {
            .x = cycle_button_x,
            .y = cycle_button_y,
            .width = cycle_button_w,
            .height = cycle_button_h
        };
        Gui_Button* cycle_button = &(gui_state->debug_buttons.buttons[BUTTON_CYCLE_DEBUG_LAYOUT]);
        cycle_button->r = cycle_button_r;
        if (CheckCollisionPointRec(gui_state->virtualMouse, cycle_button->r)) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                cycle_button->state = BUTTON_PRESSED;
            } else {
                cycle_button->state = BUTTON_HOVER;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                cycle_button->on = true;
            }
        } else {
            cycle_button->state = BUTTON_NORMAL;
        }
        if (cycle_button->on) {
            cycle_button->on = false;
            Gui_Debug_Selection_Index next = (gui_state->debug_selection +1) % (GUI_DEBUG_SELECTION_MAX+1);
            fprintf(stderr, "%s(): cyclying debug_selection: {%i} -> {%i}\n", __func__, gui_state->debug_selection, next);
            gui_state->debug_selection = next;
        }

        switch (gui_state->debug_selection) {
        case GUI_DEBUG_FIGHTER: {
            int fighter_r_w = gui_state->gameScreenWidth * 0.5f;
            int fighter_r_h = gui_state->gameScreenHeight * 0.2f;
            Rectangle fighter_r = {
                .x = (gui_state->gameScreenWidth - fighter_r_w) /2,
                .y = (gui_state->gameScreenHeight/4) - (fighter_r_h/2),
                .width = fighter_r_w,
                .height = fighter_r_h
            };

            int fighter_inner_r_h = fighter_r.height * 0.25f;
            int inner_r_y = fighter_r.y;
            for (Gui_Debug_Fighter_Layout_Group_Index i = 0; i < gui_state->debug_fighter_buttons.len; i++) {
                Gui_Button_Group* group = gui_state->debug_fighter_buttons.groups[i];
                int cells_limit = -1;
                switch (i) {
                case DEBUG_FIGHTER_LAYOUT_SPECIALSLOTS_GROUP: {
                    cells_limit = SPECIALSMAX+1;
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_SKILLSLOTS_GROUP: {
                    cells_limit = FIGHTER_SKILL_SLOTS+1;
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_TURNCOUNTERS_GROUP: {
                    cells_limit = COUNTERSMAX+1;
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_PERKS_GROUP: {
                    cells_limit = PERKSMAX+1;
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_EQUIPSLOTS_GROUP: {
                    cells_limit = EQUIPZONES+1;
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_EQUIPSBAG_GROUP: {
                    cells_limit = EQUIPSBAGSIZE+1;
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_CONSUMABLESBAG_GROUP: {
                    cells_limit = CONSUMABLESMAX+1;
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_ARTIFACTSBAG_GROUP: {
                    cells_limit = ARTIFACTSMAX+1;
                }
                break;
                }
                for (int j = 0; j < cells_limit; j++) {
                    Rectangle cell = {
                        .x = fighter_r.x + (j * (fighter_r.width/(cells_limit))),
                        .y = inner_r_y,
                        .width = fighter_r.width/(cells_limit),
                        .height = fighter_inner_r_h,
                    };
                    Color c = RED;
                    switch (i) {
                    case DEBUG_FIGHTER_LAYOUT_SPECIALSLOTS_GROUP: {
                        if ((*player)->specials[j] != NULL) c = GREEN;
                    }
                    break;
                    case DEBUG_FIGHTER_LAYOUT_SKILLSLOTS_GROUP: {
                        if ((*player)->skills[j] != NULL) c = GREEN;
                    }
                    break;
                    case DEBUG_FIGHTER_LAYOUT_TURNCOUNTERS_GROUP: {
                        if ((*player)->counters[j] != NULL) c = GREEN;
                    }
                    break;
                    case DEBUG_FIGHTER_LAYOUT_PERKS_GROUP: {
                        if ((*player)->perks[j] != NULL) c = GREEN;
                    }
                    break;
                    case DEBUG_FIGHTER_LAYOUT_EQUIPSLOTS_GROUP: {
                        if ((*player)->equipslots[j] != NULL) c = GREEN;
                    }
                    break;
                    case DEBUG_FIGHTER_LAYOUT_EQUIPSBAG_GROUP: {
                        if ((*player)->equipsBag[j] != NULL) c = GREEN;
                    }
                    break;
                    case DEBUG_FIGHTER_LAYOUT_CONSUMABLESBAG_GROUP: {
                        if ((*player)->consumablesBag[j] != NULL) c = GREEN;
                    }
                    break;
                    case DEBUG_FIGHTER_LAYOUT_ARTIFACTSBAG_GROUP: {
                        if ((*player)->artifactsBag[j] != NULL) c = GREEN;
                    }
                    break;
                    }
                    group->buttons[j].r = cell;
                    group->buttons[j].box_color = c;
                    if (CheckCollisionPointRec(gui_state->virtualMouse, cell)) {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                            group->buttons[j].state = BUTTON_PRESSED;
                        } else {
                            group->buttons[j].state = BUTTON_HOVER;
                        }
                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                            group->buttons[j].on = true;
                        }
                    } else {
                        group->buttons[j].state = BUTTON_NORMAL;
                    }
                    if (group->buttons[j].on) {
                        group->buttons[j].on = false;
                        switch (i) {
                        case DEBUG_FIGHTER_LAYOUT_SPECIALSLOTS_GROUP: {
                            if ((*player)->specials[j] != NULL) {
                                Specialslot* slot = (*player)->specials[j];
                                fprintf(stderr, "%s(): Specialslot[%i]: { enabled %i move %i name %s desc %s cost %i }\n", __func__, j, slot->enabled, slot->move, slot->name, slot->desc, slot->cost);
                            } else {
                                fprintf(stderr, "%s(): Specialslot[%i]: { NULL }\n", __func__, j);
                            }
                        }
                        break;
                        case DEBUG_FIGHTER_LAYOUT_SKILLSLOTS_GROUP: {
                            if ((*player)->skills[j] != NULL) {
                                Skillslot* slot = (*player)->skills[j];
                                fprintf(stderr, "%s(): Skillslot[%i]: { enabled %i class %i name %s desc %s cost %i }\n", __func__, j, slot->enabled, slot->class, slot->name, slot->desc, slot->cost);
                            } else {
                                fprintf(stderr, "%s(): Skillslot[%i]: { NULL }\n", __func__, j);
                            }
                        }
                        break;
                        case DEBUG_FIGHTER_LAYOUT_TURNCOUNTERS_GROUP: {
                            if ((*player)->counters[j] != NULL) {
                                Turncounter* counter = (*player)->counters[j];
                                if (counter->desc != NULL) {
                                    fprintf(stderr, "%s(): Turncounters[%i]: { desc %s }\n", __func__, j, counter->desc);
                                } else {
                                    fprintf(stderr, "%s(): Turncounters[%i]: { desc NULL }\n", __func__, j);
                                }
                                fprintf(stderr, "%s(): Turncounters[%i]: { type %i count %i value %i }\n", __func__, j, counter->type, counter->count, counter->innerValue);
                                fprintf(stderr,
                                        "%s(): Turncounters[%i]: { effect_fun 0x%" PRIxPTR " effect_e_fun 0x%" PRIxPTR " effect_b_fun 0x%" PRIxPTR " effect_fp_fun 0x%" PRIxPTR " }\n",
                                        __func__, j,
                                        (uintptr_t)counter->effect_fun,
                                        (uintptr_t)counter->effect_e_fun,
                                        (uintptr_t)counter->effect_b_fun,
                                        (uintptr_t)counter->effect_fp_fun
                                       );
                                fprintf(stderr,
                                        "%s(): Turncounters[%i]: { boost_fun 0x%" PRIxPTR " boost_e_fun 0x%" PRIxPTR " boost_b_fun 0x%" PRIxPTR " boost_fp_fun 0x%" PRIxPTR " }\n",
                                        __func__, j,
                                        (uintptr_t)counter->boost_fun,
                                        (uintptr_t)counter->boost_e_fun,
                                        (uintptr_t)counter->boost_b_fun,
                                        (uintptr_t)counter->boost_fp_fun
                                       );
                            } else {
                                fprintf(stderr, "%s(): Turncounters[%i]: { NULL }\n", __func__, j);
                            }
                        }
                        break;
                        case DEBUG_FIGHTER_LAYOUT_PERKS_GROUP: {
                            if ((*player)->perks[j] != NULL) {
                                int class = (*player)->perks[j]->class;
                                if (class >= 0 && class < PERKSMAX+1) {
                                    fprintf(stderr, "%s(): Perks[%i]: { class %s }\n", __func__, j, nameStringFromPerk(class));
                                } else {
                                    fprintf(stderr, "%s(): Perks[%i]: { class %i }\n", __func__, j, class);
                                }
                                fprintf(stderr, "%s(): Perks[%i]: { name %s desc %s}\n", __func__, j, (*player)->perks[j]->name, (*player)->perks[j]->desc);
                            } else {
                                fprintf(stderr, "%s(): Perks[%i]: { NULL }\n", __func__, j);
                            }
                        }
                        break;
                        case DEBUG_FIGHTER_LAYOUT_EQUIPSLOTS_GROUP: {
                            if ((*player)->equipslots[j] != NULL) {
                                fprintf(stderr, "%s(): Equipsslot[%i]: { active %i }\n", __func__, j, (*player)->equipslots[j]->active);
                                Equip* eq = (*player)->equipslots[j]->item;
                                if (eq != NULL) {
                                    int class = eq->class;
                                    if (class >= 0 && class < EQUIPSMAX+1) {
                                        fprintf(stderr, "%s(): Equipslot[%i].item: Equip { class %s }\n", __func__, j, stringFromEquips(class));
                                    } else {
                                        fprintf(stderr, "%s(): Equipslot[%i].item: Equip { class %i }\n", __func__, j, class);
                                    }
                                } else {
                                    fprintf(stderr, "%s(): Equipslot[%i].item: Equip { NULL }\n", __func__, j);
                                }
                            } else {
                                fprintf(stderr, "%s(): Equipsslot[%i]: { NULL }\n", __func__, j);
                            }
                        }
                        break;
                        case DEBUG_FIGHTER_LAYOUT_EQUIPSBAG_GROUP: {
                            if ((*player)->equipsBag[j] != NULL) {
                                int class = (*player)->equipsBag[j]->class;
                                if (class >= 0 && class < EQUIPSMAX+1) {
                                    fprintf(stderr, "%s(): Equipsbag[%i]: { class %s }\n", __func__, j, stringFromEquips(class));
                                } else {
                                    fprintf(stderr, "%s(): Equipsbag[%i]: { class %i }\n", __func__, j, class);
                                }
                                fprintf(stderr, "%s(): Perks [\n", __func__);
                                for (int perk_idx = 0; perk_idx < EQUIPPERKSMAX; perk_idx++) {
                                    Perk* perk = (*player)->equipsBag[j]->perks[perk_idx];
                                    if (perk != NULL) {
                                        int perk_class = perk->class;
                                        if (perk_class >= 0 && perk_class < PERKSMAX+1) {
                                            fprintf(stderr, " [%i] { class %s }\n", perk_idx, nameStringFromPerk(perk_class));
                                        } else {
                                            fprintf(stderr, " [%i] { class %i }\n", perk_idx, perk_class);
                                        }
                                    } else {
                                        fprintf(stderr, " [%i] {NULL}\n", perk_idx);
                                    }
                                }
                                fprintf(stderr, "]\n");
                            } else {
                                fprintf(stderr, "%s(): Equipsbag[%i]: { NULL }\n", __func__, j);
                            }
                        }
                        break;
                        case DEBUG_FIGHTER_LAYOUT_CONSUMABLESBAG_GROUP: {
                            if ((*player)->consumablesBag[j] != NULL) {
                                int class = (*player)->consumablesBag[j]->class;
                                if (class >= 0 && class < CONSUMABLESMAX+1) {
                                    fprintf(stderr, "%s(): Consumablesbag[%i]: { class %s }\n", __func__, j, stringFromConsumables(class));
                                } else {
                                    fprintf(stderr, "%s(): Consumablesbag[%i]: { class %i }\n", __func__, j, class);
                                }
                            } else {
                                fprintf(stderr, "%s(): Consumablesbag[%i]: { NULL }\n", __func__, j);
                            }
                        }
                        break;
                        case DEBUG_FIGHTER_LAYOUT_ARTIFACTSBAG_GROUP: {
                            if ((*player)->artifactsBag[j] != NULL) {
                                int class = (*player)->artifactsBag[j]->class;
                                if (class >= 0 && class < ARTIFACTSMAX+1) {
                                    fprintf(stderr, "%s(): Artifactsbag[%i]: { class %s qty %i }\n", __func__, j, stringFromArtifacts(class), (*player)->artifactsBag[j]->qty);
                                } else {
                                    fprintf(stderr, "%s(): Artifactsbag[%i]: { class %i qty %i }\n", __func__, j, class, (*player)->artifactsBag[j]->qty);
                                }
                            } else {
                                fprintf(stderr, "%s(): Artifactsbag[%i]: { NULL }\n", __func__, j);
                            }
                        }
                        break;
                        }
                    }
                }
                inner_r_y += fighter_inner_r_h;
            }
        }
        break;
        case GUI_DEBUG_FLOOR: {
        }
        break;
        }
    }
    break;
#endif // HELAPORDO_DEBUG_ACCESS
    default: {
    }
    break;
    }
}

// void draw_GameScreen_Texture(RenderTexture2D target_txtr, GameScreen currentScreen, float gameScreenWidth, float gameScreenHeight, Vector2 mouse, Vector2 virtualMouse, int framesCounter, int fps_target, int current_anim_frame, Floor* current_floor, int current_x, int current_y, float scale, loadInfo* load_info, int saveslot_index, char current_save_path[1000])
void draw_GameScreen_Texture(RenderTexture2D target_txtr, Gui_State gui_state, int fps_target, int current_anim_frame, Floor* current_floor, Path* game_path, Fighter* player, Room* current_room, Gamestate* gamestate, RingaBuf* rb_notifications, int current_x, int current_y, loadInfo* load_info, int saveslot_index, char current_save_path[1500], char seed[PATH_SEED_BUFSIZE+1])
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
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("TITLE SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("Controls for FLOOR_VIEW screen", 110, 100, 20, gui_state.theme.txt_color);
        DrawText("Arrow keys to move", 110, 130, 20, gui_state.theme.txt_color);
        DrawText("PRESS R to regen floor", 110, 160, 20, gui_state.theme.txt_color);
        DrawText("PRESS P to pause animations", 110, 190, 20, gui_state.theme.txt_color);
        DrawText("PRESS Left_Alt + F to toggle fullscreen", 110, 220, 20, gui_state.theme.txt_color);
        DrawText("PRESS ENTER or TAP to go to FLOOR_VIEW SCREEN", 110, 280, 20, gui_state.theme.txt_color);

        char txt[30] = {0};
        char txt_b[30] = {0};
        char txt_s4c[50] = {0};
        int txt_StartX = gui_state.gameScreenWidth * 0.4f;
        int txt_StartY = gui_state.gameScreenHeight * 0.85f;
        DrawRectangle(txt_StartX, txt_StartY, gui_state.gameScreenWidth - txt_StartX, gui_state.gameScreenHeight - txt_StartY, YELLOW);
        sprintf(txt,"Koliseo API version: %i\n", int_koliseo_version());
        DrawText(txt, txt_StartX + ( txt_StartX * 0.16), txt_StartY, 20, gui_state.theme.txt_color);
        sprintf(txt_b,"Koliseo version: %s\n", string_koliseo_version());
        DrawText(txt_b, txt_StartX + ( txt_StartX * 0.16), txt_StartY + 20, 20, gui_state.theme.txt_color);
        sprintf(txt_s4c,"s4c-animate version: %s\n", S4C_ANIMATE_VERSION );
        DrawText(txt_s4c, txt_StartX + ( txt_StartX * 0.16), txt_StartY + 40, 20, gui_state.theme.txt_color);
    }
    break;
    case SAVES_VIEW : {
        // TODO: Draw SAVES_VIEW screen here!
        switch(load_info->is_new_game) {
        case -1: { // User has to pick new (1) or load (0)
            DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
            DrawText("PICK NEW/LOAD GAME SCREEN", 20, 20, 40, gui_state.theme.txt_color);
            DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
            Gui_Button_Group group = gui_state.gamepick_buttons;
            for (Gui_GamePick_Group_Button_Index i = 0; i < group.len; i++) {
                DrawRectangleRec(group.buttons[i].r, group.buttons[i].box_color);
                DrawText(group.buttons[i].label, group.buttons[i].r.x + (gui_state.gameScreenWidth * 0.02f), group.buttons[i].r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, group.buttons[i].text_color);
            }
            //DrawText("PRESS N for new game, L to load a game.", 110, 220, 20, DARKGREEN);
        }
        break;
        case 0: {
            if (saveslot_index == -1) {  // Pick saveslot
                DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
                DrawText("LOADED GAME SCREEN", 20, 20, 40, gui_state.theme.txt_color);
                DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
                //DrawText("PRESS 1-3 to pick a saveslot", 110, 220, 20, DARKGREEN);
                Gui_Button_Group group = gui_state.saveslotpick_buttons;
                for (Gui_SaveSlotPick_Group_Button_Index i = 0; i < group.len; i++) {
                    Gui_Button button = group.buttons[i];
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
                DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
                DrawText("NEW GAME SCREEN", 20, 20, 40, gui_state.theme.txt_color);
                DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
                Gui_Button_Group group = gui_state.saveslotpick_buttons;
                for (Gui_SaveSlotPick_Group_Button_Index i = 0; i < group.len; i++) {
                    Gui_Button button = group.buttons[i];
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

        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("PICK NAME SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));

        DrawRectangleRec(gui_state.txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].r, gui_state.txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD].box_color);

        Gui_Button namefield = gui_state.txtfield_buttons.buttons[BUTTON_NAME_TXTFIELD];

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
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("PICK CLASS SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
        Gui_Button_Group row = gui_state.classpick_buttons;
        for (Gui_ClassPick_Group_Button_Index i = 0; i < row.len; i++) {
            Gui_Button button = row.buttons[i];
            if (button.state == BUTTON_HOVER) {
                DrawRectangleRec(button.r, RED);

                int anim_res = -1;
                // Draw the current class animation frame somewhere
                Rectangle anim_r = {
                    .x = gui_state.gameScreenWidth*0.50f,
                    .y = gui_state.gameScreenHeight*0.45f,
                    .width = ((int)(gui_state.gameScreenWidth * 0.25f) / 17) * 17,
                    .height = gui_state.gameScreenWidth*0.25f,
                };
                switch (i) {
                case BUTTON_CLASS_KNIGHT: {
                    anim_res = DrawSpriteRect(knight_tapis[gui_state.framesCounter%60], anim_r, 17, 17, anim_r.width/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case BUTTON_CLASS_ARCHER: {
                    anim_res = DrawSpriteRect(archer_drop[gui_state.framesCounter%60], anim_r, 17, 17, anim_r.width/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case BUTTON_CLASS_MAGE: {
                    anim_res = DrawSpriteRect(mage_spark[gui_state.framesCounter%60], anim_r, 17, 17, anim_r.width/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case BUTTON_CLASS_ASSASSIN: {
                    anim_res = DrawSpriteRect(assassin_poof[gui_state.framesCounter%60], anim_r, 17, 17, anim_r.width/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                default: {
                    fprintf(stderr, "%s():    Unexpected button value: {%i}\n", __func__, i);
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                }
                break;
                }
                (void) anim_res; // TODO: check this return value
            } else {
                DrawRectangleRec(button.r, button.box_color);
            }
            DrawText(button.label, button.r.x + (gui_state.gameScreenWidth * 0.02f), button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, button.text_color);
        }
    }
    break;
    case FLOOR_VIEW: {
        // TODO: Draw FLOOR_VIEW screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("FLOOR_VIEW SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("Controls for FLOOR_VIEW screen", 110, 160, 20, gui_state.theme.txt_color);
        DrawText("Arrow keys to move", 110, 190, 20, gui_state.theme.txt_color);
        DrawText("PRESS R to regen floor", 110, 220, 20, gui_state.theme.txt_color);
#ifdef HELAPORDO_DEBUG_ACCESS
        if (G_DEBUG_ON == 1) {
            Gui_Button button = gui_state.debug_buttons.buttons[BUTTON_DEBUG];
            if (button.state == BUTTON_HOVER) {
                DrawRectangleRec(button.r, RED);
            } else {
                DrawRectangleRec(button.r, button.box_color);
            }
            DrawText(button.label, button.r.x + (gui_state.gameScreenWidth * 0.02f), button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, button.text_color);
        }
#endif // HELAPORDO_DEBUG_ACCESS

        Gui_Button_Group row = gui_state.floor_buttons;
        for (Gui_Floor_Group_Button_Index i = 0; i < row.len; i++) {
            Gui_Button button = row.buttons[i];
            if (button.state == BUTTON_HOVER) {
                DrawRectangleRec(button.r, RED);
            } else {
                DrawRectangleRec(button.r, button.box_color);
            }
            DrawText(button.label, button.r.x + (gui_state.gameScreenWidth * 0.02f), button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, button.text_color);
        }

        Rectangle floor_r = CLITERAL(Rectangle) {
            gui_state.gameScreenHeight *0.1f,
                                       gui_state.gameScreenWidth *0.1f,
                                       (FLOOR_MAX_COLS-1) * (gui_state.gameScreenWidth*0.02f),
                                       (FLOOR_MAX_ROWS-1) * (gui_state.gameScreenWidth*0.02f),
        };

        DrawRectangleRec(floor_r, ColorFromS4CPalette(palette, S4C_SALMON));

        if (G_EXPERIMENTAL_ON != 1) {
            if (current_floor != NULL) {
                draw_floor_view(current_floor, current_x, current_y, gui_state.gameScreenWidth*0.02f, &floor_r);
            } else {
                log_tag("debug_log.txt", "DEBUG", "%s():    current_floor was NULL.", __func__);
            }
        } else {
            if (current_floor != NULL) {
                display_roomclass_layout(current_floor, &floor_r, gui_state.gameScreenWidth*0.02f);
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
    }
    break;
    case ROOM_VIEW: {
        // TODO: Draw ROOM_VIEW screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("ROOM SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        if (current_room != NULL) {
            DrawText(TextFormat("Room Type: {%s}", stringFromRoom(current_room->class)), 20, 80, 20, gui_state.theme.txt_color);
            switch (current_room->class) {
            case ENEMIES: {
                Gui_Button_Group row = gui_state.fight_buttons;
                for (Gui_Fight_Group_Button_Index i = 0; i < row.len; i++) {
                    Gui_Button button = row.buttons[i];
                    if (button.state == BUTTON_HOVER) {
                        DrawRectangleRec(button.r, RED);
                    } else {
                        DrawRectangleRec(button.r, button.box_color);
                    }
                    DrawText(button.label, button.r.x + (gui_state.gameScreenWidth * 0.02f), button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, button.text_color);
                }
                int pl_rect_Y = gui_state.gameScreenHeight * 0.1f;
                int pl_frame_W = ((int)(gui_state.gameScreenWidth * 0.3f) / 17) * 17;
                int pl_frame_H = pl_frame_W;
                int pl_rect_X = gui_state.gameScreenWidth - pl_frame_W - (gui_state.gameScreenWidth * 0.1f);
                int en_rect_X = gui_state.gameScreenWidth * 0.1f;
                int en_rect_Y = pl_rect_Y;
                int en_frame_W = pl_frame_W;
                int en_frame_H = pl_frame_H;
                float stats_label_W = gui_state.gameScreenWidth * 0.18f;
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
                float pr_name_r_height = gui_state.gameScreenHeight * 0.05f;
                Rectangle pr_name_r = CLITERAL(Rectangle) {
                    pl_rect_X,
                    pl_rect_Y - pr_name_r_height,
                    pl_frame_W,
                    pr_name_r_height,
                };

                Rectangle en_r = CLITERAL(Rectangle) {
                    en_rect_X,
                    en_rect_Y,
                    en_frame_W,
                    en_frame_H
                };

                float en_name_r_height = gui_state.gameScreenHeight * 0.05f;
                Rectangle en_name_r = CLITERAL(Rectangle) {
                    en_rect_X,
                    en_rect_Y - en_name_r_height,
                    en_frame_W,
                    en_name_r_height,
                };

                Rectangle rb_r = CLITERAL(Rectangle) {
                    gui_state.gameScreenWidth*0.1f,
                                              gui_state.gameScreenHeight*0.65f,
                                              gui_state.gameScreenWidth*0.8f,
                                              gui_state.gameScreenHeight*0.3f,
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
                Enemy* enemy = current_room->enemies[gamestate->current_enemy_index];
                assert(enemy != NULL);
                int stats_height = stats_label_r.height * 0.135f;
                Color stats_txt_color = ColorFromS4CPalette(palette, S4C_BLACK);
                DrawText(TextFormat("%i  Hp  %i", enemy->hp, player->hp), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i  Hp  %i", enemy->hp, player->hp), stats_height)/2)), (int)stats_label_r.y + stats_height /2, stats_height, stats_txt_color);
                DrawText(TextFormat("%i  Atk  %i", enemy->atk, player->atk), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i  Atk  %i", enemy->atk, player->atk), stats_height)/2)), (int)stats_label_r.y + (stats_height*3/2), stats_height, stats_txt_color);
                DrawText(TextFormat("%i  Def  %i", enemy->def, player->def), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i  Def  %i", enemy->def, player->def), stats_height)/2)), (int)stats_label_r.y + (stats_height*5/2), stats_height, stats_txt_color);
                DrawText(TextFormat("%i  Vel  %i", enemy->vel, player->vel), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i  Vel  %i", enemy->vel, player->vel), stats_height)/2)), (int)stats_label_r.y + (stats_height*7/2), stats_height, stats_txt_color);
                DrawText(TextFormat("%i/%i Enr %i/%i", enemy->energy, enemy->totalenergy, player->energy, player->totalenergy), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i/%i Enr %i/%i", enemy->energy, enemy->totalenergy, player->energy, player->totalenergy), stats_height)/2)), (int)stats_label_r.y + (stats_height*9/2), stats_height, stats_txt_color);
                DrawText(TextFormat("%i  Lvl  %i", enemy->level, player->level), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i  Lvl  %i", enemy->level, player->level), stats_height)/2)), (int)stats_label_r.y + (stats_height*11/2), stats_height, stats_txt_color);
                DrawText(TextFormat("%i  Xp  %i/%i", enemy->xp, player->currentlevelxp, player->totallevelxp), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i  Xp  %i/%i", enemy->xp, player->currentlevelxp, player->totallevelxp), stats_height)/2)), (int)stats_label_r.y + (stats_height*13/2), stats_height, stats_txt_color);

                DrawRectangleRec(pr_name_r, ColorFromS4CPalette(palette, S4C_BLACK));
                int name_height = pr_name_r.height * 0.6f;
                Color name_color = ColorFromS4CPalette(palette, S4C_RED);
                DrawText(TextFormat("%s", player->name), (int)(pr_name_r.x + (pr_name_r.width/2) - (MeasureText(TextFormat("%s", player->name), name_height)/2)), (int)(pr_name_r.y + ((pr_name_r.height - name_height)/2)), name_height, name_color);
                DrawRectangleRec(en_name_r, ColorFromS4CPalette(palette, S4C_BLACK));
                DrawText(TextFormat("%s", stringFromEClass(enemy->class)), (int)(en_name_r.x + (en_name_r.width/2) - (MeasureText(TextFormat("%s", stringFromEClass(enemy->class)), name_height)/2)), (int)(en_name_r.y + ((en_name_r.height - name_height)/2)), name_height, name_color);
                //DrawText(TextFormat("%s    %s", stringFromEClass(enemy->class), player->name), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%s    %s", stringFromEClass(enemy->class), player->name), stats_height)/2)), (int)stats_label_r.y + 7*stats_height, stats_height, stats_txt_color);

                DrawRectangleRec(rb_r, BLACK);

                if (rb_isfull(*rb_notifications) || (rb_get_head(*rb_notifications) != 0)) {
                    hlpd_draw_notifications(rb_notifications, rb_r);
                }

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
            case BOSS: {
                Gui_Button_Group row = gui_state.fight_buttons;
                for (Gui_Fight_Group_Button_Index j = 0; j < row.len; j++) {
                    Gui_Button button = row.buttons[j];
                    if (button.state == BUTTON_HOVER) {
                        DrawRectangleRec(button.r, RED);
                    } else {
                        DrawRectangleRec(button.r, button.box_color);
                    }
                    DrawText(button.label, button.r.x + (gui_state.gameScreenWidth * 0.02f), button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, button.text_color);
                }
                int pl_rect_Y = gui_state.gameScreenHeight * 0.1f;
                int pl_frame_W = ((int)(gui_state.gameScreenWidth * 0.3f) / 17) * 17;
                int pl_frame_H = pl_frame_W;
                int pl_rect_X = gui_state.gameScreenWidth - pl_frame_W - (gui_state.gameScreenWidth * 0.1f);
                int bs_rect_X = gui_state.gameScreenWidth * 0.1f;
                int bs_rect_Y = pl_rect_Y;
                int bs_frame_W = pl_frame_W;
                int bs_frame_H = pl_frame_H;
                float stats_label_W = gui_state.gameScreenWidth * 0.18f;
                float stats_label_H = stats_label_W;
                Rectangle stats_label_r = CLITERAL(Rectangle) {
                    gui_state.gameScreenWidth*0.5f - (stats_label_W/2),
                                              bs_rect_Y,
                                              stats_label_W,
                                              stats_label_H
                };
                Rectangle pl_r = CLITERAL(Rectangle) {
                    pl_rect_X,
                    pl_rect_Y,
                    pl_frame_W,
                    pl_frame_H
                };
                float pr_name_r_height = gui_state.gameScreenHeight * 0.05f;
                Rectangle pr_name_r = CLITERAL(Rectangle) {
                    pl_rect_X,
                    pl_rect_Y - pr_name_r_height,
                    pl_frame_W,
                    pr_name_r_height,
                };

                Rectangle bs_r = CLITERAL(Rectangle) {
                    bs_rect_X,
                    bs_rect_Y,
                    bs_frame_W,
                    bs_frame_H
                };

                float bs_name_r_height = gui_state.gameScreenHeight * 0.05f;
                Rectangle bs_name_r = CLITERAL(Rectangle) {
                    bs_rect_X,
                    bs_rect_Y - bs_name_r_height,
                    bs_frame_W,
                    bs_name_r_height,
                };

                Rectangle rb_r = CLITERAL(Rectangle) {
                    gui_state.gameScreenWidth*0.1f,
                                              gui_state.gameScreenHeight*0.65f,
                                              gui_state.gameScreenWidth*0.8f,
                                              gui_state.gameScreenHeight*0.3f,
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
                Boss* boss = current_room->boss;
                assert(boss != NULL);
                int stats_height = stats_label_r.height * 0.135f;
                Color stats_txt_color = ColorFromS4CPalette(palette, S4C_BLACK);
                DrawText(TextFormat("%i  Hp  %i", boss->hp, player->hp), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i  Hp  %i", boss->hp, player->hp), stats_height)/2)), (int)stats_label_r.y + stats_height /2, stats_height, stats_txt_color);
                DrawText(TextFormat("%i  Atk  %i", boss->atk, player->atk), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i  Atk  %i", boss->atk, player->atk), stats_height)/2)), (int)stats_label_r.y + (stats_height*3/2), stats_height, stats_txt_color);
                DrawText(TextFormat("%i  Def  %i", boss->def, player->def), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i  Def  %i", boss->def, player->def), stats_height)/2)), (int)stats_label_r.y + (stats_height*5/2), stats_height, stats_txt_color);
                DrawText(TextFormat("%i  Vel  %i", boss->vel, player->vel), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i  Vel  %i", boss->vel, player->vel), stats_height)/2)), (int)stats_label_r.y + (stats_height*7/2), stats_height, stats_txt_color);
                DrawText(TextFormat("%i/%i Enr %i/%i", boss->energy, boss->totalenergy, player->energy, player->totalenergy), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i/%i Enr %i/%i", boss->energy, boss->totalenergy, player->energy, player->totalenergy), stats_height)/2)), (int)stats_label_r.y + (stats_height*9/2), stats_height, stats_txt_color);
                DrawText(TextFormat("%i  Lvl  %i", boss->level, player->level), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i  Lvl  %i", boss->level, player->level), stats_height)/2)), (int)stats_label_r.y + (stats_height*11/2), stats_height, stats_txt_color);
                DrawText(TextFormat("%i  Xp  %i/%i", boss->xp, player->currentlevelxp, player->totallevelxp), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%i  Xp  %i/%i", boss->xp, player->currentlevelxp, player->totallevelxp), stats_height)/2)), (int)stats_label_r.y + (stats_height*13/2), stats_height, stats_txt_color);

                DrawRectangleRec(pr_name_r, ColorFromS4CPalette(palette, S4C_BLACK));
                int name_height = pr_name_r.height * 0.6f;
                Color name_color = ColorFromS4CPalette(palette, S4C_RED);
                DrawText(TextFormat("%s", player->name), (int)(pr_name_r.x + (pr_name_r.width/2) - (MeasureText(TextFormat("%s", player->name), name_height)/2)), (int)(pr_name_r.y + ((pr_name_r.height - name_height)/2)), name_height, name_color);
                DrawRectangleRec(bs_name_r, ColorFromS4CPalette(palette, S4C_BLACK));
                DrawText(TextFormat("%s", stringFromBossClass(boss->class)), (int)(bs_name_r.x + (bs_name_r.width/2) - (MeasureText(TextFormat("%s", stringFromBossClass(boss->class)), name_height)/2)), (int)(bs_name_r.y + ((bs_name_r.height - name_height)/2)), name_height, name_color);
                //DrawText(TextFormat("%s    %s", stringFromBossClass(boss->class), player->name), (int)(stats_label_r.x + (stats_label_r.width/2) - (MeasureText(TextFormat("%s    %s", stringFromBossClass(boss->class), player->name), stats_height)/2)), (int)stats_label_r.y + 7*stats_height, stats_height, stats_txt_color);

                DrawRectangleRec(rb_r, DARKGRAY);

                if (rb_isfull(*rb_notifications) || (rb_get_head(*rb_notifications) != 0)) {
                    hlpd_draw_notifications(rb_notifications, rb_r);
                }

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
                int bs_res = -1;
                bossClass boss_class = current_room->boss->class;
                switch (boss_class) {
                case Blue_Troll: {
                    bs_res = DrawSpriteRect(bluetroll_wonder[current_anim_frame], bs_r, 17, 17, bs_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Headless_Ninja: {
                    bs_res = DrawSpriteRect(headlessninja_throw[current_anim_frame], bs_r, 17, 17, bs_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Crawling_Dude: {
                    bs_res = DrawSpriteRect(crawlingdude_crawl[current_anim_frame], bs_r, 17, 17, bs_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Sr_Warthog: {
                    bs_res = DrawSpriteRect(srwarthog_square[current_anim_frame], bs_r, 17, 17, bs_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case Doppelganger: {
                    switch (player_class) {
                    case Knight: {
                        bs_res = DrawSpriteRect(knight_tapis[current_anim_frame], bs_r, 17, 17, bs_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                    }
                    break;
                    case Archer: {
                        bs_res = DrawSpriteRect(knight_tapis[current_anim_frame], bs_r, 17, 17, bs_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                    }
                    break;
                    case Mage: {
                        bs_res = DrawSpriteRect(knight_tapis[current_anim_frame], bs_r, 17, 17, bs_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
                    }
                    break;
                    case Assassin: {
                        bs_res = DrawSpriteRect(knight_tapis[current_anim_frame], bs_r, 17, 17, bs_frame_W/17, palette, PALETTE_S4C_H_TOTCOLORS);
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
                }
                break;
                default: {
                    log_tag("debug_log.txt", "ERROR", "%s():    Unexpected boss_class: {%i}", __func__, boss_class);
                    fprintf(stderr, "[ERROR] [%s()]    Unexpected boss_class: {%i}\n", __func__, boss_class);
                    kls_free(default_kls);
                    kls_free(temporary_kls);
                    exit(EXIT_FAILURE);
                }
                break;
                }
                if (pl_res != 0 || bs_res != 0 || CheckCollisionRecs(bs_r,stats_label_r) || CheckCollisionRecs(stats_label_r,pl_r) || CheckCollisionRecs(bs_r,pl_r)) {
                    DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, ColorFromS4CPalette(palette, S4C_RED));
                    DrawText("Window too small.", 20, 20, 20, RAYWHITE);
                    DrawText("Please resize.", 20, 50, 20, RAYWHITE);
                }
            }
            break;
            case TREASURE: {
                Gui_Button_Group group = gui_state.treasure_buttons;
                for (Gui_Treasure_Group_Button_Index i = 0; i < group.len; i++) {
                    Gui_Button button = group.buttons[i];
                    if (button.state == BUTTON_HOVER) {
                        DrawRectangleRec(button.r, RED);
                    } else {
                        DrawRectangleRec(button.r, button.box_color);
                    }
                    DrawText(button.label, button.r.x + (gui_state.gameScreenWidth * 0.02f), button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, button.text_color);
                }

                int tr_r_w = -1;
                int tr_r_h = -1;
                switch (current_room->treasure->class) {
                case TREASURE_CHEST: {
                    tr_r_w = ((int)(gui_state.gameScreenWidth * 0.3f)/ (CHEST_COLS-1)) * (CHEST_COLS-1);
                    tr_r_h = tr_r_w;
                }
                break;
                case TREASURE_CONSUMABLE:
                case TREASURE_ARTIFACT: {
                    tr_r_w = ((int)(gui_state.gameScreenWidth * 0.3f) / 12) * 12;
                    tr_r_h = ((int)(gui_state.gameScreenHeight * 0.3f) / 8) * 8;
                }
                break;
                }
                Rectangle tr_r = {
                    .x = ((gui_state.gameScreenWidth/4)*3 - tr_r_w/2),
                    .y = (gui_state.gameScreenHeight / 2  - tr_r_h /2),
                    .width = tr_r_w,
                    .height = tr_r_h,
                };
                int tr_res = -1;
                switch (current_room->treasure->class) {
                case TREASURE_CHEST: {
                    int frame_height = CHEST_ROWS-1;
                    int frame_width = CHEST_COLS-1;

                    tr_res = DrawSpriteRect(alt_chest_opening[0], tr_r, frame_height, frame_width, tr_r.width/frame_width, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case TREASURE_CONSUMABLE: {
                    tr_res = DrawSpriteRect(consumables_sprites_proper[current_room->treasure->consumable->class], tr_r, 8, 12, tr_r.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                case TREASURE_ARTIFACT: {
                    tr_res = DrawSpriteRect(artifacts_sprites_proper[current_room->treasure->artifact->class], tr_r, 8, 12, tr_r.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                break;
                }
                if (tr_res != 0) {
                    DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, ColorFromS4CPalette(palette, S4C_RED));
                    DrawText("Window too small.", 20, 20, 20, RAYWHITE);
                    DrawText("Please resize.", 20, 50, 20, RAYWHITE);
                }
            }
            break;
            case SHOP: {
                Shop* shop = current_room->shop;
                DrawText(TextFormat("Balance: $ %i", player->balance), 20, 120, 20, gui_state.theme.txt_color);

                int eq_res = -1;
                int cs_res = -1;
                for (Gui_Shop_Layout_Group_Index i=0; i < gui_state.shop_buttons.len; i++) {
                    Gui_Button_Group* group = gui_state.shop_buttons.groups[i];
                    for (int j=0; j < group->len; j++) {
                        Gui_Button button = group->buttons[j];

                        if (button.state == BUTTON_HOVER) {
                            DrawRectangleRec(button.r, RED);
                        } else {
                            DrawRectangleRec(button.r, button.box_color);
                        }
                        DrawRectangleLines(button.r.x, button.r.y, button.r.width, button.r.height, BLACK);
                        int details_r_width = button.r.width;
                        int details_r_x = button.r.x + button.r.width/2 - details_r_width/2;
                        int details_r_y = button.r.y + button.r.height;
                        Rectangle details_r = {
                            .x = details_r_x,
                            .y = details_r_y,
                            .width = details_r_width,
                            .height = (i == SHOP_LAYOUT_EQUIPS_GROUP ? button.r.height*2 : button.r.height),
                        };
                        int txt_height = (((int)details_r.height * (i == SHOP_LAYOUT_EQUIPS_GROUP ? 0.1f : 0.2f)) / 20) * 20;
                        Color txt_color = BLACK;
                        switch (i) {
                        case SHOP_LAYOUT_EQUIPS_GROUP: {
                            if (j >= shop->equipsCount) {
                                continue;
                            }
                            Equip* equip = shop->equips[j];
                            int y_pos = details_r.y + txt_height;
                            DrawText(TextFormat("$ %i", shop->equipPrices[j]), details_r.x + details_r.width/2 - MeasureText(TextFormat("$ %i", shop->equipPrices[j]), txt_height)/2, y_pos, txt_height, txt_color);
                            y_pos += txt_height;
                            DrawText(TextFormat("%s Lv. %i", stringFromEquips(equip->class), equip->level), details_r.x + details_r.width/2 - MeasureText(TextFormat("%s Lv. %i", stringFromEquips(equip->class), equip->level), txt_height)/2, y_pos, txt_height, txt_color);
                            y_pos += txt_height;
                            {
                                Color qual_txt_color = {0};
                                switch (equip->qual) {
                                case Bad: {
                                    qual_txt_color = ColorFromS4CPalette(palette, S4C_RED);
                                }
                                break;
                                case Average: {
                                    qual_txt_color = ColorFromS4CPalette(palette, S4C_BRIGHT_YELLOW);
                                }
                                break;
                                case Good: {
                                    qual_txt_color = ColorFromS4CPalette(palette, S4C_CYAN);
                                }
                                break;
                                }
                                DrawText(TextFormat("Qual: %s", stringFromQuality(equip->qual)), details_r.x + details_r.width/2 - MeasureText(TextFormat("Qual: %s", stringFromQuality(equip->qual)), txt_height)/2, y_pos, txt_height, qual_txt_color);
                            }
                            y_pos += txt_height;
                            DrawText(TextFormat("Atk: %i", equip->atk), details_r.x + details_r.width/2 - MeasureText(TextFormat("Atk: %i", equip->atk), txt_height)/2, y_pos, txt_height, txt_color);
                            y_pos += txt_height;
                            DrawText(TextFormat("Def: %i", equip->def), details_r.x + details_r.width/2 - MeasureText(TextFormat("Def: %i", equip->def), txt_height)/2, y_pos, txt_height, txt_color);
                            y_pos += txt_height;
                            DrawText(TextFormat("Vel: %i", equip->vel), details_r.x + details_r.width/2 - MeasureText(TextFormat("Vel: %i", equip->vel), txt_height)/2, y_pos, txt_height, txt_color);
                            y_pos += txt_height;
                            DrawText(TextFormat("Perks: %i", equip->perksCount), details_r.x + details_r.width/2 - MeasureText(TextFormat("Perks: %i", equip->perksCount), txt_height)/2, y_pos, txt_height, txt_color);
                            for (int perk_idx = 0; perk_idx < equip->perksCount; perk_idx++) {
                                y_pos += txt_height;
                                DrawText(TextFormat("%s", nameStringFromPerk(equip->perks[perk_idx]->class)), details_r.x + details_r.width/2 - MeasureText(TextFormat("%s", nameStringFromPerk(equip->perks[perk_idx]->class)), txt_height)/2, y_pos, txt_height, txt_color);
                            }
                            eq_res = DrawSpriteRect(equips_sprites_proper[equip->class], button.r, 8, 12, button.r.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
                        }
                        break;
                        case SHOP_LAYOUT_CONSUMABLES_GROUP: {
                            if (j >= shop->consumablesCount) {
                                continue;
                            }
                            Consumable* consumable = shop->consumables[j];
                            int y_pos = details_r.y + txt_height;
                            DrawText(TextFormat("$ %i", shop->consumablePrices[j]), details_r.x + details_r.width/2 - MeasureText(TextFormat("$ %i", shop->consumablePrices[j]), txt_height)/2, y_pos, txt_height, txt_color);
                            y_pos += txt_height;
                            DrawText(TextFormat("%s", stringFromConsumables(consumable->class)), details_r.x + details_r.width/2 - MeasureText(TextFormat("%s", stringFromConsumables(consumable->class)), txt_height)/2, y_pos, txt_height, txt_color);
                            y_pos += txt_height;
                            DrawText(TextFormat("x%i", consumable->qty), details_r.x + details_r.width/2 - MeasureText(TextFormat("x%i", consumable->qty), txt_height)/2, y_pos, txt_height, txt_color);
                            cs_res = DrawSpriteRect(consumables_sprites_proper[consumable->class], button.r, 8, 12, button.r.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
                        }
                        break;
                        case SHOP_LAYOUT_OTHERS_GROUP: {
                            DrawText(button.label, button.r.x + (gui_state.gameScreenWidth * 0.02f), button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, button.text_color);
                        }
                        break;
                        default: {
                            log_tag("debug_log.txt", "ERROR", "%s():    Unexpected group for shop_buttons: {%i}", __func__, i);
                            kls_free(default_kls);
                            kls_free(temporary_kls);
                            exit(EXIT_FAILURE);
                        }
                        }
                    }
                }

                if (eq_res != 0 || cs_res != 0) {
                    DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, ColorFromS4CPalette(palette, S4C_RED));
                    DrawText("Window too small.", 20, 20, 20, RAYWHITE);
                    DrawText("Please resize.", 20, 50, 20, RAYWHITE);
                }
            }
            break;
            case HOME: {
                DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
                DrawText("HOME_VIEW SCREEN", 20, 20, 40, gui_state.theme.txt_color);
                DrawText("WIP", 20, gui_state.gameScreenHeight*0.5f, 40, ColorFromS4CPalette(palette, S4C_SALMON));
                DrawText("PRESS Q to go back exploring", 110, 160, 20, gui_state.theme.txt_color);
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
        //DrawText("PRESS ENTER or TAP to go to FLOOR_VIEW SCREEN", 110, 220, 20, gui_state.theme.txt_color);
        //DrawText("PRESS P to pause animations", 110, 350, 20, gui_state.theme.txt_color);
        //DrawText("PRESS Left_Alt + F to toggle fullscreen", 110, 390, 20, gui_state.theme.txt_color);

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
    case PICK_SPECIAL_VIEW: {
        // TODO: Draw UNLOCK_SPECIAL_VIEW screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("PICK SPECIAL SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        Gui_Button_Group group = gui_state.special_buttons;
        for (Gui_Special_Group_Button_Index i = 0; i < group.len; i++) {
            if ((player->specials[i]->enabled)) {
                Gui_Button button = group.buttons[i];
                if (button.state == BUTTON_HOVER) {
                    DrawRectangleRec(button.r, RED);
                    DrawText(nameStringFromSpecial(player->class, i), gui_state.gameScreenWidth * 0.5f, gui_state.gameScreenHeight * 0.3f, gui_state.gameScreenHeight * 0.04f, RED);
                    DrawText(descStringFromSpecial(player->class, i), gui_state.gameScreenWidth * 0.5f, gui_state.gameScreenHeight * 0.4f, gui_state.gameScreenHeight * 0.04f, RED);
                } else {
                    DrawRectangleRec(button.r, button.box_color);
                }
                DrawText(button.label, button.r.x + (gui_state.gameScreenWidth * 0.02f), button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, button.text_color);
            }
        }
    }
    break;
    case UNLOCK_SPECIAL_VIEW: {
        // TODO: Draw UNLOCK_SPECIAL_VIEW screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("UNLOCK SPECIAL SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        Gui_Button_Group group = gui_state.special_buttons;
        for (Gui_Special_Group_Button_Index i = 0; i < group.len; i++) {
            if (!(player->specials[i]->enabled)) {
                Gui_Button button = group.buttons[i];
                if (button.state == BUTTON_HOVER) {
                    DrawRectangleRec(button.r, RED);
                    DrawText(nameStringFromSpecial(player->class, i), gui_state.gameScreenWidth * 0.5f, gui_state.gameScreenHeight * 0.3f, gui_state.gameScreenHeight * 0.04f, RED);
                    DrawText(descStringFromSpecial(player->class, i), gui_state.gameScreenWidth * 0.5f, gui_state.gameScreenHeight * 0.4f, gui_state.gameScreenHeight * 0.04f, RED);
                } else {
                    DrawRectangleRec(button.r, button.box_color);
                }
                DrawText(button.label, button.r.x + (gui_state.gameScreenWidth * 0.02f), button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, button.text_color);
            }
        }
    }
    break;
    case EQUIPS_VIEW: {
        // TODO: Draw EQUIPS_VIEW screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("EQUIPS SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS Q to go back", gui_state.gameScreenWidth*0.1f, gui_state.gameScreenHeight*0.9f, 20, gui_state.theme.txt_color);
        Gui_Button_Group group = gui_state.equips_buttons;
        for (Gui_Equips_Group_Button_Index i = 0; i < group.len; i++) {
            Gui_Button button = group.buttons[i];
            if (button.state == BUTTON_HOVER) {
                DrawRectangleRec(button.r, RED);
                //DrawText(nameStringFromSpecial(player->class, i - BUTTON_SPECIAL_UNLOCK_1), gui_state.gameScreenWidth * 0.5f, gui_state.gameScreenHeight * 0.3f, gui_state.gameScreenHeight * 0.04f, RED);
                //DrawText(descStringFromSpecial(player->class, i - BUTTON_SPECIAL_UNLOCK_1), gui_state.gameScreenWidth * 0.5f, gui_state.gameScreenHeight * 0.4f, gui_state.gameScreenHeight * 0.04f, RED);
            } else {
                DrawRectangleRec(button.r, button.box_color);
            }
            DrawText(button.label, button.r.x + (gui_state.gameScreenWidth * 0.02f), button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, button.text_color);
        }
        float equips_box_x = gui_state.gameScreenWidth*0.3f;
        float equips_box_y = 60;
        float equips_box_width = (gui_state.gameScreenWidth/2)*1.25f;
        float equips_box_height = (gui_state.gameScreenHeight/2)*1.5f;
        //DrawRectangleLines(equips_box_x, equips_box_y, equips_box_width, equips_box_height, RED);
        Rectangle head_label_box = {
            .x = equips_box_x,
            .y = equips_box_y,
            .width = (((int)equips_box_width * 0.3)/12)*12,
            .height = (((int)equips_box_height * 0.25)/8)*8,
        };
        Rectangle torso_label_box = {
            .x = head_label_box.x + head_label_box.width + 0.05*equips_box_width,
            .y = head_label_box.y,
            .width = head_label_box.width,
            .height = head_label_box.height,
        };
        Rectangle legs_label_box = {
            .x = torso_label_box.x + torso_label_box.width + 0.05*equips_box_width,
            .y = torso_label_box.y,
            .width = torso_label_box.width,
            .height = torso_label_box.height
        };
        Rectangle head_box = {
            .x = equips_box_x,
            .y = head_label_box.y + head_label_box.height + 0.05*equips_box_height,
            .width = head_label_box.width,
            .height = head_label_box.height,
        };
        Rectangle torso_box = {
            .x = head_box.x + head_box.width + 0.05*equips_box_width,
            .y = head_box.y,
            .width = head_box.width,
            .height = head_box.height,
        };
        Rectangle legs_box = {
            .x = torso_box.x + torso_box.width + 0.05*equips_box_width,
            .y = torso_box.y,
            .width = torso_box.width,
            .height = torso_box.height
        };
        Rectangle head_details_box = {
            .x = equips_box_x,
            .y = head_box.y + head_box.height + 0.05*equips_box_height,
            .width = head_box.width,
            .height = equips_box_y + equips_box_height - (head_box.y + head_box.height + 0.05*equips_box_height),
        };
        Rectangle torso_details_box = {
            .x = head_details_box.x + head_details_box.width + 0.05*equips_box_width,
            .y = head_details_box.y,
            .width = head_details_box.width,
            .height = head_details_box.height,
        };
        Rectangle legs_details_box = {
            .x = torso_details_box.x + torso_details_box.width + 0.05*equips_box_width,
            .y = torso_details_box.y,
            .width = torso_details_box.width,
            .height = torso_details_box.height
        };
        //DrawRectangleLines(equips_box_x, equips_box_y, equips_box_width, equips_box_height, RED);
        //DrawRectangleLines(head_label_box.x, head_label_box.y, head_label_box.width, head_label_box.height, BLUE);
        //DrawRectangleLines(torso_label_box.x, torso_label_box.y, torso_label_box.width, torso_label_box.height, RED);
        //DrawRectangleLines(legs_label_box.x, legs_label_box.y, legs_label_box.width, legs_label_box.height, RED);
        //DrawRectangleLines(head_box.x, head_box.y, head_box.width, head_box.height, GREEN);
        //DrawRectangleLines(torso_box.x, torso_box.y, torso_box.width, torso_box.height, GREEN);
        //DrawRectangleLines(legs_box.x, legs_box.y, legs_box.width, legs_box.height, GREEN);
        //DrawRectangleLines(head_details_box.x, head_details_box.y, head_details_box.width, head_details_box.height, ORANGE);
        //DrawRectangleLines(torso_details_box.x, torso_details_box.y, torso_details_box.width, torso_details_box.height, ORANGE);
        //DrawRectangleLines(legs_details_box.x, legs_details_box.y, legs_details_box.width, legs_details_box.height, ORANGE);
        DrawSpriteRect(equipzones_sprites_proper[HEAD], head_label_box, 8, 12, head_label_box.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
        DrawSpriteRect(equipzones_sprites_proper[TORSO], torso_label_box, 8, 12, torso_label_box.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
        DrawSpriteRect(equipzones_sprites_proper[LEGS], legs_label_box, 8, 12, legs_label_box.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
        for (Equipzone i=0; i<EQUIPZONES+1; i++) {
            if (player->equipslots[i]->active) {
                int txt_height = 20;
                switch (i) {
                case HEAD: {
                    DrawSpriteRect(equips_sprites_proper[player->equipslots[i]->item->class], head_box, 8, 12, head_box.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
                    const char* txt = TextFormat("%s", stringFromEquips(player->equipslots[i]->item->class), txt_height);
                    DrawText(txt, head_details_box.x + (head_details_box.width - MeasureText(txt, txt_height))/2, head_details_box.y, txt_height, gui_state.theme.txt_color);
                    txt = TextFormat("%s", player->equipslots[i]->item->desc, txt_height);
                    DrawText(txt, head_details_box.x + (head_details_box.width - MeasureText(txt, txt_height))/2, head_details_box.y + txt_height, txt_height, gui_state.theme.txt_color);
                }
                break;
                case TORSO: {
                    DrawSpriteRect(equips_sprites_proper[player->equipslots[i]->item->class], torso_box, 8, 12, torso_box.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
                    const char* txt = TextFormat("%s", stringFromEquips(player->equipslots[i]->item->class), txt_height);
                    DrawText(txt, torso_details_box.x + (torso_details_box.width - MeasureText(txt, txt_height))/2, torso_details_box.y, txt_height, gui_state.theme.txt_color);
                    txt = TextFormat("%s", player->equipslots[i]->item->desc, txt_height);
                    DrawText(txt, torso_details_box.x + (torso_details_box.width - MeasureText(txt, txt_height))/2, torso_details_box.y + txt_height, txt_height, gui_state.theme.txt_color);
                }
                break;
                case LEGS: {
                    DrawSpriteRect(equips_sprites_proper[player->equipslots[i]->item->class], legs_box, 8, 12, legs_box.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
                    const char* txt = TextFormat("%s", stringFromEquips(player->equipslots[i]->item->class), txt_height);
                    DrawText(txt, legs_details_box.x + (legs_details_box.width - MeasureText(txt, txt_height))/2, legs_details_box.y, txt_height, gui_state.theme.txt_color);
                    txt = TextFormat("%s", player->equipslots[i]->item->desc, txt_height);
                    DrawText(txt, legs_details_box.x + (legs_details_box.width - MeasureText(txt, txt_height))/2, legs_details_box.y + txt_height, txt_height, gui_state.theme.txt_color);
                }
                break;
                }
            } else {
                int txt_height = 20;
                switch (i) {
                case HEAD: {
                    DrawText("Empty", head_box.x + (head_box.width - MeasureText("Empty", txt_height))/2, head_box.y + (head_box.height - txt_height)/2, txt_height, gui_state.theme.txt_color);
                }
                break;
                case TORSO: {
                    DrawText("Empty", torso_box.x + (torso_box.width - MeasureText("Empty", txt_height))/2, torso_box.y + (torso_box.height - txt_height)/2, txt_height, gui_state.theme.txt_color);
                }
                break;
                case LEGS: {
                    DrawText("Empty", legs_box.x + (legs_box.width - MeasureText("Empty", txt_height))/2, legs_box.y + (legs_box.height - txt_height)/2, txt_height, gui_state.theme.txt_color);
                }
                break;
                }
            }
        }
    }
    break;
    case OPEN_BAG_VIEW: {
        // TODO: Draw OPEN_BAG_VIEW screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("BAG SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS Q to go back", gui_state.gameScreenWidth*0.1f, gui_state.gameScreenHeight*0.9f, 20, gui_state.theme.txt_color);
        Rectangle textbox_bounds = (Rectangle) {
            .x = 20,
            .y = 80,
            .width = gui_state.gameScreenWidth*0.3f,
            .height = gui_state.gameScreenHeight/2
        };
        Rectangle spritebox_bounds = (Rectangle) {
            .x = gui_state.gameScreenWidth*0.5f,
            .y = 80,
            .width = gui_state.gameScreenWidth*0.3f,
            .height = ((int)(gui_state.gameScreenHeight * 0.3f) /8) * 8,
        };
        int details_x = spritebox_bounds.x;
        int details_y = spritebox_bounds.y + spritebox_bounds.height;
        int indicator_w = 15;
        int indicator_h = 15;
        //DrawRectangleLines(textbox_bounds.x, textbox_bounds.y, textbox_bounds.width, textbox_bounds.height, ColorFromS4CPalette(palette, S4C_LIGHT_YELLOW));
        //DrawRectangleLines(spritebox_bounds.x, spritebox_bounds.y, spritebox_bounds.width, spritebox_bounds.height, ColorFromS4CPalette(palette, S4C_LIGHT_YELLOW));
        int selected_index = gui_state.selectedIndex;
        for (int i=0; i < player->equipsBagOccupiedSlots; i++) {
            Equip* e = player->equipsBag[i];
            Color color = gui_state.theme.txt_color;
            if (e) {
                if (i == selected_index) color = RED;
                DrawText(TextFormat("%s", stringFromEquips(e->class)), textbox_bounds.x + 20, textbox_bounds.y + 20*i, 20, color);
            }
            if (i == selected_index) {
                DrawRectangle(textbox_bounds.x, textbox_bounds.y + 20*i, indicator_w, indicator_h, gui_state.theme.txt_color);
                DrawSpriteRect(equips_sprites_proper[e->class], spritebox_bounds, 8, 12, spritebox_bounds.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
                int txt_height = 20;
                DrawText(TextFormat("%s", stringFromEquips(e->class)), details_x, details_y, txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Atk: %i", e->atk), details_x, details_y + txt_height, txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Def: %i", e->def), details_x, details_y + (2*txt_height), txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Vel: %i", e->vel), details_x, details_y + (3*txt_height), txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Enr: %i", e->enr), details_x, details_y + (4*txt_height), txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Qual: %s", stringFromQuality(e->qual)), details_x, details_y + (5*txt_height), txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Lvl: %i", e->level), details_x, details_y + (6*txt_height), txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Perks: %i", e->perksCount), details_x, details_y + (7*txt_height), txt_height, gui_state.theme.txt_color);
            }
        }
    }
    break;
    case CHECK_LOADOUT_VIEW: {
        // TODO: Draw CHECK_LOADOUT_VIEW screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("LOADOUT SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS Q to go back", gui_state.gameScreenWidth*0.1f, gui_state.gameScreenHeight*0.9f, 20, gui_state.theme.txt_color);
        int x_spacing = 20;
        int y_spacing = 70;
        Rectangle spritebox_bounds = (Rectangle) {
            .x = x_spacing,
            .y = y_spacing,
            .width = gui_state.gameScreenWidth - x_spacing*2,
            .height = gui_state.gameScreenHeight - y_spacing*2
        };
        Rectangle headbox_bounds = (Rectangle) {
            .x = x_spacing,
            .y = y_spacing,
            .width = spritebox_bounds.width/3 - x_spacing,
            .height = spritebox_bounds.height,
        };
        Rectangle torsobox_bounds = (Rectangle) {
            .x = headbox_bounds.x + headbox_bounds.width + x_spacing,
            .y = y_spacing,
            .width = headbox_bounds.width,
            .height = headbox_bounds.height,
        };
        Rectangle legsbox_bounds = (Rectangle) {
            .x = torsobox_bounds.x + torsobox_bounds.width + x_spacing,
            .y = y_spacing,
            .width = torsobox_bounds.width,
            .height = torsobox_bounds.height,
        };
        //DrawRectangleLines(spritebox_bounds.x, spritebox_bounds.y, spritebox_bounds.width, spritebox_bounds.height, ColorFromS4CPalette(palette, S4C_LIGHT_YELLOW));
        //DrawRectangleLines(headbox_bounds.x, headbox_bounds.y, headbox_bounds.width, headbox_bounds.height, ColorFromS4CPalette(palette, S4C_LIGHT_BLUE));
        //DrawRectangleLines(torsobox_bounds.x, torsobox_bounds.y, torsobox_bounds.width, torsobox_bounds.height, ColorFromS4CPalette(palette, S4C_LIGHT_BLUE));
        //DrawRectangleLines(legsbox_bounds.x, legsbox_bounds.y, legsbox_bounds.width, legsbox_bounds.height, ColorFromS4CPalette(palette, S4C_LIGHT_BLUE));
        for (int i=0; i < 3; i++) {
            Equipslot* slot = player->equipslots[i];
            if (slot->active) {
                Equip* e = slot->item;
                Rectangle slot_r = {0};
                switch(i) {
                case HEAD: {
                    slot_r = headbox_bounds;
                }
                break;
                case TORSO: {
                    slot_r = torsobox_bounds;
                }
                break;
                case LEGS: {
                    slot_r = legsbox_bounds;
                }
                break;
                }
                DrawSpriteRect(equips_sprites_proper[e->class], slot_r, 8, 12, slot_r.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
                int text_start_y = slot_r.width/12*10;
                int txt_height = 20;
                DrawText(TextFormat("%s", stringFromEquips(e->class)), slot_r.x, slot_r.y + text_start_y, txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Atk: %i", e->atk), slot_r.x, slot_r.y + text_start_y + txt_height, txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Def: %i", e->def), slot_r.x, slot_r.y + text_start_y + (2*txt_height), txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Vel: %i", e->vel), slot_r.x, slot_r.y + text_start_y + (3*txt_height), txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Enr: %i", e->enr), slot_r.x, slot_r.y + text_start_y + (4*txt_height), txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Qual: %s", stringFromQuality(e->qual)), slot_r.x, slot_r.y + text_start_y + (5*txt_height), txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Lvl: %i", e->level), slot_r.x, slot_r.y + text_start_y + (6*txt_height), txt_height, gui_state.theme.txt_color);
                DrawText(TextFormat("Perks: %i", e->perksCount), slot_r.x, slot_r.y + text_start_y + (7*txt_height), txt_height, gui_state.theme.txt_color);
                for (int perk_idx = 0; perk_idx < e->perksCount; perk_idx++) {
                    Perk* perk = e->perks[perk_idx];
                    if (perk) {
                        int perk_txt_height = 10;
                        DrawText(TextFormat("%s", nameStringFromPerk(perk->class)), slot_r.x, slot_r.y + text_start_y + (8*txt_height) + (2*perk_idx*txt_height), perk_txt_height, RED);
                        DrawText(TextFormat("%s", descStringFromPerk(perk->class)), slot_r.x, slot_r.y + text_start_y + (9*txt_height) + (2*perk_idx*txt_height), perk_txt_height, gui_state.theme.txt_color);
                        //} else {
                        //DrawText("NULL", slot_r.x, slot_r.y + text_start_y + (8*txt_height) + (2*perk_idx*txt_height), txt_height, gui_state.theme.txt_color);
                        //DrawText("NULL desc", slot_r.x, slot_r.y + text_start_y + (9*txt_height) + (2*perk_idx*txt_height), txt_height, gui_state.theme.txt_color);
                    }
                }
            }
        }
    }
    break;
    case CONSUMABLES_VIEW: {
        // TODO: Draw CONSUMABLES_VIEW screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("CONSUMABLES SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS Q to go back", gui_state.gameScreenWidth*0.1f, gui_state.gameScreenHeight*0.9f, 20, gui_state.theme.txt_color);
        Rectangle textbox_bounds = (Rectangle) {
            .x = 20,
            .y = 120,
            .width = gui_state.gameScreenWidth/2,
            .height = gui_state.gameScreenHeight/2
        };
        Rectangle descbox_bounds = (Rectangle) {
            .x = textbox_bounds.x,
            .y = textbox_bounds.y + textbox_bounds.height,
            .width = textbox_bounds.width,
            .height = gui_state.gameScreenHeight/4,
        };
        Rectangle spritebox_bounds = (Rectangle) {
            .x = textbox_bounds.x + textbox_bounds.width + 20,
            .y = 120,
            .width = gui_state.gameScreenWidth - textbox_bounds.width - textbox_bounds.x - 40,
            .height = gui_state.gameScreenHeight/2
        };
        int indicator_w = 15;
        int indicator_h = 15;
        //DrawRectangleLines(textbox_bounds.x, textbox_bounds.y, textbox_bounds.width, textbox_bounds.height, ColorFromS4CPalette(palette, S4C_LIGHT_YELLOW));
        //DrawRectangleLines(descbox_bounds.x, descbox_bounds.y, descbox_bounds.width, descbox_bounds.height, ColorFromS4CPalette(palette, S4C_LIGHT_YELLOW));
        //DrawRectangleLines(spritebox_bounds.x, spritebox_bounds.y, spritebox_bounds.width, spritebox_bounds.height, ColorFromS4CPalette(palette, S4C_LIGHT_YELLOW));
        int selected_index = gui_state.selectedIndex;
        for (int i=0; i < CONSUMABLESMAX+1; i++) {
            Consumable* c = player->consumablesBag[i];
            Color color = gui_state.theme.txt_color;
            if (i == selected_index) color = RED;
            DrawText(TextFormat("%s    x%i", stringFromConsumables(c->class), c->qty), textbox_bounds.x + 20, textbox_bounds.y + 20 * i, 20, color);
            if (i == selected_index) {
                DrawRectangle(textbox_bounds.x, textbox_bounds.y + 20*i, indicator_w, indicator_h, gui_state.theme.txt_color);
                DrawSpriteRect(consumables_sprites_proper[c->class], spritebox_bounds, 8, 12, spritebox_bounds.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
                const char* txt = "???";
                if (c->qty > 0) {
                    txt = c->desc;
                }
                int txt_height = 20;
                DrawText(txt, descbox_bounds.x, descbox_bounds.y, txt_height, gui_state.theme.txt_color);
            }
        }
    }
    break;
    case STATS_VIEW: {
        // TODO: Draw STATS_VIEW screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("STATS_VIEW SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS Q to go back", gui_state.gameScreenWidth*0.6f, gui_state.gameScreenHeight*0.8f, 20, gui_state.theme.txt_color);
        int txt_height = 20;
        int start_y = 80;
        int y = start_y;
        const char* txt = NULL;

        int fighter_label_x = (gui_state.gameScreenWidth/2)*0.1f;
        int fighter_value_x = (gui_state.gameScreenWidth/2)*0.6f;
        DrawText("Name:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%s", player->name);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Class:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%s", stringFromClass(player->class));
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Level:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->level);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Hp:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i/%i", player->hp, player->totalhp);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Atk:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i (+%i)", player->atk, player->equipboost_atk);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Def:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i (+%i)", player->def, player->equipboost_def);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Vel:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i (+%i)", player->vel, player->equipboost_vel);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Enr:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i/%i (+%i)", player->energy, player->totalenergy, player->equipboost_enr);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Status:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%s", stringFromStatus(player->status));
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Xp:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i/%i", player->currentlevelxp, player->totallevelxp);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Total Xp:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->totalxp);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Turnboost Atk:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i (%i turns)", player->counters[TURNBOOST_ATK]->innerValue, player->counters[TURNBOOST_ATK]->count);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Turnboost Def:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i (%i turns)", player->counters[TURNBOOST_DEF]->innerValue, player->counters[TURNBOOST_DEF]->count);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Turnboost Vel:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i (%i turns)", player->counters[TURNBOOST_VEL]->innerValue, player->counters[TURNBOOST_VEL]->count);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Turnboost Enr:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i (%i turns)", player->counters[TURNBOOST_ENR]->innerValue, player->counters[TURNBOOST_ENR]->count);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Permboost Atk:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->permboost_atk);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Permboost Def:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->permboost_def);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Permboost Vel:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->permboost_vel);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Permboost Enr:", fighter_label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->permboost_enr);
        DrawText(txt, fighter_value_x, y, txt_height, gui_state.theme.txt_color);

        y = start_y;
        int label_x = (gui_state.gameScreenWidth/2)*1.1f;
        int value_x = (gui_state.gameScreenWidth/2)*1.75f;
        DrawText("Enemies killed:", label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->stats->enemieskilled);
        DrawText(txt, value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Critical hits done:", label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->stats->criticalhits);
        DrawText(txt, value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Consumables found:", label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->stats->consumablesfound);
        DrawText(txt, value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Rooms completed:", label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->stats->roomscompleted);
        DrawText(txt, value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Equips found.", label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->stats->equipsfound);
        DrawText(txt, value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Artifacts found:", label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->stats->artifactsfound);
        DrawText(txt, value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Coins found:", label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->stats->coinsfound);
        DrawText(txt, value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Bosses killed:", label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->stats->bosseskilled);
        DrawText(txt, value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Keys found:", label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->stats->keysfound);
        DrawText(txt, value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
        DrawText("Floors completed:", label_x, y, txt_height, gui_state.theme.txt_color);
        txt = TextFormat("%i", player->stats->floorscompleted);
        DrawText(txt, value_x, y, txt_height, gui_state.theme.txt_color);
        y += txt_height;
    }
    break;
    case ARTIFACTS_VIEW: {
        // TODO: Draw ARTIFACTS_VIEW screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("ARTIFACTS_VIEW SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS Q to go back", gui_state.gameScreenWidth*0.6f, gui_state.gameScreenHeight*0.8f, 20, gui_state.theme.txt_color);
        Rectangle textbox_bounds = (Rectangle) {
            .x = 20,
            .y = 120,
            .width = gui_state.gameScreenWidth/2,
            .height = gui_state.gameScreenHeight/2
        };
        Rectangle descbox_bounds = (Rectangle) {
            .x = textbox_bounds.x,
            .y = textbox_bounds.y + textbox_bounds.height,
            .width = textbox_bounds.width,
            .height = gui_state.gameScreenHeight/4,
        };
        Rectangle spritebox_bounds = (Rectangle) {
            .x = textbox_bounds.x + textbox_bounds.width + 20,
            .y = 120,
            .width = gui_state.gameScreenWidth - textbox_bounds.width - textbox_bounds.x - 40,
            .height = gui_state.gameScreenHeight/2
        };
        int indicator_w = 15;
        int indicator_h = 15;
        //DrawRectangleLines(textbox_bounds.x, textbox_bounds.y, textbox_bounds.width, textbox_bounds.height, ColorFromS4CPalette(palette, S4C_LIGHT_YELLOW));
        //DrawRectangleLines(descbox_bounds.x, descbox_bounds.y, descbox_bounds.width, descbox_bounds.height, ColorFromS4CPalette(palette, S4C_LIGHT_YELLOW));
        //DrawRectangleLines(spritebox_bounds.x, spritebox_bounds.y, spritebox_bounds.width, spritebox_bounds.height, ColorFromS4CPalette(palette, S4C_LIGHT_YELLOW));
        int selected_index = gui_state.selectedIndex;
        for (int i=0; i < ARTIFACTSMAX+1; i++) {
            Artifact* a = player->artifactsBag[i];
            Color color = gui_state.theme.txt_color;
            if (i == selected_index) color = RED;
            const char* txt = "???";
            if (a->qty > 0) {
                txt = TextFormat("%s    x%i", stringFromArtifacts(a->class), a->qty);
            }
            DrawText(txt, textbox_bounds.x + 20, textbox_bounds.y + 20 * i, 20, color);
            if (i == selected_index) {
                DrawRectangle(textbox_bounds.x, textbox_bounds.y + 20*i, indicator_w, indicator_h, gui_state.theme.txt_color);
                txt = "???";
                if (a->qty > 0) {
                    txt = a->desc;
                    DrawSpriteRect(artifacts_sprites_proper[i], spritebox_bounds, 8, 12, spritebox_bounds.width/12, palette, PALETTE_S4C_H_TOTCOLORS);
                }
                int txt_height = 20;
                DrawText(txt, descbox_bounds.x, descbox_bounds.y, txt_height, gui_state.theme.txt_color);
            }
        }
    }
    break;
    case ENDING: {
        // TODO: Draw ENDING screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("ENDING SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS ENTER or TAP to quit the game", 120, 220, 20, gui_state.theme.txt_color);
        if (gui_state.game_won) {
            DrawText("YOU WON!", 120, 240, 20, GREEN);
        } else {
            DrawText("YOU LOST.", 120, 240, 20, RED);
        }
    }
    break;
    case DOOR_ANIM: {
        // TODO: Draw DOOR_ANIM screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("DOOR SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS ENTER or TAP to RETURN to FLOOR_VIEW SCREEN", 120, 220, 20, gui_state.theme.txt_color);

        int door_frame_W = ((int)(gui_state.gameScreenWidth * 0.3f) / 21) * 21;
        int door_frame_H = door_frame_W;
        int door_rect_X = (gui_state.gameScreenWidth/2) - (door_frame_W/2);
        int door_rect_Y = (gui_state.gameScreenHeight/2) - (door_frame_H/2);
        Rectangle door_r = CLITERAL(Rectangle) {
            door_rect_X,
            door_rect_Y,
            door_frame_W,
            door_frame_H,
        };
        int door_res = DrawSpriteRect(enter_door[current_anim_frame], door_r, 21, 21, door_frame_W/21, palette, PALETTE_S4C_H_TOTCOLORS);
        if (door_res != 0 ) {
            DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, ColorFromS4CPalette(palette, S4C_RED));
            DrawText("Window too small.", 20, 20, 20, RAYWHITE);
            DrawText("Please resize.", 20, 50, 20, RAYWHITE);
            //current_anim_frame--; // TODO: can't update the current animation frame since it's not being taken as a reference as of now.
        }
    }
    break;
    case CHEST_ANIM: {
        // TODO: Draw CHEST_ANIM screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText("CHEST SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS ENTER or TAP to RETURN to FLOOR_VIEW SCREEN", 120, 220, 20, gui_state.theme.txt_color);

        int chest_frame_W = ((int)(gui_state.gameScreenWidth * 0.3f) / (CHEST_COLS-1)) * (CHEST_COLS-1);
        int chest_frame_H = chest_frame_W;
        int chest_rect_X = (gui_state.gameScreenWidth/2) - (chest_frame_W/2);
        int chest_rect_Y = (gui_state.gameScreenHeight/2) - (chest_frame_H/2);
        Rectangle chest_r = CLITERAL(Rectangle) {
            chest_rect_X,
            chest_rect_Y,
            chest_frame_W,
            chest_frame_H,
        };
        int chest_res = DrawSpriteRect(alt_chest_opening[current_anim_frame], chest_r, CHEST_ROWS-1, CHEST_COLS-1, chest_frame_W/(CHEST_COLS-1), palette, PALETTE_S4C_H_TOTCOLORS);
        if (chest_res != 0 ) {
            DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, ColorFromS4CPalette(palette, S4C_RED));
            DrawText("Window too small.", 20, 20, 20, RAYWHITE);
            DrawText("Please resize.", 20, 50, 20, RAYWHITE);
            //current_anim_frame--; // TODO: can't update the current animation frame since it's not being taken as a reference as of now.
        }
    }
    break;
#ifdef HELAPORDO_DEBUG_ACCESS
    case DEBUG_VIEW: {
        // TODO: Draw DEBUG_VIEW screen here!
        DrawRectangle(0, 0, gui_state.gameScreenWidth, gui_state.gameScreenHeight, gui_state.theme.bg_color);
        DrawText(TextFormat("Default Mouse: [%i, %i]", (int)gui_state.mouse.x, (int)gui_state.mouse.y), 350, 25, 20, gui_state.theme.txt_color);
        DrawText(TextFormat("Virtual Mouse: [%i, %i]", (int)gui_state.virtualMouse.x, (int)gui_state.virtualMouse.y), 350, 55, 20, gui_state.theme.txt_color);
        DrawText(TextFormat("Current save path: {%s}", current_save_path), 110, gui_state.gameScreenHeight/2 + 40, 20, gui_state.theme.txt_color);
        if (game_path != NULL) {
            DrawText(TextFormat("Current seed: {%s}", game_path->seed), 110, gui_state.gameScreenHeight/2 + 60, 20, gui_state.theme.txt_color);
        }
        DrawText(TextFormat("Current WinCon: {%s} [%i / %i]", stringFromWinconClass(game_path->win_condition->class), game_path->win_condition->current_val, game_path->win_condition->target_val), 110, gui_state.gameScreenHeight/2 + 80, 20, gui_state.theme.txt_color);
        DrawText("DEBUG SCREEN", 20, 20, 40, gui_state.theme.txt_color);
        DrawText("WIP", 20, gui_state.gameScreenHeight - (10 * gui_state.scale), 40, ColorFromS4CPalette(palette, S4C_SALMON));
        DrawText("PRESS ENTER to RETURN to FLOOR_VIEW SCREEN", 110, 260, 20, gui_state.theme.txt_color);

        Gui_Button cycle_layout_button = gui_state.debug_buttons.buttons[BUTTON_CYCLE_DEBUG_LAYOUT];
        Rectangle cycle_cell = cycle_layout_button.r;
        Color cycle_c = cycle_layout_button.box_color;
        if (cycle_layout_button.state == BUTTON_HOVER) {
            DrawRectangleRec(cycle_cell, RED);
        } else {
            DrawRectangleRec(cycle_cell, cycle_c);
        }
        DrawText(cycle_layout_button.label, cycle_layout_button.r.x + (gui_state.gameScreenWidth * 0.02f), cycle_layout_button.r.y + (gui_state.gameScreenHeight * 0.02f), gui_state.gameScreenHeight * 0.04f, cycle_layout_button.text_color);

        switch (gui_state.debug_selection) {
        case GUI_DEBUG_FIGHTER: {
            DrawText("DEBUG FIGHTER", 20, 60, 20, gui_state.theme.txt_color);
            for (Gui_Debug_Fighter_Layout_Group_Index i = 0; i < gui_state.debug_fighter_buttons.len; i++) {
                Gui_Button_Group* group = gui_state.debug_fighter_buttons.groups[i];
                assert(group->len > 0);
                int txt_height = 10;
                const char* txt = NULL;
                switch (i) {
                case DEBUG_FIGHTER_LAYOUT_SPECIALSLOTS_GROUP: {
                    txt = TextFormat("SPECIALSLOTS");
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_SKILLSLOTS_GROUP: {
                    txt = TextFormat("SKILLSLOTS");
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_TURNCOUNTERS_GROUP: {
                    txt = TextFormat("TURNCOUNTERS");
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_PERKS_GROUP: {
                    txt = TextFormat("PERKS");
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_EQUIPSLOTS_GROUP: {
                    txt = TextFormat("EQUIPSLOTS");
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_EQUIPSBAG_GROUP: {
                    txt = TextFormat("EQUIPSBAG");
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_CONSUMABLESBAG_GROUP: {
                    txt = TextFormat("CONSUMABLESBAG");
                }
                break;
                case DEBUG_FIGHTER_LAYOUT_ARTIFACTSBAG_GROUP: {
                    txt = TextFormat("ARTIFACTSBAG");
                }
                break;
                }
                DrawText(txt, (group->buttons[0].r.x - MeasureText(txt, txt_height))/2, group->buttons[0].r.y + (txt_height/2), txt_height, gui_state.theme.txt_color);
                for (int j = 0; j < group->len; j++) {
                    Gui_Button button = gui_state.debug_fighter_buttons.groups[i]->buttons[j];
                    Rectangle cell = button.r;
                    Color c = button.box_color;
                    DrawRectangleRec(cell, c);
                    DrawRectangleLines(cell.x, cell.y, cell.width, cell.height, BLACK);
                }
            }
        }
        break;
        case GUI_DEBUG_FLOOR: {
            DrawText("DEBUG FLOOR", 20, 60, 20, gui_state.theme.txt_color);

            int map_r_w = (((int)gui_state.gameScreenWidth * 0.25f) / FLOOR_MAX_COLS) * FLOOR_MAX_COLS;
            //int map_r_h = map_r_w;
            int map_r_h = (((int)gui_state.gameScreenHeight * 0.3f) / FLOOR_MAX_ROWS) * FLOOR_MAX_ROWS;
            //int group_r_w = (4 * map_r_w); // + (3 * map_x_spacing);
            //int group_r_h = map_r_h;
            int group_r_x = 0;
            int group_r_y = gui_state.gameScreenHeight * 0.15f;
            //DrawRectangle(group_r_x, group_r_y, group_r_w, group_r_h, RED);

            int cell_w = map_r_w / FLOOR_MAX_COLS;
            int cell_h = map_r_h / FLOOR_MAX_ROWS;
            for (int i = 0; i < 4; i++) {
                Rectangle map_r = {
                    .x = group_r_x + (i * map_r_w),
                    .y = group_r_y,
                    .width = map_r_w,
                    .height = map_r_h
                };
                //DrawRectangleLines(map_r.x, map_r.y, map_r.width, map_r.height, BLUE);

                for (int row_idx = 0; row_idx < FLOOR_MAX_ROWS; row_idx++) {
                    for (int col_idx = 0; col_idx < FLOOR_MAX_COLS; col_idx++) {
                        Rectangle cell_r = {
                            .x = map_r.x + (col_idx * cell_w),
                            .y = map_r.y + (row_idx * cell_h),
                            .width = cell_w,
                            .height = cell_h
                        };
                        //DrawRectangleLines(cell_r.x, cell_r.y, cell_r.width, cell_r.height, BLACK);
                        Color cell_color = BLACK;
                        switch (i) {
                        case 0: {
                            if (current_floor->floor_layout[col_idx][row_idx]) {
                                cell_color = WHITE;
                            }
                        }
                        break;
                        case 1: {
                            if (current_floor->rooms_matrix[col_idx][row_idx] != NULL) {
                                cell_color = GREEN;
                            } else {
                                cell_color = DARKGRAY;
                            }
                        }
                        break;
                        case 2: {
                            switch (current_floor->roomclass_layout[col_idx][row_idx]) {
                            case BASIC: {
                                cell_color = DARKGRAY;
                            }
                            break;
                            case HOME: {
                                cell_color = GREEN;
                            }
                            break;
                            case ENEMIES: {
                                cell_color = ColorFromS4CPalette(palette, S4C_CYAN);
                            }
                            break;
                            case BOSS: {
                                cell_color = RED;
                            }
                            break;
                            case SHOP: {
                                cell_color = MAGENTA;
                            }
                            break;
                            case TREASURE: {
                                cell_color = ORANGE;
                            }
                            break;
                            case ROADFORK: {
                                cell_color = BLUE;
                            }
                            break;
                            case WALL: {
                                cell_color = DARKPURPLE;
                            }
                            break;
                            }
                        }
                        break;
                        case 3: {
                            if (current_floor->explored_matrix[col_idx][row_idx] == 1) {
                                if (current_floor->roomclass_layout[col_idx][row_idx] != WALL) {
                                    cell_color = WHITE;
                                } else {

                                    cell_color = RED;
                                }
                            }
                        }
                        break;
                        }
                        DrawRectangle(cell_r.x, cell_r.y, cell_r.width, cell_r.height, cell_color);
                    }
                }
            }
        }
        break;
        }

        int default_kls_r_w = gui_state.gameScreenWidth * 0.5f;
        int default_kls_r_h = gui_state.gameScreenHeight * 0.05f;
        Rectangle default_kls_r = {
            .x = (gui_state.gameScreenWidth - default_kls_r_w) /2,
            .y = ((gui_state.gameScreenHeight/4) *3) - (default_kls_r_h/2),
            .width = default_kls_r_w,
            .height = default_kls_r_h,
        };

        float default_kls_usage = (100 * default_kls->offset) / default_kls->size;

        Rectangle default_kls_used_r = default_kls_r;
        default_kls_used_r.width *= (default_kls_usage / 100);
        DrawRectangleRec(default_kls_r, DARKGRAY);
        DrawRectangleRec(default_kls_used_r, ColorFromS4CPalette(palette, S4C_CYAN));

        DrawText(TextFormat("Default kls usage: %.2lf%%", default_kls_usage), default_kls_r.x + default_kls_r.width/2 - (MeasureText(TextFormat("Default kls usage: %.2lf%%", default_kls_usage), 20) /2), default_kls_r.y + default_kls_r.height/2 - (20/2), 20, BLACK);

        int temporary_kls_r_w = default_kls_r_w;
        int temporary_kls_r_h = default_kls_r_h;
        int default_temporary_kls_h_spacing = 50;
        Rectangle temporary_kls_r = {
            .x = (gui_state.gameScreenWidth - temporary_kls_r_w) /2,
            .y = default_kls_r.y + temporary_kls_r.height + default_temporary_kls_h_spacing,
            .width = temporary_kls_r_w,
            .height = temporary_kls_r_h,
        };
        float temporary_kls_usage = (100 * temporary_kls->offset) / temporary_kls->size;
        Rectangle temporary_kls_used_r = temporary_kls_r;
        temporary_kls_used_r.width *= (temporary_kls_usage / 100);
        DrawRectangleRec(temporary_kls_r, DARKGRAY);
        DrawRectangleRec(temporary_kls_used_r, ColorFromS4CPalette(palette, S4C_CYAN));

        const char* txt = TextFormat("Temp kls usage: %.2lf%%", temporary_kls_usage);
        DrawText(txt, temporary_kls_r.x + temporary_kls_r.width/2 - (MeasureText(txt, 20) /2), temporary_kls_r.y + temporary_kls_r.height/2 - (20/2), 20, BLACK);
    }
    break;
#endif // HELAPORDO_DEBUG_ACCESS
    default: {
        break;
    }
    }

    EndTextureMode();
}

/**
 * Takes a specialMove, a Fighter, a Enemy, a Boss and a Path pointers (and integers for current room and enemy indexes) and uses the requested special move.
 * Prints the result to the passed WINDOW.
 * The isBoss integer determines if the receiver is the Enemy or the Boss.
 * @see Fighter
 * @see SpecialSlot
 * @see SPECIALSMAX
 * @param move The specialMove to execute.
 * @param w The WINDOW pointer to print results to.
 * @param f The Fighter pointer with a equipsBag.
 * @param e The Enemy pointer for current enemy.
 * @param b The Boss pointer.
 * @param p The Path pointer of the current game.
 * @param roomIndex The index of current room.
 * @param enemyIndex The index of current enemy.
 * @param isBoss Is equal to 1 when receiver is a Boss.
 */
void fight_Special(specialMove move, Rectangle *w, Fighter *f, Enemy *e, Boss *b,
                   Path *p, int roomIndex, int enemyIndex, int isBoss)
{

    switch (move) {
    case KSlash: {
        knightSpecial_Slash(w, f, e, b, p, roomIndex, enemyIndex, isBoss);
    }
    break;
    case KCover: {
        knightSpecial_Cover(w, f, e, b, p, roomIndex, enemyIndex, isBoss);
    }
    break;
    case KArmordrop: {
        knightSpecial_Armordrop(w, f, e, b, p, roomIndex, enemyIndex,
                                isBoss);
    }
    break;
    case KBerserk: {
        knightSpecial_Berserk(w, f, e, b, p, roomIndex, enemyIndex, isBoss);
    }
    break;
    case AHeadshot: {
        archerSpecial_Headshot(w, f, e, b, p, roomIndex, enemyIndex,
                               isBoss);
    }
    break;
    case AQuivercheck: {
        archerSpecial_Quivercheck(w, f, e, b, p, roomIndex, enemyIndex,
                                  isBoss);
    }
    break;
    case APoisonshot: {
        archerSpecial_Poisonshot(w, f, e, b, p, roomIndex, enemyIndex,
                                 isBoss);
    }
    break;
    case AFireshot: {
        archerSpecial_Fireshot(w, f, e, b, p, roomIndex, enemyIndex,
                               isBoss);
    }
    break;
    case MFatewarp: {
        mageSpecial_Fatewarp(w, f, e, b, p, roomIndex, enemyIndex, isBoss);
    }
    break;
    case MPowerup: {
        mageSpecial_Powerup(w, f, e, b, p, roomIndex, enemyIndex, isBoss);
    }
    break;
    case MSpellstrike: {
        mageSpecial_Spellstrike(w, f, e, b, p, roomIndex, enemyIndex,
                                isBoss);
    }
    break;
    case MFlamering: {
        mageSpecial_Flamering(w, f, e, b, p, roomIndex, enemyIndex, isBoss);
    }
    break;
    case XGrimdagger: {
        assassinSpecial_Grimdagger(w, f, e, b, p, roomIndex, enemyIndex,
                                   isBoss);
    }
    break;
    case XLeechknife: {
        assassinSpecial_Leechknife(w, f, e, b, p, roomIndex, enemyIndex,
                                   isBoss);
    }
    break;
    case XDisguise: {
        assassinSpecial_Disguise(w, f, e, b, p, roomIndex, enemyIndex,
                                 isBoss);
    }
    break;
    case XVenomblade: {
        assassinSpecial_Venomblade(w, f, e, b, p, roomIndex, enemyIndex,
                                   isBoss);
    }
    break;
    default: {
        fprintf(stderr, "%i is not a valid move.\n", move);
        exit(EXIT_FAILURE);
    }
    break;
    }

}
