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

#ifndef BINSAVES_H_
#define BINSAVES_H_

#include <inttypes.h>
#include "game_utils.h"

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

SaveHeader* prep_saveHeader(const char* static_path, Koliseo* kls);
#endif // BINSAVES_H_
