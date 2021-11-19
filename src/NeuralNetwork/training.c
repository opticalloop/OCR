#include "NeuralNetwork/training.h"

#define DATA_PATH "Digits-Only/"
#define DATA_FILE_PATH "data.txt"

void printResult(double expected[], Neuron neuron[])
{
    // Print expected
    for (unsigned int l = 0; l < NBOUTPUTS; l++)
    {
        if (expected[l] == 1.0)
        {
            printf("\n--> 📈 Input : image of a %u\n", l);
            break;
        }
    }
    int index = 0;
    double temp = neuron[0].value;
    for (unsigned int k = 1; k < NBOUTPUTS; k++)
    {
        if (neuron[k].value > temp)
        {
            temp = neuron[k].value;
            index = k;
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

void imageToBinary(SDL_Surface *surface, int inputs[])
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

            // Black and white
            if ((rgb.r + rgb.g + rgb.b) / 3 > 128)
            {
                // White are 0
                inputs[j * 28 + i] = 0;
            }
            else
            {
                // Black 1
                inputs[j * 28 + i] = 1;
            }
        }
    }
}

void createData(FILE *file, int inputs[NBINPUTS], double expected[NBOUTPUTS], char *lastChr)
{
    char ch;
    unsigned int input_index = 0;
    for (int i = 0; i < NBOUTPUTS; i++)
    {
        expected[i] = 0.0;
    }
    while ((ch = fgetc(file)) != EOF)
    {
        // Get expected value
        if (ch == '#')
        {
            ch = fgetc(file);
            int expected_value;

            if (ch >= '0' && ch <= '9')
            {
                expected_value = ch - '0';
            }
            else
            {
                expected_value = ch - 'A' + 10;
            }
            expected[expected_value] = 1.0;
        }
        else if (ch == '\n')
        {
            break;
        }
        else
        {
            // 0 or 1
            inputs[input_index] = ch - '0';
            input_index++;
        }
    }
    *lastChr = ch;
}

void generateDataFile(void)
{
    // File where to write
    FILE *file;
    file = fopen(DATA_FILE_PATH, "w");

    // Directory where all images are
    DIR *directory;
    struct dirent *in_file;

    // To manage Image To Binary
    int input[NBINPUTS];
    char str[1000];

    // Open the directory
    if ((directory = opendir(DATA_PATH)) == NULL)
    {
        errx(1, "Error : Failed to open input directory\n");
    }
    int count = 0;
    while ((in_file = readdir(directory)) != NULL)
    {
        if (!strcmp(in_file->d_name, ".") || !strcmp(in_file->d_name, ".."))
        {
            continue;
        }

        // Compute image path
        strcpy(str, DATA_PATH);
        strcat(str, in_file->d_name);

        // Get image binary arrays
        SDL_Surface *surface = load_image(str);
        imageToBinary(surface, input);
        SDL_FreeSurface(surface);

        // Get expected value
        char ch = in_file->d_name[strlen(in_file->d_name) - 5];
        
        // Write expected value
        char strTemp[50];
        sprintf(strTemp, "%c", ch);
        fputs("#", file);
        fputs(strTemp, file);

        // Write the image after binary conversion
        for (int i = 0; i < NBINPUTS; i++)
        {
            memset(strTemp, 0, sizeof(strTemp));
            sprintf(strTemp, "%d", input[i]);
            fputs(strTemp, file);
        }
        fputs("\n", file);
        count++;
        if (count % 100 == 0)
            printf("%d/%d\n", count, NBIMAGES);
    }
    closedir(directory);
    fclose(file);
}

void train(const unsigned int epoch, const unsigned int nbHiddenLayers,
           const unsigned int nbNodesPerHidden, const int verbose,
           char *launch_path, char *save_path)
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
        launchWeights(network, launch_path, verbose);
    }

    double errorRate;
    int input[NBINPUTS];
    double expected[NBOUTPUTS];

    // Init 0 expectation
    int zero_intput[NBINPUTS] = {0};
    double zero_expected[NBOUTPUTS] = {0.0};
    zero_expected[0] = 1.0;

    // Open file where data is
    FILE *file;
    char lastchr; // To know where the end of the file is

    int train_count = 0;
    for (unsigned int i = 0; i <= epoch; i++)
    {
        train_count = 0;
        errorRate = 0.0;
        if (i == epoch && verbose)
        {
            printf("\n    📊 ###### EPOCH %u ######\n", i);
        }
    	file = fopen(DATA_FILE_PATH, "r");
        for (;lastchr != EOF; train_count++)
        {
            createData(file, input, expected, &lastchr);

            frontPropagation(network, input);
            errorRate += backPropagation(network, expected);
            gradientDescent(network);

            if (i == epoch && verbose)
            {
                printResult(expected,
                            network->layers[nbHiddenLayers + 1].neurons);
            }

            // Train for the blank image
            if (train_count % NBOUTPUTS == 0)
            {
                frontPropagation(network, zero_intput);
                errorRate += backPropagation(network, zero_expected);
                gradientDescent(network);
            
                 if (i == epoch && verbose)
                {
                    printResult(zero_expected,
                            network->layers[nbHiddenLayers + 1].neurons);
                }
            }            
        }
        fclose(file);
        lastchr = ' ';
        
        if (verbose)
        {
            printf("    ❗ Error rate = %f\n", errorRate / NBIMAGES);
        }
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

int getNetworkOutput(Network *network, SDL_Surface *image, int verbose)
{
    if (verbose)
    {
        printf("    📈 Getting network output\n");
    }
    int inputs[NBINPUTS];
    imageToBinary(image, inputs);
    frontPropagation(network, inputs);

    double temp = network->layers[(network->nbLayers - 2) + 1].neurons[0].value;
    int result = 0;

    for (unsigned int i = 1; i < NBOUTPUTS; i++)
    {
        if (network->layers[(network->nbLayers - 2) + 1].neurons[i].value
            > temp)
        {
            temp =
                network->layers[(network->nbLayers - 2) + 1].neurons[i].value;
            result = i;
        }
    }
    return result;
}
