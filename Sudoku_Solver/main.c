#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Sudoku_Saver/sudoku_saver.h"
#include "Sudoku_Solvable/sudoku_solvable.h"
#include "sudoku_solver.h"

#define true 1
#define false 0

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        errx(1, "Usage : ./main input_path output_path");
    }

    unsigned int grid[9][9] = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}};

    // createSudokuImage(grid);

    readGrid(grid, argv[1]);

    basicPrint(grid);

    // return 0;

    // if (isSolvable(grid) == false){
    //    errx(1, "Grid can't be solved\n");
    // }

    // printf("Start solving\n");
    // solveSuduko(grid, 0, 0);
    // print(grid);

    // // saveGrid(argv[2]);

    // for (i = 0; i < dim; i++)
    // {
    //     free(grid[i]);
    // }
    // free(grid);

    return 0;
}