#ifndef TRAINING_H
#define TRAINING_H

#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "Imagery/Utils/image.h"
#include "Imagery/Utils/pixel_operations.h"
#include "NeuralNetwork/neural_network.h"
#include "NeuralNetwork/save_load.h"
#include "verbose.h"

#define NBINPUTS 28 * 28
#define NBOUTPUTS 16
#define NBIMAGES 68000

typedef struct Training_data
{
    unsigned int epoch;
    unsigned int nbHiddenLayers;
    unsigned int nbNodesPerHidden;
    int verbose;
    char *launch_path;
    char *save_path;
    int gui;
} Training_data;

/**
 * @brief
 *
 * @param expected
 * @param neuron
 */
void printResult(double expected[], Neuron neuron[]);

/**
 * @brief
 *
 * @param inputs
 */
void checkInputs(double inputs[NBINPUTS]);

/**
 * @brief
 *
 * @param image
 * @param inputs
 */
void imageToBinary(Image *image, int inputs[]);

/**
 * @brief Create a Data object
 *
 * @param file
 * @param inputs
 * @param expected
 * @param lastChr
 */
void createData(FILE *file, int inputs[], double expected[], char *lastChr);

/**
 * @brief
 *
 */
void generateDataFile(void);

/**
 * @brief
 *
 * @param epoch
 * @param nbHiddenLayers
 * @param nbNodesPerHidden
 * @param verbose
 * @param launch_path
 * @param save_path
 * @param gui
 * @return pthread_t
 */
pthread_t train_thread(const unsigned int epoch,
                       const unsigned int nbHiddenLayers,
                       const unsigned int nbNodesPerHidden, const int verbose,
                       char *launch_path, char *save_path, int gui);

/**
 * @brief
 *
 * @param args
 * @return void*
 */
void *train(void *args);

/**
 * @brief Get the Network Output object
 *
 * @param network
 * @param image
 * @param verbose
 * @return int
 */
int getNetworkOutput(Network *network, Image *image, int verbose);

/**
 * @brief
 *
 * @param image
 * @return int
 */
int isFullWhite(Image *image);

#endif