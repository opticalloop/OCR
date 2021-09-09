#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "pixel_operations.h"
#include "op.h"

int main()
{
    SDL_Surface *image_surface;
    SDL_Surface *screen_surface;

    // TODO: Initialize the SDL
    init_sdl();

    image_surface = load_image("my_image.jpg");
    screen_surface = display_image(image_surface);

    // TODO: Wait for a key to be pressed.
    wait_for_keypressed();

    int width = image_surface->w;
    int height = image_surface->h;

    //For each pixel of the image
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height;y++){
            //get the pixel value
            Uint32 pixel = get_pixel(image_surface,x,y);
            
            //getting the RGB value of the pixel
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format,&r,&g,&b);
            
            //Compute the average of the pixel
            Uint8 average = 0.3*r + 0.59*g + 0.11*b;

            //Update the RGB value by using the average r = g = b = average
            r = g = b = average;
            //printf("average : %d, r = %d; g = %d; b = %d",average,r,g,b);

            //put the new pixel value on the surface
            Uint32 pixelfinal = SDL_MapRGB(image_surface->format,r,g,b);
            
            //put the new pixel value on the surface
            put_pixel(image_surface,x,y,pixelfinal);
        }
    }

    //TODO: Redraw the surface
    update_surface(screen_surface,image_surface);
    printf("update done");
    //TODO: Wait for a key to be pressed
    wait_for_keypressed();

    //TODO: Free the surface
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

    return 0;
}