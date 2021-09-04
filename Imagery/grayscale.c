
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

//TODO: Insert all the above functions.
void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1, "Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface *load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

SDL_Surface *display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE | SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
             img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if (SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while (event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while (event.type != SDL_KEYUP);
}

void SDL_FreeSurface(SDL_Surface *surface);

int main()
{
    SDL_Surface *image_surface;
    SDL_Surface *screen_surface;

    // TODO: Initialize the SDL
    init_sdl();

    image_surface = load_image("my_image.jpg");
    screen_surface = load_image("my_image.jpg");

    // TODO: Display the image.
    display_image(image_surface);

    // TODO: Wait for a key to be pressed.
    wait_for_keypressed();

    int width = image_surface->w;
    int height = image_surface->h;

    //For each pixel of the image
    for(int x = 0; x < width; x++){
        //printf("the width is %d, and the x is: %d\n",width,x);
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