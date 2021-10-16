#include "Sudoku_Solver/Sudoku_Solved/sudoku_solved.h"

#include <stdio.h>

#define true 1
#define false 0

#define dim 9

unsigned int isRowSolved(unsigned int grid[dim][dim], unsigned int y)
{
    unsigned int val;
    for (unsigned int i = 0; i < dim; i++)
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

unsigned int isColumnSolved(unsigned int grid[dim][dim], unsigned int x)
{
    unsigned int val;
    for (unsigned int i = 0; i < dim; i++)
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

unsigned int isSquareSolved(unsigned int grid[dim][dim], unsigned int x,
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
                for (unsigned int k = x + i; k < 3; k++)
                {
                    for (unsigned int l = y + j + 1; l < 3; l++)
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

unsigned int isSolved(unsigned int grid[dim][dim])
{
    for (unsigned int x = 0; x < dim; x++)
    {
        for (unsigned int y = 0; y < dim; y++)
        {
            if (isRowSolved(grid, x) == false)
            {
                return false;
            }

            if (isColumnSolved(grid, y) == false)
            {
                return false;
            }

            // Check square only when enter it
            if (x % 3 == 0 && y % 3 == 0 && isSquareSolved(grid, x, y) == false)
            {
                return false;
            }
        }
    }

    return true;
}