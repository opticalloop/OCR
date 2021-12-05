#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "NeuralNetwork/neural_network.h"
#include "verbose.h"

/**
 * @brief
 *
 * @param network
 * @param path
 */
void saveWeights(Network *network, char *path);

/**
 * @brief
 *
 * @param network
 * @param path
 * @param verbose
 * @param gui
 */
void launchWeights(Network *network, char *path, int verbose, int gui);

#endif