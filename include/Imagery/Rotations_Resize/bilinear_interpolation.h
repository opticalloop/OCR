#ifndef BILINEAR_INTERPOLATION_H
#define BILINEAR_INTERPOLATION_H

#include "Imagery/Utils/image.h"

double bilinearly_interpolate(unsigned int top, unsigned int bottom,
                              unsigned int left, unsigned int right,
                              double horizontal_position,
                              double vertical_position, Pixel **pixels);

#endif