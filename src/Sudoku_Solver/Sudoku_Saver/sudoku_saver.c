#include "Sudoku_Solver/Sudoku_Saver/sudoku_saver.h"

void copyArray(unsigned int **grid, unsigned int **destination,
               unsigned int dimension)
{
    for (unsigned int i = 0; i < dimension; i++)
    {
        for (unsigned int j = 0; j < dimension; j++)
        {
            destination[i][j] = grid[i][j];
        }
    }
}

void basicPrint(unsigned int **grid, unsigned int dimension)
{
    printf("\n");
    for (unsigned int i = 0; i < dimension; i++)
    {
        if (i % ((unsigned int)sqrt(dimension)) == 0 && i != 0)
        {
            printf("\n");
        }

        for (unsigned int j = 0; j < dimension; j++)
        {
            if (j % ((unsigned int)sqrt(dimension)) == 0 && j != 0)
            {
                printf(" ");
            }
            if (grid[i][j] > 9)
            {
                printf("%d", grid[i][j]);
            }
            else
            {
                printf(" %d", grid[i][j]);
            }
        }

        printf("\n");
    }
}

void readGrid(unsigned int **grid, char inputPath[], int verbose,
              unsigned int dimension)
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
    unsigned int tempGrid[dimension * dimension];
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

    for (unsigned int i = 0; i < dimension; i++)
    {
        for (unsigned int j = 0; j < dimension; j++)
        {
            grid[i][j] = tempGrid[i * dimension + j];
        }
    }

    fclose(fp);
}

void saveGrid(unsigned int **grid, char outputPath[], int verbose,
              unsigned int dimension)
{
    FILE *f = fopen(outputPath, "w");
    if (f == NULL)
    {
        errx(EXIT_FAILURE, "Error opening file!\n");
    }

    if (verbose)
    {
        printf("<-- 💾 Saving grid to %s\n", outputPath);
    }

    for (unsigned int i = 0; i < dimension; i++)
    {
        for (unsigned int j = 0; j < dimension; j++)
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

Image createSudokuImage(unsigned int **grid, unsigned int **copy,
                        char *folder_path, unsigned int dimension)
{
    Image image = newImage(NULL, 0, 266, 266);

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
                image.pixels[x][y].r = 0;
                image.pixels[x][y].g = 0;
                image.pixels[x][y].b = 0;
            }
        }
    }

    // Coordonates
    unsigned int Array[9] = { 2, 31, 60, 90, 119, 148, 178, 207, 236 };
    unsigned int val;

    // SDL_Rect to copy to the actual image
    SDL_Rect rect;
    rect.w = IMAGE_SIZE;
    rect.h = IMAGE_SIZE;
    Image temp;
    for (unsigned int i = 0; i < dimension; i++)
    {
        for (unsigned int j = 0; j < dimension; j++)
        {
            val = grid[i][j];
            if (val != 0)
            {
                rect.x = Array[i];
                rect.y = Array[j];

                // Get the image number and copy it
                if (!strcmp(folder_path, ""))
                    temp = getImage(val, IMAGE_DIRECTORY, copy[i][j]);
                else
                    temp = getImage(val, folder_path, copy[i][j]);
                pasteOnImage(&temp, &image, &rect);
                freeImage(&temp, 0);
            }
        }
    }

    return image;
}

Image createHexaSudokuImage(unsigned int **grid, unsigned int **copy,
                        char *folder_path)
{
    Image image = newImage(NULL, 0, 470, 470);

    for (unsigned int x = 0; x < 470; x++)
    {
        for (unsigned int y = 0; y < 470; y++)
        {
            if (x != 0 && x != 1 && x != 355 && x != 356 && x != 30 
                 && x != 59 && x != 88 && x != 117 && x != 118 && x != 147 
                 && x != 176 && x != 205 && x != 234 && x != 235 && x != 264 
                 && x != 293 && x != 322 && x != 351 && x != 352 && x != 381 
                 && x != 410 && x != 439 && x != 468 && x != 469
                 && y != 0 && y != 1 && y != 355 && y != 356 && y != 30 
                 && y != 59 && y != 88 && y != 117 && y != 118 && y != 147
                 && y != 176 && y != 205 && y != 234 && y != 235 && y != 264
                 && y != 293 && y != 322 && y != 351 && y != 352 && y != 381
                 && y != 410 && y != 439 && y != 468 && y != 469)
            {
                image.pixels[x][y].r = 0;
                image.pixels[x][y].g = 0;
                image.pixels[x][y].b = 0;
            }
        }
    }

    // Coordonates
    unsigned int Array[16] = { 2, 31, 60, 89, 119, 148, 177, 206, 236, 265, 294, 323, 353, 382, 411, 440};
    unsigned int val;

    // SDL_Rect to copy to the actual image
    SDL_Rect rect;
    rect.w = IMAGE_SIZE;
    rect.h = IMAGE_SIZE;
    Image temp;
    for (unsigned int i = 0; i < 16; i++)
    {
        for (unsigned int j = 0; j < 16; j++)
        {
            val = grid[i][j];
            if (val != 0)
            {
                rect.x = Array[i];
                rect.y = Array[j];

                // Get the image number and copy it
                if (!strcmp(folder_path, ""))
                    temp = getImage(val, IMAGE_DIRECTORY, copy[i][j]);
                else
                    temp = getImage(val, folder_path, copy[i][j]);
                pasteOnImage(&temp, &image, &rect);
                freeImage(&temp, 0);
            }
        }
    }

    return image;
}

Image getImage(unsigned int val, char *directory, unsigned int green)
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
    Image res = newImage(surface, 0, 28, 28);
    SDL_FreeSurface(surface);
    return res;
}