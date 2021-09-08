#include <stdio.h>
#include <assert.h>

#include "../../Sudoku_Solved/sudoku_solved.h"

#define true 1
#define false 0

#define dim 9

int main(void)
{  
    // Solved
    unsigned int grid1[9][9] = {{ 1, 2, 3,  4, 5, 6,  7, 8, 9 },
                                { 4, 5, 6,  7, 8, 9,  1, 2, 3 },
                                { 7, 8, 9,  1, 2, 3,  4, 5, 6 },
                               
                                { 2, 1, 4,  3, 6, 5,  8, 9, 7 },
                                { 3, 6, 5,  8, 9, 7,  2, 1, 4 },
                                { 8, 9, 7,  2, 1, 4,  3, 6, 5 },
                                
                                { 5, 3, 1,  6, 4, 2,  9, 7, 8 },
                                { 6, 4, 2,  9, 7, 8,  5, 3, 1 },
                                { 9, 7, 8,  5, 3, 1,  6, 4, 2 }};

    assert(isSolved(grid1) == true);
    printf("IsSolved Test : Solved == True \033[0;32mDone\n\033[0m");

    // Not solved : on 0
    unsigned int grid2[9][9] = {{ 1, 2, 3,  4, 5, 6,  7, 8, 9 },
                               { 4, 5, 0,  7, 8, 9,  1, 2, 3 },
                               { 7, 8, 9,  1, 2, 3,  4, 5, 6 },
                               
                               { 2, 1, 4,  3, 6, 5,  8, 9, 7 },
                               { 3, 6, 5,  8, 9, 7,  2, 1, 4 },
                               { 8, 9, 7,  2, 1, 4,  3, 6, 5 },
                                
                               { 5, 3, 1,  6, 4, 2,  9, 7, 8 },
                               { 6, 4, 2,  9, 7, 8,  5, 3, 1 },
                               { 9, 7, 8,  5, 3, 1,  6, 4, 2 }};

    assert(isSolved(grid2) == false);
    printf("IsSolved Test : Not Solved 1 == False \033[0;32mDone\n\033[0m");

    // Not solved : two same number
    unsigned int grid3[9][9] = {{ 1, 2, 3,  4, 5, 6,  7, 8, 9 },
                               { 4, 5, 2,  7, 8, 9,  1, 2, 3 },
                               { 7, 8, 9,  1, 2, 3,  4, 5, 6 },
                               
                               { 2, 1, 4,  3, 6, 5,  8, 9, 7 },
                               { 3, 6, 5,  8, 9, 7,  2, 1, 4 },
                               { 8, 9, 7,  2, 1, 4,  3, 6, 5 },
                                
                               { 5, 3, 1,  6, 4, 2,  9, 7, 8 },
                               { 6, 4, 2,  9, 7, 8,  5, 3, 1 },
                               { 9, 7, 8,  5, 3, 1,  6, 4, 2 }};

    assert(isSolved(grid3) == false);
    printf("IsSolved Test : Not Solved 2 == False \033[0;32mDone\n\033[0m");

    return 0;
}