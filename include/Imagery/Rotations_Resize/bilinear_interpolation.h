#ifndef BILINEAR_INTERPOLATION_H
#define BILINEAR_INTERPOLATION_H

#include "Imagery/Utils/image.h"

/*
 *  Summary:
 *      Return a bilinearly interpolation based on the four pixel around one
 *
 *  Params:
 *      top : pixel above actual pixel
 *      bottom : pixel under actual pixel
 *      left : pixel at the left of the actual pixel
 *      right : pixel at the right of the actual pixel
 *      horizontal_position : x pos of the actual pixel
 *      vertical_position : y pos of the actual pixel
 *      **pixels : pixels of the image
 *
 *  Return:
 *      Return the new value of the pixel.
 */

double bilinearly_interpolate(unsigned int top, unsigned int bottom,
                              unsigned int left, unsigned int right,
                              double horizontal_position,
                              double vertical_position, Pixel **pixels);

#endif