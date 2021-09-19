#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

unsigned int canBePlaced(
    unsigned int grid[9][9], unsigned int x, unsigned int y, unsigned int val);

unsigned int solveSuduko(
    unsigned int grid[9][9], unsigned int x, unsigned int y);

#endif