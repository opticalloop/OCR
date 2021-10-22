#ifndef SUDOKU_SAVER_H
#define SUDOKU_SAVER_H

#include <SDL/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Utils/image.h"

#define dim 9

void basicPrint(unsigned int grid[dim][dim]);

void readGrid(unsigned int grid[dim][dim], char inputPath[], int verbose);

void saveGrid(unsigned int grid[dim][dim], char outputPath[], int verbose);

// Image createSudokuImage(unsigned int grid[dim][dim]);

#endif