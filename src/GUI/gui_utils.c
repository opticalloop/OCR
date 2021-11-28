#include "GUI/gui_utils.h"

void selectionFilter(Image *image, SDL_Rect *selection)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;
    const unsigned int rect_width = selection->w;
    const unsigned int rect_height = selection->h;
    const unsigned int x = selection->x;
    const unsigned int y = selection->y;

    for (unsigned int i = 0; i < width; i++)
    {
        for (unsigned int j = 0; j < height; j++)
        {
            if (i < x || i > x + rect_width || j < y || j > y + rect_height)
            {
                image->pixels[i][j].r = image->pixels[i][j].r / 2;
                image->pixels[i][j].g = image->pixels[i][j].g / 2;
                image->pixels[i][j].b = image->pixels[i][j].b / 2;
            }
        }
    }
}