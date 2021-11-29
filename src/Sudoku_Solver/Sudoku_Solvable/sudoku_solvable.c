#include "Sudoku_Solver/Sudoku_Solvable/sudoku_solvable.h"

#define true 1
#define false 0

unsigned int isRowSolvable(unsigned int **grid, unsigned int y,
                           unsigned int dimension)
{
    unsigned int val;
    for (unsigned int i = 0; i < dimension; i++)
    {
        val = grid[y][i];
        if (val != 0)
        {
            // Check if already in row, if yes not solvable
            for (unsigned int j = i + 1; j < dimension; j++)
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

unsigned int isColumnSolvable(unsigned int **grid, unsigned int x,
                              unsigned int dimension)
{
    unsigned int val;
    for (unsigned int i = 0; i < dimension; i++)
    {
        val = grid[i][x];

        if (val != 0)
        {
            // Check if already in column, if yes not solvable
            for (unsigned int j = i + 1; j < dimension; j++)
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

unsigned int isSquareSolvable(unsigned int **grid, unsigned int x,
                              unsigned int y, unsigned int dimension)
{
    int increment = (int)sqrt(dimension);
    x -= x % increment;
    y -= y % increment;

    unsigned int val;
    for (unsigned int i = 0; i < increment; i++)
    {
        for (unsigned int j = 0; j < increment; j++)
        {
            val = grid[x + i][y + j];

            if (val != 0)
            {
                // Check if val is in the square
                for (unsigned int k = x + i; k < x + increment; k++)
                {
                    for (unsigned int l = y + j + 1; l < y + increment; l++)
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

unsigned int isSolvable(unsigned int **grid, unsigned int dimension)
{
    for (unsigned int x = 0; x < dimension; x++)
    {
        if (isColumnSolvable(grid, x, dimension) == false)
        {
            return false;
        }
    }

    for (unsigned int y = 0; y < dimension; y++)
    {
        if (isRowSolvable(grid, y, dimension) == false)
        {
            return false;
        }
    }

    int increment = sqrt(dimension);
    for (unsigned int x = 0; x < dimension; x++)
    {
        for (unsigned int y = 0; y < dimension; y++)
        {
            // Check square only when enter it
            if (y % increment == 0 && x % increment == 0
                && isSquareSolvable(grid, x, y, dimension) == false)
            {
                return false;
            }
        }
    }

    return true;
}