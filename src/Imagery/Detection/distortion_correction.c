#include "Imagery/Detection/distortion_correction.h"

void correctDistortion(Image *image)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    Image tempImage = copyImage(image, 0);

    newImage(&tempImage, 0, width, height);

    // for (unsigned int i = 0; i < width; i++)
    // {
    //     for (unsigned int j = 0; j < height; j++)
    //     {
    //         updatePixelToSameValue(&(image->pixels[i][j]), 255);
    //     }
    // }

    unsigned int x;
    unsigned int y;

    // unsigned int top;
    // unsigned int bottom;
    // unsigned int left;
    // unsigned int right;

    unsigned int xCenter = width - 50;
    unsigned int yCenter = height - 50;

    double factor1 = 0.0000005;
    double factor2 = 0.000009;

    for (unsigned int i = 0; i < width; i++)
    {
        for (unsigned int j = 0; j < height; j++)
        {
            double distance = sqrt((i - xCenter) * (i - xCenter)
                                   + (j - yCenter) * (j - yCenter));
            x = i
                + (2 * factor1 * i * j
                   + factor2 * (distance * distance + 2 * (i * i)));
            y = j
                + (factor1 * (distance * distance + 2 * (j * j))
                   + 2 * factor2 * i * j);

            x -= 55;

            if (i % 100 == 0)
            {
                // printf("New x : %u\n", x);
                // printf("New y : %u\n", y);
            }

            if (x < width && y < height)
            {
                image->pixels[x][y].r = tempImage.pixels[i][j].r;
                image->pixels[x][y].g = tempImage.pixels[i][j].g;
                image->pixels[x][y].b = tempImage.pixels[i][j].b;
            }
        }
    }

    // for (unsigned int i = 0; i < width; i++)
    // {
    //     for (unsigned int j = 0; j < height; j++)
    //     {
    //          // Get the four locations around pixels
    //         // floor() : Round at inferior
    //         top = floor(j);
    //         bottom = top + 1;
    //         left = floor(i);
    //         right = left + 1;

    //         // Check if any of the four locations are invalid. If so,
    //         // skip interpolating this pixel
    //         // Unsigned int : always > 0, so dont need to check if top and
    //         left
    //         // are superior to 0
    //         if (top < height && bottom < height && left < width
    //             && right < width)
    //         {
    //             unsigned int interpolated = bilinearly_interpolate(
    //                 top, bottom, left, right, i, j, image->pixels);
    //             updatePixelToSameValue(&(image->pixels[i][j]), interpolated);
    //         }
    //     }
    // }

    freeImage(&tempImage, 0);
}