#include "rotations.h"

#include <err.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265

void rotate(Image *image, double angle)
{
    unsigned int width = image->width;
    unsigned int height = image->height;

    Image _image;
    _image.width = image->width;
    _image.height = image->height;
    _image.averageColor = image->averageColor;
    // Allocate memory
    _image.pixels = malloc((_image.width + 1) * sizeof(Pixel *));

    if (_image.pixels == NULL){
        printf("Error while allocating pixels pointers for the image");
        return;
    }

    unsigned int x;
    for (x = 0; x < _image.width; x++){
        _image.pixels[x] = malloc((_image.height + 1) * sizeof(Pixel));
        if (_image.pixels[x]== NULL){
            printf("Error while allocating pixels pointers for the image");
            return;
        }
    }
    // Make sure we don't have the '\0'
    _image.pixels[x] = NULL;

    double middleX = ((double) width / (double) 2);
    double middleY = ((double) height / (double) 2);

    Pixel pixel;
    unsigned int newX;
    unsigned int newY;
    for (unsigned int x = 0; x < width; x++){
        for (unsigned int  y = 0; y < height; y++){

            // Calculate new position
            newX = (unsigned int) ((double) (cos(angle) * ((double)x - middleX) 
                        - sin(angle) * ((double)y - middleY)) + middleX + 1);
            newY = (unsigned int) ((double) (cos(angle) * ((double)y - middleY) 
                        + sin(angle) * ((double)x - middleX)) + middleY + 1);

            // Assign it into new image and make sure he is in the image
            if (newX >= 0 && newX < width && newY >= 0 && newY < height){

                // Get the pixel on the actual image
                pixel = image->pixels[x][y];

                // Assign the pixel
                _image.pixels[newX][newY].r = pixel.r;
                _image.pixels[newX][newY].g = pixel.g;
                _image.pixels[newX][newY].b = pixel.b;
            }
        }
    }
    freeImage(image);
    image->pixels = _image.pixels;
}


double detectDiffAngle(Image *image, float precision)
{
    unsigned int width = image->width;
    unsigned int height = image->height;

    // Get diagonal
    unsigned int diagonal = (unsigned int) sqrt(width * width + height * height);

    double **accumulator;

    accumulator = malloc(sizeof(double *) * (PI * (1 / precision) + 1));
    if (accumulator == NULL){
        errx(1, "Error");
    }  
    unsigned int i = 0;
    for (; i < PI * (1 / precision); i++){
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
    for (unsigned int i = 0; i < PI* (1 / precision); i++){
        free(accumulator[i]);
    }
    free(accumulator);

    return maxAngle;
}

void autoRotate(Image *image, float precision)
{
    double angle = detectDiffAngle(image, precision);
    printf("Max angle %f\n", angle);

    rotate(image, angle);
}