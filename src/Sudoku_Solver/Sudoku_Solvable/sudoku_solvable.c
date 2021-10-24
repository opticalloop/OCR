#include "Sudoku_Solver/Sudoku_Solvable/sudoku_solvable.h"

#define true 1
#define false 0

#define dim 9

unsigned int isRowSolvable(unsigned int grid[dim][dim], unsigned int y)
{
    unsigned int val;
    for (unsigned int i = 0; i < dim; i++)
    {
        val = grid[y][i];
        if (val != 0)
        {
            // Check if already in row, if yes not solvable
            for (unsigned int j = i + 1; j < dim; j++)
            {
                if (grid[y][j] == val)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

unsigned int isColumnSolvable(unsigned int grid[dim][dim], unsigned int x)
{
    unsigned int val;
    for (unsigned int i = 0; i < dim; i++)
    {
        val = grid[i][x];

        if (val != 0)
        {
            // Check if already in column, if yes not solvable
            for (unsigned int j = i + 1; j < dim; j++)
            {
                if (grid[j][x] == val)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

unsigned int isSquareSolvable(unsigned int grid[dim][dim], unsigned int x,
                              unsigned int y)
{
    x -= x % 3;
    y -= y % 3;

    unsigned int val;
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            val = grid[x + i][y + j];

            if (val != 0)
            {
                // Check if val is in the square
                for (unsigned int k = x + i; k < x + 3; k++)
                {
                    for (unsigned int l = y + j + 1; l < y + 3; l++)
                    {
                        if (grid[k][l] == val)
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

unsigned int isSolvable(unsigned int grid[dim][dim])
{
    for (unsigned int x = 0; x < dim; x++)
    {
        if (isColumnSolvable(grid, x) == false)
        {
            return false;
        }
    }

    for (unsigned int y = 0; y < dim; y++)
    {
        if (isRowSolvable(grid, y) == false)
        {
            return false;
        }
    }

    for (unsigned int x = 0; x < dim; x++)
    {
        for (unsigned int y = 0; y < dim; y++)
        {
            // Check square only when enter it
            if (y % 3 == 0 && x % 3 == 0
                && isSquareSolvable(grid, x, y) == false)
            {
                return false;
            }
        }
    }

    return true;
}