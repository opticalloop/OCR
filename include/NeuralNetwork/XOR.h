#ifndef XOR_H
#define XOR_H

#include "NeuralNetwork/neural_network.h"

void launchXOR(unsigned int nbHiddenLayers, unsigned int nbNodesPerHidden,
               int verbose, char *launch_path, char *save_path);

#endif