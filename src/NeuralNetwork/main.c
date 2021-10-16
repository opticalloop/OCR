#include <SDL/SDL.h>

#include "Imagery/Utils/op.h"
#include "NeuralNetwork/neural_network.h"
#include "NeuralNetwork/save_load.h"
#include "NeuralNetwork/training.h"

int main(void)
{
    Network n =
        newNetwork(NBINPUTS, NBNODESPERHIDDEN, NBHIDDENLAYERS, NBOUTPUTS);
    Network *network = &n;
    launchWeights(network, "Weights/weights.txt");
    // printWeights(network);
    SDL_Surface *s = load_image("Digits-Only/digit_0_6.jpg");
    printf("Result : %d\n", getNetworkOutput(network, s));
    freeNetwork(network);
    SDL_FreeSurface(s);
    return 0;
}