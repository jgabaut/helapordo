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
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Turncounter was NULL.", __func__);
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

bool deser_Perk(SerPerk* ser, Perk* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerPerk was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Perk was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->class = ser->class;
    deser->innerValue = ser->innerValue;
    return true;
}
bool ser_Perk(Perk* deser, SerPerk* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerPerk was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Perk was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->class = deser->class;
    ser->innerValue = deser->innerValue;
    return true;
}

bool deser_Skillslot(SerSkillslot* ser, Skillslot* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerSkillslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Skillslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->class = ser->class;
    deser->enabled = ser->enabled;
    return true;
}

bool ser_Skillslot(Skillslot* deser, SerSkillslot* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerSkillslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Skillslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->class = deser->class;
    ser->enabled = deser->enabled;
    return true;
}

bool deser_Equip(SerEquip* ser, Equip* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerEquip was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Equip was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->class = ser->class;
    deser->type = ser->type;
    deser->equipped = ser->equipped;
    deser->level = ser->level;
    deser->atk = ser->atk;
    deser->def = ser->def;
    deser->vel = ser->vel;
    deser->enr = ser->enr;
    deser->perksCount = ser->perksCount;
    deser->cost = ser->cost;
    deser->qual = ser->qual;
    bool perk_deser_res = false;
    for (size_t i=0; i<EQUIPPERKSMAX; i++) {
        perk_deser_res = deser_Perk(&ser->perks[i], deser->perks[i]);
        if (!perk_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Perk(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

bool ser_Equip(Equip* deser, SerEquip* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerEquip was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Equip was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->class = deser->class;
    ser->type = deser->type;
    ser->equipped = deser->equipped;
    ser->level = deser->level;
    ser->atk = deser->atk;
    ser->def = deser->def;
    ser->vel = deser->vel;
    ser->enr = deser->enr;
    ser->perksCount = deser->perksCount;
    ser->cost = deser->cost;
    ser->qual = deser->qual;
    bool perk_ser_res = false;
    for (size_t i=0; i<EQUIPPERKSMAX; i++) {
        perk_ser_res = ser_Perk(deser->perks[i], &ser->perks[i]);
        if (!perk_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Perk(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

bool deser_Equipslot(SerEquipslot* ser, Equipslot* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerEquipslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Equipslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->type = ser->type;
    deser->active = ser->active;
    return true;
}

bool ser_Equipslot(Equipslot* deser, SerEquipslot* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerEquipslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Equipslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->type = deser->type;
    ser->active = deser->active;
    return true;
}

bool deser_Specialslot(SerSpecialslot* ser, Specialslot* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerSpecialslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Specialslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->enabled = ser->enabled;
    deser->move = ser->move;
    deser->cost = ser->cost;
    return true;
}

bool ser_Specialslot(Specialslot* deser, SerSpecialslot* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerSpecialslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Specialslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->enabled = deser->enabled;
    ser->move = deser->move;
    ser->cost = deser->cost;
    return true;
}

bool deser_Consumable(SerConsumable* ser, Consumable* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerConsumable was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Consumable was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->class = ser->class;
    deser->qty = ser->qty;
    return true;
}

bool ser_Consumable(Consumable* deser, SerConsumable* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerConsumable was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Consumable was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->class = deser->class;
    ser->qty = deser->qty;
    return true;
}

bool deser_Artifact(SerArtifact* ser, Artifact* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerArtifact was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Artifact was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->class = ser->class;
    deser->qty = ser->qty;
    deser->active = ser->active;
    deser->innerValue = ser->innerValue;
    return true;
}

bool ser_Artifact(Artifact* deser, SerArtifact* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerArtifact was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Artifact was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->class = deser->class;
    ser->qty = deser->qty;
    ser->active = deser->active;
    ser->innerValue = deser->innerValue;
    return true;
}

bool deser_countStats(SerCountstats* ser, countStats* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerCountstats was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed countStats was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    deser->enemieskilled = ser->enemieskilled;
    deser->consumablesfound = ser->consumablesfound;
    deser->equipsfound = ser->equipsfound;
    deser->artifactsfound = ser->artifactsfound;
    deser->criticalhits = ser->criticalhits;
    deser->roomscompleted = ser->roomscompleted;
    deser->floorscompleted = ser->floorscompleted;
    deser->specialsunlocked = ser->specialsunlocked;
    deser->coinsfound = ser->coinsfound;
    deser->bosseskilled = ser->bosseskilled;
    deser->unique_bosseskilled = ser->unique_bosseskilled;
    for (size_t i=0; i<BOSSCLASSESMAX+1; i++) {
        deser->killed_bosses[i] = ser->killed_bosses[i];
    }
    deser->keysfound = ser->keysfound;
    return true;
}

bool ser_countStats(countStats* deser, SerCountstats* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerCountstats was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed countStats was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    ser->enemieskilled = deser->enemieskilled;
    ser->consumablesfound = deser->consumablesfound;
    ser->equipsfound = deser->equipsfound;
    ser->artifactsfound = deser->artifactsfound;
    ser->criticalhits = deser->criticalhits;
    ser->roomscompleted = deser->roomscompleted;
    ser->floorscompleted = deser->floorscompleted;
    ser->specialsunlocked = deser->specialsunlocked;
    ser->coinsfound = deser->coinsfound;
    ser->bosseskilled = deser->bosseskilled;
    ser->unique_bosseskilled = deser->unique_bosseskilled;
    for (size_t i=0; i<BOSSCLASSESMAX+1; i++) {
        ser->killed_bosses[i] = deser->killed_bosses[i];
    }
    ser->keysfound = deser->keysfound;
    return true;
}
