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

bool appendSerTurncounter(const char* filename, SerTurncounter* data);

bool readSerTurncounter(const char* filename, size_t offset, SerTurncounter* data);

bool deser_Turncounter(SerTurncounter* ser, Turncounter* deser);

#endif // SER_CORE_H
