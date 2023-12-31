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
#include "sprites.h"

char bosses_sprites[BOSSCLASSESMAX + 1][9][15] = {

    {
        //Blue troll
        "y     bbbbb  ",
        "yy   bbgbgbb ",
        " yb bbbbbbbb ",
        "bbb y  mmbbb ",
        "bbb   yyybbb ",
        " bbb  bbbb   ",
        "  bbbccrrccb ",
        "   bbccccccbb",
    },
    {
        //Headless_Ninja
        "wwmmmbmmmbmww",
        "mmmmmbmmmbmmw",
        "myyyyybmbbwmm",
        "my@m@ybbbbwwm",
        "myyyyybbbbw@m",
        "myyyyybbbb@@@",
        "mymyymrrrrw@w",
        "wmmmmbbbbbrww",
    },
    {
        //Crawling_Dude
        "kkcccckkkkkkk",
        "kkcbccckkkkkk",
        "kccccccmmkkkk",
        "kkckcccmwmmkk",
        "kkkkccmmwwmmk",
        "kckccmmcmwwmm",
        "cckkmmccmmwwm",
        "cccwwmcccwwwm",
    },
    {
        //Sr_Warthog
        "kmmrrrkrrrmmk",
        "kmrwwrrrwwrmk",
        "kkrwkwrwkwrkk",
        "kkrrmmmmmrrkk",
        "kkrrmkmkmrrkk",
        "kkkrwmmmwrkkk",
        "kkkrwrrrwrkkk",
        "kkkkrrrrrkkkk",
    },
    {
        //Doppelganger?
        "I    yy    I",
        "I    yy    I",
        "Iy  yyyy y I",
        "I yy yy y  I",
        "I    yy    I",
        "I   y  y   I",
        "I   y  y   I",
        "I  y   y   I",
    }
};

char consumables_sprites[CONSUMABLESMAX + 1][9][14] = {
    {
        //Potion
        "I   wwww   I",
        "I   wggw   I",
        "I  wggggw  I",
        "I wggggggw I",
        "IwgggwggggwI",
        "IwggwgggggwI",
        "I wggggggw I",
        "I  wwwwww  I",
    },
    {
        //Rock
        "I          I",
        "I          I",
        "I   rrr    I",
        "I  ryyrr   I",
        "I rywyyyr  I",
        "I ryywyyyr I",
        "I  ryyyrr  I",
        "I   rrrr   I",
    },
    {
        //Bread
        "I          I",
        "I    www   I",
        "I  wwwwww  I",
        "I wyyyyyyw I",
        "I yygyyyw  I",
        "I  yyyyy   I",
        "I          I",
        "I          I",
    },
    {
        //Coffee
        "I   yy     I",
        "I  y       I",
        "Iwwwwwwww  I",
        "Iwrrwrrwww I",
        "Iwrrrrrw  wI",
        "Iwwrrrww  wI",
        "Iwwwrwwwww I",
        "Iwwwwwwww  I",
    },
    {
        //Powergem
        "Iyy  ww    I",
        "I  ywccw  yI",
        "Iy wcwccwy I",
        "Iy wcwcccw I",
        "I ywcccccw I",
        "I  ywcccw yI",
        "I y  www   I",
        "I  y    y  I",
    },
    {
        //Magicbean
        "I   bbbb   I",
        "I  bggggb  I",
        "I bggygggb I",
        "IbggygggggbI",
        "I bggggggb I",
        "I bgggggb  I",
        "I  bbbbb   I",
        "I          I",
    }
};

char equips_sprites[EQUIPSMAX + 1][9][15] = {

    {
        //Vest
        "I       bbyI",
        "I   ybbbbbyI",
        "I yyybbbb  I",
        "I bbbbbb   I",
        "I bbbwbb   I",
        "Ibbbbbbbb  I",
        "Ibb  bwbbb I",
        "Ibb bbbbbb I",
    },
    {
        //Helmet
        "I          I",
        "I    yy    I",
        "I  yyyyyy  I",
        "I  yyyyyy  I",
        "I  r    r  I",
        "I  r    r  I",
        "I  r    r  I",
        "I          I",
    },
    {
        //Boots
        "I          I",
        "I      rrr I",
        "I      rrr I",
        "Ir     ggrrI",
        "Igr    rrrrI",
        "Irrr   ggrrI",
        "Iyyy rrrrrrI",
        "Iy y rrrrryI",
    },
    {
        //Cape
        "I   bbb    I",
        "I  b  cbb  I",
        "I  b  ccb  I",
        "I   b bcb  I",
        "I  bcccbb  I",
        "I bccwcccb I",
        "IbccwcccccbI",
        "IbccwcccccbI",
    },
    {
        //Tophat
        "I          I",
        "I  bbbbbb  I",
        "I  bbbbwb  I",
        "I  bbbbbb  I",
        "I  bbbbbb  I",
        "I  wwwwww  I",
        "I bbbbbbbb I",
        "I          I",
    },
    {
        //Loafers
        "I     www  I",
        "I    wggw  I",
        "I   wgggw  I",
        "I    wgwyw I",
        "I     wyyywI",
        "Iww    wyw I",
        "Igww    w  I",
        "Iggwyyy    I",
    }
};

char equipzones_sprites[EQUIPZONES + 1][9][15] = {
    {
        //HEAD
        "I          I",
        "I   wwww   I",
        "I  wwwwww  I",
        "I  w ww w  I",
        "I  wwwwww  I",
        "I  ww  ww  I",
        "I   wwww   I",
        "I          I",
    },
    {
        //TORSO
        "I          I",
        "I    ww    I",
        "I  wwwwww  I",
        "I wwwwwwww I",
        "Iww wwww wwI",
        "Iw  wwww  wI",
        "I   wwww   I",
        "I   wwww   I",
    },
    {
        //LEGS
        "I ww   ww  I",
        "I www  ww  I",
        "I  wwy rr  I",
        "I   yyyrr  I",
        "I    yyyr  I",
        "I   yyyyrrrI",
        "I  yyyyrrrrI",
        "I yyyy     I",
    },
};

char artifacts_sprites[ARTIFACTSMAX + 1][9][15] = {
    {
        //THKSKULL
        "I   wwww   I",
        "I  w w ww  I",
        "I  w w ww  I",
        "I  wwwwwww I",
        "I  w w w w I",
        "I       w  I",
        "I w w w w  I",
        "I  wwwww   I",
    },
    {
        //TWINKIE
        "I          I",
        "I      yy  I",
        "I     yyyy I",
        "I    yyyyy I",
        "I   yyyyy  I",
        "I  yywyy   I",
        "I   yyy    I",
        "I          I",
    },
    {
        //WRISTBAND
        "I          I",
        "I    yry   I",
        "I   y   y  I",
        "I  y     y I",
        "I  y     y I",
        "I   y   y  I",
        "I    yyy   I",
        "I          I",
    },
    {
        //BOARTAIL
        "I          I",
        "I      rrr I",
        "I     rr  rI",
        "I   rrr   rI",
        "I  rrr   r I",
        "I  grr   r I",
        "Ig  rg     I",
        "I g        I",
    },
    {
        //CHAOSORB
        "I          I",
        "I   bbbb   I",
        "I  bbbrbb  I",
        "I  bbbbrb  I",
        "I  b  bbb  I",
        "I  bb  bb  I",
        "I   bbbb   I",
        "I          I",
    },
    {
        //POWERSYPHON
        "I  yyyyyy  I",
        "I   yyyy   I",
        "I    yy    I",
        "IwwwwwwwwwwI",
        "IwwwwwwwwwwI",
        "IwwggggrrwwI",
        "IwwwwwwwwwwI",
        "Iw w wwwwwwI",
    },
    {
        //GIANTFOSSILE
        "I    yy    I",
        "I    yy    I",
        "I       y  I",
        "I   yyy  y I",
        "I   yyyy   I",
        "I  yyyyy y I",
        "I yyyy     I",
        "I yyy      I",
    },

};

char enemies_sprites[ENEMYCLASSESMAX + 1][9][15] = {
    {
        //Mummy
        "I@@@bbbbb@@@I",
        "I@@bbwwwbb@@I",
        "I@@bw@w@wb@@I",
        "I@@bwwwwwb@@I",
        "I@@@bw@wb@@@I",
        "I@bbwwwwwbb@I",
        "I@bwwwwwwwb@I",
        "IbwwbwwwbwwbI",
    },
    {
        //Ghost
        "I@@@@@@@@@@@I",
        "I@@@@wwww@@@I",
        "I@@@ww@w@w@@I",
        "I@@@wwwwww@@I",
        "I@@@www@ww@@I",
        "I@@wwww@w@@@I",
        "I@wwwwwww@@@I",
        "I@@www@@w@@@I",
    },
    {
        //Zombie
        "I@@@@bbbb@@@I",
        "I@@@bggggb@@I",
        "I@@bggggggb@I",
        "I@@bg@gg@gb@I",
        "I@@@bgggggb@I",
        "I@@bgggggb@@I",
        "I@bggggggb@@I",
        "Ibggbggbggb@I",
    },
    {
        //Goblin
        "Ibb bbbbb bbI",
        "IbgbbgggbbgbI",
        "I bgbwgwbgb I",
        "I   bgggb   I",
        "I bbbbbbbbb I",
        "IbggbgggbggbI",
        "IggbgggggbggI",
        "IbbbbbbbbbbbI",
    },
    {
        //Imp
        "Iww wwwww wwI",
        "IwrwwrrrwwrwI",
        "I wrrrrrrrw I",
        "I  wr@r@rw  I",
        "I   wrrrw   I",
        "I  wrwwwrw  I",
        "I wrrrrrrrw I",
        "IwwwwwwwwwwwI",
    },
    {
        //Troll
        "Igg ggggg ggI",
        "IgyggyyyggygI",
        "I gyyyyyyyg I",
        "I  gy@y@yg  I",
        "I   gyyyg   I",
        "I  gywwwyg  I",
        "I gyyyyyyyg I",
        "IgggggggggggI",
    },
    {
        //Boar
        "I@rrrrrrrrr@I",
        "I@rgmmmmmgr@I",
        "Iwrmm@m@mmrwI",
        "IrwwmmmmmwwrI",
        "I rww   wwr I",
        "I rwww wwwr I",
        "I rrmmmmmrr I",
        "I  rrrrrrr  I",
    },
    {
        //Werewolf
        "I@@@@@@@@rr@I",
        "I@@@mmmrryr@I",
        "I@@mmwm@yr@@I",
        "I@@@mmmyyr@@I",
        "I@@@m@myyr@@I",
        "I@@@m@@yr@@@I",
        "I@@mmmmmyr@@I",
        "Immmmyyyyyr@I",
    },
};

char misc_sprites[MISC_SPRITES_MAX + 1][9][15] = {
    {
        //Old zombie
        "IkkkkkkkkkkkI",
        "IkgggggggggkI",
        "IkggkgggkggkI",
        "IkgggggggggkI",
        "IkggwwwwwggkI",
        "IkgwwgggwwgkI",
        "IkgggggggggkI",
        "IkkkkkkkkkkkI",
    },
    {
        //Default fighter sprite
        "I    yy    I",
        "I    yy    I",
        "I   yyyy y I",
        "I yy yy y  I",
        "I    yy    I",
        "I   y  y   I",
        "I   y  y   I",
        "I    y y   I",
    },
};
