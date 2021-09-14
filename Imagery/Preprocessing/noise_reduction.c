#include <stdlib.h>
#include "../Utils/image.h"
#include "../Utils/array_sort.h"
#include "noise_reduction.h"
#include <stdio.h>


void Preprocessing(Image *image) {
    unsigned int w = image->width;
    unsigned int h = image->height;


    for (unsigned int i = 0; i < w; i++) {
        for (unsigned int j = 0; j < h; j++) {
            updatePixelToSameValue(&(image->pixels[i][j]), MedianFilter(image, i, j));

        }
    }
//    for (int i = 0; i < w; i++) {
//        for (int j = 0; j < h; j++) {
//            updatePixelToSameValue(&(image->pixels[i][j]), AverageFilter(image, i, j));
//        }
//    }

}


unsigned int MedianFilter(Image *image, int x, int y) {
    unsigned int w = image->width;
    unsigned int h = image->height;

    MinMaxXY m = GetMinMaxXY(x, y, h, w);

    unsigned int *mask = malloc(sizeof(unsigned int) * (9 + 1));

    unsigned int index = 0;
    for (unsigned int i = m.minX; i < m.maxX; i++) {
        for (unsigned int j = m.minY; j < m.maxY; j++) {
            mask[index] = image->pixels[i][j].b;
            index++;
        }
    }
    for (; index < 9; index++) {
        mask[index] = 0;
    }
    array_sort(mask, 9);

    unsigned int result = mask[9 / 2];
    free(mask);
    return result;
}

unsigned int AverageFilter(Image *image, int x, int y) {
    unsigned int result = 0;
    unsigned int w = image->width - 1;
    unsigned int h = image->height - 1;

    MinMaxXY m = GetMinMaxXY(x, y, h, w);


    for (unsigned int i = m.minX; i <= m.maxX; ++i) {
        for (unsigned int j = m.minY; j <= m.maxY; ++j) {
            result += image->pixels[x][y].b;
        }
    }

    return result;
}


/*///////////////////////////////////////////////*/
/*///////////////////////////////////////////////*/

// MinMaxXY

/*///////////////////////////////////////////////*/
/*///////////////////////////////////////////////*/


MinMaxXY GetMinMaxXY(int x, int y, int h, int w) {
    MinMaxXY m;
    m.minX = (x == 0) ? 0 : x - 1;
    m.minY = (y == 0) ? 0 : y - 1;

    m.maxX = (x == w - 1) ? w - 1 : x + 1;
    m.maxY = (y == h - 1) ? h - 1 : y + 1;

    return m;
}