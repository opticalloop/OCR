#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "../../Sudoku_Printer/sudoku_printer.h"
#include "../../Sudoku_Solved/sudoku_solved.h"
#include "../../sudoku_solver.h"

#define true 1
#define false 0

int main(void)
{
    printf("Sudoku solver test\n");

    // Empty test
    unsigned int grid1[9][9] = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}};

    clock_t begin = clock();

    solveSuduko(grid1, 0, 0);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    assert(isSolved(grid1) == true);
    printf("Empty grid solved in %fs \033[0;32mDone\n\033[0m", time_spent);

    // Simple test
    unsigned int grid2[9][9] = {{5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0}, {0, 9, 8, 0, 0, 0, 0, 6, 0},

        {8, 0, 0, 0, 6, 0, 0, 0, 3}, {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},

        {0, 6, 0, 0, 0, 0, 2, 8, 0}, {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}};

    begin = clock();

    solveSuduko(grid2, 0, 0);

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    assert(isSolved(grid2) == true);
    printf("Simple grid solved in %fs \033[0;32mDone\n\033[0m", time_spent);

    // Normal test
    unsigned int grid3[9][9] = {{5, 3, 0, 0, 7, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 9, 8, 0, 0, 0, 0, 6, 0},

        {8, 0, 0, 0, 6, 0, 0, 0, 3}, {0, 0, 0, 8, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 2, 0, 0, 0, 6},

        {0, 0, 0, 0, 0, 0, 2, 0, 0}, {0, 0, 0, 4, 0, 0, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 0, 9}};

    begin = clock();

    solveSuduko(grid3, 0, 0);

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    assert(isSolved(grid3) == true);
    printf("Normal grid solved in %fs \033[0;32mDone\n\033[0m", time_spent);

    // Very hard test
    unsigned int grid4[9][9] = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}};

    begin = clock();

    solveSuduko(grid4, 0, 0);

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    assert(isSolved(grid4) == true);
    printf("Very hard grid solved in %fs \033[0;32mDone\n\033[0m", time_spent);

    // World haderst Test
    unsigned int grid5[9][9] = {{8, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 3, 6, 0, 0, 0, 0, 0}, {0, 7, 0, 0, 9, 0, 2, 0, 0},

        {0, 5, 0, 0, 0, 7, 0, 0, 0}, {0, 0, 0, 0, 4, 5, 7, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 3, 0},

        {0, 0, 1, 0, 0, 0, 0, 6, 8}, {0, 0, 8, 5, 0, 0, 0, 1, 0},
        {0, 9, 0, 0, 0, 0, 4, 0, 0}};

    begin = clock();

    solveSuduko(grid5, 0, 0);

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    assert(isSolved(grid5) == true);
    printf("World hardest grid solved in %fs \033[0;32mDone\n\033[0m",
        time_spent);

    // Impossible test
    unsigned int grid6[9][9] = {{0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 2, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 1}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}};

    begin = clock();

    // solveSuduko(grid6, 0, 0);

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    assert(isSolved(grid6) == true);
    printf("Found that grid is impossible in %fs \033[0;32mDone\n\033[0m",
        time_spent);

    return 0;
}