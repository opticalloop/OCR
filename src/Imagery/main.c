#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Color_Treatment/grayscale.h"
#include "Imagery/Rotations_Resize/resize.h"
#include "Imagery/Rotations_Resize/rotations.h"
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/noise_reduction.h"
#include "Imagery/segmentation/split81.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        errx(1, "Usage : ./main input_image output_image folder_name");
    }

    Image image;
    image.path = argv[1];
    newImage(&image);

    // displayImage(image);
    // rotate(image, 180);

    grayscale(&image);
    // TODO : Remove the tmp directory
    char s[1000] = "mkdir "; // create tmp directory for presentation
    strcat(s, argv[3]);

    if (system(s))
    {
    }
    Preprocessing(&image, argv[3]);

    // blackandwhite(image);
    // autoRotate(image, 0.01);

    // resizeImage(image, 1280, 720);

    SDL_Surface *seg81[81];
    blackandwhite(&image);
    // displayblock(&image,10,10,110,110);
    split(image, seg81, 1, "image01");
    updateSurface(&image);
    displayImage(&image);
    saveImage(&image, argv[2]);
    freeImage(&image);
    freeList(seg81, 81);

    return 0;
}
