// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2023  jgabaut

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
#include "assassin_poof.h"

char assassin_poof[61][MAXROWS][MAXCOLS] = {

	//Frame 1
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<1818<?11111",
		"1111?<6666<<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 2
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<1818<?11111",
		"1111?<6666<<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 3
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<1818<?11111",
		"1111?<6666<<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 4
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<8181<?11111",
		"1111?<6666<<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 5
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<1818<?11111",
		"1111?<6666<<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 6
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<1818<?11111",
		"1111?<6666<<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 7
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<1818<?11111",
		"1<<1?<6666<<?1<<1",
		"1<<11<<<<<<<11<<1",
		"11<<11<<<<<11<<11",
		"11<<11>888>11<<11",
		"11<<>>><<<>>><<11",
		"11<<<<<<<<<<<<<11",
		"111<<<<<<<<<<<111",
		"1111><<<<<<<>1111",
		"11111111111111111",
	},

	//Frame 8
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<1818<?11111",
		"1<<1?<6666<<?1<<1",
		"1<<11<<<<<<<11<<1",
		"11<<11<<<<<11<<11",
		"11<<11>888>11<<11",
		"11<<>>><<<>>><<11",
		"11<<<<<<<<<<<<<11",
		"111<<<<<<<<<<<111",
		"1111><<<<<<<>1111",
		"11111111111111111",
	},

	//Frame 9
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<1818<?11111",
		"1<<1?<6666<<?1<<1",
		"1<<11<<<<<<<11<<1",
		"11<<11<<<<<11<<11",
		"11<<11>888>11<<11",
		"11<<>>><<<>>><<11",
		"11<<<<<<<<<<<<<11",
		"111<<<<<<<<<<<111",
		"1111><<<<<<<>1111",
		"11111111111111111",
	},

	//Frame 10
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<1818<?11111",
		"1<<1?<6666<<?1<<1",
		"1<<11<<<<<<<11<<1",
		"11<<11<<<<<11<<11",
		"11<<11>888>11<<11",
		"11<<>>><<<>>><<11",
		"11<<<<<<<<<<<<<11",
		"111<<<<<<<<<<<111",
		"1111><<<<<<<>1111",
		"11111111111111111",
	},

	//Frame 11
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<1818<?11111",
		"1111?<6666<<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 12
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<1818<?11111",
		"1111?<6666<<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 13
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>11111111",
		"111111>>11>>11111",
		"111111<<1<<111111",
		"1111411616<141111",
		"11111<1818<111111",
		"1111?<1616<1?1111",
		"11111<11<<<111111",
		"11111111<<1111111",
		"11111111881111111",
		"11111>><111>>1111",
		"11111<<<11<<>1111",
		"1111<<11<<1<<1111",
		"111>11><<<>11>111",
		"11111111111111111",
	},

	//Frame 14
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>11111111",
		"111111>>11>>11111",
		"111111<<1<<111111",
		"1111411616<141111",
		"11111<1818<111111",
		"1111?<1616<1?1111",
		"11111<11<<<111111",
		"11111111<<1111111",
		"11111111881111111",
		"11111>><111>>1111",
		"11111<<<11<<>1111",
		"1111<<11<<1<<1111",
		"111>11><<<>11>111",
		"11111111111111111",
	},

	//Frame 15
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>11111111",
		"111111>>11>>11111",
		"111111<<1<<111111",
		"1111411616<141111",
		"11111<1818<111111",
		"1111?<1616<1?1111",
		"11111<11<<<111111",
		"11111111<<1111111",
		"11111111881111111",
		"11111>><111>>1111",
		"11111<<<11<<>1111",
		"1111<<11<<1<<1111",
		"111>11><<<>11>111",
		"11111111111111111",
	},

	//Frame 16
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"1111111>>11111111",
		"1111111111>>11111",
		"111111<11<<111111",
		"1111111116<141111",
		"1111111118<111111",
		"1111?<111611?1111",
		"11111<11111111111",
		"11111111111111111",
		"11111111111111111",
		"11111>>11111>1111",
		"11111<111111>1111",
		"1111<111<<1111111",
		"111>11><<<>11>111",
		"11111111111111111",
	},

	//Frame 17
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"1111111>>11111111",
		"1111111111>>11111",
		"111111<11<<111111",
		"1111111116<141111",
		"1111111118<111111",
		"1111?<111611?1111",
		"11111<11111111111",
		"11111111111111111",
		"11111111111111111",
		"11111>>11111>1111",
		"11111<111111>1111",
		"1111<111<<1111111",
		"111>11><<<>11>111",
		"11111111111111111",
	},

	//Frame 18
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"1111111>>11111111",
		"1111111111>>11111",
		"111111<11<<111111",
		"1111111116<141111",
		"1111111118<111111",
		"1111?<111611?1111",
		"11111<11111111111",
		"11111111111111111",
		"11111111111111111",
		"11111>>11111>1111",
		"11111<111111>1111",
		"1111<111<<1111111",
		"111>11><<<>11>111",
		"11111111111111111",
	},

	//Frame 19
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111>11111",
		"111111111<1111111",
		"11111111161111111",
		"1111111111<111111",
		"11111<111111?1111",
		"11111<11111111111",
		"11111111111111111",
		"11111111111111111",
		"111111>11111>1111",
		"111111111111>1111",
		"11111111111111111",
		"1111111<11>11>111",
		"11111111111111111",
	},

	//Frame 20
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111>11111",
		"111111111<1111111",
		"11111111161111111",
		"1111111111<111111",
		"11111<111111?1111",
		"11111<11111111111",
		"11111111111111111",
		"11111111111111111",
		"111111>11111>1111",
		"111111111111>1111",
		"11111111111111111",
		"1111111<11>11>111",
		"11111111111111111",
	},

	//Frame 21
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111>11111",
		"111111111<1111111",
		"11111111161111111",
		"1111111111<111111",
		"11111<111111?1111",
		"11111<11111111111",
		"11111111111111111",
		"11111111111111111",
		"111111>11111>1111",
		"111111111111>1111",
		"11111111111111111",
		"1111111<11>11>111",
		"11111111111111111",
	},

	//Frame 22
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111>11111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111<11111111111",
		"11111<11111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"1111111111111>111",
		"11111111111111111",
	},

	//Frame 23
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111>11111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111<11111111111",
		"11111<11111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"1111111111111>111",
		"11111111111111111",
	},

	//Frame 24
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111>11111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111<11111111111",
		"11111<11111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"1111111111111>111",
		"11111111111111111",
	},

	//Frame 25
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
	},

	//Frame 26
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
	},

	//Frame 27
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
	},

	//Frame 28
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
	},

	//Frame 29
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
	},

	//Frame 30
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111>11111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111<11111",
		"11111111111<11111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"111>1111111111111",
		"11111111111111111",
	},

	//Frame 31
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111>11111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111<11111",
		"11111111111<11111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"111>1111111111111",
		"11111111111111111",
	},

	//Frame 32
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111>11111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111<11111",
		"11111111111<11111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"111>1111111111111",
		"11111111111111111",
	},

	//Frame 33
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111>11111111111",
		"1111111<111111111",
		"11111116111111111",
		"111111<1111111111",
		"1111?111111<11111",
		"11111111111<11111",
		"11111111111111111",
		"11111111111111111",
		"1111>11111>111111",
		"1111>111111111111",
		"11111111111111111",
		"111>11>11<1111111",
		"11111111111111111",
	},

	//Frame 34
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111>11111111111",
		"1111111<111111111",
		"11111116111111111",
		"111111<1111111111",
		"1111?111111<11111",
		"11111111111<11111",
		"11111111111111111",
		"11111111111111111",
		"1111>11111>111111",
		"1111>111111111111",
		"11111111111111111",
		"111>11>11<1111111",
		"11111111111111111",
	},

	//Frame 35
	{
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111111111111111",
		"11111>11111111111",
		"1111111<111111111",
		"11111116111111111",
		"111111<1111111111",
		"1111?111111<11111",
		"11111111111<11111",
		"11111111111111111",
		"11111111111111111",
		"1111>11111>111111",
		"1111>111111111111",
		"11111111111111111",
		"111>11>11<1111111",
		"11111111111111111",
	},

	//Frame 36
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"11111111>>1111111",
		"11111>>1111111111",
		"111111<<11<111111",
		"111141<6111111111",
		"111111<8111111111",
		"1111?116111<?1111",
		"11111111111<11111",
		"11111111111111111",
		"11111111111111111",
		"1111>11111>>11111",
		"1111>111111<11111",
		"1111111<<111<1111",
		"111>11><<<>11>111",
		"11111111111111111",
	},

	//Frame 37
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"11111111>>1111111",
		"11111>>1111111111",
		"111111<<11<111111",
		"111141<6111111111",
		"111111<8111111111",
		"1111?116111<?1111",
		"11111111111<11111",
		"11111111111111111",
		"11111111111111111",
		"1111>11111>>11111",
		"1111>111111<11111",
		"1111111<<111<1111",
		"111>11><<<>11>111",
		"11111111111111111",
	},

	//Frame 38
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"11111111>>1111111",
		"11111>>1111111111",
		"111111<<11<111111",
		"111141<6111111111",
		"111111<8111111111",
		"1111?116111<?1111",
		"11111111111<11111",
		"11111111111111111",
		"11111111111111111",
		"1111>11111>>11111",
		"1111>111111<11111",
		"1111111<<111<1111",
		"111>11><<<>11>111",
		"11111111111111111",
	},

	//Frame 39
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"11111111>>>111111",
		"11111>>11>>111111",
		"111111<<1<<111111",
		"111141<6161141111",
		"111111<8181<11111",
		"1111?1<6161<?1111",
		"111111<<<11<11111",
		"1111111<<11111111",
		"11111118811111111",
		"1111>>111<>>11111",
		"1111><<11<<<11111",
		"1111<<1<<11<<1111",
		"111>11><<<>11>111",
		"11111111111111111",
	},

	//Frame 40
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"11111111>>>111111",
		"11111>>11>>111111",
		"111111<<1<<111111",
		"111141<6161141111",
		"111111<8181<11111",
		"1111?1<6161<?1111",
		"111111<<<11<11111",
		"1111111<<11111111",
		"11111118811111111",
		"1111>>111<>>11111",
		"1111><<11<<<11111",
		"1111<<1<<11<<1111",
		"111>11><<<>11>111",
		"11111111111111111",
	},

	//Frame 41
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"11111111>>>111111",
		"11111>>11>>111111",
		"111111<<1<<111111",
		"111141<6161141111",
		"111111<8181<11111",
		"1111?1<6161<?1111",
		"111111<<<11<11111",
		"1111111<<11111111",
		"11111118811111111",
		"1111>>111<>>11111",
		"1111><<11<<<11111",
		"1111<<1<<11<<1111",
		"111>11><<<>11>111",
		"11111111111111111",
	},

	//Frame 42
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<8181<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 43
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<8181<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 44
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<8181<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 45
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<6666<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 46
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<6666<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 47
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<6666<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 48
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<8181<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 49
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<8181<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 50
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<1818<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 51
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<1818<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 52
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<1818<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 53
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<1818<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 54
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<1818<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 55
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<6666<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 56
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<6666<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 57
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<8181<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 58
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"111114<<<<<<11111",
		"111144<6666<41111",
		"11111?<8181<11111",
		"1111?<<6666<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 59
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<1818<?11111",
		"1111?<6666<<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

	//Frame 60
	{
		"11111111111111111",
		"11111111111111111",
		"11111111411111111",
		"111111>>>>>111111",
		"11111>>>>>>>11111",
		"11111<<<<<<411111",
		"11114<6666<441111",
		"11111<1818<?11111",
		"1111?<6666<<?1111",
		"11111<<<<<<<11111",
		"111111<<<<<111111",
		"111111>888>111111",
		"1111>>><<<>>>1111",
		"1111><<<<<<<>1111",
		"1111<<<<<<<<<1111",
		"111><<><<<><<>111",
		"11111111111111111",
	},

};
