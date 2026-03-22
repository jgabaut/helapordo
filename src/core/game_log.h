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

#ifndef GAME_LOG_H
#define GAME_LOG_H
#include "game_core.h"
#include "game_strings.h"
//#include "game_strings.h"
#ifdef _WIN32
void log_Win_EnvVars(void);
#endif // _WIN32
void log_tag(char *filename, char *header, const char *format, ...);
void log_OP(turnOption_OP op);
void hlpd_reset_logfile(void);
#endif // GAME_LOG_H
