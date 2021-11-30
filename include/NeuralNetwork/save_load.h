#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "NeuralNetwork/neural_network.h"

/*
 * Summary:
 *      Save the weights of the network in the speicied path
 *
 * Params :
 *      *network : the network
 *      *path : path where to save the weights
 *
 * Return :
 *      Write in a file so void
 */

void saveWeights(Network *network, char *path);

/*
 * Summary:
 *       Load weights of the file in paramter
 *
 * Params :
 *      *network : the network where to load the weights
 *      *path : path where from where to load the weights
 *
 * Return :
 *      directly change the network in paramters so void
 */

void launchWeights(Network *network, char *path, int verbose, int gui);

#endif