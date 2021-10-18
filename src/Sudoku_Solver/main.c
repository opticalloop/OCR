#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Sudoku_Solver/Sudoku_Saver/sudoku_saver.h"
#include "Sudoku_Solver/Sudoku_Solvable/sudoku_solvable.h"
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

    if (argc == 3 && (!strcmp(argv[2], "-v") || !strcmp(argv[2], "-verbose")))
    {
        verbose = 1;
    }

    unsigned int grid[9][9] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },

        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },

        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    // createSudokuImage(grid);

    readGrid(grid, argv[1], verbose);

    if (isSolvable(grid) == false)
    {
        errx(EXIT_FAILURE, "Grid can't be solved\n");
    }

    if (verbose)
    {
        printf("    📈 ");
        printf(argv[1]);
        printf(" is solvable\n");
        printf("    🔧 Solving ");
        printf(argv[1]);
        printf("\n");
    }

    solveSuduko(grid, 0, 0);

    strcat(argv[1], ".result");

    saveGrid(grid, argv[1], verbose);

    return 0;
}