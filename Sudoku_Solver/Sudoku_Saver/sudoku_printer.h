#ifndef SUDOKU_PRINTER_H
#define SUDOKU_PRINTER_H

#include "../../Imagery/Utils/image.h"

void basicPrint(unsigned int arr[9][9]);

Image createSudokuImage(unsigned int grid[9][9]);

#endif