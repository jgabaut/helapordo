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
#include "bluetroll_wonder.h"

char bluetroll_wonder[61][MAXROWS][MAXCOLS] = {

	//Frame 1
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 2
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 3
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"1111@@117777711@1",
		"111@@1117777711@1",
		"11111111777771111",
	},

	//Frame 4
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"1111@@117777711@1",
		"111@@1117777711@1",
		"11111111777771111",
	},

	//Frame 5
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"1111@@117777711@1",
		"111@@1117777711@1",
		"11111111777771111",
	},

	//Frame 6
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"1111@@@1777771@@1",
		"11@@@1117777711@1",
		"111111117777711@1",
		"11111111777771111",
	},

	//Frame 7
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@@@@@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@@",
		"1111111177777111@",
		"11111111777771111",
	},

	//Frame 8
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@@@@@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@@",
		"1111111177777111@",
		"11111111777771111",
	},

	//Frame 9
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@@@@@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@@",
		"1111111177777111@",
		"11111111777771111",
	},

	//Frame 10
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@@@@@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@@",
		"1111111177777111@",
		"11111111777771111",
	},

	//Frame 11
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@@@@@@111",
		"111111@@@@@@@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@@",
		"1111111177777111@",
		"11111111777771111",
	},

	//Frame 12
	{
		"111\?1111111111111",
		"1\?1\?1111@@@@@1111",
		"1\?1\?111@@@@@@@111",
		"11\?111@@@@@@@@111",
		"11\?11@@@@@@@@@111",
		"\?1\?1114114@@@@111",
		"\?111111166@@@1111",
		"1\?\?111@444@@11111",
		"11\?1111@@@@@11111",
		"11\?11111@@@@@1111",
		"1\?1111@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@@",
		"1111111177777111@",
		"11111111777771111",
	},

	//Frame 13
	{
		"111\?1111111111111",
		"11\?11111@@@@@1111",
		"111\?\?11@@@@@@@111",
		"111\?11@@@3@3@@111",
		"\?1\?11@@@@@@@@@111",
		"\?111114114@@@@111",
		"\?111111166@@@1111",
		"1\?1111@444@@11111",
		"1\?1\?111@@@@@11111",
		"1111\?111@@@@@1111",
		"1\?1111@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@@",
		"1111111177777111@",
		"11111111777771111",
	},

	//Frame 14
	{
		"111\?1111111111111",
		"11\?11111@@@@@1111",
		"111\?\?11@@@@@@@111",
		"111\?11@@@3@3@@111",
		"\?1\?11@@@@@@@@@111",
		"\?111114114@@@@111",
		"\?111111166@@@1111",
		"1\?1111@444@@11111",
		"1\?1\?111@@@@@11111",
		"1111\?111@@@@@1111",
		"1\?1111@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@@",
		"1111111177777111@",
		"11111111777771111",
	},

	//Frame 15
	{
		"11111111111111111",
		"11\?11111@@@@@1111",
		"1111\?11@@@@@@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"111\?111@@@@@11111",
		"\?1111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@1",
		"111111117777711@1",
		"11111111777771111",
	},

	//Frame 16
	{
		"11111111111111111",
		"11\?11111@@@@@1111",
		"1111\?11@@@@@@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"111\?111@@@@@11111",
		"\?1111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@1",
		"111111117777711@1",
		"11111111777771111",
	},

	//Frame 17
	{
		"11111111111111111",
		"11\?11111@@@@@1111",
		"1111\?11@@@@@@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"111\?111@@@@@11111",
		"\?1111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@1",
		"111111117777711@1",
		"11111111777771111",
	},

	//Frame 18
	{
		"11111411111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"111111@@@@@@@@111",
		"14111@@@@@@@@@111",
		"1111114164@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111141@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@1",
		"111111117777711@1",
		"11111111777771111",
	},

	//Frame 19
	{
		"11111411111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"111111@@@@@@@@111",
		"14111@@@@@@@@@111",
		"1111114164@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111141@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@1",
		"111111117777711@1",
		"11111111777771111",
	},

	//Frame 20
	{
		"11111411111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"111111@@@@@@@@111",
		"14111@@@@@@@@@111",
		"1111114164@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111141@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@1",
		"111111117777711@1",
		"11111111777771111",
	},

	//Frame 21
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"1@@11@@177<771@@1",
		"111@@@@1777771@@1",
		"111111117777711@1",
		"111111117777711@1",
		"11111111777771111",
	},

	//Frame 22
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"111@@@@1777771@@1",
		"11@111117777711@1",
		"111111117777711@1",
		"11111111777771111",
	},

	//Frame 23
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"111@@@@1777771@@1",
		"11@111117777711@1",
		"111111117777711@1",
		"11111111777771111",
	},

	//Frame 24
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"1111@@@1777771@@1",
		"111@@1117777711@1",
		"111111117777711@1",
		"11111111777771111",
	},

	//Frame 25
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"1111@@@1777771@@1",
		"111@@1117777711@1",
		"111111117777711@1",
		"11111111777771111",
	},

	//Frame 26
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"1111@@@1777771@@1",
		"111@@1117777711@1",
		"111@11117777711@1",
		"11111111777771111",
	},

	//Frame 27
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"111111@@@3@3@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 28
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@3@3@@111",
		"1111111@@3@3@@@11",
		"1111111@@@@@@@@@1",
		"1111111@@@@411411",
		"11111111@@@661111",
		"111111111@@444@11",
		"111111111@@@@@111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 29
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@@@@@@111",
		"1111111@@3@3@@@11",
		"1111111@@@@@@@@@1",
		"1111111@@@@411411",
		"11111111@@@661111",
		"111111111@@444@11",
		"111111111@@@@@111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 30
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@@@@@@111",
		"1111111@@@@@@@@11",
		"1111111@@@@@@@@@1",
		"1111111@@@@411411",
		"11111111@@@661111",
		"111111111@@444@11",
		"111111111@@@@@111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 31
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@@@@@@111",
		"1111111@@@@@@@@11",
		"1111111@@@@@@@@@1",
		"1111111@@@@411411",
		"11111111@@@661111",
		"111111111@@444@11",
		"111111111@@@@@111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 32
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@@@@@@111",
		"1111111@@=@=@@@11",
		"1111111@@@@@@@@@1",
		"1111111@@@@411411",
		"11111111@@@661111",
		"111111111@@444@11",
		"111111111@@@@@111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 33
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@@@@@@111",
		"1111111@@=@=@@@11",
		"1111111@@@@@@@@@1",
		"1111111@@@@411411",
		"11111111@@@661111",
		"111111111@@444@11",
		"111111111@@@@@111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 34
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@=@=@@111",
		"1111111@@=@=@@@11",
		"1111111@@@@@@@@@1",
		"1111111@@@@411411",
		"11111111@@@661111",
		"111111111@@444@11",
		"111111111@@@@@111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 35
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@=@=@@111",
		"111111@@@=@=@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 36
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@=@=@@111",
		"111111@@@=@=@@111",
		"11111@@@@@@@@@111",
		"1111114114@@@@111",
		"1111111166@@@1111",
		"111111@444@@11111",
		"1111111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 37
	{
		"11111111111111111",
		"1111111@@@@@11111",
		"111111@@@@@@@1111",
		"11111@@@=@=@@1111",
		"1111@@@@@@@@@1111",
		"111114114@@@@1111",
		"111111166@@@11111",
		"11111@444@@111111",
		"111111@@@@@111111",
		"1111111@@@@@11111",
		"11111@@77<77@@111",
		"11111@@7<<<7@@111",
		"1111@@177<771@@11",
		"1111@@1777771@@11",
		"1111@117777711@11",
		"1111@117777711@11",
		"11111117777711111",
	},

	//Frame 38
	{
		"11111111111111111",
		"111111@@@@@111111",
		"11111@@@@@@@11111",
		"1111@@@=@=@@11111",
		"111@@@@@@@@@11111",
		"11114114@@@@11111",
		"11111166@@@111111",
		"1111@444@@1111111",
		"11111@@@@@1111111",
		"111111@@@@@111111",
		"1111@@77<77@@1111",
		"1111@@7<<<7@@1111",
		"111@@177<771@@111",
		"111@@1777771@@111",
		"111@117777711@111",
		"111@117777711@111",
		"11111177777111111",
	},

	//Frame 39
	{
		"11111111111111111",
		"11111@@@@@1111111",
		"1111@@@@@@@111111",
		"111@@@@@@@@111111",
		"11@@@@@@@@@111111",
		"1114114@@@@111111",
		"1111166@@@1111111",
		"111@444@@11111111",
		"1111@@@@@11111111",
		"11111@@@@@1111111",
		"111@@77<77@@11111",
		"111@@7<<<7@@11111",
		"11@@177<771@@1111",
		"11@@1777771@@1111",
		"11@117777711@1111",
		"11@117777711@1111",
		"11111777771111111",
	},

	//Frame 40
	{
		"11111111111111111",
		"11111@@@@@1111111",
		"1111@@@@@@@111111",
		"111@@@@@@@@111111",
		"11@@@@@@@@@111111",
		"1114114@@@@111111",
		"1111166@@@1111111",
		"111@444@@11111111",
		"1111@@@@@11111111",
		"11111@@@@@1111111",
		"111@@77<77@@11111",
		"111@@7<<<7@@11111",
		"11@@177<771@@1111",
		"11@@1777771@@1111",
		"11@117777711@1111",
		"11@117777711@1111",
		"11111777771111111",
	},

	//Frame 41
	{
		"11111111111111111",
		"11111@@@@@1111111",
		"1111@@@@@@@111111",
		"111@@@3@3@@111111",
		"11@@@@@@@@@111111",
		"1114114@@@@111111",
		"1111166@@@1111111",
		"111@444@@11111111",
		"1111@@@@@11111111",
		"11111@@@@@1111111",
		"111@@77<77@@11111",
		"111@@7<<<7@@11111",
		"11@@177<771@@1111",
		"11@@1777771@@1111",
		"11@117777711@1111",
		"11@117777711@1111",
		"11111777771111111",
	},

	//Frame 42
	{
		"11111111111111111",
		"111@@@@@111111111",
		"11@@3@3@@11111111",
		"1@@@3@3@@11111111",
		"@@@@@@@@@11111111",
		"14114@@@@11111111",
		"11166@@@111111111",
		"1@444@@1111111111",
		"11@@@@@1111111111",
		"111@@@@@111111111",
		"1@@77<77@@1111111",
		"1@@7<<<7@@1111111",
		"@@177<771@@111111",
		"@@1777771@@111111",
		"@117777711@111111",
		"@117777711@111111",
		"11177777111111111",
	},

	//Frame 43
	{
		"11111111111111111",
		"11@@@@@1111111111",
		"1@@3@3@@111111111",
		"@@@3@3@@111111111",
		"@@@@@@@@11111111@",
		"4114@@@@111111111",
		"1166@@@1111111111",
		"@444@@11111111111",
		"1@@@@@11111111111",
		"11@@@@@1111111111",
		"@@77<77@@11111111",
		"@@7<<<7@@11111111",
		"@177<771@@111111@",
		"@1777771@@111111@",
		"117777711@111111@",
		"117777711@111111@",
		"11777771111111111",
	},

	//Frame 44
	{
		"11111111111111111",
		"1@@@@@11111111111",
		"@@3@3@@1111111111",
		"@@3@3@@111111111@",
		"@@@@@@@11111111@@",
		"114@@@@1111111114",
		"166@@@11111111111",
		"444@@11111111111@",
		"@@@@@111111111111",
		"1@@@@@11111111111",
		"@77<77@@11111111@",
		"@7<<<7@@11111111@",
		"177<771@@111111@@",
		"1777771@@111111@@",
		"17777711@111111@1",
		"17777711@111111@1",
		"17777711111111111",
	},

	//Frame 45
	{
		"11111111111111111",
		"@@@@@111111111111",
		"@3@3@@1111111111@",
		"@3@3@@111111111@@",
		"@@@@@@11111111@@@",
		"14@@@@11111111141",
		"66@@@111111111111",
		"44@@11111111111@4",
		"@@@@111111111111@",
		"@@@@@111111111111",
		"77<77@@11111111@@",
		"7<<<7@@11111111@@",
		"77<771@@111111@@1",
		"777771@@111111@@1",
		"7777711@111111@11",
		"7777711@111111@11",
		"77777111111111111",
	},

	//Frame 46
	{
		"11111111111111111",
		"@@@@111111111111@",
		"@@@@@1111111111@@",
		"3@3@@111111111@@@",
		"@@@@@11111111@@@@",
		"4@@@@111111111411",
		"6@@@1111111111116",
		"4@@11111111111@44",
		"@@@111111111111@@",
		"@@@@111111111111@",
		"7<77@@11111111@@7",
		"<<<7@@11111111@@7",
		"7<771@@111111@@17",
		"77771@@111111@@17",
		"777711@111111@117",
		"777711@111111@117",
		"77771111111111117",
	},

	//Frame 47
	{
		"11111111111111111",
		"@@@111111111111@@",
		"@@@@1111111111@@@",
		"@@@@111111111@@@@",
		"@@@@11111111@@@@@",
		"@@@@1111111114114",
		"@@@11111111111166",
		"@@11111111111@444",
		"@@111111111111@@@",
		"@@@111111111111@@",
		"<77@@11111111@@77",
		"<<7@@11111111@@7<",
		"<771@@111111@@177",
		"7771@@111111@@177",
		"77711@111111@1177",
		"77711@111111@1177",
		"77711111111111177",
	},

	//Frame 48
	{
		"11111111111111111",
		"@@111111111111@@@",
		"@@@1111111111@@@@",
		"3@@111111111@@@3@",
		"@@@11111111@@@@@@",
		"@@@1111111114114@",
		"@@11111111111166@",
		"@11111111111@444@",
		"@111111111111@@@@",
		"@@111111111111@@@",
		"77@@11111111@@77<",
		"<7@@11111111@@7<<",
		"771@@111111@@177<",
		"771@@111111@@1777",
		"7711@111111@11777",
		"7711@111111@11777",
		"77111111111111777",
	},

	//Frame 49
	{
		"11111111111111111",
		"@111111111111@@@@",
		"@@1111111111@@3@3",
		"@@111111111@@@3@3",
		"@@11111111@@@@@@@",
		"@@1111111114114@@",
		"@11111111111166@@",
		"11111111111@444@@",
		"111111111111@@@@@",
		"@111111111111@@@@",
		"7@@11111111@@77<7",
		"7@@11111111@@7<<<",
		"71@@111111@@177<7",
		"71@@111111@@17777",
		"711@111111@117777",
		"711@111111@117777",
		"71111111111117777",
	},

	//Frame 50
	{
		"11111111111111111",
		"111111111111@@@@@",
		"@1111111111@@3@3@",
		"@111111111@@@3@3@",
		"@11111111@@@@@@@@",
		"@1111111114114@@@",
		"11111111111166@@@",
		"1111111111@444@@1",
		"11111111111@@@@@1",
		"111111111111@@@@@",
		"@@11111111@@77<77",
		"@@11111111@@7<<<7",
		"1@@111111@@177<77",
		"1@@111111@@177777",
		"11@111111@1177777",
		"11@111111@1177777",
		"11111111111177777",
	},

	//Frame 51
	{
		"11111111111111111",
		"111<<<11111@@@@@1",
		"11<111<111@@3@3@@",
		"11<111<11@@@3@3@@",
		"11111<11@@@@@@@@@",
		"1111<<1114114@@@@",
		"1111<11111166@@@1",
		"111111111@444@@11",
		"1111<11111@@@@@11",
		"11111111111@@@@@1",
		"@11111111@@77<77@",
		"@11111111@@7<<<7@",
		"@@111111@@177<771",
		"@@111111@@1777771",
		"1@111111@11777771",
		"1@111111@11777771",
		"11111111111777771",
	},

	//Frame 52
	{
		"11111111111111111",
		"11<<<11111@@@@@11",
		"1<111<111@@3@3@@1",
		"1<111<11@@@3@3@@1",
		"1111<11@@@@@@@@@1",
		"111<<1114114@@@@1",
		"111<11111166@@@11",
		"11111111@444@@111",
		"111<11111@@@@@111",
		"1111111111@@@@@11",
		"11111111@@77<77@@",
		"11111111@@7<<<7@@",
		"@111111@@177<771@",
		"@111111@@1777771@",
		"@111111@117777711",
		"@111111@117777711",
		"11111111117777711",
	},

	//Frame 53
	{
		"11111111111111111",
		"1<<<11111@@@@@111",
		"<111<111@@3@3@@11",
		"<111<11@@@3@3@@11",
		"111<11@@@@@@@@@11",
		"11<<1114114@@@@11",
		"11<11111166@@@111",
		"1111111@444@@1111",
		"11<11111@@@@@1111",
		"111111111@@@@@111",
		"1111111@@77<77@@1",
		"1111111@@7<<<7@@1",
		"111111@@177<771@@",
		"111111@@1777771@@",
		"111111@117777711@",
		"111111@117777711@",
		"11111111177777111",
	},

	//Frame 54
	{
		"11111111111111111",
		"1<<<1111@@@@@1111",
		"1919<11@@3@3@@111",
		"111<91@@@3@3@@111",
		"119<1@@@@@@@@@111",
		"1191114114@@@@111",
		"11<1111166@@@1111",
		"111111@444@@11111",
		"1191111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 55
	{
		"11111111111111111",
		"1<1<1111@@@@@1111",
		"1119111@@3@3@@111",
		"1<1<11@@@3@3@@111",
		"119<1@@@@@@@@@111",
		"1191114114@@@@111",
		"1191111166@@@1111",
		"111111@444@@11111",
		"1191111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 56
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"111<111@@3@3@@111",
		"1<9111@@@3@3@@111",
		"119<1@@@@@@@@@111",
		"1191114114@@@@111",
		"1191111166@@@1111",
		"111111@444@@11111",
		"1191111@@@@@11111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 57
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1191111@@3@3@@111",
		"1191111@@3@3@@@11",
		"1191111@@@@@@@@@1",
		"1191111@@@@411411",
		"11911111@@@661111",
		"111111111@@444@11",
		"119111111@@@@@111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 58
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1191111@@@@@@@111",
		"1191111@@3@3@@@11",
		"1191111@@@@@@@@@1",
		"1191111@@@@411411",
		"11911111@@@661111",
		"111111111@@444@11",
		"119111111@@@@@111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 59
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@@@@@@111",
		"1111111@@@@@@@@11",
		"1111111@@@@@@@@@1",
		"1111111@@@@411411",
		"11111111@@@661111",
		"111111111@@444@11",
		"111111111@@@@@111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

	//Frame 60
	{
		"11111111111111111",
		"11111111@@@@@1111",
		"1111111@@@@@@@111",
		"1111111@@3@3@@@11",
		"1111111@@@@@@@@@1",
		"1111111@@@@411411",
		"11111111@@@661111",
		"111111111@@444@11",
		"111111111@@@@@111",
		"11111111@@@@@1111",
		"111111@@77<77@@11",
		"111111@@7<<<7@@11",
		"11111@@177<771@@1",
		"11111@@1777771@@1",
		"11111@117777711@1",
		"11111@117777711@1",
		"11111111777771111",
	},

};
