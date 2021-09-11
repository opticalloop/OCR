#include "display.h"

#include <err.h>
#include "op/op.h"

//image_surface 
void display(SDL_Surface *image_surface, SDL_Surface *screen_surface)
{
    init_sdl();

    screen_surface = display_image(image_surface);

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
}