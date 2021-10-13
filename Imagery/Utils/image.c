#include "image.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>

void newImage(Image *image)
{
    SDL_Surface *surface = !strcmp(image->path, "")
        ? SDL_CreateRGBSurface(0, 266, 266, 32, 0, 0, 0, 0)
        : load_image(image->path);

    const unsigned int width = surface->w;
    const unsigned int height = surface->h;

    image->width = width;
    image->height = height;

    image->pixels = malloc((width + 1) * sizeof(Pixel *));

    if (image->pixels == NULL)
    {
        errx(EXIT_FAILURE,
             "Error while allocating pixels pointers for the image");
    }

    unsigned int x;
    for (x = 0; x < width; x++)
    {
        image->pixels[x] = malloc((height + 1) * sizeof(Pixel));
        if (image->pixels[x] == NULL)
        {
            errx(EXIT_FAILURE,
                 "Error while allocating pixels pointers for the image");
        }
    }
    // Make sure we don't have the '\0'
    image->pixels[x] = NULL;

    SDL_Color rgb;
    Uint32 pixel;
    double averageColor = 0;

    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            // Get pixel from surface
            pixel = get_pixel(surface, x, y);

            // Get RGB values from pixel
            SDL_GetRGB(pixel, surface->format, &rgb.r, &rgb.g, &rgb.b);

            image->pixels[x][y].r = rgb.r;
            image->pixels[x][y].g = rgb.g;
            image->pixels[x][y].b = rgb.b;

            averageColor += ((rgb.r + rgb.g + rgb.b) / 3);
        }
    }
    averageColor /= (width * height);
    image->averageColor = averageColor;
    image->surface = surface;
}

void displayImage(Image *image)
{
    // Init SDL (malloc inside so need to free at the end)
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(EXIT_FAILURE, "Could not initialize SDL: %s.\n", SDL_GetError());

    // Display img on screen
    display_image(image->surface);

    wait_for_keypressed();

    // Free memory took by SDL
    SDL_Quit();
}

void updateSurface(Image *image)
{
    // Get pixel format for the given image
    SDL_PixelFormat *pixel_format = image->surface->format;

    const unsigned int width = image->width;
    const unsigned int height = image->height;

    // For each pixel in the source image
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            // Get pixel from image
            Pixel _pixel = image->pixels[x][y];

            // Get pixel value for SDL
            Uint32 pixel =
                SDL_MapRGB(pixel_format, _pixel.r, _pixel.g, _pixel.b);

            // Put pixel in img
            put_pixel(image->surface, x, y, pixel);
        }
    }
}

void saveImage(Image *image, char *path)
{
    // Init SDL (malloc inside so need to free at the end)
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(EXIT_FAILURE, "Could not initialize SDL: %s.\n", SDL_GetError());

    // Update SDL_Surface inside Image struct
    updateSurface(image);

    if (SDL_SaveBMP(image->surface, path) != 0)
    {
        errx(EXIT_FAILURE, "Error while saving file");
    }

    SDL_Quit();
}

void freeImage(Image *image)
{
    unsigned int width = image->width;

    for (unsigned int x = 0; x < width; x++)
    {
        free(image->pixels[x]);
    }

    free(image->pixels);
    SDL_FreeSurface(image->surface);
}