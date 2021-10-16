#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "NeuralNetwork/neural_network.h"

void saveWeights(Network *network, char *path);

void launchWeights(Network *network, char *path);

#endif