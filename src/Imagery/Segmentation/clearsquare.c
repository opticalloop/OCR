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

    int cor[56];

    for (unsigned int i = 0; i < 56; i++)
    {
        cor[i] = -1;
    }

    unsigned int iall = 0;
    for (unsigned int x = 0; x < width; x++, iall++)
    {
        if (isColumnPartialBlack(image, x))
        {
            cor[iall] = x;
        }
    }
    for (unsigned int y = 0; y < height; y++, iall++)
    {
        if (isLinePartialBlack(image, y))
        {
            cor[iall] = y;
        }
    }

    iall = 0;

    for (unsigned int i = 0; i < 56; i++, iall++)
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
}