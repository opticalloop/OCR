#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "../Imagery/Utils/image.h"
#include "neural_network.h"
#include "save_load.h"

#define NBIMAGES 1
#define NBINPUTS 28 * 28;
#define NBOUTPUTS 9;
#define NBHIDDENLAYERS 2;
#define NBNODESPERHIDDEN 16;

void printResult(double expected[], Neuron neuron[])
{
    // Print expected
    for (unsigned int l = 0; l < NBOUTPUTS; l++)
    {
        if (expected[l] == 1.0)
        {
            printf("\nInput : image of a %u\n", l + 1);
            break;
        }
    }
    for (unsigned int k = 0; k < NBOUTPUTS; k++)
    {
        printf("Output : %f, expected : %u\n", neuron[k].value,
               (unsigned int)expected[k]);
    }
}

void checkInputs(double inputs[NBINPUTS])
{
    for (unsigned int i = 0; i < NBINPUTS; i++)
    {
        if (inputs[i] > 234124.0)
        {
            errx(1, "Too long");
        }
    }
}

void imageToBinary(SDL_Surface *surface, double inputs[])
{
    SDL_Color rgb;
    Uint32 pixel;
    for (unsigned int i = 0; i < 28; i++)
    {
        for (unsigned int j = 0; j < 28; j++)
        {
            // Get pixel colors
            pixel = get_pixel(surface, i, j);
            SDL_GetRGB(pixel, surface->format, &rgb.r, &rgb.g, &rgb.b);

            intputs[i * j] = (double)(1.0 - ((double)rgb.r / 255.0));

            // printf("Input in creation of data : %f\n", intputs[i * j]);
        }
    }
}

// launch pixel value in the intputPaths array, and define expected digit
// Consider that the image is already in grayscale
void createData(char *path, double inputs[], double expected[])
{
    // Get absolute path
    char actualpath[345 + 1];
    char *ptr;

    ptr = realpath(path, actualpath);

    printf("File : %s\n", ptr);
    SDL_Surface *surface = load_image(ptr);

    imageToBinary(surface, inputs);

    // Get expected value
    int num = path[17] - '0';

    // Init expected value
    for (int i = 0; i < NBOUTPUTS; i++)
    {
        expected[i] = i + 1 == num ? 1.0 : 0.0;
    }

    printf("Expected : %d\n", num);

    SDL_FreeSurface(surface);
}

void createAllData(char *directory, char *intputPaths[],
                   double input[NBIMAGES][NBINPUTS],
                   double expected[NBIMAGES][NBOUTPUTS])
{
    // Get all images paths
    DIR *FD;
    struct dirent *in_file;
    /* Scanning the in directory */
    if ((FD = opendir(directory)) == NULL)
    {
        errx(1, "Error : Failed to open input directory\n");
    }
    unsigned int index = 0;
    while ((in_file = readdir(FD)))
    {
        // Check that we don't have the parent directory
        if (!strcmp(in_file->d_name, "."))
            continue;
        if (!strcmp(in_file->d_name, ".."))
            continue;

        char *filename = in_file->d_name;
        intputPaths[index] = filename;
        index++;
    }
    closedir(FD);

    // Create all images data
    for (unsigned int i = 0; i < NBIMAGES; i++)
    {
        char directory[50];
        strcpy(directory, "Images/");
        strcat(directory, intputPaths[i]);
        intputPaths[i] = directory;
        createData(directory, input[i], expected[i]);
    }
}

int train(int argc, char **argv)
{
    // Need the directory
    if (argc != 2)
    {
        return 1;
    }

    double input[NBIMAGES][NBINPUTS];
    double expected[NBIMAGES][NBOUTPUTS];

    unsigned int epoch = 10000;

    char *intputPaths[NBIMAGES];
    char *directory = argv[1];

    createAllData(directory, intputPaths, input, expected);

    printf("Creating network\n");

    Network n =
        newNetwork(NBINPUTS, NBNODESPERHIDDEN, NBHIDDENLAYERS, NBOUTPUTS);
    Network *network = &n;

    printf("Initing network\n");

    initNetwork(network);

    // launchWeights(network, "Weights/test.txt");

    for (unsigned int i = 0; i <= epoch; i++)
    {
        if (i % (epoch / 10) == 0)
        {
            printf("###### Epoch : %d ######\n", i);
        }
        for (unsigned int j = 0; j < NBIMAGES; j++)
        {
            // checkInputs(input[j]);

            frontPropagationNetwork(network, input[j]);
            backPropagation(network, expected[j]);
            gradientDescent(network);

            if (i % (epoch / 10) == 0)
            {
                printResult(expected[j],
                            network->layers[NBHIDDENLAYERS + 1].neurons);
            }
        }
    }

    // printWeights(network);

    // saveWeights(network, "Weights/test.txt");

    freeNetwork(network);

    return 0;
}

int getNetworkOutput(Network *network, SDL_Surface *image)
{
    double inputs[28 * 28];
    imageToBinary(image, inputs);
    frontPropagationNetwork(network, inputs);

    double temp = network->layers[NBHIDDENLAYERS + 1].neurons[0].value;
    int result = 1;

    for (unsigned int i = 1; i < NBOUTPUTS; i++)
    {
        if (network->layers[NBHIDDENLAYERS + 1].neurons[i].value > temp)
        {
            temp = network->layers[NBHIDDENLAYERS + 1].neurons[i].value;
            result = i + 1;
        }
    }
    return result;
}