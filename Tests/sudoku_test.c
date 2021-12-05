#include <criterion/criterion.h>

#include "Sudoku_Solver/Sudoku_Solved/sudoku_solved.h"
#include "Sudoku_Solver/sudoku_solver.h"

#define dim 9

Test(TestsSudoku, EmptyGrid)
{
    // Empty test
    unsigned int **grid = allocGrid(dim);
    solveSuduko(grid, 0, 0, dim);

    cr_assert(isSolved(grid, dim), "Empty test failed");
    freeGrid(grid, dim);
}

Test(TestsSudoku, EmptyGrid16)
{
    // Empty test
    unsigned int **grid = allocGrid(16);
    solveSuduko(grid, 0, 0, 16);

    cr_assert(isSolved(grid, 16), "Empty test failed");
    freeGrid(grid, 16);
}

// Test(TestsSudoku, SimpleGrid)
// {
//     // Simple test
//     unsigned int grid[dim][dim] = {
//         { 5, 3, 0, 0, 7, 0, 0, 0, 0 }, { 6, 0, 0, 1, 9, 5, 0, 0, 0 },
//         { 0, 9, 8, 0, 0, 0, 0, 6, 0 },

//         { 8, 0, 0, 0, 6, 0, 0, 0, 3 }, { 4, 0, 0, 8, 0, 3, 0, 0, 1 },
//         { 7, 0, 0, 0, 2, 0, 0, 0, 6 },

//         { 0, 6, 0, 0, 0, 0, 2, 8, 0 }, { 0, 0, 0, 4, 1, 9, 0, 0, 5 },
//         { 0, 0, 0, 0, 8, 0, 0, 7, 9 }
//     };

//     solveSuduko(grid, 0, 0, dim);

//     cr_assert(isSolved(grid, dim), "Simple test failed");
// }

// Test(TestsSudoku, NormalGrid)
// {
//     // Normal test
//     unsigned int grid[dim][dim] = {
//         { 5, 3, 0, 0, 7, 0, 0, 0, 0 }, { 6, 0, 0, 1, 9, 5, 0, 0, 0 },
//         { 0, 9, 8, 0, 0, 0, 0, 6, 0 },

//         { 8, 0, 0, 0, 6, 0, 0, 0, 3 }, { 4, 0, 0, 8, 0, 3, 0, 0, 1 },
//         { 7, 0, 0, 0, 2, 0, 0, 0, 6 },

//         { 0, 6, 0, 0, 0, 0, 2, 8, 0 }, { 0, 0, 0, 4, 1, 9, 0, 0, 5 },
//         { 0, 0, 0, 0, 8, 0, 0, 7, 9 }
//     };

//     solveSuduko(grid, 0, 0, dim);

//     cr_assert(isSolved(grid, dim), "Normal test failed");
// }

// Test(TestsSudoku, HardGrid)
// {
//     // Hard test
//     unsigned int grid[dim][dim] = {
//         { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//         { 0, 0, 0, 0, 0, 0, 0, 0, 0 },

//         { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 1, 0, 0, 0, 0, 0, 0 },
//         { 0, 0, 0, 0, 0, 0, 2, 0, 0 },

//         { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//         { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
//     };

//     solveSuduko(grid, 0, 0, dim);

//     cr_assert(isSolved(grid, dim), "Hard test failed");
// }

// Test(TestsSudoku, WorldHardestGrid)
// {
//     // World hardest test
//     unsigned int grid[dim][dim] = {
//         { 8, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 3, 6, 0, 0, 0, 0, 0 },
//         { 0, 7, 0, 0, 9, 0, 2, 0, 0 },

//         { 0, 5, 0, 0, 0, 7, 0, 0, 0 }, { 0, 0, 0, 0, 4, 5, 7, 0, 0 },
//         { 0, 0, 0, 1, 0, 0, 0, 3, 0 },

//         { 0, 0, 1, 0, 0, 0, 0, 6, 8 }, { 0, 0, 8, 5, 0, 0, 0, 1, 0 },
//         { 0, 9, 0, 0, 0, 0, 4, 0, 0 }
//     };

//     solveSuduko(grid, 0, 0, dim);

//     cr_assert(isSolved(grid, dim), "World hardest test failed");
// }