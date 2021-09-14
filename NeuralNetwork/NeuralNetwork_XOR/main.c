#include <stdio.h>
#include "neural_network_XOR.h"

int main(void)
{
    // 00 : 0
    // 01 : 1
    // 10 : 1
    // 11 : 0
    double inputs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0};
    double expecteds[] = {0.0, 1.0, 1.0, 0.0};
    
    unsigned int nbInputs = 2;
    unsigned int nbOutputs = 1;

    unsigned int learningRate = 0.1;
    unsigned int epoch = 20;

    printf("Creating network\n");

    Network n = newNetwork(nbInputs, nbInputs, 2, nbOutputs);
    Network* network = &n;

    printf("Initing network\n");

    initNetwork(network);

    for (unsigned int i = 0; i < epoch; i++){
        printf("Epoch : %d\n", i);
        for (unsigned int j = 0; j < 8; j+=2){
            if (j != 8){
                printf("Front propagating\n");
                double input[2] = {inputs[j], inputs[j + 1]};
                double expected[1] = {expecteds[j % 2]};
                frontPropagationNetwork(network, input, expected);
                
                printf("Back propagating\n");
                backPropagation(network);

                printf("Updating all weight after back propagating\n");
                updateWeights(network, input, learningRate);
            }
        }
        for (unsigned int k = 0; k < network->nbLayers; k++){
            for (unsigned int l = 0; l < network->layers[k].nbNeurons; l++){
                printf("Layer : %d, output : %f\n", k, network->layers[k].neurons[l].value);
            }
        }
    }

    freeNetwork(network);

    return 0;
}