#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "Imagery/Utils/image.h"
#include "Imagery/Utils/pixel_operations.h"
#include "NeuralNetwork/neural_network.h"
#include "NeuralNetwork/save_load.h"

#define NBIMAGES 10
#define NBINPUTS 28 * 28
#define NBOUTPUTS 9
#define NBHIDDENLAYERS 2
#define NBNODESPERHIDDEN 16

void printResult(double expected[], Neuron neuron[]);

void checkInputs(double inputs[NBINPUTS]);

void imageToBinary(SDL_Surface *surface, double inputs[]);

// launch pixel value in the intputPaths array, and define expected digit
// Consider that the image is already in grayscale
void createData(char *path, double intputs[], double expected[]);

void createAllData(char *directory, char *intputPaths[],
                   double input[NBIMAGES][NBINPUTS],
                   double expected[NBIMAGES][NBOUTPUTS]);

int train(char *directory);

int getNetworkOutput(Network *network, SDL_Surface *image);