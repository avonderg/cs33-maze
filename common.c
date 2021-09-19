#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

/*
 * Determines whether or not the room at [row][col] is a valid room within the
 *maze with dimensions num_rows x num_cols
 *
 * Parameters:
 *  - row: row of the current room
 *  - col: column of the current room
 *  - num_rows: number of rows in the maze
 *  - num_cols: number of columns in the maze
 *
 * Returns:
 *  - 0 if room is not in the maze, 1 if room is in the maze
 */
int is_in_range(int row, int col, int num_rows, int num_cols) {
    // TODO: implement function
    if ((num_rows > row) && (num_cols > col)) {
        return 1;
    }
    else {
        return 0;
    }
    // make sure input row and column are positive!
}

/*
 * Given a pointer to the room and a Direction to travel in, return a pointer to
 *the room that neighbors the current room on the given Direction. For example:
 *get_neighbor(&maze[3][4], EAST) should return &maze[3][5]
 *
 * Parameters:
 *  - num_rows: number of rows in the maze
 *  - num_cols: number of columns in the maze
 *  - room: pointer to the current room
 *  - dir: Direction to get the neighbor from
 *  - maze: a 2D array of maze_room structs representing your maze
 * Returns:
 *  - pointer to the neighboring room
 */
struct maze_room *get_neighbor(int num_rows, int num_cols,
                               struct maze_room maze[num_rows][num_cols],
                               struct maze_room *room, Direction dir) {
    // TODO: implement function
    if (dir == NORTH && is_in_range(room->row -1, room->col,num_rows,num_cols)) {
        // decrease row by 1 in (row,col)
        // new_room->row = (*room).row - 1;
        // new_room->col = (*room).col;
        return &(maze[(*room).row - 1][(*room).col]);
    }
    else if (dir == SOUTH && is_in_range(room->row +1, room->col,num_rows,num_cols)) {
        // increase row by 1
        // new_room->row = (*room).row + 1;
        // new_room->col = (*room).col;
        return &(maze[(*room).row + 1][(*room).col]);
    }
    else if (dir == WEST && is_in_range(room->row, room->col-1,num_rows,num_cols)) {
        // decrease col by 1
        // new_room->row = (*room).row;
        // new_room->col = (*room).col - 1;
        return &(maze[(*room).row][(*room).col - 1]);
    }
    else if (dir == EAST && is_in_range(room->row, room->col+1,num_rows,num_cols)) {
        // increase col by 1
        // new_room->row = (*room).row;
        // new_room->col = (*room).col + 1;
        return &(maze[(*room).row ][(*room).col + 1]);
    }
    else {
        return NULL;
    }
}

/*
 * Initializes a 2D array of maze rooms with all of the necessary values
 *
 * Parameters:
 *  - num_rows: the number of the rows in the maze
 *  - num_cols: the number of columns in the maze
 *  - maze: a 2D array of uninitialized maze_rooms (to be initialized in
 *     this function)
 *
 * Returns:
 *  - nothing (the initialized maze will be stored in the 'maze' array)
 */
void initialize_maze(int num_rows, int num_cols,
           struct maze_room maze[num_rows][num_cols]) {
    for (int i=0; i<num_rows;i++) {
        for (int j=0; j<num_cols;j++) {
            (maze[i][j]).row = i;
            (maze[i][j]).col = j;
            (maze[i][j]).visited = 0;
            (maze[i][j]).connections[0] = 8; // value other than 1 or 0
            (maze[i][j]).connections[1] = 8;
            (maze[i][j]).connections[2] = 8;
            (maze[i][j]).connections[3] = 8;
        }
    }
}

