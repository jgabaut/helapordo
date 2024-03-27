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
        bool res = read_savedir(dir_arg);
        if (!res) {
            fprintf(stderr, "%s():    Failed read_savedir().", __func__);
        }
    }
    return 0;
}
