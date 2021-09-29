#include <err.h>
#include "resize.h"

void resizeImage(Image *image, unsigned int newWidth, unsigned int newHeight)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    const double diffX = width / newWidth;
    const double diffY = height / newHeight;

    unsigned int x1 = 0;
    unsigned int x2 = 1;
    unsigned int y1 = 0;
    unsigned int y2 = 1;

    unsigned int color = 0;
    unsigned int averageColor = 0;

    Pixel** results = malloc(sizeof(Pixel *) * (newWidth + 1));
    if (results == NULL)
    {
        errx(1, "Error while allocating memory");
    }

    unsigned int i = 0;
    for (; i < newWidth; i++)
    {
        results[i] = malloc(sizeof(Pixel) * (newHeight + 1));
        if (results[i] == NULL)
        {
            errx(1, "Error while allocating memory");
        }
    }
    results[i] = NULL;

    for (unsigned int x = 0; x < newWidth; x++)
    {
        for (unsigned int y = 0; y < newHeight; y++)
        {
            printf("x1 : %u, y1 : %u\n", x1, y1);
            color = image->pixels[x1][y1].r * (x2 - x1) * (y2 - y1);
            color += image->pixels[x1][y2].r * (x2 - x1) * (y1 - y2 + 1);
            color += image->pixels[x2][y1].r * (x1 - x2 + 1) * (y2 - y1);
            color += image->pixels[x2][y2].r * (x1 - x2 + 1) * (y1 - y2 + 1);

            results[x][y].r = color;
            results[x][y].g = color;
            results[x][y].b = color;

            averageColor += color;
            x1 += diffX;

            if (x1 > x2)
            {
                x2 = x1 + 1;
                if (x2 >= width)
                {
                    x2--;
                }
            }
        }
        y1 += diffY;
        x1 = 0;
        x2 = 1;
        if (y1 > y2)
        {
            y2 = y1 + 1;
            if (y2 >= height)
            {
                y2--;
            }
        }
    }

    // Free previosu pixels
    for (unsigned int i = 0; i < width; i++)
    {
        free(image->pixels[i]);
    }
    free(image->pixels);
    image->pixels = results;
    image->width = newWidth;
    image->height = newHeight;
    image->averageColor = averageColor / (newWidth * newHeight);
    SDL_FreeSurface(image->surface);
    image->surface
        = SDL_CreateRGBSurface(0, newWidth, newHeight, 32, 0, 0, 0, 0);
    updateSurface(image);
}
