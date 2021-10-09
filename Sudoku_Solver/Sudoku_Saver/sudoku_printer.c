#include <stdio.h>
#include "sudoku_printer.h"

#define dim 9

void basicPrint(unsigned int grid[dim][dim])
{
    for (unsigned int i = 0; i < 9; i++)
    {
        if (i % 3 == 0 && i != 0)
        {
            printf("\n");
        }

        for (unsigned int j = 0; j < 9; j++)
        {
            if (j % 3 == 0 && j != 0)
            {
                printf(" ");
            }
            printf("%d ", grid[i][j]);
        }

        printf("\n");
    }
}

Image createSudokuImage(unsigned int grid[dim][dim])
{
    Image image;
    image.width = 266;
    image.height = 266;
    image.path = ""; // To create an RGB surface
    image.averageColor = 0;
    image.surface = NULL;
    image.pixels = NULL;
    newImage(&image);

    //unsigned int temp_increment_x = 0;
    //unsigned int temp_increment_y = 0;
    for (unsigned int x = 0; x < 266; x++)
    {
        //temp_increment_x = (unsigned int)(x + temp_increment_x) / 87;
        for (unsigned int y = 0; y < 266; y++)
        {
            //temp_increment_y = (unsigned int)(y + temp_increment_y) / 87;
            if ((x != 0 && x != 1 && x != 264 && x != 265 && x != 30 && x !=
               59 && x != 88
                && x != 89 && x != 118 && x != 147 && x != 176 && x != 177
                && x != 206 && x != 235) &&
                (y != 0 && y != 1
                && y != 264 && y != 265
                && y != 30 && y != 59
                && y != 88 && y != 89
                && y != 118 && y != 147
                && y != 176 && y != 177
                && y != 206 && y != 235))
            //if ((x + temp_increment_x - 1) % 29 != 0 && x != 0)
            {
                image.pixels[x][y].r = 255;
                image.pixels[x][y].g = 255;
                image.pixels[x][y].b = 255;
            }
        }
    }

    saveImage(&image, "out.bmp");

    freeImage(&image);

    return image;
}