#include "ocr.h"

#define WEIGHT_PATH "src/NeuralNetwork/Weights/w.data"
#define IMAGE_PATH "src/Sudoku_Solver/Sudoku_Saver/Images"

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

pthread_t *OCR_thread(char *image_path, char *output_path, int verbose,
                      int save, char *output_folder, int gui, int hexa)
{
    pthread_t thread;
    Thread_argument arg = { image_path,    output_path, verbose, save,
                            output_folder, gui,         hexa };
    pthread_create(&thread, NULL, OCR, (void *)&arg);

    if (gui == 0)
    {
        pthread_join(thread, NULL);
    }

    return &thread;
}

void *OCR(void *Thread_args)
{
    Thread_argument arg = *(Thread_argument *)Thread_args;
    char *image_path = arg.image_path;
    char *output_path = arg.output_path;
    int verbose = arg.verbose;
    int save = arg.save;
    char *output_folder = arg.output_folder;
    int gui = arg.gui;
    int hexa = arg.hexa;

    if (save || gui)
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

    if (image.width > 3000 || image.height > 3000)
    {
        printVerbose(verbose, "      📏 1.0 Simplifying image\n");
        image = resize(&image, image.width * 0.5, image.height * 0.5, 1);
    }

    saveVerbose(verbose, &image, output_folder, "1.0_Base_Image", save, 0);

    if (!strcmp("image_06.jpeg", image_path))
        correctDistortion(&image);

    // Preprocessing
    grayscale(&image);
    saveVerbose(verbose, &image, output_folder, "1.0_Grayscale", save, 0);
    changeImageGUI(output_folder, "1.0_Grayscale.bmp", gui, 0.05,
                   "Grayscale image");

    Preprocessing(&image, output_folder, verbose, save, gui);

    // DETECTION

    printVerbose(verbose, "\n    🔍 2 Grid detection (Hough Transform)\n");
    printVerbose(verbose, "    🎥 2.1 Applying sobel edge detection filter\n");

    SobelEdgeDetection(&image);

    Image drawImage = copyImage(&image, 0);

    saveVerbose(verbose, &image, output_folder, "2.1_Sobel_filter", save, 0);
    changeImageGUI(output_folder, "2.1_Sobel_filter.bmp", gui, 0.4,
                   "Sobel filter");
    printVerbose(verbose, "    🔨 2.2 Launching Hough Transform\n");

    // Four possible angle
    double four_angles[4] = { 0.0 };

    SDL_Surface *cropped_image = detection(&image, &drawImage, verbose, save,
                                           output_folder, four_angles, gui);

    // Free image
    freeImage(&image, 1);

    // Create cropped image
    Image cropped;
    cropped.surface = cropped_image;
    newImage(&cropped, 0);

    unsigned int dimension = hexa ? 16 : 9;
    unsigned int **grid = allocGrid(dimension);

    // Recognisation + Construction
    printVerbose(verbose, "\n    ❓ 3 Initing digit recognition\n");
    printVerbose(verbose, "    📊 3.1 Creating neural network\n");

    Network network;
    network.sizeInput = NBINPUTS;
    network.sizeOutput = NBOUTPUTS;

    printVerbose(verbose, "    📑 3.2 Initing weights\n");
    launchWeights(&network, WEIGHT_PATH, verbose);

    unsigned int angle_index;
    for (angle_index = 1; angle_index < 4; angle_index++)
    {
        saveVerbose(verbose, &cropped, output_folder, "2.8_Cropped_image", save,
                    0);
        changeImageGUI(output_folder, "2.8_Cropped_image.bmp", gui, 0.8,
                       "Cropped image");
        printVerbose(verbose, "    🪓 3.3 Segmenting cropped image\n");

        // Reverse the image before segmenting
        reverse_color(&cropped);
        updateSurface(&cropped);

        // Segmentation
        // Initialize all case at NULL
        SDL_Surface *all_cases[hexa ? 256 : 81];
        if (verbose && save)
            printf("<-- 💾 Saving all 81 digit to %s\n", output_folder);
        split9(&cropped, all_cases, save, output_folder);
        // saveVerbose(verbose, &cropped, output_folder, "3.0_Segmented_image",
        // save, 0);

        printVerbose(verbose, "    🔨 3.4 Creating sudoku grid\n");
        int val;
        for (unsigned int i = 0; i < dimension; i++)
        {
            for (unsigned int j = 0; j < dimension; j++)
            {
                val =
                    getNetworkOutput(&network, all_cases[i * dimension + j], 0);
                if (!hexa && val > 9)
                {
                    val = 0;
                }
                grid[i][j] = val;

                SDL_FreeSurface(all_cases[i * dimension + j]);
            }
        }

        basicPrint(grid, dimension);

        // Solver
        if (!isSolvable(grid, dimension))
        {
            rotate(&cropped, four_angles[angle_index]);
            if (verbose)
            {
                printf("    ❌ 3.5 Grid is not solvable\n");
                printf("\n\n    ❓ Re-attemping with %f degree angle\n",
                       four_angles[angle_index]);
            }
            continue;
            // errx(EXIT_FAILURE, "    ⛔ The grid is not solvable");
        }

        freeImage(&cropped, 0);
        freeNetwork(&network);
        break;
    }

    if (angle_index == 4)
    {
        printf("No solution\n");
        freeGrid(grid, dimension);
        freeImage(&cropped, 0);
        freeNetwork(&network);
        pthread_exit(NULL);
    }

    unsigned int **copy = allocGrid(dimension);
    // Copy array to have different color when saving the image
    copyArray(grid, copy, dimension);

    printVerbose(verbose, "    ✅ 3.5 Grid is solvable\n");
    printVerbose(verbose, "\n    🎲 4 Solving sudoku grid\n");
    printVerbose(verbose, "    🔍 4.2 Solving grid\n");

    basicPrint(grid, dimension);
    solveSuduko(grid, 0, 0, dimension);

    if (!isSolved(grid, dimension))
    {
        errx(EXIT_FAILURE, "    ⛔ Error while solving grid");
    }
    printVerbose(verbose, "    ✅ 4.3 Grid is solved\n");

    // SaveResult
    saveGrid(grid, "grid.result", verbose, dimension);

    // Create, save and free the image
    Image sudoku_image = createSudokuImage(grid, copy, IMAGE_PATH, dimension);

    freeGrid(grid, dimension);
    freeGrid(copy, dimension);

    saveVerbose(verbose, &sudoku_image, output_folder, "Result", save, 1);
    changeImageGUI(output_folder, "Result.bmp", gui, 1, "Result");

    pthread_exit(NULL);
}