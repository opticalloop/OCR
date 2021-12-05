#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

/**
 * @brief
 *
 * @param grid
 * @param x
 * @param y
 * @param val
 * @param dimension
 * @return unsigned int
 */
unsigned int canBePlaced(unsigned int **grid, unsigned int x, unsigned int y,
                         unsigned int val, unsigned int dimension);

/**
 * @brief
 *
 * @param grid
 * @param x
 * @param y
 * @param dimension
 * @return unsigned int
 */
unsigned int solveSuduko(unsigned int **grid, unsigned int x, unsigned int y,
                         unsigned int dimension);

/**
 * @brief
 *
 * @param dimension
 * @return unsigned int**
 */
unsigned int **allocGrid(unsigned int dimension);

/**
 * @brief
 *
 * @param grid
 * @param dimension
 */
void freeGrid(unsigned int **grid, unsigned int dimension);

#endif