#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "NeuralNetwork/XOR.h"
#include "NeuralNetwork/save_load.h"

static void toUp(char *temp)
{
    char *name;
    name = strtok(temp, ":");

    // Convert to upper case
    char *s = name;
    while (*s)
    {
        *s = toupper((unsigned char)*s);
        s++;
    }
}

static char isNumber(char *text)
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

static void printHelpOCR()
{
    printf("Options ocr :\n"
           "    gui : open graphical interface\n"
           "    -o <output_path> : specify an output path\n"
           "    -r <angle> : manually rotate the image by the angle in degree\n"
           "    -v --verbose : print details of process\n"
           "    --help : print ocr help\n");
}

static void printHelpNN()
{
    printf("Options nn :\n"
           "    -xor <nb_hidden_layer> <nb_node_per_hidden> : train the "
           "neural network on the xor function\n"
           "    -train <nb_hidden_layer> <nb_node_per_hidden> : train the "
           "network with the speficied number of hidden layer and node per "
           "hidden layer\n"
           "    -reset : reset weights of the neural network (need to train "
           "the network after doing that)\n"
           "    -v --verbose : print details of process\n"
           "    -L --load <weight_path> : launch the network before trade\n"
           "    -S --save <weight_path> : save the network after train\n"
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
    if (argc <= 2)
    {
        printHelpOCR();
        return;
    }
    int verbose = 0;
    char *input_path = argv[2];
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
        else if (!strcmp(argv[i], "--help"))
        {
            printHelpOCR();
            return;
        }
        // Output path
        else if (!strcmp(argv[i], "-o"))
        {
            i++;
            if (i >= argc)
            {
                errx(EXIT_FAILURE,
                     "⛔ You need to specify an output path after -o. See "
                     "help with --help for more");
            }
            if (!strcmp(argv[i], "--help"))
            {
                printHelpOCR();
                return;
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
                     "⛔ You need to specify an rotation angle in degree "
                     "after -r. See help with --help for more");
            }
            if (!strcmp(argv[i], "--help"))
            {
                printHelpOCR();
                return;
            }
            rotateAngle = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose"))
        {
            verbose = 1;
        }
    }

    printf("%s %s %f", input_path, output_path, rotateAngle);
}

static void analyzeNN(int argc, char **argv)
{
    int verbose = 0;

    char launch_path[100] = "";
    char save_path[100] = "";

    int xor = 0;
    unsigned int nbHidden = 0;
    unsigned int sizeHidden = 0;

    if (argc == 2)
    {
        printHelpNN();
        return;
    }

    for (int i = 2; i < argc; i++)
    {
        if (!strcmp(argv[i], "--help"))
        {
            printHelpNN();
            return;
        }
        else if (!strcmp(argv[i], "-xor"))
        {
            xor = 1;
            i++;
            // nb hidden layer
            if (i >= argc)
            {
                errx(EXIT_FAILURE,
                     "⛔ You need to specify a number of hidden "
                     "layer to train the network on the xor "
                     ". See help with --help for more");
            }
            if (!strcmp(argv[i], "--help"))
            {
                printHelpNN();
                return;
            }
            nbHidden = atoi(argv[i]);

            i++;
            // nb node per layer
            if (i >= argc)
            {
                errx(EXIT_FAILURE,
                     "⛔ You need to specify a number of node per hidden layer "
                     "to "
                     "train the network on the xor "
                     ". See help with --help for more");
            }
            if (!strcmp(argv[i], "--help"))
            {
                printHelpNN();
                return;
            }
            sizeHidden = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose"))
        {
            verbose = 1;
        }
        else if (!strcmp(argv[i], "-L") || !strcmp(argv[i], "--load"))
        {
            i++;
            if (i >= argc)
            {
                errx(EXIT_FAILURE,
                     "⛔ You need to specify a path for the file to load"
                     ". See help with --help for more");
            }
            if (!strcmp(argv[i], "--help"))
            {
                printHelpNN();
                return;
            }
            strcpy(launch_path, argv[i]);
            if (access(launch_path, F_OK) != 0)
            {
                errx(EXIT_FAILURE,
                     "⛔ The specified file to load weight don't exist"
                     ". See help with --help for more");
            }
        }
        else if (!strcmp(argv[i], "-S") || !strcmp(argv[i], "--save"))
        {
            i++;
            if (i >= argc)
            {
                errx(EXIT_FAILURE,
                     "⛔ You need to specify a path for the file to save"
                     ". See help with --help for more");
            }
            if (!strcmp(argv[i], "--help"))
            {
                printHelpNN();
                return;
            }
            strcpy(save_path, argv[i]);
            if (access(save_path, F_OK) == 0)
            {
                char str[100];
                printf("    ❗ The file specified where to save weights already "
                       "exist, overwrite it ? [Y/n] : ");
                scanf("%s", str);
                toUp(str);
                // While str != Y, YES, N and NO
                while (strcmp(str, "Y") && strcmp(str, "YES")
                       && strcmp(str, "N") && strcmp(str, "NO"))
                {
                    printf("\n[Y/n] : ");
                    scanf("%s", str);
                    toUp(str);
                }
                if (!strcmp(str, "N") || !strcmp(str, "NO"))
                {
                    return;
                }
            }
        }
    }
    if (xor)
    {
        launchXOR(nbHidden, sizeHidden, verbose, launch_path, save_path);
        return;
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
             "⛔ The first argument should be ocr or nn (Neural "
             "network)\n See --help for more");
    }

    return EXIT_SUCCESS;
}