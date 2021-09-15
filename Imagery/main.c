#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Utils/image.h"
#include "Rotations/rotations.h"
#include "Color_Treatment/grayscale.h"
#include "Color_Treatment/blackandwhite.h"

int main(int argc, char *argv[])
{
    if (argc != 3){
        printf("Usage : ./main input_image output_image");
        return 1;
    }
    Image _image;
    _image.width = 0;
    _image.height = 0;
    _image.averageColor = 0;
    _image.pixels = NULL;
    _image.path = argv[1];
    _image.surface = NULL;
    Image *image = &_image;
    newImage(image);

    //displayImage(image);
    //grotate(image, 180);

    //grayscale(image);
    //blackandwhite(image);
    autoRotate(image, 0.01);
    saveImage(image, argv[2]);

    freeImage(image);

    return 0;
}
