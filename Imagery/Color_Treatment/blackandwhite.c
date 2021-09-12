#include "blackandwhite.h"

#include <err.h>
#include "../Utils/pixel_operations.h"
#include "../Utils/op.h"

void blackandwhite(SDL_Surface *image_surface, SDL_Surface *screen_surface)
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
            
            //put the new pixel value on the surface

            if((r+g+b)/3 >= 127){
                Uint32 pixelfinal = SDL_MapRGB(image_surface->format,255,255,255);
                //put the new pixel value on the surface
                put_pixel(image_surface,x,y,pixelfinal);
            }
            else{
                Uint32 pixelfinal = SDL_MapRGB(image_surface->format,0,0,0);
                //put the new pixel value on the surface
                put_pixel(image_surface,x,y,pixelfinal);
            }
        }
    }

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
}