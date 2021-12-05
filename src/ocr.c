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
        {}
    }
    char directory[200];
    snprintf(directory, sizeof(directory), "mkdir %s", output_folder);
    if (system(directory))
    {}
}

pthread_t OCR_thread(char *intput_path, char *output_path, int verbose,
                     int save, char *output_folder, int gui, int hexa)
{
    pthread_t thread;
    SDL_Surface *surface = IMG_Load(intput_path);
    Image img = newImage(surface, 1, surface->w, surface->h);
    Thread_argument arg = { .image = img,
                            .output_path = output_path,
                            .verbose = verbose,
                            .save = save,
                            .output_folder = output_folder,
                            .gui = gui,
                            .hexa = hexa };
    SDL_FreeSurface(surface);
    pthread_create(&thread, NULL, OCR, (void *)&arg);

    if (gui == 0)
    {
        pthread_join(thread, NULL);
    }

    return thread;
}

void *OCR(void *Thread_args)
{
    Thread_argument arg = *(Thread_argument *)Thread_args;
    Image image = arg.image;
    char *image_path = image.path;
    printf("%s\n", image_path);
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
    printVerbose(verbose, 0, "--> 💾 Creating image\n");

    // if (image.width > 3000 || image.height > 3000)
    // {
    //     printVerbose(verbose, "      📏 1.0 Simplifying image\n");
    //     Image img = copyImage(&image, 1);
    //     image = resize(&img, image.width * 0.5, image.height * 0.5, 1);
    // }

    saveVerbose(verbose, &image, output_folder, "1.0_Base_Image", save, 0);

    // if (!strcmp("src/Imagery/image_06.jpeg", image_path))
    //     correctDistortion(&image);

    // Preprocessing
    grayscale(&image);
    saveVerbose(verbose, &image, output_folder, "1.0_Grayscale", save, 0);
    changeImageGUI(&image, 0, 0.05, "Grayscale image", 0);

    // Binarization
    Preprocessing(&image, output_folder, verbose, save, gui);

    // DETECTION

    printVerbose(verbose, 0, "\n    🔍 2 Grid detection (Hough Transform)\n");
    printVerbose(verbose, 0,
                 "    🎥 2.1 Applying sobel edge detection filter\n");

    Image drawImage = copyImage(&image, 0);

    // Apply sobel edge detection filter
    SobelEdgeDetection(&image);

    saveVerbose(verbose, &image, output_folder, "2.1_Sobel_filter", save, 0);
    changeImageGUI(&image, gui, 0.4, "Sobel filter", 0);
    printVerbose(verbose, 0, "    🔨 2.2 Launching Hough Transform\n");

    // Four possible angle
    double four_angles[4] = { 0.0, 90.0, 180.0, 270.0 };

    // Detect the grid
    Image cropped = detection(&image, &drawImage, verbose, save, output_folder,
                              four_angles, gui);

    // Free image
    freeImage(&image, 1);

    // Reverse the image before segmenting
    reverse_color(&cropped);

    unsigned int dimension = hexa ? 16 : 9;
    unsigned int **grid = allocGrid(dimension);

    // Recognisation + Construction
    printVerbose(verbose, 0, "\n    ❓ 3 Initing digit recognition\n");
    printVerbose(verbose, 0, "    📊 3.1 Creating neural network\n");

    Network network;
    network.sizeInput = NBINPUTS;
    network.sizeOutput = NBOUTPUTS;

    printVerbose(verbose, 0, "    📑 3.2 Initing weights\n");
    launchWeights(&network, WEIGHT_PATH, verbose, gui);

    unsigned int angle_index;

    saveVerbose(verbose, &cropped, output_folder, "2.9_Inverted_image", save,
                0);
    changeImageGUI(&cropped, 0, 0.8, "Cropped image", 0);
    printVerbose(verbose, 0, "    🪓 3.3 Segmenting cropped image\n");

    // Segmentation
    // Initialize all case at NULL
    Image all_cases[dimension * dimension];
    if (verbose && save)
    {
        printf("<-- 💾 Saving all %d digit to %s\n", hexa ? 256 : 81,
               output_folder);
    }
    // Segmentation
    split(&cropped, all_cases, save, output_folder, hexa);

    printVerbose(verbose, 0, "    🔨 3.4 Creating sudoku grid\n");
    int val;
    for (unsigned int i = 0; i < dimension; i++)
    {
        for (unsigned int j = 0; j < dimension; j++)
        {
            // Get the value of the case
            val =
                getNetworkOutput(&network, &(all_cases[i * dimension + j]), 0);

            if (!hexa && val > 9)
            {
                val = 0;
            }
            val = 0;
            grid[i][j] = val;

            // Free the case
            freeImage(&(all_cases[i * dimension + j]), 0);
        }
    }

    basicPrint(grid, dimension);

    freeNetwork(&network);
    freeImage(&cropped, 0);

    if (gui)
    {
        show_result(grid);
        freeGrid(grid, dimension);
        pthread_exit(NULL); // Exit thread
    }

    if (!isSolvable(grid, dimension))
    {
        printVerbose(verbose, 0, "\n    ⚠️ 3.5 The grid is not solvable\n");
        printf("\n    ❌ Please use the graphical interface to solve the grid "
               "easily\n");
        freeGrid(grid, dimension);
        return;
    }
    else
    {
        printVerbose(verbose, 0, "\n    🎉 3.5 The grid is solvable\n");
    }

    unsigned int **copy = allocGrid(dimension);
    // Copy array to have different color when saving the image
    copyArray(grid, copy, dimension);

    printVerbose(verbose, 0, "\n    🎲 4 Solving sudoku grid\n");
    printVerbose(verbose, 0, "    🔍 4.2 Solving grid\n");

    solveSuduko(grid, 0, 0, dimension);
    basicPrint(grid, dimension);

    if (!isSolved(grid, dimension))
    {
        errx(EXIT_FAILURE, "    ⛔ Error while solving grid");
    }
    printVerbose(verbose, 0, "    ✅ 4.3 Grid is solved\n");

    // SaveResult
    saveGrid(grid, "grid.result", verbose, dimension);

    // Create, save and free the image
    Image sudoku_image;
    if (hexa)
    {
        sudoku_image = createHexaSudokuImage(grid, copy, IMAGE_PATH);
    }
    else
    {
        sudoku_image = createSudokuImage(grid, copy, IMAGE_PATH, dimension);
    }

    freeGrid(grid, dimension); // Free grid
    freeGrid(copy, dimension); // Free copy

    if (gui)
    {
        change_image(&sudoku_image, "selected_image");
        edit_progress_bar(1, "Result");
    }
    saveVerbose(verbose, &sudoku_image, output_folder, "0.0_grid", save, 1);
    pthread_exit(NULL); // Exit thread
}
