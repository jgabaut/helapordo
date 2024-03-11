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

bool appendSerTurncounter(const char* filename, SerTurncounter* data);

bool readSerTurncounter(const char* filename, size_t offset, SerTurncounter* data);

bool deser_Turncounter(SerTurncounter* ser, Turncounter* deser);

#endif // SER_CORE_H
