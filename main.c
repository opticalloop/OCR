#include <stdio.h>
#include <string.h>

static void printHelp()
{
    printf("Usage : ./main image_path [options] \n");
    printf("Options :\n");
    printf("-o output_path : specify an output path\n");
    printf("-r angle : manually rotate the image by the angle\n");
    printf("--help : print this help\n");
}

int main(int argc, char **argv)
{
    printf("Optical character recognition sudoku solver by opticalloop\n");

    for (unsigned int i = 1; i < argc; i++){
        if (!strcmp(argv[i], "--help")){
            printHelp();
        }
    }
}