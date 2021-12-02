#ifndef OCR_H
#define OCR_H

#include <SDL/SDL.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Color_Treatment/grayscale.h"
#include "Imagery/Detection/distortion_correction.h"
#include "Imagery/Detection/houghtransform.h"
#include "Imagery/Detection/reverse_color.h"
#include "Imagery/Rotations_Resize/resize.h"
#include "Imagery/Rotations_Resize/rotations.h"
#include "Imagery/Segmentation/split.h"
#include "Imagery/Utils/noise_reduction.h"
#include "NeuralNetwork/save_load.h"
#include "NeuralNetwork/training.h"
#include "Sudoku_Solver/Sudoku_Saver/sudoku_saver.h"
#include "Sudoku_Solver/Sudoku_Solvable/sudoku_solvable.h"
#include "Sudoku_Solver/Sudoku_Solved/sudoku_solved.h"
#include "Sudoku_Solver/sudoku_solver.h"
#include "verbose.h"

typedef struct Thread_argument
{
    Image image;
    char *output_path;
    int verbose;
    int save;
    char *output_folder;
    int gui;
    int hexa;
} Thread_argument;

/**
 * @brief
 *
 * @param intput_path
 * @param output_path
 * @param verbose
 * @param save
 * @param output_folder
 * @param gui
 * @param hexa
 * @return pthread_t*
 */
pthread_t *OCR_thread(char *intput_path, char *output_path, int verbose,
                      int save, char *output_folder, int gui, int hexa);

/**
 * @brief
 *
 * @param Thread_args
 * @return void*
 */
void *OCR(void *Thread_args);

#endif