#include "NeuralNetwork/training.h"

/*
 * Clamp the double given between max and min
 */
static double clamp(double val, double max, double min)
{
    return val > max ? max : val < min ? min : val;
}

void printResult(double expected[], Neuron neuron[])
{
    // Print expected
    for (unsigned int l = 0; l < NBOUTPUTS; l++)
    {
        if (expected[l] == 1.0)
        {
            printf("\n--> 📈 Input : image of a %u\n", l + 1);
            break;
        }
    }
    int index = 1;
    double temp = neuron[0].value;
    for (unsigned int k = 1; k < NBOUTPUTS; k++)
    {
        if (neuron[k].value > temp)
        {
            temp = neuron[k].value;
            index = k + 1;
        }
    }
    printf("<-- 📉 Output : %d\n", index);
}

void checkInputs(double inputs[NBINPUTS])
{
    for (unsigned int i = 0; i < NBINPUTS; i++)
    {
        if (inputs[i] > 234124.0)
        {
            inputs[i] = 1;
        }
    }
}

void imageToBinary(SDL_Surface *surface, double inputs[])
{
    SDL_Color rgb;
    Uint32 pixel;
    for (unsigned int i = 0; i < 28; i++)
    {
        for (unsigned int j = 0; j < 28; j++)
        {
            // Get pixel colors
            pixel = get_pixel(surface, i, j);
            SDL_GetRGB(pixel, surface->format, &rgb.r, &rgb.g, &rgb.b);

            inputs[j * 28 + i] = clamp(1.0 - ((double)rgb.r / 255.0), 1.0, 0.0);
        }
    }
}

void createData(char *path, double inputs[], double expected[])
{
    char *ptr = path;

    SDL_Surface *surface = load_image(ptr);

    imageToBinary(surface, inputs);

    // Get expected value
    int num = path[strlen(path) - 5] - '0';

    // Init expected value
    for (int i = 0; i < NBOUTPUTS; i++)
    {
        expected[i] = i + 1 == num ? 1.0 : 0.0;
    }

    // printf("Expected : %d\n", num);

    SDL_FreeSurface(surface);
}

void train(const unsigned int epoch, const unsigned int nbHiddenLayers,
           const unsigned int nbNodesPerHidden, const int verbose,
           char *launch_path, char *save_path, char *directory)
{
    if (verbose)
    {
        printf("    🔍 Launching Neural Network with %u hidden layers and %u "
               "nodes per hidden\n",
               nbHiddenLayers, nbNodesPerHidden);
    }

    if (verbose)
    {
        printf("    🔨 Creating network\n");
    }

    Network n;
    n.sizeInput = NBINPUTS;
    n.sizeOutput = NBOUTPUTS;
    Network *network = &n;

    if (!strcmp(launch_path, ""))
    {
        *network =
            newNetwork(NBINPUTS, nbNodesPerHidden, nbHiddenLayers, NBOUTPUTS);
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
    double input[NBINPUTS];
    double expected[NBOUTPUTS];

    DIR *FD;
    struct dirent *in_file;

    for (unsigned int i = 0; i <= epoch; i++)
    {
        if (i == epoch && verbose)
        {
            printf("\n    📊 ###### EPOCH %u ######\n", i);
        }

        // printf("Directory %s\n", directory);

        // Open the directory
        if ((FD = opendir(directory)) == NULL)
        {
            freeNetwork(network);
            errx(1, "Error : Failed to open input directory\n");
        }

        in_file = readdir(FD);
        for (unsigned int j = 0; j < NBIMAGES; j++, in_file = readdir(FD))
        {
            while (!strcmp(in_file->d_name, ".")
                   || !strcmp(in_file->d_name, ".."))
            {
                in_file = readdir(FD);
            }

            char str[1000];
            strcpy(str, "src/NeuralNetwork/Digits-Only/");
            strcat(str, in_file->d_name);

            createData(str, input, expected);

            frontPropagation(network, input);
            errorRate = backPropagation(network, expected);
            gradientDescent(network);

            if (i == epoch && verbose)
            {
                printResult(expected,
                            network->layers[nbHiddenLayers + 1].neurons);
            }
        }
        closedir(FD);
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

int getNetworkOutput(Network *network, SDL_Surface *image)
{
    double inputs[NBINPUTS];
    imageToBinary(image, inputs);
    frontPropagation(network, inputs);

    double temp = network->layers[(network->nbLayers - 2) + 1].neurons[0].value;
    int result = 1;

    for (unsigned int i = 1; i < NBOUTPUTS; i++)
    {
        if (network->layers[(network->nbLayers - 2) + 1].neurons[i].value
            > temp)
        {
            temp =
                network->layers[(network->nbLayers - 2) + 1].neurons[i].value;
            result = i + 1;
        }
    }
    return result;
}