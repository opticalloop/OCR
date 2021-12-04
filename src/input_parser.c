#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "Imagery/Utils/op.h"
#include "NeuralNetwork/XOR.h"
#include "NeuralNetwork/save_load.h"
#include "NeuralNetwork/training.h"
#include "ocr.h"
#include "verbose.h"

#define WEIGHT_PATH "src/NeuralNetwork/Weights/w.data"
#define NB_HIDDEN 2
#define SIZE_HIDDEN 32

/*
 * Check if the condition is true, if so print the error
 */
static void checkError(int condition, char *message)
{
    if (condition)
    {
        errx(EXIT_FAILURE, "%s", message);
    }
}

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

static void printHelpOCR()
{
    printf(
        "🎨 Options ocr :\n"
        "      gui : open graphical interface\n"
        "      -o <output_path> : specify an output path\n"
        "      -r <angle> : manually rotate the image by the angle in degree\n"
        "      -v --verbose : print details of process\n"
        "      -S <folder> : save all intermediate images in a folder\n"
        "      --help : print ocr help\n");
}

static void printHelpNN()
{
    printf(
        "📊 Options nn :\n"
        "      -xor <nb_hidden_layer> <nb_node_per_hidden> <nb_epoch> : train "
        "the "
        "neural network on the xor function epoch time\n"
        "      -train <nb_hidden_layer> <nb_node_per_hidden> <nb_epoch> : "
        "train the "
        "network with the speficied number of hidden layer and node per "
        "hidden layer epoch time \n"
        "      -test <image_path> : network prediction on the number (should "
        "be 28 * 28)\n"
        "      -v --verbose : print details of process\n"
        "      -L --load <weight_path> : launch the network before train\n"
        "      -S --save <weight_path> : save the network after train\n"
        "      --help : print neural network help\n");
}

static void printHelp()
{
    printf(
        "###### Optical character recognition sudoku solver by\033[0;33m "
        "opticalloop \033[0m"
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

    char *output_folder = "";
    int save = 0;

    double rotateAngle = 0.0;

    int hexa = 0;

    // Parse all input
    for (int i = 2; i < argc; i++)
    {
        // GUI
        if (!strcmp(argv[i], "gui"))
        {
            // pthread_t thread;
            // pthread_create(&thread, NULL, init_gui, NULL);
            // pthread_join(thread, NULL);

            init_gui();
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
            checkError(i >= argc,
                       "⛔ You need to specify an output path after -o. See "
                       "--help with for more");

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
            checkError(i >= argc || !isNumber(argv[i]),
                       "⛔ You need to specify an rotation angle in degree "
                       "after -r. See --help for more");

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
        // Output folderr and SAVE
        else if (!strcmp(argv[i], "-S"))
        {
            i++;
            checkError(i >= argc,
                       "⛔ You need to specify an output folder after -S. See "
                       "--help with for more");

            if (!strcmp(argv[i], "--help"))
            {
                printHelpOCR();
                return;
            }
            save = 1;
            output_folder = argv[i];
        }
        // Parse hexa
        else if (!strcmp(argv[i], "-hexa"))
        {
            hexa = 1;
        }
    }
    pthread_t thread;
    SDL_Surface *img = load_image(input_path);
    Image image = newImage(img, 0, img->w, img->h);
    SDL_FreeSurface(img);
    saveImage(&image, "temp.bmp");
    freeImage(&image, 0);
    thread = OCR_thread("temp.bmp", output_path, verbose, save, output_folder,
                        0, hexa);
}

static void analyzeNN(int argc, char **argv)
{
    int verbose = 0;

    int test = 0;
    char test_input_path[100] = "";

    char launch_path[100] = "";
    char save_path[100] = "";

    int xor = 0; // train on xor
    int trainOnImg = 0; // train on images
    unsigned int epoch = 1000;
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
        else if (!strcmp(argv[i], "-xor") || !strcmp(argv[i], "-train"))
        {
            if (!strcmp(argv[i], "-xor"))
            {
                xor = 1;
            }
            else
            {
                trainOnImg = 1;
            }
            i++;
            // nb hidden layer

            if (!strcmp(argv[i], "--help"))
            {
                printHelpNN();
                return;
            }
            nbHidden = atoi(argv[i]);

            i++;

            // nb node per layer
            checkError(
                i >= argc,
                "⛔ You need to specify a number of node per hidden layer "
                "to "
                "train the network on the xor "
                ". See --help with for more");

            if (!strcmp(argv[i], "--help"))
            {
                printHelpNN();
                return;
            }
            sizeHidden = atoi(argv[i]);
            i++;
            // Epoch
            checkError(i >= argc,
                       "⛔ You need to specify a number of epoch to"
                       "train the network on the xor "
                       ". See --help for more");

            if (!strcmp(argv[i], "--help"))
            {
                printHelpNN();
                return;
            }

            checkError(!isNumber(argv[i]),
                       "⛔ Epoch should be a number"
                       ". See --help for more");

            epoch = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose"))
        {
            verbose = 1;
        }
        else if (!strcmp(argv[i], "-L") || !strcmp(argv[i], "--load"))
        {
            i++;

            checkError(i >= argc,
                       "⛔ You need to specify a path for the file to load"
                       ". See --help for more");

            if (!strcmp(argv[i], "--help"))
            {
                printHelpNN();
                return;
            }
            strcpy(launch_path, argv[i]);
            checkError(access(launch_path, F_OK) != 0,
                       "⛔ The specified file to load weight don't exist"
                       ". See --help for more");
        }
        else if (!strcmp(argv[i], "-S") || !strcmp(argv[i], "--save"))
        {
            i++;
            checkError(i >= argc,
                       "⛔ You need to specify a path for the file to save"
                       ". See --help for more");

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
                if (scanf("%s", str) == EOF)
                {}
                toUp(str);
                // While str != Y, YES, N and NO
                while (strcmp(str, "Y") && strcmp(str, "YES")
                       && strcmp(str, "N") && strcmp(str, "NO"))
                {
                    printf("\n[Y/n] : ");
                    if (scanf("%s", str) == EOF)
                    {}
                    toUp(str);
                }
                if (!strcmp(str, "N") || !strcmp(str, "NO"))
                {
                    return;
                }
            }
        }
        else if (!strcmp(argv[i], "-test"))
        {
            test = 1;
            i++;
            checkError(i >= argc,
                       "⛔ You need to specify a path for the file to save"
                       ". See --help for more");

            if (!strcmp(argv[i], "--help"))
            {
                printHelpNN();
                return;
            }
            strcpy(test_input_path, argv[i]);
        }
    }

    checkError(trainOnImg && xor,
               "⛔ You're not supposed to train the network on the xor and "
               "images function at the same time"
               ". See --help for more");

    if (!(trainOnImg && xor) && test)
    {
        Network network;
        network.sizeInput = NBINPUTS;
        network.sizeOutput = NBOUTPUTS;

        printVerbose(verbose, 0, "    🔨 Creating network\n");

        // launchWeights(&network, WEIGHT_PATH, verbose);

        if (verbose)
        {
            printf("    💻 Opening %s\n", test_input_path);
        }

        SDL_Surface *surface = load_image(test_input_path);

        int output = getNetworkOutput(&network, surface, verbose);
        printf("<-- ✅ Output : %d\n", output);
        freeNetwork(&network);
        SDL_FreeSurface(surface);
    }

    if (trainOnImg)
    {
        checkError(test,
                   "⛔ You're not supposed to train the network and "
                   "test it at the same time"
                   ". See --help for more");
        pthread_t thread;
        thread = train_thread(epoch, nbHidden, sizeHidden, verbose, launch_path,
                              save_path, 0);
        pthread_join(thread, NULL);
    }
    else if (xor)
    {
        checkError(test,
                   "⛔ You're not supposed to train the network on the xor and "
                   "test it at the same time"
                   ". See --help for more");
        launchXOR(epoch, nbHidden, sizeHidden, verbose, launch_path, save_path);
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
    else if (!strcmp(argv[1], "--help"))
    {
        printHelp();
    }
    else
    {
        checkError(1,
                   "⛔ The first argument should be ocr or nn (Neural "
                   "network)\n See --help for more");
    }

    return EXIT_SUCCESS;
}