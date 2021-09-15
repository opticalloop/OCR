#include "rotations.h"

#include <err.h>
#include <stdio.h>
#include <math.h>

#define PI M_PI

void rotate(Image *image, double angleDegree)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;
    const double middleX = ((double) width / (double) 2);
    const double middleY = ((double) height / (double) 2);

    double angle = angleDegree / 360.0;
    angle *= 2 * PI;

    // Create two dimensional array of pixels
    Pixel **_pixels = malloc(sizeof(Pixel*) * (width + 1));
    if (_pixels == NULL){
        errx(1, "Error while allocating memory");
    }

    unsigned int x = 0;
    for (; x < width; x++){
        _pixels[x] = malloc(sizeof(Pixel) * (height + 1));
        if (_pixels[x] == NULL){
            errx(1, "Error while allocating memory");
        }
    }
    // '\0'
    _pixels[x] = NULL;

    // Copy of all pixel 
    for (unsigned int x = 0; x < width; x++){
        for (unsigned int y = 0; y < height; y++){
            _pixels[x][y] = image->pixels[x][y];
        }
    }

    Pixel pixel;
    unsigned int newX;
    unsigned int newY;
    for (unsigned int x = 0; x < width; x++){
        for (unsigned int y = 0; y < height; y++){

            // Calculate new position
            newX = (unsigned int) ((double) (cos(angle) * ((double)x - middleX) 
                        - sin(angle) * ((double)y - middleY)) + middleX);
            newY = (unsigned int) ((double) (cos(angle) * ((double)y - middleY) 
                        + sin(angle) * ((double)x - middleX)) + middleY);

            // Assign it into new image and make sure he is in the image
            if (newX > 0 && newX < width && newY > 0 && newY < height){

                // Get the pixel on the copied image
                pixel = _pixels[x][y];

                // Assign the pixel
                image->pixels[newX][newY].r = pixel.r;
                image->pixels[newX][newY].g = pixel.g;
                image->pixels[newX][newY].b = pixel.b;
            }
        }
    }

    for (unsigned int i = 0; i < width; i++){
        free(_pixels[i]);
    }
    free(_pixels);
}


double detectDiffAngle(Image *image, float precision)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    // Get diagonal
    const unsigned int diagonal = (unsigned int) sqrt(width * width + height * height);

    double **accumulator;

    double _precision = PI * (1 / precision);

    accumulator = malloc(sizeof(double *) * (_precision + 1));
    if (accumulator == NULL){
        errx(1, "Error");
    }  
    unsigned int i = 0;
    for (; i < _precision; i++){
        accumulator[i] = malloc(sizeof(double) * (diagonal + 1));
        if (accumulator[i] == NULL){
            errx(1, "Error");
        }
    }

    double maxAngle = 0.0;
    double tempMaxAngle = 0.0;
    for (unsigned int x = 0; x < width; x++){
        for (unsigned int y = 0; y < height; y++){
            
            // Only when encounter black pixel try the circle around
            if (image->pixels[x][y].r != 0){
                continue;
            }

            // From 0 to 180
            double angle = -PI / 2;
            for (; angle <= PI / 2; angle += precision){
                int diff = (int) ((x * sin(angle) + y * cos(angle)));
                if (diff >= 0){
                    int teta = (int) ((angle * (1 / precision) + (PI / 2) * (1 / precision)));
                    accumulator[teta][diff]++;
                    
                    if (accumulator[teta][diff] > tempMaxAngle){
                        tempMaxAngle = accumulator[teta][diff];
                        maxAngle = angle;
                    }
                }
            }
        }
    }
    // Free memory
    for (unsigned int i = 0; i < _precision; i++){
        free(accumulator[i]);
    }
    free(accumulator);

    return maxAngle;
}

void autoRotate(Image *image, float precision)
{
    double angle = detectDiffAngle(image, precision);
    printf("Max angle %f\n", angle);

    rotate(image, angle * (180 / PI));
}