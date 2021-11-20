#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Sudoku_Solver/Sudoku_Saver/sudoku_saver.h"
#include "Sudoku_Solver/Sudoku_Solvable/sudoku_solvable.h"
#include "Sudoku_Solver/Sudoku_Solved/sudoku_solved.h"
#include "Sudoku_Solver/sudoku_solver.h"

#define true 1
#define false 0

int main(int argc, char **argv)
{
    if (argc != 2 && argc != 3)
    {
        errx(1,
             "Usage : ./solver input_path [options]\n"
             "options:\n"
             "   -v --verbose : print details\n");
    }

    int verbose = 0;

    if (argc == 3 && (!strcmp(argv[2], "-v") || !strcmp(argv[2], "--verbose")))
    {
        verbose = 1;
    }

    unsigned int grid[dim][dim] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },

        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },

        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    readGrid(grid, argv[1], verbose);

    // basicPrint(grid);

    // Copy array to have different color when saving the image
    unsigned int copy[dim][dim];
    copyArray(grid, copy);

    // Check if the soduko is solvable
    if (!isSolvable(grid))
    {
        errx(EXIT_FAILURE, "Grid can't be solved\n");
    }

    if (verbose)
    {
        printf("    📈 %s is solvable\n", argv[1]);
        printf("    🔧 Solving %s\n", argv[1]);
    }

    // Solve the sudoku and save the time
    time_t t1 = time(NULL);
    solveSuduko(grid, 0, 0);
    time_t t2 = time(NULL);
    double diff = difftime(t1, t2);

    if (!isSolved(grid))
    {
        errx(EXIT_FAILURE, "⛔ Error solving the grid");
    }

    printf("    ✅ Solved grid in %fs\n", diff);

    // Save image
    char output_path[1000];
    snprintf(output_path, sizeof(output_path), "%s.result", argv[1]);

    saveGrid(grid, output_path, verbose);

    // Create, save and free the image
    Image image = createSudokuImage(grid, copy, "");

    char str[1000];
    snprintf(str, sizeof(str), "%s_result.bmp", argv[1]);
    if (SDL_SaveBMP(image.surface, str) != 0)
    {
        errx(EXIT_FAILURE, "Error while saving file");
    }
    freeImage(&image, 0);

    return 0;
}