#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Color_Treatment/grayscale.h"
#include "Imagery/Rotations_Resize/resize.h"
#include "Imagery/Rotations_Resize/rotations.h"
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/noise_reduction.h"

static void printHelp(void)
{
    printf(
        "Usage : ./main <input_image> <output_image> [options]\n"
        "Options :\n"
        "      	 -v --verbose : print the details\n"
        "         -p --preprocessing <output_folder> : preprocess the image "
        "with all filters, save the details in output folder\n"
        "         -r --rotate <degree> : rotate image by degree in parameter\n"
        "         -R --resize <width> <height> : resize the image\n"
        "         -s --segment <output_folder> : segment the image\n"
        "         --help : print this help");
}

static int isNumber(char *text)
{
    int len = strlen(text);
    for (int j = 0; j < len; j++)
    {
        // Accept comma but need to convert it to a point
        if (text[j] == ',')
        {
            text[j] = '.';
        }

        if ((text[j] < '0' || text[j] > '9') && text[j] != '.')
            return 0;
    }
    return 1;
}

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

    int _rotate = 0;
    double degree = 0;

    int _resize = 0;
    int resize_width = 0;
    int resize_height = 0;

    int segment = 0;
    char *s_output_folder = "";

    for (int i = 3; i < argc; i++)
    {
        if (!strcmp(argv[i], "--help"))
        {
            printHelp();
            return EXIT_SUCCESS;
        }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose"))
        {
            verbose = 1;
        }
        else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--rotate"))
        {
            i++;
            if (i >= argc || !isNumber(argv[i]))
            {
                printHelp();
                return EXIT_SUCCESS;
            }
            _rotate = 1;
            degree = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--preprocessing"))
        {
            i++;
            if (i >= argc)
            {
                printHelp();
                return EXIT_SUCCESS;
            }
            preprocessing = 1;
            p_output_folder = argv[i];
        }
        else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--segment"))
        {
            i++;
            if (i >= argc)
            {
                printHelp();
                return EXIT_SUCCESS;
            }
            segment = 1;
            s_output_folder = argv[i];
        }
        else if (!strcmp(argv[i], "-R") || !strcmp(argv[i], "--resize"))
        {
            i++;
            if (i >= argc || !isNumber(argv[i]))
            {
                printHelp();
                return EXIT_SUCCESS;
            }
            resize_width = atoi(argv[i]);
            i++;
            if (i >= argc || !isNumber(argv[i]))
            {
                printHelp();
                return EXIT_SUCCESS;
            }
            _resize = 1;
            resize_height = atoi(argv[i]);
        }
    }

    Image image;
    image.path = input_path;
    newImage(&image);

    if (preprocessing)
    {
        // Create directory
        char s[1000] = "mkdir ";
        strcat(s, p_output_folder);
        if (system(s))
        {
        }

        // Preprocessing
        grayscale(&image);
        Preprocessing(&image, p_output_folder, verbose);
    }

    if (_rotate)
    {
        rotate(&image, degree);
    }

    if (_resize)
    {
        Image resized_image = resize(&image, resize_width, resize_height);
        saveImage(&resized_image, output_path);
        return EXIT_SUCCESS;
    }

    if (segment)
    {
        SDL_Surface *seg81[81];
        split(image, seg81, 1, s_output_folder);
        updateSurface(&image);
        freeList(seg81, 81);
    }

    saveImage(&image, output_path);
    freeImage(&image);

    return EXIT_SUCCESS;
}
