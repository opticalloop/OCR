#include "resize.h" 

#include "../Pixel_operations/pixel_operations.h"
#include "../op/op.h"

void (SDL_Surface *image, SDL_Surface *screen_surface, unsigned int newWidth, unsigned int newHeight)
{
    
    Uint32 pixel;
    unsigned int width = image->w;
    unsigned int height = image->h;

    for (unsigned int x = 0; x < width; x++){
        for (unsigned int y = 0; y = height; y++){
            pixel = get_pixel(image, x, y);
        }
    }

}