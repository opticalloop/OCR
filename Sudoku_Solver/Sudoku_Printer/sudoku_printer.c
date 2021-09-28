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

    for (unsigned int x = 0; x < 266; x++)
    {
        for (unsigned int y = 0; y < 266; y++)
        {
            if (x != 0 && x != 1 && y != 0 && y != 1 && x != 264 && x != 265
                && y != 264 && y != 265 && x != 31 && x != 59 && x != 87
                && x != 88 && x != 116 && x != 144 && x != 172 && x != 173
                && x != 201 && x != 229)
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