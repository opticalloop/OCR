#ifndef XOR_H
#define XOR_H

#include "NeuralNetwork/neural_network.h"

/*
 * Initialize and train the ocr on the xor function
 * Params :
 *  epoch : number of train
 *  nbHiddenLayers : number of hidden layer of the network
 *  nbNodesPerHidden : number of nodes per hidden layer
 *  verbose : boolean to print the details
 * *launch_path : launch the network from a file ? ("" if not)
 *  *save_path : save the network to a file ? ("" if not)
 * return :
 *  void
 */

void launchXOR(unsigned int epoch, unsigned int nbHiddenLayers,
               unsigned int nbNodesPerHidden, int verbose, char *launch_path,
               char *save_path);

#endif