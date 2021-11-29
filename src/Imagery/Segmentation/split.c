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

void savesquare(Image *image, unsigned int iall, char *imagename, int hexa)
{
    if(hexa){
        char str[200];
        int dozen = iall / 9;
        int unit = iall % 9;

        snprintf(str, sizeof(str), "%s/%d%d.bmp", imagename, dozen, unit);
        saveImage(image, str);
    }else{
        char str[200];
        int dozen = iall / 16;
        int unit = iall % 16;

        snprintf(str, sizeof(str), "%s/%d_%d.bmp", imagename, dozen, unit);
        saveImage(image, str);
    }
}

void split(Image *image, Image seg[81], int save, char *imagename, int hexa)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    const unsigned int xincrem = hexa ? width / 9 : width / 16;
    const unsigned int yincrem = hexa ? height / 9 : height / 16;

    const unsigned int nbblock = hexa ? 81 : 256;

    SDL_Rect block;
    unsigned int iall = 0;
    for (unsigned int y = 0; y < height && iall < nbblock; y += yincrem)
    {
        for (unsigned int x = 0; x < width && iall < nbblock; x += xincrem, iall++)
        {
            if (y + yincrem <= height && x + xincrem <= width)
            {
                displayblock(image, x, y, xincrem, yincrem);
                block.x = x;
                block.y = y;
                block.w = xincrem;
                block.h = yincrem;

                Image imagebis = cropImage(image, &block);

                // imagebis free in resize
                Image imageresized = resize(&imagebis, 28, 28, 0);

                clearsquare(&imageresized);
                seg[iall] = imageresized;

                if (save)
                {
                    savesquare(&imageresized, iall, imagename, hexa);
                }
            }
            else
            {
                iall--;
            }
        }
    }
}
