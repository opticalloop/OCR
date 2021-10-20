#include "Sudoku_Solver/sudoku_solver.h"


unsigned int canBePlaced(unsigned int grid[dim][dim], unsigned int x,
                         unsigned int y, unsigned int val)
{
    // Check row
    for (unsigned int i = 0; i < dim; i++)
    {
        if (grid[x][i] == val)
        {
            return false;
        }
    }

    // Check column
    for (unsigned int i = 0; i < dim; i++)
    {
        if (grid[i][y] == val)
        {
            return false;
        }
    }

    // Check square
    x -= x % 3;
    y -= y % 3;

    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            if (grid[x + i][y + j] == val)
            {
                return false;
            }
        }
    }

    return true;
}

unsigned int solveSuduko(unsigned int grid[dim][dim], unsigned int x,
                         unsigned int y)
{
    // End of recursion
    if (x >= 8 && y > 8)
    {
        return true;
    }

    // End of line
    if (y > 8)
    {
        x++;
        y = 0;
    }

    // Check is not empty
    if (grid[x][y] != 0)
    {
        return solveSuduko(grid, x, y + 1);
    }

    // Test all 9 possibilities
    for (unsigned int val = 1; val <= dim; val++)
    {
        // If number can be placed at this case
        if (canBePlaced(grid, x, y, val) == true)
        {
            // Assign number
            grid[x][y] = val;

            // Test with backtracking whether is it solved with this possiblity
            if (solveSuduko(grid, x, y + 1) == true)
            {
                return true;
            }

            // Undo the assignement
            grid[x][y] = 0;
        }
    }
    return false;
}