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
#include "bin_saves.h"

/**
 * Writes passed SerSaveHeader to passed file path.
 * @param filename The path to write to.
 * @param data The structure to write.
 * @see SerSaveHeader
 * @return True on success
 */
bool writeSerSaveHeader(const char* filename, SerSaveHeader* data)
{
    FILE* file = fopen(filename, "wb");

    if (file != NULL) {
        // Write the structure to the file
        //
        int64_t header_size = sizeof(SerSaveHeader);

        fwrite(&header_size, sizeof(header_size), 1, file);
        fwrite(data, sizeof(SerSaveHeader), 1, file);

        // Close the file
        fclose(file);
    } else {
        fprintf(stderr, "%s(): Error opening file {%s} for writing\n", __func__, filename);
        log_tag("debug_log.txt", "[ERROR]", "%s():    Error opening file {%s} for writing", __func__, filename);
        return false;
    }
    return true;
}

/**
 * Reads SerSaveHeader from passed file path into passed structure.
 * @param filename The path to read from.
 * @param data The structure to read into.
 * @see SerSaveHeader
 * @return True on success
 */
bool readSerSaveHeader(const char* filename, SerSaveHeader* data)
{
    FILE* file = fopen(filename, "rb");

    if (file != NULL) {
        size_t length = -1;
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);

        int64_t header_size = -1;
        fread(&header_size, sizeof(header_size), 1, file);
        size_t sersh_size = sizeof(SerSaveHeader);

#ifdef WIN_32
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Read header size: {%lli}", __func__, header_size);
        log_tag("debug_log.txt", "[DEBUG]", "%s():    SerSaveHeader size: {%lli}", __func__, sersh_size);
#else
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Read header size: {%li}", __func__, header_size);
        log_tag("debug_log.txt", "[DEBUG]", "%s():    SerSaveHeader size: {%li}", __func__, sersh_size);
#endif

        if (header_size != sersh_size) {
            log_tag("debug_log.txt", "[ERROR]", "%s():    Header size from {%s} doesn't match SemSaveHeader size.", __func__, filename);
            if (header_size < sersh_size) {

#ifdef WIN_32
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is less than expected {%lli}.", __func__, header_size, sersh_size);
#else
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is less than expected {%li}.", __func__, header_size, sersh_size);
#endif
                fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            } else {

#ifdef WIN_32
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is greater than expected {%lli}.", __func__, header_size, sersh_size);
#else
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is greater than expected {%li}.", __func__, header_size, sersh_size);
#endif
                fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
        }

        // Update len
        length -= sizeof(header_size);

        if (length < header_size) {
#ifdef WIN_32
            log_tag("debug_log.txt", "[ERROR]", "%s():    Remaining file length {%lli} is less than stored header size {%lli}.", __func__, length, header_size);
#else
            log_tag("debug_log.txt", "[ERROR]", "%s():    Remaining file length {%li} is less than stored header size {%li}.", __func__, length, header_size);
#endif
            fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }

#ifdef WIN_32
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Read file size: {%lli}", __func__, length);
        log_tag("debug_log.txt", "[DEBUG]", "%s():    SerSaveHeader size: {%lli}", __func__, sersh_size);
#else
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Read file size: {%li}", __func__, length);
        log_tag("debug_log.txt", "[DEBUG]", "%s():    SerSaveHeader size: {%li}", __func__, sersh_size);
#endif

        size_t expected_len = sizeof(SerSaveHeader);
        if (length != expected_len) {
            log_tag("debug_log.txt", "[ERROR]", "%s():    File {%s} size doesn't match expected size.", __func__, filename);
            if (length < expected_len) {

#ifdef WIN_32
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is less than expected {%lli}.", __func__, length, expected_len);
#else
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is less than expected {%li}.", __func__, length, expected_len);
#endif
                fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            } else {

#ifdef WIN_32
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is greater than expected {%lli}.", __func__, length, expected_len);
#else
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is greater than expected {%li}.", __func__, length, expected_len);
#endif
            }
        }

        // Read the structure from the file
        fread(data, sizeof(SerSaveHeader), 1, file);

        // Close the file
        fclose(file);
    } else {
        fprintf(stderr, "%s(): Error opening file {%s} for reading\n", __func__, filename);
        log_tag("debug_log.txt", "[ERROR]", "%s():    Error opening file {%s} for reading", __func__, filename);
        return false;
    }
    return true;
}

/**
 * Converts passed SerSaveHeader into a SaveHeader.
 * @param ser The SerSaveHeader to convert.
 * @param deser The SaveHeader destination.
 * @see SerSaveHeader
 * @see SaveHeader
 * @return True on success
 */
bool deser_SaveHeader(SerSaveHeader* ser, SaveHeader* deser)
{
    // ALL strings in the SerSaveHeader should be NULL-terminated
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerSaveHeader was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): koliseo was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->api_level = ser->api_level;
    ser->save_version[SERSAVEHEADER_BUFSIZE] = '\0';
    memcpy(deser->save_version, ser->save_version, SAVEHEADER_BUFSIZE);
    deser->save_version[SAVEHEADER_BUFSIZE] = '\0';
    ser->game_version[SERSAVEHEADER_BUFSIZE] = '\0';
    memcpy(deser->game_version, ser->game_version, SAVEHEADER_BUFSIZE);
    deser->game_version[SAVEHEADER_BUFSIZE] = '\0';
    ser->os[SERSAVEHEADER_BUFSIZE] = '\0';
    memcpy(deser->os, ser->os, SAVEHEADER_BUFSIZE);
    deser->os[SAVEHEADER_BUFSIZE] = '\0';
    ser->machine[SERSAVEHEADER_BUFSIZE] = '\0';
    memcpy(deser->machine, ser->machine, SAVEHEADER_BUFSIZE);
    deser->machine[SAVEHEADER_BUFSIZE] = '\0';

    return true;
}

/**
 * Tries reading binary save from passed path.
 * @param static_path The path to which we append to find our file.
 * @param kls Koliseo used for allocation.
 * @param force_init When true, forces the write of a new SerSaveHeader.
 * @param did_init Set to true when a new saveHeader is written.
 * @param saveslot_index Used to pass current saveslot index.
 * @see SerSaveHeader
 * @see SaveHeader
 * @return The newly allocated SaveHeader.
 * TODO Contract should meaningfully capture case of read failure + init.
 */
SaveHeader* prep_saveHeader(const char* static_path, Koliseo* kls, bool force_init, bool* did_init, int saveslot_index)
{
    if (kls == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): koliseo as NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    char path_to_bin_savefile[1000];
    char bin_savefile_name[300];

    //Copy current_save_path
#ifdef HELAPORDO_CURSES_BUILD
    sprintf(bin_savefile_name, "%s", CURSES_BINSAVE_NAME);
#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else
    sprintf(bin_savefile_name, "%s", RL_BINSAVE_NAME);
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD

    sprintf(path_to_bin_savefile, "%s/%s/%s", static_path, default_saveslots[saveslot_index].save_path, bin_savefile_name);
    if (force_init) {
        log_tag("debug_log.txt", "[BINSAVE]", "%s():    Forcing init of binsave at {%s}.", __func__, path_to_bin_savefile);
        *did_init = true;
        // Failed reading existing binsave, create a new one
        SerSaveHeader ser_saveheader = {
            .api_level = HELAPORDO_API_VERSION_INT,
            .game_version = VERSION,
            .save_version = HELAPORDO_BINSAVEFILE_VERSION,
            .os = HELAPORDO_OS,
            .machine = HELAPORDO_MACHINE,
        };

        // Write packed structure to a binary file
        bool write_res = writeSerSaveHeader(path_to_bin_savefile, &ser_saveheader);

        if (!write_res) {
            // Failed writing new binsave
            fprintf(stderr, "%s():    Failed to create a binsave.\n", __func__);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        } else {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): success for forced writeSerSaveHeader()", __func__);
        }

        SaveHeader* save_head = KLS_PUSH(kls, SaveHeader);
        bool deser_result = deser_SaveHeader(&ser_saveheader, save_head);
        if (!deser_result) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): failed deser_SaveHeader().", __func__);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        } else {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): success for deser_SaveHeader()", __func__);
        }
        //log_tag("debug_log.txt", "[BINSAVE]", "Initialised Data: api_level=%" PRId32 ", save_version=%s, game_version=%s, os=%s, machine=%s", save_head->api_level, save_head->save_version, save_head->game_version, save_head->os, save_head->machine);
        return save_head;
    }

    Koliseo_Temp* kls_t = kls_temp_start(kls);
    // Try reading an existing binsave
    SerSaveHeader* read_SSH = KLS_PUSH_T(kls_t, SerSaveHeader);
    bool read_res = readSerSaveHeader(path_to_bin_savefile, read_SSH);

    if (!read_res) {
        kls_temp_end(kls_t);
        log_tag("debug_log.txt", "[BINSAVE]", "Failed reading binsave at {%s}, creating a new one.", path_to_bin_savefile);
        *did_init = true;
        // Failed reading existing binsave, create a new one
        SerSaveHeader ser_saveheader = {
            .api_level = HELAPORDO_API_VERSION_INT,
            .game_version = VERSION,
            .save_version = HELAPORDO_BINSAVEFILE_VERSION,
            .os = HELAPORDO_OS,
            .machine = HELAPORDO_MACHINE,
        };

        // Write packed structure to a binary file
        bool write_res = writeSerSaveHeader(path_to_bin_savefile, &ser_saveheader);

        if (!write_res) {
            // Failed writing new binsave
            fprintf(stderr, "%s():    Failed to create a binsave.\n", __func__);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        } else {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): success for writeSerSaveHeader()", __func__);
        }

        SaveHeader* save_head = KLS_PUSH(kls, SaveHeader);
        bool deser_result = deser_SaveHeader(&ser_saveheader, save_head);
        if (!deser_result) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): failed deser_SaveHeader().", __func__);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        } else {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): success for deser_SaveHeader()", __func__);
        }
        //log_tag("debug_log.txt", "[BINSAVE]", "Initialised Data: api_level=%" PRId32 ", save_version=%s, game_version=%s, os=%s, machine=%s", save_head->api_level, save_head->save_version, save_head->game_version, save_head->os, save_head->machine);
        return save_head;
    } else {
        SerSaveHeader tmp = (SerSaveHeader) {
            .api_level = read_SSH->api_level,
        };
        read_SSH->save_version[SERSAVEHEADER_BUFSIZE] = '\0';
        memcpy(tmp.save_version, read_SSH->save_version, SERSAVEHEADER_BUFSIZE);
        read_SSH->game_version[SERSAVEHEADER_BUFSIZE] = '\0';
        memcpy(tmp.game_version, read_SSH->game_version, SERSAVEHEADER_BUFSIZE);
        tmp.game_version[SERSAVEHEADER_BUFSIZE] = '\0';
        read_SSH->os[SERSAVEHEADER_BUFSIZE] = '\0';
        memcpy(tmp.os, read_SSH->os, SERSAVEHEADER_BUFSIZE);
        read_SSH->machine[SERSAVEHEADER_BUFSIZE] = '\0';
        memcpy(tmp.machine, read_SSH->machine, SERSAVEHEADER_BUFSIZE);
        kls_temp_end(kls_t);

        if ((strcmp(tmp.save_version, HELAPORDO_BINSAVEFILE_VERSION) != 0)) {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): mismatch on read_SSH->save_version.", __func__);
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): Expected {%s}, found {%s}", __func__, HELAPORDO_BINSAVEFILE_VERSION, tmp.save_version);
        }

        if ((strcmp(tmp.game_version, VERSION) != 0)) {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): mismatch on read_SSH->game_version.", __func__);
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): Expected {%s}, found {%s}", __func__, VERSION, tmp.game_version);
        }

        if ((strcmp(tmp.os, HELAPORDO_OS) != 0)) {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): mismatch on read_SSH->os.", __func__);
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): Expected {%s}, found {%s}", __func__, HELAPORDO_OS, tmp.os);
        }
        if ((strcmp(tmp.machine, HELAPORDO_MACHINE) != 0)) {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): mismatch on read_SSH->machine.", __func__);
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): Expected {%s}, found {%s}", __func__, HELAPORDO_MACHINE, tmp.machine);
        }

        SaveHeader* save_head = KLS_PUSH(kls, SaveHeader);

        bool deser_result = deser_SaveHeader(&tmp, save_head);
        if (!deser_result) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): failed deser_SaveHeader().", __func__);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        } else {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): success for deser_SaveHeader()", __func__);
        }

        if (save_head == NULL) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): save_head was NULL after deser_SaveHeader().", __func__);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }

        //log_tag("debug_log.txt", "[BINSAVE]", "Read Data: api_level=%" PRId32 ", save_version=%s, game_version=%s, os=%s, machine=%s", save_head->api_level, save_head->save_version, save_head->game_version, save_head->os, save_head->machine);
        return save_head;
    }
}
