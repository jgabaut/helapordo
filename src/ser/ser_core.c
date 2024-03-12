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
            kls_free(default_kls);
            kls_free(temporary_kls);
            exit(EXIT_FAILURE);
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
