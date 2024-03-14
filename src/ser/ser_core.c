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

#ifdef _WIN32
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Read blob size: {%lli}", __func__, blob_size);
        log_tag("debug_log.txt", "[DEBUG]", "%s():    SerTurncounter size: {%lli}", __func__, sizeof(SerTurncounter));
#else
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Read header size: {%li}", __func__, blob_size);
        log_tag("debug_log.txt", "[DEBUG]", "%s():    SerTurncounter size: {%li}", __func__, sizeof(SerTurncounter));
#endif

        if (blob_size != sertc_size) {
            log_tag("debug_log.txt", "[ERROR]", "%s():    Header size from {%s} doesn't match SerTurnCounter size.", __func__, filename);
            if (blob_size < sertc_size) {

#ifdef _WIN32
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is less than expected {%lli}.", __func__, blob_size, sertc_size);
#else
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is less than expected {%li}.", __func__, blob_size, sertc_size);
#endif
                fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            } else if (blob_size > sertc_size) {

#ifdef _WIN32
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


        if (remaining_length < blob_size) {

#ifdef _WIN32
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

#ifdef _WIN32
            log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is less than expected {%lli}.", __func__, remaining_length, expected_len);
#else
            log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is less than expected {%li}.", __func__, remaining_length, expected_len);
#endif
            fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        } else if (remaining_length > expected_len) {
#ifdef _WIN32
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

bool ser_Turncounter(Turncounter* deser, SerTurncounter* ser) {
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
    ser->count = deser->count;
    ser->innerValue = deser->innerValue;
    //TODO: check type here?
    ser->type = deser->type;
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
        return false;
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
        return false;
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
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Equip was NULL. Putting zeros.", __func__);
        *ser = (SerEquip){0};
        return true;
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
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Perk(). Index: {%li}. Putting zeros.", __func__, i);
            ser->perks[i] = (SerPerk){0};
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

bool deser_Enemy(SerEnemy* ser, Enemy* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerEnemy was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Enemy was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->index = ser->index;
    deser->class = ser->class;
    deser->hp = ser->hp;
    deser->atk = ser->atk;
    deser->def = ser->def;
    deser->vel = ser->vel;
    deser->level = ser->level;
    deser->luck = ser->luck;
    deser->xp = ser->xp;
    deser->totalhp = ser->totalhp;
    deser->energy = ser->energy;
    deser->totalenergy = ser->totalenergy;
    deser->stamina = ser->stamina;
    deser->totalstamina = ser->totalstamina;
    deser->status = ser->status;
    deser->beast = ser->beast;
    bool skillslot_deser_res = false;
    for (size_t i=0; i<ENEMY_SKILL_SLOTS+1; i++) {
        skillslot_deser_res = deser_Skillslot(&ser->skills[i], deser->skills[i]);
        if (!skillslot_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Skillslot(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    bool turncounter_deser_res = false;
    for (size_t i=0; i<COUNTERSMAX+1; i++) {
        turncounter_deser_res = deser_Turncounter(&ser->counters[i], deser->counters[i]);
        if (!turncounter_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Turncounter(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    deser->turnboost_atk = ser->turnboost_atk;
    deser->turnboost_def = ser->turnboost_def;
    deser->turnboost_vel = ser->turnboost_vel;
    deser->turnboost_enr = ser->turnboost_enr;
    deser->prize = ser->prize;
    return true;
}

bool ser_Enemy(Enemy* deser, SerEnemy* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerEnemy was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Enemy was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->index = deser->index;
    ser->class = deser->class;
    ser->hp = deser->hp;
    ser->atk = deser->atk;
    ser->def = deser->def;
    ser->vel = deser->vel;
    ser->level = deser->level;
    ser->luck = deser->luck;
    ser->xp = deser->xp;
    ser->totalhp = deser->totalhp;
    ser->energy = deser->energy;
    ser->totalenergy = deser->totalenergy;
    ser->stamina = deser->stamina;
    ser->totalstamina = deser->totalstamina;
    ser->status = deser->status;
    ser->beast = deser->beast;
    bool skillslot_ser_res = false;
    for (size_t i=0; i<ENEMY_SKILL_SLOTS+1; i++) {
        skillslot_ser_res = ser_Skillslot(deser->skills[i], &ser->skills[i]);
        if (!skillslot_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Skillslot(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    bool turncounter_ser_res = false;
    for (size_t i=0; i<COUNTERSMAX+1; i++) {
        turncounter_ser_res = ser_Turncounter(deser->counters[i], &ser->counters[i]);
        if (!turncounter_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Turncounter(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    ser->turnboost_atk = deser->turnboost_atk;
    ser->turnboost_def = deser->turnboost_def;
    ser->turnboost_vel = deser->turnboost_vel;
    ser->turnboost_enr = deser->turnboost_enr;
    ser->prize = deser->prize;
    return true;
}

bool deser_Boss(SerBoss* ser, Boss* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerBoss was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Boss was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->class = ser->class;
    deser->hp = ser->hp;
    deser->atk = ser->atk;
    deser->def = ser->def;
    deser->vel = ser->vel;
    deser->level = ser->level;
    deser->luck = ser->luck;
    deser->xp = ser->xp;
    deser->totalhp = ser->totalhp;
    deser->energy = ser->energy;
    deser->totalenergy = ser->totalenergy;
    deser->stamina = ser->stamina;
    deser->totalstamina = ser->totalstamina;
    deser->status = ser->status;
    deser->beast = ser->beast;
    bool skillslot_deser_res = false;
    for (size_t i=0; i<ENEMY_SKILL_SLOTS+1; i++) {
        skillslot_deser_res = deser_Skillslot(&ser->skills[i], deser->skills[i]);
        if (!skillslot_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Skillslot(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    bool turncounter_deser_res = false;
    for (size_t i=0; i<COUNTERSMAX+1; i++) {
        turncounter_deser_res = deser_Turncounter(&ser->counters[i], deser->counters[i]);
        if (!turncounter_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Turncounter(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    deser->turnboost_atk = ser->turnboost_atk;
    deser->turnboost_def = ser->turnboost_def;
    deser->turnboost_vel = ser->turnboost_vel;
    deser->turnboost_enr = ser->turnboost_enr;
    deser->prize = ser->prize;
    return true;
}

bool ser_Boss(Boss* deser, SerBoss* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerBoss was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Boss was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->class = deser->class;
    ser->hp = deser->hp;
    ser->atk = deser->atk;
    ser->def = deser->def;
    ser->vel = deser->vel;
    ser->level = deser->level;
    ser->luck = deser->luck;
    ser->xp = deser->xp;
    ser->totalhp = deser->totalhp;
    ser->energy = deser->energy;
    ser->totalenergy = deser->totalenergy;
    ser->stamina = deser->stamina;
    ser->totalstamina = deser->totalstamina;
    ser->status = deser->status;
    ser->beast = deser->beast;
    bool skillslot_ser_res = false;
    for (size_t i=0; i<ENEMY_SKILL_SLOTS+1; i++) {
        skillslot_ser_res = ser_Skillslot(deser->skills[i], &ser->skills[i]);
        if (!skillslot_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Skillslot(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    bool turncounter_ser_res = false;
    for (size_t i=0; i<COUNTERSMAX+1; i++) {
        turncounter_ser_res = ser_Turncounter(deser->counters[i], &ser->counters[i]);
        if (!turncounter_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Turncounter(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    ser->turnboost_atk = deser->turnboost_atk;
    ser->turnboost_def = deser->turnboost_def;
    ser->turnboost_vel = deser->turnboost_vel;
    ser->turnboost_enr = deser->turnboost_enr;
    ser->prize = deser->prize;
    return true;
}

bool deser_Fighter(SerFighter* ser, Fighter* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerFighter was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Fighter was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->name[SERFIGHTER_NAME_BUFSIZE-1] = '\0';
    memcpy(deser->name, ser->name, FIGHTER_NAME_BUFSIZE-1);
    deser->name[FIGHTER_NAME_BUFSIZE-1] = '\0';
    deser->class = ser->class;
    deser->hp = ser->hp;
    deser->atk = ser->atk;
    deser->def = ser->def;
    deser->vel = ser->vel;
    deser->level = ser->level;
    deser->luck = ser->luck;
    deser->totalxp = ser->totalxp;
    deser->currentlevelxp = ser->currentlevelxp;
    deser->totallevelxp = ser->totallevelxp;
    deser->totalhp = ser->totalhp;
    deser->status = ser->status;
    deser->energy = ser->energy;
    deser->totalenergy = ser->totalenergy;
    deser->stamina = ser->stamina;
    deser->totalstamina = ser->totalstamina;
    bool specialslot_deser_res = false;
    for (size_t i=0; i<SPECIALSMAX+1; i++) {
        specialslot_deser_res = deser_Specialslot(&ser->specials[i], deser->specials[i]);
        if (!specialslot_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Specialslot(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    bool skillslot_deser_res = false;
    for (size_t i=0; i<ENEMY_SKILL_SLOTS+1; i++) {
        skillslot_deser_res = deser_Skillslot(&ser->skills[i], deser->skills[i]);
        if (!skillslot_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Skillslot(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    bool turncounter_deser_res = false;
    for (size_t i=0; i<COUNTERSMAX+1; i++) {
        turncounter_deser_res = deser_Turncounter(&ser->counters[i], deser->counters[i]);
        if (!turncounter_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Turncounter(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    deser->turnboost_atk = ser->turnboost_atk;
    deser->turnboost_def = ser->turnboost_def;
    deser->turnboost_vel = ser->turnboost_vel;
    deser->turnboost_enr = ser->turnboost_enr;

    deser->perksCount = ser->perksCount;

    bool perk_deser_res = false;
    for (size_t i=0; i<PERKSMAX; i++) {
        perk_deser_res = deser_Perk(&ser->perks[i], deser->perks[i]);
        if (!perk_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Perk(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    bool equipslot_deser_res = false;
    for (size_t i=0; i<EQUIPZONES+1; i++) {
        equipslot_deser_res = deser_Equipslot(&ser->equipslots[i], deser->equipslots[i]);
        if (!equipslot_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Equipslot(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    bool equips_deser_res = false;
    for (size_t i=0; i<EQUIPSBAGSIZE+1; i++) {
        equips_deser_res = deser_Equip(&ser->equipsBag[i], deser->equipsBag[i]);
        if (!equips_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Equip(). Index: {%li}", __func__, i);
            deser->equipsBag[i] = NULL;
        }
    }

    bool consumables_deser_res = false;
    for (size_t i=0; i<CONSUMABLESMAX+1; i++) {
        consumables_deser_res = deser_Consumable(&ser->consumablesBag[i], deser->consumablesBag[i]);
        if (!consumables_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Consumable(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    bool artifacts_deser_res = false;
    for (size_t i=0; i<ARTIFACTSMAX+1; i++) {
        artifacts_deser_res = deser_Artifact(&ser->artifactsBag[i], deser->artifactsBag[i]);
        if (!artifacts_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Artifact(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    deser->equipsBagOccupiedSlots = ser->equipsBagOccupiedSlots;
    deser->earliestBagSlot = ser->earliestBagSlot;
    deser->permboost_atk = ser->permboost_atk;
    deser->permboost_def = ser->permboost_def;
    deser->permboost_vel = ser->permboost_vel;
    deser->permboost_enr = ser->permboost_enr;

    deser->equipboost_atk = ser->equipboost_atk;
    deser->equipboost_def = ser->equipboost_def;
    deser->equipboost_vel = ser->equipboost_vel;
    deser->equipboost_enr = ser->equipboost_enr;

    bool stats_deser_res = deser_countStats(&ser->stats, deser->stats);

    if (!stats_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_countStats()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    deser->balance = ser->balance;
    deser->keys_balance = ser->keys_balance;
    return true;
}

bool ser_Fighter(Fighter* deser, SerFighter* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerFighter was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Fighter was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->name[FIGHTER_NAME_BUFSIZE-1] = '\0';
    memcpy(ser->name, deser->name, SERFIGHTER_NAME_BUFSIZE-1);
    ser->name[SERFIGHTER_NAME_BUFSIZE-1] = '\0';
    ser->class = deser->class;
    ser->hp = deser->hp;
    ser->atk = deser->atk;
    ser->def = deser->def;
    ser->vel = deser->vel;
    ser->level = deser->level;
    ser->luck = deser->luck;
    ser->totalxp = deser->totalxp;
    ser->currentlevelxp = deser->currentlevelxp;
    ser->totallevelxp = deser->totallevelxp;
    ser->totalhp = deser->totalhp;
    ser->status = deser->status;
    ser->energy = deser->energy;
    ser->totalenergy = deser->totalenergy;
    ser->stamina = deser->stamina;
    ser->totalstamina = deser->totalstamina;
    bool specialslot_ser_res = false;
    for (size_t i=0; i<SPECIALSMAX+1; i++) {
        specialslot_ser_res = ser_Specialslot(deser->specials[i], &ser->specials[i]);
        if (!specialslot_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Specialslot(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    bool skillslot_ser_res = false;
    for (size_t i=0; i<ENEMY_SKILL_SLOTS+1; i++) {
        skillslot_ser_res = ser_Skillslot(deser->skills[i], &ser->skills[i]);
        if (!skillslot_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Skillslot(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    bool turncounter_ser_res = false;
    for (size_t i=0; i<COUNTERSMAX+1; i++) {
        turncounter_ser_res = ser_Turncounter(deser->counters[i], &ser->counters[i]);
        if (!turncounter_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Turncounter(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    ser->turnboost_atk = deser->turnboost_atk;
    ser->turnboost_def = deser->turnboost_def;
    ser->turnboost_vel = deser->turnboost_vel;
    ser->turnboost_enr = deser->turnboost_enr;

    ser->perksCount = deser->perksCount;

    bool perk_ser_res = false;
    for (size_t i=0; i<PERKSMAX; i++) {
        perk_ser_res = ser_Perk(deser->perks[i], &ser->perks[i]);
        if (!perk_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Perk(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    bool equipslot_ser_res = false;
    for (size_t i=0; i<EQUIPZONES+1; i++) {
        equipslot_ser_res = ser_Equipslot(deser->equipslots[i], &ser->equipslots[i]);
        if (!equipslot_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Equipslot(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    bool equips_ser_res = false;
    for (size_t i=0; i<EQUIPSBAGSIZE+1; i++) {
        equips_ser_res = ser_Equip(deser->equipsBag[i], &ser->equipsBag[i]);
        if (!equips_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Equip(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    bool consumables_ser_res = false;
    for (size_t i=0; i<CONSUMABLESMAX+1; i++) {
        consumables_ser_res = ser_Consumable(deser->consumablesBag[i], &ser->consumablesBag[i]);
        if (!consumables_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Consumable(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    bool artifacts_ser_res = false;
    for (size_t i=0; i<ARTIFACTSMAX+1; i++) {
        artifacts_ser_res = ser_Artifact(deser->artifactsBag[i], &ser->artifactsBag[i]);
        if (!artifacts_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Artifact(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    ser->equipsBagOccupiedSlots = deser->equipsBagOccupiedSlots;
    ser->earliestBagSlot = deser->earliestBagSlot;
    ser->permboost_atk = deser->permboost_atk;
    ser->permboost_def = deser->permboost_def;
    ser->permboost_vel = deser->permboost_vel;
    ser->permboost_enr = deser->permboost_enr;

    ser->equipboost_atk = deser->equipboost_atk;
    ser->equipboost_def = deser->equipboost_def;
    ser->equipboost_vel = deser->equipboost_vel;
    ser->equipboost_enr = deser->equipboost_enr;

    bool stats_ser_res = ser_countStats(deser->stats, &ser->stats);

    if (!stats_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_countStats()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    ser->balance = deser->balance;
    ser->keys_balance = deser->keys_balance;
    return true;
}

bool deser_FoeParty(SerFoeParty* ser, FoeParty* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerFoeParty was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed FoeParty was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->class = ser->class;
    deser->level = ser->level;

    bool turncounter_deser_res = false;
    for (size_t i=0; i<COUNTERSMAX+1; i++) {
        turncounter_deser_res = deser_Turncounter(&ser->counters[i], deser->counters[i]);
        if (!turncounter_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Turncounter(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    deser->turnboost_atk = ser->turnboost_atk;
    deser->turnboost_def = ser->turnboost_def;
    deser->turnboost_vel = ser->turnboost_vel;
    deser->turnboost_enr = ser->turnboost_enr;

    bool enemies_deser_res = false;
    for (size_t i=0; i<ROOM_ENEMIES_MAX+1; i++) {
        enemies_deser_res = deser_Enemy(&ser->enemy_foes[i], deser->enemy_foes[i]);
        if (!enemies_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Enemy(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    bool bosses_deser_res = false;
    for (size_t i=0; i<FOES_BOSSES_MAX+1; i++) {
        bosses_deser_res = deser_Boss(&ser->boss_foes[i], deser->boss_foes[i]);
        if (!bosses_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Boss(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    deser->size = ser->size;
    deser->tot_alive = ser->tot_alive;
    deser->current_index = ser->current_index;
    for (size_t i=0; i<ROOM_ENEMIES_MAX+1; i++) {
        deser->alive_enemies[i] = ser->alive_enemies[i];
    }
    for (size_t i=0; i<FOES_BOSSES_MAX+1; i++) {
        deser->alive_bosses[i] = ser->alive_bosses[i];
    }
    return true;
}
bool ser_FoeParty(FoeParty* deser, SerFoeParty* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerFoeParty was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed FoeParty was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->class = deser->class;
    ser->level = deser->level;

    bool turncounter_ser_res = false;
    for (size_t i=0; i<COUNTERSMAX+1; i++) {
        turncounter_ser_res = ser_Turncounter(deser->counters[i], &ser->counters[i]);
        if (!turncounter_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Turncounter(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    ser->turnboost_atk = deser->turnboost_atk;
    ser->turnboost_def = deser->turnboost_def;
    ser->turnboost_vel = deser->turnboost_vel;
    ser->turnboost_enr = deser->turnboost_enr;

    bool enemies_ser_res = false;
    for (size_t i=0; i<ROOM_ENEMIES_MAX+1; i++) {
        enemies_ser_res = ser_Enemy(deser->enemy_foes[i], &ser->enemy_foes[i]);
        if (!enemies_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Enemy(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    bool bosses_ser_res = false;
    for (size_t i=0; i<FOES_BOSSES_MAX+1; i++) {
        bosses_ser_res = ser_Boss(deser->boss_foes[i], &ser->boss_foes[i]);
        if (!bosses_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Boss(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    ser->size = deser->size;
    ser->tot_alive = deser->tot_alive;
    ser->current_index = deser->current_index;
    for (size_t i=0; i<ROOM_ENEMIES_MAX+1; i++) {
        ser->alive_enemies[i] = deser->alive_enemies[i];
    }
    for (size_t i=0; i<FOES_BOSSES_MAX+1; i++) {
        ser->alive_bosses[i] = deser->alive_bosses[i];
    }
    return true;
}

bool deser_Chest(SerChest* ser, Chest* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerChest was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Chest was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->class = ser->class;
    deser->consumablesCount = ser->consumablesCount;

    bool consumables_deser_res = false;
    for (size_t i=0; i<CONSUMABLESMAX+1; i++) {
        consumables_deser_res = deser_Consumable(&ser->consumables[i], deser->consumables[i]);
        if (!consumables_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Consumable(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    deser->equipsCount = ser->equipsCount;

    bool equips_deser_res = false;
    for (size_t i=0; i<EQUIPSBAGSIZE+1; i++) {
        equips_deser_res = deser_Equip(&ser->equips[i], deser->equips[i]);
        if (!equips_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Equip(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

bool ser_Chest(Chest* deser, SerChest* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerChest was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Chest was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->class = deser->class;
    ser->consumablesCount = deser->consumablesCount;

    bool consumables_ser_res = false;
    for (size_t i=0; i<CONSUMABLESMAX+1; i++) {
        consumables_ser_res = ser_Consumable(deser->consumables[i], &ser->consumables[i]);
        if (!consumables_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Consumable(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    ser->equipsCount = deser->equipsCount;

    bool equips_ser_res = false;
    for (size_t i=0; i<EQUIPSBAGSIZE+1; i++) {
        equips_ser_res = ser_Equip(deser->equips[i], &ser->equips[i]);
        if (!equips_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Equip(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

bool deser_Treasure(SerTreasure* ser, Treasure* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerTreasure was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Treasure was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->class = ser->class;
    bool chest_deser_res = deser_Chest(&ser->chest, deser->chest);
    if (!chest_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Chest().", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    bool consumable_deser_res = deser_Consumable(&ser->consumable, deser->consumable);
    if (!consumable_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Consumable()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    bool artifact_deser_res = deser_Artifact(&ser->artifact, deser->artifact);
    if (!artifact_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Artifact()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    return true;
}

bool ser_Treasure(Treasure* deser, SerTreasure* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerTreasure was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Treasure was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->class = deser->class;
    bool chest_ser_res = ser_Chest(deser->chest, &ser->chest);
    if (!chest_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Chest().", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    bool consumable_ser_res = ser_Consumable(deser->consumable, &ser->consumable);
    if (!consumable_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Consumable()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    bool artifact_ser_res = ser_Artifact(deser->artifact, &ser->artifact);
    if (!artifact_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Artifact()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    return true;
}

bool deser_Shop(SerShop* ser, Shop* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerShop was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Shop was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    deser->itemCount = ser->itemCount;
    deser->equipsCount = ser->equipsCount;

    bool equips_deser_res = false;
    for (size_t i=0; i<EQUIPSBAGSIZE+1; i++) {
        equips_deser_res = deser_Equip(&ser->equips[i], deser->equips[i]);
        if (!equips_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Equip(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i=0; i<EQUIP_SHOP_MAX; i++) {
        deser->equipPrices[i] = ser->equipPrices[i];
    }

    deser->consumablesCount = ser->consumablesCount;
    deser->uniqueConsumablesCount = ser->uniqueConsumablesCount;

    bool consumables_deser_res = false;
    for (size_t i=0; i<CONSUMABLESMAX+1; i++) {
        consumables_deser_res = deser_Consumable(&ser->consumables[i], deser->consumables[i]);
        if (!consumables_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Consumable(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i=0; i<CONSUMABLE_SHOP_MAX; i++) {
        deser->consumablePrices[i] = ser->consumablePrices[i];
    }
    return true;
}
bool ser_Shop(Shop* deser, SerShop* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerShop was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Shop was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    ser->itemCount = deser->itemCount;
    ser->equipsCount = deser->equipsCount;

    bool equips_ser_res = false;
    for (size_t i=0; i<EQUIPSBAGSIZE+1; i++) {
        equips_ser_res = ser_Equip(deser->equips[i], &ser->equips[i]);
        if (!equips_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Equip(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i=0; i<EQUIP_SHOP_MAX; i++) {
        ser->equipPrices[i] = deser->equipPrices[i];
    }

    ser->consumablesCount = deser->consumablesCount;
    ser->uniqueConsumablesCount = deser->uniqueConsumablesCount;

    bool consumables_ser_res = false;
    for (size_t i=0; i<CONSUMABLESMAX+1; i++) {
        consumables_ser_res = ser_Consumable(deser->consumables[i], &ser->consumables[i]);
        if (!consumables_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Consumable(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i=0; i<CONSUMABLE_SHOP_MAX; i++) {
        ser->consumablePrices[i] = deser->consumablePrices[i];
    }
    return true;
}

bool deser_Roadfork(SerRoadfork* ser, Roadfork* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerRoadfork was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Roadfork was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    for (size_t i=0; i<2; i++) {
        deser->options[i] = ser->options[i];
    }
    return true;
}

bool ser_Roadfork(Roadfork* deser, SerRoadfork* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerRoadfork was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Roadfork was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    for (size_t i=0; i<2; i++) {
        ser->options[i] = deser->options[i];
    }
    return true;
}

bool deser_Room(SerRoom* ser, Room* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerRoom was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Room was NULL.", __func__);
        return NULL;
    }

    deser->index = ser->index;
    deser->class = ser->class;
    bool shop_deser_res = deser_Shop(&ser->shop, deser->shop);
    if (!shop_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Shop().", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    bool roadfork_deser_res = deser_Roadfork(&ser->roadfork, deser->roadfork);
    if (!roadfork_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Roadfork().", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    bool treasure_deser_res = deser_Treasure(&ser->treasure, deser->treasure);
    if (!treasure_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Treasure().", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    bool boss_deser_res = deser_Boss(&ser->boss, deser->boss);
    if (!boss_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Boss().", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    deser->enemyTotal = ser->enemyTotal;
    bool enemies_deser_res = false;
    for (size_t i=0; i<ROOM_ENEMIES_MAX; i++) {
        enemies_deser_res = deser_Enemy(&ser->enemies[i], deser->enemies[i]);
        if (!enemies_deser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Enemy(). Index: {%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    bool foeparty_deser_res = deser_FoeParty(&ser->foes, deser->foes);
    if (!foeparty_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_FoeParty().", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    return true;
}

bool ser_Room(Room* deser, SerRoom* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerRoom was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Room was NULL.", __func__);
        return false;
    }

    ser->index = deser->index;
    ser->class = deser->class;
    bool shop_ser_res = ser_Shop(deser->shop, &ser->shop);
    if (!shop_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Shop().", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    bool roadfork_ser_res = ser_Roadfork(deser->roadfork, &ser->roadfork);
    if (!roadfork_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Roadfork().", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    bool treasure_ser_res = ser_Treasure(deser->treasure, &ser->treasure);
    if (!treasure_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Treasure().", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    bool boss_ser_res = ser_Boss(deser->boss, &ser->boss);
    if (!boss_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Boss().", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    ser->enemyTotal = deser->enemyTotal;
    bool enemies_ser_res = false;
    for (size_t i=0; i<ROOM_ENEMIES_MAX; i++) {
        enemies_ser_res = ser_Enemy(deser->enemies[i], &ser->enemies[i]);
        if (!enemies_ser_res) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Enemy(). Index: {%li}{%li}", __func__, i);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }
    }

    bool foeparty_ser_res = ser_FoeParty(deser->foes, &ser->foes);
    if (!foeparty_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_FoeParty().", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    return true;
}

bool deser_Floor(SerFloor* ser, Floor* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerFloor was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Floor was NULL.", __func__);
        return false;
    }

    deser->index = ser->index;
    deser->class = ser->class;

    for (size_t i=0; i<FLOOR_MAX_COLS; i++) {
        for(size_t k=0; k<FLOOR_MAX_ROWS; k++) {
            deser->floor_layout[i][k] = ser->floor_layout[i][k];
        }
    }

    deser->area = ser->area;

    bool rooms_deser_res = false;
    for (size_t i=0; i<FLOOR_MAX_COLS; i++) {
        for(size_t k=0; k<FLOOR_MAX_ROWS; k++) {
            rooms_deser_res = deser_Room(&ser->rooms_matrix[i][k],deser->rooms_matrix[i][k]);
            if (!rooms_deser_res) {
                log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Room(). Index: {%li}{%li}", __func__, i, k);
                deser->rooms_matrix[i][k] = NULL;
            }
        }
    }

    for (size_t i=0; i<FLOOR_MAX_COLS; i++) {
        for(size_t k=0; k<FLOOR_MAX_ROWS; k++) {
            deser->roomclass_layout[i][k] = ser->roomclass_layout[i][k];
        }
    }

    for (size_t i=0; i<FLOOR_MAX_COLS; i++) {
        for(size_t k=0; k<FLOOR_MAX_ROWS; k++) {
            deser->explored_matrix[i][k] = ser->explored_matrix[i][k];
        }
    }

    deser->explored_area = ser->explored_area;

    return true;
}
bool ser_Floor(Floor* deser, SerFloor* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerFloor was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Floor was NULL. Putting zeros.", __func__);
        *ser = (SerFloor){0};
        return true;
    }

    ser->index = deser->index;
    ser->class = deser->class;

    for (size_t i=0; i<FLOOR_MAX_COLS; i++) {
        for(size_t k=0; k<FLOOR_MAX_ROWS; k++) {
            ser->floor_layout[i][k] = deser->floor_layout[i][k];
        }
    }

    ser->area = deser->area;

    bool rooms_ser_res = false;
    for (size_t i=0; i<FLOOR_MAX_COLS; i++) {
        for(size_t k=0; k<FLOOR_MAX_ROWS; k++) {
            rooms_ser_res = ser_Room(deser->rooms_matrix[i][k],&ser->rooms_matrix[i][k]);
            if (!rooms_ser_res) {
                log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Room(). Index: {%li}{%li}. Putting zeros.", __func__, i, k);
                ser->rooms_matrix[i][k] = (SerRoom){0};
            }
        }
    }

    for (size_t i=0; i<FLOOR_MAX_COLS; i++) {
        for(size_t k=0; k<FLOOR_MAX_ROWS; k++) {
            ser->roomclass_layout[i][k] = deser->roomclass_layout[i][k];
        }
    }

    for (size_t i=0; i<FLOOR_MAX_COLS; i++) {
        for(size_t k=0; k<FLOOR_MAX_ROWS; k++) {
            ser->explored_matrix[i][k] = deser->explored_matrix[i][k];
        }
    }

    ser->explored_area = deser->explored_area;

    return true;
}

bool deser_Wincon(SerWincon* ser, Wincon* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerWincon was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Wincon was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    deser->class = ser->class;
    deser->current_val = ser->current_val;
    deser->target_val = ser->target_val;
    return true;
}

bool ser_Wincon(Wincon* deser, SerWincon* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerWincon was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Wincon was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    ser->class = deser->class;
    ser->current_val = deser->current_val;
    ser->target_val = deser->target_val;
    return true;
}

bool deser_Saveslot(SerSaveslot* ser, Saveslot* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerSaveslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Saveslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    ser->name[49] = '\0';
    memcpy(deser->name, ser->name, 49);
    deser->name[49] = '\0';

    ser->save_path[244] = '\0';
    memcpy(deser->save_path, ser->save_path, 244);
    deser->save_path[244] = '\0';
    return true;
}

bool ser_Saveslot(Saveslot* deser, SerSaveslot* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerSaveslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Saveslot was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    deser->name[49] = '\0';
    memcpy(ser->name, deser->name, 49);
    ser->name[49] = '\0';

    deser->save_path[244] = '\0';
    memcpy(ser->save_path, deser->save_path, 244);
    ser->save_path[244] = '\0';
    return true;
}

bool deser_Path(SerPath* ser, Path* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerPath was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Path was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    deser->length = ser->length;
    deser->luck = ser->luck;
    deser->prize = ser->prize;
    deser->loreCounter = ser->loreCounter;

    bool wincon_deser_res = deser_Wincon(&ser->win_condition, deser->win_condition);
    if (!wincon_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Wincon()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    bool saveslot_deser_res = deser_Saveslot(&ser->current_saveslot, deser->current_saveslot);
    if (!saveslot_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Saveslot()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    return true;
}
bool ser_Path(Path* deser, SerPath* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerPath was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Path was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    ser->length = deser->length;
    ser->luck = deser->luck;
    ser->prize = deser->prize;
    ser->loreCounter = deser->loreCounter;

    bool wincon_ser_res = ser_Wincon(deser->win_condition, &ser->win_condition);
    if (!wincon_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Wincon()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    bool saveslot_ser_res = ser_Saveslot(deser->current_saveslot, &ser->current_saveslot);
    if (!saveslot_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Saveslot()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    return true;
}

bool appendSerGamestate(const char* filename, SerGamestate* data) {
    FILE* file = fopen(filename, "ab");

    if (file != NULL) {
        // Write the structure to the file
        //
        int64_t ser_gmst_size = sizeof(SerGamestate);

        fwrite(&ser_gmst_size, sizeof(ser_gmst_size), 1, file);
        fwrite(data, sizeof(SerGamestate), 1, file);

        // Close the file
        fclose(file);
    } else {
        fprintf(stderr, "%s(): Error opening file {%s} for writing", __func__, filename);
        log_tag("debug_log.txt", "[ERROR]", "%s():    Error opening file {%s} for writing", __func__, filename);
        return false;
    }
    return true;
}

bool readSerGamestate(const char* filename, size_t offset, SerGamestate* data) {
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
        remaining_length = tot_length - offset;

#ifdef _WIN32
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Total file size: {%lli}, Offset: {%lli}, Remaining: {%lli}", __func__, tot_length, offset, remaining_length);
#else
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Total file size: {%li}, Offset: {%lli}, Remaining: {%li}", __func__, tot_length, offset, remaining_length);
#endif

        fread(&blob_size, sizeof(blob_size), 1, file);
        size_t sergmst_size = sizeof(SerGamestate);

#ifdef _WIN32
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Read blob size: {%lli}", __func__, blob_size);
        log_tag("debug_log.txt", "[DEBUG]", "%s():    SerGamestate size: {%lli}", __func__, sizeof(SerGamestate));
#else
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Read header size: {%li}", __func__, blob_size);
        log_tag("debug_log.txt", "[DEBUG]", "%s():    SerGamestate size: {%li}", __func__, sizeof(SerGamestate));
#endif

        if (blob_size != sergmst_size) {
            log_tag("debug_log.txt", "[ERROR]", "%s():    Header size from {%s} doesn't match SerGamestate size.", __func__, filename);
            if (blob_size < sergmst_size) {

#ifdef _WIN32
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is less than expected {%lli}.", __func__, blob_size, sergmst_size);
#else
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is less than expected {%li}.", __func__, blob_size, sergmst_size);
#endif
                fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            } else if (blob_size > sergmst_size) {

#ifdef _WIN32
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is greater than expected {%lli}.", __func__, blob_size, sergmst_size);
#else
                log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is greater than expected {%li}.", __func__, blob_size, sergmst_size);
#endif
                fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
                kls_free(default_kls);
                kls_free(temporary_kls);
                exit(EXIT_FAILURE);
            }
        }
        // Update len
        remaining_length -= sizeof(blob_size);


        if (remaining_length < blob_size) {

#ifdef _WIN32
            log_tag("debug_log.txt", "[ERROR]", "%s():    Remaining file length {%lli} is less than stored header size {%lli}.", __func__, remaining_length, blob_size);
#else
            log_tag("debug_log.txt", "[ERROR]", "%s():    Remaining file length {%li} is less than stored header size {%li}.", __func__, remaining_length, blob_size);
#endif
            fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }

        size_t expected_len = sizeof(SerGamestate);
        if (remaining_length < expected_len) {

#ifdef _WIN32
            log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is less than expected {%lli}.", __func__, remaining_length, expected_len);
#else
            log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is less than expected {%li}.", __func__, remaining_length, expected_len);
#endif
            fprintf(stderr, "%s():    Failed reading {%s}.\n", __func__, filename);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        } else if (remaining_length > expected_len) {

#ifdef _WIN32
            log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%lli} is greater than expected {%lli}.", __func__, remaining_length, expected_len);
#else
            log_tag("debug_log.txt", "[ERROR]", "%s():    Size {%li} is greater than expected {%li}.", __func__, remaining_length, expected_len);
#endif
        }

        // Read the structure from the file
        fread(data, sizeof(SerGamestate), 1, file);

        // Close the file
        fclose(file);
    } else {
        fprintf(stderr, "%s(): Error opening file {%s} for reading", __func__, filename);
        log_tag("debug_log.txt", "[ERROR]", "%s():    Error opening file {%s} for reading", __func__, filename);
        return false;
    }
    return true;
}

bool deser_Gamestate(SerGamestate* ser, Gamestate* deser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerGamestate was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Gamestate was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    deser->start_time = ser->start_time;

    bool stats_deser_res = deser_countStats(&ser->stats, deser->stats);
    if (!stats_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_countStats()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    deser->current_fighters = ser->current_fighters;

    deser->current_roomtype = deser->current_roomtype;
    deser->current_room_index = ser->current_room_index;


    bool wincon_deser_res = deser_Wincon(&ser->wincon, deser->wincon);
    if (!wincon_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Wincon()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    bool path_deser_res = deser_Path(&ser->path, deser->path);
    if (!path_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Path()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    bool fighter_deser_res = deser_Fighter(&ser->player, deser->player);
    if (!fighter_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Fighter()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    deser->gamemode = ser->gamemode;

    bool floor_deser_res = deser_Floor(&ser->current_floor, deser->current_floor);
    if (!floor_deser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed deser_Floor()", __func__);
        deser->current_floor = NULL;
    }

    deser->is_localexe = ser->is_localexe;
    return true;
}

bool ser_Gamestate(Gamestate* deser, SerGamestate* ser) {
    if (ser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed SerGamestate was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    if (deser == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): passed Gamestate was NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    ser->start_time = deser->start_time;

    bool stats_ser_res = ser_countStats(deser->stats, &ser->stats);
    if (!stats_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_countStats()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    ser->current_fighters = deser->current_fighters;

    ser->current_roomtype = deser->current_roomtype;
    ser->current_room_index = deser->current_room_index;


    bool wincon_ser_res = ser_Wincon(deser->wincon, &ser->wincon);
    if (!wincon_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Wincon()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    bool path_ser_res = ser_Path(deser->path, &ser->path);
    if (!path_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Path()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    bool fighter_ser_res = ser_Fighter(deser->player, &ser->player);
    if (!fighter_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Fighter()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    ser->gamemode = deser->gamemode;

    bool floor_ser_res = ser_Floor(deser->current_floor, &ser->current_floor);
    if (!floor_ser_res) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): Failed ser_Floor()", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }

    ser->is_localexe = deser->is_localexe;
    return true;
}

/**
 * Tries reading binary gamestate from passed path, at passed offset.
 * @param static_path The path to which we append to find our file.
 * @param offset The offset of the SerGameState into passed file.
 * @param kls Koliseo used for allocation.
 * @param force_init Used to force writing a new SerGamestate.
 * @see SerGamestate
 * @see Gamestate
 * @return The newly allocated Gamestate.
 * TODO Contract should meaningfully capture case of read failure + init.
 */
bool prep_Gamestate(Gamestate* gmst, const char* static_path, size_t offset, Koliseo* kls, bool force_init)
{
    if (kls == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "%s(): koliseo as NULL.", __func__);
        kls_free(default_kls);
        kls_free(temporary_kls);
        exit(EXIT_FAILURE);
    }
    bool gmst_null = false;
    if (gmst != NULL) {
        log_tag("debug_log.txt", "[DEBUG]", "%s(): passed Gamestate is not NULL.", __func__);
    } else {
        gmst_null = true;
        log_tag("debug_log.txt", "[DEBUG]", "%s(): passed Gamestate is NULL, will be allocated.", __func__);
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

    sprintf(path_to_bin_savefile, "%s/%s", static_path, bin_savefile_name);

    if (force_init) {
        log_tag("debug_log.txt", "[BINSAVE]", "%s():    Forced init of SerGamestate.", __func__);
        SerGamestate ser_gmst = {0};
        if (!gmst_null) {
            log_tag("debug_log.txt", "[BINSAVE]", "%s():    using passed Gamestate.", __func__);
            bool ser_res = ser_Gamestate(gmst, &ser_gmst);
            if (!ser_res) {
                log_tag("debug_log.txt", "[BINSAVE]", "%s():    Failed serializing passed Gamestate.", __func__);
                return false;
            }
        }

        // Write packed structure to a binary file
        bool write_res = appendSerGamestate(path_to_bin_savefile, &ser_gmst);

        if (!write_res) {
            // Failed writing new binsave
            fprintf(stderr, "%s():    Failed to create a binsave.\n", __func__);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        } else {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): success for appendSerGamestate()", __func__);
        }

        if (gmst_null) {
            gmst = KLS_PUSH(kls, Gamestate);
        }
        bool deser_result = deser_Gamestate(&ser_gmst, gmst);
        if (!deser_result) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): failed deser_Gamestate().", __func__);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        } else {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): success for deser_Gamestate()", __func__);
        }
        //log_tag("debug_log.txt", "[BINSAVE]", "Initialised Data: api_level=%" PRId32 ", save_version=%s, game_version=%s, os=%s, machine=%s", save_head->api_level, save_head->save_version, save_head->game_version, save_head->os, save_head->machine);
        return true;
    }

    Koliseo_Temp* kls_t = kls_temp_start(kls);
    // Try reading an existing binsave
    SerGamestate* read_gmst = KLS_PUSH_T(kls_t, SerGamestate);
    bool read_res = readSerGamestate(path_to_bin_savefile, offset, read_gmst);

    if (!read_res) {
        kls_temp_end(kls_t);
        log_tag("debug_log.txt", "[BINSAVE]", "Failed reading binsave at {%s}, creating a new one.", path_to_bin_savefile);
        // Failed reading existing binsave, create a new one
        SerGamestate ser_gmst = {0};
        if (!gmst_null) {
            log_tag("debug_log.txt", "[BINSAVE]", "%s():    using passed Gamestate.", __func__);
            bool ser_res = ser_Gamestate(gmst, &ser_gmst);
            if (!ser_res) {
                log_tag("debug_log.txt", "[BINSAVE]", "%s():    Failed serializing passed Gamestate.", __func__);
                return false;
            }
        }

        // Write packed structure to a binary file
        bool write_res = appendSerGamestate(path_to_bin_savefile, &ser_gmst);

        if (!write_res) {
            // Failed writing new binsave
            fprintf(stderr, "%s():    Failed to create a binsave.\n", __func__);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        } else {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): success for appendSerGamestate()", __func__);
        }

        if (gmst_null) {
            gmst = KLS_PUSH(kls, Gamestate);
        }
        bool deser_result = deser_Gamestate(&ser_gmst, gmst);
        if (!deser_result) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): failed deser_Gamestate().", __func__);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        } else {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): success for deser_Gamestate()", __func__);
        }
        //log_tag("debug_log.txt", "[BINSAVE]", "Initialised Data: api_level=%" PRId32 ", save_version=%s, game_version=%s, os=%s, machine=%s", save_head->api_level, save_head->save_version, save_head->game_version, save_head->os, save_head->machine);
        return true;
    } else {
        SerGamestate tmp = (SerGamestate) {0};
        tmp = *read_gmst;
        kls_temp_end(kls_t);

        if (gmst_null) {
            gmst = KLS_PUSH(kls, Gamestate);
        }

        bool deser_result = deser_Gamestate(&tmp, gmst);
        if (!deser_result) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): failed deser_Gamestate().", __func__);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        } else {
            log_tag("debug_log.txt", "[BINSAVE]", "%s(): success for deser_Gamestate()", __func__);
        }

        if (gmst == NULL) {
            log_tag("debug_log.txt", "[ERROR]", "%s(): gmst was NULL after deser_Gamestate().", __func__);
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
        }

        //log_tag("debug_log.txt", "[BINSAVE]", "Read Data: api_level=%" PRId32 ", save_version=%s, game_version=%s, os=%s, machine=%s", save_head->api_level, save_head->save_version, save_head->game_version, save_head->os, save_head->machine);
        return true;
    }
}
