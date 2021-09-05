#include <stdio.h>

#include "sudoku_solver.h"
#include "Sudoku_Solvable/sudoku_solvable.h"
#include "Sudoku_Printer/sudoku_printer.h"

#define true 1
#define false 0

int main()
{
    unsigned int grid[9][9] = {{ 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                                
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 }};
 
    if (isSolvable(grid) == false){
        printf("No solution exists\n");
        return 1;
    }
    printf("Start solving\n");
    solveSuduko(grid, 0, 0);
    print(grid);
    
    return 0;

}