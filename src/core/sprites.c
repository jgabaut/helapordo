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

char consumables_sprites_proper[CONSUMABLESMAX + 1][9][MAXCOLS] = {
    {
        //Potion
        "111188881111",
        "111183381111",
        "111833338111",
        "118333333811",
        "183338333381",
        "183383333381",
        "118333333811",
        "111888888111",
    },
    {
        //Rock
        "111111111111",
        "111111111111",
        "111122211111",
        "111244221111",
        "112484442111",
        "112448444211",
        "111244422111",
        "111122221111",
    },
    {
        //Bread
        "111111111111",
        "111118881111",
        "111888888111",
        "118444444811",
        "114434448111",
        "111444441111",
        "111111111111",
        "111111111111",
    },
    {
        //Coffee
        "111144111111",
        "111411111111",
        "188888888111",
        "182282288811",
        "182222281181",
        "188222881181",
        "188828888811",
        "188888888111",
    },
    {
        //Powergem
        "144118811111",
        "111487781141",
        "141878778411",
        "141878777811",
        "114877777811",
        "111487778141",
        "114118881111",
        "111411114111",
    },
    {
        //Magicbean
        "111155551111",
        "111533335111",
        "115334333511",
        "153343333351",
        "115333333511",
        "115333335111",
        "111555551111",
        "111111111111",
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

char equips_sprites_proper[EQUIPSMAX + 1][9][MAXCOLS] = {

    {
        //Vest
        "111111115541",
        "111145555541",
        "114445555111",
        "115555551111",
        "115558551111",
        "155555555111",
        "155115855511",
        "155155555511",
    },
    {
        //Helmet
        "111111111111",
        "111114411111",
        "111444444111",
        "111444444111",
        "111211112111",
        "111211112111",
        "111211112111",
        "111111111111",
    },
    {
        //Boots
        "111111111111",
        "111111122211",
        "111111122211",
        "121111133221",
        "132111122221",
        "122211133221",
        "144412222221",
        "141412222241",
    },
    {
        //Cape
        "111155511111",
        "111511755111",
        "111511775111",
        "111151575111",
        "111577755111",
        "115778777511",
        "157787777751",
        "157787777751",
    },
    {
        //Tophat
        "111111111111",
        "111555555111",
        "111555585111",
        "111555555111",
        "111555555111",
        "111888888111",
        "115555555511",
        "111111111111",
    },
    {
        //Loafers
        "111111888111",
        "111118338111",
        "111183338111",
        "111118384811",
        "111111844481",
        "188111184811",
        "138811118111",
        "133844411111",
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

char equipzones_sprites_proper[EQUIPZONES + 1][9][MAXCOLS] = {
    {
        //HEAD
        "111111111111",
        "111188881111",
        "111888888111",
        "111818818111",
        "111888888111",
        "111881188111",
        "111188881111",
        "111111111111",
    },
    {
        //TORSO
        "111111111111",
        "111118811111",
        "111888888111",
        "118888888811",
        "188188881881",
        "181188881181",
        "111188881111",
        "111188881111",
    },
    {
        //LEGS
        "118811188111",
        "118881188111",
        "111884122111",
        "111144422111",
        "111114442111",
        "111144442221",
        "111444422221",
        "114444111111",
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
