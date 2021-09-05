#include <stdio.h>

void print(unsigned int grid[9][9])
{
    for (unsigned int i = 0; i < 9; i++){
        if (i % 3 == 0 && i != 0){
            printf("\n");
        }

        for (unsigned int j = 0; j < 9; j++){
            if (j % 3 == 0 && j != 0){
                printf(" ");
            }
            printf("%d ", grid[i][j]);
        }
        
        printf("\n");
    }
}