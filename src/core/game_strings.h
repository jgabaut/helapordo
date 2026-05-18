// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2022-2026 jgabaut

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

#ifndef GAME_STRINGS_H
#define GAME_STRINGS_H
#include "game_core.h"
const char *stringFrom_HLPD_DirectionalKeys_Schema(int dks);
char *stringFrom_HLPD_KeyClass(HLPD_KeyClass k);
char *stringFrom_HLP_Region_Type(HLP_Region_Type t);
char *stringFrom_OP_res(OP_res r);
char *stringFrom_saveType(saveType s);
char *stringFrom_fightResult(fightResult fr);
char *stringFromTurnOP(turnOption_OP t);
char *stringFromFoeTurnOP(foeTurnOption_OP ft);
char *stringFromStatus(fighterStatus s);
char *stringFromClass(fighterClass f);
char *stringFromEClass(enemyClass e);
char *stringFromBossClass(bossClass b);
char *stringFromConsumables(int c);
char *stringFromEquips(int e);
char *stringFromEquipzones(int z);
char *stringFromQuality(int q);
char *stringFromArtifacts(int a);
char *stringFromFloorclass(floorClass fc);

char *descStringFromSpecial(fighterClass class, specialMove move);
char *nameStringFromSpecial(fighterClass class, specialMove move);
char *descStringFromSkill(skillType class);
char *nameStringFromSkill(skillType class);

char *nameStringFromPerk(int p);
char *descStringFromPerk(int p);

char *descStringFromTreasure(int t);
char *descStringFromChest(int c);

char *stringFromWinconClass(winconClass w);

char *stringFromFoePartyClass(foePartyClass fp);
char *stringFromRoom(roomClass r);
char *stringFromGamemode(Gamemode g);
#endif // GAME_STRINGS_H
