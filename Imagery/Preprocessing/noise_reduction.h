//
// Created by dov on 11/09/2021.
//

#ifndef OCR_NOISE_REDUCTION_H
#define OCR_NOISE_REDUCTION_H

#include <stdlib.h>
#include "../Utils/image.h"
#include "../Utils/array_sort.h"

void Preprocessing(Image *image);

typedef struct {
    unsigned int minX, maxX, minY, maxY;
} MinMaxXY;

unsigned int AverageFilter(Image *image, unsigned int x, unsigned int y);

unsigned int MedianFilter(Image *image, unsigned int x, unsigned int y);

MinMaxXY GetMinMaxXY(unsigned int x, unsigned int y, unsigned int h, unsigned int w);


#endif //OCR_NOISE_REDUCTION_H