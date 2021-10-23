#include "Imagery/Segmentation/split9.h"

void split9(Image *image, SDL_Surface *seg81[], int save, char *imagename)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    const unsigned int xincrem = width / 9;
    const unsigned int yincrem = height / 9;

    // printf("width: %d, height: %d, xincrem: %d, yincrem: %d\n", width,
    // height,
    //       xincrem, yincrem);

    char directory[200];
    snprintf(directory, sizeof(directory), "mkdir %s", imagename);
    if (!system(directory))
    {
        char delete[200];
        snprintf(delete, sizeof(delete), "rm -rf %s", imagename);
        system(delete);
        system(directory);
    }

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

                // printf("xstart: %d, ystart: %d, width: %d, height: %d\n",
                //       block.x, block.y, block.w, block.h);

                SDL_BlitSurface(image->surface, &block, surface, NULL);

                // display_image(surface);

                Image imagebis;
                imagebis.surface = surface;
                imagebis.width = block.w;
                imagebis.height = block.h;

                newImage(&imagebis);

                // imagebis free in resize
                Image imageresized = resize(&imagebis, 28, 28);

                clearsquare(&imageresized);
                // printf("iall: %d\n",iall);
                seg81[iall] = imageresized.surface;

                if (save)
                {
                    // printf("x: %d, y:%d, iall:%d\n",x,y,iall);
                    savesquare(&imageresized, iall, imagename);
                }

                // freeImage(&imagebis);
                for (unsigned int xbis = 0; xbis < 28; xbis++)
                {
                    for (unsigned int ybis = 0; ybis < 28; ybis++)
                    {
                        free(imageresized.pixels[xbis][ybis].matrix);
                    }
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