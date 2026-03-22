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
#include "game_log.h"

#ifdef _WIN32
/**
 * Logs Windows envvars to debug log file.
 */
void log_Win_EnvVars(void)
{
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "Windows Environment vars:");
    log_tag("debug_log.txt", "[WIND32-DEBUG", "UserProfile: { %s }",
            getenv("UserProfile"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "HomeDrive: { %s }",
            getenv("HomeDrive"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "HomePath: { %s }",
            getenv("HomePath"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "ComputerName: { %s }",
            getenv("ComputerName"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "Processor_Revision: { %s }",
            getenv("Processor_Revision"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "Processor_Identifier: { %s }",
            getenv("Processor_Identifier"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "Processor_Level: { %s }",
            getenv("Processor_Level"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "Number_Of_Processors: { %s }",
            getenv("Number_Of_Processors"));
    log_tag("debug_log.txt", "[WIN32-DEBUG]", "OS: { %s }", getenv("OS"));
}
#endif // _WIN32

/**
 * Takes a filename string, a string headear and a format string.
 * Tries logging the message to the passed file if global var G_DEBUG_ON is set.
 * @param filename The filename to open.
 * @param header The string header for the message to log.
 * @param format The format string for message.
 */
void log_tag(char *filename, char *header, const char *format, ...)
{
#ifndef HELAPORDO_DEBUG_LOG
#else
    // Open log file if log flag is set and append to it
    if (G_LOG_ON == 1) {
        char path_to_debug_file[500];
        char static_path[500];

        // Set static_path value to the correct static dir path
        resolve_staticPath(static_path);

        sprintf(path_to_debug_file, "%s", static_path);

#ifndef _WIN32
        strncat(path_to_debug_file, "/", 2);
#else
        strncat(path_to_debug_file, "\\", 2);
#endif

        strncat(path_to_debug_file, filename, 200);

        //fprintf(stderr, "Using %s as path to debug log.\n", path_to_debug_file);

        FILE *logfile = fopen(path_to_debug_file, "a");
        if (!logfile) {
            fprintf(stderr,
                    "Error opening log file.\n Static path: (%s) Filename : (%s).\n",
                    static_path, filename);
            fprintf(stderr, "Path to debug file was: (%s).",
                    path_to_debug_file);
            fprintf(stderr, "Format was:    %s", format);
            exit(EXIT_FAILURE);
        }
        va_list args;
        va_start(args, format);
        time_t now = time(0);
        struct tm *mytime = localtime(&now);
        char timeheader[500];
        if (strftime(timeheader, sizeof timeheader, "%X", mytime)) {
            fprintf(logfile, "[ %s ] [ %-12.12s ] [", timeheader, header);
            vfprintf(logfile, format, args);
            fprintf(logfile, "]\n");
        }
        va_end(args);
        fclose(logfile);
    }
#endif
}

/**
 * Logs a turnOption_OP to "$STATICDIR/OPS_log.txt".
 * @param op The turnOption_OP to log.
 */
void log_OP(turnOption_OP op)
{
    log_tag(OPS_LOGFILE, "[LOG]", "[OP_code: %i, value: %s]", op,
            stringFromTurnOP(op));
    log_tag("debug_log.txt", "[LOG_TURNOP]", "[OP_code: %i, value: %s]", op,
            stringFromTurnOP(op));
}

void hlpd_reset_logfile(void)
{
#ifndef HELAPORDO_DEBUG_LOG
#else
    FILE *debug_file = NULL;
    FILE *OPS_debug_file = NULL;
    // Open log file if log flag is set and reset it
    if (G_LOG_ON == 1) {
        char path_to_debug_file[600];
        char path_to_OPS_debug_file[600];
        char static_path[500];
        // Set static_path value to the correct static dir path
        resolve_staticPath(static_path);

        //Truncate "debug_log.txt"
        sprintf(path_to_debug_file, "%s/%s", static_path, "debug_log.txt");
        debug_file = fopen(path_to_debug_file, "w");
        if (!debug_file) {
#ifdef HELAPORDO_CURSES_BUILD
            endwin();	//TODO: Can/should we check if we have to do this only in curses mode?
#endif // HELAPORDO_CURSES_BUILD
            fprintf(stderr,
                    "[ERROR]    Can't open debug logfile (%s/debug_log.txt).\n",
                    static_path);
            exit(EXIT_FAILURE);
        }
        fprintf(debug_file, "[DEBUGLOG]    --New game--  \n");
#ifdef HELAPORDO_CURSES_BUILD
        if (NCURSES_VERSION_MAJOR < EXPECTED_NCURSES_VERSION_MAJOR
            || (NCURSES_VERSION_MAJOR == EXPECTED_NCURSES_VERSION_MAJOR && NCURSES_VERSION_MINOR < EXPECTED_NCURSES_VERSION_MINOR)
            || (NCURSES_VERSION_MAJOR == EXPECTED_NCURSES_VERSION_MAJOR && NCURSES_VERSION_MINOR == EXPECTED_NCURSES_VERSION_MINOR && NCURSES_VERSION_PATCH < EXPECTED_NCURSES_VERSION_PATCH)) {
            fprintf(debug_file,
                    "[WARN]    ncurses version is lower than expected {%s: %i.%i.%i} < {%i.%i.%i}\n",
                    NCURSES_VERSION, NCURSES_VERSION_MAJOR,
                    NCURSES_VERSION_MINOR, NCURSES_VERSION_PATCH,
                    EXPECTED_NCURSES_VERSION_MAJOR,
                    EXPECTED_NCURSES_VERSION_MINOR,
                    EXPECTED_NCURSES_VERSION_PATCH);
        }
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined"
#else
        if (RAYLIB_VERSION_MAJOR < EXPECTED_RAYLIB_VERSION_MAJOR
            && RAYLIB_VERSION_MINOR < EXPECTED_RAYLIB_VERSION_MINOR
            && RAYLIB_VERSION_PATCH < EXPECTED_RAYLIB_VERSION_PATCH) {
            fprintf(debug_file,
                    "[WARN]    raylib version is lower than expected {%s: %i.%i.%i} < {%i.%i.%i}\n",
                    RAYLIB_VERSION, RAYLIB_VERSION_MAJOR,
                    RAYLIB_VERSION_MINOR, RAYLIB_VERSION_PATCH,
                    EXPECTED_RAYLIB_VERSION_MAJOR,
                    EXPECTED_RAYLIB_VERSION_MINOR,
                    EXPECTED_RAYLIB_VERSION_PATCH);
        }
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
        fprintf(debug_file, "[DEBUG]    --Default kls debug info:--  \n");
        print_kls_2file(debug_file, default_kls);
        fprintf(debug_file, "[DEBUG]    --Temporary kls debug info:--  \n");
        print_kls_2file(debug_file, temporary_kls);
        fprintf(debug_file,
                "[DEBUG]    --Closing header for new game.--  \n");
        fclose(debug_file);

        //Lay debug info
        log_tag("debug_log.txt", "[DEBUG]", "G_DEBUG_ON == (%i)",
                G_DEBUG_ON);
        log_tag("debug_log.txt", "[DEBUG]", "G_LOG_ON == (%i)", G_LOG_ON);
        log_tag("debug_log.txt", "[DEBUG]", "small DEBUG FLAG ASSERTED");
        log_tag("debug_log.txt", "[DEBUG]",
                "[Current position in default_kls] [pos: %li]\n",
                kls_get_pos(default_kls));

        //Truncate OPS_LOGFILE
        sprintf(path_to_OPS_debug_file, "%s/%s", static_path, OPS_LOGFILE);
        OPS_debug_file = fopen(path_to_OPS_debug_file, "w");
        if (!OPS_debug_file) {
#ifdef HELAPORDO_CURSES_BUILD
            endwin();	//TODO: Can/should we check if we have to do this only in curses mode?
#endif // HELAPORDO_CURSES_BUILD
            fprintf(stderr, "[ERROR]    Can't open OPS logfile (%s/%s).\n",
                    static_path, OPS_LOGFILE);
            exit(EXIT_FAILURE);
        }
        fprintf(OPS_debug_file, "[OPLOG]    --New game--  \n");
        fclose(OPS_debug_file);
        log_tag("debug_log.txt", "[DEBUG]", "Truncated [%s]", OPS_LOGFILE);
    }
#endif
}
