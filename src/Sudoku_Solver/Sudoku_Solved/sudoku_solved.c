#include "Sudoku_Solver/Sudoku_Solved/sudoku_solved.h"

#define true 1
#define false 0

unsigned int isRowSolved(unsigned int **grid, unsigned int y,
                         unsigned int dimension)
{
    unsigned int val;
    for (unsigned int i = 0; i < dimension; i++)
    {
        val = grid[y][i];

        if (val != 0)
        {
            // Check if already in row, if yes not solvable
            for (unsigned int j = i + 1; j < i; j++)
            {
                if (grid[y][j] == val)
                {
                    return false;
                }
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

unsigned int isColumnSolved(unsigned int **grid, unsigned int x,
                            unsigned int dimension)
{
    unsigned int val;
    for (unsigned int i = 0; i < dimension; i++)
    {
        val = grid[i][x];

        if (val != 0)
        {
            // Check if already in column, if yes not solvable
            for (unsigned int j = i + 1; j < i; j++)
            {
                if (grid[j][x] == val)
                {
                    return false;
                }
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

unsigned int isSquareSolved(unsigned int **grid, unsigned int x, unsigned int y,
                            unsigned int dimension)
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
                for (unsigned int k = x + i; k < increment; k++)
                {
                    for (unsigned int l = y + j + 1; l < increment; l++)
                    {
                        if (grid[k][l] == val)
                        {
                            return false;
                        }
                    }
                }
            }
            // If there is a 0 it is'nt solved
            else
            {
                return false;
            }
        }
    }

    return true;
}

unsigned int isSolved(unsigned int **grid, unsigned int dimension)
{
    for (unsigned int x = 0; x < dimension; x++)
    {
        for (unsigned int y = 0; y < dimension; y++)
        {
            if (isRowSolved(grid, x, dimension) == false)
            {
                return false;
            }

            if (isColumnSolved(grid, y, dimension) == false)
            {
                return false;
            }

            // Check square only when enter it
            if (x % ((unsigned int)sqrt(dimension)) == 0
                && y % ((unsigned int)sqrt(dimension)) == 0
                && isSquareSolved(grid, x, y, dimension) == false)
            {
                return false;
            }
        }
    }

    return true;
}