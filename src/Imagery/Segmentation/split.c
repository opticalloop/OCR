#include "Imagery/Segmentation/split.h"

void savesquare(Image *image, unsigned int iall, char *imagename, int hexa)
{
    char str[200];
    int dozen = iall / (hexa ? 16 : 9);
    int unit = iall % (hexa ? 16 : 9);

    snprintf(str, sizeof(str), "%s/3.%d_%d.bmp", imagename, dozen, unit);


void split(Image *image, Image seg[], int save, char *imagename, int hexa)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    const unsigned int xincrem = hexa ? width / 16 : width / 9;
    const unsigned int yincrem = hexa ? height / 16 : height / 9;

    const unsigned int nbblock = hexa ? 256 : 81;


    SDL_Rect block;
    unsigned int iall = 0;
    for (unsigned int y = 0; y < height && iall < nbblock; y += yincrem)
    {
        for (unsigned int x = 0; x < width && iall < nbblock;
             x += xincrem, iall++)
        {
            if (y + yincrem <= height && x + xincrem <= width)
            {
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