#ifndef SOBEL_H
#define SOBEL_H

#include <math.h>

#include "Imagery/Utils/image.h"

void SobelEdgeDetection(Image *image);
double Convolution(Image *image, double kernel[3][3], int row, int col);

#endif