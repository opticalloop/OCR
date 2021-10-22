#include "NeuralNetwork/XOR.h"

void launchXOR(unsigned int epoch, unsigned int nbHiddenLayers,
               unsigned int nbNodesPerHidden, const int verbose,
               char *launch_path, char *save_path)
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
    const double inputs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0 };
    const double expecteds[] = { 0.0, 1.0, 1.0, 0.0 };

    const unsigned int nbInputs = 2;
    const unsigned int nbOutputs = 1;

    if (verbose)
    {
        printf("    🔨 Creating network\n");
    }

    Network n;
    n.sizeInput = nbInputs;
    n.sizeOutput = nbOutputs;
    Network *network = &n;

    // Does we need to launch weights ?
    if (!strcmp(launch_path, ""))
    {
        *network =
            newNetwork(nbInputs, nbNodesPerHidden, nbHiddenLayers, nbOutputs);
        if (verbose)
        {
            printf("    🎰 Initing network\n");
        }
        initNetwork(network);
    }
    else
    {
        launchWeights(network, launch_path, verbose);
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