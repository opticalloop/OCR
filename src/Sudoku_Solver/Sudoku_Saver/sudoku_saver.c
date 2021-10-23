#include "Sudoku_Solver/Sudoku_Saver/sudoku_saver.h"

void copyArray(unsigned int grid[dim][dim], unsigned int destination[dim][dim])
{
    for (unsigned int i = 0; i < dim; i++)
    {
        for (unsigned int j = 0; j < dim; j++)
        {
            destination[i][j] = grid[i][j];
        }
    }
}

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

void readGrid(unsigned int grid[dim][dim], char inputPath[], int verbose)
{
    FILE *fp;

    fp = fopen(inputPath, "r");

    if (fp == NULL)
    {
        errx(EXIT_FAILURE, "Can't read the input file : File doesn't exist.");
        return;
    }

    if (verbose)
    {
        printf("--> 📂 Reading %s\n", inputPath);
    }

    char ch = 0;
    unsigned int tempGrid[dim * dim];
    unsigned int index = 0;

    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '.')
        {
            tempGrid[index] = 0;
        }
        else if (ch > '0' && ch <= '9')
        {
            tempGrid[index] = ch - '0';
        }
        else if (ch != '\n' && ch != '\0' && ch != ' ')
        {
            errx(EXIT_FAILURE, "File doesn't respect the format");
        }
        else
        {
            continue;
        }
        index++;
    }

    for (unsigned int i = 0; i < dim; i++)
    {
        for (unsigned int j = 0; j < dim; j++)
        {
            grid[i][j] = tempGrid[i * dim + j];
        }
    }

    fclose(fp);
}

void saveGrid(unsigned int grid[dim][dim], char outputPath[], int verbose)
{
    FILE *f = fopen(outputPath, "w");
    if (f == NULL)
    {
        errx(EXIT_FAILURE, "Error opening file!\n");
    }

    if (verbose)
    {
        printf("<-- 📂 Saving grid to ");
        printf("%s", outputPath);
        printf("\n");
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

Image createSudokuImage(unsigned int grid[dim][dim],
                        unsigned int copy[dim][dim])
{
    Image image;
    image.width = 266;
    image.height = 266;
    image.surface = NULL;
    image.path = ""; // To create an RGB surface
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

    // Update surface,
    updateSurface(&image);

    // Coordonates
    unsigned int Array[dim] = { 2, 31, 60, 90, 119, 148, 178, 207, 236 };
    unsigned int val;

    // SDL_Rect to copy to the actual image
    SDL_Rect rect;
    rect.w = IMAGE_SIZE;
    rect.h = IMAGE_SIZE;

    for (unsigned int i = 0; i < dim; i++)
    {
        for (unsigned int j = 0; j < dim; j++)
        {
            val = grid[i][j];
            if (val != 0)
            {
                rect.x = Array[j];
                rect.y = Array[i];

                // Get the image number and copy it
                SDL_Surface *surface =
                    getImage(val, IMAGE_DIRECTORY, copy[i][j]);
                SDL_BlitSurface(surface, NULL, image.surface, &rect);
                SDL_FreeSurface(surface);
            }
        }
    }

    return image;
}

SDL_Surface *getImage(unsigned int val, char *directory, unsigned int green)
{
    char str[1000];
    if (!green)
    {
        snprintf(str, sizeof(str), "%s/%u.jpg", directory, val);
    }
    else
    {
        snprintf(str, sizeof(str), "%s/%u_black.jpg", directory, val);
    }
    SDL_Surface *surface = load_image(str);
    return surface;
}