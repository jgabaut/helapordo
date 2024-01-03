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
#include "archer_drop.h"

char archer_drop[61][MAXROWS][MAXCOLS] = {

	//Frame 1
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1=1;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 2
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1=1;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 3
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1=1;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 4
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1=1;00001",
		"1;>00;;====;00001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1::0>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 5
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1=1;00001",
		"1;>00;;====;00001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1::0>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 6
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1=1;00001",
		"1;>00;;====;00001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1::0>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 7
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;;0001",
		"10000;====;;;0001",
		"10000;====;;00001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1::0>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 8
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;;0001",
		"10000;====;;;0001",
		"10000;====;;00001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1::0>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 9
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;;0001",
		"10000;====;;;0001",
		"10000;1=1=;;00001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1::0>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 10
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;;0001",
		"10000;====;;;0001",
		"10000;1=1=;;00001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1::0>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 11
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;;0<01",
		"10000;====;;;0001",
		"10000;====;;0<0<1",
		"1;>00;====;0<0001",
		"1;>000@@@@@@00<01",
		"1;0>0:@@@@0:@0001",
		"1;0>::::::0::@001",
		"1;0::::::00::@001",
		"1;::;0:::0:::0001",
		"1::;;0:::0::00001",
		"1::0>0::;00:00001",
		"1;000>::;;:700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 12
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;;0<01",
		"10000;====;;;0001",
		"10000;====;;0<0<1",
		"1;>00;====;0<0001",
		"1;>000@@@@@@00<01",
		"1;0>0:@@@@0:@0001",
		"1;0>::::::0::@001",
		"1;0::::::00::@001",
		"1;::;0:::0:::0001",
		"1::;;0:::0::00001",
		"1::0>0::;00:00001",
		"1;000>::;;:700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 13
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;;0<01",
		"10000;====;;;0001",
		"10000;====;;0<0<1",
		"1;>00;====;0<0001",
		"1;>000@@@@@@00<01",
		"1;0>0:@@@@0:@0001",
		"1;0>::::::0::@001",
		"1;0::::::00::@001",
		"1;::;0:::0:::0001",
		"1::;;0:::0::00001",
		"1::0>0::;00:00001",
		"1;000>::;;:700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 14
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;;0<01",
		"10000;====;;;0001",
		"10000;====;;0<0<1",
		"1;>00;====;0<0001",
		"1;>000@@@@@@00<01",
		"1;0>0:@@@@0:@0001",
		"1;0>::::::0::@001",
		"1;0::::::00::@001",
		"1;::;0:::0:::0001",
		"1::;;0:::0::00001",
		"1::0>0::;00:00001",
		"1;000>::;;:700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 15
	{
		"11111111111111111",
		"100000;;;;;;6<001",
		"10000;;;;;;;<<<61",
		"10000;====;;<4<01",
		"10000;1=1=;604401",
		"1;>00;====;000401",
		"1;>000@@@@@@06661",
		"1;0>0:@@@@0:@0601",
		"1;0>::::::06:@001",
		"1;0::::::00::@001",
		"1;::;0:::0:::0001",
		"1::;;0:::0::00001",
		"1::0>0::;00:00061",
		"1;000>::;;:700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 16
	{
		"11111111111111111",
		"100000;;;;;;6<001",
		"10000;;;;;;;<<<61",
		"10000;====;;<4<01",
		"10000;1=1=;604401",
		"1;>00;====;000401",
		"1;>000@@@@@@06661",
		"1;0>0:@@@@0:@0601",
		"1;0>::::::06:@001",
		"1;0::::::00::@001",
		"1;::;0:::0:::0001",
		"1::;;0:::0::00001",
		"1::0>0::;00:00061",
		"1;000>::;;:700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 17
	{
		"11111111111111111",
		"100000;;;;;;6<001",
		"10000;;;;;;;<<<61",
		"10000;====;;<4<01",
		"10000;1=1=;604401",
		"1;>00;====;000401",
		"1;>000@@@@@@06661",
		"1;0>0:@@@@0:@0601",
		"1;0>::::::06:@001",
		"1;0::::::00::@001",
		"1;::;0:::0:::0001",
		"1::;;0:::0::00001",
		"1::0>0::;00:00061",
		"1;000>::;;:700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 18
	{
		"11111111111111111",
		"100000;;;;;;6<001",
		"10000;;;;;;;<<<61",
		"10000;====;;<4<01",
		"10000;1=1=;604401",
		"1;>00;====;000401",
		"1;>000@@@@@@06661",
		"1;0>0:@@@@0:@0601",
		"1;0>::::::06:@001",
		"1;0::::::00::@001",
		"1;::;0:::0:::0001",
		"1::;;0:::0::00001",
		"1::0>0::;00:00061",
		"1;000>::;;:700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 19
	{
		"11111111111111111",
		"100000;;;;;;6<001",
		"10000;;;;;;;<<<61",
		"10000;====;;<4<01",
		"10000;1=1=;604401",
		"1;>00;====;000401",
		"1;>000@@@@@@06661",
		"1;0>0:@@@@0:@0601",
		"1;0>::::::06:@001",
		"1;0::::::00::@001",
		"1;::;0:::0:::0001",
		"1::;;0:::0::00001",
		"1::0>0::;00:00061",
		"1;000>::;;:700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 20
	{
		"11111111111111111",
		"100000;;;;;;0<001",
		"10000;;;;;;;<<<01",
		"10000;====;;<4<01",
		"10000;1=1=;004001",
		"1;>00;====;004001",
		"1;>000@@@@::66601",
		"1;0>0:@@@@::@6001",
		"1;0>:::::000:@001",
		"1;0::::::0:::@001",
		"1;::;0:::0:::0001",
		"1::;;0::;00:00001",
		"1::0>0::;;::00001",
		"1;000>:::::700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 21
	{
		"11111111111111111",
		"100000;;;;;;0<001",
		"10000;;;;;;;<<<01",
		"10000;====;;<4<01",
		"10000;1===;004001",
		"1;>00;====;004001",
		"1;>000@@@@::66601",
		"1;0>0:@@@@::@6001",
		"1;0>:::::000:@001",
		"1;0::::::0:::@001",
		"1;::;0:::0:::0001",
		"1::;;0::;00:00001",
		"1::0>0::;;::00001",
		"1;000>:::::700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 22
	{
		"11111111111111111",
		"100000;;;;;;0<001",
		"10000;;;;;;;<<<01",
		"10000;====;;<4<01",
		"10000;1===;004001",
		"1;>00;====;004001",
		"1;>000@@@@::66601",
		"1;0>0:@@@@::@6001",
		"1;0>:::::000:@001",
		"1;0::::::0:::@001",
		"1;::;0:::0:::0001",
		"1::;;0::;00:00001",
		"1::0>0::;;::00001",
		"1;000>:::::700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 23
	{
		"11111111111111111",
		"100000;;;;;;0<001",
		"10000;;;;;;;<<<01",
		"10000;====;;<4<01",
		"10000;1===;004001",
		"1;>00;====;004001",
		"1;>000@@@@::66601",
		"1;0>0:@@@@::@6001",
		"1;0>:::::000:@001",
		"1;0::::::0:::@001",
		"1;::;0:::0:::0001",
		"1::;;0::;00:00001",
		"1::0>0::;;::00001",
		"1;000>:::::700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 24
	{
		"11111111111111111",
		"100000;;;;;;0<001",
		"10000;;;;;;;<<<01",
		"10000;====;;<4<01",
		"10000;1===;004001",
		"1;>00;====;004001",
		"1;>000@@@@::66601",
		"1;0>0:@@@@::@6001",
		"1;0>:::::000:@001",
		"1;0::::::0:::@001",
		"1;::;0:::0:::0001",
		"1::;;0::;00:00001",
		"1::0>0::;;::00001",
		"1;000>:::::700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 25
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1===;000001",
		"1;>00;====;000001",
		"1;>000@@@@::00001",
		"1;0>0<<@@6::@0001",
		"1;0><<44466::@001",
		"1;0::<<::6:::@001",
		"1;::;0:::0:::0001",
		"1::;;0:0000;00001",
		"1::0>0::00;;00001",
		"1;000>:::::700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 26
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1===;000001",
		"1;>00;====;000001",
		"1;>000@@@@::00001",
		"1;0>0<<@@6::@0001",
		"1;0><<44466::@001",
		"1;0::<<::6:::@001",
		"1;::;0:::0:::0001",
		"1::;;0:0000;00001",
		"1::0>0::00;;00001",
		"1;000>:::::700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 27
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;====;000001",
		"1;>00;====;000001",
		"1;>000@@@@::00001",
		"1;0>0<<@@6::@0001",
		"1;0><<44466::@001",
		"1;0::<<::6:::@001",
		"1;::;0:::0:::0001",
		"1::;;0:0000;00001",
		"1::0>0::00;;00001",
		"1;000>:::::700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 28
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;====;000001",
		"1;>00;====;000001",
		"1;>000@@@@::00001",
		"1;0>0<<@@6::@0001",
		"1;0><<44466::@001",
		"1;0::<<::6:::@001",
		"1;::;0:::0:::0001",
		"1::;;0:0000;00001",
		"1::0>0::00;;00001",
		"1;000>:::::700001",
		"10;;0>77777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 29
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1=1=;000001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1::<<0:6::0:00001",
		"1;<<44:::0:700001",
		"10;<<>:6777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 30
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1=1=;000001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1::<<0:6::0:00001",
		"1;<<44:::0:700001",
		"10;<<>:6777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 31
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1=1=;000001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1::<<0:6::0:00001",
		"1;<<44:::0:700001",
		"10;<<>:6777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 32
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1=1=;000001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1::<<0:6::0:00001",
		"1;<<44:::0:700001",
		"10;<<>:6777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 33
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1=1=;000001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1::<<0:6::0:00001",
		"1;<<44:::0:700001",
		"10;<<>:6777\?00001",
		"1000;;\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 34
	{
		"11111111111111111",
		"100000;;;;;;02001",
		"10000;;;;;;;02001",
		"10000;====;;02001",
		"10000;====;000001",
		"1;>00;====;002001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1:00>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1<<0;6\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 35
	{
		"11111111111111111",
		"100000;;;;;;02001",
		"10000;;;;;;;02001",
		"10000;====;;02001",
		"10000;====;000001",
		"1;>00;====;002001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1:00>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1<<0;6\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 36
	{
		"11111111111111111",
		"100000;;;;;;02001",
		"10000;;;;;;;02001",
		"10000;====;;02001",
		"10000;====;000001",
		"1;>00;====;002001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1:00>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1<<0;6\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 37
	{
		"11111111111111111",
		"100000;;;;;;02001",
		"10000;;;;;;;02001",
		"10000;====;;02001",
		"10000;====;000001",
		"1;>00;====;002001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1:00>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1<<0;6\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 38
	{
		"11111111111111111",
		"100000;;;;;;02001",
		"10000;;;;;;;02001",
		"10000;====;;02001",
		"10000;====;000001",
		"1;>00;====;002001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1:00>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1<<0;6\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 39
	{
		"11111111111111111",
		"100000;;;;;;02001",
		"10000;;;;;;;02001",
		"10000;====;;02001",
		"10000;====;000001",
		"1;>00;====;002001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1:00>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1<<0;6\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 40
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1=1=;000001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1:00>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;0\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 41
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1=1=;000001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1:00>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;0\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 42
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1=1=;000001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1:00>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;0\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 43
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1=1=;000001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1:00>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;0\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 44
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1=1=;000001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1:00>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;0\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 45
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1=1=;000001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1:00>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;0\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 46
	{
		"11111111111111111",
		"100000;;;;;;00001",
		"10000;;;;;;;00001",
		"10000;====;;00001",
		"10000;1=1=;000001",
		"1;>00;====;000001",
		"1;>000@@@@@@00001",
		"1;0>0:@@@@@:@0001",
		"1;0>::::::0::@001",
		"1;0:::::::0::@001",
		"1;::;0:::0::;0001",
		"1::;;0::0::;;0001",
		"1:00>0:0::0:00001",
		"1;000>:::0:700001",
		"10;;0>::777\?00001",
		"1000;0\?\?\?\?\?\?00001",
		"11111111111111111",
	},

	//Frame 47
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1=1;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 48
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1=1;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 49
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;====;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 50
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;====;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 51
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;====;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 52
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;====;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 53
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1==;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 54
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1==;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 55
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1==;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 56
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1==;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 57
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1=1;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 58
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1=1;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 59
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1=1;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

	//Frame 60
	{
		"11111111111111111",
		"10000;;;;;;;00001",
		"1000;;;;;;;;00001",
		"1000;;;====;00001",
		"10000;;=1=1;00001",
		"10000;;====;00>;1",
		"10000@@@@@@000>;1",
		"1000@:@@@@@:0>0;1",
		"100@::0::::::>0;1",
		"100@::0:::::::0;1",
		"1000;::0:::0;::;1",
		"1000;;::0::0;;::1",
		"10000:0::0:0>0::1",
		"100007:0:::>000;1",
		"10000\?777::>0;;01",
		"10000\?\?\?\?\?\?;;0001",
		"11111111111111111",
	},

};
