#include "rotations.h"

#include <math.h>
#include "../op/op.h"
#include "../Pixel_operations/pixel_operations.h"

void rotate(Image *image, float angle)
{
    unsigned int width = image->width;
    unsigned int height = image->height;

    unsigned int middleX = width / 2;
    unsigned int middleY = height / 2;

    Pixel pixel;
    unsigned int newX;
    unsigned int newY;
    for (unsigned int x = 0; x < width; x++){
        for (unsigned int y = 0; y < height; y++){
            
            pixel = image->pixels[x][y];

            newX = cos(angle) * (x - middleX) + cos(angle) * (y - middleY);
            newY = sin(angle) * (x - middleX) + sin(angle) * (y - middleY);
        }
    }
}