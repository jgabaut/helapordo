#include "ser_core.h"

bool appendSerTurncounter(const char* filename, SerTurncounter* data) {

    FILE* file = fopen(filename, "ab");

    if (file != NULL) {
        // Write the structure to the file
        //
        int64_t header_size = sizeof(SerTurncounter);

        fwrite(&header_size, sizeof(header_size), 1, file);
        fwrite(data, sizeof(SerTurncounter), 1, file);

        // Close the file
        fclose(file);
    } else {
        fprintf(stderr, "%s(): Error opening file {%s} for writing", __func__, filename);
        log_tag("debug_log.txt", "[ERROR]", "%s():    Error opening file {%s} for writing", __func__, filename);
        return false;
    }
    return true;
}

bool readSerTurncounter(const char* filename, size_t offset, SerTurncounter* data) {
    FILE* file = fopen(filename, "rb");

    if (file != NULL) {
        size_t tot_length = -1;
        fseek(file, 0, SEEK_END);
        tot_length = ftell(file);
        int64_t blob_size = -1;

        if (tot_length < offset ) {
#ifdef _WIN32
            log_tag("debug_log.txt", "[ERROR]", "%s():    Total file size {%lli} is less than passed offset {%lli}", __func__, tot_length, offset);
#else
            log_tag("debug_log.txt", "[ERROR]", "%s():    Total file size {%li} is less than passed offset {%li}", __func__, tot_length, offset);
#endif
            fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }

        size_t remaining_length = -1;
        fseek(file, offset, SEEK_SET);
        remaining_length = ftell(file);

        fread(&blob_size, sizeof(blob_size), 1, file);
        size_t sertc_size = sizeof(SerTurncounter);

#ifdef WIN_32
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Read blob size: {%lli}", __func__, blob_size);
        log_tag("debug_log.txt", "[DEBUG]", "%s():    SerTurncounter size: {%lli}", __func__, sizeof(SerTurncounter));
#else
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Read header size: {%li}", __func__, blob_size);
        log_tag("debug_log.txt", "[DEBUG]", "%s():    SerSaveHeader size: {%li}", __func__, sizeof(SerTurncounter));
#endif

        if (blob_size != sertc_size) {
            log_tag("debug_log.txt", "[ERROR]", "%s():    Header size from {%s} doesn't match SemTurnCounter size.", __func__, filename);
            if (blob_size < sertc_size) {

#ifdef WIN_32
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is less than expected {%lli}.", __func__, blob_size, sertc_size);
#else
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is less than expected {%li}.", __func__, blob_size, sertc_size);
#endif
                fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            } else {

#ifdef WIN_32
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is greater than expected {%lli}.", __func__, blob_size, sertc_size);
#else
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is greater than expected {%li}.", __func__, blob_size, sertc_size);
#endif
                fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
        }
        // Update len
        remaining_length -= sizeof(blob_size);


        if (remaining_length <= blob_size) {

#ifdef WIN_32
            log_tag("debug_log.txt", "[ERROR]", "%s():    Remaining file length {%lli} is less than stored header size {%lli}.", __func__, remaining_length, blob_size);
#else
            log_tag("debug_log.txt", "[ERROR]", "%s():    Remaining file length {%li} is less than stored header size {%li}.", __func__, remaining_length, blob_size);
#endif
            fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }

        size_t expected_len = sizeof(SerTurncounter);
        if (remaining_length < expected_len) {

#ifdef WIN_32
            log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is less than expected {%lli}.", __func__, remaining_length, expected_len);
#else
            log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is less than expected {%li}.", __func__, remaining_length, expected_len);
#endif
            fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        } else {

#ifdef WIN_32
            log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is greater than expected {%lli}.", __func__, remaining_length, expected_len);
#else
            log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is greater than expected {%li}.", __func__, remaining_length, expected_len);
#endif
        }

        // Read the structure from the file
        fread(data, sizeof(SerTurncounter), 1, file);

        // Close the file
        fclose(file);
    } else {
        fprintf(stderr, "%s(): Error opening file {%s} for reading", __func__, filename);
        log_tag("debug_log.txt", "[ERROR]", "%s():    Error opening file {%s} for reading", __func__, filename);
        return false;
    }
    return true;
}

bool deser_Turncounter(SerTurncounter* ser, Turncounter* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerTurncounter was NULL.", __func__);
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
    deser->count = ser->count;
    deser->innerValue = ser->innerValue;
    //TODO: check type here?
    deser->type = ser->type;
    return true;
}
