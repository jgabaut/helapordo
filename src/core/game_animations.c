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
#include "game_animations.h"

void hlpd_load_fighter_animations(char fighter_sprites[CLASSESMAX+1][MAXFRAMES][MAXROWS][MAXCOLS])
{
    /*
     * Legacy code for loading animations from an s4c-file.
     char fighter_filename[600];
     FILE* fighter_sprite_file;
     */
    for (int i = 0; i < CLASSESMAX + 1; i++) {

        int n_load_frames = 60;
        int n_load_rows = 17;
        int n_load_cols = 17;

        switch (i) {
        case Knight: {
            s4c_copy_animation(knight_tapis, fighter_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Mage: {
            s4c_copy_animation(mage_spark, fighter_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Archer: {
            s4c_copy_animation(archer_drop, fighter_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Assassin: {
            s4c_copy_animation(assassin_poof, fighter_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        default: {
            log_tag("debug_log.txt", "[ERROR]",
                    "Unexpected fighterclass index while loading animation for class (%i): [%s]",
                    i, stringFromClass(i));
            exit(EXIT_FAILURE);
        }
        break;
        }

        /*
         * Legacy code for loading animations from an s4c-file
         fighter_sprite_file = fopen(fighter_filename, "r");
         if (!fighter_sprite_file) {
         fprintf(stderr,"Error opening animation file at (%s).\n",fighter_filename);
         fprintf(stderr,"Static path was (%s).\n",static_path);
         exit(EXIT_FAILURE);
         }
         int fighter_loadCheck = load_sprites(fighter_sprites[i], fighter_sprite_file, 17, 17);
         sprintf(load_msg,"Loaded animation for %s, load result was %i.", stringFromClass(i), fighter_loadCheck);
         log_tag("debug_log.txt","[PREP]",load_msg);
         */

        log_tag("debug_log.txt", "[PREP]",
                "Copied animation from default matrix vector for: [%s] with dimensions: [%i][%i][%i].",
                stringFromClass(i), n_load_frames, n_load_rows,
                n_load_cols);

        //Massive log of all loaded lines
        /*
           for (int k=0; k<MAXFRAMES+1; k++) {
           sprintf(load_msg,"[DEBUG]    frame %i:", k);
           debug_log("debug_log.txt",load_msg);
           for (int z=0; z<MAXROWS+1;z++) {
           sprintf(load_msg,"[DEBUG] %i  ( %s ).", z, fighter_sprites[i][k][z]);
           debug_log("debug_log.txt",load_msg);
           }
           }
         */
    }
}

void hlpd_load_enemy_animations(char enemy_sprites[ENEMYCLASSESMAX+1][MAXFRAMES][MAXROWS][MAXCOLS])
{
    //Try preloading all enemy animations
    for (int i = 0; i < (ENEMYCLASSESMAX + 1); i++) {
        /* Legacy code to load animations from an s4c-file
           char static_path[500];

           // Set static_path value to the correct static dir path
           resolve_staticPath(static_path);

           char enemy_filename[600];
           FILE* enemy_sprite_file;
         */

        int n_load_frames = 60;
        int n_load_rows = 17;
        int n_load_cols = 17;
        // Prepare the enemy frames
        switch (i) {
        case Imp: {
            s4c_copy_animation(imp_fireball, enemy_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Zombie: {
            s4c_copy_animation(zombie_walk, enemy_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Troll: {
            s4c_copy_animation(troll_club, enemy_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Ghost: {
            s4c_copy_animation(ghost_spell, enemy_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Boar: {
            s4c_copy_animation(boar_scream, enemy_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Mummy: {
            s4c_copy_animation(mummy_shuffle, enemy_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Goblin: {
            s4c_copy_animation(goblin_shoot, enemy_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Werewolf: {
            s4c_copy_animation(werewolf_transform, enemy_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        default: {
            log_tag("debug_log.txt", "[ERROR]",
                    "Unexpected enemy index while loading animation for enemy (%i): [%s]",
                    i, stringFromEClass(i));
            exit(EXIT_FAILURE);
        }
        }

        /* Legacy code loading the animations from an s4c-file
           enemy_sprite_file = fopen(enemy_filename, "r");
           if (!enemy_sprite_file) {
           fprintf(stderr,"Error opening animation file at (%s/%s).\n",static_path,enemy_filename);
           exit(EXIT_FAILURE);
           }
           int enemy_loadCheck = load_sprites(enemy_sprites[i], enemy_sprite_file, 17, 17);
           sprintf(load_msg,"Loaded animation for %s, load result was %i.", stringFromEClass(i), enemy_loadCheck);
           log_tag("debug_log.txt","[PREP]",load_msg);
         */

        log_tag("debug_log.txt", "[PREP]",
                "Copied animation from default matrix vector for: [%s] with dimensions: [%i][%i][%i].",
                stringFromEClass(i), n_load_frames, n_load_rows,
                n_load_cols);

        //Massive log of all loaded lines
        /*
           for (int k=0; k<MAXFRAMES+1; k++) {
           sprintf(load_msg,"[DEBUG]    frame %i:", k);
           debug_log("debug_log.txt",load_msg);
           for (int z=0; z<MAXROWS+1;z++) {
           sprintf(load_msg,"[DEBUG] %i  ( %s ).", z, enemy_sprites[i][k][z]);
           debug_log("debug_log.txt",load_msg);
           }
           }
         */
    }
}

void hlpd_load_boss_animations(char boss_sprites[BOSSCLASSESMAX + 1][MAXFRAMES][MAXROWS][MAXCOLS])
{
    //Try preloading all boss animations
    for (int i = 0; i < (BOSSCLASSESMAX + 1); i++) {
        int n_load_frames = 60;
        int n_load_rows = 17;
        int n_load_cols = 17;

        /*
         * Legacy code for loading animations from an s4c-file.
         char static_path[500];

         // Set static_path value to the correct static dir path
         resolve_staticPath(static_path);

         char boss_filename[600];
         FILE* boss_sprite_file;
         */

        // Prepare the boss frames
        switch (i) {
        case Blue_Troll: {
            s4c_copy_animation(bluetroll_wonder, boss_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Headless_Ninja: {
            s4c_copy_animation(headlessninja_throw, boss_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Crawling_Dude: {
            s4c_copy_animation(crawlingdude_crawl, boss_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Sr_Warthog: {
            s4c_copy_animation(srwarthog_square, boss_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        case Doppelganger: {
            s4c_copy_animation(knight_tapis, boss_sprites[i],
                               n_load_frames, n_load_rows, n_load_cols);
        }
        break;
        default: {
            log_tag("debug_log.txt", "[ERROR]",
                    "Unexpected boss index while loading animation for boss (%i): [%s]",
                    i, stringFromBossClass(i));
            exit(EXIT_FAILURE);
        }
        }
        /*
         * Legacy code for loading animations from an s4c-file.
         boss_sprite_file = fopen(boss_filename, "r");
         if (!boss_sprite_file) {
         fprintf(stderr,"Error opening animation file at (%s/%s).\n",static_path,boss_filename);
         exit(EXIT_FAILURE);
         }
         int boss_loadCheck = load_sprites(boss_sprites[i], boss_sprite_file, 17, 17);
         sprintf(load_msg,"Loaded animation for %s, load result was %i.", stringFromBossClass(i), boss_loadCheck);
         log_tag("debug_log.txt","[PREP]",load_msg);
         */

        log_tag("debug_log.txt", "[PREP]",
                "Copied animation from default matrix vector for: [%s] with dimensions: [%i][%i][%i].",
                stringFromBossClass(i), n_load_frames, n_load_rows,
                n_load_cols);

        //Massive log of all loaded lines
        /*
           for (int k=0; k<MAXFRAMES+1; k++) {
           sprintf(load_msg,"[DEBUG]    frame %i:", k);
           debug_log("debug_log.txt",load_msg);
           for (int z=0; z<MAXROWS+1;z++) {
           sprintf(load_msg,"[DEBUG] %i  ( %s ).", z, enemy_sprites[i][k][z]);
           debug_log("debug_log.txt",load_msg);
           }
           }
         */
    }			//End loading boss animations

}

void hlpd_load_animations(char fighter_sprites[CLASSESMAX+1][MAXFRAMES][MAXROWS][MAXCOLS], char enemy_sprites[ENEMYCLASSESMAX+1][MAXFRAMES][MAXROWS][MAXCOLS], char boss_sprites[BOSSCLASSESMAX + 1][MAXFRAMES][MAXROWS][MAXCOLS])
{
    log_tag("debug_log.txt", "[DEBUG]", "%s():    Moving sprites from global arrays to passed arrays", __func__);
    hlpd_load_fighter_animations(fighter_sprites);
    hlpd_load_enemy_animations(enemy_sprites);
    hlpd_load_boss_animations(boss_sprites);
}
