#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Color_Treatment/grayscale.h"
#include "Color_Treatment/blackandwhite.h"
#include "Pixel_operations/pixel_operations.h"
#include "Rotations/rotations.h"
#include "op/op.h"
#include "display.h"

int main(void)
{   
    printf("Initing SDL_Surfaces");

    SDL_Surface *image_surface = NULL;
    SDL_Surface *screen_surface = NULL;

    printf("Waiting for a key to be pressed");

    wait_for_keypressed();

    // Display
    image_surface = load_image("my_image.jpg");
    display(image_surface, screen_surface);

    // Rotate
    wait_for_keypressed();
    rotate(image_surface, screen_surface, 45);
    

    return 0;
}