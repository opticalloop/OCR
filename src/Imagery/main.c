#include <err.h>
#include <stdio.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Color_Treatment/grayscale.h"
#include "Imagery/Rotations/rotations.h"
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/noise_reduction.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        errx(1, "Usage : ./main input_image output_image");
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

    grayscale(image);
    Preprocessing(image);

    saveImage(image, argv[2]);

    freeImage(image);

    return 0;
}
