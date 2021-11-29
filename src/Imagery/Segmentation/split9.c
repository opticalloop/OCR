#include "Imagery/Segmentation/split9.h"

void split9(Image *image, Image seg81[81], int save, char *imagename)
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
                seg81[iall] = imageresized;

                if (save)
                {
                    savesquare(&imageresized, iall, imagename);
                }
            }
            else
            {
                iall--;
            }
        }
    }
}
