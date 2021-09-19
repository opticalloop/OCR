#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
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

static void printWeights(Network *network)
{
    printf("\n######## ALL WEIGHTS ########\n");
    for (unsigned int i = 0; i < network->nbLayers; i++)
    {
        printf("###### LAYER %u ######\n", i);
        for (unsigned int j = 0; j < network->layers[i].nbNeurons; j++)
        {
            printf("#### NEURONS %u #### \n", j);
            for (unsigned int k = 0;
                 k < network->layers[i].neurons[j].nbWeights; k++)
            {
                printf("Weight %u : %f\n", k,
                    network->layers[i].neurons[j].weights[k]);
            }
        }
    }
}

static void printResult(double expected[], Neuron neuron[])
{
    // Print expected
    for (unsigned int l = 0; l < 9; l++)
    {
        if (expected[l] == 1.0)
        {
            printf("\nInput : image of a %u\n", l + 1);
            break;
        }
    }
    for (unsigned int k = 0; k < 9; k++)
    {
        printf("Output : %f, expected : %u\n", neuron[k].value,
            (unsigned int)expected[k]);
    }
}

static void printInput(double inputs[28 * 28])
{
    for (unsigned int i = 0; i < 28 * 28; i++)
    {
        printf("Input : %f\n", inputs[i]);
        if (inputs[i] > 234124.0)
        {
            errx(1, "Too long");
        }
    }
}

// launch pixel value in the intputPaths array, and define expected digit
// Consider that the image is already in grayscale
static void createData(char *path, double intputs[28 * 28], double expected[9])
{
    // Get absolute path
    char actualpath[PATH_MAX + 1];
    char *ptr;

    ptr = realpath(path, actualpath);

    printf("File : %s\n", ptr);
    SDL_Surface *surface = load_image(ptr);

    SDL_Color rgb;
    for (unsigned int i = 0; i < 28; i++)
    {
        for (unsigned int j = 0; j < 28; j++)
        {
            // Get pixel colors
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGB(pixel, surface->format, &rgb.r, &rgb.g, &rgb.b);

            intputs[i * j] = (double)(1.0 - ((double)rgb.r / 255.0));

            // printf("Input in creation of data : %f\n", intputs[i * j]);
        }
    }

    // Get expected value
    int num = path[17] - '0';

    // Init expected value
    for (int i = 0; i < 9; i++)
    {
        expected[i] = i + 1 == num ? 1.0 : 0.0;
    }

    printf("Expected : %d\n", num);

    SDL_FreeSurface(surface);
}

static void createAllData(char *directory, char *intputPaths[],
    double input[][], double expected[][])
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
    for (unsigned int i = 0; i < nbImages; i++)
    {
        char directory[50];
        strcpy(directory, "Images/");
        strcat(directory, intputPaths[i]);
        intputPaths[i] = directory;
        printf("%s\n", directory);
        createData(directory, input[i], expected[i]);
    }
}

int main(int argc, char **argv)
{
    // Need the directory
    if (argc != 2)
    {
        return 1;
    }
    unsigned int nbImages = 4;

    double input[nbImages][28 * 28];
    double expected[nbImages][9];

    unsigned int nbInputs = 28 * 28;
    unsigned int nbOutputs = 9;
    unsigned int nbHiddenLayers = 2;
    unsigned int nbNodesPerHidden = 16;

    unsigned int epoch = 10000;

    char *intputPaths[nbImages];
    char *directory = argv[1];

    createAllData(directory, intputPaths, input, expected);

    printf("Creating network\n");

    Network n
        = newNetwork(nbInputs, nbNodesPerHidden, nbHiddenLayers, nbOutputs);
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
        for (unsigned int j = 0; j < 3; j++)
        {

            // printInput(input[j]);

            frontPropagationNetwork(network, input[j]);
            backPropagation(network, expected[j]);
            gradientDescent(network);

            if (i % (epoch / 10) == 0)
            {
                printResult(
                    expected[j], network->layers[nbHiddenLayers + 1].neurons);
            }
        }
    }

    // printWeights(network);

    // saveWeights(network, "Weights/test.txt");

    freeNetwork(network);

    return 0;
}