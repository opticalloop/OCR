#include <stdio.h>

#include "Sudoku_Solvable/solvable.h"
#include "sudoku_solver.h"
#include "Sudoku_Printer/sudoku_printer.h"

#define true 1
#define false 0

int main()
{
    int grid[9][9] = {{ 1, 0, 0,  0, 0, 0,  0, 0, 0 },
                      { 0, 0, 0,  1, 0, 0,  0, 0, 0 },
                      { 0, 0, 0,  0, 0, 0,  0, 0, 0 },

                      { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                      { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                      { 0, 0, 0,  0, 0, 0,  0, 1, 0 },
                      
                      { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                      { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                      { 0, 0, 0,  0, 0, 0,  0, 0, 1 }};
 
    if (isSolvable(grid) == false){
        printf("No solution exists\n");
        return 1;
    }
    
    solveSuduko(grid, 0, 0);
    print(grid);
    
    return 0;

}