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
#include "floors.h"

/**
 * Takes a Floor pointer and initialises its floor_layout fields to empty.
 * It also sets the area field to 0.
 * @see Floor
 * @see floorClass
 */
void init_floor_layout(Floor *floor)
{
    // Initialize the layout to empty (0)
    for (int x = 0; x < FLOOR_MAX_COLS; x++) {
        for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
            floor->floor_layout[x][y] = 0;
        }
    }
    // Initialize the explored_matrix to -1 (invalid), for cells
    for (int x = 0; x < FLOOR_MAX_COLS; x++) {
        for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
            floor->explored_matrix[x][y] = -1;
        }
    }
    floor->area = 0;
    floor->explored_area = 0;
}

/**
 * Function to calculate the distance between two points
 */
//
float calc_distance(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

/**
 * Takes a Floor pointer and initialises its rooms_matrix and roomclass_matrix, using the values in floor_layout fields.
 * @see Floor
 * @see floorClass
 */
void init_floor_rooms(Floor *floor)
{
    // Initialize the rooms_matrix to NULL
    for (int x = 0; x < FLOOR_MAX_COLS; x++) {
        for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
            floor->rooms_matrix[x][y] = NULL;
        }
    }
    // Initialize the roomclass_matrix to BASIC
    for (int x = 0; x < FLOOR_MAX_COLS; x++) {
        for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
            floor->roomclass_layout[x][y] =
                floor->floor_layout[x][y] == 1 ? BASIC : -1;
        }
    }
}

/**
 * Takes a Floor pointer and tries walking randomly to initialises its floor_layout field.
 * If do_layout_clean is 1, it will reset floor layout when walked area ratio is below or above expectations.
 * @see Floor
 * @see floorClass
 */
void floor_random_walk(Floor *floor, int x, int y, int steps,
                       int do_layout_clean)
{
    int unique_steps = 0;
    int directions[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
    int max_area_ratio = 80;
    int min_area_ratio = 45;

    do {
        log_tag("debug_log.txt", "[FLOOR]", "Doing floor_random_walk().");
        unique_steps = 0;
        int lastdir = -1;
        int last_x = -1;
        int last_y = -1;
        int did_bonk = -1;
        int walked_steps = 0;
        for (int i = 0; i < steps; i++) {
            int dir = rand() % 4;
            if (rand() % 20 == 0) {	//5% chance to retry same direction
                log_tag("debug_log.txt", "[FLOOR]", "Retrying same direction!");
                dir = lastdir > 0 ? lastdir : (rand() % 4);
            }
            log_tag("debug_log.txt", "[FLOOR]", "dir is [%i]", dir);
            while (dir == lastdir && did_bonk) {
                log_tag("debug_log.txt", "[FLOOR]",
                        "dir [%i] == [%i] lastdir!! Won't bonk again.", dir,
                        lastdir);
                dir = rand() % 4;
                log_tag("debug_log.txt", "[FLOOR]", "newdir is [%i]", dir);
            }
            int new_x = -1;
            int new_y = -1;
            do {		//Discard backwards steps
                new_x = x + directions[dir][0];
                new_y = y + directions[dir][1];
            } while (new_x == last_x && new_y == last_y);

            // Check if the new position is within bounds
            if (new_x > 0 && new_x < (FLOOR_MAX_COLS - 1) && new_y > 0
                && new_y < (FLOOR_MAX_ROWS - 1)) {
                if (floor->floor_layout[new_x][new_y] == 0) {
                    floor->floor_layout[new_x][new_y] = 1;
                    unique_steps++;
                } else if (floor->floor_layout[new_x][new_y] == 1) {
                    log_tag("debug_log.txt", "[FLOOR]",
                            "This was not a new cell.");
                } else {
                    log_tag("debug_log.txt", "[ERROR]",
                            "Unexpected value in floor->floor_layout[%i][%i]:  [%i].",
                            new_x, new_y, floor->floor_layout[new_x][new_y]);
                    exit(EXIT_FAILURE);
                }
                x = new_x;
                y = new_y;
                last_x = new_x;
                last_y = new_y;
                lastdir = dir;
                did_bonk = 0;
                walked_steps++;
            } else {
                did_bonk = 1;
            }
        }

        log_tag("debug_log.txt", "[FLOOR]",
                "floor_random_walk():  walked [%i] steps, [%i] unique.",
                walked_steps, unique_steps);
        if ((unique_steps <= (steps / 100 * min_area_ratio)
             || unique_steps >= (steps / 100 * max_area_ratio))
            && (do_layout_clean)) {
            log_tag("debug_log.txt", "[FLOOR]",
                    "floor_random_walk():  area too small, walked [%i] unique steps, will retry. Calling init_floor_layout().",
                    unique_steps);
            init_floor_layout(floor);
            floor->floor_layout[FLOOR_MAX_COLS / 2][FLOOR_MAX_COLS / 2] = 1;
        }
    } while ((unique_steps <= (steps / 100 * min_area_ratio)
              || unique_steps >= (steps / 100 * max_area_ratio))
             && (do_layout_clean));

    floor->area += unique_steps;
    log_tag("debug_log.txt", "[FLOOR]",
            "floor_random_walk():  added [%i] to floor->area. New value: [%i].",
            unique_steps, floor->area);
}

/**
 * Takes a Floor pointer and initialises its roomclass_layout, using the floor_layout fields.
 * @see Floor
 * @see floorClass
 */
void floor_set_room_types(Floor *floor)
{

    int placed_rooms = 0;
    // Assume the center room is the HOME room
    int center_x = FLOOR_MAX_COLS / 2;
    int center_y = FLOOR_MAX_ROWS / 2;
    floor->roomclass_layout[center_x][center_y] = HOME;

    placed_rooms++;
    /*
       FILE* logfile = NULL;
       logfile = fopen("static/floor_debug_log.txt", "w");
       if (logfile == NULL) {
       log_tag("debug_log.txt","[???]","floor_set_room_types():  Failed opening static/floor_debug_log.txt");
       exit(EXIT_FAILURE);
       }
       fprintf(logfile,"%s","");
       fclose(logfile);
       logfile = fopen("static/floor_debug_log.txt", "a");
       if (logfile == NULL) {
       log_tag("debug_log.txt","[???]","floor_set_room_types():  Failed opening static/floor_debug_log.txt");
       exit(EXIT_FAILURE);
       }
     */

    int max_x = -1;
    int min_x = -1;
    int max_y = -1;
    int min_y = -1;

    for (int x = 0; x < FLOOR_MAX_COLS; x++) {
        for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
            if (floor->floor_layout[x][y] == 1) {
                if (min_x == -1) {
                    min_x = x;
                }
                if (max_x < x) {
                    max_x = x;
                }
                if (min_y == -1) {
                    min_y = y;
                }
                if (max_y < y) {
                    max_y = y;
                }
            }
        }
    }

    log_tag("debug_log.txt", "[FLOOR]",
            "floor_set_room_types():  (max/min) x [%i/%i] y [%i/%i].", max_x,
            min_x, max_y, min_y);

    int shop_x = 0;
    int shop_y = 0;
    int shop_attemps = 100;
    do {
        if (shop_attemps == 0) {
            log_tag("debug_log.txt", "[ERROR]",
                    "floor_set_room_types():  failed generating a distant SHOP room in 100 attempts. Just give me a valid one.");
            //TODO: enable logging of insidious floor setups.
            //debug_print_floor_layout(floor, logfile);
            //fprintf(logfile,"Area is [ %i ]", floor->area);
            do {
                // Select a valid random room as the SHOP
                shop_x = rand() % max_x + 1;
                shop_y = rand() % max_y + 1;
            } while (floor->floor_layout[shop_x][shop_y] != 1
                     && (shop_x != center_x || shop_y != center_y));
        } else {
            // Select a valid random room as the SHOP
            shop_x = rand() % max_x + 1;
            shop_y = rand() % max_y + 1;
            shop_attemps--;
        }
    } while (floor->floor_layout[shop_x][shop_y] != 1
             || calc_distance(shop_x, shop_y, center_x, center_y) < 2);

    floor->roomclass_layout[shop_x][shop_y] = SHOP;
    placed_rooms++;

    int boss_x = 0;
    int boss_y = 0;
    int boss_attemps = 100;
    do {
        if (boss_attemps == 0) {
            log_tag("debug_log.txt", "[ERROR]",
                    "floor_set_room_types():  failed generating a distant BOSS room in 100 attempts. Just give me a valid one.");
            //TODO: enable logging of insidious floor setups.
            //debug_print_floor_layout(floor, logfile);
            //fprintf(logfile,"Area is [ %i ]", floor->area);
            do {
                // Select a valid random room as the BOSS
                boss_x = rand() % max_x + 1;
                boss_y = rand() % max_y + 1;
            } while (floor->floor_layout[boss_x][boss_y] != 1
                     && (boss_x != center_x || boss_y != center_y)
                     && (boss_x != shop_x || boss_y != shop_y));
        } else {
            // Select a valid random room as the SHOP
            boss_x = rand() % max_x + 1;
            boss_y = rand() % max_y + 1;
            boss_attemps--;
        }
    } while (floor->floor_layout[boss_x][boss_y] != 1
             || (calc_distance(boss_x, boss_y, center_x, center_y) < 4)
             || (calc_distance(boss_x, boss_y, shop_x, shop_y) < 4));

    floor->roomclass_layout[boss_x][boss_y] = BOSS;
    placed_rooms++;

    int treasure_x = 0;
    int treasure_y = 0;
    int treasure_attemps = 100;
    int tried_treasure = -1;
    int done_treasure = -1;

    int treasure_roll = rand() % 100;	//75% to have a treasure room
    if (treasure_roll >= 25) {
        tried_treasure = 1;
        do {
            if (treasure_attemps == 0) {
                log_tag("debug_log.txt", "[ERROR]",
                        "floor_set_room_types():  failed generating a distant TREASURE room in 100 attempts. Just give me a valid one.");
                //TODO: enable logging of insidious floor setups.
                //debug_print_floor_layout(floor, logfile);
                //fprintf(logfile,"Area is [ %i ]", floor->area);
                do {
                    // Select a valid random room as the TREASURE
                    treasure_x = rand() % max_x + 1;
                    treasure_y = rand() % max_y + 1;
                } while (floor->floor_layout[treasure_x][treasure_y] != 1
                         && (treasure_x != center_x || treasure_y != center_y)
                         && (treasure_x != shop_x || treasure_y != shop_y)
                         && (treasure_x != boss_x || treasure_y != boss_y));
                break;
            } else {
                // Select a valid random room as the TREASURE
                treasure_x = rand() % max_x + 1;
                treasure_y = rand() % max_y + 1;
                treasure_attemps--;
            }
        } while (floor->floor_layout[treasure_x][treasure_y] != 1
                 || (calc_distance(treasure_x, treasure_y, center_x, center_y) <
                     4)
                 || (calc_distance(treasure_x, treasure_y, shop_x, shop_y) < 4)
                 || (calc_distance(treasure_x, treasure_y, boss_x, boss_y) <
                     4));

        floor->roomclass_layout[treasure_x][treasure_y] = TREASURE;
        done_treasure = 1;
    }

    if (tried_treasure && done_treasure) {
        placed_rooms++;
    } else if (tried_treasure && !done_treasure) {
        log_tag("debug_log.txt", "[FLOOR]",
                "Tried and failed treasure generation.");
        exit(EXIT_FAILURE);
    }
    // Mark remaining rooms as corridors or walls
    for (int x = 0; x < FLOOR_MAX_COLS; x++) {
        for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
            if (floor->floor_layout[x][y] == 0) {
                floor->roomclass_layout[x][y] = WALL;
            } else if (floor->roomclass_layout[x][y] == -1) {
                //TODO:
                // If it's an empty room in the dungeon layout, mark it as a corridor
                floor->roomclass_layout[x][y] = BASIC;
            }
        }
    }

    while (floor->area > (placed_rooms * 6)) {
        //Spice it up with enemy rooms
        int enemy_x = -1;
        int enemy_y = -1;
        do {
            enemy_x = rand() % max_x;
            enemy_y = rand() % max_y;
        } while (floor->floor_layout[enemy_x][enemy_y] != 1
                 && floor->roomclass_layout[enemy_x][enemy_y] != BASIC);
        floor->roomclass_layout[enemy_x][enemy_y] = ENEMIES;
        placed_rooms++;
    }
    /*
       fclose(logfile);
     */
}

/**
 * Takes a Floor pointer and initialises its explored_matrix using the values in floor_layout fields.
 * @see Floor
 * @see floorClass
 */
void load_floor_explored(Floor *floor)
{
    // Initialize the explored_matrix to 0 (unexplored), for cells with a 1 in floor_layout, or -1 for empty cells
    for (int x = 0; x < FLOOR_MAX_COLS; x++) {
        for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
            floor->explored_matrix[x][y] =
                floor->floor_layout[x][y] == 1 ? 0 : -1;
        }
    }
}

/**
 * Takes a Floor pointer and prints its roomClass layout to the passed FILE pointer.
 * @see Floor
 * @see floorClass
 * @see roomClass
 */
void debug_print_roomclass_layout(Floor *floor, FILE *fp)
{
    for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
        for (int x = 0; x < FLOOR_MAX_COLS; x++) {
            char ch = '.';
            switch (floor->roomclass_layout[x][y]) {
            case HOME: {
                ch = 'H';
            }
            break;
            case ENEMIES: {
                ch = 'E';
            }
            break;
            case BOSS: {
                ch = 'B';
            }
            break;
            case SHOP: {
                ch = '$';
            }
            break;
            case TREASURE: {
                ch = '*';
            }
            break;
            case WALL: {
                ch = '#';
            }
            break;
            case BASIC: {
                ch = ' ';
            }
            break;
            default: {
                ch = '?';
            }
            break;
            }
            fprintf(fp, "%c ", ch);
        }
        fprintf(fp, "\n");
    }
}

/**
 * Takes a Floor pointer and prints its floor layout to the passed FILE pointer.
 * @see Floor
 * @see floorClass
 */
void debug_print_floor_layout(Floor *floor, FILE *fp)
{
    for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
        for (int x = 0; x < FLOOR_MAX_COLS; x++) {
            fprintf(fp, "%c ", floor->floor_layout[x][y] == 1 ? '1' : ' ');
        }
        fprintf(fp, "\n");
    }
}

#ifdef HELAPORDO_CURSES_BUILD
/**
 * Takes a Floor pointer and prints its roomClass layout to the passed WINDOW pointer.
 * @see Floor
 * @see floorClass
 * @see roomClass
 */
void display_roomclass_layout(Floor *floor, WINDOW *win)
{
    if (win == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "display_roomclass_layout:  win was NULL");
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
        for (int x = 0; x < FLOOR_MAX_COLS; x++) {
            char ch = '.';
            int isColored = -1;
            switch (floor->roomclass_layout[x][y]) {
            case HOME: {
                ch = 'H';
#ifndef _WIN32
                isColored = S4C_BRIGHT_GREEN;
#else
                isColored = S4C_WIN_GREEN;
#endif
            }
            break;
            case ENEMIES: {
                ch = 'E';
#ifndef _WIN32
                isColored = S4C_PURPLE;
#else
                isColored = S4C_WIN_BLUE;
#endif
            }
            break;
            case BOSS: {
                ch = 'B';
#ifndef _WIN32
                isColored = S4C_RED;
#else
                isColored = S4C_WIN_RED;
#endif
            }
            break;
            case SHOP: {
                ch = '$';
#ifndef _WIN32
                isColored = S4C_CYAN;
#else
                isColored = S4C_WIN_CYAN;
#endif
            }
            break;
            case TREASURE: {
                ch = '*';
#ifndef _WIN32
                isColored = S4C_ORANGE;
#else
                isColored = S4C_WIN_WHITE_ON_RED;
#endif
            }
            break;
            case WALL: {
                ch = '#';
#ifndef _WIN32
                isColored = S4C_BRIGHT_YELLOW;
#else
                isColored = S4C_WIN_WHITE;
#endif
            }
            break;
            case BASIC: {
                ch = ' ';
            }
            break;
            default: {
                ch = '?';
            }
            break;
            }

            if (isColored >= 0)
                wattron(win, COLOR_PAIR(isColored));
            mvwprintw(win, y + 3, x + 3, "%c", ch);
            if (isColored >= 0) {
                wattroff(win, COLOR_PAIR(isColored));
                isColored = -1;
            };
            wrefresh(win);
        }
    }

    refresh();
}

/**
 * Takes a Floor pointer and prints its floor layout to the passed WINDOW pointer.
 * @see Floor
 * @see floorClass
 */
void display_floor_layout(Floor *floor, WINDOW *win)
{
    if (win == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "display_floor_layout():  win was NULL.");
        exit(EXIT_FAILURE);
    }
    int isFull = -1;
    int isColored = -1;
    for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
        for (int x = 0; x < FLOOR_MAX_COLS; x++) {
            isFull = (floor->floor_layout[x][y] == 1 ? 1 : 0);
            isColored = isFull;
            if (isColored > 0) {
#ifndef _WIN32
                wattron(win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
#else
                wattron(win, COLOR_PAIR(S4C_WIN_YELLOW));
#endif
            };
            mvwprintw(win, y + 3, x + 3, "%c", (isFull == 1 ? 'X' : ' '));
            if (isColored > 0) {
#ifndef _WIN32
                wattroff(win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
#else
                wattroff(win, COLOR_PAIR(S4C_WIN_YELLOW));
#endif
            };
            wrefresh(win);
        }
    }
    refresh();
}

/**
 * Takes a Floor pointer and prints its explored layout to the passed WINDOW pointer.
 * @see Floor
 * @see floorClass
 */
void display_explored_layout(Floor *floor, WINDOW *win)
{
    if (win == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "display_explored_layout():  win was NULL.");
        exit(EXIT_FAILURE);
    }
    int isWalkable = -1;
    int isExplored = -1;
    for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
        for (int x = 0; x < FLOOR_MAX_COLS; x++) {
            isWalkable = (floor->explored_matrix[x][y] >= 0 ? 1 : 0);
            isExplored = (floor->explored_matrix[x][y] > 0 ? 1 : 0);
            if (isWalkable > 0) {
#ifndef _WIN32
                wattron(win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
#else
                wattron(win, COLOR_PAIR(S4C_WIN_YELLOW));
#endif
                mvwprintw(win, y + 3, x + 3, "%c",
                          (isExplored == 1 ? '1' : '0'));
            };
            if (isWalkable > 0) {
#ifndef _WIN32
                wattroff(win, COLOR_PAIR(S4C_BRIGHT_YELLOW));
#else
                wattroff(win, COLOR_PAIR(S4C_WIN_YELLOW));
#endif
            };
            wrefresh(win);
        }
    }
    refresh();
}

/**
 * Takes a Floor pointer and current x and y position. Draws render to passed WINDOW pointer.
 * @see Floor
 * @see floorClass
 */
void draw_floor_view(Floor *floor, int current_x, int current_y, WINDOW *win)
{
    if (win == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "draw_view():  win was NULL.");
        exit(EXIT_FAILURE);
    }
    clear();
    refresh();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    wclear(win);
    wrefresh(win);
    keypad(win, TRUE);

    // Print a border around the window
    box(win, 0, 0);
    wrefresh(win);

    /*
       int xShift = 3;
       int yShift = 3;
     */

    int xSize = 3;
    int ySize = 3;

    //Center
    draw_cell(floor, current_x, current_y, win, 10, 10, xSize, ySize, 3);

    //Draw player char
    mvwprintw(win, FLOOR_MAX_COLS / 2 - 1, FLOOR_MAX_ROWS / 2 - 1, "%c", '@');
    wrefresh(win);

    refresh();
}

/**
 * Takes a Floor pointer and cell x and y position. Draws render to passed WINDOW pointer.
 * @see Floor
 * @see floorClass
 */
void draw_cell(Floor *floor, int cell_x, int cell_y, WINDOW *win,
               int drawcorner_x, int drawcorner_y, int x_size, int y_size,
               int recurse)
{
    if (win == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "draw_cell():  win was NULL.");
        exit(EXIT_FAILURE);
    }
    int xShift = 0;
    int yShift = 0;

    int xSize = x_size;
    int ySize = y_size;

    if (floor->floor_layout[cell_x][cell_y] == 0) {
        if (floor->roomclass_layout[cell_x][cell_y] != WALL) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "draw_cell(): floor->floor_layout[%i][%i] was (%i).",
                    cell_x, cell_y, floor->floor_layout[cell_x][cell_y]);
            log_tag("debug_log.txt", "[DEBUG]",
                    "draw_cell(): floor->roomclass_layout[%i][%i] was (%s).",
                    cell_x, cell_y,
                    stringFromRoom(floor->roomclass_layout[cell_x][cell_y]));
        }
        for (int i = 0; i < xSize; i++) {
            for (int j = 0; j < ySize; j++) {
                char ch = '?';
                int isWall = -1;
                int isColored = -1;
                isWall =
                    floor->roomclass_layout[cell_x][cell_y] == WALL ? 1 : 0;
                if (isWall > 0) {
                    ch = '#';
#ifndef _WIN32
                    isColored = S4C_PURPLE;
#else
                    isColored = S4C_WIN_PURPLE;
#endif
                } else {
                    ch = '?';
                }
                if (isColored >= 0) {
                    wattron(win, COLOR_PAIR(isColored));
                };
                mvwprintw(win, yShift + j + (drawcorner_y),
                          xShift + i + (drawcorner_x), "%c", ch);
                if (isColored >= 0) {
                    wattroff(win, COLOR_PAIR(isColored));
                    isColored = -1;
                };
                if (isWall > 0) {
                    isWall = -1;
                    ch = '?';
                };
            }
        }
    } else if (floor->floor_layout[cell_x][cell_y] == 1) {
        for (int i = 0; i < xSize; i++) {
            for (int j = 0; j < ySize; j++) {
                char ch = '?';
                int isColored = -1;
                switch (floor->roomclass_layout[cell_x][cell_y]) {
                case WALL: {
                    ch = '#';
#ifndef _WIN32
                    isColored = S4C_BLUE;
#else
                    isColored = S4C_WIN_BLUE;
#endif
                }
                break;
                case BASIC: {
                    ch = '.';
#ifndef _WIN32
                    isColored = S4C_LIGHT_BROWN;
#else
                    isColored = S4C_WIN_WHITE;
#endif
                }
                break;
                case HOME: {
                    ch = 'H';
#ifndef _WIN32
                    isColored = S4C_WHITE;
#else
                    isColored = S4C_WIN_WHITE;
#endif
                }
                break;
                case BOSS: {
                    ch = 'B';
#ifndef _WIN32
                    isColored = S4C_RED;
#else
                    isColored = S4C_WIN_RED;
#endif
                }
                break;
                case TREASURE: {
                    ch = '*';
#ifndef _WIN32
                    isColored = S4C_ORANGE;
#else
                    isColored = S4C_WIN_WHITE_ON_RED;
#endif
                }
                break;
                case SHOP: {
                    ch = '$';
#ifndef _WIN32
                    isColored = S4C_MAGENTA;
#else
                    isColored = S4C_WIN_WHITE_ON_PURPLE;
#endif
                }
                break;
                case ENEMIES: {
                    ch = '^';
#ifndef _WIN32
                    isColored = S4C_CYAN;
#else
                    isColored = S4C_WIN_CYAN;
#endif
                }
                break;
                default: {
                    log_tag("debug_log.txt", "[ERROR]",
                            "draw_cell(): tried drawing an invalid cell for floor->roomclass_layout[%i][%i].",
                            cell_x, cell_y);
                    ch = '?';
#ifndef _WIN32
                    isColored = S4C_DARK_GREEN;
#else
                    isColored = S4C_WIN_WHITE_ON_PURPLE;
#endif
                }
                break;
                }
                if (isColored >= 0) {
                    wattron(win, COLOR_PAIR(isColored));
                };
                mvwprintw(win, yShift + j + (drawcorner_y),
                          xShift + i + (drawcorner_x), "%c", ch);
                if (isColored >= 0) {
                    wattroff(win, COLOR_PAIR(isColored));
                    isColored = -1;
                };
            }
        }
        if (recurse > 0 && cell_x < FLOOR_MAX_COLS - 1)
            draw_cell(floor, cell_x + 1, cell_y, win, drawcorner_x + 3,
                      drawcorner_y, x_size, y_size, recurse - 1);
        if (recurse > 0 && cell_x > 0)
            draw_cell(floor, cell_x - 1, cell_y, win, drawcorner_x - 3,
                      drawcorner_y, x_size, y_size, recurse - 1);
        if (recurse > 0 && cell_y < FLOOR_MAX_ROWS - 1)
            draw_cell(floor, cell_x, cell_y + 1, win, drawcorner_x,
                      drawcorner_y + 3, x_size, y_size, recurse - 1);
        if (recurse > 0 && cell_y > 0)
            draw_cell(floor, cell_x, cell_y - 1, win, drawcorner_x,
                      drawcorner_y - 3, x_size, y_size, recurse - 1);
    } else {
        log_tag("debug_log.txt", "[ERROR]",
                "draw_cell(): floor->floor_layout[%i][%i] was (%i).", cell_x,
                cell_y, floor->floor_layout[cell_x][cell_y]);
        log_tag("debug_log.txt", "[ERROR]",
                "draw_cell(): floor->roomclass_layout[%i][%i] was (%s).",
                cell_x, cell_y,
                stringFromRoom(floor->roomclass_layout[cell_x][cell_y]));
        exit(EXIT_FAILURE);
    }
}

/**
 * Takes a Floor pointer and cell x and y position. Move one square and update passed WINDOW pointer.
 * The additional parameters are needed for handleRogueMenu().
 * @see handleRogueMenu()
 * @see Floor
 * @see floorClass
 */
void move_update(Gamestate *gamestate, Floor *floor, int *current_x,
                 int *current_y, WINDOW *win, Path *path, Fighter *player,
                 Room *room, loadInfo *load_info, Koliseo *kls,
                 Koliseo_Temp *t_kls)
{
    if (win == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "move_update():  win was NULL.");
        exit(EXIT_FAILURE);
    }

    int picked = 0;
    int c = -1;
    int target_x = *current_x;
    int target_y = *current_y;
    while (!picked && (c = wgetch(win))) {
        target_x = *current_x;
        target_y = *current_y;
        switch (c) {
            //TODO
            //Implement a working menu for the Windows build
#ifndef _WIN32
        case 'm': {
            picked = 0;
            handleRogueMenu(gamestate, path, player, room, load_info, kls,
                            t_kls);
            //Draw current FOV
            draw_floor_view(floor, *current_x, *current_y, win);
            continue;
        }
        break;
#endif
        case 'q': {
            if (G_FASTQUIT_ON == 1) {
                log_tag("debug_log.txt", "[DEBUG]",
                        "Player used q to quit from [%s].", __func__);
                kls_free(temporary_kls);
                kls_free(default_kls);
                exit(EXIT_SUCCESS);
            } else {
                log_tag("debug_log.txt", "[DEBUG]",
                        "Player used q in from [%s], but G_FASTQUIT_ON was not 1.",
                        __func__);
            }
        }
        break;
        case KEY_DOWN: {
            picked = 1;
            target_y += 1;
        }
        break;
        case KEY_UP: {
            picked = 1;
            target_y -= 1;
        }
        break;
        case KEY_LEFT: {
            picked = 1;
            target_x -= 1;
        }
        break;
        case KEY_RIGHT: {
            picked = 1;
            target_x += 1;
        }
        break;
        default: {
            log_tag("debug_log.txt", "[FLOOR]",
                    "move_update():  Player char ( %c ) was not accounted for. Target (x=%i,y=%i) class (%s).",
                    c, target_x, target_y,
                    stringFromRoom(floor->
                                   roomclass_layout[target_x][target_y]));
            picked = 0;
            continue;
        }
        }
        if (floor->floor_layout[target_x][target_y] != 1) {
            picked = 0;
            continue;
        } else {
            if (floor->roomclass_layout[target_x][target_y] != WALL
                && floor->floor_layout[target_x][target_y] > 0) {
                if (floor->explored_matrix[target_x][target_y] == 0) {
                    floor->explored_matrix[target_x][target_y] = 1;
                    (floor->explored_area)++;
                    log_tag("debug_log.txt", "[FLOOR]",
                            "move_update():  target x[%i],y[%i] was not walked before. Class: (%s).",
                            target_x, target_y,
                            stringFromRoom(floor->
                                           roomclass_layout[target_x]
                                           [target_y]));
                    log_tag("debug_log.txt", "[FLOOR]",
                            "move_update(): explored area [%i].",
                            floor->explored_area);
                } else {
                    log_tag("debug_log.txt", "[FLOOR]",
                            "move_update():  target x[%i],y[%i] was walked before. Class: (%s).",
                            target_x, target_y,
                            stringFromRoom(floor->
                                           roomclass_layout[target_x]
                                           [target_y]));
                    log_tag("debug_log.txt", "[FLOOR]",
                            "move_update(): explored area [%i], tot area [%i].",
                            floor->explored_area, floor->area);
                }
                *current_x = target_x;
                *current_y = target_y;
                draw_floor_view(floor, *current_x, *current_y, win);
            } else {
                picked = 0;
                log_tag("debug_log.txt", "[DEBUG]",
                        "Bonked in a wall in move_update().");
                continue;
            }

            switch (floor->roomclass_layout[target_x][target_y]) {
            default: {
                log_tag("debug_log.txt", "[FLOOR]",
                        "move_update():  target x[%i],y[%i] was of class (%s).",
                        target_x, target_y,
                        stringFromRoom(floor->
                                       roomclass_layout[target_x]
                                       [target_y]));

            }

            }

        }
    }
}

#else
#ifndef HELAPORDO_RAYLIB_BUILD
#error "HELAPORDO_CURSES_BUILD and HELAPORDO_RAYLIB_BUILD are both undefined.\n"
#else

/**
 * Takes a Floor pointer and prints its roomClass layout using the passed Rectangle as reference position.
 * @see Floor
 * @see floorClass
 * @see roomClass
 */
void display_roomclass_layout(Floor *floor, Rectangle *win, float pixelSize)
{
    if (win == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "display_roomclass_layout:  win was NULL");
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
        for (int x = 0; x < FLOOR_MAX_COLS; x++) {
            char ch = '.';
            int isColored = -1;
            switch (floor->roomclass_layout[x][y]) {
            case HOME: {
                ch = 'H';
                isColored = S4C_BRIGHT_GREEN;
            }
            break;
            case ENEMIES: {
                ch = 'E';
                isColored = S4C_PURPLE;
            }
            break;
            case BOSS: {
                ch = 'B';
                isColored = S4C_RED;
            }
            break;
            case SHOP: {
                ch = '$';
                isColored = S4C_CYAN;
            }
            break;
            case TREASURE: {
                ch = '*';
                isColored = S4C_ORANGE;
            }
            break;
            case WALL: {
                ch = '#';
                isColored = S4C_BRIGHT_YELLOW;
            }
            break;
            case BASIC: {
                ch = ' ';
                isColored = S4C_LIGHT_BROWN;
            }
            break;
            default: {
                ch = '?';
            }
            break;
            }

            Color color = {0};
            if (isColored >= 0) {
                color = ColorFromS4CPalette(palette, isColored);
            }
            DrawRectangle(win->x + (x * ((int)pixelSize) ), win->y + (y * ((int)pixelSize)), pixelSize, pixelSize, color);
            if (isColored >= 0) {
                isColored = -1;
            };
            (void) ch;
        }
    }

}

/**
 * Takes a Floor pointer and prints its floor layout using the passed Rectangle as reference position.
 * @see Floor
 * @see floorClass
 */
void display_floor_layout(Floor * floor, Rectangle * win, float pixelSize)
{
    if (win == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "display_floor_layout():  win was NULL.");
        exit(EXIT_FAILURE);
    }
    int isFull = -1;
    int isColored = -1;
    for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
        for (int x = 0; x < FLOOR_MAX_COLS; x++) {
            isFull = (floor->floor_layout[x][y] == 1 ? 1 : 0);
            isColored = isFull;
            Color color = {0};
            if (isColored > 0) {
                color = ColorFromS4CPalette(palette, S4C_BRIGHT_YELLOW);
            }
            DrawRectangle(win->x + (x * ((int)pixelSize) ), win->y + (y * ((int)pixelSize)), pixelSize, pixelSize, color);
            //mvwprintw(win, y + 3, x + 3, "%c", (isFull == 1 ? 'X' : ' '));
        }
    }
}

/**
 * Takes a Floor pointer and prints its explored layout using the passed Rectangle as reference position.
 * @see Floor
 * @see floorClass
 */
void display_explored_layout(Floor *floor, Rectangle *win, float pixelSize)
{
    if (win == NULL) {
        log_tag("debug_log.txt", "[ERROR]",
                "display_explored_layout():  win was NULL.");
        exit(EXIT_FAILURE);
    }
    int isWalkable = -1;
    int isExplored = -1;
    for (int y = 0; y < FLOOR_MAX_ROWS; y++) {
        for (int x = 0; x < FLOOR_MAX_COLS; x++) {
            isWalkable = (floor->explored_matrix[x][y] >= 0 ? 1 : 0);
            isExplored = (floor->explored_matrix[x][y] > 0 ? 1 : 0);
            Color color = {0};
            if (isWalkable > 0) {
                if (isExplored == 1) {
                    color = ColorFromS4CPalette(palette, S4C_BRIGHT_YELLOW);
                } else {
                    color = ColorFromS4CPalette(palette, S4C_PURPLE);
                }
                DrawRectangle(win->x + (x * ((int)pixelSize) ), win->y + (y * ((int)pixelSize)), pixelSize, pixelSize, color);
            }
        }
    }
}

/**
 * Takes a Floor pointer and cell x and y position. Draws render using the passed Rectangle as reference position.
 * @see Floor
 * @see floorClass
 */
void draw_cell(Floor *floor, int cell_x, int cell_y, Rectangle *win,
               int drawcorner_x, int drawcorner_y, int x_size, int y_size, float pixelSize,
               int recurse)
{
    if (win == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "draw_cell():  win was NULL.");
        exit(EXIT_FAILURE);
    }
    int xSize = x_size;
    int ySize = y_size;

    if (floor->floor_layout[cell_x][cell_y] == 0) {
        if (floor->roomclass_layout[cell_x][cell_y] != WALL) {
            log_tag("debug_log.txt", "[DEBUG]",
                    "draw_cell(): floor->floor_layout[%i][%i] was (%i).",
                    cell_x, cell_y, floor->floor_layout[cell_x][cell_y]);
            log_tag("debug_log.txt", "[DEBUG]",
                    "draw_cell(): floor->roomclass_layout[%i][%i] was (%s).",
                    cell_x, cell_y,
                    stringFromRoom(floor->roomclass_layout[cell_x][cell_y]));
        }
        for (int i = 0; i < xSize; i++) {
            for (int j = 0; j < ySize; j++) {
                char ch = '?';
                int isWall = -1;
                int isColored = -1;
                Color color = {0};
                isWall =
                    floor->roomclass_layout[cell_x][cell_y] == WALL ? 1 : 0;
                if (isWall > 0) {
                    ch = '#';
                    isColored = S4C_PURPLE;
                } else {
                    ch = '?';
                }
                if (isColored >= 0) {
                    color = ColorFromS4CPalette(palette, isColored);
                };
                DrawRectangle(win->x + ((j + drawcorner_x) * ((int)pixelSize) ), win->y + ((i + drawcorner_y) * ((int)pixelSize)), pixelSize, pixelSize, color);
                if (isColored >= 0) {
                    isColored = -1;
                };
                if (isWall > 0) {
                    isWall = -1;
                    ch = '?';
                };
                (void) ch;
            }
        }
    } else if (floor->floor_layout[cell_x][cell_y] == 1) {
        for (int i = 0; i < xSize; i++) {
            for (int j = 0; j < ySize; j++) {
                char ch = '?';
                int isColored = -1;
                Color color = {0};
                switch (floor->roomclass_layout[cell_x][cell_y]) {
                case WALL: {
                    ch = '#';
                    isColored = S4C_BLUE;
                }
                break;
                case BASIC: {
                    ch = '.';
                    isColored = S4C_LIGHT_BROWN;
                }
                break;
                case HOME: {
                    ch = 'H';
                    isColored = S4C_WHITE;
                }
                break;
                case BOSS: {
                    ch = 'B';
                    isColored = S4C_RED;
                }
                break;
                case TREASURE: {
                    ch = '*';
                    isColored = S4C_ORANGE;
                }
                break;
                case SHOP: {
                    ch = '$';
                    isColored = S4C_MAGENTA;
                }
                break;
                case ENEMIES: {
                    ch = '^';
                    isColored = S4C_CYAN;
                }
                break;
                default: {
                    log_tag("debug_log.txt", "[ERROR]",
                            "draw_cell(): tried drawing an invalid cell for floor->roomclass_layout[%i][%i].",
                            cell_x, cell_y);
                    ch = '?';
                    isColored = S4C_DARK_GREEN;
                }
                break;
                }
                if (isColored >= 0) {
                    color = ColorFromS4CPalette(palette, isColored);
                };
                DrawRectangle(win->x + ((j + drawcorner_x) * ((int)pixelSize) ), win->y + ((i + drawcorner_y) * ((int)pixelSize)), pixelSize, pixelSize, color);
                if (isColored >= 0) {
                    isColored = -1;
                };
                (void) ch;
            }
        }
        if (recurse > 0 && cell_x < FLOOR_MAX_COLS - 1)
            draw_cell(floor, cell_x + 1, cell_y, win, drawcorner_x + 3,
                      drawcorner_y, x_size, y_size, pixelSize, recurse - 1);
        if (recurse > 0 && cell_x > 0)
            draw_cell(floor, cell_x - 1, cell_y, win, drawcorner_x - 3,
                      drawcorner_y, x_size, y_size, pixelSize, recurse - 1);
        if (recurse > 0 && cell_y < FLOOR_MAX_ROWS - 1)
            draw_cell(floor, cell_x, cell_y + 1, win, drawcorner_x,
                      drawcorner_y + 3, x_size, y_size, pixelSize, recurse - 1);
        if (recurse > 0 && cell_y > 0)
            draw_cell(floor, cell_x, cell_y - 1, win, drawcorner_x,
                      drawcorner_y - 3, x_size, y_size, pixelSize, recurse - 1);
    } else {
        log_tag("debug_log.txt", "[ERROR]",
                "draw_cell(): floor->floor_layout[%i][%i] was (%i).", cell_x,
                cell_y, floor->floor_layout[cell_x][cell_y]);
        log_tag("debug_log.txt", "[ERROR]",
                "draw_cell(): floor->roomclass_layout[%i][%i] was (%s).",
                cell_x, cell_y,
                stringFromRoom(floor->roomclass_layout[cell_x][cell_y]));
        exit(EXIT_FAILURE);
    }
}

/**
 * Takes a Floor pointer and current x and y position. Draws render using the passed Rectangle as reference position.
 * @see Floor
 * @see floorClass
 */
void draw_floor_view(Floor *floor, int current_x, int current_y, float pixelSize, Rectangle *win)
{
    if (win == NULL) {
        log_tag("debug_log.txt", "[ERROR]", "draw_view():  win was NULL.");
        exit(EXIT_FAILURE);
    }
    int xSize = 3;
    int ySize = 3;

    //Center
    draw_cell(floor, current_x, current_y, win, 10, 10, xSize, ySize, pixelSize, 3);

    //Draw player char
    DrawRectangle( win->x + ((FLOOR_MAX_COLS/2 -1) * pixelSize), win->y + ((FLOOR_MAX_COLS / 2 - 1) * pixelSize), pixelSize, pixelSize, ColorFromS4CPalette(palette, S4C_BLUE));
}

/**
 * Takes a Floor pointer and cell x and y position. Move one square update passed Rectangle window pointer.
 * TODO: add handleRogueMenu() args
 * @see Floor
 * @see floorClass
 */
void step_floor(Floor *floor, int *current_x,
                int *current_y, int control)
{

    int picked = 0;
    int target_x = *current_x;
    int target_y = *current_y;
    while (!picked && control > 0) {
        target_x = *current_x;
        target_y = *current_y;
        switch (control) {
        //TODO
        //Implement a working menu for the raylib build
        case KEY_DOWN: {
            picked = 1;
            target_y += 1;
        }
        break;
        case KEY_UP: {
            picked = 1;
            target_y -= 1;
        }
        break;
        case KEY_LEFT: {
            picked = 1;
            target_x -= 1;
        }
        break;
        case KEY_RIGHT: {
            picked = 1;
            target_x += 1;
        }
        break;
        default: {
            log_tag("debug_log.txt", "[FLOOR]",
                    "move_update():  Player char ( %c ) was not accounted for. Target (x=%i,y=%i) class (%s).",
                    control, target_x, target_y,
                    stringFromRoom(floor->
                                   roomclass_layout[target_x][target_y]));
            fprintf(stderr, "Invalid char: {%c}\n", control);
            return;
        }
        }
        if (floor->floor_layout[target_x][target_y] != 1) {
            fprintf(stderr, "%s\n", "floor->floor_layout[target_x][target_y] was not 1.");
            return;
        } else {
            if (floor->roomclass_layout[target_x][target_y] != WALL
                && floor->floor_layout[target_x][target_y] > 0) {
                if (floor->explored_matrix[target_x][target_y] == 0) {
                    floor->explored_matrix[target_x][target_y] = 1;
                    (floor->explored_area)++;
                    log_tag("debug_log.txt", "[FLOOR]",
                            "move_update():  target x[%i],y[%i] was not walked before. Class: (%s).",
                            target_x, target_y,
                            stringFromRoom(floor->
                                           roomclass_layout[target_x]
                                           [target_y]));
                    log_tag("debug_log.txt", "[FLOOR]",
                            "move_update(): explored area [%i].",
                            floor->explored_area);
                } else {
                    log_tag("debug_log.txt", "[FLOOR]",
                            "move_update():  target x[%i],y[%i] was walked before. Class: (%s).",
                            target_x, target_y,
                            stringFromRoom(floor->
                                           roomclass_layout[target_x]
                                           [target_y]));
                    log_tag("debug_log.txt", "[FLOOR]",
                            "move_update(): explored area [%i], tot area [%i].",
                            floor->explored_area, floor->area);
                }
                *current_x = target_x;
                *current_y = target_y;
                //draw_floor_view(floor, *current_x, *current_y, pixelSize, win);
            } else {
                picked = 0;
                log_tag("debug_log.txt", "[DEBUG]",
                        "Bonked in a wall in move_update().");
                fprintf(stderr, "%s\n", "BONK!");
                return;
            }

            switch (floor->roomclass_layout[target_x][target_y]) {
            default: {
                log_tag("debug_log.txt", "[FLOOR]",
                        "move_update():  target x[%i],y[%i] was of class (%s).",
                        target_x, target_y,
                        stringFromRoom(floor->
                                       roomclass_layout[target_x]
                                       [target_y]));
            }
            }
        }
    }
}
#endif // HELAPORDO_RAYLIB_BUILD
#endif // HELAPORDO_CURSES_BUILD
