# 1_maze README

This program has two major parts: the first being the generator, and the second being the solver. 

_Generator_
Given any dimensions forming a square, as well as a file to print to, this part of the program generates a maze, represented by a 2D array of maze[row][col] structs, which it then encodes by representing each respective room as a hexadecimal value, and stores those values in a file. In the generator, first, the inputs are processed, and then the maze is then initialized with the appropriate amounts of rows and columns. Following this, using an algorithm known as drunken walk, the program recursively steps through the maze and sets its walls, openings, and connections. After doing this, the maze is then encoded and written into a file.


_Solver_
With its inputs being the file containing the encoded maze, the dimenions, the starting row/columns and goal row/columns, as well as the solution file to write the paths into, the solver 'solves' any given input maze by using depth-first-search in order to find the goal room. Depending on whether the program as ran as FULL (i.e., all the paths taken including backtracks) and PRUNED (the final official path), after the encoded maze is properly decoded using bitwise operations, the depth-first search algorithm is called, which essentially recursively solves the maze by backtracking every time a wall is reached, visiting every possible room until the specified goal room is reached. The appropriate paths (either PRUNED or FULL) are then written to the file, and the program ends.
