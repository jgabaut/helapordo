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
#include "game_core.h"

const char* helapordo_title_string = "     __  __  ____  __     ______  _____   ______  ____    _____    ______\n\
    / / / / / __/ / /    / __  / / __  / / __  / / __ \\  / __  \\  / __  /\n\
   / /_/ / / /_  / /    / / / / / /_/ / / / / / / /_/ / / / /  / / / / /\n\
  / __  / / __/ / /    / /_/ / / ____/ / / / / /    _/ / / /  / / / / /\n\
 / / / / / /_  / /_   / __  / / /     / /_/ / / /\\ |  / /_/ _/ / /_/ /\n\
/_/ /_/ /____/ \\___/ /_/ /_/ /_/     /_____/ /_/ /_/ /_____/  /_____/\n";

#ifdef HELAPORDO_BUILD_STR
const char* helapordo_build_string = HELAPORDO_BUILD_STR "-" VERSION;
#else
const char* helapordo_build_string = "unknown-" VERSION;
#endif

char *gamemodenamestrings[GAMEMODE_MAX + 1] = {
    "Standard",
    "Story",
    "Rogue"
};

char *hlp_regiontype_strings[HLP_MAX_INDEX + 1] = {
    "None",
    "KLS_Temp_Header",
    "KLS_Header",
    "HLP_Path",
    "HLP_Wincon",
    "HLP_Room",
    "HLP_Floor",
    "HLP_turnOP_args",
    "HLP_Fighter",
    "HLP_Enemy",
    "HLP_Boss",
    "HLP_FoeParty",
    "HLP_Equip",
    "HLP_Equipslot",
    "HLP_Specialslot",
    "HLP_Skillslot",
    "HLP_Turncounter",
    "HLP_Perk",
    "HLP_Consumable",
    "HLP_Artifact",
    "HLP_Chest",
    "HLP_Treasure",
    "HLP_Shop",
    "HLP_Roadfork",
    "HLP_Turncounter_desc",
    "HLP_Room_desc",
    "HLP_countStats",
    "HLP_Saveslot",
    "HLP_Gamestate",
    "HLP_Gamescreen",
    "HLP_loadInfo",
};

Saveslot default_saveslots[MAX_SAVESLOTS + 1] = {
    {"default_1", "hlpd-1"},
    {"default_2", "hlpd-2"},
    {"default_3", "hlpd-3"},
};

turnOption_OP turnOP_from_turnOption(turnOption t)
{
    turnOption_OP res = OP_INVALID;

    switch (t) {
    case INVALID: {
        res = OP_INVALID;
    }
    break;
    case QUIT: {
        res = OP_QUIT;
    }
    break;
    case NEW_GAME: {
        res = OP_NEW_GAME;
    }
    break;
    case LOAD_GAME: {
        res = OP_LOAD_GAME;
    }
    break;
    case FIGHT: {
        res = OP_FIGHT;
    }
    break;
    case SPECIAL: {
        res = OP_SPECIAL;
    }
    break;
    case CONSUMABLE: {
        res = OP_CONSUMABLE;
    }
    break;
    case ARTIFACTS: {
        res = OP_ARTIFACTS;
    }
    break;
    case EQUIPS: {
        res = OP_EQUIPS;
    }
    break;
    case PERKS: {
        res = OP_PERKS;
    }
    break;
    case STATS: {
        res = OP_STATS;
    }
    break;
    case SAVE: {
        res = OP_SAVE;
    }
    break;
    case EXPLORE: {
        res = OP_EXPLORE;
    }
    break;
    case DEBUG: {
        res = OP_DEBUG;
    }
    break;
    case SKILL: {
        res = OP_SKILL;
    }
    break;
    default: {
        //FIXME:
        //Signal error right here?
        return OP_INVALID;
    }
    }
    return res;
}

foeTurnOption_OP foeTurnOP_from_foeTurnOption(foeTurnOption ft)
{
    foeTurnOption_OP res = FOE_OP_INVALID;

    switch (ft) {
    case FOE_INVALID: {
        res = FOE_OP_INVALID;
    }
    break;
    case FOE_FIGHT: {
        res = FOE_OP_FIGHT;
    }
    break;
    case FOE_SPECIAL: {
        res = FOE_OP_SPECIAL;
    }
    break;
    case FOE_IDLE: {
        res = FOE_OP_IDLE;
    }
    break;
    default: {
        //FIXME:
        //Signal error right here?
        return FOE_OP_INVALID;
    }
    }
    return res;
}

OP_res OP_res_from_fightResult(fightResult fr)
{
    OP_res res = INVALID_OP;

    switch (fr) {
    case FIGHTRES_INVALID: {
        res = INVALID_OP;
    }
    break;
    case FIGHTRES_KILL_DONE: {
        res = OP_RES_KILL_DONE;
    }
    break;
    case FIGHTRES_DMG_DEALT: {
        res = OP_RES_DMG_DEALT;
    }
    break;
    case FIGHTRES_DMG_TAKEN: {
        res = OP_RES_DMG_TAKEN;
    }
    break;
    case FIGHTRES_DEATH: {
        res = OP_RES_DEATH;
    }
    break;
    case FIGHTRES_NO_DMG: {
        res = OP_RES_NO_DMG;
    }
    break;
    case FIGHTRES_CLASH: {
        res = OP_RES_CLASH;
    }
    break;
    default: {
        //FIXME:
        //Signal error right here?
        return INVALID_OP;
    }
    }
    return res;
}

char *savetypestrings[SAVETYPE_MAX + 1] = {
    "Home_Room",
    "Enemies_Room",
    "Floor_Menu",
};

char *opresstrings[OP_RES_MAX + 1] = {
    "Ok",
    "Invalid OP",
    "No OP",
    "Can't open savefile",
    "Kill done",
    "Damage dealt",
    "Damage taken",
    "Death",
    "No damage done",
    "Mutual damage done",
    "Loaded enemy room",
};

char *turnopstrings[TURNOP_MAX + 1] = {
    "New Game",
    "Load Save",
    "!INVALID!",
    "Fight",
    "Special",
    "Quit",
    "Consumable",
    "Artifacts",
    "Equips",
    "Perks",
    "Stats",
    "Save",
    "Explore",
    "Debug",
    "Load Enemy Room",
    "Load Home Room",
    "Skill",
};

char *foeturnopstrings[FOETURNOP_MAX + 1] = {
    "!INVALID!",
    "Idle",
    "Fight",
    "Special",
};

char *fightresultstrings[FIGHT_RES_TOT + 1] = {
    "!Invalid fight result!",
    "Kill done",
    "Damage dealt",
    "Damage taken",
    "Death",
    "No damage done",
    "Mutual damage done",
};

char *consumablestrings[CONSUMABLESMAX + 1] = {
    "Potion",
    "Rock",
    "Bread",
    "Coffee",
    "Powergem",
    "Magic Bean"
};

Consumable consumablesBase[CONSUMABLESMAX + 1] = {

    {Potion, "Amberosia", "Completely restores your health.", 0},
    {Rock, "Dwayne Johnson", "Hurts the enemy and lowers its defences.", 0},
    {Bread, "Carbowheat", "Enhances your power, slows you a little.", 0},
    {Coffee, "Espresso", "Makes you faster and tense.", 0},
    {Powergem, "Power Gem", "You will feel wiser.", 0},
    {Magicbean, "Magic Bean", "Grants you new magic strength.", 0}
};

char *artifactstrings[ARTIFACTSMAX + 1] = {
    "Thinking Skull",
    "Soft Twinkie",
    "Gold Wristband",
    "Boar Tail",
    "Dark Orb",
    "Power Syphon",
    "Giant Fossile"
};

Artifact artifactsBase[ARTIFACTSMAX + 1] = {

    {THKSKULL, "Thinking Skull", "You learn from questioning.", 0, 0, 0},
    {TWINKIE, "Soft Twinkie", "Zombies seem to not like this.", 0, 0, 0},
    {
        WRISTBAND, "Gold Wristband", "I'd have been cooler on your finger.", 0, 0,
        0
    },
    {BOARTAIL, "Boar Tail", "The beast managed to run.", 0, 0, 0},
    {CHAOSORB, "Dark Orb", "This feels ominous.", 0, 0, 0},
    {POWERSYPHON, "Power Syphon", "Energy can only be transformed.", 0, 0, 0},
    {
        GIANTFOSSILE, "Giant Fossile", "Maybe some species really are related.", 0,
        0, 0
    }
};

char *classesstrings[CLASSESMAX + 1] = {
    "Knight",
    "Archer",
    "Mage",
    "Assassin"
};

BaseStats basestats[CLASSESMAX + 1] = {

    {Knight, 32, 12, 22, 7, 1, 0, 1, 0, 32, 5, 5},
    {Archer, 24, 15, 14, 11, 1, 0, 1, 0, 24, 5, 5},
    {Mage, 17, 19, 10, 8, 1, 0, 1, 0, 17, 5, 5},
    {Assassin, 20, 16, 12, 10, 1, 0, 1, 0, 20, 5, 5}

};

char *classenemystrings[ENEMYCLASSESMAX + 1] = {
    "Mummy",
    "Ghost",
    "Zombie",
    "Goblin",
    "Imp",
    "Troll",
    "Boar",
    "Werewolf"
};

EnemyBaseStats baseenemystats[ENEMYCLASSESMAX + 1] = {

    {Mummy, 22, 11, 7, 5, 1, 1, 22, 0, 5, 5},
    {Ghost, 35, 5, 16, 6, 1, 2, 35, 0, 5, 5},
    {Zombie, 20, 6, 7, 5, 1, 1, 20, 0, 5, 5},
    {Goblin, 32, 10, 8, 11, 1, 2, 32, 0, 5, 5},
    {Imp, 24, 12, 7, 10, 1, 3, 24, 0, 5, 5},
    {Troll, 42, 4, 17, 4, 2, 3, 42, 0, 5, 5},
    {Boar, 30, 9, 12, 8, 2, 4, 30, 0, 5, 5},
    {Werewolf, 20, 14, 5, 7, 1, 3, 20, 0, 5, 5}
};

char *classbossstrings[BOSSCLASSESMAX + 1] = {
    "Blue Troll",
    "Headless Ninja",
    "Crawling Dude",
    "Sr. Warthog",
    "Doppelganger"
};

BossBaseStats basebossstats[BOSSCLASSESMAX + 1] = {

    {Blue_Troll, 90, 11, 26, 5, 1, 18, 90, 0, 8, 5},
    {Headless_Ninja, 65, 19, 16, 9, 1, 18, 65, 0, 8, 5},
    {Crawling_Dude, 76, 13, 32, 7, 1, 18, 76, 0, 8, 5},
    {Sr_Warthog, 58, 17, 21, 10, 1, 18, 58, 0, 8, 5},
    {Doppelganger, 15, 10, 10, 10, 1, 18, 15, 0, 8, 5}
};

char *specialsnamestrings[CLASSESMAX + 1][SPECIALSMAX + 1] = {
    {"Slash Hit", "Shield Cover", "Armor Drop", "Berserk Rush"},   /**< names for Knight, fighterClass 0*/
    {"Headshot", "Quiver Check", "Poison Shot", "Burning Arrow"},   /**< names for Archer, fighterClass 1*/
    {"Fate Warp", "Power Spell", "Pity Spell", "Flame Ring"},	/**< names for Mage, fighterClass 2*/
    {"Grim Dagger", "Leech Knife", "Disguise Kit", "Venomous Needles"},	  /**< names for Assassin, fighterClass 3*/
};

char *specialsdescstrings[CLASSESMAX + 1][SPECIALSMAX + 1] = {
    {"Powerful slash.", "Shield for better defense.", "Less armor makes you dapper but frail.", "You recoil from your fury."},	 /**< names for Knight, fighterClass 0*/
    {"That's a clean hit,", "Stronger arrows for your bow.", "Lay poison on your tips.", "Set fire to the rain."},   /**< names for Archer, fighterClass 1*/
    {"You will feel luckier.", "Your power grows.", "You spare your victim.", "Summon a ring of fire."},   /**< names for Mage, fighterClass 2*/
    {"Stab enemy's defense.", "Sap enemy's health.", "Camouflage for better defense.", "Throw poisoned needles."},   /**< names for Assassin, fighterClass 3*/
};

int specialscosts[CLASSESMAX + 1][SPECIALSMAX + 1] = {

    {1, 2, 3, 3},   /**< costs for Knight, fighterClass 0*/
    {2, 4, 2, 3},   /**< costs for Archer, fighterClass 1*/
    {4, 3, 2, 3},   /**< costs for Mage, fighterClass 2*/
    {4, 3, 3, 3},    /**< costs for Assassin, fighterClass 3*/
};

char *skillsnamestrings[SKILLSTOTAL + 1] = {
    "Atk boost",
    "Last unlockable",
    "Idle",
    "Skills_MAX",
};

char *skillsdescstrings[SKILLSTOTAL + 1] = {
    "A temporary boost to atk",
    "Sentinel value for last unlockable skill",
    "Do nothing",
    "Sentinel value for total skills",
};

int skillscosts[SKILLSTOTAL + 1] = {
    1,
    99,
    0,
    99,
};

char *equipzonestrings[EQUIPZONES + 1] = {
    "Head",
    "Torso",
    "Legs"
};

char *perksnamestrings[PERKSMAX + 1] = {
    "Mighty Strikes",
    "Lucky Charm",
    "Garlic Fever",
    "Runic Circle",
    "Hot Body",
    "Harvester",
    "Biohazard",
    "Oracle Gift",
    "Penicillin"
};

char *perksdescstrings[PERKSMAX + 1] = {
    "When you hit hard, you hit REALLY HARD.",
    "You may have better chances at smashing.",
    "Enemy blood seems attractive.",
    "Monks don't have to rest.",
    "Feels burning to the touch.",
    "You reap more fruits.",
    "You shall bring the plague upon them.",
    "A second call from the door.",
    "You are prepared for pollen season."
};

char *winconstrings[WINCON_CLASS_MAX + 1] = {
    "All Artifacts",
    "Full path",
    "All bosses"
};

char *qualitytrings[QUALITIESMAX + 1] = {
    "Bad",
    "Average",
    "Good"
};

char *equipstrings[EQUIPSMAX + 1] = {
    "Vest",
    "Helmet",
    "Boots",
    "Cape",
    "Tophat",
    "Loafers"
};

Equip equips[EQUIPSMAX + 1] = {

    {Vest, TORSO, "Vest", "Nice cloth.", 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {Helmet, HEAD, "Helmet", "Fits on your head.", 0, 0, 1, 0, 2, 0, 0, 0, 0},
    {Boots, LEGS, "Boots", "NOT made for crawling.", 0, 0, 1, 1, 0, 1, 0, 0, 0},
    {Cape, TORSO, "Cape", "Doesn't make you fly.", 0, 0, 1, 2, 0, 0, 1, 0, 0},
    {Tophat, HEAD, "Top Hat", "Dapper you say?", 0, 0, 1, 0, 1, 0, 1, 0, 0},
    {
        Loafers, LEGS, "Loafers", "Soft and comfy for battles.", 0, 0, 1, 0, 1, 2,
        0, 0, 0
    }
};

char *chestdescstrings[CHEST_CLASS_MAX + 1] = {
    "Base",
    "Beast"
};

char *treasuredescstrings[TREASURE_CLASS_MAX + 1] = {
    "Chest",
    "Consumable",
    "Artifact"
};

char *roadforkoptionstrings[ROADFORK_OPTIONS_MAX + 1] = {
    "Boss",
    "Shop",
    "Treasure",
    "Enemies",
    "Roadfork"
};

char *roomnamestrings[ROOM_CLASS_MAX + 1] = {
    "Enemies",
    "Shop",
    "Boss",
    "Treasure",
    "Roadfork",
    "Home",
    "Wall",
    "Basic"
};

char *floornamestrings[FLOOR_CLASS_MAX + 1] = {
    "Normal Floor",
    "Debug Floor"
};

char *foepartystrings[FOEPARTY_CLASS_MAX + 1] = {
    "Enemies",
    "Bosses"
};

FoePartyBaseStats basefoepartystats[FOEPARTY_CLASS_MAX + 1] = {
    {Enemies, 1},
    {Bosses, 1}
};

#ifndef _WIN32
const wchar_t HEAD_CHAR_ICON = 0x1F3A9;
const wchar_t TORSO_CHAR_ICON = 0x1F455;
const wchar_t LEGS_CHAR_ICON = 0x1F45F;
#else
const wchar_t HEAD_CHAR_ICON = 'H';
const wchar_t TORSO_CHAR_ICON = 'T';
const wchar_t LEGS_CHAR_ICON = 'L';
#endif // _WIN32
