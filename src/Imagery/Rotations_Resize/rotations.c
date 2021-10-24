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

double detectDiffAngle(Image *image, float precision)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    // Get diagonal
    const unsigned int diagonal =
        (unsigned int)sqrt(width * width + height * height);

    double **accumulator;

    double _precision = M_PI * (1 / precision);

    accumulator = malloc(sizeof(double *) * (_precision + 1));
    if (accumulator == NULL)
    {
        errx(EXIT_FAILURE, "Error");
    }

    unsigned int i = 0;
    for (; i < _precision; i++)
    {
        accumulator[i] = malloc(sizeof(double) * (diagonal + 1));
        if (accumulator[i] == NULL)
        {
            errx(EXIT_FAILURE, "Error");
        }
    }

    double maxAngle = 0.0;
    double tempMaxAngle = 0.0;
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            // Only when encounter black pixel try the circle around
            if (image->pixels[x][y].r != 0)
            {
                continue;
            }

            // From 0 to 180
            double angle = 0;
            for (; angle <= M_PI / 2; angle += precision)
            {
                int diff = (int)((x * sin(angle) + y * cos(angle)));
                if (diff >= 0)
                {
                    int teta = (int)((angle * (1 / precision)
                                      + (M_PI / 2) * (1 / precision)));
                    accumulator[teta][diff]++;

                    if (accumulator[teta][diff] > tempMaxAngle)
                    {
                        tempMaxAngle = accumulator[teta][diff];
                        maxAngle = angle;
                    }
                }
            }
        }
    }
    // Free memory
    for (unsigned int i = 0; i < _precision; i++)
    {
        free(accumulator[i]);
    }
    free(accumulator);

    return maxAngle;
}

void autoRotate2(Image *image, float precision)
{
    double angle = detectDiffAngle(image, precision);
    angle *= (180 / M_PI);
    printf("Max angle %f\n", angle);

    if (fabs(angle) <= 0.1)
    {
        return;
    }

    rotate(image, angle);
}