#include "ocr.h"

#define WEIGHT_PATH "src/NeuralNetwork/Weights/weights.txt"

static void printVerbose(int verbose, char *message)
{
    if (verbose)
    {
        printf("%s", message);
    }
}

void OCR(char *image_path, char *output_path, int verbose)
{
    // Create image
    printVerbose(verbose, "--> 📂 Creating image from %s\n");
    Image image;
    image.path = image_path;

    newImage(&image);

    // Preprocessing
    printVerbose(verbose, "    🎨 Preprocessing image\n");
    Preprocessing(&image);

    // Segmentation
    SDL_Surface **all_cases = NULL;

    // Free image
    freeImage(&image);

    // Recognisation + Construction
    printVerbose(verbose, "    📊 Creating neural network\n");
    Network network = newNetwork(NBINPUTS, 2, 32, NBOUTPUTS);

    printVerbose(verbose, "    💾 Initing weights\n");
    launchWeights(&network, WEIGHT_PATH);

    printVerbose(verbose, "    🔨 Creating sudoku grid\n");
    unsigned int grid[dim][dim];
    for (unsigned int i = 0; i < dim; i++)
    {
        for (unsigned int j = 0; j < dim; j++)
        {
            grid[i][j] = getNetworkOutput(&network, all_cases[i * dim + j]);
        }
    }
    freeNetwork(&network);

    // Solver
    if (!isSolvable(grid))
    {
        errx(EXIT_FAILURE, "    ⛔ The grid is not solvable");
    }
    printVerbose(verbose, "    ✅ Grid is solvable\n");
    solveSuduko(grid, 0, 0);

    if (!isSolved(grid))
    {
        errx(EXIT_FAILURE, "    ⛔ Error while solving grid");
    }
    printVerbose(verbose, "    ✅ Grid is solved\n");

    // SaveResult
    saveGrid(grid, output_path, verbose);
}