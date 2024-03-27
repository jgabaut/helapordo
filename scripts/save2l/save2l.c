#include "save2l.h"

int G_PRELOAD_ANIMATIONS_ON = 0;
int G_DEBUG_ON = 0;
int G_LOG_ON = 0;
int G_EXPERIMENTAL_ON = 0;
int G_FASTQUIT_ON = 0;
int G_GODMODE_ON = 0;
int G_DEBUG_ROOMTYPE = ENEMIES;
int G_DEBUG_ROOMTYPE_ON = 0;
char *G_DEBUG_ROOMTYPE_ARG = NULL;
int G_DEBUG_ENEMYTYPE = Zombie;
int G_DEBUG_ENEMYTYPE_ON = 0;
char *G_DEBUG_ENEMYTYPE_ARG = NULL;

int G_DOTUTORIAL_ON = 0;

int GS_AUTOSAVE_ON = 1;

int G_USE_CURRENTDIR = 0;
Gamemode GAMEMODE = Rogue;
Koliseo* default_kls = NULL;
Koliseo* temporary_kls = NULL;

void saverdr_usage(const char* progname)
{
    fprintf(stderr, "Usage:    %s <savedir>\n", progname);
}

#define RDR_SAVE_MODE 0
#define RDR_RUN_MODE 1
int main(int argc, char** argv)
{

    if (argc != 2) {
        saverdr_usage(argv[0]);
        return 1;
    } else {
        const char* dir_arg = argv[1];
        char meta_filepath[800];
        char run_filepath[800];

        snprintf(meta_filepath, 799, "%s/save-nc.bin", dir_arg);
        snprintf(run_filepath, 799, "%s/run-nc.bin", dir_arg);

        meta_filepath[799] = '\0';
        run_filepath[799] = '\0';

        SerSaveHeader s_hdr = {0};

        bool read_res = readSerSaveHeader(meta_filepath, &s_hdr);
        if (!read_res) {
            fprintf(stderr, "Error while reading from {%s}.\n", meta_filepath);
            return 1;
        } else {
            printf("Save info: {\n    Api level: {%" PRId32 "}\n    Game version: {%s}\n    Save version: {%s}\n    Os: {%s}\n    Machine: {%s}\n}\n", s_hdr.api_level, s_hdr.game_version, s_hdr.save_version, s_hdr.os, s_hdr.machine);
        }
        SerGamestate s_gmst = {0};

        bool run_read_res = readSerGamestate(run_filepath, 0, &s_gmst);
        if (!run_read_res) {
            fprintf(stderr, "Error while reading from {%s}.\n", run_filepath);
            return 1;
        } else {

            printf("Gamemode: {%s}\n", stringFromGamemode(s_gmst.gamemode));
            printf("Current room index: {%i}\n", s_gmst.current_room_index);
            printf("Current room type: {%s}\n", stringFromRoom(s_gmst.current_room.class));
            printf("Player info: {\n    Name: {%s}\n    Class: {%s}\n}\n", s_gmst.player.name, stringFromClass(s_gmst.player.class));
        }
    }
    return 0;
}
