#include "rotations.h"

#include <math.h>

void rotate(Image *image, float angle)
{
    unsigned int width = image->width;
    unsigned int height = image->height;
 
    Image _image;
    _image.width = image->width;
    _image.height = image->height;
    _image.averageColor = image->averageColor;
    // Allocate memory
    _image.pixels = malloc((_image.width + 1) * sizeof(Pixel *));

    if (_image->pixels == NULL){
        printf("Error while allocating pixels pointers for the image");
        return;
    }

    unsigned int x;
    for (x = 0; x < _image.width; x++){
        _image->pixels[x] = (Pixel *) malloc((_image.height + 1) * sizeof(Pixel));  
        if (_image->pixels[x]== NULL){
            printf("Error while allocating pixels pointers for the image");
            return;
        }
    }
    // Make sure we don't have the '\0'
    _image->pixels[x] = NULL;

    unsigned int middleX = width / 2;
    unsigned int middleY = height / 2;

    Pixel pixel;
    unsigned int newX;
    unsigned int newY;
    for (unsigned int x = 0; x < width; x++){
        for (unsigned int y = 0; y < height; y++){
            
            // Get the pixel on the actual image
            pixel = image->pixels[x][y];

            // Calculate new position
            newX = cos(angle) * (x - middleX) + cos(angle) * (y - middleY);
            newY = sin(angle) * (x - middleX) + sin(angle) * (y - middleY);

            // Assign it into new image and make sure he is in the image
            if (newX < width && newY < height){
                _image.pixels[newX][newY].r = pixel.r;
                _image.pixels[newX][newY].g = pixel.b;
                _image.pixels[newX][newY].b = pixel.g;
            }
        }
    }
    freeImage(image);
    image->pixels = _image.pixels;
}