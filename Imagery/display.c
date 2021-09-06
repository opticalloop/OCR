#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "op.h"

int main(void)
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    // TODO: Initialize the SDL
    init_sdl();

    image_surface = load_image("my_image.jpg");
    screen_surface = load_image("my_image.jpg");

    // TODO: Display the image.
    display_image(image_surface);

    // TODO: Wait for a key to be pressed.
    wait_for_keypressed();

    // TODO: Free the image surface.
    SDL_FreeSurface(image_surface);
    // TODO: Free the screen surface.
    SDL_FreeSurface(screen_surface);

    return 0;
}
