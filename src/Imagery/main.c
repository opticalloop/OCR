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
    _image.path = argv[1];
    _image.surface = NULL;
    Image *image = &_image;
    newImage(image);
    // blackandwhite(image);
    // reverse_color(image);

    Image _drawImage;
    _drawImage.path = argv[1];
    _drawImage.surface = NULL;
    Image *drawImage = &_drawImage;

    newImage(drawImage);

    Image _simpleImage;
    _simpleImage.path = argv[1];
    _simpleImage.surface = NULL;
    Image *simpleImage = &_simpleImage;
    newImage(simpleImage);

    Image _squareImage;
    _squareImage.path = argv[1];
    _squareImage.surface = NULL;
    Image *squareImage = &_squareImage;
    newImage(squareImage);

    Image _lastSquare;
    _lastSquare.path = argv[1];
    _lastSquare.surface = NULL;
    Image *lastSquare = &_lastSquare;
    newImage(lastSquare);

    detection(image, drawImage, simpleImage, squareImage, lastSquare);

    saveImage(image, argv[2]);
    saveImage(drawImage, "1.0_all_lines.bmp");
    saveImage(simpleImage, "1.1_simplified_lines.bmp");
    saveImage(squareImage, "1.2_squares_only.bmp");
    saveImage(lastSquare, "1.3_last_square.bmp");

    freeImage(lastSquare);
    freeImage(squareImage);
    freeImage(simpleImage);
    freeImage(drawImage);
    freeImage(image);

    return 0;
}