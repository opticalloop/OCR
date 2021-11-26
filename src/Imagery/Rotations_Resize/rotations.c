#include "Imagery/Rotations_Resize/rotations.h"

void rotate(Image *image, double angleDegree)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    const double middleX = ((double)width / 2.0);
    const double middleY = ((double)height / 2.0);

    const double angle = angleDegree * M_PI / 180.0;

    // Create two dimensional array of pixels
    Pixel **_pixels = malloc(sizeof(Pixel *) * (width + 1));
    if (_pixels == NULL)
    {
        errx(EXIT_FAILURE, "Error while allocating memory");
    }

    unsigned int x = 0;
    for (; x < width; x++)
    {
        _pixels[x] = malloc(sizeof(Pixel) * (height + 1));
        if (_pixels[x] == NULL)
        {
            errx(EXIT_FAILURE, "Error while allocating memory");
        }
    }
    // '\0'
    _pixels[x] = NULL;

    // Copy of all pixel
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            // Consider that the image is in grayscale
            updatePixelToSameValue(&(_pixels[x][y]), image->pixels[x][y].r);
            updatePixelToSameValue(&(image->pixels[x][y]), 0);
        }
    }

    double newX;
    double newY;
    // Four pixels around
    unsigned int top;
    unsigned int bottom;
    unsigned int left;
    unsigned int right;
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            // Calculate new position
            newX = ((double)(cos(angle) * ((double)x - middleX)
                             - sin(angle) * ((double)y - middleY))
                    + middleX);
            newY = ((double)(cos(angle) * ((double)y - middleY)
                             + sin(angle) * ((double)x - middleX))
                    + middleY);

            // Get the four locations around pixels
            // floor() : Round at inferior
            top = floor(newY);
            bottom = top + 1;
            left = floor(newX);
            right = left + 1;

            // Check if any of the four locations are invalid. If so,
            // skip interpolating this pixel
            // Unsigned int : always > 0, so dont need to check if top and left
            // are superior to 0
            if (top < height && bottom < height && left < width
                && right < width)
            {
                unsigned int interpolated = bilinearly_interpolate(
                    top, bottom, left, right, newX, newY, _pixels);
                updatePixelToSameValue(&(image->pixels[x][y]), interpolated);
            }
        }
    }

    for (unsigned int i = 0; i < width; i++)
    {
        free(_pixels[i]);
    }
    free(_pixels);
}