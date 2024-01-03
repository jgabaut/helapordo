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
#include "alt_chest_opening.h"

char alt_chest_opening[26][MAXROWS][MAXCOLS] = {

	//Frame 1
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1====9999999====1",
		"1===994444499===1",
		"1==99444444499==1",
		"1==94444444449==1",
		"1==99994449999==1",
		"1==94499999449==1",
		"1==94444544449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==99444444499==1",
		"1===999999999===1",
		"1===============1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 2
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1====9999999====1",
		"1===994444499===1",
		"1==99444444499==1",
		"1==94444444449==1",
		"1==99994449999==1",
		"1==94499999449==1",
		"1==94444544449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==99444444499==1",
		"1===999999999===1",
		"1=7=============1",
		"177=============1",
		"11111111111111111",
	},

	//Frame 3
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1====9999999====1",
		"1===994444499===1",
		"1==99444444499==1",
		"1==94444444449==1",
		"1==99994449999==1",
		"1==94499999449==1",
		"1==94444544449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==99444444499==1",
		"1==7999999999===1",
		"1=77============1",
		"177=============1",
		"11111111111111111",
	},

	//Frame 4
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1====9999999====1",
		"1===994444499===1",
		"1==99444444499==1",
		"1==94444444449==1",
		"1==99994449999==1",
		"1==94499999449==1",
		"1==94444544449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==97444444499==1",
		"1==7799999999===1",
		"1=77============1",
		"177=7===========1",
		"11111111111111111",
	},

	//Frame 5
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1====9999999====1",
		"1===994444499===1",
		"1==99444444499==1",
		"1==94444444449==1",
		"1==99994449999==1",
		"1==94499999449==1",
		"1==94444544449==1",
		"1==94444444449==1",
		"1==94744444449==1",
		"1==97444444499==1",
		"17=7799999999===1",
		"1777=7==========1",
		"177=============1",
		"11111111111111111",
	},

	//Frame 6
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1====9999999====1",
		"1===994444499===1",
		"1==99444444499==1",
		"1==94444444449==1",
		"1==99994449999==1",
		"1==94499999449==1",
		"1==94447544449==1",
		"1==94474444449==1",
		"1==94774444449==1",
		"1=774747444499==1",
		"1=77779999999===1",
		"17=77===========1",
		"1=777===========1",
		"11111111111111111",
	},

	//Frame 7
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1====9999999====1",
		"1===994444499===1",
		"1==99444464499==1",
		"1==94444446449==1",
		"1==99994469999==1",
		"1==94466799449==1",
		"1==94447644449==1",
		"1==94477464449==1",
		"1==77474744449==1",
		"1==77774444499==1",
		"1=7=779999999===1",
		"1==777==========1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 8
	{
		"11111111111111111",
		"1===============1",
		"1==========6====1",
		"1====9999966====1",
		"1===994444699===1",
		"1==69444464499==1",
		"1=666444446449==1",
		"1==99694469999==1",
		"1==94466799449==1",
		"1==94447644449==1",
		"1==94477664449==1",
		"1==77476744449==1",
		"1==77774644499==1",
		"1=7=779969999===1",
		"1==777==========1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 9
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1====9699966====1",
		"1===964444699===1",
		"1===9644464499==1",
		"1==66644446649==1",
		"1==99694469966==1",
		"1==94466799449==1",
		"1==94447644449==1",
		"1==94477666449==1",
		"1==77476746649==1",
		"1==77774644499==1",
		"1=7=779969999===1",
		"1==777==========1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 10
	{
		"11111111111111111",
		"1===============1",
		"1==========6====1",
		"1====9699966====1",
		"1===964444699===1",
		"1==69644464499==1",
		"1=6666444466496=1",
		"1==996944699666=1",
		"1==94466799449==1",
		"1==94447644449==1",
		"1==94477666449==1",
		"1==77476746649==1",
		"1==77774644669==1",
		"1=7=779969996===1",
		"1==777==========1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 11
	{
		"11111111111111111",
		"1======6========1",
		"1==6===6===6====1",
		"1==669696966====1",
		"1===664464699===1",
		"1==69644664499==1",
		"1=6666444466496=1",
		"1==996944699666=1",
		"1==96466799446==1",
		"1==66447644446==1",
		"1==944776664466=1",
		"1==774767466496=1",
		"1==77774646669==1",
		"1=7=779969696===1",
		"1==777====6=====1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 12
	{
		"11111111111111111",
		"1======6========1",
		"1==6===6===6==6=1",
		"1=6669696966==6=1",
		"1===6646646996==1",
		"1==69646664469==1",
		"1=6666464466496=1",
		"1==696964669666=1",
		"1==66466796646=61",
		"1=6664476446466=1",
		"1==944666664666=1",
		"1==776667466496=1",
		"1==76674646669==1",
		"1=76676969696===1",
		"1==6776===6=====1",
		"1=====6=========1",
		"11111111111111111",
	},

	//Frame 13
	{
		"11111111111111111",
		"1======6========1",
		"1==67==6===6==6=1",
		"1=6677696966==6=1",
		"1===6646646996==1",
		"1==69646664469==1",
		"1=6666464466496=1",
		"1==696664669666=1",
		"1==76466796646=61",
		"1=6664466446466=1",
		"1=7944666664666=1",
		"1==766667476496=1",
		"1==66674647669==1",
		"1=66676769676===1",
		"1==67667=76=====1",
		"1=====6=========1",
		"11111111111111111",
	},

	//Frame 14
	{
		"11111111111111111",
		"1==7=========7==1",
		"1==6=======6==7=1",
		"1=66=7996999==6=1",
		"1===6946644996==1",
		"1==69646664499==1",
		"176944464444796=1",
		"1==999664666696=1",
		"1==76496699646==1",
		"1=6644466446466=1",
		"1=794666666466==1",
		"1==96664444649==1",
		"17=99644647669==1",
		"1==6676799676===1",
		"1==67667=7======1",
		"1=7==========7==1",
		"11111111111111111",
	},

	//Frame 15
	{
		"11111111111111111",
		"1============7==1",
		"1===============1",
		"1====9996999====1",
		"1===9944644996==1",
		"1==99644664499==1",
		"17=94444444449==1",
		"1==99964466999==1",
		"1==96499999646==1",
		"1==644446646466=1",
		"1=794446666466==1",
		"1==94464444649==1",
		"1==99644644499==1",
		"1===996999699===1",
		"1===7==7=7======1",
		"1=7==========7==1",
		"11111111111111111",
	},

	//Frame 16
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1====9999999====1",
		"1===994444499===1",
		"1==99444444499==1",
		"1==94444444449==1",
		"1==99994469999==1",
		"1==94499999449==1",
		"1==94444664649==1",
		"1==94446644449==1",
		"1==94444444449==1",
		"1==99444444499==1",
		"1===999999999===1",
		"1===============1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 17
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1====9999999====1",
		"1===994444499===1",
		"1==99444444499==1",
		"1==94444444449==1",
		"1==99994449999==1",
		"1==94499999449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==99444444499==1",
		"1===999999999===1",
		"1===============1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 18
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1====9999999====1",
		"1===994444499===1",
		"1==99444444499==1",
		"1==94444444449==1",
		"1==99994449999==1",
		"1==94499999449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==99444444499==1",
		"1===999999999===1",
		"1===============1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 19
	{
		"11111111111111111",
		"1===============1",
		"1====9999999====1",
		"1===994444499===1",
		"1==99444444499==1",
		"1==94444444449==1",
		"1==99994449999==1",
		"1==91199999119==1",
		"1==94411111449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==99444444499==1",
		"1===999999999===1",
		"1===============1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 20
	{
		"11111111111111111",
		"1===============1",
		"1====9999999====1",
		"1===994444499===1",
		"1==99444444499==1",
		"1==94444444449==1",
		"1==99994449999==1",
		"1==91199999119==1",
		"1==94411111449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==99444444499==1",
		"1===999999999===1",
		"1===============1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 21
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1===999999999===1",
		"1==99444444499==1",
		"1==94444444449==1",
		"1==99994449999==1",
		"1==91199999119==1",
		"1==94411111449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==99444444499==1",
		"1===999999999===1",
		"1===============1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 22
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1====9999999====1",
		"1===994444499===1",
		"1==99994449999==1",
		"1==91199999119==1",
		"1==91111111119==1",
		"1==94411111449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==99444444499==1",
		"1===999999999===1",
		"1===============1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 23
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1===999999999===1",
		"1==99194449199==1",
		"1==91199999119==1",
		"1==91111111119==1",
		"1==91111111119==1",
		"1==94411111449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==99444444499==1",
		"1===999999999===1",
		"1===============1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 24
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1==99999999999==1",
		"1==91199999119==1",
		"1==91111111119==1",
		"1==91111111119==1",
		"1==91111111119==1",
		"1==94411111449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==99444444499==1",
		"1===999999999===1",
		"1===============1",
		"1===============1",
		"11111111111111111",
	},

	//Frame 25
	{
		"11111111111111111",
		"1===============1",
		"1===============1",
		"1==99999999999==1",
		"1==91199999119==1",
		"1==91111111119==1",
		"1==91111111119==1",
		"1==91111111119==1",
		"1==94411111449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==94444444449==1",
		"1==99444444499==1",
		"1===999999999===1",
		"1===============1",
		"1===============1",
		"11111111111111111",
	},

};
