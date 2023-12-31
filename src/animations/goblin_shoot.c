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
#include "goblin_shoot.h"

char goblin_shoot[61][MAXROWS][MAXCOLS] = {

	//Frame 1
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>77777>>>>>>>1",
		"1>777777777>>>>>1",
		"17777777@777>>>>1",
		"177>77777>777>>>1",
		"17>>77777>>77>>>1",
		"1=>>;;;;;>9=9>>>1",
		"1=>>:::::>9==>>>1",
		"11111111111111111",
	},

	//Frame 2
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>77777>>>>>>>1",
		"1>777777777>>>>>1",
		"17777777@777>>>>1",
		"177>77777>777>>>1",
		"17>>77777>>77>>>1",
		"1=>>;;;;;>9=9>>>1",
		"1=>>:::::>9==>>>1",
		"11111111111111111",
	},

	//Frame 3
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>7777777>>>>>1",
		"1>7777777777>>>>1",
		"177777777>777>>>1",
		"177>77777>>777>>1",
		"17>>77777>>9=9>>1",
		"1=>>;;;;;>>9==>>1",
		"1=>>:::::>>9>>>>1",
		"11111111111111111",
	},

	//Frame 4
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>7777777>>>>>1",
		"1>7777777777>>>>1",
		"177777777>777>>>1",
		"177>77777>>777>>1",
		"17>>77777>>9=9>>1",
		"1=>>;;;;;>>9==>>1",
		"1=>>:::::>>9>>>>1",
		"11111111111111111",
	},

	//Frame 5
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>777777777>>>>>1",
		"17777777@777>9991",
		"177>77777>777==>1",
		"17>>77777>>779=>1",
		"1=>>;;;;;>>>>>>>1",
		"1=>>:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 6
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>777777777>>>>>1",
		"17777777@777>9991",
		"177>77777>777==>1",
		"17>>77777>>779=>1",
		"1=>>;;;;;>>>>>>>1",
		"1=>>:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 7
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>>77777777>>>>>1",
		"1>777777@777>9991",
		"1777@7777>777==>1",
		"177>77777>>779=>1",
		"1==>;;;;;>>>>>>>1",
		"1==>:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 8
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>>77777777>>>>>1",
		"1>777777@777>9991",
		"1777@7777>777==>1",
		"177>77777>>779=>1",
		"1==>;;;;;>>>>>>>1",
		"1==>:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 9
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>77777>>>>>>>1",
		"1>>7777777>>>>>>1",
		"1>777777@77>99931",
		"1777@777@777==>>1",
		"177>77777>779=>>1",
		"1==>;;;;;>>>>>>>1",
		"1==>:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 10
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>77777>>>>>>>1",
		"1>>7777777>>>>>>1",
		"1>777777@77>99931",
		"1777@777@777==>>1",
		"177>77777>779=>>1",
		"1==>;;;;;>>>>>>>1",
		"1==>:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 11
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>77777>>>>>>>1",
		"1>>7777777>>>>>>1",
		"1>777777@77>99931",
		"1>77@777@777==>>1",
		"1>77@7777>779=>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 12
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>77777>>>>>>>1",
		"1>>7777777>>>>>>1",
		"1>777777@77>99931",
		"1>77@777@777==>>1",
		"1>77@7777>779=>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 13
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>>77777777>99931",
		"1>777777@777==>>1",
		"1>77@777@>779=>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 14
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>>77777777>99931",
		"1>777777@777==>>1",
		"1>77@777@>779=>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 15
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>=88==>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>>77777777>99931",
		"1>777777@777==>>1",
		"1>77@777@>779=>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 16
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>=88==>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>>77777777>99931",
		"1>777777@777==>>1",
		"1>77@777@>779=>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 17
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>=88==>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>>77777777>99931",
		"1>777777@777==>>1",
		"1>77@777@>779=>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 18
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>=88==>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>>77777777>99931",
		"1>777777@777==>>1",
		"1>77@777@>779=>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 19
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>=88==>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 20
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>=88==>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 21
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 22
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 23
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>3>>1",
		"1>>>=====>>99>>>1",
		"1>>>>===>>>9=>2>1",
		"1>>>777777>7==>>1",
		"1>>7777@777779>>1",
		"1>777777@777>>>>1",
		"1>77@777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 24
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>3>>1",
		"1>>>=====>>99>>>1",
		"1>>>>===>>>9=>2>1",
		"1>>>777777>7==>>1",
		"1>>7777@777779>>1",
		"1>777777@777>>>>1",
		"1>77@777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 25
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>3>>1",
		"1>>>=====>>99>>>1",
		"1>>>>===>>>9=>2>1",
		"1>>>777777>7==>>1",
		"1>>7777@777779>>1",
		"1>777777@777>>>>1",
		"1>77@777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 26
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>3>>1",
		"1>>>=====>>99>>>1",
		"1>>>>===>>>9=>2>1",
		"1>>>777777>7==>>1",
		"1>>7777@777779>>1",
		"1>777777@777>>>>1",
		"1>77@777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 27
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>3>>1",
		"1>>>>===>>>99>>21",
		"1>>>777777>9=>>>1",
		"1>>7777@7777==>>1",
		"1>777777@>7779>>1",
		"1>77@7777>>7>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 28
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>3>>1",
		"1>>>>===>>>99>>21",
		"1>>>777777>9=>>>1",
		"1>>7777@7777==>>1",
		"1>777777@>7779>>1",
		"1>77@7777>>7>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 29
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>3>>1",
		"1>>>>===>>>99>>21",
		"1>>>777777>9=>>>1",
		"1>>7777@7777==>>1",
		"1>777777@>7779>>1",
		"1>77@7777>>7>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 30
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>3>>1",
		"1>>>>===>>>99>>21",
		"1>>>777777>9=>>>1",
		"1>>7777@7777==>>1",
		"1>777777@>7779>>1",
		"1>77@7777>>7>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 31
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 32
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 33
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 34
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 35
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 36
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 37
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 38
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 39
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>3>>1",
		"1>>>=====>>99>>>1",
		"1>>>>===>>>9=>2>1",
		"1>>>777777>7==>>1",
		"1>>7777@777779>>1",
		"1>777777@777>>>>1",
		"1>77@777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 40
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>3>>1",
		"1>>>=====>>99>>>1",
		"1>>>>===>>>9=>2>1",
		"1>>>777777>7==>>1",
		"1>>7777@777779>>1",
		"1>777777@777>>>>1",
		"1>77@777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 41
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>3>>1",
		"1>>>=====>>99>>>1",
		"1>>>>===>>>9=>2>1",
		"1>>>777777>7==>>1",
		"1>>7777@777779>>1",
		"1>777777@777>>>>1",
		"1>77@777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 42
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>3>>1",
		"1>>>=====>>99>>>1",
		"1>>>>===>>>9=>2>1",
		"1>>>777777>7==>>1",
		"1>>7777@777779>>1",
		"1>777777@777>>>>1",
		"1>77@777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 43
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>3>>1",
		"1>>>>===>>>99>>21",
		"1>>>777777>9=>>>1",
		"1>>7777@7777==>>1",
		"1>777777@>7779>>1",
		"1>77@7777>>7>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 44
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>3>>1",
		"1>>>>===>>>99>>21",
		"1>>>777777>9=>>>1",
		"1>>7777@7777==>>1",
		"1>777777@>7779>>1",
		"1>77@7777>>7>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 45
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>3>>1",
		"1>>>>===>>>99>>21",
		"1>>>777777>9=>>>1",
		"1>>7777@7777==>>1",
		"1>777777@>7779>>1",
		"1>77@7777>>7>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 46
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>3>>1",
		"1>>>>===>>>99>>21",
		"1>>>777777>9=>>>1",
		"1>>7777@7777==>>1",
		"1>777777@>7779>>1",
		"1>77@7777>>7>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 47
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 48
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 49
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>9993>1",
		"1>>>7777777==>>>1",
		"1>>7777@7779=>>>1",
		"1>777777@>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 50
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>=88==>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>>77777777>99931",
		"1>777777@777==>>1",
		"1>77@777@>779=>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 51
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>=88==>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>>77777777>99931",
		"1>777777@777==>>1",
		"1>77@777@>779=>>1",
		"1>77@7777>>>>>>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 52
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>=88==>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>77777>>>>>>>1",
		"1>>7777777>>>>>>1",
		"1>777777@77>99931",
		"1>77@777@777==>>1",
		"1>77@777@>779=>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 53
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>=88==>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>77777>>>>>>>1",
		"1>>7777777>>>>>>1",
		"1>777777@77>99931",
		"1>77@777@777==>>1",
		"1>77@777@>779=>>1",
		"1>==;;;;;>>>>>>>1",
		"1>==:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 54
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>=88==>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>77777>>>>>>>1",
		"1>>7777777>>>>>>1",
		"1>777777@77>99931",
		"1777@777@777==>>1",
		"177>7777@>779=>>1",
		"1==>;;;;;>>>>>>>1",
		"1==>:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 55
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>=88==>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>77777>>>>>>>1",
		"1>>7777777>>>>>>1",
		"1>777777@77>99931",
		"1777@777@777==>>1",
		"177>7777@>779=>>1",
		"1==>;;;;;>>>>>>>1",
		"1==>:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 56
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>>77777777>>>>>1",
		"1>777777@777>9991",
		"1777@777@>777==>1",
		"177>77777>>779=>1",
		"1==>;;;;;>>>>>>>1",
		"1==>:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 57
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>777777777>>>>>1",
		"17777777@777>9991",
		"177>7777@>777==>1",
		"17>>77777>>779=>1",
		"1=>>;;;;;>>>>>>>1",
		"1=>>:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 58
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>777777>>>>>>1",
		"1>777777777>>>>>1",
		"17777777@777>9991",
		"177>7777@>777==>1",
		"17>>77777>>779=>1",
		"1=>>;;;;;>>>>>>>1",
		"1=>>:::::>>>>>>>1",
		"11111111111111111",
	},

	//Frame 59
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>7777777>>>>>1",
		"1>7777777777>>>>1",
		"177777777>777>>>1",
		"177>77777>>777>>1",
		"17>>77777>>9=9>>1",
		"1=>>;;;;;>>9==>>1",
		"1=>>:::::>>9>>>>1",
		"11111111111111111",
	},

	//Frame 60
	{
		"11111111111111111",
		"1>>>>>=>>>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"13=>=====>=3>>>>1",
		"1====1=1====>>>>1",
		"1>>=======>>>>>>1",
		"1>>>=888=>>>>>>>1",
		"1>>>=====>>>>>>>1",
		"1>>>>===>>>>>>>>1",
		"1>>>7777777>>>>>1",
		"1>7777777777>>>>1",
		"177777777>777>>>1",
		"177>77777>>777>>1",
		"17>>77777>>9=9>>1",
		"1=>>;;;;;>>9==>>1",
		"1=>>:::::>>9>>>>1",
		"11111111111111111",
	},

};
