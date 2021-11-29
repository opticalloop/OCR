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
#include "Imagery/Segmentation/split9.h"
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

/*
 * OCR
 *
 * @param image_path : path of the image to be OCRed
 * @param output_path : path of the output file
 * @param verbose : verbose mode
 * @param save : save mode
 * @param output_folder : path of the output folder
 * @param gui : gui mode
 * @param hexa : hexa mode
 *
 * @return 0 if success, -1 if error
 */

pthread_t *OCR_thread(char *intput_path, char *output_path, int verbose,
                      int save, char *output_folder, int gui, int hexa);

/*
 * OCR_thread_function
 *
 * @param arg : Thread_argument
 *
 * @return 0 if success, -1 if error
 */
void *OCR(void *Thread_args);

#endif