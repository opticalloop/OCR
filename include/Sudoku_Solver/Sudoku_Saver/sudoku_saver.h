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

/**
 * @brief
 *
 * @param grid
 * @param destination
 * @param dimension
 */
void copyArray(unsigned int **grid, unsigned int **destination,
               unsigned int dimension);

/**
 * @brief
 *
 * @param grid
 * @param dimension
 */
void basicPrint(unsigned int **grid, unsigned int dimension);

/**
 * @brief
 *
 * @param grid
 * @param inputPath
 * @param verbose
 * @param dimension
 */
void readGrid(unsigned int **grid, char inputPath[], int verbose,
              unsigned int dimension);

/**
 * @brief
 *
 * @param grid
 * @param outputPath
 * @param verbose
 * @param dimension
 */
void saveGrid(unsigned int **grid, char outputPath[], int verbose,
              unsigned int dimension);

/**
 * @brief Create a Sudoku Image object
 *
 * @param grid
 * @param copy
 * @param folder_path
 * @param dimension
 * @return Image
 */
Image createSudokuImage(unsigned int **grid, unsigned int **copy,
                        char *folder_path, unsigned int dimension);

/**
 * @brief Create a Hexa Sudoku Image object
 * 
 * @param grid 
 * @param copy 
 * @param folder_path 
 * @return Image 
 */
Image createHexaSudokuImage(unsigned int **grid, unsigned int **copy,
                        char *folder_path);

/**
 * @brief Get the Image object
 *
 * @param val
 * @param directory
 * @param green
 * @return Image
 */
Image getImage(unsigned int val, char *directory, unsigned int green);

#endif