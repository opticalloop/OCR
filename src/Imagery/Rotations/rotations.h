#ifndef ROTATIONS_H
#define ROTATIONS_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "../Utils/image.h"

void rotate(Image *image, double angleDegree);

double detectDiffAngle(Image *image, float precision);

void autoRotate(Image *image, float precision);

#endif
