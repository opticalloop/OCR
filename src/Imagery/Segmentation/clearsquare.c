#include "Imagery/Segmentation/clearsquare.h"

int isLinePartialBlack(Image *image, unsigned int y)
{
    blackandwhite(image);
    unsigned int nbblack = 0;

    for (unsigned int x = 0; x < 28; x++)
    {
        if (image->pixels[x][y].r == 0)
            nbblack++;
    }

    if (nbblack == 0)
        return 1;
    return nbblack > 20 ? 1 : 0;
}

int isColumnPartialBlack(Image *image, unsigned int x)
{
    blackandwhite(image);
    unsigned int nbblack = 0;
    // displayImage(image);
    for (unsigned int y = 0; y < 28; y++)
    {
        if (image->pixels[x][y].r < 128)
            nbblack++;
    }

    if (nbblack == 0)
        return 1;
    return nbblack > 20 ? 1 : 0;
}

void clearLine(Image *image, unsigned int y)
{
    const unsigned int width = image->width;

    for (unsigned int x = 0; x < width; x++)
    {
        updatePixelToSameValue(&(image->pixels[x][y]), 255);
    }
}

void clearColumn(Image *image, unsigned int x)
{
    const unsigned int height = image->height;

    for (unsigned int y = 0; y < height; y++)
    {
        updatePixelToSameValue(&(image->pixels[x][y]), 255);
    }
}

void clearsquare(Image *image)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    int cor[56] = { -1 };

    unsigned int iall = 0;
    for (unsigned int x = 0; x < width && iall < 56; x++, iall++)
    {
        if (isColumnPartialBlack(image, x))
        {
            cor[iall] = x;
        }
    }
    for (unsigned int y = 0; y < height && iall < 56; y++, iall++)
    {
        if (isLinePartialBlack(image, y))
        {
            cor[iall] = y;
        }
    }

    iall = 0;

    for (unsigned int i = 0; i < 56 && iall < 56; i++, iall++)
    {
        if (cor[iall] != -1 && i < 28)
        {
            clearColumn(image, cor[iall]);
        }

        else if (cor[iall] != -1 && i >= 28)
        {
            clearLine(image, cor[iall]);
        }
    }
    clear_imperfections(image);
}

int __dfs(int **M, int x, int y, int **cur, Image *image)
{
    M[x][y] = 1;
    cur[x][y] = 1;
    unsigned int lengraph = 0;

    if (x - 1 >= 0 && x - 1 < 28 && y - 1 >= 0 && y - 1 < 28
        && M[x - 1][y - 1] == 0 && image->pixels[x - 1][y - 1].r == 0)
        lengraph = 1 + __dfs(M, x - 1, y - 1, cur, image);

    if (x >= 0 && x < 28 && y - 1 >= 0 && y < 28 && M[x][y - 1] == 0
        && image->pixels[x][y - 1].r == 0)
        lengraph = 1 + __dfs(M, x, y - 1, cur, image);

    if (x + 1 >= 0 && x + 1 < 28 && y - 1 >= 0 && y - 1 < 28
        && M[x + 1][y - 1] == 0 && image->pixels[x + 1][y - 1].r == 0)
        lengraph = 1 + __dfs(M, x + 1, y - 1, cur, image);

    if (x - 1 >= 0 && x - 1 < 28 && y >= 0 && y < 28 && M[x - 1][y] == 0
        && image->pixels[x - 1][y].r == 0)
        lengraph = 1 + __dfs(M, x - 1, y, cur, image);

    if (x + 1 > 0 && x + 1 < 28 && y >= 0 && y < 28 && M[x + 1][y] == 0
        && image->pixels[x + 1][y].r == 0)
        lengraph = 1 + __dfs(M, x + 1, y, cur, image);

    if (x - 1 >= 0 && x - 1 < 28 && y + 1 >= 0 && y + 1 < 28
        && M[x - 1][y + 1] == 0 && image->pixels[x - 1][y + 1].r == 0)
        lengraph = 1 + __dfs(M, x - 1, y + 1, cur, image);

    if (x >= 0 && x < 28 && y + 1 > 0 && y + 1 < 28 && M[x][y + 1] == 0
        && image->pixels[x][y + 1].r == 0)
        lengraph = 1 + __dfs(M, x, y + 1, cur, image);

    if (x + 1 > 0 && x + 1 < 28 && y + 1 >= 0 && y + 1 < 28
        && M[x + 1][y + 1] == 0 && image->pixels[x + 1][y + 1].r == 0)
        lengraph = 1 + __dfs(M, x + 1, y + 1, cur, image);

    return lengraph;
}

void clear_matrice(int **M, Image *image)
{
    for (int x = 0; x < 28; x++)
    {
        for (int y = 0; y < 28; y++)
        {
            if (M[x][y] != 1)
            {
                updatePixelToSameValue(&(image->pixels[x][y]), 255);
            }
        }
    }
}

void clear_imperfections(Image *image)
{
    int **M = initMatriceInt(28, 28);
    unsigned int maxlen = 0;
    // reset the current matrix
    for (unsigned int i = 0; i < 28; i++)
    {
        for (unsigned int j = 0; j < 28; j++)
        {
            M[i][j] = 0;
        }
    }

    // the matrix who will keep the current graph connexe
    int **cur = initMatriceInt(28, 28);
    // result matrix
    int **res = initMatriceInt(28, 28);

    for (unsigned int x = 0; x < 28; x++)
    {
        for (unsigned int y = 0; y < 28; y++)
        {
            // reset the current matrix
            for (unsigned int i = 0; i < 28; i++)
            {
                for (unsigned int j = 0; j < 28; j++)
                {
                    cur[i][j] = 0;
                }
            }
            if (M[x][y] == 0 && image->pixels[x][y].r == 0)
            {
                unsigned int len = __dfs(M, x, y, cur, image);
                if (len > maxlen)
                {
                    maxlen = len;
                    // copy the current graph in the result matrix
                    for (unsigned int i = 0; i < 28; i++)
                    {
                        for (unsigned int j = 0; j < 28; j++)
                        {
                            res[i][j] = cur[i][j];
                        }
                    }
                }
            }
        }
    }
    if (maxlen > 15)
    {
        clear_matrice(res, image);
    }
    else
    {
        for (int x = 0; x < 28; x++)
        {
            for (int y = 0; y < 28; y++)
            {
                updatePixelToSameValue(&(image->pixels[x][y]), 255);
            }
        }
    }
    freeMatriceInt(M, 28);
    freeMatriceInt(cur, 28);
    freeMatriceInt(res, 28);
}
