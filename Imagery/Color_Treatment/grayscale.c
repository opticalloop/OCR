#include "grayscale.h"

#include <err.h>
#include "../Pixel_operations/pixel_operations.h"
#include "../op/op.h"

void grayscale(SDL_Surface *image_surface, SDL_Surface *screen_surface)
{
    screen_surface = display_image(image_surface);

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

    update_surface(screen_surface,image_surface);

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
}