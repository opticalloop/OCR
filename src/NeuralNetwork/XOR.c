#include "NeuralNetwork/XOR.h"

#include <stdio.h>

void launchXOR(unsigned int nbHiddenLayers, unsigned int nbNodesPerHidden, int verbose)
{
    if (verbose)
    {
        printf("    🔍 Launching XOR with %u hidden layers and %u nodes per hidden\n", nbHiddenLayers, nbNodesPerHidden);
    }
    // 00 : 0
    // 01 : 1
    // 10 : 1
    // 11 : 0
    double inputs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0 };
    double expecteds[] = { 0.0, 1.0, 1.0, 0.0 };

    unsigned int nbInputs = 2;
    unsigned int nbOutputs = 1;

    unsigned int epoch = 1000;

    if (verbose)
    {
        printf("    🔨 Creating network\n");
    }

    Network n =
        newNetwork(nbInputs, nbNodesPerHidden, nbHiddenLayers, nbOutputs);
    Network *network = &n;

    if (verbose)
    {
        printf("    🔍 Initing network\n");
    }

    initNetwork(network);

    for (unsigned int i = 0; i <= epoch; i++)
    {
        if (i == epoch && verbose)
        {
            printf("    📊 ###### EPOCH %u ######\n", i);
        }
        for (unsigned int j = 0; j < 8; j += 2)
        {
            if (j != 8)
            {
                double input[2] = { inputs[j], inputs[j + 1] };
                double expected[1] = { expecteds[j / 2] };

                frontPropagation(network, input);
                backPropagation(network, expected);
                gradientDescent(network);

                if (i == epoch && verbose)
                {
                    printf("--> 📈 Input : %u %u\n", (unsigned int)input[0],
                           (unsigned int)input[1]);
                    printf("<-- 📉 Output : %f, expected : %u\n\n",
                           network->layers[nbHiddenLayers + 1].neurons[0].value,
                           (unsigned int)expected[0]);
                }
            }
        }
    }


    if (verbose)
    {
        printf("    ❗ Error rate = %f\n", averageErrorRate(network));
        printf("    ✅ Done\n");
    }
    // printWeights(network);

    freeNetwork(network);
}