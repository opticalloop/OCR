#include "Imagery/Color_Treatment/grayscale.h"

void grayscale(Image *image)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    Pixel pixel;
    double average;
    // For each pixel of the image
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            // get the pixel
            pixel = image->pixels[x][y];

            // Compute the average of the pixel
            average = 0.3 * pixel.r + 0.59 * pixel.g + 0.11 * pixel.b;
            image->pixels[x][y].r = average;
            image->pixels[x][y].g = average;
            image->pixels[x][y].b = average;
        }
    }
}
