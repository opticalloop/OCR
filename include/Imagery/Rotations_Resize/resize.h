#ifndef RESIZE_H
#define RESIZE_H

#include <math.h>
#include <stdio.h>

#include "Imagery/Rotations_Resize/bilinear_interpolation.h"
#include "Imagery/Utils/image.h"

/**
 * @brief
 *
 * @param image
 * @param newwidth
 * @param newheight
 * @param matrix
 * @return Image
 */

Image resize(Image *image, unsigned int newwidth, unsigned int newheight,
             unsigned int matrix);

#endif
