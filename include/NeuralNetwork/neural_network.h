#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <SDL/SDL.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // To call srand(time(NULL));

typedef struct Neuron
{
    unsigned int nbWeights;
    double *weights;

    double value;
    double bias;
    double delta;
} Neuron;

typedef struct Layer
{
    unsigned int nbNeurons;
    Neuron *neurons;
} Layer;

typedef struct Network
{
    unsigned int nbLayers;
    unsigned int sizeInput;
    unsigned int sizeHidden;
    unsigned int sizeOutput;
    Layer *layers;
} Network;

// ------ Neuron ------

/**
 * @brief
 *
 * @param nbWeights
 * @return Neuron
 */
Neuron newNeuron(unsigned int nbWeights);

/**
 * @brief
 *
 * @param neuron
 */
void initNeuron(Neuron *neuron);

/**
 * @brief
 *
 * @param neuron
 */
void freeNeuron(Neuron *neuron);

// ------ /Neuron ------

// ------ Layer ------

/**
 * @brief
 *
 * @param sizeLayer
 * @param sizePreviousLayer
 * @return Layer
 */
Layer newLayer(unsigned int sizeLayer, unsigned int sizePreviousLayer);

/**
 * @brief
 *
 * @param layer
 */
void freeLayer(Layer *layer);
// ------ /Layer ------

// ------ Network ------

/**
 * @brief
 *
 * @param sizeInput
 * @param sizeHidden
 * @param nbHiddenLayers
 * @param sizeOutput
 * @return Network
 */
Network newNetwork(unsigned int sizeInput, unsigned int sizeHidden,
                   unsigned int nbHiddenLayers, unsigned int sizeOutput);

/**
 * @brief
 *
 * @param network
 */
void initNetwork(Network *network);

/**
 * @brief
 *
 * @param network
 * @param input
 */
void frontPropagation(Network *network, int input[]);

/**
 * @brief
 *
 * @param network
 */
void freeNetwork(Network *network);

/**
 * @brief
 *
 * @param network
 * @param expected
 * @return double
 */
double backPropagation(Network *network, double expected[]);

/**
 * @brief
 *
 * @param network
 * @param learningRate
 */
void gradientDescent(Network *network, double learningRate);

// ------ /Network ------

/**
 * @brief
 *
 * @param x
 * @return double
 */
double sigmoid(double x);

/**
 * @brief
 *
 * @param x
 * @return double
 */
double sigmoidPrime(double x);

/**
 * @brief
 *
 * @param x
 * @return double
 */
double softmax(double x);

/**
 * @brief 
 * 
 * @param layer 
 */
void softmaxLayer(Layer *layer);

/**
 * @brief
 *
 * @param network
 */
void printWeights(Network *network);

/**
 * @brief
 *
 * @param network
 * @return double
 */
double averageErrorRate(Network *network);

#endif