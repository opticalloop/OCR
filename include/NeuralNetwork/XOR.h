#ifndef XOR_H
#define XOR_H

#include <stdio.h>
#include <string.h>

#include "NeuralNetwork/neural_network.h"
#include "NeuralNetwork/save_load.h"

/**
 * @brief
 *
 * @param epoch
 * @param nbHiddenLayers
 * @param nbNodesPerHidden
 * @param verbose
 * @param launch_path
 * @param save_path
 */
void launchXOR(unsigned int epoch, unsigned int nbHiddenLayers,
               unsigned int nbNodesPerHidden, int verbose, char *launch_path,
               char *save_path);

#endif