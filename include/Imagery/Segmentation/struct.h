#ifndef STRUCT_H
#define STRUCT_H

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Utils/image.h"

/**
 * @brief
 *
 * @param x
 * @param y
 * @return int**
 */
int **initMatriceInt(int x, int y);

/**
 * @brief
 *
 * @param matrice
 * @param height
 */
void freeMatriceInt(int **matrice, int height);

#endif
