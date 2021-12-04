#include "Imagery/Detection/struct.h"

unsigned int **initMatrice(unsigned int x, unsigned int y)
{
    unsigned int **matrice = NULL;
    matrice = calloc(y + 1, sizeof(unsigned int *));
    if (matrice == NULL)
    {
        errx(1, "Memory error");
    }
    for (size_t j = 0; j < y; j++)
    {
        matrice[j] = calloc(x + 1, sizeof(unsigned int));
        if (matrice[j] == NULL)
        {
            errx(1, "Memory error");
        }
    }
    return matrice;
}

void freeMatrice(unsigned int **matrice, size_t height)
{
    for (size_t y = 0; y < height; y++)
    {
        free(matrice[y]);
    }
    free(matrice);
}

void setSquarePoint(Line *point, unsigned int x, unsigned int y)
{
    point->xStart = x;
    point->yStart = y;
}