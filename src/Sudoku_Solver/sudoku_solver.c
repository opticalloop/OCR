#include "Sudoku_Solver/sudoku_solver.h"

unsigned int canBePlaced(unsigned int **grid, unsigned int x, unsigned int y,
                         unsigned int val, unsigned int dimension)
{
    // Check row
    for (unsigned int i = 0; i < dimension; i++)
    {
        if (grid[x][i] == val)
        {
            return false;
        }
    }

    // Check column
    for (unsigned int i = 0; i < dimension; i++)
    {
        if (grid[i][y] == val)
        {
            return false;
        }
    }

    // Check square
    int increment = (int)sqrt(dimension);
    x -= x % increment;
    y -= y % increment;

    for (int i = 0; i < increment; i++)
    {
        for (int j = 0; j < increment; j++)
        {
            if (grid[x + i][y + j] == val)
            {
                return false;
            }
        }
    }

    return true;
}

unsigned int solveSuduko(unsigned int **grid, unsigned int x, unsigned int y,
                         unsigned int dimension)
{
    // End of recursion
    if (x >= (dimension - 1) && y > (dimension - 1))
    {
        return true;
    }

    // End of line
    if (y > (dimension - 1))
    {
        x++;
        y = 0;
    }

    // Check is not empty
    if (grid[x][y] != 0)
    {
        return solveSuduko(grid, x, y + 1, dimension);
    }

    // Test all 9 possibilities
    for (unsigned int val = 1; val <= dimension; val++)
    {
        // If number can be placed at this case
        if (canBePlaced(grid, x, y, val, dimension) == true)
        {
            // Assign number
            grid[x][y] = val;

            // Test with backtracking whether is it solved with this possiblity
            if (solveSuduko(grid, x, y + 1, dimension) == true)
            {
                return true;
            }

            // Undo the assignement
            grid[x][y] = 0;
        }
    }
    return false;
}

unsigned int **allocGrid(unsigned int dimension)
{
    unsigned int **grid = NULL;
    grid = calloc(dimension + 1, sizeof(unsigned int *));
    if (grid == NULL)
    {
        errx(EXIT_FAILURE, "Failing while allocating grid");
    }
    for (unsigned int i = 0; i < dimension; i++)
    {
        grid[i] = calloc(dimension + 1, sizeof(unsigned int));
        if (grid[i] == NULL)
        {
            errx(EXIT_FAILURE, "Failing while allocating grid");
        }
    }
    return grid;
}

void freeGrid(unsigned int **grid, unsigned int dimension)
{
    for (unsigned int i = 0; i < dimension; i++)
    {
        free(grid[i]);
    }
    free(grid);
}