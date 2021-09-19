#include <err.h>
#include <stdio.h>
#include <string.h>

static void printHelp()
{
    printf("Optical character recognition sudoku solver by opticalloop\n"
           "Usage : ./main image_path [options] \n"
           "Options :\n"
           "gui : open graphical interface\n"
           "-o <output_path> : specify an output path\n"
           "-r <angle> : manually rotate the image by the angle in degree\n"
           "--help : print this help\n");
}

int main(int argc, const char **argv)
{
    if (argc == 1)
    {
        printHelp();
        return 0;
    }

    char *output_path = "auto_save.bmp";
    double rotateAngle = 0.0;

    // Parse all input
    for (int i = 1; i < argc; i++)
    {
        // GUI
        if (!strcmp(argv[i], "gui"))
        {
            // TODO : Launch GUI
            return 0;
        }
        // Output path
        else if (!strcmp(argv[i], "-o"))
        {
            i++;
            if (i >= argc)
            {
                errx(1, "You need to specify an output path after -o. See "
                        "help with --help for more");
            }
            output_path = argv[i];
        }
        // Manually rotate
        else if (!strcmp(argv[i], "-r"))
        {
            i++;
            if (i >= argc)
            {
                errx(1, "You need to specify an rotation angle in degree "
                        "after -r. See help with --help for more");
            }
            // TODO : Convert argv[i] in double
            rotateAngle = 0.0;
        }
        else if (!strcmp(argv[i], "--help"))
        {
            printHelp();
            return 0;
        }
    }

    printf("%s %f", output_path, rotateAngle);

    return 0;
}