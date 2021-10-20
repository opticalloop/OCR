#include "Imagery/Segmentation/reverse_color.h"

void reverse_color(Image *image)
{
    for (size_t y = 0; y < image->height; y++)
    {
        for (size_t x = 0; x < image->width; x++)
        {
            image->pixels[x][y].r = abs(255 - image->pixels[x][y].r);
            image->pixels[x][y].g = abs(255 - image->pixels[x][y].g);
            image->pixels[x][y].b = abs(255 - image->pixels[x][y].b);
        }
    }
}
