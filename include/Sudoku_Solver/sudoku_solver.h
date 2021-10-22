#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include <stdio.h>

#define true 1
#define false 0

#define dim 9

unsigned int canBePlaced(unsigned int grid[9][9], unsigned int x,
                         unsigned int y, unsigned int val);

unsigned int solveSuduko(unsigned int grid[9][9], unsigned int x,
                         unsigned int y);

#endif