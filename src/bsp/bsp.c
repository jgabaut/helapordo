//
// Directly inspired -basically stolen- from bsp-dungeon-generator by Matúš Ďurica.
//
// Reference repo: https://github.com/mtsdurica/bsp-dungeon-generator
// Original license: MIT.
// The author of the original code gave permission to re-license it to ship helapordo with just one license.
//
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

#include "bsp.h"

const char* BSP_Wall_Index_strings[BSP_WALL_INDEX_MAX+1] = {
    [WALL_TOP] = "TOP",
    [WALL_BOTTOM] = "BOTTOM",
    [WALL_RIGHT] = "RIGHT",
    [WALL_LEFT] = "LEFT",
};

const char* stringFromBSP_Wall_Index(int i)
{
    if (i < 0 || i > BSP_WALL_INDEX_MAX) return "";
    return BSP_Wall_Index_strings[i];
}

/**
 * Set horizontal wall in Floor layout.
 * @param w The BSP_Wall to carve
 * @param floor The Floor to carve into
 */
static void set_horizontal_wall(BSP_Wall *w, Floor *floor)
{
    for (int x = w->start_x; x <= w->end_x; x++) {
        int tmp_x = x;
        int tmp_y = w->start_y;
        if ((tmp_x == w->start_x && tmp_y == w->start_y)
            || (tmp_x == w->end_x && tmp_y == w->end_y))
            floor->floor_layout[x][w->start_y] = 0;
        else
            floor->floor_layout[x][w->start_y] = 0;
    }
}

/**
 * Set vertical wall in Floor layout.
 * @param w The BSP_Wall to carve
 * @param floor The Floor to carve into
 */
static void set_vertical_wall(BSP_Wall *w, Floor *floor)
{
    for (int y = w->start_y; y <= w->end_y; y++) {
        int tmp_x = w->start_x;
        int tmp_y = y;
        if ((tmp_x == w->start_x && tmp_y == w->start_y)
            || (tmp_x == w->end_x && tmp_y == w->end_y))
            floor->floor_layout[w->start_x][y] = 0;
        else
            floor->floor_layout[w->start_x][y] = 0;
    }
}

/**
 * Return a random integer between min and max.
 * @param min Min bound
 * @param max Max bound
 * @return A random integer within the bounds
 */
int random_in_range(int min, int max)
{
    // If min is greater than max, swap them
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }

    // Generate a random number within the range [min, max]
    return min + hlpd_rand() % (max - min + 1);
}

/**
 * Set center values for a BSP_Room from its walls data
 * @param room Pointer to the BSP_Room to set the center for.
 */
static void set_room_center(BSP_Room *room)
{
    int center_x, center_y;
    center_x = room->walls[WALL_TOP].start_x + (room->walls[WALL_TOP].end_x - room->walls[WALL_TOP].start_x) / 2;
    center_y = room->walls[WALL_RIGHT].start_y + (room->walls[WALL_RIGHT].end_y - room->walls[WALL_RIGHT].start_y) / 2;
    room->center_x = center_x;
    room->center_y = center_y;
}

/**
 * Prepares base room for bsp.
 * @param floor Floor to set the base room to.
 * @return BSP_Room Base BSP room
 */
static BSP_Room* prep_bsp_baseroom(Floor *floor, Koliseo_Temp* t_kls)
{
    BSP_Room* new_room = KLS_PUSH_T_TYPED(t_kls, BSP_Room, HR_BSP_Room, "BSP_Room", "Base BSP_Room");
    *new_room = (BSP_Room) {
        .walls[WALL_TOP] = (BSP_Wall) {
            .start_x = 0,
            .start_y = 0,
            .end_x = FLOOR_MAX_COLS - 1,
            .end_y = 0
        },
        .walls[WALL_BOTTOM] = (BSP_Wall) {
            .start_x = 0,
            .start_y = FLOOR_MAX_ROWS - 1,
            .end_x = FLOOR_MAX_COLS - 1,
            .end_y = FLOOR_MAX_ROWS - 1
        },
        .walls[WALL_LEFT] = (BSP_Wall) {
            .start_x = 0,
            .start_y = 0,
            .end_x = 0,
            .end_y = FLOOR_MAX_ROWS - 1
        },
        .walls[WALL_RIGHT] = (BSP_Wall) {
            .start_x = FLOOR_MAX_COLS - 1,
            .start_y = 0,
            .end_x = FLOOR_MAX_COLS - 1,
            .end_y = FLOOR_MAX_ROWS - 1
        },
        .child_left = KLS_PUSH_T_TYPED(t_kls, BSP_Room, HR_BSP_Room, "BSP_Room", "Base BSP_Room->left"),
        .child_right = KLS_PUSH_T_TYPED(t_kls, BSP_Room, HR_BSP_Room, "BSP_Room", "Base BSP_Room->right"),
        .center_x = 0,
        .center_y = 0,
    };
    set_horizontal_wall(&(new_room->walls[WALL_TOP]), floor);
    set_horizontal_wall(&(new_room->walls[WALL_BOTTOM]), floor);
    set_vertical_wall(&(new_room->walls[WALL_LEFT]), floor);
    set_vertical_wall(&(new_room->walls[WALL_RIGHT]), floor);
    set_room_center(new_room);
    return new_room;
}

static void bsp_gen_vert_split(BSP_Room *room, Floor *floor, Koliseo_Temp* t_kls);

/**
 * Generate horizontal split
 *
 * @param room The parent room to split
 * @param floor The Floor holding the floor_layout
 */
static void bsp_gen_horiz_split(BSP_Room *room, Floor *floor, Koliseo_Temp* t_kls)
{
    if (room == NULL)
        return;

    if (room->walls[WALL_LEFT].start_y + (BSP_ROOM_SIZE / 2) >= room->walls[WALL_LEFT].end_y - (BSP_ROOM_SIZE / 2)) {
	    log_tag("debug_log.txt", "[DEBUG]", "%s():    passed BSP_Room can't be split further", __func__);
        /*
        for (int i=0; i < 4; i++) {
            log_tag("debug_log.txt", "[BSP_ROOM]", "Wall [%s]: {" BSP_Wall_Fmt "}", stringFromBSP_Wall_Index(i), BSP_Wall_Arg(room->walls[i]));
        }
        */
        return;
    }

    int rand_y =
        random_in_range(room->walls[WALL_LEFT].start_y + (BSP_ROOM_SIZE / 2), room->walls[WALL_LEFT].end_y - (BSP_ROOM_SIZE / 2));

    BSP_Wall new_split = (BSP_Wall) {
        .start_x = room->walls[WALL_TOP].start_x,
        .start_y = rand_y,
        .end_x = room->walls[WALL_TOP].end_x,
        .end_y = rand_y,
    };

    BSP_Room* child1 = KLS_PUSH_T_TYPED(t_kls, BSP_Room, HR_BSP_Room, "BSP_Room", "Horiz split c1");
    *child1 = (BSP_Room) {
        .walls[WALL_TOP] = room->walls[WALL_TOP],
                           .walls[WALL_BOTTOM] = new_split,
        .walls[WALL_LEFT] = (BSP_Wall) {
            .start_x = room->walls[WALL_TOP].start_x,
            .start_y = room->walls[WALL_TOP].start_y,
            .end_x = new_split.start_x,
            .end_y = new_split.start_y
        },
        .walls[WALL_RIGHT] = (BSP_Wall) {
            .start_x = room->walls[WALL_TOP].end_x,
            .start_y = room->walls[WALL_TOP].end_y,
            .end_x = new_split.end_x,
            .end_y = new_split.end_y
        },
        .child_left = NULL,
        .child_right = NULL,
        .center_x = 0,
        .center_y = 0,
    };
    room->child_left = child1;

    BSP_Room* child2 = KLS_PUSH_T_TYPED(t_kls, BSP_Room, HR_BSP_Room, "BSP_Room", "Horiz split c2");
    *child2 = (BSP_Room) {
        .walls[WALL_TOP] = new_split,
                           .walls[WALL_BOTTOM] = room->walls[WALL_BOTTOM],
        .walls[WALL_LEFT] = (BSP_Wall) {
            .start_x = new_split.start_x,
            .start_y = new_split.start_y,
            .end_x = room->walls[WALL_BOTTOM].start_x,
            .end_y = room->walls[WALL_BOTTOM].start_y
        },
        .walls[WALL_RIGHT] = (BSP_Wall) {
            .start_x = new_split.end_x,
            .start_y = new_split.end_y,
            .end_x = room->walls[WALL_BOTTOM].end_x,
            .end_y = room->walls[WALL_BOTTOM].end_y
        },
        .child_left = NULL,
        .child_right = NULL,
        .center_x = 0,
        .center_y = 0,
    };
    room->child_right = child2;

    set_horizontal_wall(&new_split, floor);

    bsp_gen_vert_split(child1, floor, t_kls);
    set_room_center(child1);

    bsp_gen_vert_split(child2, floor, t_kls);
    set_room_center(child2);
    // Carve corridor between coupled rooms
    for (int i = child2->center_y; i > child1->center_y; i--)
        floor->floor_layout[child2->center_x][i] = 1;
}

/**
 * Generate vertical split
 *
 * @param room The parent room to split
 * @param floor The Floor holding the floor_layout
 */
static void bsp_gen_vert_split(BSP_Room *room, Floor *floor, Koliseo_Temp* t_kls)
{
    if (room == NULL)
        return;

    if (room->walls[WALL_TOP].start_x + BSP_ROOM_SIZE >= room->walls[WALL_TOP].end_x - BSP_ROOM_SIZE) {
	    log_tag("debug_log.txt", "[DEBUG]", "%s():    passed BSP_Room can't be split further", __func__);
        /*
        for (int i=0; i < 4; i++) {
            log_tag("debug_log.txt", "[BSP_ROOM]", "Wall [%s]: {" BSP_Wall_Fmt "}", stringFromBSP_Wall_Index(i), BSP_Wall_Arg(room->walls[i]));
        }
        */
        return;
    }

    int rand_x = random_in_range(room->walls[WALL_TOP].start_x + BSP_ROOM_SIZE, room->walls[WALL_TOP].end_x - BSP_ROOM_SIZE);

    BSP_Wall new_split = (BSP_Wall) {
        .start_x = rand_x,
        .start_y = room->walls[WALL_TOP].start_y,
        .end_x = rand_x,
        .end_y = room->walls[WALL_BOTTOM].start_y,
    };

    BSP_Room* child1 = KLS_PUSH_T_TYPED(t_kls, BSP_Room, HR_BSP_Room, "BSP_Room", "Vert split c1");
    *child1 = (BSP_Room) {
        .walls[WALL_TOP] = (BSP_Wall) {
            .start_x = room->walls[WALL_TOP].start_x,
            .start_y = room->walls[WALL_TOP].start_y,
            .end_x = new_split.start_x,
            .end_y = new_split.start_y
        },
        .walls[WALL_BOTTOM] = (BSP_Wall) {
            .start_x = room->walls[WALL_BOTTOM].start_x,
            .start_y = room->walls[WALL_BOTTOM].start_y,
            .end_x = new_split.end_x,
            .end_y = new_split.end_y
        },
        .walls[WALL_LEFT] = room->walls[WALL_LEFT],
                            .walls[WALL_RIGHT] = new_split,
                                .child_left = NULL,
                                .child_right = NULL,
                                .center_x = 0,
                                .center_y = 0,
    };

    room->child_left = child1;

    BSP_Room* child2 = KLS_PUSH_T_TYPED(t_kls, BSP_Room, HR_BSP_Room, "BSP_Room", "Vert split c2");
    *child2 = (BSP_Room) {
        .walls[WALL_TOP] = (BSP_Wall) {
            .start_x = new_split.start_x,
            .start_y = new_split.start_y,
            .end_x = room->walls[WALL_TOP].end_x,
            .end_y = room->walls[WALL_TOP].end_y
        },
        .walls[WALL_BOTTOM] = (BSP_Wall) {
            .start_x = new_split.end_x,
            .start_y = new_split.end_y,
            .end_x = room->walls[WALL_BOTTOM].end_x,
            .end_y = room->walls[WALL_BOTTOM].end_y
        },
        .walls[WALL_LEFT] = new_split,
                            .walls[WALL_RIGHT] = room->walls[WALL_RIGHT],
                                .child_left = NULL,
                                .child_right = NULL,
                                .center_x = 0,
                                .center_y = 0,
    };

    room->child_right = child2;

    set_vertical_wall(&new_split, floor);

    bsp_gen_horiz_split(child1, floor, t_kls);
    set_room_center(child1);

    bsp_gen_horiz_split(child2, floor, t_kls);
    set_room_center(child2);
    // Carve corridor between coupled rooms
    for (int i = child2->center_x; i > child1->center_x; i--)
        floor->floor_layout[i][child2->center_y] = 1;
}

/**
 * Prepares the floor layout with BSP.
 * @param floor the Floor whose layout is to be prepared
 * @param base_x Base x coordinate
 * @param base_y Base y coordinate
 * @return The BSP_Room tree for the generated floor.
 */
BSP_Room* floor_bsp_gen(Floor* floor, Koliseo_Temp* t_kls, int base_x, int base_y)
{
    for (size_t i = 0; i < FLOOR_MAX_ROWS; i++) {
        for (size_t j = 0; j < FLOOR_MAX_COLS; j++) {
            floor->floor_layout[i][j] = 1;
        }
    }

    BSP_Room* base_room = prep_bsp_baseroom(floor, t_kls);

    if (hlpd_rand() % 2) {
        bsp_gen_vert_split(base_room, floor, t_kls);
    } else {
        bsp_gen_horiz_split(base_room, floor, t_kls);
    }
    return base_room;
}

void dbg_BSP_Room(BSP_Room* bsp_room)
{
    if (bsp_room == NULL) {
        return;
    }
    for (int i=0; i<4; i++) {
        log_tag("debug_log.txt", "[DEBUG-BSP]", "%s():    {" BSP_Wall_Fmt "}", __func__, BSP_Wall_Arg(bsp_room->walls[i]));
    }
    log_tag("debug_log.txt", "[DEBUG-BSP]", "%s():    {c_x: %i, c_y: %i}", __func__, bsp_room->center_x, bsp_room->center_y);
    if (bsp_room->child_left != NULL) {
        dbg_BSP_Room(bsp_room->child_left);
    } else {
        for (int i=0; i<4; i++) {
            log_tag("debug_log.txt", "[DEBUG-BSP]", "%s():    Room with no left child. {" BSP_Wall_Fmt "}", __func__, BSP_Wall_Arg(bsp_room->walls[i]));
        }
    }
    if (bsp_room->child_right != NULL) {
        dbg_BSP_Room(bsp_room->child_right);
    } else {
        for (int i=0; i<4; i++) {
            log_tag("debug_log.txt", "[DEBUG-BSP]", "%s():    Room with no right child. {" BSP_Wall_Fmt "}", __func__, BSP_Wall_Arg(bsp_room->walls[i]));
        }
    }
}

static void draw_vertical_wall(WINDOW* win, BSP_Wall* w, int start_y, int start_x, int color_index)
{
    for (int y = w->start_y; y <= w->end_y; y++) {
        int tmp_x = w->start_x;
        int tmp_y = y;
        if ((tmp_x == w->start_x && tmp_y == w->start_y)
            || (tmp_x == w->end_x && tmp_y == w->end_y)) {
            wattron(win, COLOR_PAIR(color_index));
            mvwprintw(win, start_y + y, start_x + w->start_x, "%s", "#");
            wattroff(win, COLOR_PAIR(color_index));
        } else {
            wattron(win, COLOR_PAIR(color_index));
            mvwprintw(win, start_y + y, start_x + w->start_x, "%s", "@");
            wattroff(win, COLOR_PAIR(color_index));
        }
    }
}

static void draw_horizontal_wall(WINDOW* win, BSP_Wall* w, int start_y, int start_x, int color_index)
{
    for (int x = w->start_x; x <= w->end_x; x++) {
        int tmp_x = x;
        int tmp_y = w->start_y;
        if ((tmp_x == w->start_x && tmp_y == w->start_y)
            || (tmp_x == w->end_x && tmp_y == w->end_y)) {
            wattron(win, COLOR_PAIR(color_index));
            mvwprintw(win, start_y + w->start_y, start_x + x, "%s", "#");
            wattroff(win, COLOR_PAIR(color_index));
        } else {
            wattron(win, COLOR_PAIR(color_index));
            mvwprintw(win, start_y + w->start_y, start_x + x, "%s", "@");
            wattroff(win, COLOR_PAIR(color_index));
        }
    }
}

void draw_BSP_Room(WINDOW* win, BSP_Room* bsp_room, int start_y, int start_x, int color_index)
{
    if (bsp_room == NULL) return;
    if (bsp_room->child_left != NULL) {
        // Depth first
        draw_BSP_Room(win, bsp_room->child_left, start_y, start_x, (color_index == PALETTE_S4C_H_TOTCOLORS ? 9 : color_index+1));
    } else {
        // No left child
        if (bsp_room->child_right != NULL) {
            // Depth first
            draw_BSP_Room(win, bsp_room->child_right, start_y, start_x, (color_index == PALETTE_S4C_H_TOTCOLORS-1  ? 9 : color_index+2));
        } else {
            // No right child, leaf room
            for (int i=0; i<4; i++) {
                if (i == WALL_TOP || i == WALL_BOTTOM) {
                    draw_horizontal_wall(win, &(bsp_room->walls[i]), start_y, start_x, color_index);
                } else {
                    draw_vertical_wall(win, &(bsp_room->walls[i]), start_y, start_x, color_index);
                }
            }
            wattron(win, COLOR_PAIR(color_index));
            mvwprintw(win, start_y + bsp_room->center_y, start_x + bsp_room->center_x, "%s", "c");
            wattroff(win, COLOR_PAIR(color_index));
            return;
        }
    }

    if (bsp_room->child_right != NULL) {
        draw_BSP_Room(win, bsp_room->child_right, start_y, start_x, (color_index == PALETTE_S4C_H_TOTCOLORS-2 ? 9 : color_index+3));
    } else {
        // No right child
        if (bsp_room->child_left != NULL) {
            // Depth first
            draw_BSP_Room(win, bsp_room->child_left, start_y, start_x, (color_index == PALETTE_S4C_H_TOTCOLORS-3 ? 9 : color_index+4));
        } else {
            // No left child, leaf room
            for (int i=0; i<4; i++) {
                if (i == WALL_TOP || i == WALL_BOTTOM) {
                    draw_horizontal_wall(win, &(bsp_room->walls[i]), start_y, start_x, color_index);
                } else {
                    draw_vertical_wall(win, &(bsp_room->walls[i]), start_y, start_x, color_index);
                }
            }
            wattron(win, COLOR_PAIR(color_index));
            mvwprintw(win, start_y + bsp_room->center_y, start_x + bsp_room->center_x, "%s", "c");
            wattroff(win, COLOR_PAIR(color_index));
            return;
        }
    }
}
