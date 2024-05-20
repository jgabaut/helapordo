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

#ifndef SER_CORE_H
#define SER_CORE_H
#include <inttypes.h>
#include "../utils/game_utils.h"

#ifdef HELAPORDO_RAYLIB_BUILD
#include "../build-rl/game_rl.h"
#endif

/**
 * Defines size for all SerSaveHeader char buffers.
 * @see SerSaveHeader
 */
#define SERSAVEHEADER_BUFSIZE 15

/**
 * Serialized save header. Packed struct.
 * Can be turned into a SaveHeader with deser_SaveHeader().
 * @see deser_SaveHeader()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerSaveHeader {
#else
#pragma pack(push, 1)
typedef struct SerSaveHeader {
#endif
    int32_t api_level;
    char game_version[SERSAVEHEADER_BUFSIZE+1];
    char save_version[SERSAVEHEADER_BUFSIZE+1];
    char os[SERSAVEHEADER_BUFSIZE+1];
    char machine[SERSAVEHEADER_BUFSIZE+1];
#ifdef __GNUC__
} SerSaveHeader;
#else
} SerSaveHeader;
#pragma pack(pop)
#endif

/**
 * Defines size for all SaveHeader char buffers.
 * @see SaveHeader
 */
#define SAVEHEADER_BUFSIZE SERSAVEHEADER_BUFSIZE

/**
 * Save header. Normal struct.
 * Can be obtained from a SerSaveHeader with deser_SaveHeader().
 * @see deser_SaveHeader()
 * @see SerSaveHeader
 */
typedef struct SaveHeader {
    int32_t api_level;
    char game_version[SAVEHEADER_BUFSIZE+1];
    char save_version[SAVEHEADER_BUFSIZE+1];
    char os[SAVEHEADER_BUFSIZE+1];
    char machine[SAVEHEADER_BUFSIZE+1];
} SaveHeader;

bool writeSerSaveHeader(const char* filename, SerSaveHeader* data);

bool readSerSaveHeader(const char* filename, SerSaveHeader* data);

bool deser_SaveHeader(SerSaveHeader* ser, SaveHeader* deser);

SaveHeader* prep_saveHeader(const char* static_path, Koliseo* kls, bool force_init, bool* did_init, int saveslot_index);

/**
 * Serialized Turncounter. Packed struct.
 * Can be turned into a Turncounter with deser_turnCounter().
 * Can be obtained from a Turncounter with ser_turnCounter().
 * @see Turncounter
 * @see deser_turnCounter()
 * @see ser_turnCounter()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerTurncounter {
#else
#pragma pack(push, 1)
typedef struct SerTurncounter {
#endif
    int32_t count;	   /**< Defines the current count.*/
    int32_t innerValue;	/**< Contains an integer for counters that need a state*/
    Countertype type;	  /**< Defines which kind of Countertype the instance relates to*/
#ifdef __GNUC__
} SerTurncounter;
#else
} SerTurncounter;
#pragma pack(pop)
#endif

/**
 * Serialized Perk. Packed struct.
 * Can be turned into a Perk with deser_Perk().
 * Can be obtained from a Perk with ser_Perk().
 * @see Perk
 * @see deser_Perk()
 * @see ser_Perk()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerPerk {
#else
#pragma pack(push, 1)
typedef struct SerPerk {
#endif
    int32_t class;	  /**< Defines which kind of Perk the instance relates to*/
    int32_t innerValue;	/**< Contains an integer for perks that need a state*/
#ifdef __GNUC__
} SerPerk;
#else
} SerPerk;
#pragma pack(pop)
#endif

#define SerPerk_Fmt "SerPerk { Class: %s , InnerValue: %" PRId32 " }"

#define SerPerk_Arg(sp) (nameStringFromPerk(sp.class)), (sp.innerValue)

/**
 * Serialized Skillslot. Packed struct.
 * Can be turned into a Skillslot with deser_Skillslot().
 * Can be obtained from a Skillslot with ser_Skillslot().
 * @see Skillslot
 * @see deser_Skillslot()
 * @see ser_Skillslot()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerSkillslot {
#else
#pragma pack(push, 1)
typedef struct SerSkillslot {
#endif
    int32_t class;	  /**< Defines which kind of Skillslot the instance relates to*/
    int8_t enabled;	/**< Flag defining if the slot is initialised.*/
#ifdef __GNUC__
} SerSkillslot;
#else
} SerSkillslot;
#pragma pack(pop)
#endif

/**
 * Serialized Equip. Packed struct.
 * Can be turned into an Equip with deser_Equip().
 * Can be obtained from an Equip with ser_Equip().
 * @see Equip
 * @see deser_Equip()
 * @see ser_Equip()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerEquip {
#else
#pragma pack(push, 1)
typedef struct SerEquip {
#endif
    int32_t class;	  /**< Defines which kind of Equip the instance relates to*/
    int32_t type;	/**< Defines which kind of Equipzone the instance relates to*/
    int8_t equipped; /**< Flag indicating the instance as currently in use if set.*/
    int8_t level;
    int8_t atk;
    int8_t def;
    int8_t vel;
    int8_t enr;
    int8_t bonus;
    int8_t perksCount;
    int8_t cost;
    quality qual;
    SerPerk perks[EQUIPPERKSMAX];
#ifdef __GNUC__
} SerEquip;
#else
} SerEquip;
#pragma pack(pop)
#endif

/**
 * Serialized Equipslot. Packed struct.
 * Can be turned into an Equipslot with deser_Equipslot().
 * Can be obtained from an Equipslot with ser_Equipslot().
 * @see Equipslot
 * @see deser_Equipslot()
 * @see ser_Equipslot()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerEquipslot {
#else
#pragma pack(push, 1)
typedef struct SerEquipslot {
#endif
    int32_t type;	/**< Defines which kind of Equipzone the instance relates to*/
    int8_t active; /**< Flag indicating the instance as currently in use if set.*/
#ifdef __GNUC__
} SerEquipslot;
#else
} SerEquipslot;
#pragma pack(pop)
#endif

/**
 * Serialized Specialslot. Packed struct.
 * Can be turned into a Specialslot with deser_Specialslot().
 * Can be obtained from a Specialslot with ser_Speciaslot().
 * @see Specialslot
 * @see deser_Specialslot()
 * @see ser_Specialslot()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerSpecialslot {
#else
#pragma pack(push, 1)
typedef struct SerSpecialslot {
#endif
    int8_t enabled;     /**< Flag defining if the current slot is initialised*/
    specialMove move;	  /**< Defines which kind of specialMove the slot is holding*/
    int8_t cost;	  /**< Cost of use*/
#ifdef __GNUC__
} SerSpecialslot;
#else
} SerSpecialslot;
#pragma pack(pop)
#endif

/**
 * Serialized Consumable. Packed struct.
 * Can be turned into a Consumable with deser_Consumable().
 * Can be obtained from a Consumable with ser_Consumable().
 * @see Consumable
 * @see deser_Consumable()
 * @see ser_Consumable()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerConsumable {
#else
#pragma pack(push, 1)
typedef struct SerConsumable {
#endif
    int32_t class;     /**< Defines which kind of Consumable this is*/
    int qty;	 /**< Indicates how many copies of the consumable the instance of this class holds*/
#ifdef __GNUC__
} SerConsumable;
#else
} SerConsumable;
#pragma pack(pop)
#endif

/**
 * Serialized Artifact. Packed struct.
 * Can be turned into a Artifact with deser_Artifact().
 * Can be obtained from an Artifact with ser_Artifact().
 * @see Artifact
 * @see deser_Artifact()
 * @see ser_Artifact()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerArtifact {
#else
#pragma pack(push, 1)
typedef struct SerArtifact {
#endif
    int32_t class;     /**< Defines which kind of artifactClass the instance relates to*/
    int qty;	 /**< Indicates how many copies of the artifact the instance of this class holds*/
    int active;	    /**< Flag used to mark instance whose function pointer has been called already*/
    int innerValue;	/**< Indicates interal state of the item when needed*/
#ifdef __GNUC__
} SerArtifact;
#else
} SerArtifact;
#pragma pack(pop)
#endif

/**
 * Serialized countStats. Packed struct.
 * Can be turned into a countStats with deser_countStats().
 * Can be obtained from a countStats with ser_countStats().
 * @see countStats
 * @see deser_countStats()
 * @see ser_countStats()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerCountstats {
#else
#pragma pack(push, 1)
typedef struct SerCountstats {
#endif
    int8_t enemieskilled;	   /**< How many enemies were defeated*/
    int8_t consumablesfound;     /**< How many consumabls were found*/
    int8_t equipsfound;	 /**< How many equips were found*/
    int8_t artifactsfound;	    /**< How many artifacts were found*/
    int8_t criticalhits;	  /**< How many criticals were dealt*/
    int8_t roomscompleted;	    /**< How many rooms were completed*/
    int8_t floorscompleted;     /**< How many floors were completed*/
    int8_t specialsunlocked;     /**< How many special move were unlocked*/
    int8_t coinsfound;	/**< How many coins you found in total*/
    int8_t bosseskilled;	  /**< How many bosses were defeated*/
    int8_t unique_bosseskilled;	 /**< How many unique bosses were defeated*/
    int8_t killed_bosses[BOSSCLASSESMAX + 1];   /**< Keeps track of which bossClass you've killed.*/
    int8_t keysfound;     /**< How many keys you found in total*/
#ifdef __GNUC__
} SerCountstats;
#else
} SerCountstats;
#pragma pack(pop)
#endif

/**
 * Serialized Enemy. Packed struct.
 * Can be turned into an Enemy with deser_Enemy().
 * Can be obtained from a Enemy with ser_Enemy().
 * @see Enemy
 * @see deser_Enemy()
 * @see ser_Enemy()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerEnemy {
#else
#pragma pack(push, 1)
typedef struct SerEnemy {
#endif
    int8_t index;	   /**< Defines position inside containing foeParty.*/
    int32_t class;	  /**< Defines which kind of enemyClass the instance relates to*/
    int8_t hp;	/**< Current hp value*/
    int8_t atk;	 /**< Current atk value*/
    int8_t def;	 /**< Current def value*/
    int8_t vel;	 /**< Current vel value*/
    int8_t level;	   /**< Level value*/
    int8_t luck;	  /**< Luck value*/
    int8_t xp;	/**< Xp value given on death*/
    int8_t totalhp;     /**< Full hp value*/

    int8_t energy;	    /**< Current energy value*/
    int8_t totalenergy;	 /**< Full energy value*/

    int8_t stamina;     /**< Current stamina value*/
    int8_t totalstamina;	  /**< Full stamina value*/

    fighterStatus status;     /**< Defines active fighterStatus*/
    int8_t beast;	   /**< Flag defining the instance as "beast" if set*/

    SerSkillslot skills[ENEMY_SKILL_SLOTS + 1];   /**< Array with all the SerSkillslot*/

    SerTurncounter counters[COUNTERSMAX + 1];   /**< Array with all the SerTurncounter*/
    int8_t turnboost_atk;	   /**< Current temp boost value for atk*/
    int8_t turnboost_def;	   /**< Current temp boost value for def*/
    int8_t turnboost_vel;	   /**< Current temp boost value for vel*/
    int8_t turnboost_enr;	   /**< Current temp boost value for enr*/

    int8_t prize;	   /**< Amount of currency dropped on defeat*/
#ifdef __GNUC__
} SerEnemy;
#else
} SerEnemy;
#pragma pack(pop)
#endif

/**
 * Serialized Boss. Packed struct.
 * Can be turned into an Boss with deser_Boss().
 * Can be obtained from a Boss with ser_Boss().
 * @see Boss
 * @see deser_Boss()
 * @see ser_Boss()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerBoss {
#else
#pragma pack(push, 1)
typedef struct SerBoss {
#endif
    int32_t class;	 /**< Defines which kind of enemyClass the instance relates to*/
    int8_t hp;	/**< Current hp value*/
    int8_t atk;	 /**< Current atk value*/
    int8_t def;	 /**< Current def value*/
    int8_t vel;	 /**< Current vel value*/
    int8_t level;	   /**< Level value*/
    int8_t luck;	  /**< Luck value*/
    int8_t xp;	/**< Xp value given on death*/
    int8_t totalhp;     /**< Full hp value*/

    int8_t energy;	    /**< Current energy value*/
    int8_t totalenergy;	 /**< Full energy value*/

    int8_t stamina;     /**< Current stamina value*/
    int8_t totalstamina;	  /**< Full stamina value*/

    fighterStatus status;     /**< Defines active fighterStatus*/
    int8_t beast;	   /**< Flag defining the instance as "beast" if set*/

    SerSkillslot skills[BOSS_SKILL_SLOTS + 1];   /**< Array with all the SerSkillslot*/
    SerTurncounter counters[COUNTERSMAX + 1];   /**< Array with all the SerTurncounter*/
    int8_t turnboost_atk;	   /**< Current temp boost value for atk*/
    int8_t turnboost_def;	   /**< Current temp boost value for def*/
    int8_t turnboost_vel;	   /**< Current temp boost value for vel*/
    int8_t turnboost_enr;	   /**< Current temp boost value for enr*/

    int8_t prize;	   /**< Amount of currency dropped on defeat*/
#ifdef __GNUC__
} SerBoss;
#else
} SerBoss;
#pragma pack(pop)
#endif

/**
 * Defines size for SerFighter name buffer.
 * @see SerFighter
 */
#define SERFIGHTER_NAME_BUFSIZE FIGHTER_NAME_BUFSIZE

/**
 * Serialized Fighter. Packed struct.
 * Can be turned into an Fighter with deser_Fighter().
 * Can be obtained from a Fighter with ser_Fighter().
 * @see Fighter
 * @see deser_Fighter()
 * @see ser_Fighter()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerFighter {
#else
#pragma pack(push, 1)
typedef struct SerFighter {
#endif
    char name[SERFIGHTER_NAME_BUFSIZE];     /**< Name string*/
    int32_t class;	    /**< Defines which kind of fighterClass the instance relates to*/
    int8_t hp;	/**< Current hp value*/
    int8_t atk;	 /**< Current atk value*/
    int8_t def;	 /**< Current def value*/
    int8_t vel;	 /**< Current vel value*/
    int8_t level;	   /**< Current level value*/
    int8_t luck;	  /**< Current luck value*/
    int8_t totalxp;     /**< Lifetime xp value*/
    int8_t currentlevelxp;	    /**< Xp gained for the current level*/
    int8_t totallevelxp;	  /**< Xp needed to level up*/
    int8_t totalhp;     /**< Full hp value*/
    fighterStatus status;     /**< Defines active fighterStatus*/

    int8_t energy;	    /**< Current energy value*/
    int8_t totalenergy;	 /**< Full energy value*/
    int8_t stamina;     /**< Current stamina value*/
    int8_t totalstamina;	  /**< Full stamina value*/
    SerSpecialslot specials[SPECIALSMAX + 1];   /**< Array with all the SerSpecialslot*/
    SerSkillslot skills[FIGHTER_SKILL_SLOTS + 1];   /**< Array with all the SerSkillslot*/

    SerTurncounter counters[COUNTERSMAX + 1];   /**< Array with all the SerTurncounter*/
    int8_t turnboost_atk;	   /**< Current temp boost value for atk*/
    int8_t turnboost_def;	   /**< Current temp boost value for def*/
    int8_t turnboost_vel;	   /**< Current temp boost value for vel*/
    int8_t turnboost_enr;	   /**< Current temp boost value for enr*/

    int8_t perksCount;	/**< Keeps track of how many perks are active.*/
    SerPerk perks[PERKSMAX + 1];	 /**< Array with all the SerPerk*/

    SerEquipslot equipslots[EQUIPZONES + 1];   /**< Array with all the SerEquipslot*/
    SerEquip equipsBag[EQUIPSBAGSIZE + 1];	  /**< Array with all the SerEquip found*/
    SerConsumable consumablesBag[CONSUMABLESMAX + 1];   /**< Array with all the Consumables found*/
    SerArtifact artifactsBag[ARTIFACTSMAX + 1];   /**< Array with all the Artifacts found*/

    int8_t equipsBagOccupiedSlots;	    /**< Keeps track of how many slots are occupied.*/
    int8_t earliestBagSlot;	/**< Keeps track of first available spot.*///TODO To always use the array efficiently (???) I sense linked lists may be better

    int8_t permboost_atk;	   /**< Current temp boost value for atk.*/
    int8_t permboost_def;	   /**< Current temp boost value for def.*/
    int8_t permboost_vel;	   /**< Current temp boost value for vel.*/
    int8_t permboost_enr;	   /**< Current temp boost value for enr.*/

    int8_t equipboost_atk;	    /**< Current equip boost value for atk.*/
    int8_t equipboost_def;	    /**< Current equip boost value for def.*/
    int8_t equipboost_vel;	    /**< Current equip boost value for vel.*/
    int8_t equipboost_enr;	    /**< Current equip boost value for enr.*/

    SerCountstats stats;	    /**< SerCountstats instance*/

    int8_t balance;     /**< Amount of currency owned*/
    int8_t keys_balance;	  /**< Amount of keys owned*/

    int8_t floor_x; /**< Current x position in floor.*/
    int8_t floor_y; /**< Current y position in floor.*/
#ifdef __GNUC__
} SerFighter;
#else
} SerFighter;
#pragma pack(pop)
#endif

/**
 * Serialized FoeParty. Packed struct.
 * Can be turned into an FoeParty with deser_FoeParty().
 * Can be obtained from a FoeParty with ser_FoeParty().
 * @see FoeParty
 * @see deser_FoeParty()
 * @see ser_FoeParty()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerFoeParty {
#else
#pragma pack(push, 1)
typedef struct SerFoeParty {
#endif
    int32_t class;     /**< Defines which kind of foePartyClass the instance relates to*/
    int8_t level;	   /**< Level for the whole FoeParty*/
    SerTurncounter counters[COUNTERSMAX + 1];   /**< Array with all the SerTurncounter*/
    int8_t turnboost_atk;	   /**< Current temp boost value for atk*/
    int8_t turnboost_def;	   /**< Current temp boost value for def*/
    int8_t turnboost_vel;	   /**< Current temp boost value for vel*/
    int8_t turnboost_enr;	   /**< Current temp boost value for enr*/
    SerEnemy enemy_foes[ROOM_ENEMIES_MAX + 1];   /**< Array for all enemies in a room*/
    //TODO: this is not ideal. Could we ever need to have a different max for enemy_foes? Only if the group is not in a Room??
    SerBoss boss_foes[FOES_BOSSES_MAX + 1];   /**< Array of for all bosses in a room*/
    int8_t size;	  /**<  Number of members in the FoeParty.*/
    int8_t tot_alive;     /**<  Number of alive members of the FoeParty.*/
    int8_t current_index;	   /**< Index of current foe advancement*/
    int8_t alive_enemies[ROOM_ENEMIES_MAX + 1];   /**< Array of integers with 1 for alive enemies in that position*/
    int8_t alive_bosses[FOES_BOSSES_MAX + 1];   /**< Array of integers with 1 for alive bosses in that position*/
#ifdef __GNUC__
} SerFoeParty;
#else
} SerFoeParty;
#pragma pack(pop)
#endif

/**
 * Serialized Chest. Packed struct.
 * Can be turned into a Chest with deser_Chest().
 * Can be obtained from a Chest with ser_Chest().
 * @see Chest
 * @see deser_Chest()
 * @see ser_Chest()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerChest {
#else
#pragma pack(push, 1)
typedef struct SerChest {
#endif
    int32_t class;	  /**< Defines which kind of chesteClass the instance relates to*/
    int consumablesCount;     /**< Defines how many consumables the chest contains.*/

    SerConsumable consumables[CHEST_CONSUMABLES_MAX];	/**< The contained consumables.*/
    int equipsCount;	 /**< Defines how many equips the chest contains.*/

    SerEquip equips[CHEST_EQUIPS_MAX];	 /**< The contained equips.*/
#ifdef __GNUC__
} SerChest;
#else
} SerChest;
#pragma pack(pop)
#endif

/**
 * Serialized Treasure. Packed struct.
 * Can be turned into a Treasure with deser_Treasure().
 * Can be obtained from a Treasure with ser_Treasure().
 * @see Treasure
 * @see deser_Treasure()
 * @see ser_Treasure()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerTreasure {
#else
#pragma pack(push, 1)
typedef struct SerTreasure {
#endif
    int32_t class;     /**< Defines which kind of treasureClass the instance relates to*/
    SerChest chest;     /**< The treasure SerChest, initialised if class == TREASURE_CHEST*/
    SerConsumable consumable;	/**< The treasure SerConsumable, initialised if class == TREASURE_CONSUMABLE*/
    SerArtifact artifact;	    /**< The treasure SerArtifact, initialised if class == TREASURE_ARTIFACT*/
#ifdef __GNUC__
} SerTreasure;
#else
} SerTreasure;
#pragma pack(pop)
#endif

/**
 * Serialized Shop. Packed struct.
 * Can be turned into a Shop with deser_Shop().
 * Can be obtained from a Shop with ser_Shop().
 * @see Shop
 * @see deser_Shop()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerShop {
#else
#pragma pack(push, 1)
typedef struct SerShop {
#endif
    int8_t itemCount;     /**< Total number of items for sale*/

    int8_t equipsCount;	 /**< Total number of equips for sale*/
    SerEquip equips[EQUIP_SHOP_MAX];     /**< Array for equips on sale*/
    int8_t equipPrices[EQUIP_SHOP_MAX];	 /**< Array for equips prices*/

    int8_t consumablesCount;     /**< Total number of consumables for sale, includes quantity for each kind of consumable*/
    int8_t uniqueConsumablesCount;	    /**< Total of unique consumables for sale*/
    SerConsumable consumables[CONSUMABLE_SHOP_MAX];     /**< Array for consumables on sale*/
    int8_t consumablePrices[CONSUMABLE_SHOP_MAX];	   /**< Array for consumables prices. They assume to a consumable with qty == 1*/
#ifdef __GNUC__
} SerShop;
#else
} SerShop;
#pragma pack(pop)
#endif

/**
 * Serialized Roadfork. Packed struct.
 * Can be turned into a Roadfork with deser_Roadfork().
 * Can be obtained from a Roadfork with ser_Roadfork().
 * @see Roadfork
 * @see deser_Roadfork()
 * @see ser_Roadfork()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerRoadfork {
#else
#pragma pack(push, 1)
typedef struct SerRoadfork {
#endif
    int32_t options[2];	   /**< Defines which choices the fork has.*/
#ifdef __GNUC__
} SerRoadfork;
#else
} SerRoadfork;
#pragma pack(pop)
#endif

/**
 * Serialized Room. Packed struct.
 * Can be turned into a Room with deser_Room().
 * Can be obtained from a Room with ser_Room().
 * @see Room
 * @see deser_Room()
 * @see ser_Room()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerRoom {
#else
#pragma pack(push, 1)
typedef struct SerRoom {
#endif
    int8_t index;	   /**< The room's own number.*/
    int32_t class;	 /**< Defines which kind of roomClass the instance relates to*/
    SerShop shop;	    /**< The SerShop, initialised for rooms of class SHOP.*/
    SerRoadfork roadfork;	    /**< The SerRoadfork, initialised for rooms of class ROADFORK.*/
    SerTreasure treasure;	    /**< The SerTreasure, initialised for rooms of class TREASURE.*/

    SerBoss boss;	    /**< The SerBoss, initialised for rooms of class BOSS.*/
    int8_t enemyTotal;	/**< Total number of enemies initialised in enemies array.*/
    SerEnemy enemies[ROOM_ENEMIES_MAX];	  /**< Array for enemies.*/
    SerFoeParty foes;	/**< The SerFoeParty pointer, initialised for rooms of class ENEMIES.*/
#ifdef __GNUC__
} SerRoom;
#else
} SerRoom;
#pragma pack(pop)
#endif

/**
 * Serialized Floor. Packed struct.
 * Can be turned into a Floor with deser_Floor().
 * Can be obtained from a Floor with ser_Floor().
 * @see Floor
 * @see deser_Floor()
 * @see ser_Floor()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerFloor {
#else
#pragma pack(push, 1)
typedef struct SerFloor {
#endif
    int8_t index;	   /**< The floor's own number.*/

    int32_t class;	  /**< Defines which kind of floorClass the instance relates to*/
    int8_t floor_layout[FLOOR_MAX_COLS][FLOOR_MAX_ROWS];	  /**< Defines the layout for the Floor (which cells are actually filled).*/
    int8_t area;	  /**< Holds how many cells we succesfully random walked.*/
    //SerRoom rooms_matrix[FLOOR_MAX_COLS][FLOOR_MAX_ROWS];	    /**< SerRoom matrix for rooms of this floor.*/
    int32_t roomclass_layout[FLOOR_MAX_COLS][FLOOR_MAX_ROWS];	    /**< roomClass matrix for class value for rooms of this floor.*/
    int8_t explored_matrix[FLOOR_MAX_COLS][FLOOR_MAX_ROWS];     /**< Int matrix for explored value for rooms of this floor.*/
    int8_t explored_area;	   /**< Holds how many cells we explored.*/
    bool from_bsp; /**< Flag for floors generated using bsp.*/
#ifdef __GNUC__
} SerFloor;
#else
} SerFloor;
#pragma pack(pop)
#endif

/**
 * Serialized Wincon. Packed struct.
 * Can be turned into a Wincon with deser_Wincon().
 * Can be obtained from a Wincon with ser_Wincon().
 * @see Wincon
 * @see deser_Wincon()
 * @see ser_Wincon()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerWincon {
#else
#pragma pack(push, 1)
typedef struct SerWincon {
#endif
    int32_t class;	   /**< Defines which kind of winconClass the instance relates to.*/
    int8_t current_val;	 /**< Defines the current progress.*/
    int8_t target_val;	/**< Defines the total progress.*/
#ifdef __GNUC__
} SerWincon;
#else
} SerWincon;
#pragma pack(pop)
#endif

/**
 * Serialized Saveslot. Packed struct.
 * Can be turned into a Saveslot with deser_Saveslot().
 * Can be obtained from a Saveslot with ser_Saveslot().
 * @see Saveslot
 * @see deser_Saveslot()
 * @see ser_Saveslot()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerSaveslot {
#else
#pragma pack(push, 1)
typedef struct SerSaveslot {
#endif
    char name[50];     /**< Name string for the saveslot.*/
    char save_path[255];     /**< Path to savefile*/
    int8_t index; /**< Index of saveslot.*/
#ifdef __GNUC__
} SerSaveslot;
#else
} SerSaveslot;
#pragma pack(pop)
#endif

#define SERPATH_SEED_BUFSIZE PATH_SEED_BUFSIZE

/**
 * Serialized SerPath. Packed struct.
 * Can be turned into a Path with deser_Path().
 * Can be obtained from a Path with ser_Path().
 * @see Path
 * @see deser_Path()
 * @see ser_Path()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerPath {
#else
#pragma pack(push, 1)
typedef struct SerPath {
#endif
    int8_t length;	    /**< Defines how many rooms there are in total.*/
    int8_t luck;	  /**< Defines global luck value.*/
    int8_t prize;	   /**< Defines the reward for getting to length*/
    SerWincon win_condition;     /**< Defines the win condition for the current game.*/
    SerSaveslot current_saveslot;	    /**< Defines current SerSaveslot for the game.*/
    char seed[SERPATH_SEED_BUFSIZE+1]; /**< Holds seed for current run.*/
    int64_t rng_advancements; /**< Current advancements for rng.*/
#ifdef __GNUC__
} SerPath;
#else
} SerPath;
#pragma pack(pop)
#endif

/**
 * Serialized Gamestate. Packed struct.
 * Can be turned into a Gamestate with deser_Gamestate().
 * Can be obtained from a Gamestate with ser_Gamestate().
 * @see Gamestate
 * @see deser_Gamestate()
 * @see ser_Gamestate()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerGamestate {
#else
#pragma pack(push, 1)
typedef struct SerGamestate {
#endif
    clock_t start_time; /**< Keeps track of game start time.*/

    SerCountstats stats;	   /**< Keeps track of stats for the game.*/

    int8_t current_fighters;     /**< Keeps track of Fighter number for the game.*/
    int32_t current_roomtype;	    /**< Keeps track of current room type.*/
    int8_t current_room_index;	/**< Keeps track of current room index.*/

    int8_t current_enemy_index;	 /**< Keeps track of current enemy index.*/
    SerWincon wincon;	/**< Keeps track of current Wincon.*/

    SerPath path;	    /**< Keeps track of current Path.*/

    SerFighter player;	 /**< Keeps track of current Player.*/

    int32_t gamemode;	   /**< Keeps track of current Gamemode.*/

    SerFloor current_floor; /**< Pointer to current floor, initialised when gamemode==Rogue.*/

    SerRoom current_room; /**< Pointer to current room.*/
    bool is_seeded; /**< Denotes if the current game was started with a set seed.*/
#ifdef __GNUC__
} SerGamestate;
#else
} SerGamestate;
#pragma pack(pop)
#endif

/**
 * Serialized GameOptions. Packed struct.
 * Can be turned into a GameOptions with deser_GameOptions().
 * Can be obtained from a GameOptions with ser_GameOptions().
 * @see SerGameOptions
 * @see deser_GameOptions()
 * @see ser_GameOptions()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerGameOptions {
#else
#pragma pack(push, 1)
typedef struct SerGameOptions {
#endif
    bool use_default_background; //<** Turn on usage of default terminal background */
    bool do_autosave; //<* Turns on autosave */
    int32_t directional_keys_schema; //* Defines the current schema for cardinal directions movement */
#ifdef __GNUC__
} SerGameOptions;
#else
} SerGameOptions;
#pragma pack(pop)
#endif

bool appendSerTurncounter(const char* filename, SerTurncounter* data);

bool readSerTurncounter(const char* filename, size_t offset, SerTurncounter* data);

bool deser_Turncounter(SerTurncounter* ser, Turncounter* deser);
bool ser_Turncounter(Turncounter* deser, SerTurncounter* ser);

bool deser_Perk(SerPerk* ser, Perk* deser);
bool ser_Perk(Perk* deser, SerPerk* ser);

bool deser_Skillslot(SerSkillslot* ser, Skillslot* deser);
bool ser_Skillslot(Skillslot* deser, SerSkillslot* ser);

bool deser_Equip(SerEquip* ser, Equip* deser);
bool ser_Equip(Equip* deser, SerEquip* ser);

bool deser_Equipslot(SerEquipslot* ser, Equipslot* deser);
bool ser_Equipslot(Equipslot* deser, SerEquipslot* ser);

bool deser_Specialslot(SerSpecialslot* ser, Specialslot* deser);
bool ser_Specialslot(Specialslot* deser, SerSpecialslot* ser);

bool deser_Consumable(SerConsumable* ser, Consumable* deser);
bool ser_Consumable(Consumable* deser, SerConsumable* ser);

bool deser_Artifact(SerArtifact* ser, Artifact* deser);
bool ser_Artifact(Artifact* deser, SerArtifact* ser);

bool deser_countStats(SerCountstats* ser, countStats* deser);
bool ser_countStats(countStats* deser, SerCountstats* ser);

bool deser_Enemy(SerEnemy* ser, Enemy* deser);
bool ser_Enemy(Enemy* deser, SerEnemy* ser);

bool deser_Boss(SerBoss* ser, Boss* deser);
bool ser_Boss(Boss* deser, SerBoss* ser);

bool deser_Fighter(SerFighter* ser, Fighter* deser);
bool ser_Fighter(Fighter* deser, SerFighter* ser);

bool deser_FoeParty(SerFoeParty* ser, FoeParty* deser);
bool ser_FoeParty(FoeParty* deser, SerFoeParty* ser);

bool deser_Chest(SerChest* ser, Chest* deser);
bool ser_Chest(Chest* deser, SerChest* ser);

bool deser_Treasure(SerTreasure* ser, Treasure* deser);
bool ser_Treasure(Treasure* deser, SerTreasure* ser);

bool deser_Shop(SerShop* ser, Shop* deser);
bool ser_Shop(Shop* deser, SerShop* ser);

bool deser_Roadfork(SerRoadfork* ser, Roadfork* deser);
bool ser_Roadfork(Roadfork* deser, SerRoadfork* ser);

bool deser_Room(SerRoom* ser, Room* deser);
bool ser_Room(Room* deser, SerRoom* ser);

bool deser_Floor(SerFloor* ser, Floor* deser);
bool ser_Floor(Floor* deser, SerFloor* ser);

bool deser_Wincon(SerWincon* ser, Wincon* deser);
bool ser_Wincon(Wincon* deser, SerWincon* ser);

bool deser_Saveslot(SerSaveslot* ser, Saveslot* deser);
bool ser_Saveslot(Saveslot* deser, SerSaveslot* ser);

bool deser_Path(SerPath* ser, Path* deser);
bool ser_Path(Path* deser, SerPath* ser);

bool writeSerGamestate(const char* filename, SerGamestate* data);
bool appendSerGamestate(const char* filename, SerGamestate* data);
bool readSerGamestate(const char* filename, size_t offset, SerGamestate* data);

bool deser_Gamestate(SerGamestate* ser, Gamestate* deser);
bool ser_Gamestate(Gamestate* deser, SerGamestate* ser);
bool prep_Gamestate(Gamestate* gmst, const char* static_path, size_t offset, Koliseo* kls, bool force_init);
bool read_savedir(const char* dirpath);

bool deser_GameOptions(SerGameOptions* ser, GameOptions* deser);
bool ser_GameOptions(GameOptions* deser, SerGameOptions* ser);
bool writeSerGameOptions(const char* filename, SerGameOptions* data);
bool appendSerGameOptions(const char* filename, SerGameOptions* data);
bool readSerGameOptions(const char* filename, size_t offset, SerGameOptions* data);
bool prep_GameOptions(GameOptions* game_options, const char* static_path, size_t offset, Koliseo* kls, bool force_init);
#endif // SER_CORE_H
