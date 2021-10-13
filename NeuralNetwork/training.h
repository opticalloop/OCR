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

void printResult(double expected[], Neuron neuron[]);

void checkInputs(double inputs[28 * 28]);

void imageToBinary(SDL_Surface *surface, double inputs[]);

// launch pixel value in the intputPaths array, and define expected digit
// Consider that the image is already in grayscale
void createData(char *path, double intputs[], double expected[]);

void createAllData(char *directory, char *intputPaths[],
                   double input[NBIMAGES][28 * 28],
                   double expected[NBIMAGES][9]);

int train(int argc, char **argv);

int getNetworkOutput(Network *network, SDL_Surface *image);