#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "pixel_operations/pixel_operations.h"
#include "op/op.h"

void blackandwhite(SDL_Surface *image_surface)
{
    SDL_Surface *screen_surface;

    init_sdl();

    screen_surface = display_image(image_surface);

    //wait_for_keypressed();

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

    update_surface(screen_surface,image_surface);
    //wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
}

int main(void){
    SDL_Surface *image_surface = load_image("my_image.jpg");
    blackandwhite(image_surface);
    return 0;
}