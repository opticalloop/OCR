#include <stdio.h>
#include "neural_network_XOR.h"

int main(void)
{
    double inputs[] = {0, 0, 0, 1, 1, 0, 1, 1};
    double expecteds[] = {0, 1, 1, 0};

    unsigned int nbInputs = 8;
    unsigned int nbOutputs = 4;

    unsigned int learningRate = 0.5;
    unsigned int epoch = 20;

    printf("Initing network");

    Network n = newNetwork(nbInputs, 6, 2, nbOutputs);
    Network* network = &n;
    initNetwork(network);

    for (unsigned int i = 0; i < epoch; i++){
        for (unsigned int j = 0; j < nbInputs; j+=2){
            double input[2] = {inputs[j], inputs[j + 1]};
            frontPropagationNetwork(network);
            double expected[1] = {expecteds[j % 2]};
            backPropagation(network, expected);
            updateWeights(network, input, learningRate);
        }
        printf("Epoch : %d\n", i);
        for (unsigned int k = 0; k < network->nbLayers; k++){
            for (unsigned int l = 0; l < network->layers[k].nbNeurons; l++){
                printf("Layer : %d, output : %f", k, network->layers[k].neurons[l].output);
            }
        }
        
    }

    return 0;
}