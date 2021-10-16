#include "Imagery/Color_Treatment/grayscale.h"

void grayscale(Image *image)
{
    int width = image->width;
    int height = image->height;

    // For each pixel of the image
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            // get the pixel
            Pixel pixel = image->pixels[x][y];

            // Compute the average of the pixel
            float average = 0.3 * pixel.r + 0.59 * pixel.g + 0.11 * pixel.b;
            image->pixels[x][y].r = average;
            image->pixels[x][y].g = average;
            image->pixels[x][y].b = average;
        }
    }
}
