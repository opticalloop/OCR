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
} Image;

/**
 * @brief
 *
 * @param surface
 * @param matrix
 * @param width
 * @param height
 * @return Image
 */
Image newImage(SDL_Surface *surface, int matrix, unsigned int width,
               unsigned int height);

/**
 * @brief
 *
 * @param image
 */
void displayImage(Image *image);

/**
 * @brief Create a Surface object
 *
 * @param image
 * @return SDL_Surface*
 */
SDL_Surface *createSurface(Image *image);

/**
 * @brief
 *
 * @param image
 * @param path
 */
void saveImage(Image *image, char *path);

/**
 * @brief
 *
 * @param image
 * @param matrix
 */
void freeImage(Image *image, int matrix);

/**
 * @brief
 *
 * @param pixel
 * @param value
 */
void updatePixelToSameValue(Pixel *pixel, unsigned int value);

/**
 * @brief
 *
 * @return Pixel
 */
Pixel InstantiatePixelZero();

/**
 * @brief
 *
 * @param image
 * @param matrix
 * @return Pixel**
 */
Pixel **copyPixelsArray(Image *image, int matrix);

/**
 * @brief
 *
 * @param image
 * @param matrix
 * @return Image
 */
Image copyImage(Image *image, int matrix);

/**
 * @brief
 *
 * @param mask
 * @param w
 * @param h
 */
void freeMatrixArray(Pixel **mask, int w, int h);

/**
 * @brief
 *
 * @param image
 */
void updateNeigbourgs(Image *image);

/**
 * @brief
 *
 * @param image
 * @param rect
 * @return Image
 */
Image cropImage(Image *image, SDL_Rect *rect);

/**
 * @brief
 *
 * @param src
 * @param dest
 * @param rect
 */
void pasteOnImage(Image *src, Image *dest, SDL_Rect *rect);

#endif