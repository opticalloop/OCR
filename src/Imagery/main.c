#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Color_Treatment/grayscale.h"
#include "Imagery/Detection/houghtransform.h"
#include "Imagery/Detection/sobel.h"
#include "Imagery/Rotations_Resize/resize.h"
#include "Imagery/Rotations_Resize/rotations.h"
#include "Imagery/Segmentation/split.h"
#include "Imagery/Segmentation/clearsquare.h"
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/noise_reduction.h"


int main(int argc, char *argv[])
{
    char *input_path = argv[1];
    char *output_path = argv[2];

    int verbose = 0;

    int preprocessing = 0;
    char *p_output_folder = "";
    int hexa = 1;

    int _rotate = 0;
    double degree = 0;

    int detect = 0;

    int _resize = 0;
    int resize_width = 0;
    int resize_height = 0;

    int segment = 0;
    char *s_output_folder = "";

    SDL_Surface* surface = load_image(input_path);
    Image image = newImage(surface,0, surface->w, surface->h);
    
    grayscale(&image);
    blackandwhite(&image);
    clear_imperfections(&image);
    saveImage(&image, output_path);
    freeImage(&image, 1);

    return EXIT_SUCCESS;
}