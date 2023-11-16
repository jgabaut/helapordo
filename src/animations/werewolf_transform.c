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
#include "werewolf_transform.h"

char werewolf_transform[61][MAXROWS][MAXCOLS] = {

	//Frame 1
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11111111111111111",
	},

	//Frame 2
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11555555555555551",
		"11155555555555551",
		"11111111111111111",
	},

	//Frame 3
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11155555555555551",
		"1;115555555555551",
		"1;;11555555555551",
		"1;;;1555555555551",
		"16;11555555555551",
		"11111111111111111",
	},

	//Frame 4
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11111555555555551",
		"1;;;1155555555551",
		"1;;;;115555555551",
		"1;;;;;15555555551",
		"1886;115555555551",
		"18161155555555551",
		"16661555555555551",
		"16661555555555551",
		"11111111111111111",
	},

	//Frame 5
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11111115555555551",
		"1;;;;;11555555551",
		"1;;;;;;1155555551",
		"1;;;;;;;155555551",
		"186886;1155555551",
		"11681611555555551",
		"16666615555555551",
		"11166615555555551",
		"16666111555555551",
		"116111=1155555551",
		"11111111111111111",
	},

	//Frame 6
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"11111111155555551",
		"1;;;;;;;115555551",
		"1;;;;;;;;11555551",
		"1;;;;;;;;;1555551",
		"16886886;11555551",
		"16816816115555551",
		"16666666155555551",
		"16611666155555551",
		"11666661115555551",
		"11116111=11555551",
		"1==111====1155551",
		"1==========155551",
		"1==4====1==155551",
		"11111111111111111",
	},

	//Frame 7
	{
		"11111111111111111",
		"15555555555555551",
		"11111111115555551",
		"11;;;;;;;11555551",
		"1;;;;;;;;;1155551",
		"1;;;;;;;;;;155551",
		"1;6886886;1155551",
		"1;681681611555551",
		"11666666615555551",
		"11661166615555551",
		"11166666111555551",
		"1=1116111=1155551",
		"1===111====115551",
		"1===========15551",
		"1===4====1==15551",
		"11======11==15551",
		"11111111111111111",
	},

	//Frame 8
	{
		"11111111111111111",
		"15555555555555551",
		"15111111111555551",
		"111;;;;;;;1155551",
		"11;;;;;;;;;115551",
		"11;;;;;;;;;;15551",
		"1;;6886886;115551",
		"1;;68168161155551",
		"1;166666661555551",
		"11166116661555551",
		"11116666611155551",
		"1==1116111=115551",
		"1====111====11551",
		"1============1551",
		"11===4====1==1551",
		"111======11==1551",
		"11111111111111111",
	},

	//Frame 9
	{
		"11111111111111111",
		"15111111111555551",
		"111;;;;;;;1155551",
		"11;;;;;;;;;115551",
		"11;;;;;;;;;;15551",
		"1;;6886886;115551",
		"1;;68168161155551",
		"1;166666661555551",
		"11166116661555551",
		"11116666611155551",
		"1==1116111=115551",
		"1====111====11551",
		"1============1551",
		"11===4====1==1551",
		"111======11==1551",
		"111==4===11==1551",
		"11111111111111111",
	},

	//Frame 10
	{
		"11111111111111111",
		"15511111111155551",
		"1511;;;;;;;115551",
		"151;;;;;;;;;11551",
		"111;;;;;;;;;;1551",
		"11;;6886886;11551",
		"11;;6816816115551",
		"11;16666666155551",
		"11116611666155551",
		"11111666661115551",
		"11==1116111=11551",
		"1=====111====1151",
		"1=============151",
		"1=1===4====1==151",
		"1=11======11==151",
		"1=11==4===11==151",
		"11111111111111111",
	},

	//Frame 11
	{
		"11111111111111111",
		"15511111111155551",
		"1511;;;;;;;115551",
		"151;;;;;;;;;11551",
		"111;;;;;;;;;;1551",
		"11;;6886886;11551",
		"11;;6816816115551",
		"11;16666666155551",
		"11116611166155551",
		"11111611661115551",
		"11==1666111=11551",
		"1====1111====1151",
		"1=============151",
		"1=1===4====1==151",
		"1=11======11==151",
		"1=11==4===11==151",
		"11111111111111111",
	},

	//Frame 12
	{
		"11111111111111111",
		"15555555555555551",
		"15511111111155551",
		"1511;;;;;;;115551",
		"151;;;;;;;;;11551",
		"111;;;;;;;;;;1551",
		"11;;6886886;11551",
		"11;;6816816115551",
		"11;16666666155551",
		"11116611166155551",
		"11111611661115551",
		"11==1666111=11551",
		"1====1111====1151",
		"1=============151",
		"1=1===4====1==151",
		"1=11======11==151",
		"11111111111111111",
	},

	//Frame 13
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15511111111155551",
		"1511;;;;;;;115551",
		"151;;;;;;;;;11551",
		"111;;;;;;;;;;1551",
		"11;;6886886;11551",
		"11;;6816816115551",
		"11;16666666155551",
		"11116611166155551",
		"11111611661115551",
		"11==1666111=11551",
		"1====1111====1151",
		"1=============151",
		"1=1===4====1==151",
		"11111111111111111",
	},

	//Frame 14
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15111111111555551",
		"111;;;;;;;1155551",
		"11;;;;;;;;;115551",
		"11;;;;;;;;;;15551",
		"1;;6886886;115551",
		"1;;68168161155551",
		"1;166666661555551",
		"11166111661555551",
		"11116116611155551",
		"1==1666111=115551",
		"1===1111====11551",
		"1============1551",
		"11===4====1==1551",
		"11111111111111111",
	},

	//Frame 15
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15111111111555551",
		"111;;;;;;;1155551",
		"11;;;;;;;;;115551",
		"11;;;;;;;;;;15551",
		"1;;6816816;115551",
		"1;;68168161155551",
		"1;166666661555551",
		"11166111661555551",
		"11116116611155551",
		"1==1666111=115551",
		"1===1111====11551",
		"1============1551",
		"11111111111111111",
	},

	//Frame 16
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15111111111555551",
		"111;;;;;;;1155551",
		"11;;;;;;;;;115551",
		"11;;;;;;;;;;15551",
		"1;;6816816;115551",
		"1;;68868861155551",
		"1;166666661555551",
		"11166111661555551",
		"11116116611155551",
		"1==1666111=115551",
		"1===1111====11551",
		"11111111111111111",
	},

	//Frame 17
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11111111115555551",
		"11;;;;;;;11555551",
		"1;;;;;;;;;1155551",
		"1;;;;;;;;;;155551",
		"1;6816816;1155551",
		"1;688688611555551",
		"11666666615555551",
		"11661116615555551",
		"11161166111555551",
		"1=1666111=1155551",
		"11111111111111111",
	},

	//Frame 18
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11111111115555551",
		"11;;>;>;>11555551",
		"1>;>;>;>;;1155551",
		"1>;;;;;;;>;155551",
		"1;6846846;1155551",
		"1>688688611555551",
		"11>66>66>15555551",
		"11661116615555551",
		"111>116>111555551",
		"1=1666111=1155551",
		"11111111111111111",
	},

	//Frame 19
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11111111115555551",
		"11;;>;>;>11555551",
		"1>;>;>;>;;1155551",
		"1>;;;;;;;>;155551",
		"1;6846846;1155551",
		"1>688688611555551",
		"11>66>66>15555551",
		"11661116615555551",
		"111>116>111555551",
		"1=1666111=1155551",
		"11111111111111111",
	},

	//Frame 20
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11111111115555551",
		"11>;>;;;>11555551",
		"1>;;;;>;>;1155551",
		"1>>;>;;>;>;155551",
		"1;6846846;1155551",
		"1>688688611555551",
		"11>>6>6>>15555551",
		"116611>6615555551",
		"111>6>6>111555551",
		"11111111111111111",
	},

	//Frame 21
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11111111115555551",
		"11>;>;;;>11555551",
		"1>;;;;>;>;1155551",
		"1>>;>;;>;>;155551",
		"1;6846846;1155551",
		"1>688688611555551",
		"11>>6>6>>15555551",
		"116611>6615555551",
		"111>6>6>111555551",
		"11111111111111111",
	},

	//Frame 22
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11111115555555551",
		"1>;>;;>1555555551",
		"1;>;;>;>155555551",
		"1>;>>>>;115555551",
		"16>4>>4>;15555551",
		"16886886115555551",
		"1>>6>6>>155555551",
		"1>611>6>155555551",
		"11111111111111111",
	},

	//Frame 23
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15111555555555551",
		"11>>1155555555551",
		"1>;;>115555555551",
		"1;>>>>11555555551",
		"1>4>>4>1555555551",
		"1>8>>861555555551",
		"1>6>6>>1555555551",
		"11111111111111111",
	},

	//Frame 24
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15111555555555551",
		"11>>1155555555551",
		"1>;;>115555555551",
		"1;>>>>11555555551",
		"1>4>>4>1555555551",
		"1>8>>861555555551",
		"1>6>6>>1555555551",
		"11111111111111111",
	},

	//Frame 25
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11115555555555551",
		"1>>11555555555551",
		"1>;>1155555555551",
		"1>>>>115555555551",
		"1\?>>\?>15555555551",
		"1\?>>\?>15555555551",
		"11111111111111111",
	},

	//Frame 26
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11555555555555551",
		"11155555555555551",
		"1>115555555555551",
		"1>>11555555555551",
		"11111111111111111",
	},

	//Frame 27
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11555555555555551",
		"11155555555555551",
		"1>115555555555551",
		"1>>11555555555551",
		"11111111111111111",
	},

	//Frame 28
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11111111111111111",
	},

	//Frame 29
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11111111111111111",
	},

	//Frame 30
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15551111111555551",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"11111111111111111",
	},

	//Frame 31
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15551111111555551",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"11111111111111111",
	},

	//Frame 32
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15551111111555551",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>4>4>>155551",
		"151>>>4>4>>111551",
		"11111111111111111",
	},

	//Frame 33
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15551111111555551",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>4>4>>155551",
		"151>>>4>4>>111551",
		"151>1>>>>>>>>1551",
		"11111111111111111",
	},

	//Frame 34
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15551111111555551",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>4>4>>155551",
		"151>>>4>4>>111551",
		"151>1>>>>>>>>1551",
		"11111111111111111",
	},

	//Frame 35
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15551111111555551",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>4>4>>155551",
		"151>>>4>4>>111551",
		"151>1>>>>>>>>1551",
		"151>18>>>>66>1551",
		"151>118181>>>1551",
		"11111111111111111",
	},

	//Frame 36
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15551111111555551",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>4>4>>155551",
		"151>>>4>4>>111551",
		"151>1>>>>>>>>1551",
		"151>18>>>>66>1551",
		"151>118181>>>1551",
		"151>>81111>>11551",
		"1511>>181>>115551",
		"11111111111111111",
	},

	//Frame 37
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15551111111555551",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>4>4>>155551",
		"151>>>4>4>>111551",
		"151>1>>>>>>>>1551",
		"151>18>>>>66>1551",
		"151>118181>>>1551",
		"151>>81111>>11551",
		"1511>>181>>115551",
		"111>>>>>>>1115551",
		"11>>>>>>>>>>11111",
		"11111111111111111",
	},

	//Frame 38
	{
		"11111111111111111",
		"15555555555555551",
		"15551111111555551",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>4>4>>155551",
		"151>>>4>4>>111551",
		"151>1>>>>>>>>1551",
		"151>18>>>>66>1551",
		"151>118181>>>1551",
		"151>>81111>>11551",
		"1511>>181>>115551",
		"111>>>>>>>1115551",
		"11>>>>>>>>>>11111",
		"11>>>>000>>>>>>11",
		"11>>>>>>>0>>>>>11",
		"11111111111111111",
	},

	//Frame 39
	{
		"11111111111111111",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>4>4>>155551",
		"151>>>4>4>>111551",
		"151>1>>>>>>>>1551",
		"151>18>>>>66>1551",
		"151>118181>>>1551",
		"151>>81111>>11551",
		"1511>>181>>115551",
		"111>>>>>>>1115551",
		"11>>>>>>>>>>11111",
		"11>>>>000>>>>>>11",
		"11>>>>>>>0>>>>>11",
		"111>>>>>>00011111",
		"1510000000>155551",
		"11111111111111111",
	},

	//Frame 40
	{
		"11111111111111111",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>\?>\?>>155551",
		"151>>>4>4>>111551",
		"151>>>>>>>>>>1551",
		"151>18>>>>66>1551",
		"151>118181>>>1551",
		"151>111111>>11551",
		"1511>81111>115551",
		"111>>>181>>115551",
		"11>>>>>>>>>>11111",
		"11>>>>000>>>>>>11",
		"11>>>>>>>0>>>>>11",
		"111>>>>>>00011111",
		"1510000000>155551",
		"11111111111111111",
	},

	//Frame 41
	{
		"11111111111111111",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>\?>\?>>155551",
		"151>>>4>4>>111551",
		"151>>>>>>>>>>1551",
		"151>18>>>>66>1551",
		"151>118181>>>1551",
		"151>111111>>11551",
		"1511>81111>115551",
		"151>>>181>>115551",
		"151>>>>>>>>>11111",
		"151>>>>0000>>>>>1",
		"151>>>>>>>0>>>>>1",
		"1511>>>>>>0011111",
		"1551000000>155551",
		"11111111111111111",
	},

	//Frame 42
	{
		"11111111111111111",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>\?>\?>>155551",
		"151>>>4>4>>111551",
		"151>>>>>>>>>>1551",
		"151>18>>>>66>1551",
		"151>118181>>>1551",
		"151>111111>>11551",
		"1511>81111>115551",
		"111>>>181>>115551",
		"11>>>>>>>>>>11111",
		"11>>>>000>>>>>>11",
		"11>>>>>>>0>>>>>11",
		"111>>>>>>00011111",
		"1510000000>155551",
		"11111111111111111",
	},

	//Frame 43
	{
		"11111111111111111",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>4>4>>155551",
		"151>>>4>4>>111551",
		"151>1>>>>>>>>1551",
		"151>18>>>>66>1551",
		"151>118181>>>1551",
		"151>>81111>>11551",
		"1511>>181>>115551",
		"111>>>>>>>1115551",
		"11>>>>>>>>>>11111",
		"11>>>>000>>>>>>11",
		"11>>>>>>>0>>>>>11",
		"111>>>>>>00011111",
		"1510000000>155551",
		"11111111111111111",
	},

	//Frame 44
	{
		"11111111111111111",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>\?>\?>>155551",
		"151>>>4>4>>111551",
		"151>>>>>>>>>>1551",
		"151>18>>>>66>1551",
		"151>118181>>>1551",
		"151>111111>>11551",
		"1511>81111>115551",
		"151>>>181>>115551",
		"151>>>>>>>>>11111",
		"151>>>>0000>>>>>1",
		"151>>>>>>>0>>>>>1",
		"1511>>>>>>0011111",
		"1551000000>155551",
		"11111111111111111",
	},

	//Frame 45
	{
		"11111111111111111",
		"15551111111555551",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>\?>\?>>155551",
		"151>>>4>4>>111551",
		"151>>>>>>>>>>1551",
		"151>18>>>>66>1551",
		"151>118181>>>1551",
		"151>111111>>11551",
		"1511>81111>115551",
		"151>>>181>>115551",
		"151>>>>>>>>>11111",
		"151>>>>0000>>>>>1",
		"151>>>>>>>0>>>>>1",
		"1511>>>>>>1111111",
		"11111111111111111",
	},

	//Frame 46
	{
		"11111111111111111",
		"15551111111555551",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>\?>\?>>155551",
		"151>>>4>4>>111551",
		"151>1>>>>>>>>1551",
		"151>18>>>>66>1551",
		"151>118181>>>1551",
		"151>181111>>11551",
		"1511>>181>>115551",
		"151>>>>>>>1115551",
		"151>>>>>>>>>11111",
		"151>>>>0000>>>>>1",
		"151>>>>>>>0>>>>>1",
		"1511>>>>>>1111111",
		"11111111111111111",
	},

	//Frame 47
	{
		"11111111111111111",
		"15555555555555551",
		"15551111111555551",
		"15551>>1>>1555551",
		"15511>>>>>1155551",
		"1511>>\?>\?>>155551",
		"151>>>\?>\?>>111551",
		"151>1>>>>>>>>1551",
		"151>18>>>>66>1551",
		"151>118181>>>1551",
		"151>181111>>11551",
		"1511>>181>>115551",
		"151>>>>>>>1115551",
		"151>>>>0000>11111",
		"151>>>>>>>0>>>>>1",
		"151>>>>>>>0>>>>>1",
		"11111111111111111",
	},

	//Frame 48
	{
		"11111111111111111",
		"15555555555555551",
		"15555111111155551",
		"155551>>1>>155551",
		"155511>>>>>115551",
		"15511>>\?>\?>>15551",
		"1551>>>\?>\?>>11151",
		"1551>1>>>>>>>>151",
		"1551>18>>>>66>151",
		"1551>118181>>>151",
		"1551>181111>>1151",
		"15511>>181>>11551",
		"1551>>>>>>>111551",
		"151>>>>0000>>1111",
		"151>>>>>>>0>>>>>1",
		"151>>>>>>>0>>>>>1",
		"11111111111111111",
	},

	//Frame 49
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555511111115551",
		"1555551>>1>>15551",
		"1555511>>>>>11551",
		"155511>>\?>\?>>1551",
		"15551>>>\?>\?>>1111",
		"15551>1>>>>>>>>11",
		"15551>18>>>>66>11",
		"15551>118181>>>11",
		"15551>181111>>111",
		"155511>>181>>1151",
		"15551>>>>>>>11151",
		"1551>>>>0000>>111",
		"1551>>>>>>>0>>>>1",
		"11111111111111111",
	},

	//Frame 50
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555551111111551",
		"15555551>>1>>1551",
		"15555511>>>>>1151",
		"1555511>>\?>\?>>151",
		"155551>>>\?>\?>>111",
		"155551>1>>>>>>>>1",
		"155551>18>>>>66>1",
		"155551>118181>>>1",
		"155551>181111>>11",
		"1555511>>181>>111",
		"155551>>>>>>>1111",
		"15551>>>0000>>>11",
		"11111111111111111",
	},

	//Frame 51
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555111111151",
		"155555551>>1>>151",
		"155555511>>>>>111",
		"15555511>>\?>\?>>11",
		"1555551>>>\?>\?>>11",
		"1555551>1>>>>>>>1",
		"1555551>18>>>>661",
		"1555551>118181>>1",
		"1555551>181111>>1",
		"15555511>>181>>11",
		"1555551>>>>>>>111",
		"11111111111111111",
	},

	//Frame 52
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555111111151",
		"155555551>>1>>151",
		"155555511>>>>>111",
		"15555511>>\?>\?>>11",
		"1555551>>>\?>\?>>11",
		"1555551>1>>>>>>>1",
		"1555551>18>>>>661",
		"1555551>118181>>1",
		"1555551>181111>>1",
		"15555511>>181>>11",
		"11111111111111111",
	},

	//Frame 53
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555111111151",
		"155555551>>1>>151",
		"155555511>>>>>111",
		"15555511>>\?>\?>>11",
		"1555551>>>\?>\?>>11",
		"1555551>1>>>>>>>1",
		"1555551>18>>>>661",
		"1555551>118181>>1",
		"1555551>181111>>1",
		"15555511>>181>>11",
		"11111111111111111",
	},

	//Frame 54
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555511111111",
		"1555555551>>1>>11",
		"1555555511>>>>>11",
		"155555511>>\?>\?>>1",
		"15555551>>>\?>\?>>1",
		"15555551>1>>>>>>1",
		"15555551>18>>>>61",
		"15555551>118181>1",
		"15555551>181111>1",
		"11111111111111111",
	},

	//Frame 55
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555111111",
		"155555555551>>1>1",
		"155555555511>>>>1",
		"15555555511>>\?>\?1",
		"1555555551>>>\?>\?1",
		"1555555551>1>>>>1",
		"1555555551>18>>>1",
		"11111111111111111",
	},

	//Frame 56
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555551111",
		"15555555555551>>1",
		"15555555555511>>1",
		"1555555555511>>\?1",
		"155555555551>>>\?1",
		"11111111111111111",
	},

	//Frame 57
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555551111",
		"15555555555551>>1",
		"15555555555511>>1",
		"1555555555511>>\?1",
		"155555555551>>>\?1",
		"11111111111111111",
	},

	//Frame 58
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555511",
		"15555555555555511",
		"15555555555555111",
		"11111111111111111",
	},

	//Frame 59
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11111111111111111",
	},

	//Frame 60
	{
		"11111111111111111",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"15555555555555551",
		"11111111111111111",
	},

};
