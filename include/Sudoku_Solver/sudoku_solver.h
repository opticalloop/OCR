#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

unsigned int canBePlaced(unsigned int **grid, unsigned int x, unsigned int y,
                         unsigned int val, unsigned int dimension);

unsigned int solveSuduko(unsigned int **grid, unsigned int x, unsigned int y,
                         unsigned int dimension);

unsigned int **allocGrid(unsigned int dimension);

void freeGrid(unsigned int **grid, unsigned int dimension);

#endif