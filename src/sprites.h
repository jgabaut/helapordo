#ifndef SPRITES_H
#define SPRITES_H

#include "game_core.h"

extern char bosses_sprites[BOSSCLASSESMAX+1][9][15];
extern char consumables_sprites[CONSUMABLESMAX+1][9][14];
extern char equips_sprites[EQUIPSMAX+1][9][15];
extern char equipzones_sprites[EQUIPZONES+1][9][15];
extern char artifacts_sprites[ARTIFACTSMAX+1][9][15];
extern char enemies_sprites[ENEMYCLASSESMAX+1][9][15];

#define MISC_SPRITES_MAX 1
extern char misc_sprites[MISC_SPRITES_MAX+1][9][15];

#endif
