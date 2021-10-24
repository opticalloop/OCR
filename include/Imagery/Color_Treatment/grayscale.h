#ifndef GRAYSCALE_H
#define GRAYSCALE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Imagery/Utils/image.h"

/*
 * Summary:
 *       Transform a colored image to an grayscale image
 *
 * Params :
 *       *image: the image to be transformed
 *
 *  Return:
 *       Noting, the grayscale is applied to the image directly
 */

void grayscale(Image *image);

#endif
