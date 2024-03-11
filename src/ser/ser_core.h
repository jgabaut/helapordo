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

/**
 * Serialized Turncounter. Packed struct.
 * Can be turned into a Turncounter with deser_turnCounter().
 * @see Turncounter
 * @see deser_turnCounter()
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
 * @see Perk
 * @see deser_Perk()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerPerk {
#else
#pragma pack(push, 1)
typedef struct SerPerk {
#endif
    perkClass class;	  /**< Defines which kind of Perk the instance relates to*/
    int32_t innerValue;	/**< Contains an integer for perks that need a state*/
#ifdef __GNUC__
} SerPerk;
#else
} SerPerk;
#pragma pack(pop)
#endif

/**
 * Serialized Skillslot. Packed struct.
 * Can be turned into a Skillslot with deser_Skillslot().
 * @see Skillslot
 * @see deser_Skillslot()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerSkillslot {
#else
#pragma pack(push, 1)
typedef struct SerSkillslot {
#endif
    skillType class;	  /**< Defines which kind of Skillslot the instance relates to*/
    int32_t enabled;	/**< Flag defining if the slot is initialised.*/
#ifdef __GNUC__
} SerSkillslot;
#else
} SerSkillslot;
#pragma pack(pop)
#endif

/**
 * Serialized Equip. Packed struct.
 * Can be turned into an Equip with deser_Equip().
 * @see Equip
 * @see deser_Equip()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerEquip {
#else
#pragma pack(push, 1)
typedef struct SerEquip {
#endif
    equipClass class;	  /**< Defines which kind of Equip the instance relates to*/
    Equipzone type;	/**< Defines which kind of Equipzone the instance relates to*/
    int32_t equipped; /**< Flag indicating the instance as currently in use if set.*/
    int32_t level;
    int32_t atk;
    int32_t def;
    int32_t vel;
    int32_t enr;
    int32_t bonus;
    int32_t perksCount;
    int32_t cost;
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
 * @see Equipslot
 * @see deser_Equipslot()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerEquipslot {
#else
#pragma pack(push, 1)
typedef struct SerEquipslot {
#endif
    Equipzone type;	/**< Defines which kind of Equipzone the instance relates to*/
    int32_t active; /**< Flag indicating the instance as currently in use if set.*/
#ifdef __GNUC__
} SerEquipslot;
#else
} SerEquipslot;
#pragma pack(pop)
#endif

/**
 * Serialized Specialslot. Packed struct.
 * Can be turned into a Specialslot with deser_Specialslot().
 * @see Specialslot
 * @see deser_Specialslot()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerSpecialslot {
#else
#pragma pack(push, 1)
typedef struct SerSpecialslot {
#endif
    int32_t enabled;     /**< Flag defining if the current slot is initialised*/
    specialMove move;	  /**< Defines which kind of specialMove the slot is holding*/
    int32_t cost;	  /**< Cost of use*/
#ifdef __GNUC__
} SerSpecialslot;
#else
} SerSpecialslot;
#pragma pack(pop)
#endif

/**
 * Serialized Consumable. Packed struct.
 * Can be turned into a Consumable with deser_Consumable().
 * @see Consumable
 * @see deser_Consumable()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerConsumable {
#else
#pragma pack(push, 1)
typedef struct SerConsumable {
#endif
    consumableClass class;     /**< Defines which kind of Consumable this is*/
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
 * @see Artifact
 * @see deser_Artifact()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerArtifact {
#else
#pragma pack(push, 1)
typedef struct SerArtifact {
#endif
    artifactClass class;     /**< Defines which kind of artifactClass the instance relates to*/
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
 * @see countStats
 * @see deser_countStats()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerCountstats {
#else
#pragma pack(push, 1)
typedef struct SerCountstats {
#endif
    int32_t enemieskilled;	   /**< How many enemies were defeated*/
    int32_t consumablesfound;     /**< How many consumabls were found*/
    int32_t equipsfound;	 /**< How many equips were found*/
    int32_t artifactsfound;	    /**< How many artifacts were found*/
    int32_t criticalhits;	  /**< How many criticals were dealt*/
    int32_t roomscompleted;	    /**< How many rooms were completed*/
    int32_t floorscompleted;     /**< How many floors were completed*/
    int32_t specialsunlocked;     /**< How many special move were unlocked*/
    int32_t coinsfound;	/**< How many coins you found in total*/
    int32_t bosseskilled;	  /**< How many bosses were defeated*/
    int32_t unique_bosseskilled;	 /**< How many unique bosses were defeated*/
    int32_t killed_bosses[BOSSCLASSESMAX + 1];   /**< Keeps track of which bossClass you've killed.*/
    int32_t keysfound;     /**< How many keys you found in total*/
#ifdef __GNUC__
} SerCountstats;
#else
} SerCountstats;
#pragma pack(pop)
#endif

/**
 * Serialized Enemy. Packed struct.
 * Can be turned into an Enemy with deser_Enemy().
 * @see Enemy
 * @see deser_Enemy()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerEnemy {
#else
#pragma pack(push, 1)
typedef struct SerEnemy {
#endif
    int32_t index;	   /**< Defines position inside containing foeParty.*/
    enemyClass class;	  /**< Defines which kind of enemyClass the instance relates to*/
    int32_t hp;	/**< Current hp value*/
    int32_t atk;	 /**< Current atk value*/
    int32_t def;	 /**< Current def value*/
    int32_t vel;	 /**< Current vel value*/
    int32_t level;	   /**< Level value*/
    int32_t luck;	  /**< Luck value*/
    int32_t xp;	/**< Xp value given on death*/
    int32_t totalhp;     /**< Full hp value*/

    int32_t energy;	    /**< Current energy value*/
    int32_t totalenergy;	 /**< Full energy value*/

    int32_t stamina;     /**< Current stamina value*/
    int32_t totalstamina;	  /**< Full stamina value*/

    fighterStatus status;     /**< Defines active fighterStatus*/
    int32_t beast;	   /**< Flag defining the instance as "beast" if set*/

    SerSkillslot skills[ENEMY_SKILL_SLOTS + 1];   /**< Array with all the SerSkillslot*/

    SerTurncounter counters[COUNTERSMAX + 1];   /**< Array with all the SerTurncounter*/
    int32_t turnboost_atk;	   /**< Current temp boost value for atk*/
    int32_t turnboost_def;	   /**< Current temp boost value for def*/
    int32_t turnboost_vel;	   /**< Current temp boost value for vel*/
    int32_t turnboost_enr;	   /**< Current temp boost value for enr*/

    int32_t prize;	   /**< Amount of currency dropped on defeat*/
#ifdef __GNUC__
} SerEnemy;
#else
} SerEnemy;
#pragma pack(pop)
#endif

/**
 * Serialized Boss. Packed struct.
 * Can be turned into an Boss with deser_Boss().
 * @see Boss
 * @see deser_Boss()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerBoss {
#else
#pragma pack(push, 1)
typedef struct SerBoss {
#endif
    bossClass class;	 /**< Defines which kind of enemyClass the instance relates to*/
    int32_t hp;	/**< Current hp value*/
    int32_t atk;	 /**< Current atk value*/
    int32_t def;	 /**< Current def value*/
    int32_t vel;	 /**< Current vel value*/
    int32_t level;	   /**< Level value*/
    int32_t luck;	  /**< Luck value*/
    int32_t xp;	/**< Xp value given on death*/
    int32_t totalhp;     /**< Full hp value*/

    int32_t energy;	    /**< Current energy value*/
    int32_t totalenergy;	 /**< Full energy value*/

    int32_t stamina;     /**< Current stamina value*/
    int32_t totalstamina;	  /**< Full stamina value*/

    fighterStatus status;     /**< Defines active fighterStatus*/
    int32_t beast;	   /**< Flag defining the instance as "beast" if set*/

    SerSkillslot skills[BOSS_SKILL_SLOTS + 1];   /**< Array with all the SerSkillslot*/
    SerTurncounter counters[COUNTERSMAX + 1];   /**< Array with all the SerTurncounter*/
    int32_t turnboost_atk;	   /**< Current temp boost value for atk*/
    int32_t turnboost_def;	   /**< Current temp boost value for def*/
    int32_t turnboost_vel;	   /**< Current temp boost value for vel*/
    int32_t turnboost_enr;	   /**< Current temp boost value for enr*/

    int32_t prize;	   /**< Amount of currency dropped on defeat*/
#ifdef __GNUC__
} SerBoss;
#else
} SerBoss;
#pragma pack(pop)
#endif

/**
 * Serialized Fighter. Packed struct.
 * Can be turned into an Fighter with deser_Fighter().
 * @see Fighter
 * @see deser_Fighter()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerFighter {
#else
#pragma pack(push, 1)
typedef struct SerFighter {
#endif
    char name[50];     /**< Name string*/
    fighterClass class;	    /**< Defines which kind of fighterClass the instance relates to*/
    int32_t hp;	/**< Current hp value*/
    int32_t atk;	 /**< Current atk value*/
    int32_t def;	 /**< Current def value*/
    int32_t vel;	 /**< Current vel value*/
    int32_t level;	   /**< Current level value*/
    int32_t luck;	  /**< Current luck value*/
    int32_t totalxp;     /**< Lifetime xp value*/
    int32_t currentlevelxp;	    /**< Xp gained for the current level*/
    int32_t totallevelxp;	  /**< Xp needed to level up*/
    int32_t totalhp;     /**< Full hp value*/
    fighterStatus status;     /**< Defines active fighterStatus*/

    int32_t energy;	    /**< Current energy value*/
    int32_t totalenergy;	 /**< Full energy value*/
    int32_t stamina;     /**< Current stamina value*/
    int32_t totalstamina;	  /**< Full stamina value*/
    SerSpecialslot specials[SPECIALSMAX + 1];   /**< Array with all the SerSpecialslot*/
    SerSkillslot skills[FIGHTER_SKILL_SLOTS + 1];   /**< Array with all the SerSkillslot*/

    SerTurncounter counters[COUNTERSMAX + 1];   /**< Array with all the SerTurncounter*/
    int32_t turnboost_atk;	   /**< Current temp boost value for atk*/
    int32_t turnboost_def;	   /**< Current temp boost value for def*/
    int32_t turnboost_vel;	   /**< Current temp boost value for vel*/
    int32_t turnboost_enr;	   /**< Current temp boost value for enr*/

    int32_t perksCount;	/**< Keeps track of how many perks are active.*/
    SerPerk perks[PERKSMAX + 1];	 /**< Array with all the SerPerk*/

    SerEquipslot equipslots[EQUIPZONES + 1];   /**< Array with all the SerEquipslot*/
    SerEquip equipsBag[EQUIPSBAGSIZE + 1];	  /**< Array with all the SerEquip found*/
    SerConsumable consumablesBag[CONSUMABLESMAX + 1];   /**< Array with all the Consumables found*/
    SerArtifact artifactsBag[ARTIFACTSMAX + 1];   /**< Array with all the Artifacts found*/

    int32_t equipsBagOccupiedSlots;	    /**< Keeps track of how many slots are occupied.*/
    int32_t earliestBagSlot;	/**< Keeps track of first available spot.*///TODO To always use the array efficiently (???) I sense linked lists may be better

    int32_t permboost_atk;	   /**< Current temp boost value for atk.*/
    int32_t permboost_def;	   /**< Current temp boost value for def.*/
    int32_t permboost_vel;	   /**< Current temp boost value for vel.*/
    int32_t permboost_enr;	   /**< Current temp boost value for enr.*/

    int32_t equipboost_atk;	    /**< Current equip boost value for atk.*/
    int32_t equipboost_def;	    /**< Current equip boost value for def.*/
    int32_t equipboost_vel;	    /**< Current equip boost value for vel.*/
    int32_t equipboost_enr;	    /**< Current equip boost value for enr.*/

    SerCountstats stats;	    /**< SerCountstats instance*/

    int32_t balance;     /**< Amount of currency owned*/
    int32_t keys_balance;	  /**< Amount of keys owned*/
#ifdef __GNUC__
} SerFighter;
#else
} SerFighter;
#pragma pack(pop)
#endif

/**
 * Serialized FoeParty. Packed struct.
 * Can be turned into an FoeParty with deser_FoeParty().
 * @see FoeParty
 * @see deser_FoeParty()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerFoeParty {
#else
#pragma pack(push, 1)
typedef struct SerFoeParty {
#endif
    foePartyClass class;     /**< Defines which kind of foePartyClass the instance relates to*/
    int32_t level;	   /**< Level for the whole FoeParty*/
    SerTurncounter counters[COUNTERSMAX + 1];   /**< Array with all the SerTurncounter*/
    int32_t turnboost_atk;	   /**< Current temp boost value for atk*/
    int32_t turnboost_def;	   /**< Current temp boost value for def*/
    int32_t turnboost_vel;	   /**< Current temp boost value for vel*/
    int32_t turnboost_enr;	   /**< Current temp boost value for enr*/
    SerEnemy enemy_foes[ROOM_ENEMIES_MAX + 1];   /**< Array for all enemies in a room*/
    //TODO: this is not ideal. Could we ever need to have a different max for enemy_foes? Only if the group is not in a Room??
    SerBoss boss_foes[FOES_BOSSES_MAX + 1];   /**< Array of for all bosses in a room*/
    int32_t size;	  /**<  Number of members in the FoeParty.*/
    int32_t tot_alive;     /**<  Number of alive members of the FoeParty.*/
    int32_t current_index;	   /**< Index of current foe advancement*/
    int32_t alive_enemies[ROOM_ENEMIES_MAX + 1];   /**< Array of integers with 1 for alive enemies in that position*/
    int32_t alive_bosses[FOES_BOSSES_MAX + 1];   /**< Array of integers with 1 for alive bosses in that position*/
#ifdef __GNUC__
} SerFoeParty;
#else
} SerFoeParty;
#pragma pack(pop)
#endif

/**
 * Serialized Chest. Packed struct.
 * Can be turned into a Chest with deser_Chest().
 * @see Chest
 * @see deser_Chest()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerChest {
#else
#pragma pack(push, 1)
typedef struct SerChest {
#endif
    chestClass class;	  /**< Defines which kind of chesteClass the instance relates to*/
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
 * @see Treasure
 * @see deser_Treasure()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerTreasure {
#else
#pragma pack(push, 1)
typedef struct SerTreasure {
#endif
    treasureClass class;     /**< Defines which kind of treasureClass the instance relates to*/
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
 * @see Shop
 * @see deser_Shop()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerShop {
#else
#pragma pack(push, 1)
typedef struct SerShop {
#endif
    int32_t itemCount;     /**< Total number of items for sale*/

    int32_t equipsCount;	 /**< Total number of equips for sale*/
    SerEquip equips[EQUIP_SHOP_MAX];     /**< Array for equips on sale*/
    int32_t equipPrices[EQUIP_SHOP_MAX];	 /**< Array for equips prices*/

    int32_t consumablesCount;     /**< Total number of consumables for sale, includes quantity for each kind of consumable*/
    int32_t uniqueConsumablesCount;	    /**< Total of unique consumables for sale*/
    SerConsumable consumables[CONSUMABLE_SHOP_MAX];     /**< Array for consumables on sale*/
    int32_t consumablePrices[CONSUMABLE_SHOP_MAX];	   /**< Array for consumables prices. They assume to a consumable with qty == 1*/
#ifdef __GNUC__
} SerShop;
#else
} SerShop;
#pragma pack(pop)
#endif

/**
 * Serialized Roadfork. Packed struct.
 * Can be turned into a Roadfork with deser_Roadfork().
 * @see Roadfork
 * @see deser_Roadfork()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerRoadfork {
#else
#pragma pack(push, 1)
typedef struct SerRoadfork {
#endif
    roadforkOption options[2];	   /**< Defines which choices the fork has.*/
#ifdef __GNUC__
} SerRoadfork;
#else
} SerRoadfork;
#pragma pack(pop)
#endif

/**
 * Serialized Room. Packed struct.
 * Can be turned into a Room with deser_Room().
 * @see Room
 * @see deser_Room()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerRoom {
#else
#pragma pack(push, 1)
typedef struct SerRoom {
#endif
    int32_t index;	   /**< The room's own number.*/
    roomClass class;	 /**< Defines which kind of roomClass the instance relates to*/
    SerShop shop;	    /**< The SerShop, initialised for rooms of class SHOP.*/
    SerRoadfork roadfork;	    /**< The SerRoadfork, initialised for rooms of class ROADFORK.*/
    SerTreasure treasure;	    /**< The SerTreasure, initialised for rooms of class TREASURE.*/

    SerBoss boss;	    /**< The SerBoss, initialised for rooms of class BOSS.*/
    int32_t enemyTotal;	/**< Total number of enemies initialised in enemies array.*/
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
 * @see Floor
 * @see deser_Floor()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerFloor {
#else
#pragma pack(push, 1)
typedef struct SerFloor {
#endif
    int32_t index;	   /**< The floor's own number.*/

    floorClass class;	  /**< Defines which kind of floorClass the instance relates to*/
    int32_t floor_layout[FLOOR_MAX_COLS][FLOOR_MAX_ROWS];	  /**< Defines the layout for the Floor (which cells are actually filled).*/
    int32_t area;	  /**< Holds how many cells we succesfully random walked.*/
    SerRoom rooms_matrix[FLOOR_MAX_COLS][FLOOR_MAX_ROWS];	    /**< SerRoom matrix for rooms of this floor.*/
    roomClass roomclass_layout[FLOOR_MAX_COLS][FLOOR_MAX_ROWS];	    /**< roomClass matrix for class value for rooms of this floor.*/
    int32_t explored_matrix[FLOOR_MAX_COLS][FLOOR_MAX_ROWS];     /**< Int matrix for explored value for rooms of this floor.*/
    int32_t explored_area;	   /**< Holds how many cells we explored.*/
#ifdef __GNUC__
} SerFloor;
#else
} SerFloor;
#pragma pack(pop)
#endif

/**
 * Serialized Wincon. Packed struct.
 * Can be turned into a Wincon with deser_Wincon().
 * @see Wincon
 * @see deser_Wincon()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerWincon {
#else
#pragma pack(push, 1)
typedef struct SerWincon {
#endif
    winconClass class;	   /**< Defines which kind of winconClass the instance relates to.*/
    int32_t current_val;	 /**< Defines the current progress.*/
    int32_t target_val;	/**< Defines the total progress.*/
#ifdef __GNUC__
} SerWincon;
#else
} SerWincon;
#pragma pack(pop)
#endif

/**
 * Serialized Saveslot. Packed struct.
 * Can be turned into a Saveslot with deser_Saveslot().
 * @see Saveslot
 * @see deser_Saveslot()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerSaveslot {
#else
#pragma pack(push, 1)
typedef struct SerSaveslot {
#endif
    char name[50];     /**< Name string for the saveslot.*/
    char save_path[255];     /**< Path to savefile*/
#ifdef __GNUC__
} SerSaveslot;
#else
} SerSaveslot;
#pragma pack(pop)
#endif

/**
 * Serialized SerPath. Packed struct.
 * Can be turned into a Path with deser_Path().
 * @see Path
 * @see deser_Path()
 */
#ifdef __GNUC__
typedef struct __attribute__((packed)) SerPath {
#else
#pragma pack(push, 1)
typedef struct SerPath {
#endif
    int32_t length;	    /**< Defines how many rooms there are in total.*/
    int32_t luck;	  /**< Defines global luck value.*/
    int32_t prize;	   /**< Defines the reward for getting to length*/
    int32_t loreCounter;	 /**< Counts how many lore prompts have been displayed*/
    SerWincon win_condition;     /**> Defines the win condition for the current game.*/
    SerSaveslot current_saveslot;	    /** Defines current SerSaveslot for the game.*/
#ifdef __GNUC__
} SerPath;
#else
} SerPath;
#pragma pack(pop)
#endif


bool appendSerTurncounter(const char* filename, SerTurncounter* data);

bool readSerTurncounter(const char* filename, size_t offset, SerTurncounter* data);

bool deser_Turncounter(SerTurncounter* ser, Turncounter* deser);

#endif // SER_CORE_H
