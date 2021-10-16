#include <err.h>
#include <stdio.h>
<<<<<<< HEAD:Imagery/main.c
#include "Color_Treatment/blackandwhite.h"
#include "Color_Treatment/grayscale.h"
#include "Resize/resize.h"
#include "Rotations/rotations.h"
#include "Utils/image.h"
#include "Preprocessing/noise_reduction.h"
=======

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Color_Treatment/grayscale.h"
#include "Imagery/Resize/resize.h"
#include "Imagery/Rotations/rotations.h"
#include "Imagery/Utils/image.h"
>>>>>>> ea9609810b42a6e49f91d2b28947012aca2ecf48:src/Imagery/main.c


int main(int argc, char *argv[]) {
    if (argc != 3) {
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

<<<<<<< HEAD:Imagery/main.c

//    displayImage(image);
    // rotate(image, 180);

    grayscale(image);
//    displayImage(image);
    Preprocessing(image);

//     blackandwhite(image);
//    displayImage(image);

//    autoRotate(image, 0.01);
=======
    // displayImage(image);
    rotate(image, 180);

    // grayscale(image);
    // blackandwhite(image);
    // autoRotate(image, 0.01);
>>>>>>> ea9609810b42a6e49f91d2b28947012aca2ecf48:src/Imagery/main.c

    // resizeImage(image, 1280, 720);

    saveImage(image, argv[2]);
//
    freeImage(image);

    return 0;
}
