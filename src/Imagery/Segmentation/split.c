#include "Imagery/Segmentation/split.h"

int isBlackLine(Image *image, unsigned int y)
{
    const unsigned int width = image->width;

    for (unsigned int x = 0; x < width; x++)
    {
        if (image->pixels[x][y].r != 0)
        {
            return 0;
        }
    }
    return 1;
}

/*
 *To test the square taken in the image
 */

void displayblock(Image *image, unsigned int xstart, unsigned int ystart,
                  unsigned int xend, unsigned int yend)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    unsigned int y = ystart;
    unsigned int x = xstart;
    if (xend < width && yend < height)
    {
        for (; ystart <= yend; ystart++)
        {
            for (unsigned int xstart = x; xstart <= xend; xstart++)
            {
                if (ystart == y || ystart == yend)
                {
                    image->pixels[xstart][ystart].r = 255;
                    image->pixels[xstart][ystart].g = 0;
                    image->pixels[xstart][ystart].b = 0;
                }
                else
                {
                    image->pixels[x][ystart].r = 255;
                    image->pixels[xend][ystart].r = 255;
                    image->pixels[x][ystart].g = 0;
                    image->pixels[xend][ystart].g = 0;
                    image->pixels[x][ystart].b = 0;
                    image->pixels[xend][ystart].b = 0;
                    break;
                }
            }
        }
    }
    else
    {
        printf("Out of bounds\n");
    }
}

void savesquare(Image *image, unsigned int iall, char *imagename)
{
    char str[200];
    int dozen = iall / 9;
    int unit = iall % 9;

    snprintf(str, sizeof(str), "%s/3_%d%d.bmp", imagename, dozen, unit);
    saveImage(image, str);
}

void freeList(SDL_Surface *surface[], unsigned int len)
{
    for (unsigned int i = 0; i < len; i++)
    {
        SDL_FreeSurface(surface[i]);
    }
}
