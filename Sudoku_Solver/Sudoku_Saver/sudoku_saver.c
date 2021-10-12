#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "sudoku_saver.h"

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

    FILE* fp;

    fp = fopen(inputPath, "r");

    if (fp == NULL)
    {
        errx(EXIT_FAILURE, "Read Grid : File doesn't exist.");
        return;
    }

    char ch;

    unsigned int xIndex = 0;
    unsigned int yIndex = 0;

    for (int i = 0; i < 11 && ch != EOF; i++)
    {
        for (int j = 0; j < 11 && ch != EOF; j++)
        {
            ch = fgetc(fp);

            printf("%c", ch);

            if (ch == '\n' || ch == '\0' || ch == ' ')
            {
                continue;
            }

            if (ch != '.')
            {
                grid[xIndex][yIndex] = ch - '0';
            }
            yIndex++;
        }
        yIndex = 0;
        if (i != 3 && i != 7)
        {
            xIndex++;
        }
    }
    fclose(fp);
}

void saveGrid(unsigned int grid[dim][dim], char outputPath[])
{
    FILE *f = fopen("file.txt", "w");
    if (f == NULL)
    {
        errx(1, "Error opening file!\n");
    }

    /* print some text */
    const char *text = "Write this to the file";
    fprintf(f, "Some text: %s\n", text);

    /* print integers and floats */
    int i = 1;
    float pi = 3.1415927;
    fprintf(f, "Integer: %d, float: %f\n", i, pi);

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