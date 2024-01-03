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

#ifndef ARTIFACTS_H
#define ARTIFACTS_H

#include "game_core.h"
#include "game_utils.h"
#include <stdio.h>
#include <unistd.h>

void artifact_thinkingskull(Fighter * f, Enemy * e, Boss * b, int mode);
void artifact_twinkie(Fighter * f, Enemy * e, Boss * b, int mode);
void artifact_wristband(Fighter * f, Enemy * e, Boss * b, int mode);
void artifact_boartail(Fighter * f, Enemy * e, Boss * b, int mode);
void artifact_chaosorb(Fighter * f, Enemy * e, Boss * b, int mode);
void artifact_powersyphon(Fighter * f, Enemy * e, Boss * b, int mode);
void artifact_giantfossile(Fighter * f, Enemy * e, Boss * b, int mode);

#endif
