#ifndef SUDOKU_SOLVED_H
#define SUDOKU_SOLVED_H

#include <stdio.h>

unsigned int isRowSolved(unsigned int **grid, unsigned int y,
                         unsigned int dimension);

unsigned int isColumnSolved(unsigned int **grid, unsigned int x,
                            unsigned int dimension);

unsigned int isSquareSolved(unsigned int **grid, unsigned int x, unsigned int y,
                            unsigned int dimension);

unsigned int isSolved(unsigned int **grid, unsigned int dimension);

#endif