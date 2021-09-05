#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "../../sudoku_solver.h"
#include "../../Sudoku_Solved/sudoku_solved.h"

#define true 1
#define false 0

int main(void)
{
    printf("Sudoku solver test\n");

    // Empty test
    unsigned int grid[9][9] = {{ 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                                
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 }};
    
    clock_t begin = clock();

    solveSuduko(grid, 0, 0);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    assert(isSolved(grid) == true);
    printf("Empty grid took %f\033[0;32mDone\n\033[0m", time_spent);

    // Simple test

    // Normal test

    // Hard Test

    // Impossible test


    return 0;
}