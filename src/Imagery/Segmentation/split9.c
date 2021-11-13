#include "Imagery/Segmentation/split9.h"

void split9(Image *image, SDL_Surface *seg81[81], int save, char *imagename)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    const unsigned int xincrem = width / 9;
    const unsigned int yincrem = height / 9;

    SDL_Rect block;
    unsigned int iall = 0;
    for (unsigned int y = 0; y < height && iall < 81; y += yincrem)
    {
        for (unsigned int x = 0; x < width && iall < 81; x += xincrem, iall++)
        {
            if (y + yincrem < height && x + xincrem < width)
            {
                displayblock(image, x, y, xincrem, yincrem);
                block.x = x;
                block.y = y;
                block.w = xincrem;
                block.h = yincrem;

                SDL_Surface *surface =
                    SDL_CreateRGBSurface(0, block.w, block.h, 24, 0, 0, 0, 0);

                SDL_BlitSurface(image->surface, &block, surface, NULL);

                Image imagebis;
                imagebis.surface = surface;
                imagebis.width = block.w;
                imagebis.height = block.h;

                newImage(&imagebis, 0);

                // imagebis free in resize
                Image imageresized = resize(&imagebis, 28, 28);

                clearsquare(&imageresized);

                seg81[iall] = imageresized.surface;

                if (save)
                {
                    savesquare(&imageresized, iall, imagename);
                }

                // freeImage(&imagebis);
                for (unsigned int xbis = 0; xbis < 28; xbis++)
                {
                    free(imageresized.pixels[xbis]);
                }

                free(imageresized.pixels);
            }
            else
            {
                iall--;
            }
        }
    }
}
