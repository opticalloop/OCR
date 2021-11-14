#include "ocr.h"

#define WEIGHT_PATH "src/NeuralNetwork/Weights/w.data"

#define NB_HIDDEN 2
#define SIZE_HIDDEN 32

static void checkFolderOutput(char *output_folder)
{
    DIR *dir = opendir(output_folder);
    if (dir)
    {
        closedir(dir);
        char delete[200];
        snprintf(delete, sizeof(delete), "rm -rf %s", output_folder);
        if (system(delete))
        {
        }
    }
    char directory[200];
    snprintf(directory, sizeof(directory), "mkdir %s", output_folder);
    if (system(directory))
    {
    }
}

void OCR(char *image_path, char *output_path, int verbose, int save,
         char *output_folder)
{
    if (save)
    {
        // Clean the output folder
        checkFolderOutput(output_folder);
    }

    // Create image
    printVerbose(verbose, "--> 💾 Creating image\n");
    Image image;
    image.path = image_path;
    image.surface = NULL;

    newImage(&image, 1);

    // Preprocessing
    grayscale(&image);
    Preprocessing(&image, output_folder, verbose, save);

    // DETECTION

    printVerbose(verbose, "\n    🔍 2 Grid detection (Hough Transform)\n");
    printVerbose(verbose, "    🎥 2.1 Applying sobel edge detection filter\n");

    SobelEdgeDetection(&image);

    Image drawImage;
    drawImage.path = image_path;
    drawImage.surface =
        SDL_CreateRGBSurface(0, image.width, image.height, 24, 0, 0, 0, 0);
    SDL_BlitSurface(image.surface, NULL, drawImage.surface, NULL);

    newImage(&drawImage, 0);

    saveVerbose(verbose, &image, output_folder, "2.1_Sobel_filter", save, 0);
    printVerbose(verbose, "    🔨 2.2 Launching Hough Transform\n");

    SDL_Surface *cropped_image =
        detection(&image, &drawImage, verbose, save, output_folder);

    // Free image
    freeImage(&image, 1);

    Image cropped;
    cropped.path = image_path;
    cropped.surface = cropped_image;

    newImage(&cropped, 0);

    saveVerbose(verbose, &cropped, output_folder, "2.8_Cropped_image", save, 0);
    printVerbose(verbose, "\n    🪓 3 Segmenting cropped image\n");

    // Segmentation
    // Initialize all case at NULL
    SDL_Surface *all_cases[81];
    if (verbose && save)
        printf("<-- 💾 Saving all 81 digit to %s\n", output_folder);
    split9(&cropped, all_cases, save, output_folder);

    freeImage(&cropped, 0);

    // Recognisation + Construction
    printVerbose(verbose, "\n    ❓ 4 Initing digit recognition\n");
    printVerbose(verbose, "    📊 4.1 Creating neural network\n");

    Network network;
    network.sizeInput = NBINPUTS;
    network.sizeOutput = NBOUTPUTS;

    printVerbose(verbose, "    📑 4.2 Initing weights\n");
    launchWeights(&network, WEIGHT_PATH, verbose);

    printVerbose(verbose, "    🔨 4.3 Creating sudoku grid\n");
    unsigned int grid[dim][dim];
    for (unsigned int i = 0; i < dim; i++)
    {
        for (unsigned int j = 0; j < dim; j++)
        {
            grid[i][j] =
                getNetworkOutput(&network, all_cases[i * dim + j], verbose);
        }
    }
    freeNetwork(&network);

    printVerbose(verbose, "\n    🎲 5 Solving sudoku grid\n");

    // Copy array to have different color when saving the image
    unsigned int copy[dim][dim];
    copyArray(grid, copy);

    // Solver
    if (!isSolvable(grid))
    {
        errx(EXIT_FAILURE, "    ⛔ The grid is not solvable");
    }
    printVerbose(verbose, "    ✅ 5.1 Grid is solvable\n");
    printVerbose(verbose, "    🔍 5.2 Solving grid\n");

    solveSuduko(grid, 0, 0);

    if (!isSolved(grid))
    {
        errx(EXIT_FAILURE, "    ⛔ Error while solving grid");
    }
    printVerbose(verbose, "    ✅ 5.3 Grid is solved\n");

    // SaveResult
    saveGrid(grid, output_path, verbose);

    // Create, save and free the image
    Image sudoku_image = createSudokuImage(grid, copy);
    saveVerbose(verbose, &sudoku_image, output_folder, "Result", save, 1);
}