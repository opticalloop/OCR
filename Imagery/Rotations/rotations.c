#include "rotations.h"

#include <math.h>

void rotate(Image *image, double angle)
{
    int width = image->width;
    int height = image->height;

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
        _image.pixels[x] = (Pixel *) malloc((_image.height + 1) 
                * sizeof(Pixel));
        if (_image.pixels[x]== NULL){
            printf("Error while allocating pixels pointers for the image");
            return;
        }
    }
    // Make sure we don't have the '\0'
    _image.pixels[x] = NULL;

    double middleX = (double) (width / 2);
    double middleY = (double) (height / 2);

    Pixel pixel;
    int newX;
    int newY;
    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){

            // Calculate new position
            newX = (int) ((double) (cos(angle) * ((double)x - middleX) 
                        - sin(angle) * ((double)y - middleY)) + middleX);
            newY = (int) ((double) (cos(angle) * ((double)y - middleY) 
                        + sin(angle) * ((double)x - middleX)) + middleY);

            // Assign it into new image and make sure he is in the image
            if (newX > 0 && newX < width && newY > 0 && newY < height){

                // Get the pixel on the actual image
                pixel = image->pixels[x][y];

                // Assin the pi
                _image.pixels[newX][newY].r = pixel.r;
                _image.pixels[newX][newY].g = pixel.g;
                _image.pixels[newX][newY].b = pixel.b;
            }
        }
    }
    freeImage(image);
    image->pixels = _image.pixels;
}
