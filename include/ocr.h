#ifndef OCR_H
#define OCR_H

#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Color_Treatment/grayscale.h"
#include "Imagery/Detection/houghtransform.h"
#include "Imagery/Detection/reverse_color.h"
#include "Imagery/Rotations_Resize/resize.h"
#include "Imagery/Rotations_Resize/rotations.h"
#include "Imagery/Segmentation/split9.h"
#include "Imagery/Utils/noise_reduction.h"
#include "NeuralNetwork/save_load.h"
#include "NeuralNetwork/training.h"
#include "Sudoku_Solver/Sudoku_Saver/sudoku_saver.h"
#include "Sudoku_Solver/Sudoku_Solvable/sudoku_solvable.h"
#include "Sudoku_Solver/Sudoku_Solved/sudoku_solved.h"
#include "Sudoku_Solver/sudoku_solver.h"
#include "verbose.h"

/*
 *  Summary:
 *
 *  Params:
 *      *image_path:
 *      *output_path:
 *      verbose:
 *
 *  Return:
 *
 */
void OCR(char *image_path, char *output_path, int verbose, int save,
         char *output_folder);

#endif