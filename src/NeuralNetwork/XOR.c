#include "NeuralNetwork/XOR.h"

#include <stdio.h>

#include "NeuralNetwork/save_load.h"

void launchXOR(unsigned int nbHiddenLayers, unsigned int nbNodesPerHidden,
               int verbose, char *launch_path, char *save_path)
{
    if (verbose)
    {
        printf("    🔍 Launching XOR with %u hidden layers and %u nodes per "
               "hidden\n",
               nbHiddenLayers, nbNodesPerHidden);
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

    // Does we need to launch weights ?
    if (!strcmp(launch_path, ""))
    {
        if (verbose)
        {
            printf("    🎰 Initing network\n");
        }
        initNetwork(network);
    }
    else
    {
        if (verbose)
        {
            printf("--> 💾 Initing weights from %s\n", launch_path);
        }
        launchWeights(network, launch_path);
    }

    double errorRate = 0.0;

    for (unsigned int i = 0; i <= epoch; i++)
    {
        if (i == epoch && verbose)
        {
            printf("\n    📊 ###### EPOCH %u ######\n", i);
        }
        for (unsigned int j = 0; j < 8; j += 2)
        {
            if (j != 8)
            {
                double input[2] = { inputs[j], inputs[j + 1] };
                double expected[1] = { expecteds[j / 2] };

                frontPropagation(network, input);
                errorRate = backPropagation(network, expected);
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
        printf("    ❗ Error rate = %f\n", errorRate);
    }

    if (strcmp(save_path, ""))
    {
        if (verbose)
        {
            printf("<-- 💾 Saving weights to %s\n", save_path);
        }
        saveWeights(network, save_path);
    }

    printf("    ✅ Done\n");

    freeNetwork(network);
}