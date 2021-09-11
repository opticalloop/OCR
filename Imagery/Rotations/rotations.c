#include "rotations.h"

#include <math.h>
#include "../op/op.h"
#include "../Pixel_operations/pixel_operations.h"

void rotate(SDL_Surface *image, SDL_Surface *screen_surface, float angle)
{
    screen_surface = display_image(image);

    unsigned int width = image->w;
    unsigned int height = image->h;

    unsigned int middleX = width / 2;
    unsigned int middleY = height / 2;


    Uint32 pixel;
    unsigned int newX;
    unsigned int newY;
    for (unsigned int x = 0; x < width; x++){
        for (unsigned int y = 0; y < height; y++){
            pixel = get_pixel(image, x, y);
            newX = cos(angle) * (x - middleX) + cos(angle) * (y - middleY);
            newY = sin(angle) * (x - middleX) + sin(angle) * (y - middleY);;
            put_pixel(image, newX, newY, pixel);
        }
    }

    
    update_surface(screen_surface, image);

    SDL_FreeSurface(image);
    SDL_FreeSurface(screen_surface);
}