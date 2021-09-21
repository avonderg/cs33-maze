#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "solver.h"
Direction directions[4] = {NORTH, SOUTH, WEST, EAST};
/*
 * Given a pointer to a maze_room, set its connections in all four directions
 *based on the hex value passed in.
 *
 * For example:
 *
 *      create_room_connections(&maze[3][4], 0xb)
 *
 * 0xb is 1011 in binary, which means we have walls on all sides except the
 *WEST. This means that every connection in maze[3][4] should be set to 1,
 *except the WEST connection, which should be set to 0.
 *
 * See the handout for more details about our hexadecimal representation, as
 *well as examples on how to use bit masks to extract information from this
 *hexadecimal representation.
 *
 * Parameters:
 *  - room: pointer to the current room
 *  - hex: hexadecimal integer (between 0-15 inclusive) that represents the
 *connections in all four directions from the given room.
 *
 * Returns:
 *  - nothing. The connections should be saved in the maze_room struct
 *pointed to by the parameter (make sure to use pointers correctly!).
 */
void create_room_connections(struct maze_room *room, unsigned int hex) {
    // TODO: implement this function
    int base = 8;
    for (int i=0; i<4;i++) {
        if (hex&base != 0) {
            room->connections[i] = 1;
        }
        else {
            room->connections[i] = 0;
        }
        base = base/2;
    }
}

/*
 * Recursive depth-first search algorithm for solving your maze.
 * This function should also print out either every visited room as it goes, or
 * the final pruned solution, depending on whether the FULL macro is set.
 *
 * Note that you may need to add extra parameters to this function's signature,
 * depending on how you choose to store the pruned solution.
 *
 * See handout for more details, as well as a pseudocode implementation.
 *
 * Parameters:
 *  - row: row of the current room
 *  - col: column of the current room
 *  - goal_row: row of the goal room
 *  - goal_col: col of the goal room
 *  - num_rows: number of rows in the maze
 *  - num_cols: number of columns in the maze
 *  - maze: a 2d array of maze_room structs representing your maze
 *  - file: the file to write the solution to
 *
 * Returns:
 *  - 1 if the current branch finds a valid solution, 0 if no branches are
 *valid.
 */
int dfs(int row, int col, int goal_row, int goal_col, int num_rows,
        int num_cols, struct maze_room maze[num_rows][num_cols], FILE *file) {
    Direction directions[4] = { NORTH, SOUTH, EAST, WEST };
    struct maze_room *r = &(maze[row][col]);
    // TODO: implement this function
    #ifdef FULL
        // write it to the file here?
        fprintf(file, "%d\t", row);
        fprintf(file, "%d\n", col);
        if (fclose(file)) {
        fprintf(stderr, "[Error Writing to File.]\n");
        return 0; // yes or no
    }
    #endif
    if (row == goal_row && col == goal_col) {
        return 1;
    }
    maze[row][col].visited = 1;
    for (int i=0; i<4; i++) {
        struct maze_room *new_room = get_neighbor(num_rows, num_cols, maze, r, directions[i]);
        if (new_room->visited == 0 && r->connections[directions[i]] == 0) {
            if (dfs(new_room->row, new_room->col, goal_row, goal_col, num_rows, num_cols, maze, file)) {
                #ifdef FULL
                // write it to the file here?
                fprintf(file, "%d\t", row);
                fprintf(file, "%d\n", col);
                if (fclose(file)) {
                    fprintf(stderr, "[Error Writing to File.]\n");
                    return 0; // yes or no
                }
                #endif
                r->next = new_room; //pointer to next room
                return 1;
            }
        #ifdef FULL
        // write it to the file here?
        fprintf(file, "%d\t", row);
        fprintf(file, "%d\n", col);
        if (fclose(file)) {
            fprintf(stderr, "[Error Writing to File.]\n");
            return 0; // would i even return something here?
        }
        #endif
        }
    }
    return 0;
}

/*
 * Decodes an encoded maze and stores the resulting maze room data in the
 * passed 'maze' variable.
 *
 * Parameters:
 *  - num_rows: number of rows in the maze
 *  - num_cols: number of columns in the maze
 *  - maze: a 2d array of maze_room structs (where to store the decoded
 *maze)
 *  - encoded_maze: a 2d array of numbers representing a maze
 *
 * Returns:
 *  - nothing; the decoded maze room is stored in the `maze` variable
 */
void decode_maze(int num_rows, int num_cols,
                 struct maze_room maze[num_rows][num_cols],
                 int encoded_maze[num_rows][num_cols]) {
    // TODO: implement this function
    for (int i=0; i<num_rows; i++) {
        for (int j=0; j<num_cols;j++) {
            (maze[i][j]).row = i;
            (maze[i][j]).col = j;
            (maze[i][j]).visited = 0;
            create_room_connections(&maze[i][j], encoded_maze[i][j]);
        }
    }
}

/*
 * Recursively prints the pruned solution path (using the current maze room
 * and its next pointer)
 *
 * Parameters:
 *  - room: a pointer to the current maze room
 *  - file: the file where to print the path
 *
 * Returns:
 *  - 1 if an error occurs, 0 otherwise
 */
int print_pruned_path(struct maze_room *room, FILE *file) {
    // TODO: implement this function
    if (room == NULL) {
        return 1;
    }
    fprintf(file, "%d\t", room->row);
    fprintf(file, "%d\n", room->col);
    if (fclose(file)) {
        fprintf(stderr, "[Error Writing to File.]\n");
        return 1;
    }
    else if (room->next == NULL) {
        return 0;
    }
    print_pruned_path(room->next, file); //recursive call
}

/*
 * Reads encoded maze from the file passed in
 *
 * Parameters:
 *  - num_rows: number of rows in the maze
 *  - num_cols: number of columns in the maze
 *  - encoded_maze: a maze_room array as a hexadecimal array based on its
 *    connections
 *  - file_name: input file to read the encoded maze from
 *
 * Returns:
 *  - 1 if an error occurs, 0 otherwise
 */
int read_encoded_maze_from_file(int num_rows, int num_cols,
                                int encoded_maze[num_rows][num_cols],
                                char *file_name) {
    int err = 0;

    // open file
    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }
    // read each hex value into 2D array
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            unsigned int encoded_room;
            err = fscanf(f, "%1x", &encoded_room);
            encoded_maze[i][j] = encoded_room;
            if (err < 0) {
                fprintf(stderr, "Reading from file failed");
                return 1;
            }
        }
    }
    // close file
    int close = fclose(f);
    if (close == EOF) {
        fprintf(stderr, "Could not close file.\n");
        return 1;
    }
    return 0;
}

/*
 * Main function
 *
 * Parameters:
 *  - argc: the number of command line arguments - for this function 9
 *  - **argv: a pointer to the first element in the command line
 *            arguments array - for this function:
 *            ["solver", <input maze file>, <number of rows>, <number of
 *columns> <output path file>, <starting row>, <starting column>, <ending row>,
 *<ending column>]
 *
 * Returns:
 *  - 0 if program exits correctly, 1 if there is an error
 */

int main(int argc, char **argv) {
    int num_rows, num_cols, start_row, start_col, goal_row, goal_col;
    char *maze_file_name;
    char *path_file_name;
    if (argc != 9) {
        printf("Incorrect number of arguments.\n");
        printf(
            "./solver <input maze file> <number of rows> <number of columns>");
        printf(" <output path file> <starting row> <starting column>");
        printf(" <ending row> <ending column>\n");
        return 1;
    } else {
        maze_file_name = argv[1];
        num_rows = atoi(argv[2]);
        num_cols = atoi(argv[3]);
        path_file_name = argv[4];
        start_row = atoi(argv[5]);
        start_col = atoi(argv[6]);
        goal_row = atoi(argv[7]);
        goal_col = atoi(argv[8]);
        struct maze_room encoded_maze[num_rows][num_cols];
        struct maze_room maze[num_rows][num_cols];
        read_encoded_maze_from_file(num_rows,num_cols,encoded_maze,maze_file_name); //correct?
        decode_maze(num_rows,num_cols,maze,encoded_maze);
        #ifdef FULL
        fprintf(path_file_name, "%7s\n", "FULL");
        if (fclose(path_file_name)) {
            fprintf(stderr, "[Error Writing to File.]\n");
            return 0; // would i even return something here?
        }
        #endif
        dfs(start_row, start_col, goal_row, goal_col, num_rows, num_cols,maze,path_file_name);
        #ifdef PRUNED
        fprintf(path_file_name, "%7s\n", "PRUNED");
        print_pruned_path(&maze[start_row][start_col],path_file_name);
        if (fclose(path_file_name)) {
            fprintf(stderr, "[Error Writing to File.]\n");
            return 0; // would i even return something here?
        }
        #endif
        return 0;
        // call dfs first
        // call print pruned path
    }
    // TODO: implement this function
}
