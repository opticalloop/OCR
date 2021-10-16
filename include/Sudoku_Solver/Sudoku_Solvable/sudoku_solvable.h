#ifndef SOLVABLE_H
#define SOLVABLE_H

#define dim 9

unsigned int isRowSolvable(unsigned int grid[dim][dim], unsigned int y);

unsigned int isColumnSolvable(unsigned int grid[dim][dim], unsigned int x);

unsigned int isSquareSolvable(unsigned int grid[dim][dim], unsigned int x,
                              unsigned int y);

unsigned int isSolvable(unsigned int grid[dim][dim]);

#endif