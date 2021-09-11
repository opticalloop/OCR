#ifndef IMAGE_H
#define IMAGE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct Pixel{
    unsigned int r, g, b;
}Pixel;

typedef struct Image{
    unsigned int width;
    unsigned int height;
    double averageColor;
    Pixel **pixels;
} Image;

void newImage(char* path, Image* image);

void freeImage(Image* image);

#endif