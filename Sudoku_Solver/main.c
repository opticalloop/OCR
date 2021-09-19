#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Sudoku_Printer/sudoku_printer.h"
#include "Sudoku_Solvable/sudoku_solvable.h"
#include "sudoku_solver.h"

#define true 1
#define false 0

static void readGrid(unsigned int **grid, char inputPath[])
{

    FILE *fp;

    fp = fopen(inputPath, "r");

    if (fp == NULL)
    {
        printf("File doesn't exist.\n");
        return;
    }

    char ch;

    while ((ch = fgetc(fp)) != EOF)
    {
        // putchar(ch);
        unsigned int xIndex = 0;
        unsigned int yIndex = 0;

        for (int i = 0; i < 11; i++)
        {
            for (int j = 0; j < 11; j++)
            {
                ch = inputPath[j + i];

                if (ch == '\n')
                {
                    break;
                }

                if (ch != ' ')
                {
                    yIndex++;
                }

                if (ch != '.' && ch != ' ')
                {
                    grid[xIndex % 9][yIndex % dim] = ch - '0';
                }
            }
            xIndex++;
        }
    }
    fclose(fp);
}

static void saveGrid(unsigned int **grid, char outputPath[])
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

    /* printing single chatacters */
    char c = 'A';
    fprintf(f, "A character: %c\n", c);

    fclose(f);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        errx(1, "Usage : ./main input_path output_path");
    }

    // unsigned int **grid = malloc(sizeof(unsigned int *) * (dim + 1));

    // unsigned int i = 0;
    // for (; i < dim; i++)
    // {
    //     grid[i] = malloc(sizeof(unsigned int) * (dim + 1));
    // }
    // // '\0'
    // grid[i] = NULL;

    // readGrid(grid, argv[1]);

    // return 0;

    // if (isSolvable(grid) == false){
    //    errx(1, "Grid can't be solved\n");
    // }

    // printf("Start solving\n");
    // solveSuduko(grid, 0, 0);
    // print(grid);

    // // saveGrid(argv[2]);

    // for (i = 0; i < dim; i++)
    // {
    //     free(grid[i]);
    // }
    // free(grid);

    return 0;
}