#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>
#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "../Imagery/Utils/image.h"
#include "neural_network.h"

static void printWeights(Network *network)
{
    printf("\n######## ALL WEIGHTS ########\n");
    for (unsigned int i = 0; i < network->nbLayers; i++){
        printf("###### LAYER %u ######\n", i);
        for (unsigned int j = 0; j < network->layers[i].nbNeurons; j++){
            printf("#### NEURONS %u #### \n", j);
            for (unsigned int k = 0; k < network->layers[i].neurons[j].nbWeights; k++){
                printf("Weight %u : %f\n", k, network->layers[i].neurons[j].weights[k]);
            }
        }
    }
}

// launch pixel value in the inputs array, and define expected digit
// Consider that the image is already in grayscale
static void createData(char* path, double inputs[28 * 28], double expected[9])
{
    SDL_Surface* surface = load_image(path);

    //printf("W : %u, H : %u\n", surface->w, surface->h);

    SDL_Color rgb;
    for (unsigned int i = 0; i < 28; i++){
        for (unsigned int j = 0; j < 28; j++){
            // Get pixel colors 
            Uint32 pixel = get_pixel(surface, i, j);
            SDL_GetRGB(pixel, surface->format, &rgb.r, &rgb.g, &rgb.b);
            
            if ((rgb.r + rgb.g + rgb.b) / 3 >= 127){
                inputs[i * j] = 0.0;
            }
            else{
                inputs[i * j] = 1.0;
            }
            //printf("Input : %f\n", inputs[i * j]);
        }
    }

	char delim[] = "_";
	char *s = strtok(path, delim);

	while(s != NULL){
		//printf("'%s'\n", s);
		s = strtok(NULL, delim);

        // format : 5.jpg
        if (strstr(s, ".jpg") != NULL) {
            break;
        }
	}  

    uintmax_t num = strtoumax(s, NULL, 10);
    if (num == UINTMAX_MAX && errno == ERANGE){
        errx(1, "Cannot convert file name to expected value");
    }

    //printf("Expected\n");
    // Init expected value
    for (unsigned int i = 0; i < 9; i++){
        if (i + 1 == num){
            expected[i] = 1.0;
        }
        else{
            expected[i] = 0.0;
        }
        //printf(" %f", expected[i]);
    }
    
    SDL_FreeSurface(surface);
}

int main(int argc, char **argv)
{
    if (argc != 2){
        return 1;
    }

    double input[28 * 28];
    double expected[9];
    
    unsigned int nbInputs = 28 * 28;
    unsigned int nbOutputs = 9;
    unsigned int nbHiddenLayers = 1;
    unsigned int nbNodesPerHidden = 5;

    unsigned int nbImages = 4;

    unsigned int epoch = 1000;

    char* inputs[nbImages];
    char* directory = argv[1];

    // Read all images
    DIR* FD;
    struct dirent* in_file;
    /* Scanning the in directory */
    if ((FD = opendir(directory)) == NULL) {
        errx(1, "Error : Failed to open input directory\n");
    }
    unsigned int index = 0;
    while ((in_file = readdir(FD))) {
        // Check that we don't have the parent directory
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))    
            continue;

        char* filename = in_file->d_name;
        printf("\nFile : %s\n", filename);
        strcpy(directory, "Images");
        printf("Directory before cat : %s\n", directory);
        strcat(directory, "/");
        strcat(directory, filename);
        inputs[index] = directory;
        index++;
    }
    closedir(FD);
    
    printf("Creating network\n");

    Network n = newNetwork(nbInputs, nbNodesPerHidden, nbHiddenLayers, nbOutputs);
    Network* network = &n;

    printf("Initing network\n");

    initNetwork(network);

    for (unsigned int i = 0; i <= epoch; i++){
        if (i % 1 == 0){  
            printf("###### Epoch : %d ######\n", i);
        }
        for (unsigned int j = 0; j < nbImages; j++){ 

            createData(inputs[j], input, expected);

            frontPropagationNetwork(network, input);
            
            //printf("Back propagating\n");
            backPropagation(network, expected);
            gradientDescent(network);

            // Print expected
            for (unsigned int l = 0; l < 9; l++){
                if (expected[l] == 1.0){
                    printf("\nInput : image of a %u\n", l + 1);
                    break;
                }
            }
            for (unsigned int k = 0; k < nbOutputs; k++){
                printf("Output : %f, expected : %u\n", network->layers[nbHiddenLayers + 1].neurons[k].value, (unsigned int) expected[k]);
            }
        }
    }

    printWeights(network);

    freeNetwork(network);
    
    return 0;
}