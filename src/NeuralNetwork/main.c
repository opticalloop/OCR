#include <SDL/SDL.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Detection/reverse_color.h"
#include "Imagery/Utils/image.h"
#include "NeuralNetwork/training.h"

int main(int argc, char **argv)
{
    // generateDataFile();
    train(100, 1, 15, 1, "", argv[1]);

    Network network;
    network.sizeInput = NBINPUTS;
    network.sizeOutput = NBOUTPUTS;

    launchWeights(&network, argv[1], 1);

    // Network n;
    // n.sizeInput = NBINPUTS;
    // n.sizeOutput = NBOUTPUTS;
    // Network *network = &n;
    // launchWeights(network, "w3.data", 1);

    // Directory where all images are
    DIR *directory;
    struct dirent *in_file;

    // To manage Image To Binary
    int input[NBINPUTS];
    char str[1000];

    // Open the directory
    if ((directory = opendir("Test_data")) == NULL)
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
        strcpy(str, "Test_data/");
        strcat(str, in_file->d_name);

        // Get image binary arrays
        printf("%s\n", str);
        SDL_Surface *surface = load_image(str);
        Image img = newImage(surface, 0, surface->w, surface->h);
        SDL_FreeSurface(surface);
        imageToBinary(&img, input);

        char ch = in_file->d_name[strlen(in_file->d_name) - 5];
        printf("Input : %c\n", ch);

        int res = getNetworkOutput(&network, &img, 0);
        printf("Ouput : %d\n\n", res);
        freeImage(&img, 0);
    }
    closedir(directory);

    int zero_intput[NBINPUTS] = { 0.0 };
    printf("Input : 0\n");
    frontPropagation(&network, zero_intput);
    double temp = network.layers[(network.nbLayers - 2) + 1].neurons[0].value;
    int result = 0;

    for (unsigned int i = 1; i < NBOUTPUTS; i++)
    {
        if (network.layers[(network.nbLayers - 2) + 1].neurons[i].value > temp)
        {
            temp = network.layers[(network.nbLayers - 2) + 1].neurons[i].value;
            result = i;
        }
    }
    printf("Ouput : %d\n\n", result);

    freeNetwork(&network);

    return 0;
}