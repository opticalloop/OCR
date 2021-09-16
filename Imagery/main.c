#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Utils/image.h"
#include "Rotations/rotations.h"
#include "Preprocessing/noise_reduction.h"
#include "Color_Treatment/grayscale.h"
#include "Color_Treatment/blackandwhite.h"

int main(void)
{   
    Image _image;
    _image.width = 0;
    _image.height = 0;
    _image.averageColor = 0;
    _image.pixels = NULL;
    _image.path = "image_02.jpeg";
    _image.surface = NULL;
    Image *image = &_image;
    newImage(image);

    grayscale(image);
    blackandwhite(image);

//    displayImage(image);
    saveImage(image, "out.bmp");
    Preprocessing(image);
//    rotate(image, 180);

    saveImage(image, "out2.bmp");

    freeImage(image);

    return 0;
}