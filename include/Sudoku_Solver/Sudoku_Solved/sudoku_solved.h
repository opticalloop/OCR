#ifndef SUDOKU_SOLVED_H
#define SUDOKU_SOLVED_H

#include <stdio.h>
#include <math.h>

/**
 * @brief
 *
 * @param grid
 * @param y
 * @param dimension
 * @return unsigned int
 */
unsigned int isRowSolved(unsigned int **grid, unsigned int y,
                         unsigned int dimension);

/**
 * @brief
 *
 * @param grid
 * @param x
 * @param dimension
 * @return unsigned int
 */
unsigned int isColumnSolved(unsigned int **grid, unsigned int x,
                            unsigned int dimension);

/**
 * @brief
 *
 * @param grid
 * @param x
 * @param y
 * @param dimension
 * @return unsigned int
 */
unsigned int isSquareSolved(unsigned int **grid, unsigned int x, unsigned int y,
                            unsigned int dimension);

/**
 * @brief
 *
 * @param grid
 * @param dimension
 * @return unsigned int
 */
unsigned int isSolved(unsigned int **grid, unsigned int dimension);

#endif