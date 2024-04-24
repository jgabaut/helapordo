#include "bsp.h"

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
static BSP_Room prep_bsp_baseroom(Floor *floor)
{
    BSP_Room new_room = (BSP_Room) {
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
        .child_left = NULL,
        .child_right = NULL,
        .center_x = 0,
        .center_y = 0,
    };
    set_horizontal_wall(&new_room.walls[WALL_TOP], floor);
    set_horizontal_wall(&new_room.walls[WALL_BOTTOM], floor);
    set_vertical_wall(&new_room.walls[WALL_LEFT], floor);
    set_vertical_wall(&new_room.walls[WALL_RIGHT], floor);
    return new_room;
}

static void bsp_gen_vert_split(BSP_Room *room, Floor *floor);

/**
 * Generate horizontal split
 *
 * @param room The parent room to split
 * @param floor The Floor holding the floor_layout
 */
static void bsp_gen_horiz_split(BSP_Room *room, Floor *floor)
{
    if (room == NULL)
        return;

    if (room->walls[WALL_LEFT].start_y + (BSP_ROOM_SIZE / 2) >= room->walls[WALL_LEFT].end_y - (BSP_ROOM_SIZE / 2))
        return;

    int rand_y =
        random_in_range(room->walls[WALL_LEFT].start_y + (BSP_ROOM_SIZE / 2), room->walls[WALL_LEFT].end_y - (BSP_ROOM_SIZE / 2));

    BSP_Wall new_split = (BSP_Wall) {
        .start_x = room->walls[WALL_TOP].start_x,
        .start_y = rand_y,
        .end_x = room->walls[WALL_TOP].end_x,
        .end_y = rand_y,
    };

    BSP_Room child1 = (BSP_Room) {
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
    room->child_left = &child1;

    BSP_Room child2 = (BSP_Room) {
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
    room->child_right = &child2;

    set_horizontal_wall(&new_split, floor);

    bsp_gen_vert_split(&child1, floor);
    set_room_center(&child1);

    bsp_gen_vert_split(&child2, floor);
    set_room_center(&child2);
    // Carve corridor between coupled rooms
    for (int i = child2.center_y; i > child1.center_y; i--)
        floor->floor_layout[child2.center_x][i] = 1;
}

/**
 * Generate vertical split
 *
 * @param room The parent room to split
 * @param floor The Floor holding the floor_layout
 */
static void bsp_gen_vert_split(BSP_Room *room, Floor *floor)
{
    if (room == NULL)
        return;

    if (room->walls[WALL_TOP].start_x + BSP_ROOM_SIZE >= room->walls[WALL_TOP].end_x - BSP_ROOM_SIZE)
        return;

    int rand_x = random_in_range(room->walls[WALL_TOP].start_x + BSP_ROOM_SIZE, room->walls[WALL_TOP].end_x - BSP_ROOM_SIZE);

    BSP_Wall new_split = (BSP_Wall) {
        .start_x = rand_x,
        .start_y = room->walls[WALL_TOP].start_y,
        .end_x = rand_x,
        .end_y = room->walls[WALL_BOTTOM].start_y,
    };

    BSP_Room child1 = (BSP_Room) {
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

    room->child_left = &child1;

    BSP_Room child2 = (BSP_Room) {
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

    room->child_right = &child2;

    set_vertical_wall(&new_split, floor);

    bsp_gen_horiz_split(&child1, floor);
    set_room_center(&child1);

    bsp_gen_horiz_split(&child2, floor);
    set_room_center(&child2);
    // Carve corridor between coupled rooms
    for (int i = child2.center_x; i > child1.center_x; i--)
        floor->floor_layout[i][child2.center_y] = 1;
}

/**
 * Prepares the floor layout with BSP.
 * @param floor the Floor whose layout is to be prepared
 * @param x Base x coordinate
 * @param y Base y coordinate
 */
void floor_bsp_gen(Floor* floor, int x, int y)
{
    for (size_t i = 0; i < FLOOR_MAX_ROWS; i++) {
        for (size_t j = 0; j < FLOOR_MAX_COLS; j++) {
            floor->floor_layout[i][j] = 1;
        }
    }

    BSP_Room base_room = prep_bsp_baseroom(floor);

    if (hlpd_rand() % 2) {
        bsp_gen_vert_split(&base_room, floor);
    } else {
        bsp_gen_horiz_split(&base_room, floor);
    }
}
