#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sudoku_solver.h"
#include "Sudoku_Solvable/sudoku_solvable.h"
#include "Sudoku_Printer/sudoku_printer.h"

#define true 1
#define false 0

void readGrid(unsigned int grid[9][9], char inputGrid[]){
    
    FILE *fp;

    fp = fopen(inputGrid, "r");
    
    if (fp == NULL)
    {
        printf("File doesn't exist.\n");
        return;
    }

    printf("%s\n", inputGrid);

    char ch;
    
    while((ch = fgetc(fp)) != EOF)
    {
        //putchar(ch);
        unsigned int xIndex = 0;
        unsigned int yIndex = 0;

        for (int i = 0; i < 11; i++){
            for (int j = 0; j < 11; j++){
                ch = inputGrid[j + i]; 
                
                printf("%c", ch);
                if (ch == '\n'){
                    break;
                }

                if (ch != ' '){
                    yIndex++;
                }
                
                if (ch != '.' && ch != ' '){
                    grid[xIndex][yIndex] = ch - '0';
                }
            }
            xIndex++;
        }
    }
    fclose(fp);
}

int main(int argc, char **argv)
{
    if (argc != 2){
        printf("Only one argument is required");
        return 1;
    }

    unsigned int grid[9][9] = {{ 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                                
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 },
                               { 0, 0, 0,  0, 0, 0,  0, 0, 0 }};

    readGrid(grid, argv[1]);

    return 0;
    
    if (isSolvable(grid) == false){
        printf("No solution exists\n");
        return 1;
    }

    printf("Start solving\n");
    solveSuduko(grid, 0, 0);
    print(grid);
    
    return 0;

}