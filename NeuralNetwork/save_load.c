#include <ctype.h> // To use toupper()
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "save_load.h"

static void writeToFile(FILE* path, double number, char* extra)
{
    char str[20];

    // Writing a neuron or a layer ?
    if (strstr(extra, "#") != NULL) {
        fputs("\n", path);
        fputs(extra, path);
        sprintf(str, "%d", (int) number);
    }
    else{
        // Writing a weight
        sprintf(str, "%f", number);
        fputs(extra, path);
    }
    fputs(str, path);

    // New line when writing a neuron and layer
    if (strstr(extra, "#") != NULL) {
        fputs("\n", path);
    }
}

void saveWeights(Network* network, char* path)
{
    // Check if file already exist
    if(access(path, F_OK) == 0){
        printf("Specified file path to save the neural network already exist\n"
               "Do you want to overwrite it ? [Y/n] ");
        
        // Get input result, should be y or n (caps doens't import)
        char answer = toupper(getchar());
        while (answer != 'Y' && answer != 'N'){
            printf("\n Answer by entering Y or N : ");
            answer = toupper(getchar());
        }
        if (answer == 'N'){
            printf("Returned");
            return;
        }
    }

    // Open file
    FILE* file;
    file = fopen(path, "w");

    // First layer don't have any weight
    for (unsigned int i = 1; i < network->nbLayers; i++){
        writeToFile(file, (double) i, "## ");

        for (unsigned int j = 0; j < network->layers[i].nbNeurons; j++){
            writeToFile(file, (double) j, "# ");
        
            for (unsigned int k = 0; k < network->layers[i].neurons[j].nbWeights; k++){
                writeToFile(file, network->layers[i].neurons[j].weights[k], k == 0 ? "" : "|");
            }
        }
    }

    // Close file 
    fclose(file);
}

void launchWeights(Network* network, char* path)
{
    // Open file
    FILE* file;
    file = fopen(path, "r");

    int layerIndex = 0;
    int neuronIndex = 0;
    int weightIndex = 0;

    char chr;
    char tempStr[50];

    // For each character
    while ((chr = getc(file)) != EOF){
        printf("chr : %c\n", chr);
        // New neuron or layer
        if (chr == '#'){
            chr = getc(file);
            // New layer
            if (chr == '#'){
                chr = getc(file); // Should be a ' '
                chr = getc(file); // Number of the layer
                layerIndex = (int) ((int) chr - (int)'0');
                printf("New layer index : %d\n", layerIndex);
                weightIndex = 0;
                neuronIndex = 0;
            } 
            // New neuron
            else if (chr == ' '){
                chr = getc(file); // Number of the neuron
                neuronIndex = (int) ((int) chr - (int)'0');
                printf("New neuron index : %d\n", neuronIndex);
                weightIndex = 0;
            }
        }
        else if (chr == ' ' || chr == '\n'){
            continue;
        }
        else if(chr == '|'){ // Save weights
            network->layers[layerIndex]
            .neurons[neuronIndex].weights[weightIndex] = atof(tempStr);
            printf("Weight launched : %f\n", atof(tempStr));
            memset(tempStr, 0, sizeof(tempStr));
            weightIndex++;
        }
        else{
            strncat(tempStr, &chr, 1);
        }
    }

    // Close file 
    fclose(file);
}