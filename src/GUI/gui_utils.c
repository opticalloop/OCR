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