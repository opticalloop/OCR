//
// Created by dov on 11/09/2021.
//

#ifndef OCR_NOISE_REDUCTION_H
#define OCR_NOISE_REDUCTION_H

void Preprocessing(Image *image);

typedef struct {
    unsigned int minX, maxX, minY, maxY;
} MinMaxXY;

unsigned int AverageFilter(Image *image, int x, int y);

unsigned int MedianFilter(Image *image, int x, int y);

MinMaxXY GetMinMaxXY(int x, int y, int h, int w);





#endif //OCR_NOISE_REDUCTION_H