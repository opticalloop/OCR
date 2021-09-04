#ifndef SOLVABLE_H
#define SOLVABLE_H

#define dim 9

int alreadyInRow(int grid[dim][dim], int x, int val);

int alreadyInColumn(int grid[dim][dim], int y, int val);

int alreadyInSquare(int grid[dim][dim], int x, int y, int val);

int isRowSolvable(int grid[dim][dim], int x);

int isColumnSolvable(int grid[dim][dim], int y);

int isSquareSolvable(int grid[dim][dim], int x, int y);

int isSolvable(int grid[dim][dim]);

#endif