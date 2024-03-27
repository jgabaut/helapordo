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
    fprintf(stderr, "Usage:    %s <-s/-r> <savefile>\n", progname);
}

#define RDR_SAVE_MODE 0
#define RDR_RUN_MODE 1
int main(int argc, char** argv)
{

    if (argc != 3) {
        saverdr_usage(argv[0]);
        return 1;
    } else {
        const char* mode_arg = argv[1];
        int mode = -1;
        if (strcmp(mode_arg, "-s") == 0) {
            mode = RDR_SAVE_MODE;
        } else if (strcmp(mode_arg, "-r") == 0) {
            mode = RDR_RUN_MODE;
        } else {
            saverdr_usage(argv[0]);
            return 1;
        }
        const char* filepath = argv[2];

        switch(mode) {
            case RDR_SAVE_MODE: {
                SerSaveHeader s_hdr = {0};

                bool read_res = readSerSaveHeader(filepath, &s_hdr);
                if (!read_res) {
                    fprintf(stderr, "Error while reading from {%s}.\n", filepath);
                    return 1;
                } else {
                    printf("Save info: {\n    Api level: {%" PRId32 "}\n    Game version: {%s}\n    Save version: {%s}\n    Os: {%s}\n    Machine: {%s}\n}\n", s_hdr.api_level, s_hdr.game_version, s_hdr.save_version, s_hdr.os, s_hdr.machine);
                }
            }
            break;
            case RDR_RUN_MODE: {
                SerGamestate s_gmst = {0};

                bool read_res = readSerGamestate(filepath, 0, &s_gmst);
                if (!read_res) {
                    fprintf(stderr, "Error while reading from {%s}.\n", filepath);
                    return 1;
                } else {

                    printf("Gamemode: {%s}\n", stringFromGamemode(s_gmst.gamemode));
                    printf("Current room index: {%i}\n", s_gmst.current_room_index);
                    printf("Current room type: {%s}\n", stringFromRoom(s_gmst.current_room.class));
                    printf("Player info: {\n    Name: {%s}\n    Class: {%s}\n}\n", s_gmst.player.name, stringFromClass(s_gmst.player.class));
                }
            }
            break;
            default: {
                fprintf(stderr, "%s():    Unexpected mode. {%i}\n", __func__, mode);
                return 1;
            }
            break;
        }

    }
    return 0;
}
