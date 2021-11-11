#include "NeuralNetwork/training.h"
#include <SDL/SDL.h>

int main(void)
{
    // train(10, 2, 16, 1, "", "w.data");

    Network n;
    n.sizeInput = NBINPUTS;
    n.sizeOutput = NBOUTPUTS;
    Network *network = &n;
    launchWeights(network, "w.data", 1);
    
    SDL_Surface *surface = load_image("Digits-Only/digit_0_9.jpg");
    
    int res = getNetworkOutput(network, surface, 1);
    printf("Input : 9\n");
    printf("Ouput : %d\n", res);
    freeNetwork(network);
    SDL_FreeSurface(surface);
}