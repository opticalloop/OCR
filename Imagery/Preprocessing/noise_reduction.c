#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../Utils/array_sort.h"


void Preprocessing(SDL_Surface *p) {
    int w = img->w;
    int h = img->h;

    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            // TODO : finish the function
            Uint32 pixel = AverageFilter(&p, i, j);
        }
    }
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            // TODO : finish the function
            Uint32 pixel = MedianFilter(&p, i, j);

        }
    }
}


Uint32 MedianFilter(SDL_Surface *p, int x, int y) {
    int w = img->w - 1;
    int h = img->h - 1;

    Struct m = GetMinMaxXY(x, y, h, w);

    Uint32 size = (m.maxX - m.minX + 1) * (m.maxY - m.minY + 1);
    Uint32 *mask = malloc(sizeof(int) * size);

    int index = 0;
    for (int i = m.minX; i <= m.maxX; ++i) {
        for (int j = m.minY; j <= m.maxY; ++j) {
            mask[index] = get_pixel(img, i, j);
            index++;
        }
    }
    array_sort_Uint32(&mask, size);

    Uint32 result = mask[size / 2];
    free(mask);

    return result;
}

Uint32 AverageFilter(SDL_Surface *img, int x, int y) {
    Uint32 result;
    Uint32 w = img->w - 1;
    Uint32 h = img->h - 1;

    Struct m = GetMinMaxXY(x, y, h, w);

    for (int i = minX; i <= maxX; ++i) {
        for (int y = minY; j <= maxY; ++j) {
            result += get_pixel(img, i, j);
        }
    }

    return result;
}
/*///////////////////////////////////////////////*/
/*///////////////////////////////////////////////*/

// MinMaxXY

/*///////////////////////////////////////////////*/
/*///////////////////////////////////////////////*/





Struct GetMinMaxXY(int x, int y, int h, int w) {
    Struct m;
    m.minX = (x > 0) ? x - 1 : 0;
    m.minY = (y > 0) ? y - 1 : 0;

    m.maxX = (x < h - 1) ? x + 1 : h;
    m.maxY = (y < w - 1) ? y + 1 : w;
    return m;
}