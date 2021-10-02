//
// Created by dov on 11/09/2021.
//

#ifndef OCR_NOISE_REDUCTION_H
#define OCR_NOISE_REDUCTION_H

#include <stdlib.h>
#include "../Utils/image.h"
#include "../Utils/array_sort.h"

void Preprocessing(Image *image);


unsigned int AverageFilter(Pixel *matrix, float *binomialFilter);

unsigned int MedianFilter(Pixel *matrix);

unsigned int ConstrastFilter(Pixel pixel, unsigned int *histogram);

unsigned int *GetHistogram(Pixel **pixels, unsigned int w, unsigned h);
void OtsuFilter(Pixel **pixels, unsigned int w, unsigned int h, unsigned int *histogram);
#endif //OCR_NOISE_REDUCTION_H