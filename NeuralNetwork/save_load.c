#include <ctype.h> // To use toupper()
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define _OPEN_SYS_ITOA_EXT

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
                writeToFile(file, network->layers[i].neurons[j].weights[k], k == 0 ? "" : " | ");
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

    char chr;

    // For each character
    while ((chr = getc(file)) != EOF){
        if (chr == '\n'){
            
        }
    }

    // Close file 
    fclose(file);
}