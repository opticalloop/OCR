#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

int canBePlaced(int grid[9][9], int x, int y, int val);

int solveSuduko(int grid[9][9], int x, int y);

#endif