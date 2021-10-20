#ifndef OCR_H
#define OCR_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "NeuralNetwork/training.h"
#include "NeuralNetwork/save_load.h"

#include "Imagery/Utils/noise_reduction.h"

#include "Sudoku_Solver/sudoku_solver.h"
#include "Sudoku_Solver/Sudoku_Saver/sudoku_saver.h"
#include "Sudoku_Solver/Sudoku_Solvable/sudoku_solvable.h"
#include "Sudoku_Solver/Sudoku_Solved/sudoku_solved.h"

void OCR(char *image_path, char *output_path, int verbose);

#endif