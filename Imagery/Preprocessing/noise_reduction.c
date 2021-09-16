
#include "noise_reduction.h"
#include <stdio.h>


void Preprocessing(Image *image) {
    unsigned int w = image->width;
    unsigned int h = image->height;

    Pixel **mask = malloc((w + 1) * sizeof(Pixel *));
    for (unsigned int i = 0; i < w; i++) {
        mask[i] = (Pixel *) malloc((h + 1) * sizeof(Pixel));
        for (unsigned int j = 0; j < h; j++) {
            updatePixelToSameValue(&(mask[i][j]), AverageFilter(image, i, j) > 4 ? 255 : 0);

        }
    }
    for (unsigned int i = 0; i < w; i++) {
        for (unsigned int j = 0; j < h; j++) {

            updatePixelToSameValue(&(image->pixels[i][j]),mask[i][j].b);
        }
        free(mask[i]);
    }

    free(mask);


}

//static void printArray(unsigned int *array, unsigned int n) {
//    printf("{ ");
//    for (unsigned int i = 0; i < n - 1; ++i) {
//        printf("%u, ", array[i]);
//    }
//    printf("%u }", array[n - 1]);
//}

//unsigned int MedianFilter(Image *image, unsigned int x, unsigned int y) {
//    unsigned int w = image->width;
//    unsigned int h = image->height;
//
//    MinMaxXY m = GetMinMaxXY(x, y, h, w);
//    unsigned int size = sizeof(unsigned int) * (9 + 1);
//    unsigned int *mask = malloc(size);
//
//    unsigned int index = 0;
//    for (unsigned int i = m.minX; i <= m.maxX; ++i) {
//        for (unsigned int j = m.minY; j <= m.maxY; ++j) {
//            mask[index] = image->pixels[i][j].b;
//            index++;
//        }
//    }
//    for (; index < 9; ++index) {
//        mask[index] = 0;
//    }
//    array_sort(mask, 9);
//
//
//    unsigned int result = mask[4];
//    free(mask);
//    return result;
//}


unsigned int AverageFilter(Image *image, unsigned int x, unsigned int y) {
    unsigned int result = 0;
    unsigned int w = image->width;
    unsigned int h = image->height ;

    MinMaxXY m = GetMinMaxXY(x, y, h, w);


    for (unsigned int i = m.minX; i <= m.maxX; ++i) {
        for (unsigned int j = m.minY; j <= m.maxY; ++j) {
            result += image->pixels[x][y].b / 255;
        }
    }

    return result;
}


/*///////////////////////////////////////////////*/
/*///////////////////////////////////////////////*/

// MinMaxXY

/*///////////////////////////////////////////////*/
/*///////////////////////////////////////////////*/


MinMaxXY GetMinMaxXY(unsigned int x, unsigned int y, unsigned int h, unsigned int w) {
    MinMaxXY m;
    m.minX = (x == 0) ? 0 : x - 1;
    m.minY = (y == 0) ? 0 : y - 1;

    m.maxX = (x == w - 1) ? w - 1 : x + 1;
    m.maxY = (y == h - 1) ? h - 1 : y + 1;

    return m;
}