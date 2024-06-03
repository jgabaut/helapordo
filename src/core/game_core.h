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

#ifndef GAME_CORE_H
#define GAME_CORE_H

#ifndef HELAPORDO_OS
#define HELAPORDO_OS "unknown"
#endif

#ifndef HELAPORDO_MACHINE
#define HELAPORDO_MACHINE "unknown"
#endif

#include "../../ringabuf/src/ringabuf.h"

#ifdef HELAPORDO_CURSES_BUILD
#ifdef _WIN32
#include <ncursesw/panel.h>
#define S4C_WIN_BG 0
#define S4C_WIN_RED 1
#define S4C_WIN_GREEN 2
#define S4C_WIN_BLUE 3
#define S4C_WIN_CYAN 4
#define S4C_WIN_WHITE 5
#define S4C_WIN_YELLOW 6
#define S4C_WIN_BLACK_ON_WHITE 7
#define S4C_WIN_PURPLE 8
#define S4C_WIN_WHITE_ON_RED 9
#define S4C_WIN_WHITE_ON_PURPLE 10
#else
#include <panel.h>
#endif // _WIN32

/**
 * Defines GameScreen type, wrapping curses WINDOW.
 */
typedef struct GameScreen {
    WINDOW* win; /**< Pointer to window for the gamescreen.*/
    int colors; /**< Number of colors supported by the gamescreen.*/
    int color_pairs; /**< Number of color pairs supported by the gamescreen.*/
    int cols; /**< Number of columns for the gamescreen.*/
    int rows; /**< Number of rows for the gamescreen.*/
    int escape_delay; /**< Number of milliseconds to wait after reading an escape character, to distinguish between an individual escape character entered on the keyboard from escape sequences sent by cursor- and function-keys. See: https://manpages.debian.org/bullseye/ncurses-doc/curses_variables.3ncurses.en.html*/
    int tabsize; /**< Number of columns used by curses to convert a tab to spaces when adding tab to a window.*/
} GameScreen;

#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined."
#else
/**
 * Defines GameScreen type, as an enum.
 */
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING, DOOR_ANIM } GameScreen;
// Add more includes for rl-build here
#ifdef _WIN32
/**
 * We need to turn off some part of Windows API to avoid clashes with raylib header redefinitions.
 * Relevant links:
 * https://learn.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers#faster-builds-with-smaller-header-files
 * https://stackoverflow.com/a/1394929
 * https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-rectangle
 */
#define WIN32_LEAN_AND_MEAN // We don't want no playsound
#define NOGDI // We don't want Rectangle from Windows
#define NOUSER // We don't want no user routines
#endif // _WIN32
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

#include "../../koliseo/src/koliseo.h"
#include "../palette.h" /**< Defines the color palette used for the s4c functions, includes animate.h.*/

extern const char* helapordo_title_string; /**< Defines a formatted string for title output to FILE.*/

extern const char* helapordo_build_string;

/**
 * The different kinds of HLPD_Key.
 * Ordering is directly related to hlpd_keyclass_strings.
 * @see hlpd_keyclass_strings
 */
typedef enum HLPD_KeyClass {
    HLPD_KEY_UP = 0,
    HLPD_KEY_RIGHT,
    HLPD_KEY_DOWN,
    HLPD_KEY_LEFT,
    HLPD_KEY_DWNPAGE,
    HLPD_KEY_UPPAGE,
    HLPD_KEY_CONFIRM,
    HLPD_KEY_QUIT,
    HLPD_KEY_MENU,
} HLPD_KeyClass;

/** Maximum index of HLPD_KeyClass, so that the size has to add 1 for the 0th index
 * @see HLPD_KeyClass
 */
#define HLPD_KEYCLASS_MAX 8

/**
 * Array with the name strings for Gamemode.
 * @see Gamemode
 */
extern char *hlpd_keyclass_strings[HLPD_KEYCLASS_MAX + 1];

/**
 * Holds info about a keybind for the game.
 * @see HLPD_KeyClass
 */
typedef struct HLPD_Key {
    int val;
    HLPD_KeyClass class;
} HLPD_Key;

/**
 * Array with the keybinds for the game.
 * @see HLPD_Key
 */
extern HLPD_Key hlpd_default_keybinds[HLPD_KEYCLASS_MAX+1];

/**
 * The different kinds of schemas available for directional keys.
 * Ordering is directly related to hlpd_directionalkeyschema_strings.
 * @see hlpd_directionalkeyschema_strings
 */
typedef enum HLPD_DirectionalKeys_Schema {
    HLPD_ARROW_KEYS=0,
    HLPD_VIM_KEYS,
    HLPD_WASD_KEYS,
} HLPD_DirectionalKeys_Schema;

/** Maximum index of HLPD_DirectionalKeys_Schema, so that the size has to add 1 for the 0th index
 * @see HLPD_DirectionalKeys_Schema
 */
#define HLPD_DIRECTIONALKEYS_SCHEMAS_MAX 2

/**
 * Array with the name strings for HLPD_DirectionalKeys_Schema.
 * @see HLPD_DirectionalKeys_Schema
 */
extern char *hlpd_directionalkeyschemas_strings[HLPD_DIRECTIONALKEYS_SCHEMAS_MAX + 1];

/**
 * Holds a set of HLPD_Key to use for cardinal direction movement.
 * @see HLPD_Key
 */
typedef struct HLPD_DirectionalKeys {
    HLPD_Key up;
    HLPD_Key right;
    HLPD_Key down;
    HLPD_Key left;
} HLPD_DirectionalKeys;

/**
 * Array with the default directional keys for all schemas defined as HLPD_DirectionalKeys_Schema.
 * @see HLPD_DirectionalKeys_Schema
 * @see HLPD_DirectionalKeys
 */
extern HLPD_DirectionalKeys hlpd_default_directional_keys[HLPD_DIRECTIONALKEYS_SCHEMAS_MAX+1];

/**
 * Holds options useful for user runtime preferences.
 */
typedef struct GameOptions {
    bool use_default_background; //<** Turn on usage of default terminal background */
    bool do_autosave; //<* Turns on autosave */
    HLPD_DirectionalKeys_Schema directional_keys_schema; //* Defines the current schema for cardinal directions movement */
} GameOptions;

extern const GameOptions default_GameOptions;

/**
 * Defines indexes for all types that are allocated with Koliseo.
 */
typedef enum HLP_Region_Type {
    HR_Path = KLS_REGIONTYPE_MAX + 100,	//Start counting from last index of Koliseo's own Region_Type values
    HR_Wincon,
    HR_Room,
    HR_Floor,
    HR_turnOP_args,
    HR_Fighter,
    HR_Enemy,
    HR_Boss,
    HR_FoeParty,
    HR_Equip,
    HR_Equipslot,
    HR_Specialslot,
    HR_Skillslot,
    HR_Turncounter,
    HR_Perk,
    HR_Consumable,
    HR_Artifact,
    HR_Chest,
    HR_Treasure,
    HR_Shop,
    HR_Roadfork,
    HR_Turncounter_desc,
    HR_Room_desc,
    HR_countStats,
    HR_Saveslot,
    HR_Gamestate,
    HR_Gamescreen,
    HR_GameOptions,
    HR_BSP_Room,
    HR_Notification,
    HR_RingaBuf,
    HR_loadInfo,
} HLP_Region_Type;

/**
 * Defines max index for an HLP_Region_Type value.
 */
#define HLP_MAX_INDEX (HR_loadInfo-100+KLS_REGIONTYPE_MAX)

/**
 * Array with the name strings for HLP_Region_Type values.
 * @see HLP_Region_Type
 */
extern char *hlp_regiontype_strings[HLP_MAX_INDEX + 1];

/**
 * Global variable for default Koliseo.
 */
extern Koliseo *default_kls;
/**
 * Global variable for temporary Koliseo.
 */
extern Koliseo *temporary_kls;
/**
 * Global variable for support Koliseo.
 */
extern Koliseo *support_kls;

/**
 * TODO Remove mentions of this.
 * Global variable for load animations flag. Legacy.
 */
extern int G_PRELOAD_ANIMATIONS_ON;
/**
 * Global variable for debug flag.
 */
extern int G_DEBUG_ON;
/**
 * Global variable for debug logging flag.
 */
extern int G_LOG_ON;
/**
 * Global variable for experimental features flag.
 */
extern int G_EXPERIMENTAL_ON;
/**
 * Global variable for fast quit flag.
 */
extern int G_FASTQUIT_ON;
/**
 * Global variable for godmode flag.
 */
extern int G_GODMODE_ON;
/**
 * Global variable for debug roomtype flag.
 */
extern int G_DEBUG_ROOMTYPE;
/**
 * Global variable for debug room flag.
 */
extern int G_DEBUG_ROOMTYPE_ON;
/**
 * Global variable for debug room flag, set to the passed string argument.
 */
extern char *G_DEBUG_ROOMTYPE_ARG;
/**
 * Global variable for debug enemy in ENEMIES roomtype flag.
 */
extern int G_DEBUG_ENEMYTYPE;
/**
 * Global variable for debug enemy flag.
 */
extern int G_DEBUG_ENEMYTYPE_ON;
/**
 * Global variable for debug enemy flag, set to the passed string argument.
 */
extern char *G_DEBUG_ENEMYTYPE_ARG;
/**
 * Global variable for autosave setting.
 */
extern int GS_AUTOSAVE_ON;
/**
 * Global variable for a tutorial launch.
 */
extern int G_DOTUTORIAL_ON;

/**
 * Global variable for using current directory to store game run files.
 */
extern int G_USE_CURRENTDIR;

/**
 * Global variable for using terminal default color.
 */
extern int G_USE_DEFAULT_BACKGROUND;

/**
 * Global variable for using vim-like directional keys.
 */
extern int G_USE_VIM_DIRECTIONAL_KEYS;

/**
 * Global variable for using WASD directional keys.
 */
extern int G_USE_WASD_DIRECTIONAL_KEYS;


/**
 * Global variable used to count advancements of the rng.
 */
extern int64_t G_RNG_ADVANCEMENTS;

/**
 * Global variable for seeded run flag.
 */
extern int G_SEEDED_RUN_ON;
/**
 * Global variable for seeded run flag, set to the passed string argument.
 */
extern char *G_SEEDED_RUN_ARG;

/**
 * Current major release.
 */
#define HELAPORDO_MAJOR_VERSION 1

/**
 * Current minor release.
 */
#define HELAPORDO_MINOR_VERSION 5

/**
 * Current patch release.
 */
#define HELAPORDO_PATCH_VERSION 0

/**
 * Current version string identifier, with MAJOR.MINOR.PATCH format.
 */
#define VERSION "1.5.0-dev"

#define HELAPORDO_BINSAVEFILE_VERSION "0.0.4"

/**
 * Defines current API version number from HELAPORDO_MAJOR_VERSION, HELAPORDO_MINOR_VERSION and HELAPORDO_PATCH_VERSION.
 */
static const int HELAPORDO_API_VERSION_INT =
    (HELAPORDO_MAJOR_VERSION * 1000000 + HELAPORDO_MINOR_VERSION * 10000 + HELAPORDO_PATCH_VERSION * 100);
/**< Represents current version with numeric format.*/

#define EXPECTED_NCURSES_VERSION_MAJOR 6 /**< Defines min expected major ncurses version.*/
#define EXPECTED_NCURSES_VERSION_MINOR 4 /**< Defines min expected minor ncurses version.*/
#define EXPECTED_NCURSES_VERSION_PATCH 20230520	/**< Defines min expected patch ncurses version.*/

/**
 * Holds arguments for a saveslot.
 * @see handleSave()
 * @see Path
 * @see randomise_path()
 */
typedef struct {
    char name[50];     /**< Name string for the saveslot.*/
    char save_path[255];     /**< Path to savefile*/
    int index; /**< Index of saveslot.*/
} Saveslot;

/**
 * Total number of Saveslot for the game.
 * @see Saveslot
 */
#define MAX_SAVESLOTS 3

/**
 * Array with the default saveslots for a game.
 * @see Saveslot
 * @see Path
 */
extern Saveslot default_saveslots[MAX_SAVESLOTS + 1];

/**
 * Ceiling for Fighter luck.
 * @see Fighter
 */
#define MAXPLAYERLUCK 30

/**
 * Ceiling for Path length.
 * @see Path
 */
#define MAXLENGTH 100

/**
 * Ceiling for Path luck.
 * @see Path
 */
#define MAXLUCK 50

/**
 * The different kinds of Gamemode.
 * Ordering is directly related to gamemodeStrings
 * @see gamemodeStrings
 */
typedef enum {
    Rogue = 0,
} Gamemode;

/** Maximum index of Gamemode, so that the size has to add 1 for the 0th index
 * @see roomClass
 */
#define GAMEMODE_MAX 0

/**
 * Array with the name strings for Gamemode.
 * @see Gamemode
 */
extern char *gamemodenamestrings[GAMEMODE_MAX + 1];

/** Maximum number of lores.
 * @see gameloop()
 * @see Gamemode
 */
#define LORES_MAX 2

/**
 * Global variable for gamemode flag.
 */
extern Gamemode GAMEMODE;

/**
 * The different kinds of Consumable.
 * Ordering is directly related to consumablestrings
 * @see consumablestrings
 * @see Consumable
 */
typedef enum {
    Potion = 0,
    Rock = 1,
    Bread = 2,
    Coffee = 3,
    Powergem = 4,
    Magicbean = 5
} consumableClass;

/** Maximum index of consumableClass, so that the size has to add 1 for the 0th index
 * @see consumableClass
 */
#define CONSUMABLESMAX 5

/**
 * Holds a certain qty of a consumableClass.
 * @see useConsumable()
 */
typedef struct {
    consumableClass class;     /**< Defines which kind of Consumable this is*/
    char name[25];     /**< Contains the name of the consumable*/
    char desc[50];     /**< Contains a brief description of the consumable*/
    int qty;	 /**< Indicates how many copies of the consumable the instance of this class holds*/
    char sprite[8][20];	    /**< Char matrix to hold the encoded sprite.*/
} Consumable;

/**
 * Array with the name strings for Consumable.
 * @see consumableClass
 * @see Consumable
 */
extern char *consumablestrings[CONSUMABLESMAX + 1];

/**
 * Array containing all Consumable instances.
 * All the Consumable->qty values are initialised to 0
 * @see Consumables
 * @see consumableClass
 */
extern Consumable consumablesBase[CONSUMABLESMAX + 1];

/**
 * The different kinds of artifactClass.
 * Ordering is directly related to artifactstrings.
 * There must always be exactly ARTIFACTSMAX +1 values.
 * @see artifactstrings
 * @see Artifact
 */
typedef enum {
    THKSKULL = 0,
    TWINKIE = 1,
    WRISTBAND = 2,
    BOARTAIL = 3,
    CHAOSORB = 4,
    POWERSYPHON = 5,
    GIANTFOSSILE = 6
} artifactClass;

/** Maximum index of artifactClass, so that the size has to add 1 for the 0th index
 * @see Equipzone
 */
#define ARTIFACTSMAX 6

/**
 * Represents the entity initialised from a artifactClass.
 * @see artifactClass
 */
typedef struct Artifact {
    artifactClass class;     /**< Defines which kind of artifactClass the instance relates to*/
    char name[25];     /**< Name string*/
    char desc[50];     /**< Description string*/
    int qty;	 /**< Indicates how many copies of the artifact the instance of this class holds*/
    int active;	    /**< Flag used to mark instance whose function pointer has been called already*/
    int innerValue;	/**< Indicates interal state of the item when needed*/
    char sprite[8][20];	    /**< Char matrix to hold the encoded sprite.*/
} Artifact;

/**
 * Array with the name strings for artifactClass.
 * @see artifactClass
 */
extern char *artifactstrings[ARTIFACTSMAX + 1];

/**
 * Array with all the Artifact found.
 * @see Artifact
 */
extern Artifact artifactsBase[ARTIFACTSMAX + 1];

#define ENEMY_ARTIFACTDROP_CHANCE 1001 /**< Defines rare Artifact drop chance for normal enemies (e->beast == 0)*/

/**
 * The different kinds of fighterStatus.
 */
typedef enum {
    Normal,
    Poison,
    Frozen,
    Burned,
    Weak,
    Strong
} fighterStatus;

/** Maximum index of fighterStatus, so that the size has to add 1 for the 0th index
 * @see fighterStatus
 */
#define STATUSMAX 5

/**
 * The different kinds of Fighter.
 * Ordering is directly related to classesstrings
 * @see classesstrings
 * @see Fighter
 */
typedef enum {
    Knight = 0,
    Archer = 1,
    Mage = 2,
    Assassin = 3
} fighterClass;

/** Maximum index of fighterClass, so that the size has to add 1 for the 0th index
 * @see fighterClass
 */
#define CLASSESMAX 3

/**
 * Array with the name strings for fighterClass.
 * @see fighterClass
 */
extern char *classesstrings[CLASSESMAX + 1];

/**
 * The different kinds of Stat.
 */
typedef enum {
    ATK,
    DEF,
    VEL,
    ENR
} Stat;

/** Maximum index of Stat, so that the size has to add 1 for the 0th index
 * @see Stat
 */
#define STATMAX 3

/**
 * Holds the base stats for a fighterClass.
 * @see Fighter
 * @see fighterClass
 * @see initStats()
 */
typedef struct BaseStats {
    fighterClass class;	    /**< Defines which kind of fighterClass the stats relate to*/
    int hp;	/**< Hp value*/
    int atk;	 /**< Atk value*/
    int def;	 /**< Def value*/
    int vel;	 /**< Vel value*/
    int level;	   /**< Level value*/
    int totalxp;     /**< Lifetime xp value*/
    int totallevelxp;	  /**< Xp value needed to level up from current level*/
    int currentlevelxp;	    /**< Total xp value for current level*/
    int totalhp;     /**< Total hp value*/
    int totalenergy;	 /**< Total energy value*/
    int totalstamina;	  /**< Total stamina value*/
} BaseStats;

/**
 * Array containing all BaseStats instances.
 * All the classes get their starting stats from here.
 * Ordering of parameters is: hp, atk, def, vel, level, totalxp (0), totallevelxp (1), currentlevelxp (0), totalhp, totalenergy
 * @see Fighter
 * @see fighterClass
 * @see initStats()
 */
extern BaseStats basestats[CLASSESMAX + 1];

/**
 * The different kinds of Enemy.
 * Ordering is directly related to classenemystrings
 * @see classenemystrings
 * @see Enemy
 */
typedef enum {
    Mummy = 0,
    Ghost = 1,
    Zombie = 2,
    Goblin = 3,
    Imp = 4,
    Troll = 5,
    Boar = 6,
    Werewolf = 7
} enemyClass;

/** Maximum index of enemyClass, so that the size has to add 1 for the 0th index
 * @see enemyClass
 */
#define ENEMYCLASSESMAX 7

/**
 * Array with the name strings for enemyClass.
 * @see enemyClass
 * @see printStringFromEClass()
 */
extern char *classenemystrings[ENEMYCLASSESMAX + 1];

/**
 * Holds the base stats for a enemyClass.
 * @see Enemy
 * @see enemyClass
 * @see initEnemyStats()
 */
typedef struct EnemyBaseStats {
    enemyClass class;	  /**< Defines which kind of enemyClass the stats relate to*/
    int hp;	/**< Hp value*/
    int atk;	 /**< Atk value*/
    int def;	 /**< Def value*/
    int vel;	 /**< Vel value*/
    int level;	   /**< Level value*/
    int xp;	/**< Xp value given on death */
    int totalhp;     /**< Total hp value*/
    int beast;	   /**< Flag used for "beast" enemies*/
    int totalenergy;	/**< Total energy value*/
    int totalstamina;	 /**< Total stamina value*/
} EnemyBaseStats;

/**
 * Array containing all EnemyBaseStats instances.
 * All the classes get their starting stats from here.
 * Ordering of parameters is: hp, atk, def, vel, level, xp, totalhp, beast(0), totalenergy
 * @see Enemy
 * @see enemyClass
 * @see initEnemyStats()
 */
extern EnemyBaseStats baseenemystats[ENEMYCLASSESMAX + 1];

/**
 * The different kinds of Boss.
 * Ordering is directly related to classbossstrings
 * @see classbossstrings
 * @see Boss
 */
typedef enum {
    Blue_Troll = 0,
    Headless_Ninja = 1,
    Crawling_Dude = 2,
    Sr_Warthog = 3,
    Doppelganger = 4
} bossClass;

/** Maximum index of bossClass, so that the size has to add 1 for the 0th index
 * @see bossClass
 */
#define BOSSCLASSESMAX 4

/**
 * Array with the name strings for bossClass.
 * @see bossClass
 * @see printStringFromBossClass()
 */
extern char *classbossstrings[BOSSCLASSESMAX + 1];

/**
 * Holds the base stats for a bossClass.
 * @see Boss
 * @see bossClass
 * @see initBossStats()
 */
typedef struct BossBaseStats {
    bossClass class;	 /**< Defines which kind of bossClass the stats relate to*/
    int hp;	/**< Hp value*/
    int atk;	 /**< Atk value*/
    int def;	 /**< Def value*/
    int vel;	 /**< Vel value*/
    int level;	   /**< Level value*/
    int xp;	/**< Xp value given on death */
    int totalhp;     /**< Total hp value*/
    int beast;	   /**< Flag used for "beast" enemies*/
    int totalenergy;	/**< Total energy value*/
    int totalstamina;	 /**< Total stamina value*/
} BossBaseStats;

/**
 * Array containing all BossBaseStats instances.
 * All the classes get their starting stats from here.
 * Ordering of parameters is: hp, atk, def, vel, level, xp, totalhp, beast(0), totalenergy
 * @see Boss
 * @see bossClass
 * @see initBossStats()
 */
extern BossBaseStats basebossstats[BOSSCLASSESMAX + 1];

/**
 * Holds the lifetime stats of the player.
 * @see Fighter
 */
typedef struct {
    int enemieskilled;	   /**< How many enemies were defeated*/
    int consumablesfound;     /**< How many consumabls were found*/
    int equipsfound;	 /**< How many equips were found*/
    int artifactsfound;	    /**< How many artifacts were found*/
    int criticalhits;	  /**< How many criticals were dealt*/
    int roomscompleted;	    /**< How many rooms were completed*/
    int floorscompleted;     /**< How many floors were completed*/
    int specialsunlocked;     /**< How many special move were unlocked*/
    int coinsfound;	/**< How many coins you found in total*/
    int bosseskilled;	  /**< How many bosses were defeated*/
    int unique_bosseskilled;	 /**< How many unique bosses were defeated*/
    int killed_bosses[BOSSCLASSESMAX + 1];   /**< Keeps track of which bossClass you've killed.*/
    int keysfound;     /**< How many keys you found in total*/
} countStats;

/**
 * The different kinds of special moves.
 * Ordering is directly related to specialsstrings and specialscosts.
 * There must always be exactly SPECIALSMAX +1 values for each fighterClass.
 * @see specialsstrings
 * @see specialscosts
 */
typedef enum {
    KSlash = 0,
    KCover = 1,
    KArmordrop = 2,
    KBerserk = 3,
    AHeadshot = 4,
    AQuivercheck = 5,
    APoisonshot = 6,
    AFireshot = 7,
    MFatewarp = 8,
    MPowerup = 9,
    MSpellstrike = 10,
    MFlamering = 11,
    XGrimdagger = 12,
    XLeechknife = 13,
    XDisguise = 14,
    XVenomblade = 15
} specialMove;

/** Maximum index of specialMove per class, so that the size for each Fighter has to add 1 for the 0th index
 * @see specialMove
 */
#define SPECIALSMAX 3		//num of specials per class (0 inclusive)

/**
 * Array with the name strings for special moves.
 * @see specialMove
 */
extern char *specialsnamestrings[CLASSESMAX + 1][SPECIALSMAX + 1];

/**
 * Array with the desc strings for special moves.
 * @see specialMove
 */
extern char *specialsdescstrings[CLASSESMAX + 1][SPECIALSMAX + 1];

/**
 * Array with the cost integer values for special moves.
 * @see specialMove
 */
extern int specialscosts[CLASSESMAX + 1][SPECIALSMAX + 1];

/**
 * Holds state for a Fighter specials.
 * @see Fighter
 * @see specialMove
 */
typedef struct {
    int enabled;     /**< Flag defining if the current slot is initialised*/
    specialMove move;	  /**< Defines which kind of specialMove the slot is holding*/
    char name[80];     /**< Name string*/
    char desc[80];     /**< Desc string*/
    int cost;	  /**< Cost of use*/
} Specialslot;

typedef enum {
    SKILL_TYPE_ATKBOOST=0,
    SKILL_TYPE_LAST_UNLOCKABLE,
    SKILL_TYPE_IDLE,
    SKILL_TYPE_MAX,
} skillType;

#define SKILLSTOTAL SKILL_TYPE_MAX

#define FIGHTER_SKILL_SLOTS SKILLSTOTAL
#define ENEMY_SKILL_SLOTS SKILLSTOTAL
#define BOSS_SKILL_SLOTS SKILLSTOTAL

/**
 * Array with the name strings for skills.
 * @see skillType
 */
extern char *skillsnamestrings[SKILLSTOTAL + 1];

/**
 * Array with the desc strings for skills.
 * @see skillType
 */
extern char *skillsdescstrings[SKILLSTOTAL + 1];

/**
 * Array with the cost integer values for skills.
 * @see skillType
 */
extern int skillscosts[SKILLSTOTAL + 1];

/**
 * Holds state for a skillType.
 * @see Fighter
 * @see Enemy
 * @see Boss
 * @see skillType
 */
typedef struct {
    int enabled;     /**< Flag defining if the current slot is initialised*/
    skillType class;	  /**< Defines which kind of skillType the slot is holding*/
    char name[80];     /**< Name string*/
    char desc[80];     /**< Desc string*/
    int cost;	  /**< Cost of use*/
} Skillslot;

//Forward declarations for counter fields in entities
struct Turncounter;

/**
 * The different indexes for Fighter->counters.
 * Ordering is directly related to Fighter->counters.
 * There must always be exactly COUNTERSMAX +1 values.
 * @see Turncounter
 * @see Fighter
 */
typedef enum {
    NORMAL = 0,
    POISON = 1,
    BURNED = 2,
    FROZEN = 3,
    WEAK = 4,
    STRONG = 5,
    TURNBOOST_ATK = 6,
    TURNBOOST_DEF = 7,
    TURNBOOST_VEL = 8,
    TURNBOOST_ENR = 9
} counterIndexes;
//Number of counters per entity

/** Maximum index of counterIndexes, so that the size has to add 1 for the 0th index
 * @see counterIndexes
 */
#define COUNTERSMAX 9

//Forward declarations for equipslot fields in entities
struct Equipslot;
struct Equip;

/**
 * The different kinds of equip zones.
 * Ordering is directly related to equipzonestrings.
 * There must always be exactly EQUIPZONES +1 values.
 * @see equipzonestrings
 */
typedef enum {
    HEAD = 0,
    TORSO = 1,
    LEGS = 2
} Equipzone;

/** Maximum index of Equipzone, so that the size has to add 1 for the 0th index
 * @see Equipzone
 */
#define EQUIPZONES 2

/**
 * Array with the name strings for equip zones.
 * @see Equipzone
 */
extern char *equipzonestrings[EQUIPZONES + 1];

/* Ceiling for Fighter equipsBag indexes.
 * @see Fighter
 */
#define EQUIPSBAGSIZE 20

/**
 * The different kinds of perks.
 * Ordering is directly related to perksstrings.
 * There must always be exactly PERKSMAX +1 values.
 * @see perksstrings
 */
typedef enum {
    CRITBOOST_DMG = 0,
    CRITBOOST_CHANCE = 1,
    VAMPIRISM = 2,
    RUNIC_MAGNET = 3,
    HARVESTER = 4,
    HOT_BODY = 5,
    BIOHAZARD = 6,
    ORACLE_GIFT = 7,
    PENICILLIN = 8
} perkClass;

/** Maximum index of perkClass, so that the size has to add 1 for the 0th index
 * @see perkClass
 */
#define PERKSMAX 8

/** Maximum number of Perk per Equip
 * @see Perk
 * @see Equip
 */
#define EQUIPPERKSMAX 2

/**
 * Array with the name strings for perkClass.
 * @see perkClass
 */
extern char *perksnamestrings[PERKSMAX + 1];

/**
 * Array with the desc strings for perkClass.
 * @see perkClass
 */
extern char *perksdescstrings[PERKSMAX + 1];

/**
 * Represents the entity initialised from a perkClass.
 * @see perkClass
 */
typedef struct {
    perkClass class;	 /**< Defines which kind of perk the instance relates to*/
    char name[25];     /**< Name string*/
    char desc[50];     /**< Description string*/
    int innerValue;	/**< Contains an integer for perks that need a state*/
} Perk;

//Macros for special moves callback
struct Fighter;
struct Enemy;
struct Boss;
struct Path;
struct FoeParty;

//Macros for counters callback

//#define REGISTER_COUNTER_CALLBACK(c,x,f) register_counter_callback(c, (callback_void_t)x,f)
//#define GET_COUNTER_CALLBACK(c,C) ((C)callback_counter_ptrs[c])

//Macro to get size of an array
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

/**
 * Defines a function pointer returning void and taking a Fighter pointer.
 * @see Fighter
 * @see Turncounter
 */
typedef void (*callback_turncounter_fighter_t)(struct Fighter *);

/**
 * Defines a function pointer returning void and taking a Enemy pointer.
 * @see Enemy
 * @see Turncounter
 */
typedef void (*callback_turncounter_enemy_t)(struct Enemy *);

/**
 * Defines a function pointer returning void and taking a WINDOW, a Fighter, a Enemy, a Boss and a Path pointers, along with three ints.
 * @see Fighter
 * @see Enemy
 * @see Path
 * @see SpecialSlot
 */
#ifdef HELAPORDO_CURSES_BUILD
typedef void (*callback_special_t)(WINDOW *, struct Fighter *, struct Enemy *,
                                   struct Boss *, struct Path *, int, int, int);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined."
#else
typedef void (*callback_special_t)(Rectangle* r, struct Fighter *, struct Enemy *,
                                   struct Boss *, struct Path *, int, int, int);
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

/**
 * Defines a function pointer returning void and taking a Fighter, Enemy and Boss pointers; plus as int.
 * @see Fighter
 * @see Enemy
 * @see Boss
 * @see Artifact
 */
typedef void (*callback_artifact_t)(struct Fighter *, struct Enemy *,
                                    struct Boss *, int isBoss);

/**
 * Defines a function pointer returning void and taking NO parameters (different than saying just (), unspecified parameters).
 */
typedef void (*callback_void_t)(void);

#define FIGHTER_NAME_BUFSIZE 50

/**
 * Represents the entity initialised from a fighterClass.
 * @see fighterClass
 * @see fighterStatus
 * @see initStats()
 * @see Specialslot
 * @see Turncounter
 * @see Equipslot
 * @see countStats
 */
typedef struct Fighter {
    char name[FIGHTER_NAME_BUFSIZE];     /**< Name string*/
    fighterClass class;	    /**< Defines which kind of fighterClass the instance relates to*/
    int hp;	/**< Current hp value*/
    int atk;	 /**< Current atk value*/
    int def;	 /**< Current def value*/
    int vel;	 /**< Current vel value*/
    int level;	   /**< Current level value*/
    int luck;	  /**< Current luck value*/
    int totalxp;     /**< Lifetime xp value*/
    int currentlevelxp;	    /**< Xp gained for the current level*/
    int totallevelxp;	  /**< Xp needed to level up*/
    int totalhp;     /**< Full hp value*/
    fighterStatus status;     /**< Defines active fighterStatus*/

    int energy;	    /**< Current energy value*/
    int totalenergy;	 /**< Full energy value*/
    int stamina;     /**< Current stamina value*/
    int totalstamina;	  /**< Full stamina value*/
    Specialslot *specials[SPECIALSMAX + 1];   /**< Array with all the Specialslot*/
    Skillslot *skills[FIGHTER_SKILL_SLOTS + 1];   /**< Array with all the Skillslot*/

    struct Turncounter *counters[COUNTERSMAX + 1];   /**< Array with all the Turncounter pointers*/
    int turnboost_atk;	   /**< Current temp boost value for atk*/
    int turnboost_def;	   /**< Current temp boost value for def*/
    int turnboost_vel;	   /**< Current temp boost value for vel*/
    int turnboost_enr;	   /**< Current temp boost value for enr*/

    int perksCount;	/**< Keeps track of how many perks are active.*/
    Perk *perks[PERKSMAX + 1];	 /**< Array with all the Perk*/

    struct Equipslot *equipslots[EQUIPZONES + 1];   /**< Array with all the Equipslot*/
    struct Equip *equipsBag[EQUIPSBAGSIZE + 1];	  /**< Array with all the Equip found*/
    Consumable *consumablesBag[CONSUMABLESMAX + 1];   /**< Array with all the Consumables found*/
    struct Artifact *artifactsBag[ARTIFACTSMAX + 1];   /**< Array with all the Artifacts found*/

    int equipsBagOccupiedSlots;	    /**< Keeps track of how many slots are occupied.*/
    int earliestBagSlot;	/**< Keeps track of first available spot.*///TODO To always use the array efficiently (???) I sense linked lists may be better

    int permboost_atk;	   /**< Current temp boost value for atk.*/
    int permboost_def;	   /**< Current temp boost value for def.*/
    int permboost_vel;	   /**< Current temp boost value for vel.*/
    int permboost_enr;	   /**< Current temp boost value for enr.*/

    int equipboost_atk;	    /**< Current equip boost value for atk.*/
    int equipboost_def;	    /**< Current equip boost value for def.*/
    int equipboost_vel;	    /**< Current equip boost value for vel.*/
    int equipboost_enr;	    /**< Current equip boost value for enr.*/

    countStats *stats;	    /**< Pointer to countStats instance*/

    int balance;     /**< Amount of currency owned*/
    int keys_balance;	  /**< Amount of keys owned*/
    char sprite[8][20];	    /**< Char matrix to hold the encoded sprite.*/

    int floor_x; /**< Current x position in floor.*/
    int floor_y; /**< Current y position in floor.*/

    //TODO eval this callback bit if we need to have some without nesting in another turncounter
    //callback_void_t callback_counter_ptrs[COUNTERSMAX]; /**< Array of callbacks for turnCounter functions.*/
} Fighter;

/**
 * Represents the entity initialised from a enemyClass.
 * @see enemyClass
 * @see fighterStatus
 * @see initEnemyStats()
 * @see Turncounter
 * @see counterIndexes
 */
typedef struct Enemy {
    int index;	   /**< Defines position inside containing foeParty.*/
    enemyClass class;	  /**< Defines which kind of enemyClass the instance relates to*/
    int hp;	/**< Current hp value*/
    int atk;	 /**< Current atk value*/
    int def;	 /**< Current def value*/
    int vel;	 /**< Current vel value*/
    int level;	   /**< Level value*/
    int luck;	  /**< Luck value*/
    int xp;	/**< Xp value given on death*/
    int totalhp;     /**< Full hp value*/

    int energy;	    /**< Current energy value*/
    int totalenergy;	 /**< Full energy value*/

    int stamina;     /**< Current stamina value*/
    int totalstamina;	  /**< Full stamina value*/

    fighterStatus status;     /**< Defines active fighterStatus*/
    int beast;	   /**< Flag defining the instance as "beast" if set*/

    Skillslot *skills[ENEMY_SKILL_SLOTS + 1];   /**< Array with all the Skillslot*/

    struct Turncounter *counters[COUNTERSMAX + 1];   /**< Array with all the Turncounter pointers*/
    int turnboost_atk;	   /**< Current temp boost value for atk*/
    int turnboost_def;	   /**< Current temp boost value for def*/
    int turnboost_vel;	   /**< Current temp boost value for vel*/
    int turnboost_enr;	   /**< Current temp boost value for enr*/

    int prize;	   /**< Amount of currency dropped on defeat*/
    char sprite[8][20];	    /**< Char matrix to hold the encoded sprite.*/
} Enemy;

/**
 * Represents the entity initialised from a bossClass.
 * @see bossClass
 * @see fighterStatus
 * @see initBossStats()
 * @see Turncounter
 * @see counterIndexes
 */
typedef struct Boss {
    bossClass class;	 /**< Defines which kind of enemyClass the instance relates to*/
    int hp;	/**< Current hp value*/
    int atk;	 /**< Current atk value*/
    int def;	 /**< Current def value*/
    int vel;	 /**< Current vel value*/
    int level;	   /**< Level value*/
    int luck;	  /**< Luck value*/
    int xp;	/**< Xp value given on death*/
    int totalhp;     /**< Full hp value*/

    int energy;	    /**< Current energy value*/
    int totalenergy;	 /**< Full energy value*/

    int stamina;     /**< Current stamina value*/
    int totalstamina;	  /**< Full stamina value*/

    fighterStatus status;     /**< Defines active fighterStatus*/
    int beast;	   /**< Flag defining the instance as "beast" if set*/

    Skillslot *skills[BOSS_SKILL_SLOTS + 1];   /**< Array with all the Skillslot*/
    struct Turncounter *counters[COUNTERSMAX + 1];   /**< Array with all the Turncounter pointers*/
    int turnboost_atk;	   /**< Current temp boost value for atk*/
    int turnboost_def;	   /**< Current temp boost value for def*/
    int turnboost_vel;	   /**< Current temp boost value for vel*/
    int turnboost_enr;	   /**< Current temp boost value for enr*/

    int prize;	   /**< Amount of currency dropped on defeat*/
    char sprite[8][20];	    /**< Char matrix to hold the encoded sprite.*/
} Boss;

/**
 * Defines a function pointer returning void and taking a Fighter pointer.
 * @see Fighter
 * @see Turncounter
 */
typedef void (*effect_fun)(Fighter *);
/**
 * Defines a function pointer returning void and taking a Enemy pointer.
 * @see Enemy
 * @see Turncounter
 */
typedef void (*effect_e_fun)(Enemy *);
/**
 * Defines a function pointer returning void and taking a Boss pointer.
 * @see Boss
 * @see Turncounter
 */
typedef void (*effect_b_fun)(Boss *);
/**
 * Defines a function pointer returning void and taking a FoeParty pointer.
 * @see FoeParty
 * @see Turncounter
 */
typedef void (*effect_fp_fun)(struct FoeParty *);

/**
 * Defines a function pointer returning void and taking a Fighter pointer and an integer.
 * @see Fighter
 * @see Turncounter
 */
typedef void (*boost_fun)(Fighter *, int boost);
/**
 * Defines a function pointer returning void and taking a Enemy pointer and an integer.
 * @see Enemy
 * @see Turncounter
 */
typedef void (*boost_e_fun)(Enemy *, int boost);
/**
 * Defines a function pointer returning void and taking a Boss pointer and an integer.
 * @see Boss
 * @see Turncounter
 */
typedef void (*boost_b_fun)(Boss *, int boost);
/**
 * Defines a function pointer returning void and taking a FoeParty pointer and an integer.
 * @see FoeParty
 * @see Turncounter
 */
typedef void (*boost_fp_fun)(struct FoeParty *, int boost);

/**
 * The different kinds of Countertype.
 * @see Turncounter
 */
typedef enum {
    CNT_STATUS,	    /**< Counter for a fighterStatus.*/
    CNT_ATKBOOST,     /**< Counter for a temp boost on atk.*/
    CNT_DEFBOOST,      /**< Counter for a temp boost on def.*/
    CNT_VELBOOST,     /**< Counter for a temp boost on vel.*/
    CNT_ENRBOOST      /**< Counter for a temp boost on enr.*/
} Countertype;

/**
 * Represents the entity initialised from a CounterType.
 * @see Countertype
 * @see Fighter
 * @see Enemy
 */
typedef struct Turncounter {
    int count;	   /**< Defines the current count.*/
    char *desc;	    /**< Description string*/
    int innerValue;	/**< Contains an integer for counters that need a state*/
    Countertype type;	  /**< Defines which kind of Countertype the instance relates to*/
    void (*effect_fun)(Fighter *);     /**< Pointer to effect function applying to Fighter*/
    void (*effect_e_fun)(Enemy *);     /**< Pointer to effect function applying to Enemy*/
    void (*effect_b_fun)(Boss *);     /**< Pointer to effect function applying to Boss*/
    void (*effect_fp_fun)(struct FoeParty *);	  /**< Pointer to effect function applying to FoeParty*/
    void (*boost_fun)(Fighter *, int);	   /**< Pointer to temp boost function applying to Fighter, takes a int*/
    void (*boost_e_fun)(Enemy *, int);	   /**< Pointer to temp boost function applying to Enemy, takes a int*/
    void (*boost_b_fun)(Boss *, int);	  /**< Pointer to temp boost function applying to Boss, takes a int*/
    void (*boost_fp_fun)(struct FoeParty *, int);     /**< Pointer to temp boost function applying to FoeParty, takes a int*/
} Turncounter;

/**
 * The different kinds of Wincon.
 * Ordering is directly related to winconstrings.
 * There must always be exactly WINCON_CLASS_MAX +1 values.
 * @see Wincon
 */
typedef enum {
    ALL_ARTIFACTS = 0,	 /**< Collect all the artifacts*/
    FULL_PATH = 1,   /**< Finish all the room in your path.*/
    ALL_BOSSES = 2    /**< Kill all the bosses once.*/
} winconClass;

/** Maximum index of winconClass, so that the size has to add 1 for the 0th index
 * @see winconClass
 */
#define WINCON_CLASS_MAX 2

/**
 * Defines the entity initialised from a winconClass.
 */
typedef struct Wincon {
    winconClass class;	   /**< Defines which kind of winconClass the instance relates to.*/
    int current_val;	 /**< Defines the current progress.*/
    int target_val;	/**< Defines the total progress.*/
} Wincon;

/**
 * Array with the desc strings for winconClass.
 * @see winconClass
 */
extern char *winconstrings[WINCON_CLASS_MAX + 1];

#define PATH_SEED_BUFSIZE 20

/**
 * Holds the state of game progression.
 * @see Wincon
 * @see Saveslot
 */
typedef struct Path {
    int length;	    /**< Defines how many rooms there are in total.*/
    int luck;	  /**< Defines global luck value.*/
    int prize;	   /**< Defines the reward for getting to length*/
    Wincon *win_condition;     /**< Defines the win condition for the current game.*/
    Saveslot *current_saveslot;	    /**< Defines current Saveslot for the game.*/
    char seed[PATH_SEED_BUFSIZE+1]; /**< Contains seed for current run.*/
    int64_t* rng_advancements; /**< Pointer to current advancements for rng.*/
} Path;

/**
 * The different kinds of Equip.
 * Ordering is directly related to equipstring.
 * There must always be exactly EQUIPSMAX +1 values.
 * @see equipstrings
 */
typedef enum {
    Vest = 0,
    Helmet = 1,
    Boots = 2,
    Cape = 3,
    Tophat = 4,
    Loafers = 5
} equipClass;

/** Maximum index of equipClass, so that the size has to add 1 for the 0th index
 * @see equipClass
 */
#define EQUIPSMAX 5

/**
 * The different grades of Equip.
 * Ordering is directly related to qualitytrings.
 * There must always be exactly QUALITIESMAX +1 values.
 * @see qualitytrings
 */
typedef enum {
    Bad = 0,
    Average = 1,
    Good = 2,
} quality;

/** Maximum index of quality, so that the size has to add 1 for the 0th index
 * @see quality
 */
#define QUALITIESMAX 2

/**
 * Array with the name strings for quality.
 * @see quality
 */
extern char *qualitytrings[QUALITIESMAX + 1];

/**
 * Represents the entity initialised from a equipClass.
 * @see equipClass
 * @see Equipzone
 * @see quality
 */
typedef struct Equip {
    equipClass class;	  /**< Defines which kind of equipClass the instance relates to*/
    Equipzone type;	/**< Defines which kind of Equipzone the instance relates to*/
    char name[25];     /**< Name string*/
    char desc[50];     /**< Description string*/
    int qty;	 /**< Defines how many copies of the equipClass the instance holds. Should be <=1, >=0 */

    int equipped;     /**< Flag indicating the instance as currently in use if set.*/

    int level;	   /**< Level value*/
    //Modifiers
    //  modifier
    int atk;	 /**< equip boost atk value*/
    int def;	 /**< equip boost def value*/
    int vel;	 /**< equip boost vel value*/
    int enr;	 /**< equip boost enr value*/

    int bonus;	   /**< Defines how many pointers to bonus functions the instance holds.*/
    int perksCount;	/**< Defines how many Perk pointers the instance holds.*/

    int cost;	  /**< Helps giving a coin value when selling or buying the equip.*/

    quality qual;     /**< Defines which kind of quality the instances relates to*/
    void (*equip_fun)(Fighter *, Enemy *, Path *);/**< Pointer to temp boost function applying to Enemy, takes a int*/
    Perk *perks[EQUIPPERKSMAX];	    /**< Array of Perk of the instance*/
    char sprite[8][20];	    /**< Char matrix to hold the encoded sprite.*/
} Equip;

/**
 * Array with the name strings for equipClass.
 * @see equipClass
 */
extern char *equipstrings[EQUIPSMAX + 1];

/**
 * Array containing all Equip instances.
 * All the Equip->qty values are initialised to 0
 * @see Equip
 * @see equipClass
 */
extern Equip equips[EQUIPSMAX + 1];

/**
 * Defines a function pointer returning void and taking a Fighter and a Enemy pointers.
 * @see Equip
 * @see Fighter
 * @see equipClass
 */
typedef void (*equip_fun)(Fighter *, Enemy *);

/**
 * Represents the entity containing an Equip instance.
 * @see Equip
 * @see Equipzone
 */
typedef struct Equipslot {
    int active;	    /**< Flag indicating the instance as currently ready to use if set.*/
    char *desc;	    /**< Description string.*/
    Equipzone type;	/**< Defines which kind of Equipzone the instance relates to*/
    char sprite[8][20];	    /**< Char matrix to hold the encoded sprite.*/
    Equip *item;     /**< Pointer to the Equip instance associated*/
    //void (*equip_fun)( Fighter*,Enemy*); /**< Pointer to temp boost function applying to Enemy, takes a int
} Equipslot;

/** Maximum number of total items sold in a shop. Must be at least equal to CONSUMABLE_SHOP_MAX + EQUIP_SHOP_MAX.
 * @see Shop
 */
#define ITEM_SHOP_MAX 20

/** Maximum number of equips sold in a shop
 * @see Shop
 */
#define EQUIP_SHOP_MAX 2

/** Maximum number of total consumables sold in a shop
 * @see Shop
 */
#define CONSUMABLE_SHOP_MAX 3

/** Maximum number of single consumables sold in a shop
 * @see Shop
 */
#define SINGLE_CONSUMABLE_SHOP_MAX 3

/**
 * Represents a shop instance.
 */
typedef struct Shop {

    char *desc;	    /**< Description string.*/
    int itemCount;     /**< Total number of items for sale*/

    int equipsCount;	 /**< Total number of equips for sale*/
    Equip *equips[EQUIP_SHOP_MAX];     /**< Array for equips on sale*/
    int equipPrices[EQUIP_SHOP_MAX];	 /**< Array for equips prices*/

    int consumablesCount;     /**< Total number of consumables for sale, includes quantity for each kind of consumable*/
    int uniqueConsumablesCount;	    /**< Total of unique consumables for sale*/
    Consumable *consumables[CONSUMABLE_SHOP_MAX];     /**< Array for consumables on sale*/
    int consumablePrices[CONSUMABLE_SHOP_MAX];	   /**< Array for consumables prices. They assume to a consumable with qty == 1*/

} Shop;

/**
 * The different kinds of Chest.
 * Ordering is directly related to chestnamestrings.
 * There must always be exactly CHEST_CLASS_MAX +1 values.
 * @see CHEST_CLASS_MAX
 */
typedef enum {
    CHEST_BASE = 0,
    CHEST_BEAST = 1
} chestClass;

/** Maximum index of chestClass, so that the size has to add 1 for the 0th index
 * @see chestClass
 */
#define CHEST_CLASS_MAX 1

/** Maximum number of Consumable held in a chest.
 * @see Chest
 */
#define CHEST_CONSUMABLES_MAX 5
/** Maximum number of Equip held in a chest.
 * @see Chest
 */
#define CHEST_EQUIPS_MAX 5

/**
 * Array with the desc strings for chestClass.
 * @see chestClass
 */
extern char *chestdescstrings[CHEST_CLASS_MAX + 1];

/**
 * Represents the entity initialised from a chestClass.
 * @see chestClass
 * @see Chest
 * @see Consumable
 * @see Equip
 */
typedef struct Chest {

    char desc[80];     /**< Description string.*/

    chestClass class;	  /**< Defines which kind of chesteClass the instance relates to*/
    int consumablesCount;     /**< Defines how many consumables the chest contains.*/

    Consumable *consumables[CHEST_CONSUMABLES_MAX];	/**< The contained consumables.*/
    int equipsCount;	 /**< Defines how many equips the chest contains.*/

    Equip *equips[CHEST_EQUIPS_MAX];	 /**< The contained equips.*/

    char sprite[8][20];	    /**< Char matrix to hold the encoded sprite.*/
} Chest;

/**
 * The different kinds of Treasure.
 * Ordering is directly related to treasuredescstrings.
 * There must always be exactly TREASURE_CLASS_MAX +1 values.
 * @see TREASURE_CLASS_MAX
 */
typedef enum {
    TREASURE_CHEST = 0,
    TREASURE_CONSUMABLE = 1,
    TREASURE_ARTIFACT = 2
} treasureClass;

/** Maximum index of treasureClass, so that the size has to add 1 for the 0th index
 * @see treasureClass
 */
#define TREASURE_CLASS_MAX 3

/**
 * Array with the name strings for treasureClass.
 * @see treasureClass
 */
extern char *treasuredescstrings[TREASURE_CLASS_MAX + 1];

/**
 * Represents the entity initialised from a treasureClass.
 * @see treasureClass
 * @see Chest
 * @see Consumable
 * @see Equip
 * @see Artifact
 */
typedef struct Treasure {

    char desc[80];     /**< Description string.*/

    treasureClass class;     /**< Defines which kind of treasureClass the instance relates to*/

    Chest *chest;     /**< The treasure Chest, initialised if class == TREASURE_CHEST*/
    Consumable *consumable;	/**< The treasure Consumable, initialised if class == TREASURE_CONSUMABLE*/
    Artifact *artifact;	    /**< The treasure Artifact, initialised if class == TREASURE_ARTIFACT*/

} Treasure;

/**
 * The different kinds of Roadfork.
 * Ordering is directly related to roadforknamestrings.
 * There must always be exactly ROADFORK_CLASS_MAX +1 values.
 * @see ROADFORK_CLASS_MAX
 */
typedef enum {
    ROADFORK_BOSS = 0,
    ROADFORK_SHOP = 1,
    ROADFORK_TREASURE = 2,
    ROADFORK_ENEMIES = 3,
    ROADFORK_ROADFORK = 4
} roadforkOption;

/** Maximum index of roadforkOption, so that the size has to add 1 for the 0th index
 * @see roadforkClass
 */
#define ROADFORK_OPTIONS_MAX 4

/**
 * Array with the name strings for roadforkClass.
 * @see roadforkClass
 */
extern char *roadforkoptionstrings[ROADFORK_OPTIONS_MAX + 1];

/**
 * Represents the entity initialised from a roadforkClass.
 * @see roadforkClass
 * @see Boss
 * @see Shop
 * @see Treasure
 */
typedef struct Roadfork {

    char *desc;	    /**< Description string.*/

    roadforkOption options[2];	   /**< Defines which choices the fork has.*/

} Roadfork;

/**
 * The different kinds of Room.
 * Ordering is directly related to roomnamestrings.
 * There must always be exactly ROOM_CLASS_MAX +1 values.
 * @see roomnamestrings
 */
typedef enum {
    ENEMIES = 0,
    SHOP = 1,
    BOSS = 2,
    TREASURE = 3,
    ROADFORK = 4,
    HOME = 5,
    WALL = 6,
    BASIC = 7
} roomClass;

/** Maximum index of roomClass, so that the size has to add 1 for the 0th index
 * @see roomClass
 */
#define ROOM_CLASS_MAX 7

/**
 * Array with the name strings for roomClass.
 * @see roomClass
 */
extern char *roomnamestrings[ROOM_CLASS_MAX + 1];

/** Maximum number of enemies in a room.
 * @see initRoom_Enemies()
 */
#define ROOM_ENEMIES_MAX 4

/** Maximum number of bosses when in a FoeParty.
 * Can be confusing to name it ROOM_BOSSES_MAX since a Room only has one boss pointer.
 * @see FoeParty
 * @see prepareBoss()FoeParty
 */
#define FOES_BOSSES_MAX 2

/**
 * The different kinds of FoeParty.
 * Ordering is directly related to foepartystrings
 * @see foepartystrings
 * @see Enemy
 * @see Boss
 */
typedef enum {
    Enemies = 0,
    Bosses = 1,
} foePartyClass;

/** Maximum index of foePartyClass, so that the size has to add 1 for the 0th index
 * @see foePartyClass
 */
#define FOEPARTY_CLASS_MAX 1

/**
 * Array with the name strings for foePartyClass.
 * @see foePartyClass
 * @see printStringFromFoePartyClass()
 */
extern char *foepartystrings[FOEPARTY_CLASS_MAX + 1];

/**
 * Represents the entity initialised from a foePartyClass.
 * @see foePartyClass
 * @see Turncounter
 * @see counterIndexes
 */
typedef struct FoeParty {
    foePartyClass class;     /**< Defines which kind of foePartyClass the instance relates to*/
    int level;	   /**< Level for the whole FoeParty*/
    struct Turncounter *counters[COUNTERSMAX + 1];   /**< Array with all the Turncounter pointers*/
    int turnboost_atk;	   /**< Current temp boost value for atk*/
    int turnboost_def;	   /**< Current temp boost value for def*/
    int turnboost_vel;	   /**< Current temp boost value for vel*/
    int turnboost_enr;	   /**< Current temp boost value for enr*/
    Enemy *enemy_foes[ROOM_ENEMIES_MAX + 1];   /**< Array of pointers to all enemies in a room*/
    //TODO: this is not ideal. Could we ever need to have a different max for enemy_foes? Only if the group is not in a Room??
    Boss *boss_foes[FOES_BOSSES_MAX + 1];   /**< Array of pointers to all enemies in a room*/
    int size;	  /**<  Number of members in the FoeParty.*/
    int tot_alive;     /**<  Number of alive members of the FoeParty.*/
    int current_index;	   /**< Index of current foe advancement*/
    int alive_enemies[ROOM_ENEMIES_MAX + 1];   /**< Array of integers with 1 for alive enemies in that position*/
    int alive_bosses[FOES_BOSSES_MAX + 1];   /**< Array of integers with 1 for alive bosses in that position*/
} FoeParty;

/**
 * Holds the base stats for a foePartyClass.
 * @see FoeParty
 * @see foePartyClass
 * @see initFoePartyStats()
 */
typedef struct FoePartyBaseStats {
    foePartyClass class;     /**< Defines which kind of foePartyClass the stats relate to*/
    int level;	   /**< Defines level of the FoeParty*/
} FoePartyBaseStats;

/**
 * Array containing all FoePartyBaseStats instances.
 * All the classes get their starting stats from here.
 * Ordering of parameters is: class, level
 * @see FoeParty
 * @see foePartyClass
 * @see initFoePartyStats()
 */
extern FoePartyBaseStats basefoepartystats[FOEPARTY_CLASS_MAX + 1];

/**
 * Represents the entity initialised from a roomClass.
 * @see roomClass
 * @see Shop
 * @see Enemy
 */
typedef struct Room {

    int index;	   /**< The room's own number.*/
    char *desc;	    /**< Description string.*/

    roomClass class;	 /**< Defines which kind of roomClass the instance relates to*/
    Shop *shop;	    /**< The shop pointer, initialised for rooms of class SHOP.*/
    Roadfork *roadfork;	    /**< The roadfork pointer, initialised for rooms of class ROADFORK.*/
    Treasure *treasure;	    /**< The treasure pointer, initialised for rooms of class TREASURE.*/

    Boss *boss;	    /**< The boss pointer, initialised for rooms of class BOSS.*/
    int enemyTotal;	/**< Total number of enemies initialised in enemies array.*/
    Enemy *enemies[ROOM_ENEMIES_MAX];	  /**< Array for enemies.*/
    FoeParty *foes;	/**< The FoeParty pointer, initialised for rooms of class ENEMIES.*/
} Room;

/**
 * The different kinds of Floor.
 * Ordering is directly related to floornamestrings.
 * There must always be exactly FLOOR_CLASS_MAX +1 values.
 * @see floornamestrings
 */
typedef enum {
    STD_FLOOR = 0,
    DBG_FLOOR = 1,
} floorClass;

/** Maximum index of floorClass, so that the size has to add 1 for the 0th index
 * @see floorClass
 */
#define FLOOR_CLASS_MAX 1

/**
 * Array with the name strings for floorClass.
 * @see floorClass
 */
extern char *floornamestrings[FLOOR_CLASS_MAX + 1];

/**
 * Maximum value of cells in a Floor matrix column.
 * @see Floor
 */
#define FLOOR_MAX_COLS 24

/**
 * Maximum value of cells in a Floor matrix row.
 * @see Floor
 */
#define FLOOR_MAX_ROWS 24

/**
 * Represents the entity initialised from a floorClass.
 * @see floorClass
 * @see Room
 */
typedef struct Floor {

    int index;	   /**< The floor's own number.*/
    char *desc;	    /**< Description string.*/

    floorClass class;	  /**< Defines which kind of floorClass the instance relates to*/
    int floor_layout[FLOOR_MAX_COLS][FLOOR_MAX_ROWS];	  /**< Defines the layout for the Floor (which cells are actually filled).*/
    int area;	  /**< Holds how many cells we succesfully random walked.*/
    Room *rooms_matrix[FLOOR_MAX_COLS][FLOOR_MAX_ROWS];	    /**< Pointer matrix for rooms of this floor.*/
    roomClass roomclass_layout[FLOOR_MAX_COLS][FLOOR_MAX_ROWS];	    /**< roomClass matrix for class value for rooms of this floor.*/
    int explored_matrix[FLOOR_MAX_COLS][FLOOR_MAX_ROWS];     /**< Int matrix for explored value for rooms of this floor.*/
    int explored_area;	   /**< Holds how many cells we explored.*/
    bool from_bsp; /**< Flag for floors generated using bsp.*/

    //TODO: add some pointer to a loot instance, initialised for floors having some flag active.*/
} Floor;

/**
 * Maximum number of floors to beat in Rogue mode.
 * @see Floor
 */
#define MAX_ROGUE_FLOORS 17

/**
 * The different kinds of saveType.
 * @see loadInfo
 */
typedef enum saveType {
    HOME_SAVE = 0,
    ENEMIES_SAVE = 1,
    FLOORMENU_SAVE = 2,
} saveType;

/**
 * Maximum value of saveType values.
 * @see saveType
 */
#define SAVETYPE_MAX 2

/**
 * Array with the name strings for saveType.
 * @see saveType
 * @see stringFrom_saveType()
 */
extern char *savetypestrings[SAVETYPE_MAX + 1];

/**
 * Contains loaded values to pass to initRoom().
 * @see initRoom()
 * @see gameloop()
 * @see handleLoadgame()
 * @see Enemy
 */
typedef struct loadInfo {

    int is_new_game;
    int done_loading;
    int enemy_index;
    int total_foes;
    Enemy *loaded_enemy;
    int *ptr_to_roomtotalenemies;
    int *ptr_to_roomindex;
    saveType save_type;

} loadInfo;

/**
 * Holds arguments for a game.
 * @see countStats
 * @see roomClass
 * @see Wincon
 * @see Path
 * @see GameOptions
 */
typedef struct {

    clock_t start_time; /**< Keeps track of game start time.*/

    countStats *stats;	   /**< Keeps track of stats for the game.*/

    int current_fighters;     /**< Keeps track of Fighter number for the game.*/
    roomClass current_roomtype;	    /**< Keeps track of current room type.*/
    int current_room_index;	/**< Keeps track of current room index.*/

    int current_enemy_index;	 /**< Keeps track of current enemy index.*/
    Wincon *wincon;	/**< Keeps track of current Wincon.*/

    Path *path;	    /**< Keeps track of current Path.*/

    Fighter *player;	 /**< Keeps track of current Player.*/

    Gamemode gamemode;	   /**< Keeps track of current Gamemode.*/

    Floor *current_floor; /**< Pointer to current floor, initialised when gamemode==Rogue.*/
    Room *current_room; /**< Pointer to current room.*/
#ifdef HELAPORDO_CURSES_BUILD
    GameScreen *screen; /**< Pointer to main screen.*/
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

    GameOptions *options; /**< Keeps track of current options.*/

    bool is_seeded; /**< Denotes if the current game was started with a set seed.*/
} Gamestate;

#ifndef KOLISEO_HAS_REGION
/**
 * Global variable used to store Gamestate address when KLS_Region is not available. Could be used even with it.
 * Useful to save the game in a pinch, like when receiving SIGINT.
 */
extern Gamestate* G_GAMESTATE;
#endif

/**
 * The different kinds of turnOption.
 * @see getTurnChoice()
 * @see fight()
 * @see quit()
 * @see debug()
 */
typedef enum turnOption {
    NEW_GAME = 0,
    LOAD_GAME = 1,
    INVALID = 69,
    FIGHT = 100,
    SPECIAL = 101,
    QUIT = 102,
    CONSUMABLE = 103,
    ARTIFACTS = 104,
    EQUIPS = 105,
    PERKS = 106,
    STATS = 107,
    SAVE = 108,
    EXPLORE = 109,
    DEBUG = 1337,
    TUTORIAL = 777,
    CLOSE_MENU = 110,
    SKILL = 111,
    GAME_OPTIONS = 112,
} turnOption;

/**
 * The different kinds of turnOption_OP.
 * @see stringFromTurnOP()
 */
typedef enum turnOption_OP {
    OP_NEW_GAME = 0,
    OP_LOAD_GAME = 1,
    OP_INVALID = 2,
    OP_FIGHT = 3,
    OP_SPECIAL = 4,
    OP_QUIT = 5,
    OP_CONSUMABLE = 6,
    OP_ARTIFACTS = 7,
    OP_EQUIPS = 8,
    OP_PERKS = 9,
    OP_STATS = 10,
    OP_SAVE = 11,
    OP_EXPLORE = 12,
    OP_DEBUG = 13,
    OP_LOAD_ENEMYROOM = 14,
    OP_LOAD_HOMEROOM = 15,
    OP_SKILL = 16,
    OP_CHANGE_OPTIONS = 17,
} turnOption_OP;

/**
 * Returns a turnOption_OP from a turnOption.
 * @param t The turnOption to convert.
 * @return The corresponding turnOption_OP
 */
turnOption_OP turnOP_from_turnOption(turnOption t);

/**
 * Maximum value for turnOption_OP.
 * At runtime, not all of them are valid options.
 * @see turnOption_OP
 * @see stringFromTurnOP()
 */
#define TURNOP_MAX 17
/**
 * Array with the name strings for turnOption.
 * @see turnOption
 * @see stringFromTurnOption()
 */
extern char *turnopstrings[TURNOP_MAX + 1];

/**
 * The different kinds of foeTurnOption.
 */
typedef enum foeTurnOption {
    FOE_INVALID = 0,
    FOE_IDLE = 1,
    FOE_FIGHT = 2,
    FOE_SPECIAL = 3,
} foeTurnOption;

/**
 * The different kinds of foeTurnOption_OP.
 * @see stringFromFoeTurnOP()
 */
typedef enum foeTurnOption_OP {
    FOE_OP_INVALID = 0,
    FOE_OP_IDLE = 1,
    FOE_OP_FIGHT = 2,
    FOE_OP_SPECIAL = 3,
} foeTurnOption_OP;

/**
 * Returns a foeTurnOption_OP from a foeTurnOption.
 * @param ft The foeTurnOption to convert.
 * @return The corresponding foeTurnOption_OP
 */
foeTurnOption_OP foeTurnOP_from_foeTurnOption(foeTurnOption ft);

/**
 * Maximum value for foeTurnOption_OP.
 * At runtime, not all of them are valid options.
 * @see foeTurnOption_OP
 * @see stringFromFoeTurnOP()
 */
#define FOETURNOP_MAX 3
/**
 * Array with the name strings for foeTurnOption.
 * @see foeTurnOption
 * @see stringFromFoeTurnOption()
 */
extern char *foeturnopstrings[FOETURNOP_MAX + 1];

/**
 * Filename for OPS log.
 * @see turnOP()
 * @see turnOption_OP
 */
#define OPS_LOGFILE "OPS_log.txt"

/**
 * The different kinds of OP_res.
 * @see turnOP()
 */
typedef enum OP_res {
    OP_OK = 0,
    INVALID_OP = 1,
    NO_OP = 2,
    OP_CANNOT_OPEN_SAVEFILE = 3,
    OP_RES_KILL_DONE = 4,   /**< Value of fight status indicating the enemy died.*/
    OP_RES_DMG_DEALT = 5,   /**< Value of fight status indicating the enemy was damaged.*/
    OP_RES_DMG_TAKEN = 6,   /**< Value of fight status indicating the fighter was damaged.*/
    OP_RES_DEATH = 7,	/**< Value of fight status indicating the fighter died.*/
    OP_RES_NO_DMG = 8,	 /**< Value of fight status indicating no damage occurred. Used to reset the fight status to neutral after the end of a combat. */
    OP_RES_CLASH = 9,	/**< Value of fight status indicating damage on both sides occurred.*/
    OP_RES_LOADED_ENEMYROOM = 10,   /**< Value of OP_res indicating success in loading a enemyroom*/
} OP_res;

/**
 * Maximum value of OP_res values.
 * @see OP_res
 * @see turnOP()
 */
#define OP_RES_MAX 10

/**
 * Array with the name strings for OP_res.
 * @see turnOption_OP
 * @see turnOP()
 * @see stringFrom_OP_res()
 */
extern char *opresstrings[OP_RES_MAX + 1];

/**
 * Holds arguments for a turnOption_OP.
 * @see turnOption_OP
 * @see turnOption
 * @see turnOP()
 */
typedef struct {
    Fighter *actor;	/**< Pointer to Fighter actor for the OP*/
    Path *path;	    /**< Pointer to Rath for OP*/
    Room *room;	    /**< Pointer to Room for OP*/
    loadInfo *load_info;     /**< Pointer to loadInfo for OP*/
    Enemy *enemy;     /**< Pointer to Enemy for OP*/
    Boss *boss;	    /**< Pointer to Boss for OP*/
    FILE *save_file;	 /**< Pointer to savefile for OP*/
#ifdef HELAPORDO_CURSES_BUILD
    WINDOW *notify_win;	    /**< Pointer to notification window for OP*/
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined."
#else
    Rectangle *notify_win; /**< Pointer to notification area for OP*/
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
    RingaBuf* rb_notifications; /**< Pointer to RingaBuf for notifications for OP*/
    Koliseo_Temp *t_kls;     /**< Pointer to Koliseo_Temp for OP*/
    Gamestate *gmst;	 /**< Pointer to Gamestate for OP*/
    foeTurnOption_OP foe_op;	 /**< Picked FoeTurnOption_OP, initialised only for some OPs.*/
    skillType picked_skill;	 /**< Picked skillType, initialised only for OP_SKILL.*/
} turnOP_args;

/**
 * Ratio of shop rooms. Every Xth room will have a shop.
 * @see initRoom()
 */
#define SHOPROOM 4
/**
 * Ratio of boss rooms. Every Xth room will have a boss.
 * @see initRoom()
 */
#define BOSSROOM 5
/**
 * Ratio of home rooms. Every Xth room will be a Home room.
 * @see initRoom()
 */
#define HOMEROOM 6
/**
 * Ratio of beast rooms to normal ones. Every Xth room will have a beast.
 * @see initEnemyStats()
 */
#define BEASTROOM 3
/**
 * Factor for stat boost to beast enemies.
 * @see initEnemyStats()
 */
#define BSTFACTOR 1.5

/**
 * Ratio of Fighter levels to Enemy levels. Every Xth level up will apply to enemies.
 * @see initEnemyStats()
 */
#define ENEMYLVLRATIO 2
/**
 * Ratio of Fighter levels to unlock a new special move. Every Xth level up will unlock one.
 * @see unlockSpecial()
 */
#define SPECIALLVLRATIO 2
/**
 * Ratio of Fighter levels to Equip levels. Every Xth level up will apply to equips.
 * @see dropEquip()
 */
#define EQUIPLVLBOOSTRATIO 3

/**
 * The different kinds of fightResult.
 */
typedef enum {
    FIGHTRES_INVALID = 0,   /**< Value of fight status indicating errors in the fight call.*/
    FIGHTRES_KILL_DONE = 1,   /**< Value of fight status indicating the enemy died.*/
    FIGHTRES_DMG_DEALT = 2,   /**< Value of fight status indicating the enemy was damaded.*/
    FIGHTRES_DMG_TAKEN = 3,   /**< Value of fight status indicating the fighter was damaded.*/
    FIGHTRES_DEATH = 4,	  /**< Value of fight status indicating the fighter died.*/
    FIGHTRES_NO_DMG = 5,   /**< Value of fight status indicating no damage occurred. Used to reset the fight status to neutral after the end of a combat. */
    FIGHTRES_CLASH = 6,	  /**< Value of fight status indicating damage on both sides occurred.*/
} fightResult;

/**
 * Total number of defined fightResult values.
 * @see fightResult
 */
#define FIGHT_RES_TOT 7

/**
 * Array with the name strings for fightResult.
 * @see fightResult
 * @see stringFrom_fightResult()
 */
extern char *fightresultstrings[FIGHT_RES_TOT + 1];

/**
 * Returns a OP_res from a fightResult fr.
 * @param fr The fightResult to convert.
 * @return The corresponding OP_res
 */
OP_res OP_res_from_fightResult(fightResult fr);

#define CHEST_FRAMES 25	/**< Defines the number of frames for the chest animation.*/
#define CHEST_ROWS 18 /**< Defines the number of rows for the chest animation.*/
#define CHEST_COLS 18 /**< Defines the number of cols for the chest animation.*/
#define CHEST_FRAMETIME 67 /**< Defines for how many millisecs a sprite should stay on screen for the chest animation.*/

#define CURSES_BINSAVE_NAME "save-nc.bin" /**< Defines file name used for saveheader binary save. */
#define RL_BINSAVE_NAME "save-rl.bin" /**< Defines file name used for saveheader binary save. */

#define CURSES_GMSTSAVE_NAME "run-nc.bin" /**< Defines file name used for gamestate binary save. */
#define RL_GMSTSAVE_NAME "run-rl.bin" /**< Defines file name used for gamestate binary save. */

#define CURSES_SETTINGS_SAVE_NAME "settings-nc.bin" /**< Defines file name used for game options binary save. */
#define RL_SETTINGS_SAVE_NAME "settings-rl.bin" /**< Defines file name used for game options binary save. */

extern const wchar_t HEAD_CHAR_ICON;
extern const wchar_t TORSO_CHAR_ICON;
extern const wchar_t LEGS_CHAR_ICON;

#define NOTIFICATION_BUFFER_SIZE 200 /**< Defines message buffer size for Notification.*/

/**
 * Holds text for a notification.
 */
typedef struct Notification {
    char buf[NOTIFICATION_BUFFER_SIZE+1];
    int8_t color;
    bool displayed;
} Notification;

#define NOTIFICATIONS_RINGBUFFER_SIZE 7 /**< Defines the size for notifications ringbuffer. */

#endif // GAME_CORE_H
