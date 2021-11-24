#ifndef ROTATIONS_H
#define ROTATIONS_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#include <math.h>
#include <stdio.h>

#include "Imagery/Rotations_Resize/bilinear_interpolation.h"
#include "Imagery/Utils/image.h"

/*
 * @brief Rotate an image
 * @param surface The image to rotate
 * @param angle The angle of rotation
 * @return The rotated image
 */

void rotate(Image *image, double angleDegree);

#endif
