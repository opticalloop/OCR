#include "NeuralNetwork/save_load.h"

#include <ctype.h> // To use toupper()
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char isNumber(char text)
{
    if ((text < '0' || text > '9'))
        return 0;
    return 1;
}

static void writeToFile(FILE *path, double number, char *extra)
{
    char str[20];

    // Writing a neuron or a layer ?
    if (strstr(extra, "#") != NULL)
    {
        fputs("\n", path);
        fputs(extra, path);
        sprintf(str, "%d", (int)number);
    }
    else
    {
        // Writing a weight
        sprintf(str, "%f", number);
        fputs(extra, path);
    }
    fputs(str, path);

    // New line when writing a neuron and layer
    if (strstr(extra, "#") != NULL)
    {
        fputs("\n", path);
    }
}

void saveWeights(Network *network, char *path)
{
    // Check if file already exist
    if (access(path, F_OK) == 0)
    {
        printf("Specified file path to save the neural network already exist\n"
               "Do you want to overwrite it ? [Y/n] : ");

        // Get input result, should be y or n (caps doens't import)
        char answer = toupper(getchar());
        while (answer != 'Y' && answer != 'N')
        {
            printf("\n Answer by entering Y or N : ");
            answer = toupper(getchar());
        }
        if (answer == 'N')
        {
            printf("Returned");
            return;
        }
    }

    // Open file
    FILE *file;
    file = fopen(path, "w");

    // Write number of hidden layers and node per hidden
    char str[50];
    if (network->nbLayers - 2 > 0)
    {
        sprintf(str, "%d|%d\n", (int)(network->nbLayers - 2),
                network->layers[1].nbNeurons);
    }
    else
    {
        sprintf(str, "0|0\n");
    }
    fputs(str, file);

    // First layer don't have any weight
    for (unsigned int i = 1; i < network->nbLayers; i++)
    {
        // writeToFile(file, (double)i, "## ");
        fputs("##", file);
        for (unsigned int j = 0; j < network->layers[i].nbNeurons; j++)
        {
            // writeToFile(file, (double)j, "# ");
            fputs("\n#\n", file);
            for (unsigned int k = 0;
                 k < network->layers[i].neurons[j].nbWeights; k++)
            {
                writeToFile(file, network->layers[i].neurons[j].weights[k],
                            k == 0 ? "" : "|");
            }
            fputs("|\n", file);
        }
    }

    // Close file
    fclose(file);
}

void launchWeights(Network *network, char *path, int verbose)
{
    if (verbose)
    {
        printf("--> 💾 Initing weights from %s\n", path);
    }

    // Open file
    FILE *file;
    file = fopen(path, "r");

    char chr = getc(file);
    char tempStr[50];

    int nbHidden = 0;
    int nbNodePerHidden = 0;

    // Check number of hidden and hidden
    char str[1000];
    memset(str, 0, sizeof(str));
    while (chr != EOF && chr != '|' && isNumber(chr))
    {
        strncat(str, &chr, 1);
        chr = getc(file);
    }
    if (chr != EOF)
    {
        nbHidden = atoi(str);
    }
    else
    {
        errx(EXIT_FAILURE, "File too short");
    }
    memset(str, 0, sizeof(str));
    chr = getc(file);
    while (chr != EOF && chr != '\n')
    {
        strncat(str, &chr, 1);
        chr = getc(file);
    }
    if (chr != EOF)
    {
        nbNodePerHidden = atoi(str);
    }
    else
    {
        errx(EXIT_FAILURE, "File too short");
    }

    *network = newNetwork(network->sizeInput, nbNodePerHidden, nbHidden,
                          network->sizeOutput);

    int layerIndex = 0; // To begin at 1
    int neuronIndex = -1; // To begin at 0
    int weightIndex = 0;

    memset(tempStr, 0, sizeof(tempStr));
    // For each character
    while ((chr = getc(file)) != EOF)
    {
        printf("chr : %c\n", chr);
        // New neuron or layer
        if (chr == '#')
        {
            chr = getc(file);
            // New layer
            if (chr == '#')
            {
                layerIndex++;
                neuronIndex = -1;
                weightIndex = 0;
            }
            // New neuron
            else if (chr == '\n')
            {
                neuronIndex++;
                weightIndex = 0;
            }
        }
        else if (chr == ' ' || chr == '\n')
        {
            continue;
        }
        else if (chr == '|')
        {
            printf("Layer %d Neuron %d Weight %d : %f\n", layerIndex,
                   neuronIndex, weightIndex, atof(tempStr));

            // Save weights
            network->layers[layerIndex]
                .neurons[neuronIndex]
                .weights[weightIndex] = atof(tempStr);

            // Reset string
            memset(tempStr, 0, sizeof(tempStr));
            weightIndex++;
        }
        else
        {
            // Append digit or - and .
            strncat(tempStr, &chr, 1);
        }
    }

    // Close file
    fclose(file);
}