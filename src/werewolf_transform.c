#include "werewolf_transform.h"

char werewolf_transform[MAXFRAMES][MAXROWS][MAXCOLS] = {

	//Frame 1
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 2
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbffffffffffffffb",
		"bbbfffffffffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 3
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbfffffffffffffb",
		"blbbffffffffffffb",
		"bllbbfffffffffffb",
		"blllbfffffffffffb",
		"bglbbfffffffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 4
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbfffffffffffb",
		"blllbbffffffffffb",
		"bllllbbfffffffffb",
		"blllllbfffffffffb",
		"biiglbbfffffffffb",
		"bibgbbffffffffffb",
		"bgggbfffffffffffb",
		"bgggbfffffffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 5
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbbbfffffffffb",
		"blllllbbffffffffb",
		"bllllllbbfffffffb",
		"blllllllbfffffffb",
		"bigiiglbbfffffffb",
		"bbgibgbbffffffffb",
		"bgggggbfffffffffb",
		"bbbgggbfffffffffb",
		"bggggbbbffffffffb",
		"bbgbbbnbbfffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 6
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbbbbbfffffffb",
		"blllllllbbffffffb",
		"bllllllllbbfffffb",
		"blllllllllbfffffb",
		"bgiigiiglbbfffffb",
		"bgibgibgbbffffffb",
		"bgggggggbfffffffb",
		"bggbbgggbfffffffb",
		"bbgggggbbbffffffb",
		"bbbbgbbbnbbfffffb",
		"bnnbbbnnnnbbffffb",
		"bnnnnnnnnnnbffffb",
		"bnnennnnbnnbffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 7
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bbbbbbbbbbffffffb",
		"bblllllllbbfffffb",
		"blllllllllbbffffb",
		"bllllllllllbffffb",
		"blgiigiiglbbffffb",
		"blgibgibgbbfffffb",
		"bbgggggggbffffffb",
		"bbggbbgggbffffffb",
		"bbbgggggbbbfffffb",
		"bnbbbgbbbnbbffffb",
		"bnnnbbbnnnnbbfffb",
		"bnnnnnnnnnnnbfffb",
		"bnnnennnnbnnbfffb",
		"bbnnnnnnbbnnbfffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 8
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfbbbbbbbbbfffffb",
		"bbblllllllbbffffb",
		"bblllllllllbbfffb",
		"bbllllllllllbfffb",
		"bllgiigiiglbbfffb",
		"bllgibgibgbbffffb",
		"blbgggggggbfffffb",
		"bbbggbbgggbfffffb",
		"bbbbgggggbbbffffb",
		"bnnbbbgbbbnbbfffb",
		"bnnnnbbbnnnnbbffb",
		"bnnnnnnnnnnnnbffb",
		"bbnnnennnnbnnbffb",
		"bbbnnnnnnbbnnbffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 9
	{
		"bbbbbbbbbbbbbbbbb",
		"bfbbbbbbbbbfffffb",
		"bbblllllllbbffffb",
		"bblllllllllbbfffb",
		"bbllllllllllbfffb",
		"bllgiigiiglbbfffb",
		"bllgibgibgbbffffb",
		"blbgggggggbfffffb",
		"bbbggbbgggbfffffb",
		"bbbbgggggbbbffffb",
		"bnnbbbgbbbnbbfffb",
		"bnnnnbbbnnnnbbffb",
		"bnnnnnnnnnnnnbffb",
		"bbnnnennnnbnnbffb",
		"bbbnnnnnnbbnnbffb",
		"bbbnnennnbbnnbffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 10
	{
		"bbbbbbbbbbbbbbbbb",
		"bffbbbbbbbbbffffb",
		"bfbblllllllbbfffb",
		"bfblllllllllbbffb",
		"bbbllllllllllbffb",
		"bbllgiigiiglbbffb",
		"bbllgibgibgbbfffb",
		"bblbgggggggbffffb",
		"bbbbggbbgggbffffb",
		"bbbbbgggggbbbfffb",
		"bbnnbbbgbbbnbbffb",
		"bnnnnnbbbnnnnbbfb",
		"bnnnnnnnnnnnnnbfb",
		"bnbnnnennnnbnnbfb",
		"bnbbnnnnnnbbnnbfb",
		"bnbbnnennnbbnnbfb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 11
	{
		"bbbbbbbbbbbbbbbbb",
		"bffbbbbbbbbbffffb",
		"bfbblllllllbbfffb",
		"bfblllllllllbbffb",
		"bbbllllllllllbffb",
		"bbllgiigiiglbbffb",
		"bbllgibgibgbbfffb",
		"bblbgggggggbffffb",
		"bbbbggbbbggbffffb",
		"bbbbbgbbggbbbfffb",
		"bbnnbgggbbbnbbffb",
		"bnnnnbbbbnnnnbbfb",
		"bnnnnnnnnnnnnnbfb",
		"bnbnnnennnnbnnbfb",
		"bnbbnnnnnnbbnnbfb",
		"bnbbnnennnbbnnbfb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 12
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bffbbbbbbbbbffffb",
		"bfbblllllllbbfffb",
		"bfblllllllllbbffb",
		"bbbllllllllllbffb",
		"bbllgiigiiglbbffb",
		"bbllgibgibgbbfffb",
		"bblbgggggggbffffb",
		"bbbbggbbbggbffffb",
		"bbbbbgbbggbbbfffb",
		"bbnnbgggbbbnbbffb",
		"bnnnnbbbbnnnnbbfb",
		"bnnnnnnnnnnnnnbfb",
		"bnbnnnennnnbnnbfb",
		"bnbbnnnnnnbbnnbfb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 13
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bffbbbbbbbbbffffb",
		"bfbblllllllbbfffb",
		"bfblllllllllbbffb",
		"bbbllllllllllbffb",
		"bbllgiigiiglbbffb",
		"bbllgibgibgbbfffb",
		"bblbgggggggbffffb",
		"bbbbggbbbggbffffb",
		"bbbbbgbbggbbbfffb",
		"bbnnbgggbbbnbbffb",
		"bnnnnbbbbnnnnbbfb",
		"bnnnnnnnnnnnnnbfb",
		"bnbnnnennnnbnnbfb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 14
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfbbbbbbbbbfffffb",
		"bbblllllllbbffffb",
		"bblllllllllbbfffb",
		"bbllllllllllbfffb",
		"bllgiigiiglbbfffb",
		"bllgibgibgbbffffb",
		"blbgggggggbfffffb",
		"bbbggbbbggbfffffb",
		"bbbbgbbggbbbffffb",
		"bnnbgggbbbnbbfffb",
		"bnnnbbbbnnnnbbffb",
		"bnnnnnnnnnnnnbffb",
		"bbnnnennnnbnnbffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 15
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfbbbbbbbbbfffffb",
		"bbblllllllbbffffb",
		"bblllllllllbbfffb",
		"bbllllllllllbfffb",
		"bllgibgibglbbfffb",
		"bllgibgibgbbffffb",
		"blbgggggggbfffffb",
		"bbbggbbbggbfffffb",
		"bbbbgbbggbbbffffb",
		"bnnbgggbbbnbbfffb",
		"bnnnbbbbnnnnbbffb",
		"bnnnnnnnnnnnnbffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 16
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfbbbbbbbbbfffffb",
		"bbblllllllbbffffb",
		"bblllllllllbbfffb",
		"bbllllllllllbfffb",
		"bllgibgibglbbfffb",
		"bllgiigiigbbffffb",
		"blbgggggggbfffffb",
		"bbbggbbbggbfffffb",
		"bbbbgbbggbbbffffb",
		"bnnbgggbbbnbbfffb",
		"bnnnbbbbnnnnbbffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 17
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbbbbbbffffffb",
		"bblllllllbbfffffb",
		"blllllllllbbffffb",
		"bllllllllllbffffb",
		"blgibgibglbbffffb",
		"blgiigiigbbfffffb",
		"bbgggggggbffffffb",
		"bbggbbbggbffffffb",
		"bbbgbbggbbbfffffb",
		"bnbgggbbbnbbffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 18
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbbbbbbffffffb",
		"bbllololobbfffffb",
		"bolololollbbffffb",
		"bolllllllolbffffb",
		"blgiegieglbbffffb",
		"bogiigiigbbfffffb",
		"bboggoggobffffffb",
		"bbggbbbggbffffffb",
		"bbbobbgobbbfffffb",
		"bnbgggbbbnbbffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 19
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbbbbbbffffffb",
		"bbllololobbfffffb",
		"bolololollbbffffb",
		"bolllllllolbffffb",
		"blgiegieglbbffffb",
		"bogiigiigbbfffffb",
		"bboggoggobffffffb",
		"bbggbbbggbffffffb",
		"bbbobbgobbbfffffb",
		"bnbgggbbbnbbffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 20
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbbbbbbffffffb",
		"bbololllobbfffffb",
		"bollllololbbffffb",
		"boolollololbffffb",
		"blgiegieglbbffffb",
		"bogiigiigbbfffffb",
		"bboogogoobffffffb",
		"bbggbboggbffffffb",
		"bbbogogobbbfffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 21
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbbbbbbffffffb",
		"bbololllobbfffffb",
		"bollllololbbffffb",
		"boolollololbffffb",
		"blgiegieglbbffffb",
		"bogiigiigbbfffffb",
		"bboogogoobffffffb",
		"bbggbboggbffffffb",
		"bbbogogobbbfffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 22
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbbbfffffffffb",
		"bolollobffffffffb",
		"blollolobfffffffb",
		"boloooolbbffffffb",
		"bgoeooeolbffffffb",
		"bgiigiigbbffffffb",
		"boogogoobfffffffb",
		"bogbbogobfffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 23
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfbbbfffffffffffb",
		"bboobbffffffffffb",
		"bollobbfffffffffb",
		"bloooobbffffffffb",
		"boeooeobffffffffb",
		"boiooigbffffffffb",
		"bogogoobffffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 24
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfbbbfffffffffffb",
		"bboobbffffffffffb",
		"bollobbfffffffffb",
		"bloooobbffffffffb",
		"boeooeobffffffffb",
		"boiooigbffffffffb",
		"bogogoobffffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 25
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbffffffffffffb",
		"boobbfffffffffffb",
		"bolobbffffffffffb",
		"boooobbfffffffffb",
		"bpoopobfffffffffb",
		"bpoopobfffffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 26
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbffffffffffffffb",
		"bbbfffffffffffffb",
		"bobbffffffffffffb",
		"boobbfffffffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 27
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbffffffffffffffb",
		"bbbfffffffffffffb",
		"bobbffffffffffffb",
		"boobbfffffffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 28
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 29
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 30
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffbbbbbbbfffffb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 31
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffbbbbbbbfffffb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 32
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffbbbbbbbfffffb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbbooeoeoobffffb",
		"bfboooeoeoobbbffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 33
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffbbbbbbbfffffb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbbooeoeoobffffb",
		"bfboooeoeoobbbffb",
		"bfboboooooooobffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 34
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffbbbbbbbfffffb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbbooeoeoobffffb",
		"bfboooeoeoobbbffb",
		"bfboboooooooobffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 35
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffbbbbbbbfffffb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbbooeoeoobffffb",
		"bfboooeoeoobbbffb",
		"bfboboooooooobffb",
		"bfbobiooooggobffb",
		"bfbobbibibooobffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 36
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffbbbbbbbfffffb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbbooeoeoobffffb",
		"bfboooeoeoobbbffb",
		"bfboboooooooobffb",
		"bfbobiooooggobffb",
		"bfbobbibibooobffb",
		"bfbooibbbboobbffb",
		"bfbboobiboobbfffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 37
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffbbbbbbbfffffb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbbooeoeoobffffb",
		"bfboooeoeoobbbffb",
		"bfboboooooooobffb",
		"bfbobiooooggobffb",
		"bfbobbibibooobffb",
		"bfbooibbbboobbffb",
		"bfbboobiboobbfffb",
		"bbbooooooobbbfffb",
		"bboooooooooobbbbb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 38
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffbbbbbbbfffffb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbbooeoeoobffffb",
		"bfboooeoeoobbbffb",
		"bfboboooooooobffb",
		"bfbobiooooggobffb",
		"bfbobbibibooobffb",
		"bfbooibbbboobbffb",
		"bfbboobiboobbfffb",
		"bbbooooooobbbfffb",
		"bboooooooooobbbbb",
		"bbooooaaaoooooobb",
		"bboooooooaooooobb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 39
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbbooeoeoobffffb",
		"bfboooeoeoobbbffb",
		"bfboboooooooobffb",
		"bfbobiooooggobffb",
		"bfbobbibibooobffb",
		"bfbooibbbboobbffb",
		"bfbboobiboobbfffb",
		"bbbooooooobbbfffb",
		"bboooooooooobbbbb",
		"bbooooaaaoooooobb",
		"bboooooooaooooobb",
		"bbbooooooaaabbbbb",
		"bfbaaaaaaaobffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 40
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbboopopoobffffb",
		"bfboooeoeoobbbffb",
		"bfboooooooooobffb",
		"bfbobiooooggobffb",
		"bfbobbibibooobffb",
		"bfbobbbbbboobbffb",
		"bfbboibbbbobbfffb",
		"bbbooobiboobbfffb",
		"bboooooooooobbbbb",
		"bbooooaaaoooooobb",
		"bboooooooaooooobb",
		"bbbooooooaaabbbbb",
		"bfbaaaaaaaobffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 41
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbboopopoobffffb",
		"bfboooeoeoobbbffb",
		"bfboooooooooobffb",
		"bfbobiooooggobffb",
		"bfbobbibibooobffb",
		"bfbobbbbbboobbffb",
		"bfbboibbbbobbfffb",
		"bfbooobiboobbfffb",
		"bfbooooooooobbbbb",
		"bfbooooaaaaooooob",
		"bfboooooooaooooob",
		"bfbbooooooaabbbbb",
		"bffbaaaaaaobffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 42
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbboopopoobffffb",
		"bfboooeoeoobbbffb",
		"bfboooooooooobffb",
		"bfbobiooooggobffb",
		"bfbobbibibooobffb",
		"bfbobbbbbboobbffb",
		"bfbboibbbbobbfffb",
		"bbbooobiboobbfffb",
		"bboooooooooobbbbb",
		"bbooooaaaoooooobb",
		"bboooooooaooooobb",
		"bbbooooooaaabbbbb",
		"bfbaaaaaaaobffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 43
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbbooeoeoobffffb",
		"bfboooeoeoobbbffb",
		"bfboboooooooobffb",
		"bfbobiooooggobffb",
		"bfbobbibibooobffb",
		"bfbooibbbboobbffb",
		"bfbboobiboobbfffb",
		"bbbooooooobbbfffb",
		"bboooooooooobbbbb",
		"bbooooaaaoooooobb",
		"bboooooooaooooobb",
		"bbbooooooaaabbbbb",
		"bfbaaaaaaaobffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 44
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbboopopoobffffb",
		"bfboooeoeoobbbffb",
		"bfboooooooooobffb",
		"bfbobiooooggobffb",
		"bfbobbibibooobffb",
		"bfbobbbbbboobbffb",
		"bfbboibbbbobbfffb",
		"bfbooobiboobbfffb",
		"bfbooooooooobbbbb",
		"bfbooooaaaaooooob",
		"bfboooooooaooooob",
		"bfbbooooooaabbbbb",
		"bffbaaaaaaobffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 45
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffbbbbbbbfffffb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbboopopoobffffb",
		"bfboooeoeoobbbffb",
		"bfboooooooooobffb",
		"bfbobiooooggobffb",
		"bfbobbibibooobffb",
		"bfbobbbbbboobbffb",
		"bfbboibbbbobbfffb",
		"bfbooobiboobbfffb",
		"bfbooooooooobbbbb",
		"bfbooooaaaaooooob",
		"bfboooooooaooooob",
		"bfbboooooobbbbbbb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 46
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffbbbbbbbfffffb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbboopopoobffffb",
		"bfboooeoeoobbbffb",
		"bfboboooooooobffb",
		"bfbobiooooggobffb",
		"bfbobbibibooobffb",
		"bfbobibbbboobbffb",
		"bfbboobiboobbfffb",
		"bfbooooooobbbfffb",
		"bfbooooooooobbbbb",
		"bfbooooaaaaooooob",
		"bfboooooooaooooob",
		"bfbboooooobbbbbbb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 47
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffbbbbbbbfffffb",
		"bfffbooboobfffffb",
		"bffbbooooobbffffb",
		"bfbboopopoobffffb",
		"bfbooopopoobbbffb",
		"bfboboooooooobffb",
		"bfbobiooooggobffb",
		"bfbobbibibooobffb",
		"bfbobibbbboobbffb",
		"bfbboobiboobbfffb",
		"bfbooooooobbbfffb",
		"bfbooooaaaaobbbbb",
		"bfboooooooaooooob",
		"bfboooooooaooooob",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 48
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bffffbbbbbbbffffb",
		"bffffbooboobffffb",
		"bfffbbooooobbfffb",
		"bffbboopopoobfffb",
		"bffbooopopoobbbfb",
		"bffboboooooooobfb",
		"bffbobiooooggobfb",
		"bffbobbibibooobfb",
		"bffbobibbbboobbfb",
		"bffbboobiboobbffb",
		"bffbooooooobbbffb",
		"bfbooooaaaaoobbbb",
		"bfboooooooaooooob",
		"bfboooooooaooooob",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 49
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffbbbbbbbfffb",
		"bfffffbooboobfffb",
		"bffffbbooooobbffb",
		"bfffbboopopoobffb",
		"bfffbooopopoobbbb",
		"bfffboboooooooobb",
		"bfffbobiooooggobb",
		"bfffbobbibibooobb",
		"bfffbobibbbboobbb",
		"bfffbboobiboobbfb",
		"bfffbooooooobbbfb",
		"bffbooooaaaaoobbb",
		"bffboooooooaoooob",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 50
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bffffffbbbbbbbffb",
		"bffffffbooboobffb",
		"bfffffbbooooobbfb",
		"bffffbboopopoobfb",
		"bffffbooopopoobbb",
		"bffffboboooooooob",
		"bffffbobiooooggob",
		"bffffbobbibibooob",
		"bffffbobibbbboobb",
		"bffffbboobiboobbb",
		"bffffbooooooobbbb",
		"bfffboooaaaaooobb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 51
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffbbbbbbbfb",
		"bfffffffbooboobfb",
		"bffffffbbooooobbb",
		"bfffffbboopopoobb",
		"bfffffbooopopoobb",
		"bfffffbobooooooob",
		"bfffffbobiooooggb",
		"bfffffbobbibiboob",
		"bfffffbobibbbboob",
		"bfffffbboobiboobb",
		"bfffffbooooooobbb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 52
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffbbbbbbbfb",
		"bfffffffbooboobfb",
		"bffffffbbooooobbb",
		"bfffffbboopopoobb",
		"bfffffbooopopoobb",
		"bfffffbobooooooob",
		"bfffffbobiooooggb",
		"bfffffbobbibiboob",
		"bfffffbobibbbboob",
		"bfffffbboobiboobb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 53
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffbbbbbbbfb",
		"bfffffffbooboobfb",
		"bffffffbbooooobbb",
		"bfffffbboopopoobb",
		"bfffffbooopopoobb",
		"bfffffbobooooooob",
		"bfffffbobiooooggb",
		"bfffffbobbibiboob",
		"bfffffbobibbbboob",
		"bfffffbboobiboobb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 54
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bffffffffbbbbbbbb",
		"bffffffffbooboobb",
		"bfffffffbbooooobb",
		"bffffffbboopopoob",
		"bffffffbooopopoob",
		"bffffffboboooooob",
		"bffffffbobioooogb",
		"bffffffbobbibibob",
		"bffffffbobibbbbob",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 55
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bffffffffffbbbbbb",
		"bffffffffffboobob",
		"bfffffffffbboooob",
		"bffffffffbboopopb",
		"bffffffffbooopopb",
		"bffffffffboboooob",
		"bffffffffbobiooob",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 56
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bffffffffffffbbbb",
		"bffffffffffffboob",
		"bfffffffffffbboob",
		"bffffffffffbboopb",
		"bffffffffffbooopb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 57
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bffffffffffffbbbb",
		"bffffffffffffboob",
		"bfffffffffffbboob",
		"bffffffffffbboopb",
		"bffffffffffbooopb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 58
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bffffffffffffffbb",
		"bffffffffffffffbb",
		"bfffffffffffffbbb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 59
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

	//Frame 60
	{
		"bbbbbbbbbbbbbbbbb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bfffffffffffffffb",
		"bbbbbbbbbbbbbbbbb",
	},

};
