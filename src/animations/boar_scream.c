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
#include "boar_scream.h"

char boar_scream[61][MAXROWS][MAXCOLS] = {

	//Frame 1
	{
		"11111111111111111",
		"1>>>>>>>9>>>>>>>1",
		"1>>>>>>>922222221",
		"1>>>>>>>921121121",
		"1>>>>>>>>23123121",
		"1>>>>>>8>22222221",
		"1>>>>>>8826666621",
		"1>>>>>>>886161681",
		"1>>9>>>>>86666681",
		"1>>99>22288111881",
		"1>>>>222221111121",
		"1>>>2222222111221",
		"1>>>2222222222221",
		"1>>22299>22>>>221",
		"1>>2>>>9>2>>>>>21",
		"1>:>>>>;>:>>>>>:1",
		"11111111111111111",
	},

	//Frame 2
	{
		"11111111111111111",
		"1>>>>>>>9>>>>>>>1",
		"1>>>>>>>922222221",
		"1>>>>>>>921121121",
		"1>>>>>>>>23123121",
		"1>>>>>>8>22222221",
		"1>>>>>>8826666621",
		"1>>>>>>>886161681",
		"1>>9>>>>>86666681",
		"1>>99>22288111881",
		"1>>>>222221111121",
		"1>>>2222222111221",
		"1>>>2222222222221",
		"1>>22299>22>>>221",
		"1>>2>>>9>2>>>>>21",
		"1>:>>>>;>:>>>>>:1",
		"11111111111111111",
	},

	//Frame 3
	{
		"11111111111111111",
		"1>>>>>>>9>>>>>>>1",
		"1>>>>>>>922222221",
		"1>>>>>>>921121121",
		"1>>>>>>>>23123121",
		"1>>>>>>8>22222221",
		"1>>>>>>8826666621",
		"1>>>>>>>886161681",
		"1>>>9>>>>86666681",
		"1>>>99>2288111881",
		"1>>>>>22221111121",
		"1>>>>222222111221",
		"1>>>>222222222221",
		"1>>>2229922>>>221",
		"1>>>2>>>92>>>>>21",
		"1>>:>>>>;:>>>>>:1",
		"11111111111111111",
	},

	//Frame 4
	{
		"11111111111111111",
		"1>>>>>>>9>>>>>>>1",
		"1>>>>>>>922222221",
		"1>>>>>>>921121121",
		"1>>>>>>>>23123121",
		"1>>>>>>8>22222221",
		"1>>>>>>8826666621",
		"1>>>>>>>886161681",
		"1>>>9>>>>86666681",
		"1>>>99>2288111881",
		"1>>>>>22221111121",
		"1>>>>222222111221",
		"1>>>>222222222221",
		"1>>>2229922>>>221",
		"1>>>2>>>92>>>>>21",
		"1>>:>>>>;:>>>>>:1",
		"11111111111111111",
	},

	//Frame 5
	{
		"11111111111111111",
		"1>>>>>>>9>>>>>>>1",
		"1>>>>>>>922222221",
		"1>>>>>>>921121121",
		"1>>>>>>>>23123121",
		"1>>>>>>8>22222221",
		"1>>>>>>8826666621",
		"1>>>>>>>886161681",
		"1>>>>9>>>86666681",
		"1>>>>99>288111881",
		"1>>>>>>2221111121",
		"1>>>>>22222111221",
		"1>>>>>22222222221",
		"1>>>>222922>>>221",
		"1>>>>2>>>2>>>>>21",
		"1>>>:>>>>:>>>>>:1",
		"11111111111111111",
	},

	//Frame 6
	{
		"11111111111111111",
		"1>>>>>>>9>>>>>>>1",
		"1>>>>>>>922222221",
		"1>>>>>>>921121121",
		"1>>>>>>>>23123121",
		"1>>>>>>8>22222221",
		"1>>>>>>8826666621",
		"1>>>>>>>886161681",
		"1>>>>9>>>86666681",
		"1>>>>99>288111881",
		"1>>>>>>2221111121",
		"1>>>>>22222111221",
		"1>>>>>22222222221",
		"1>>>>222922>>>221",
		"1>>>>2>>>2>>>>>21",
		"1>>>:>>>>:>>>>>:1",
		"11111111111111111",
	},

	//Frame 7
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2312312>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22111112>1",
		"1>>>>>222211122>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 8
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2312312>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 9
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2312312>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 10
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2312312>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 11
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2132132>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 12
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2132132>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 13
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2132132>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 14
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2132132>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 15
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2132132>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 16
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2312312>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 17
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2312312>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 18
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2312312>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 19
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2132132>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 20
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2132132>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 21
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2132132>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 22
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2132132>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 23
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2132132>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>9>>8666668>1",
		"1>>>>9928811188>1",
		"1>>>>>>22211122>1",
		"1>>>>>222222222>1",
		"1>>>>>222222222>1",
		"1>>>>22922>>>22>1",
		"1>>>>2>>2>>>>>2>1",
		"1>>>:>>>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 24
	{
		"11111111111111111",
		"1>>>>9>>>>>>>9>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>>>2132132>>>1",
		"1>>>8>2222222>8>1",
		"1>>>88266666288>1",
		"1>>>>886161688>>1",
		"1>>9>>8666668>>>1",
		"1>>9928811188>>>1",
		"1>>>>22211122>>>1",
		"1>>>222222222>>>1",
		"1>>>222222222>>>1",
		"1>>>2>229>>>22>>1",
		"1>>>2>2>9>>>>2>>1",
		"1>>>:>:>;>>>>:>>1",
		"11111111111111111",
	},

	//Frame 25
	{
		"11111111111111111",
		"1>>>>9>>>>>>>9>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>>>2132132>>>1",
		"1>>>8>2222222>8>1",
		"1>>>88266666288>1",
		"1>>>>886161688>>1",
		"1>>9>>8666668>>>1",
		"1>>9928811188>>>1",
		"1>>>>22211122>>>1",
		"1>>>222222222>>>1",
		"1>>>222222222>>>1",
		"1>>>2>229>>>22>>1",
		"1>>>2>2>9>>>>2>>1",
		"1>>>:>:>;>>>>:>>1",
		"11111111111111111",
	},

	//Frame 26
	{
		"11111111111111111",
		"1>>>>9>>>>>>>9>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>>>2132132>>>1",
		"1>>>8>2222222>8>1",
		"1>>>88266666288>1",
		"1>>>>886161688>>1",
		"1>>9>>8666668>>>1",
		"1>>9928811188>>>1",
		"1>>>>22211122>>>1",
		"1>>>222222222>>>1",
		"1>>>222222222>>>1",
		"1>>>2>229>>>22>>1",
		"1>>>2>2>9>>>>2>>1",
		"1>>>:>:>;>>>>:>>1",
		"11111111111111111",
	},

	//Frame 27
	{
		"11111111111111111",
		"1>>>>9>>>>>>>9>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>>>2132132>>>1",
		"1>>>8>2222222>8>1",
		"1>>>88266666288>1",
		"1>>>>886161688>>1",
		"1>>9>>8666668>>>1",
		"1>>9928811188>>>1",
		"1>>>>22211122>>>1",
		"1>>>222222222>>>1",
		"1>>>222222222>>>1",
		"1>>>2>229>>>22>>1",
		"1>>>2>2>9>>>>2>>1",
		"1>>>:>:>;>>>>:>>1",
		"11111111111111111",
	},

	//Frame 28
	{
		"11111111111111111",
		"1>>>>9>>>>>>>9>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>>>2132132>>>1",
		"1>>>8>2222222>8>1",
		"1>>>88266666288>1",
		"1>>>>886161688>>1",
		"1>>9>>8666668>>>1",
		"1>>9928811188>>>1",
		"1>>>>22211122>>>1",
		"1>>>222222222>>>1",
		"1>>>222222222>>>1",
		"1>>22922>>>22>>>1",
		"1>>2>>2>>>>>2>>>1",
		"1>:>>>:>>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 29
	{
		"11111111111111111",
		"1>>>>9>>>>>>>9>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>>>2132132>>>1",
		"1>>>8>2222222>8>1",
		"1>>>88266666288>1",
		"1>>>>886161688>>1",
		"1>>>9>8666668>>>1",
		"1>>>998811188>>>1",
		"1>>>>92211122>>>1",
		"1>>>>22222222>>>1",
		"1>>>>22222222>>>1",
		"1>>>2222>>>22>>>1",
		"1>>>2>2>>>>>2>>>1",
		"1>>:>>:>>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 30
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>>>2132132>>>1",
		"1>>>8>2222222>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>9>82666668>>1",
		"1>>>9988111888>>1",
		"1>>>>92211122>>>1",
		"1>>>>22221222>>>1",
		"1>>>>22222222>>>1",
		"1>>>2222>>>22>>>1",
		"1>>>2>2>>>>>2>>>1",
		"1>>:>>:>>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 31
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>>>2132132>>>1",
		"1>>>8>2222222>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>9>82666668>>1",
		"1>>>9988111888>>1",
		"1>>>>92211122>>>1",
		"1>>>>22221222>>>1",
		"1>>>>22222222>>>1",
		"1>>>2222>>>22>>>1",
		"1>>>2>2>>>>>2>>>1",
		"1>>:>>:>>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 32
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>8>2132132>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>88111888>>1",
		"1>>>992211122>>>1",
		"1>>>>92211122>>>1",
		"1>;9>22221222>>>1",
		"1>>9922222222>>>1",
		"1>>>2222>>>22>>>1",
		"1>>>2>2>>>>>2>>>1",
		"1>>:>>:>>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 33
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>8>2132132>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>88111888>>1",
		"1>>>992211122>>>1",
		"1>>>>92211122>>>1",
		"1>;9>22221222>>>1",
		"1>>9922222222>>>1",
		"1>>>2222>>>22>>>1",
		"1>>>2>2>>>>>2>>>1",
		"1>>:>>:>>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 34
	{
		"11111111111111111",
		"1>>><<>>>>>><<>>1",
		"1>>>><2222222<>>1",
		"1>>>><2112112<>>1",
		"1>>>8>21<21<2>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>881<1888>>1",
		"1>>>99221<122>>>1",
		"1>>>>92211122>>>1",
		"1>;9>22221222>>>1",
		"1>>9922222222>>>1",
		"1>>>2222>>>22>>>1",
		"1>>>2>2>>>>>2>>>1",
		"1>>:>>:>>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 35
	{
		"11111111111111111",
		"1>>><<>>>>>><<>>1",
		"1>>>><2222222<>>1",
		"1>>>><2112112<>>1",
		"1>>>8>21<21<2>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>881<1888>>1",
		"1>>>99221<122>>>1",
		"1>>>>92211122>>>1",
		"1>;9>22221222>>>1",
		"1>>9922222222>>>1",
		"1>>>2222>>>22>>>1",
		"1>>>2>2>>>>>2>>>1",
		"1>>:>>:>>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 36
	{
		"11111111111111111",
		"1>>><<>>>>>><<>>1",
		"1>>>><2222222<>>1",
		"1>>>><2112112<>>1",
		"1>>>8>21<21<2>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>881<1888>>1",
		"1>>>99221<122>>>1",
		"1>>>>92211122>>>1",
		"1>;9>22221222>>>1",
		"1>>9922222222>>>1",
		"1>>>2222>>>22>>>1",
		"1>>>2>2>>>>>2>>>1",
		"1>>:>>:>>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 37
	{
		"11111111111111111",
		"1>>><<>>>>>><<>>1",
		"1>>>><2222222<>>1",
		"1>>>><2112112<>>1",
		"1>>>8>21<21<2>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>881<1888>>1",
		"1>>>99221<122>>>1",
		"1>>>>92211122>>>1",
		"1>;9>22221222>>>1",
		"1>>9922222222>>>1",
		"1>>>2222>>>22>>>1",
		"1>>>2>2>>>>>2>>>1",
		"1>>:>>:>>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 38
	{
		"11111111111111111",
		"1>>><<>>>>>><<>>1",
		"1>>>><2222222<>>1",
		"1>>>><2112112<>>1",
		"1>>>8>21<21<2>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>88<<<888>>1",
		"1>>>9921<<<12>>>1",
		"1>>>>921<<<12>>>1",
		"1>;9>22211122>>>1",
		"1>>9922222222>>>1",
		"1>>>2222>>>22>>>1",
		"1>>>2>2>>>>>2>>>1",
		"1>>:>>:>>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 39
	{
		"11111111111111111",
		"1>>><<>>>>>><<>>1",
		"1>>>><2222222<>>1",
		"1>>>><2112112<>>1",
		"1>>>8>21<21<2>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>88<<<888>>1",
		"1>>>9921<<<12>>>1",
		"1>>>>921<<<12>>>1",
		"1>;9>22211122>>>1",
		"1>>9922222222>>>1",
		"1>>>2222>>>22>>>1",
		"1>>>2>2>>>>>2>>>1",
		"1>>:>>:>>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 40
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>8>21<21<2>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>88<<<888>>1",
		"1>>>9921<<<12>>>1",
		"1>>>>921<<<12>>>1",
		"1>>>>22211122>>>1",
		"1>>>222222222>>>1",
		"1>>22229>>>22>>>1",
		"1>>2>2>9>>>>2>>>1",
		"1>:>>:>;>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 41
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>8>21<21<2>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>88<<<888>>1",
		"1>>>9921<<<12>>>1",
		"1>>>>921<<<12>>>1",
		"1>>>>22211122>>>1",
		"1>>>222222222>>>1",
		"1>>22229>>>22>>>1",
		"1>>2>2>9>>>>2>>>1",
		"1>:>>:>;>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 42
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>8>21<21<2>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>88111888>>1",
		"1>>>99211<112>>>1",
		"1>>>>9211<112>>>1",
		"1>>>>22211122>>>1",
		"1>>>222222222>>>1",
		"1>>22229>>>22>>>1",
		"1>>2>2>9>>>>2>>>1",
		"1>:>>:>;>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 43
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>8>21<21<2>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>88111888>>1",
		"1>>>99211<112>>>1",
		"1>>>>9211<112>>>1",
		"1>>>>22211122>>>1",
		"1>>>222222222>>>1",
		"1>>22229>>>22>>>1",
		"1>>2>2>9>>>>2>>>1",
		"1>:>>:>;>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 44
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>8>2132132>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>88111888>>1",
		"1>>>992111112>>>1",
		"1>>>>92111112>>>1",
		"1>>>>22211122>>>1",
		"1>>>222222222>>>1",
		"1>>22229>>>22>>>1",
		"1>>2>2>9>>>>2>>>1",
		"1>:>>:>;>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 45
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>8>2132132>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>88111888>>1",
		"1>>>992111112>>>1",
		"1>>>>92111112>>>1",
		"1>>>>22211122>>>1",
		"1>>>222222222>>>1",
		"1>>22229>>>22>>>1",
		"1>>2>2>9>>>>2>>>1",
		"1>:>>:>;>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 46
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>8>2132132>8>1",
		"1>>>88226666688>1",
		"1>>>>882616168>>1",
		"1>>>>>82666668>>1",
		"1>>>9>88111888>>1",
		"1>>>992111112>>>1",
		"1>>>>92111112>>>1",
		"1>>>>22211122>>>1",
		"1>>>222222222>>>1",
		"1>>22229>>>22>>>1",
		"1>>2>2>9>>>>2>>>1",
		"1>:>>:>;>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 47
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>>>2132132>>>1",
		"1>>>>82222222>8>1",
		"1>>>>8826666688>1",
		"1>>>>>82616168>>1",
		"1>>>9>88666668>>1",
		"1>>>9928811888>>1",
		"1>>>>92211112>>>1",
		"1>>>>22221122>>>1",
		"1>>>222222222>>>1",
		"1>>22222>>>22>>>1",
		"1>>2>>29>>>>2>>>1",
		"1>>:>>:;>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 48
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>>>2132132>>>1",
		"1>>>>82222222>8>1",
		"1>>>>8826666688>1",
		"1>>>>>82616168>>1",
		"1>>>9>88666668>>1",
		"1>>>9928811888>>1",
		"1>>>>92211112>>>1",
		"1>>>>22221122>>>1",
		"1>>>222222222>>>1",
		"1>>22222>>>22>>>1",
		"1>>2>>29>>>>2>>>1",
		"1>>:>>:;>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 49
	{
		"11111111111111111",
		"1>>>99>>>>>>99>>1",
		"1>>>>922222229>>1",
		"1>>>>921121129>>1",
		"1>>>>>2132132>>>1",
		"1>>>>82222222>8>1",
		"1>>>>8826666688>1",
		"1>>>>>82616168>>1",
		"1>>>9>88666668>>1",
		"1>>>9928811888>>1",
		"1>>>>92221122>>>1",
		"1>>>>22222222>>>1",
		"1>>>>22222222>>>1",
		"1>>>22222>>22>>>1",
		"1>>>2>>2>>>>2>>>1",
		"1>>>:>>:>>>>:>>>1",
		"11111111111111111",
	},

	//Frame 50
	{
		"11111111111111111",
		"1>>>>99>>>>>>99>1",
		"1>>>>>922222229>1",
		"1>>>>>921121129>1",
		"1>>>>>>2312312>>1",
		"1>>>>>82222222>81",
		"1>>>>>88266666881",
		"1>>>>>>82616168>1",
		"1>>>>>>88666668>1",
		"1>>>9>>28811888>1",
		"1>>>99>2211122>>1",
		"1>>>>222222222>>1",
		"1>>>>222222222>>1",
		"1>>>222222>>22>>1",
		"1>>>2>>92>>>>2>>1",
		"1>>>:>>;:>>>>:>>1",
		"11111111111111111",
	},

	//Frame 51
	{
		"11111111111111111",
		"1>>>>>9>>>>>>>9>1",
		"1>>>>>922222229>1",
		"1>>>>>921121129>1",
		"1>>>>>>2312312>>1",
		"1>>>>8>2222222>81",
		"1>>>>882666662881",
		"1>>>>>886161688>1",
		"1>>>>>>8666668>>1",
		"1>>9>>>8811188>>1",
		"1>>99>>2211122>>1",
		"1>>>2222222222>>1",
		"1>>>2222222222>>1",
		"1>>2222222>>222>1",
		"1>>2>>9>2>>>>22>1",
		"1>>:>>;>:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 52
	{
		"11111111111111111",
		"1>>>>>9>>>>>>>9>1",
		"1>>>>>922222229>1",
		"1>>>>>921121129>1",
		"1>>>>>>2312312>>1",
		"1>>>>8>2222222>81",
		"1>>>>882666662881",
		"1>>>>>886161688>1",
		"1>>>>>>8666668>>1",
		"1>>9>>>8811188>>1",
		"1>>99>>2211122>>1",
		"1>>>2222222222>>1",
		"1>>>2222222222>>1",
		"1>>2222922>>222>1",
		"1>>2>>>92>>>>22>1",
		"1>>:>>>;:>>>>>:>1",
		"11111111111111111",
	},

	//Frame 53
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2312312>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>>>>8666668>1",
		"1>>9>>>>8811188>1",
		"1>>999>>2211122>1",
		"1>>>22222222222>1",
		"1>>>22222222222>1",
		"1>>22222922>>22>1",
		"1>>2>>>>92>>>>2>1",
		"1>>:>>>>;:>>>>:>1",
		"11111111111111111",
	},

	//Frame 54
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2312312>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>>>>8666668>1",
		"1>>9>>>>8811188>1",
		"1>>999>>2211122>1",
		"1>>>22222222222>1",
		"1>>>22222222222>1",
		"1>>22222922>>22>1",
		"1>>2>>>>92>>>>2>1",
		"1>>:>>>>;:>>>>:>1",
		"11111111111111111",
	},

	//Frame 55
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2312312>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>>>>8666668>1",
		"1>>9>>>>8811188>1",
		"1>>999>>2211122>1",
		"1>>>22222222222>1",
		"1>>>22222222222>1",
		"1>>22229922>>22>1",
		"1>>2>>>>92>>>>2>1",
		"1>>:>>>>;:>>>>:>1",
		"11111111111111111",
	},

	//Frame 56
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2312312>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>>>>8666668>1",
		"1>>9>>>>8811188>1",
		"1>>992222111112>1",
		"1>>>22222211122>1",
		"1>>>22222222222>1",
		"1>>22299>22>>22>1",
		"1>>2>>>9>2>>>>2>1",
		"1>:>>>>;>:>>>>:>1",
		"11111111111111111",
	},

	//Frame 57
	{
		"11111111111111111",
		"1>>>>>>9>>>>>>>91",
		"1>>>>>>9222222291",
		"1>>>>>>9211211291",
		"1>>>>>>>2312312>1",
		"1>>>>>8>2222222>1",
		"1>>>>>88266666281",
		"1>>>>>>8861616881",
		"1>>>>>>>8666668>1",
		"1>>9>>>>8811188>1",
		"1>>992222111112>1",
		"1>>>22222211122>1",
		"1>>>22222222222>1",
		"1>>22299>22>>22>1",
		"1>>2>>>9>2>>>>2>1",
		"1>:>>>>;>:>>>>:>1",
		"11111111111111111",
	},

	//Frame 58
	{
		"11111111111111111",
		"1>>>>>>>9>>>>>>>1",
		"1>>>>>>>922222221",
		"1>>>>>>>921121121",
		"1>>>>>>>>23123121",
		"1>>>>>>8>22222221",
		"1>>>>>>8826666621",
		"1>>>>>>>886161681",
		"1>>>>>>>>86666681",
		"1>>>9>>>>88111881",
		"1>>>9922221111121",
		"1>>>>222222111221",
		"1>>>2222222222221",
		"1>>>22299>22>>221",
		"1>>>2>>>9>2>>>>21",
		"1>>:>>>>;>:>>>>:1",
		"11111111111111111",
	},

	//Frame 59
	{
		"11111111111111111",
		"1>>>>>>>9>>>>>>>1",
		"1>>>>>>>922222221",
		"1>>>>>>>921121121",
		"1>>>>>>>>23123121",
		"1>>>>>>8>22222221",
		"1>>>>>>8826666621",
		"1>>>>>>>886161681",
		"1>>9>>>>>86666681",
		"1>>99>22288111881",
		"1>>>>222221111121",
		"1>>>2222222111221",
		"1>>>2222222222221",
		"1>>>2299>22>>>221",
		"1>>>2>>9>2>>>>>21",
		"1>>:>>>;>:>>>>>:1",
		"11111111111111111",
	},

	//Frame 60
	{
		"11111111111111111",
		"1>>>>>>>9>>>>>>>1",
		"1>>>>>>>922222221",
		"1>>>>>>>921121121",
		"1>>>>>>>>23123121",
		"1>>>>>>8>22222221",
		"1>>>>>>8826666621",
		"1>>>>>>>886161681",
		"1>>9>>>>>86666681",
		"1>>99>22288111881",
		"1>>>>222221111121",
		"1>>>2222222111221",
		"1>>>2222222222221",
		"1>>>2299>22>>>221",
		"1>>>2>>9>2>>>>>21",
		"1>>:>>>;>:>>>>>:1",
		"11111111111111111",
	},

};
