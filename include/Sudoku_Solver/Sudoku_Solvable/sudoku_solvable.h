#ifndef SOLVABLE_H
#define SOLVABLE_H

#include <stdio.h>

unsigned int isRowSolvable(unsigned int **grid, unsigned int y,
                           unsigned int dimension);

unsigned int isColumnSolvable(unsigned int **grid, unsigned int x,
                              unsigned int dimension);

unsigned int isSquareSolvable(unsigned int **grid, unsigned int x,
                              unsigned int y, unsigned int dimension);

unsigned int isSolvable(unsigned int **grid, unsigned int dimension);

#endif