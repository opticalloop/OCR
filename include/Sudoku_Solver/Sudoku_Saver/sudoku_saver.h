#ifndef SUDOKU_SAVER_H
#define SUDOKU_SAVER_H

#include "../../Imagery/Utils/image.h"

#define dim 9

void basicPrint(unsigned int arr[dim][dim]);

void readGrid(unsigned int grid[dim][dim], char inputPath[]);

void saveGrid(unsigned int grid[dim][dim], char outputPath[]);

Image createSudokuImage(unsigned int grid[dim][dim]);

#endif