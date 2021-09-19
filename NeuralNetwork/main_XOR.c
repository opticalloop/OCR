#include <stdio.h>
#include "neural_network.h"

static void printWeights(Network *network)
{
    printf("\n######## ALL WEIGHTS ########\n");
    for (unsigned int i = 0; i < network->nbLayers; i++)
    {
        printf("###### LAYER %u ######\n", i);
        for (unsigned int j = 0; j < network->layers[i].nbNeurons; j++)
        {
            printf("#### NEURONS %u #### \n", j);
            for (unsigned int k = 0;
                 k < network->layers[i].neurons[j].nbWeights; k++)
            {
                printf("Weight %u : %f\n", k,
                    network->layers[i].neurons[j].weights[k]);
            }
        }
    }
}

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

    unsigned int epoch = 1000000;

    unsigned int nbHiddenLayers = 2;
    unsigned int nbNodesPerHidden = 5;

    printf("Creating network\n");

    Network n
        = newNetwork(nbInputs, nbNodesPerHidden, nbHiddenLayers, nbOutputs);
    Network *network = &n;

    printf("Initing network\n");

    initNetwork(network);

    for (unsigned int i = 0; i <= epoch; i++)
    {
        if (i % 10000 == 0)
        {
            printf("###### Epoch : %d ######\n", i);
        }
        for (unsigned int j = 0; j < 8; j += 2)
        {
            if (j != 8)
            {
                double input[2] = {inputs[j], inputs[j + 1]};
                double expected[1] = {expecteds[j / 2]};

                frontPropagationNetwork(network, input);
                backPropagation(network, expected);
                gradientDescent(network);

                if (i % 10000 == 0)
                {
                    printf("Input : %u %u\n", (unsigned int)input[0],
                        (unsigned int)input[1]);
                    printf("Output : %f, expected : %u\n\n",
                        network->layers[nbHiddenLayers + 1].neurons[0].value,
                        (unsigned int)expected[0]);
                }
            }
        }
    }

    printWeights(network);

    freeNetwork(network);

    return 0;
}