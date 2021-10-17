#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "NeuralNetwork/neural_network.h"

/*
 * Save the weights of the network in the speicied path
 * Params : 
 *  *network : the network
 *  *path : path where to save the weights
 * return :
 *  write in a file so void
 */

void saveWeights(Network *network, char *path);

/*
 * Load weights of the file in paramter
 * Params : 
 *  *network : the network where to load the weights
 *  *path : path where from where to load the weights
 * return :
 *  directly change the network in paramters so void
 */

void launchWeights(Network *network, char *path);

#endif