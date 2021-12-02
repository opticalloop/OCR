#ifndef SOLVABLE_H
#define SOLVABLE_H

#include <stdio.h>

/**
 * @brief
 *
 * @param grid
 * @param y
 * @param dimension
 * @return unsigned int
 */
unsigned int isRowSolvable(unsigned int **grid, unsigned int y,
                           unsigned int dimension);

/**
 * @brief
 *
 * @param grid
 * @param x
 * @param dimension
 * @return unsigned int
 */
unsigned int isColumnSolvable(unsigned int **grid, unsigned int x,
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
unsigned int isSquareSolvable(unsigned int **grid, unsigned int x,
                              unsigned int y, unsigned int dimension);

/**
 * @brief
 *
 * @param grid
 * @param dimension
 * @return unsigned int
 */
unsigned int isSolvable(unsigned int **grid, unsigned int dimension);

#endif