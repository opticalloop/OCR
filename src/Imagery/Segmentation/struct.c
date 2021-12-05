#include "Imagery/Segmentation/struct.h"

int **initMatriceInt(int x, int y)
{
    int **matrice = NULL;
    matrice = calloc(y + 1, sizeof(int *));
    if (matrice == NULL)
    {
        errx(1, "Memory error");
    }
    for (int j = 0; j < y; j++)
    {
        matrice[j] = calloc(x + 1, sizeof(int));
        if (matrice[j] == NULL)
        {
            errx(1, "Memory error");
        }
    }
    return matrice;
}

void freeMatriceInt(int **matrice, int height)
{
    for (int y = 0; y < height; y++)
    {
        free(matrice[y]);
    }
    free(matrice);
}