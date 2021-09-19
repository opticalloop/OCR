#include <stdio.h>
#include <string.h>

static void printHelp()
{
    printf("Usage : ./main image_path [options] \n");
    printf("Options :\n"
           "gui : open graphical interface"
           "-o <output_path> : specify an output path\n"
           "-r <angle> : manually rotate the image by the anglec in degree\n"
           "--help : print this help\n");
}

int main(int argc, char **argv)
{
    printf("Optical character recognition sudoku solver by opticalloop\n");

    for (unsigned int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--help"))
        {
            printHelp();
        }
    }
}