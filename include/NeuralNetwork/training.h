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

/*
 * Print the result of the nework
 * Params : 
 * expected[] : expected value
 * neuron[] : all output neuron
 * return :
 *  just print so void
 */

void printResult(double expected[], Neuron neuron[]);

/*
 * Some times, have difficulty to read image so check is there is no error
 * Params : 
 * inputs[NBINPUTS] : input to check
 * return :
 *  void
 */

void checkInputs(double inputs[NBINPUTS]);

/*
 * Change the image to a binary array (black pixel => 1, white pixel => 0)
 * Params : 
 *  *surrace : image to binarize 
 *  inputs[] : array to fill
 * return :
 *  fill the array inputs so empty
 */

void imageToBinary(SDL_Surface *surface, double inputs[]);

/*
 * launch pixel value in the intputPaths array, and define expected digit
 * Consider that the image is already in grayscale
 * Params : 
 *  *path : path where to load the image
 *  inputs[] : array to fill with inputs
 *  expected[] : array to fill with expected values (9 numbers, for exemple 0,0,0,0,0,0,1,0,0 represent 7)
 * return :
 *  fill the arrays so empty
 */
void createData(char *path, double intputs[], double expected[]);

/*
 * Call createData on all the image of the directory
 * Params : 
 *  *directory : directory where all images are stored
 *  *intputPaths[] : temporar list for the functions itself
 *  input[][] : two dimentional array where all inputs images are saved
 *  expected[][] : two dimentional array where all expected value are saved
 * return :
 *  fill the arrays so empty
 */
void createAllData(char *directory, char *intputPaths[],
                   double input[NBIMAGES][NBINPUTS],
                   double expected[NBIMAGES][NBOUTPUTS]);

/*
 * Train the neural network
 * Params : 
 *  *directory : directory where all images are stored
 *  verbose : print details
 * return :
 *   so void
 */

int train(char *directory, int verbose);

/*
 * Get the number shown in the image thanks to the network
 * Params : 
 *  *network : the network already trained
 *  *image : the image to identify
 * return :
 *   number shown in the image
 */

int getNetworkOutput(Network *network, SDL_Surface *image);