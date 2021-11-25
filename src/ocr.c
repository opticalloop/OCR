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

int OCR_thread(char *image_path, char *output_path, int verbose, int save,
         char *output_folder)
{
    pthread_t thread;
    Thread_argument arg = {image_path, output_path, verbose, save, output_folder};
    pthread_create(&thread, NULL, OCR, (void *)&arg);
    pthread_exit(NULL);
}

void *OCR(void *Thread_args)
{
    Thread_argument arg = *(Thread_argument *)Thread_args;
    char *image_path = arg.image_path;
    char *output_path = arg.output_path;
    int verbose = arg.verbose;
    int save = arg.save;
    char *output_folder = arg.output_folder;
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

    saveVerbose(verbose, &image, output_folder, "1.0_Base_Image", save, 0);

    if (!strcmp("image_06.jpeg", image_path))
        correctDistortion(&image);

    // Preprocessing
    grayscale(&image);
    Preprocessing(&image, output_folder, verbose, save);

    // DETECTION

    printVerbose(verbose, "\n    🔍 2 Grid detection (Hough Transform)\n");
    printVerbose(verbose, "    🎥 2.1 Applying sobel edge detection filter\n");

    SobelEdgeDetection(&image);

    Image drawImage = copyImage(&image, 0);

    saveVerbose(verbose, &image, output_folder, "2.1_Sobel_filter", save, 0);
    printVerbose(verbose, "    🔨 2.2 Launching Hough Transform\n");

    // Four possible angle
    double four_angles[4] = {0.0, 90.0, 180.0, 270.0};

    SDL_Surface *cropped_image =
        detection(&image, &drawImage, verbose, save, output_folder, four_angles);

    // Free image
    freeImage(&image, 1);

    // Create cropped image
    Image cropped;
    cropped.surface = cropped_image;
    newImage(&cropped, 0);

    unsigned int grid[dim][dim];
    unsigned int copy[dim][dim];
    
     // Recognisation + Construction
    printVerbose(verbose, "\n    ❓ 3 Initing digit recognition\n");
    printVerbose(verbose, "    📊 3.1 Creating neural network\n");

    Network network;
    network.sizeInput = NBINPUTS;
    network.sizeOutput = NBOUTPUTS;

    printVerbose(verbose, "    📑 3.2 Initing weights\n");
    launchWeights(&network, WEIGHT_PATH, verbose);
    
    for (unsigned int angle_index = 1; angle_index < 4; angle_index++)
    {
        saveVerbose(verbose, &cropped, output_folder, "2.8_Cropped_image", save, 0);
        printVerbose(verbose, "    🪓 3.3 Segmenting cropped image\n");

        // Reverse the image before segmenting
        reverse_color(&cropped);
        updateSurface(&cropped);

        // Segmentation
        // Initialize all case at NULL
        SDL_Surface *all_cases[81];
        if (verbose && save)
            printf("<-- 💾 Saving all 81 digit to %s\n", output_folder);
        split9(&cropped, all_cases, save, output_folder);
        // saveVerbose(verbose, &cropped, output_folder, "3.0_Segmented_image", save, 0);

        printVerbose(verbose, "    🔨 3.4 Creating sudoku grid\n");
        for (unsigned int i = 0; i < dim; i++)
        {
            for (unsigned int j = 0; j < dim; j++)
            {
                grid[i][j] =
                    getNetworkOutput(&network, all_cases[i * dim + j], 0);
                printf("grid[i][j] = %u\n", grid[i][j]);
       
                SDL_FreeSurface(all_cases[i * dim + j]);
            }
        }

        // Solver
        if (!isSolvable(grid))
        {
            rotate(&cropped, four_angles[angle_index]);
            if (angle_index == 4)
            {
                printf("No solution\n");
                freeImage(&cropped, 0);   
                freeNetwork(&network);
                pthread_exit(NULL);
            }
            if (verbose)
            {
                
                printf("    ❌ 3.5 Grid is not solvable\n");
                printf("\n\n    ❓ Re-attemping with %f degree angle\n", four_angles[angle_index]);
            }
            continue;
            // errx(EXIT_FAILURE, "    ⛔ The grid is not solvable");
        }
        
        // Copy array to have different color when saving the image
        copyArray(grid, copy);
        
        freeImage(&cropped, 0);   
        freeNetwork(&network);
        break;
    }
    
    printVerbose(verbose, "    ✅ 3.5 Grid is solvable\n");
    printVerbose(verbose, "\n    🎲 4 Solving sudoku grid\n");
    printVerbose(verbose, "    🔍 4.2 Solving grid\n");

    solveSuduko(grid, 0, 0);

    if (!isSolved(grid))
    {
        errx(EXIT_FAILURE, "    ⛔ Error while solving grid");
    }
    printVerbose(verbose, "    ✅ 4.3 Grid is solved\n");

    // SaveResult
    saveGrid(grid, "grid.result", verbose);

    // Create, save and free the image
    Image sudoku_image = createSudokuImage(grid, copy, IMAGE_PATH);
    saveVerbose(verbose, &sudoku_image, output_folder, "Result", save, 1);
    pthread_exit(NULL);
}