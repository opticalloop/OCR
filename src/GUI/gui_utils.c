#include "GUI/gui_utils.h"

double bilinearly_interpolateSurface(unsigned int top, unsigned int left,
                                     double horizontal_position,
                                     double vertical_position, double top_left,
                                     double top_right, double bottom_left,
                                     double bottom_right)
{
    // Figure out "how far" the output pixel being considered is
    // between *_left and *_right.
    double horizontal_progress = horizontal_position - (double)left;
    double vertical_progress = vertical_position - (double)top;

    // Combine top_left and top_right into one large, horizontal
    // block.
    double top_block = top_left + horizontal_progress * (top_right - top_left);

    // Combine bottom_left and bottom_right into one large, horizontal
    // block.
    double bottom_block =
        bottom_left + horizontal_progress * (bottom_right - bottom_left);

    // Combine the top_block and bottom_block using vertical
    // interpolation and return as the resulting pixel.
    return top_block + vertical_progress * (bottom_block - top_block);
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
                    top, left, newX, newY, r1, r2, r3, r4);

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

void selectionFilter(SDL_Surface *surface, SDL_Rect *selection)
{
    const unsigned int width = surface->w;
    const unsigned int height = surface->h;
    const unsigned int rect_width = selection->w;
    const unsigned int rect_height = selection->h;
    const unsigned int x = selection->x;
    const unsigned int y = selection->y;

    Uint32 pixel;
    Uint8 r, g, b;
    for (unsigned int i = 0; i < width; i++)
    {
        for (unsigned int j = 0; j < height; j++)
        {
            if (i < x || i > x + rect_width || j < y || j > y + rect_height)
            {
                // Make pixel darker
                pixel = get_pixel(surface, i, j);
                SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                pixel = SDL_MapRGB(surface->format, r / 2, g / 2, b / 2);
                put_pixel(surface, i, j, pixel);
            }
        }
    }
}

void cropSurface(SDL_Surface *surface, SDL_Rect *selection)
{
    SDL_Surface *tempSurface =
        SDL_CreateRGBSurface(0, selection->w, selection->h, 32, 0, 0, 0, 0);
    SDL_BlitSurface(tempSurface, NULL, surface, NULL);
    SDL_BlitSurface(surface, selection, tempSurface, NULL);
    SDL_FreeSurface(tempSurface);
}