#include <err.h>
#include <stdio.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Color_Treatment/grayscale.h"
#include "Imagery/Resize/resize.h"
#include "Imagery/Rotations/rotations.h"
#include "Imagery/Segmentation/houghtransform.h"
#include "Imagery/Utils/image.h"

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
    blackandwhite(image);
    reverse_color(image);

    Image _drawImage;
    _drawImage.path = argv[1];
    _drawImage.surface = NULL;
    Image *drawImage = &_drawImage;
    newImage(drawImage);
    detection(image, drawImage);
    // displayImage(image);
    // rotate(image, 180);

    // grayscale(image);

    // autoRotate(image, 0.01);

    // resizeImage(image, 1280, 720);

    saveImage(image, argv[2]);
    saveImage(drawImage, "drawImage.bmp");
    freeImage(drawImage);
    freeImage(image);

    return 0;
}