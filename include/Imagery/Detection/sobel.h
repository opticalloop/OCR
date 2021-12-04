#ifndef SOBEL_H
#define SOBEL_H

#include <math.h>

#include "Imagery/Utils/image.h"

/**
 * @brief
 *
 * @param image
 */
void SobelEdgeDetection(Image *image);

/**
 * @brief
 *
 * @param image
 * @param kernel
 * @param row
 * @param col
 * @return double
 */
double Convolution(Image *image, double kernel[3][3], int row, int col);

#endif