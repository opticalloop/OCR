#ifndef STRUCT_H
#define STRUCT_H

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Utils/image.h"

int **initMatriceInt(int x, int y);

void freeMatrice(int **matrice, size_t height);

#endif
