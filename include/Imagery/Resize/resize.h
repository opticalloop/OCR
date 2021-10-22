#ifndef RESIZE_H
#define RESIZE_H

#include <math.h>
#include <stdio.h>

#include "Imagery/Utils/image.h"

/*
 *  Summary:
 *      Resize an image with the dimension wanted
 *      Usign the algorithme of bilinear interpolation
 *
 *  Params:
 *      *image: the image we want to resize
 *      width: the width of the output image
 *      height: the height of the output image
 *
 *  Return:
 *      Return a new image, with the goods dimensions.
 */

Image resize(Image *image, unsigned int width, unsigned int height);

#endif
