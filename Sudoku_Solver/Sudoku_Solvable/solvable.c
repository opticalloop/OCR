#include <stdio.h>

#define true 1
#define false 0

#define dim 9

int alreadyInRow(int grid[dim][dim], int x, int val)
{
    for (int i = 0; i < dim; i++){
        if (grid[x][i] == val){
            return true;
        }
    }
    return false;
}

int alreadyInColumn(int grid[dim][dim], int y, int val)
{
    for (int i = 0; i < dim; i++){
        if (grid[i][y] == val){
            return true;
        }
    }
    return false;
}

int alreadyInSquare(int grid[dim][dim], int x, int y, int val)
{
    x -= x % 3;
    y -= y % 3;

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (grid[x + i][y + j] == val){
                return true;
            }
        }
    }
    return false;
}   


int isRowSolvable(int grid[dim][dim], int x)
{
    int val;
    for (int i = 0; i < dim; i++){
        
        val = grid[x][i];
        
        // Check if already in row, if yes not solvable
        for (int j = 0; j < i ; j++){
            if (grid[x][j] == val){
                return false;
            }
        }
    }

    return true;
}

int isColumnSolvable(int grid[dim][dim], int y)
{
    int val;
    for (int i = 0; i < dim; i++){
        
        val = grid[i][y];
        
        // Check if already in column, if yes not solvable
        for (int j = 0; j < i ; j++){
            if (grid[j][y] == val){
                return false;
            }
        }
    }

    return true;
}

int isSquareSolvable(int grid[dim][dim], int x, int y)
{
    return true;
}


int isSolvable(int grid[dim][dim])
{
    for (int x = 0; x < dim; x++){
        for (int y = 0; y < dim; y++){
            for (int val = 1; val <= dim; val++){
                if (alreadyInRow(grid, x, val) == true 
                && alreadyInColumn(grid, y, val) == true
                && alreadyInSquare(grid, x, y, val) == true){
                    return false;
                }
            }
        }
    }

    return true;
}