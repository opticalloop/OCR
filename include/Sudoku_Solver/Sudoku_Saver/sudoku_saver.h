#ifndef SUDOKU_SAVER_H
#define SUDOKU_SAVER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Utils/image.h"

#define IMAGE_DIRECTORY "Sudoku_Saver/Images"
#define IMAGE_SIZE 28

void copyArray(unsigned int **grid, unsigned int **destination,
               unsigned int dimension);

void basicPrint(unsigned int **grid, unsigned int dimension);

void readGrid(unsigned int **grid, char inputPath[], int verbose,
              unsigned int dimension);

void saveGrid(unsigned int **grid, char outputPath[], int verbose,
              unsigned int dimension);

SDL_Surface *createSudokuImage(unsigned int **grid, unsigned int **copy,
                        char *folder_path, unsigned int dimension);

SDL_Surface *getImage(unsigned int val, char *directory, unsigned int green);

#endif