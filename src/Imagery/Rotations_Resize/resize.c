#include "Imagery/Rotations_Resize/resize.h"

Image resize(Image *image, unsigned int newwidth, unsigned int newheight,
             unsigned int matrix)
{
    unsigned int width = image->width;
    unsigned int height = image->height;

    double xscale = newwidth / (double)width;
    double yscale = newheight / (double)height;

    Image newimage;
    newimage.width = newwidth;
    newimage.height = newheight;
    newimage.path = "";
    newimage.surface = NULL;

    newImage(&newimage, matrix);

    for (unsigned int x = 0; x < newwidth; x++)
    {
        for (unsigned int y = 0; y < newheight; y++)
        {
            double oldx = x / xscale;
            double oldy = y / yscale;

            unsigned int top = floor(oldy);
            unsigned int bottom = top + 1;
            unsigned int left = floor(oldx);
            unsigned int right = left + 1;

            if (top < height && left < width && bottom < height
                && right < width)
            {
                double pixel = bilinearly_interpolate(
                    top, bottom, left, right, oldx, oldy, image->pixels);

                updatePixelToSameValue(&(newimage.pixels[x][y]), pixel);
            }
        }
    }

    freeImage(image, 0);
    return newimage;
}