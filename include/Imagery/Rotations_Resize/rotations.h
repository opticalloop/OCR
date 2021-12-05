#ifndef ROTATIONS_H
#define ROTATIONS_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#include <math.h>
#include <stdio.h>

#include "Imagery/Rotations_Resize/bilinear_interpolation.h"
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/noise_reduction.h"

/**
 * @brief Rotate an image
 * @param Image The image to rotate
 * @param angleDegree The angle of rotation in degree
 * @return The rotated image
 */

void rotate(Image *image, double angleDegree);

#endif
