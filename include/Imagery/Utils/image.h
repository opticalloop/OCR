#ifndef IMAGE_H
#define IMAGE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Imagery/Utils/op.h"
#include "Imagery/Utils/pixel_operations.h"

typedef struct Pixel
{
    unsigned int r, g, b;
    struct Pixel *matrix;
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

/*
 *  Summary:
 *
 *  Params:
 *      *image:
 *
 *  Return:
 *
 */
void newImage(Image *image, int matrix);

/*
 *  Summary:
 *
 *  Params:
 *      *image:
 *
 *  Return:
 *
 */
void displayImage(Image *image);

/*
 *  Summary:
 *
 *  Params:
 *      *image:
 *
 *  Return:
 *
 */
void updateSurface(Image *image);

/*
 *  Summary:
 *
 *  Params:
 *      *image:
 *      **pixels:
 *
 *  Return:
 *
 */
void updateSurfacePixels(Image *image, Pixel **pixels);

/*
 *  Summary:
 *
 *  Params:
 *      *image:
 *      *path:
 *
 *  Return:
 *
 */
void saveImage(Image *image, char *path);

/*
 *  Summary:
 *
 *  Params:
 *      *image:
 *
 *  Return:
 *
 */
void freeImage(Image *image, int matrix);

/*
 *  Summary:
 *
 *  Params:
 *      *pixel:
 *      value:
 *
 *  Return:
 *
 */
void updatePixelToSameValue(Pixel *pixel, unsigned int value);

/*
 *  Summary:
 *
 *  Params:
 *
 *
 *  Return:
 *
 */
Pixel InstantiatePixelZero();

/*
 *  Summary:
 *
 *  Params:
 *      *image:
 *
 *  Return:
 *
 */
Pixel **copyPixelsArray(Image *image, int matrix);

Image copyImage(Image *image, int matrix, int pixel);

/*
 *  Summary:
 *
 *  Params:
 *      **mask:
 *      w:
 *      h:
 *
 *  Return:
 *
 */
void freeMatrixArray(Pixel **mask, int w, int h);

/*
 *  Summary:
 *
 *  Params:
 *      *image:
 *
 *  Return:
 *
 */
void updateNeigbourgs(Image *image);
#endif