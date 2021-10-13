#include "sudoku_saver.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void basicPrint(unsigned int grid[dim][dim])
{
    printf("\n");
    for (unsigned int i = 0; i < dim; i++)
    {
        if (i % ((unsigned int)sqrt(dim)) == 0 && i != 0)
        {
            printf("\n");
        }

        for (unsigned int j = 0; j < dim; j++)
        {
            if (j % ((unsigned int)sqrt(dim)) == 0 && j != 0)
            {
                printf(" ");
            }
            printf("%d ", grid[i][j]);
        }

        printf("\n");
    }
}

void readGrid(unsigned int grid[dim][dim], char inputPath[])
{
    FILE *fp;

    fp = fopen(inputPath, "r");

    if (fp == NULL)
    {
        errx(EXIT_FAILURE, "Read Grid : File doesn't exist.");
        return;
    }

    char ch = 0;

    unsigned int xIndex = 0;
    unsigned int yIndex = 0;

    for (int i = 0; i < 12 && ch != EOF; i++)
    {
        for (int j = 0; j < 12 && (ch = fgetc(fp)) != EOF; j++)
        {
            if (ch == '\n' || ch == '\0' || ch == ' ')
            {
                if (i == 3 || i == 7)
                {
                    xIndex--; // Dont move, it's an empty line
                    break;
                }
                continue;
            }

            if (ch != '.')
            {
                grid[xIndex][yIndex] = ch - '0';
            }
            yIndex++;
        }
        yIndex = 0;
        xIndex++;
    }
    fclose(fp);
}

void saveGrid(unsigned int grid[dim][dim], char outputPath[])
{
    FILE *f = fopen(outputPath, "w");
    if (f == NULL)
    {
        errx(EXIT_FAILURE, "Error opening file!\n");
    }

    for (unsigned int i = 0; i < dim; i++)
    {
        for (unsigned int j = 0; j < dim; j++)
        {
            if (grid[i][j] == 0)
            {
                fprintf(f, "%c", '.');
            }
            else
            {
                fprintf(f, "%d", grid[i][j]);
            }
            if (j == 2 || j == 5)
            {
                fprintf(f, " ");
            }
        }
        if (i != 8)
        {
            fprintf(f, "\n");
        }
        if (i == 2 || i == 5)
        {
            fprintf(f, "\n");
        }
    }
    fclose(f);
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
            if ((x != 0 && x != 1 && x != 264 && x != 265 && x != 30 && x != 59
                 && x != 88 && x != 89 && x != 118 && x != 147 && x != 176
                 && x != 177 && x != 206 && x != 235)
                && (y != 0 && y != 1 && y != 264 && y != 265 && y != 30
                    && y != 59 && y != 88 && y != 89 && y != 118 && y != 147
                    && y != 176 && y != 177 && y != 206 && y != 235))
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