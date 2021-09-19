#include <err.h>
#include <stdio.h>
#include <string.h>

static void printHelpOCR()
{
    prinftf(
        "Options ocr :\n"
        "    gui : open graphical interface\n"
        "    -o <output_path> : specify an output path\n"
        "    -r <angle> : manually rotate the image by the angle in degree\n"
        "    --help : print this help\n");
}

static void printHelpNN()
{
    printf("Options nn :\n"
           "    -xor : train the neural network on the xor function\n"
           "    -reset : reset weights of the neural network (need to train "
           "the network after doing that)\n"
           "    -train <nb_hidden_layer> <nb_node_per_hidden> : train the "
           "network with the speficied number of hidden layer and node per "
           "hidden layer\n");
}

static void printHelp()
{
    printf(
        "###### Optical character recognition sudoku solver by opticalloop "
        "######\n"
        "Usage : ./main ocr <image_path> [options] or ./main nn [options] \n");

    printHelpOCR();
    printHelpNN();
}

static void analyzeOCR(int argc, const char **argv)
{
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
            printHelpOCR();
            return 0;
        }
    }

    printf("%s %f", output_path, rotateAngle);
}

static void analyzeNN(int argc, const char **argv)
{
}

int main(int argc, const char **argv)
{
    if (argc == 1)
    {
        printHelp();
        return 0;
    }

    if (!strcmp(argv[1], "ocr"))
    {
        analyzeOCR(argc, argv);
    }
    else if (!strcmp(argv[1], "nn"))
    {
        analyzeNN(argc, argv);
    }
    else
    {
        errx(1, "The first argument should be ocr or nn (Neural network)");
    }

    return 0;
}