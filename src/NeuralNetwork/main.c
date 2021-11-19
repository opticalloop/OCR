#include "NeuralNetwork/training.h"
#include <SDL/SDL.h>
#include "Imagery/Utils/image.h"
#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Detection/reverse_color.h"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Specify <epoch_times> <path>");
        return 0;
    }
    // generateDataFile();
    int epoch = atoi(argv[1]);
    train(epoch, 3, 128, 1, "", argv[2]);

    // Network n;
    // n.sizeInput = NBINPUTS;
    // n.sizeOutput = NBOUTPUTS;
    // Network *network = &n;
    // launchWeights(network, "w3.data", 1);
    
    // SDL_Surface *surface = load_image("9.jpg");
    
    // int res = getNetworkOutput(network, surface, 1);
    // printf("Input : 9\n");
    // printf("Ouput : %d\n", res);
    // freeNetwork(network);
    // SDL_FreeSurface(surface);    
    return 0;
}