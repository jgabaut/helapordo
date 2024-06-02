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
#include "saves.h"

#ifdef HELAPORDO_CURSES_BUILD
bool set_Saveslot_name(Saveslot *sv)
{

    //printf("\nLoading game...\n");
    log_tag("debug_log.txt", "[LOAD]", "%s():    Starting loading from savefile.", __func__);

    char buf[500];

    log_tag("debug_log.txt", "[LOAD]", "%s():    Doing binary file loading.", __func__);
    char path_to_sv_file[800];
    char static_path[500];
    // Set static_path value to the correct static dir path
    resolve_staticPath(static_path);

#ifndef _WIN32
    sprintf(path_to_sv_file, "%s/%s/%s", static_path,
            sv->save_path, "run-nc.bin");
#else
    sprintf(path_to_sv_file, "%s\\%s\\%s", static_path,
            sv->save_path, "run-nc.bin");
#endif
    SerGamestate ser_gmst = {0};
    bool read_res = readSerGamestate(path_to_sv_file, 0, &ser_gmst);
    if (!read_res) {
        log_tag("debug_log.txt", "[WARN]", "%s():    Failed readSerGamestate() for {%s}.", __func__, path_to_sv_file);
        return false;
    } else {
        ser_gmst.player.name[49] = '\0';
        memcpy(buf, ser_gmst.player.name, 50);
    }
    buf[49] = '\0';
    memcpy(sv->name, buf, 50);
    sv->name[49] = '\0';
    return true;
}
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined."
#else
bool set_Saveslot_name(FILE *file, Saveslot *sv)
{
    (void) file;
    (void) sv;
    printf("%s():    TODO - implement set_Saveslot_namefor rl-build.\n", __func__);
    return false;
}
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
