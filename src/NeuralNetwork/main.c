#include <SDL/SDL.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Detection/reverse_color.h"
#include "Imagery/Utils/image.h"
#include "NeuralNetwork/training.h"

int main(void)
{
    // generateDataFile();
    train(100, 2, 32, 1, "", "w.data");

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
}