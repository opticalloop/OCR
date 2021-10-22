#ifndef ROTATIONS_H
#define ROTATIONS_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#include <math.h>
#include <stdio.h>

#include "Imagery/Utils/image.h"

/*
 *  Summary:
 *       left rotation with bilinear interpolation
 *
 *  Params:
 *      *image: the image to rotate
 *      angleDegree: the degree to rotate
 *
 *  Return:
 *      Nothing, the rotation is applied to the image directly
 */

void rotate(Image *image, double angleDegree);

double detectDiffAngle(Image *image, float precision);

void autoRotate2(Image *image, float precision);

#endif
