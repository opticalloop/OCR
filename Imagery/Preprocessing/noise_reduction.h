//
// Created by dov on 11/09/2021.
//

#ifndef OCR_NOISE_REDUCTION_H
#define OCR_NOISE_REDUCTION_H

void Preprocessing(SDL_Surface *p);
Uint32 AverageFilter(SDL_Surface *img, int x, int y);
Uint32 MedianFilter(SDL_Surface *p, int x, int y);

struct MinMaxXY {
    Uint32 minX, maxX, minY, maxY;
};
typedef struct MinMaxXY Struct;

#endif //OCR_NOISE_REDUCTION_H