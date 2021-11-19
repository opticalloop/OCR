#ifndef SUDOKU_SAVER_H
#define SUDOKU_SAVER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Utils/image.h"

#define dim 9
#define IMAGE_DIRECTORY "Sudoku_Saver/Images"
#define IMAGE_SIZE 28

void copyArray(unsigned int grid[dim][dim], unsigned int destination[dim][dim]);

void basicPrint(unsigned int grid[dim][dim]);

void readGrid(unsigned int grid[dim][dim], char inputPath[], int verbose);

void saveGrid(unsigned int grid[dim][dim], char outputPath[], int verbose);

Image createSudokuImage(unsigned int grid[dim][dim],
                        unsigned int copy[dim][dim], char *folder_path);

SDL_Surface *getImage(unsigned int val, char *directory, unsigned int green);

#endif