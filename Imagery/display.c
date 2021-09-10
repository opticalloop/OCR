#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "op/op.h"

void display(void)
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image("my_image.jpg");
    screen_surface = load_image("my_image.jpg");

    display_image(image_surface);

    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
}

int main(void){
    display();
    return 0;
}