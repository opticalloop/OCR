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

#define NBIMAGES 36711
#define NBINPUTS 28 * 28
#define NBOUTPUTS 9

/*
 *  Summary:
 *      Print the result of the nework
 *
 *  Params :
 *      expected[] : expected value
 *      neuron[] : all output neuron
 *
 * Return :
 *      Just print so void
 */

void printResult(double expected[], Neuron neuron[]);

/*
 * Summary:
 *     Some times, have difficulty to read image so check is there is no error
 *
 * Params :
 *     inputs[NBINPUTS] : input to check
 *
 * Return :
 *     void
 */

void checkInputs(double inputs[NBINPUTS]);

/*
 * Summary:
 *      Change the image to a binary array (black pixel => 1, white pixel => 0)
 *
 * Params :
 *      *surrace : image to binarize
 *      inputs[] : array to fill
 * Return :
 *      Fill the array inputs so void
 */

void imageToBinary(SDL_Surface *surface, double inputs[]);

/*
 * Summary:
 *      launch pixel value in the intputPaths array, and define expected digit
 *      Consider that the image is already in grayscale
 * Params :
 *      *path : path where to load the image
 *      inputs[] : array to fill with inputs
 *      expected[] : array to fill with expected values (9 numbers, for exemple
 *               0,0,0,0,0,0,1,0,0 represent 7)
 * Return :
 *      Fill the arrays so void
 */
void createData(char *path, double inputs[], double expected[]);

/*
 *  Summary:
 *      Train the neural network
 *
 * Params :
 *      *directory : directory where all images are stored
 *      verbose : print details
 *
 * return :
 *   Void
 */

void train(const unsigned int epoch, const unsigned int nbHiddenLayers,
           const unsigned int nbNodesPerHidden, const int verbose,
           char *launch_path, char *save_path, char *directory);

/*
 *  Summary:
 *      Get the number shown in the image thanks to the network
 *
 * Params :
 *      *network : the network already trained
 *      *image : the image to identify
 *
 * Return :
 *      Number shown in the image
 */

int getNetworkOutput(Network *network, SDL_Surface *image, int verbose);