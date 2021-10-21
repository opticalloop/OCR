#ifndef SUDOKU_SOLVED_H
#define SUDOKU_SOLVED_H

#include <stdio.h>

#define dim 9

unsigned int isRowSolved(unsigned int grid[dim][dim], unsigned int y);

unsigned int isColumnSolved(unsigned int grid[dim][dim], unsigned int x);

unsigned int isSquareSolved(unsigned int grid[dim][dim], unsigned int x,
                            unsigned int y);

unsigned int isSolved(unsigned int grid[dim][dim]);

#endif