#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Color_Treatment/grayscale.h"
#include "Imagery/Resize/resize.h"
#include "Imagery/Rotations/rotations.h"
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/noise_reduction.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        errx(1, "Usage : ./main input_image output_image folder_name");
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

    // displayImage(image);
    // rotate(image, 180);

    grayscale(image);
    // TODO : Remove the tmp directory
    char s[1000] = "mkdir "; // create tmp directory for presentation
    strcat(s, argv[3]);

    if (system(s))
    {
    }
    Preprocessing(image, argv[3]);

    // blackandwhite(image);
    // autoRotate(image, 0.01);

    // resizeImage(image, 1280, 720);
    Image newimage = resize(image, 100, 100);

    saveImage(&newimage, argv[2]);

    freeImage(&newimage);

    return 0;
}
