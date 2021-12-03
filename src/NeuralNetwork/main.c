#include <SDL/SDL.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Detection/reverse_color.h"
#include "Imagery/Utils/image.h"
#include "NeuralNetwork/training.h"


int training_test(int argc, char **argv)
{
    // generateDataFile();
    train(10, 1, 15, 1, "", argv[1]);
    
    return 0;
}