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

    unsigned int epoch = 10000;

    printf("Creating network\n");

    Network n = newNetwork(nbInputs, 3, 1, nbOutputs);
    Network* network = &n;

    printf("Initing network\n");

    initNetwork(network);

    for (unsigned int i = 0; i < epoch; i++){
        printf("###### Epoch : %d ######\n", i);
        for (unsigned int j = 0; j < 8; j+=2){
            if (j != 8){
                //printf("Front propagating\n");
                double input[2] = {inputs[j], inputs[j + 1]};
                double expected[1] = {expecteds[j / 2]};
                frontPropagationNetwork(network, input);
                
                //printf("Back propagating\n");
                backPropagation(network, expected);

                //printf("Updating all weight after back propagating\n");
                printf("Input : %f %f\n", input[0], input[1]);
                printf("Output : %f, expected : %f\n\n", network->layers[2].neurons[0].value, expected[0]);
            }
        }
    }

    freeNetwork(network);

    return 0;
}