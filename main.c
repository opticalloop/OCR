#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char isNumber(char *text)
{
    int j;
    j = strlen(text);
    while (j--)
    {
        if (text[j] >= '0' && text[j] <= '9')
            continue;

        return 0;
    }
    return 1;
}

static void printHelpOCR()
{
    printf(
        "Options ocr :\n"
        "    gui : open graphical interface\n"
        "    -o <output_path> : specify an output path\n"
        "    -r <angle> : manually rotate the image by the angle in degree\n"
        "    --help : print ocr help\n");
}

static void printHelpNN()
{
    printf("Options nn :\n"
           "    -xor <nb_hidden_layer> <nb_node_per_hidden> : train the "
           "neural network on the xor function\n"
           "    -reset : reset weights of the neural network (need to train "
           "the network after doing that)\n"
           "    -train <nb_hidden_layer> <nb_node_per_hidden> : train the "
           "network with the speficied number of hidden layer and node per "
           "hidden layer\n"
           "    -save : \n"
           "    --help : print neural network help\n");
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

static void analyzeOCR(int argc, char **argv)
{
    char *input_path = argv[1];
    char *output_path = "auto_save.bmp";
    double rotateAngle = 0.0;

    // Parse all input
    for (int i = 2; i < argc; i++)
    {
        // GUI
        if (!strcmp(argv[i], "gui"))
        {
            // TODO : Launch GUI
            return;
        }
        // Output path
        else if (!strcmp(argv[i], "-o"))
        {
            i++;
            if (i >= argc)
            {
                errx(EXIT_FAILURE,
                    "You need to specify an output path after -o. See "
                    "help with --help for more");
            }
            output_path = argv[i];
        }
        // Manually rotate
        else if (!strcmp(argv[i], "-r"))
        {
            i++;
            if (i >= argc || !isNumber(argv[i]))
            {
                errx(EXIT_FAILURE,
                    "You need to specify an rotation angle in degree "
                    "after -r. See help with --help for more");
            }
            rotateAngle = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "--help"))
        {
            printHelpOCR();
            return;
        }
    }

    printf("%s %f", output_path, rotateAngle);
}

static void analyzeNN(int argc, char **argv)
{
    // unsigned int nbHidden = 0;
    // unsigned int sizeHidden = 0;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--help"))
        {
            printHelpNN();
        }
    }
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printHelp();
        return EXIT_SUCCESS;
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
        errx(EXIT_FAILURE,
            "The first argument should be ocr or nn (Neural network)");
    }

    return EXIT_SUCCESS;
}