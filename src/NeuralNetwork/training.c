#include "NeuralNetwork/training.h"

#define DATA_PATH "src/NeuralNetwork/Digits-Only/"
#define DATA_FILE_PATH "src/NeuralNetwork/data.txt"
#define TEST_DATA_PATH "src/NeuralNetwork/Test_data"

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

void imageToBinary(Image *image, int inputs[])
{
    for (unsigned int i = 0; i < 28; i++)
    {
        for (unsigned int j = 0; j < 28; j++)
        {
            // Black and white
            if ((image->pixels[i][j].r + image->pixels[i][j].g
                 + image->pixels[i][j].b)
                    / 3
                > 128)
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

void createData(FILE *file, int inputs[NBINPUTS], double expected[NBOUTPUTS],
                char *lastChr)
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
    int counter = 0;
    char ch;
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
        Image img = newImage(surface, 0, surface->w, surface->h);
        SDL_FreeSurface(surface);
        imageToBinary(&img, input);
        freeImage(&img, 0);

        // Get expected value
        ch = in_file->d_name[strlen(in_file->d_name) - 5];

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
        counter++;
        fputs("\n", file);
        if (counter % 100 == 0)
        {
            printf("%d images written\n", counter);
        }
    }
    closedir(directory);
    fclose(file);
}

pthread_t train_thread(const unsigned int epoch,
                       const unsigned int nbHiddenLayers,
                       const unsigned int nbNodesPerHidden, const int verbose,
                       char *launch_path, char *save_path, int gui)
{
    pthread_t thread;
    Training_data args = { .epoch = epoch,
                           .nbHiddenLayers = nbHiddenLayers,
                           .nbNodesPerHidden = nbNodesPerHidden,
                           .verbose = verbose,
                           .launch_path = launch_path,
                           .save_path = save_path,
                           .gui = gui };
    pthread_create(&thread, NULL, train, (void *)&args);

    return thread;
}

void *train(void *args)
{
    Training_data *data = (Training_data *)args;
    const unsigned int epoch = data->epoch;
    const unsigned int nbHiddenLayers = data->nbHiddenLayers;
    const unsigned int nbNodesPerHidden = 30; 
    const int verbose = data->verbose;
    const int gui = data->gui;
    char *launch_path = data->launch_path;
    char *save_path = data->save_path;

    char print_message[1000];
    snprintf(print_message, sizeof(print_message),
             "    🔍 Launching Neural Network with %u hidden layers and %u "
             "nodes per hidden\n",
             nbHiddenLayers, nbNodesPerHidden);

    printVerbose(verbose, gui, print_message, "terminal_text");
    memset(print_message, 0, sizeof(print_message));
    printVerbose(verbose, gui, "    🔨 Creating network\n", "terminal_text");

    Network n;
    n.sizeInput = NBINPUTS;
    n.sizeOutput = NBOUTPUTS;
    Network *network = &n;

    if (!strcmp(launch_path, ""))
    {
        *network =
            newNetwork(NBINPUTS, nbNodesPerHidden, nbHiddenLayers, NBOUTPUTS);

        printVerbose(verbose, gui, "    🎰 Initing network\n", "terminal_text");

        initNetwork(network);
    }
    else
    {
        launchWeights(network, launch_path, verbose, gui);
    }

    double errorRate;
    int input[NBINPUTS];
    double expected[NBOUTPUTS];

    // Init 0 expectation

    int zero_intput[NBINPUTS] = { 0.0 };
    double zero_expected[NBOUTPUTS] = {
        1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };

    // Open file where data is
    FILE *file;
    char lastchr; // To know where the end of the file is

    int train_count = 0;
    for (unsigned int i = 0; i <= epoch; i++)
    {
        train_count = 0;
        errorRate = 0.0;

        snprintf(print_message, sizeof(print_message),
                 "\n    📊 ###### EPOCH %u ######\n", i);
        printVerbose(verbose, gui, print_message, "terminal_text");
        memset(print_message, 0, sizeof(print_message));

        file = fopen(DATA_FILE_PATH, "r");
        for (; lastchr != EOF; train_count++)
        {
            createData(file, input, expected, &lastchr);

            frontPropagation(network, input);
            errorRate += backPropagation(network, expected);
            gradientDescent(network, 0.001);

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

                gradientDescent(network, 0.001);

                if (i == epoch && verbose)
                {
                    printResult(zero_expected,
                                network->layers[nbHiddenLayers + 1].neurons);
                }
            }
        }
        fclose(file);
        lastchr = ' ';

        snprintf(print_message, sizeof(print_message),
                 "    ❗ Error rate = %f\n", errorRate / NBIMAGES);
        printVerbose(verbose, gui, print_message, "terminal_text");
        memset(print_message, 0, sizeof(print_message));
    }

    testTrain(network);

    if (strcmp(save_path, ""))
    {
        snprintf(print_message, sizeof(print_message),
                 "<-- 💾 Saving weights to %s\n", save_path);
        memset(print_message, 0, sizeof(print_message));
        saveWeights(network, save_path);
    }

    printf("    ✅ Done\n");

    freeNetwork(network);
    pthread_exit(NULL); // Exit thread
}

int getNetworkOutput(Network *network, Image *image, int verbose)
{
    if (verbose)
    {
        printf("    📈 Getting network output\n");
    }
    if (isFullWhite(image))
    {
        return 0;
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

int isFullWhite(Image *image)
{
    for (unsigned int i = 0; i < image->width; i++)
    {
        for (unsigned int j = 0; j < image->height; j++)
        {
            if (image->pixels[i][j].r == 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

void testTrain(Network *network)
{
    // Directory where all images are
    DIR *directory;
    struct dirent *in_file;

    // To manage Image To Binary
    int input[NBINPUTS];
    char str[1000];

    // Open the directory
    if ((directory = opendir(TEST_DATA_PATH)) == NULL)
    {
        errx(1, "Error : Failed to open input directory\n");
    }
    while ((in_file = readdir(directory)) != NULL)
    {
        if (!strcmp(in_file->d_name, ".") || !strcmp(in_file->d_name, ".."))
        {
            continue;
        }

        // Compute image path
        strcpy(str,  TEST_DATA_PATH);
        strcat(str, "/");
        strcat(str, in_file->d_name);

        // Get image binary arrays
        printf("%s\n", str);
        SDL_Surface *surface = load_image(str);
        Image img = newImage(surface, 0, surface->w, surface->h);
        SDL_FreeSurface(surface);
        imageToBinary(&img, input);

        char ch = in_file->d_name[strlen(in_file->d_name) - 5];
        printf("Input : %c\n", ch);

        int res = getNetworkOutput(network, &img, 0);
        printf("Ouput : %d\n\n", res);
        freeImage(&img, 0);
    }
    closedir(directory);

    int zero_intput[NBINPUTS] = { 0.0 };
    printf("Input : 0\n");
    frontPropagation(network, zero_intput);
    double temp = network->layers[(network->nbLayers - 2) + 1].neurons[0].value;
    int result = 0;

    for (unsigned int i = 1; i < NBOUTPUTS; i++)
    {
        if (network->layers[(network->nbLayers - 2) + 1].neurons[i].value > temp)
        {
            temp = network->layers[(network->nbLayers - 2) + 1].neurons[i].value;
            result = i;
        }
    }
    printf("Ouput : %d\n\n", result);
}