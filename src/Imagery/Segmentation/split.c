#include "Imagery/Segmentation/split.h"

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

void split(Image *image, Image seg[], int save, char *imagename, int hexa)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    const unsigned int xincrem = !hexa ? width / 9 : width / 16;
    const unsigned int yincrem = !hexa ? height / 9 : height / 16;

    const unsigned int nbblock = !hexa ? 81 : 256;

    SDL_Rect block;
    unsigned int iall = 0;
    for (unsigned int y = 0; y < height && iall < nbblock; y += yincrem)
    {
        for (unsigned int x = 0; x < width && iall < nbblock; x += xincrem, iall++)
        {
            if (y + yincrem <= height && x + xincrem <= width)
            {
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