#ifndef IMAGE_H
#define IMAGE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "op.h"
#include "pixel_operations.h"

typedef struct Pixel
{
    unsigned int r, g, b;
} Pixel;

typedef struct Image
{
    unsigned int width;
    unsigned int height;
    double averageColor;
    Pixel **pixels;
    char *path;
    SDL_Surface *surface;
} Image;

void newImage(Image *image);

void displayImage(Image *image);

void updateSurface(Image *image);

void saveImage(Image *image, char *path);

void freeImage(Image *image);

#endif