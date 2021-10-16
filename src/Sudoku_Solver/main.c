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
    if (argc != 2)
    {
        errx(1, "Usage : ./solver input_path");
    }

    unsigned int grid[9][9] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },

        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },

        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    createSudokuImage(grid);

    readGrid(grid, argv[1]);

    if (isSolvable(grid) == false)
    {
        errx(EXIT_FAILURE, "Grid can't be solved\n");
    }

    printf("Start solving\n");
    solveSuduko(grid, 0, 0);

	strcat(argv[1], ".result");

    saveGrid(grid, argv[1]);

    return 0;
}