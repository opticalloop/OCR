#include "rotations.h"

#include <err.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265

void rotate(Image *image, double angleDegree)
{
    unsigned int width = image->width;
    unsigned int height = image->height;
    double angle = angleDegree / 360.0;
    angle *= 2 * PI;
    double middleX = ((double) width / (double) 2);
    double middleY = ((double) height / (double) 2);

    // Create temporary image
    Image _image = copyImage(image);

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
            if (newX >= 0 && newX < width && newY >= 0 && newY < height){

                // Get the pixel on the copied image
                pixel = _image.pixels[x][y];

                // Assign the pixel
                image->pixels[newX][newY].r = pixel.r;
                image->pixels[newX][newY].g = pixel.g;
                image->pixels[newX][newY].b = pixel.b;
            }
        }
    }
    freeImage(&_image);
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