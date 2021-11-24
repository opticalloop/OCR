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

void rotateSurface(SDL_Surface *surface, double angleDegree)
{
    const unsigned int width = surface->w;
    const unsigned int height = surface->h;

    const double middleX = ((double)width / 2.0);
    const double middleY = ((double)height / 2.0);

    const double angle = angleDegree * M_PI / 180.0;

    // Create temporary surface
    SDL_Surface *tempSurface =
        SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_BlitSurface(surface, NULL, tempSurface, NULL);

    double newX;
    double newY;
    // Four pixels around
    unsigned int top;
    unsigned int bottom;
    unsigned int left;
    unsigned int right;
    Uint32 pixel;
    Uint32 topLeft;
    Uint32 topRight;
    Uint32 bottomLeft;
    Uint32 bottomRight;
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
                // Get 4 corners
                topLeft = get_pixel(tempSurface, left, top);
                topRight = get_pixel(tempSurface, right, top);
                bottomLeft = get_pixel(tempSurface, left, bottom);
                bottomRight = get_pixel(tempSurface, right, bottom);

                // Get rgb values
                Uint8 r1, g1, b1;
                Uint8 r2, g2, b2;
                Uint8 r3, g3, b3;
                Uint8 r4, g4, b4;
                SDL_GetRGB(topLeft, tempSurface->format, &r1, &g1, &b1);
                SDL_GetRGB(topRight, tempSurface->format, &r2, &g2, &b2);
                SDL_GetRGB(bottomLeft, tempSurface->format, &r3, &g3, &b3);
                SDL_GetRGB(bottomRight, tempSurface->format, &r4, &g4, &b4);

                unsigned int interpolated = bilinearly_interpolateSurface(
                    top, bottom, left, right, newX, newY, r1, r2, r3, r4);

                // Put pixel
                pixel = SDL_MapRGB(surface->format, interpolated, interpolated,
                                   interpolated);
            }
            else
            {
                // Put black pixel
                pixel = SDL_MapRGB(surface->format, 0, 0, 0);
            }
            put_pixel(surface, x, y, pixel);
        }
    }

    SDL_FreeSurface(tempSurface);
}
