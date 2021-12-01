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
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/noise_reduction.h"


int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printHelp();
        return EXIT_SUCCESS;
    }

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

    Image* image 
    if(hexa){
        Image seg[81];
        split(&img, seg[81], 1, s_output_folder,hexa);
        updateSurface(&img);
        freeList(seg, 81);
    }else{
        Image seg[256];
        split(&img, seg[256], 1, s_output_folder,hexa);
        updateSurface(&img);
        freeList(seg, 256);
    }

    saveImage(&img, output_path);
    freeImage(&img, 1);

    return EXIT_SUCCESS;
}