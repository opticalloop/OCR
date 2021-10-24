#ifndef BLACK_AND_WHITE_H
#define BLACK_AND_WHITE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Imagery/Utils/image.h"

/*
 * Summary:
 *       Transform a colored image to an black and white image
 *
 * Params :
 *       *image: the image to be transformed
 *
 *  Return:
 *       Noting the black and white is applied to the image directly
 */

void blackandwhite(Image *image);

#endif
