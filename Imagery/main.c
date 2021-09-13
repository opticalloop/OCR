#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Utils/image.h"
#include "Rotations/rotations.h"
#include "Color_Treatment/grayscale.h"
#include "Color_Treatment/blackandwhite.h"

int main(void)
{
    Image _image;
    _image.width = 0;
    _image.height = 0;
    _image.averageColor = 0;
    _image.pixels = NULL;
    _image.path = "my_image.jpg";
    _image.surface = NULL;
    Image *image = &_image;
    newImage(image);

    //displayImage(image);
    rotate(image, 180);

    //grayscale(image);
    //blackandwhite(image);
    saveImage(image, "out.bmp");

    freeImage(image);

    return 0;
}
