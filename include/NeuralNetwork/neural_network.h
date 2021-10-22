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

/*
 *  Summary:
 *
 *  Params:
 *      nbWeights:
 *
 *  Return:
 *
 */
Neuron newNeuron(unsigned int nbWeights);

/*
 *  Summary:
 *
 *  Params:
 *      *neuron:
 *
 *  Return:
 *
 */
void initNeuron(Neuron *neuron);

/*
 *  Summary:
 *
 *  Params:
 *      *neuron:
 *
 *  Return:
 *
 */
void freeNeuron(Neuron *neuron);

// ------ /Neuron ------

// ------ Layer ------

/*
 *  Summary:
 *
 *  Params:
 *      sizeInput:
 *      sizePreviousLayer:
 *
 *  Return:
 *
 */
Layer newLayer(unsigned int sizeLayer, unsigned int sizePreviousLayer);

/*
 *  Summary:
 *
 *  Params:
 *      layer:
 *
 *  Return:
 *
 */
void freeLayer(Layer *layer);
// ------ /Layer ------

// ------ Network ------

/*
 *  Summary:
 *
 *  Params:
 *      sizeInput:
 *      sizeHidden:
 *      nbHiddenLayers:
 *      sizeOutput:
 *
 *  Return:
 *
 */

Network newNetwork(unsigned int sizeInput, unsigned int sizeHidden,
                   unsigned int nbHiddenLayers, unsigned int sizeOutput);

/*
 *  Summary:
 *
 *  Params:
 *      network:
 *
 *  Return:
 *
 */

void initNetwork(Network *network);

/*
 *  Summary:
 *
 *  Params:
 *      network:
 *      input[]:
 *
 *  Return:
 *
 */

void frontPropagation(Network *network, double input[]);

/*
 *  Summary:
 *
 *  Params:
 *      network:
 *
 *  Return:
 *
 */

void freeNetwork(Network *network);

/*
 *  Summary:
 *
 *  Params:
 *      network:
 *      expected[]:
 *
 *  Return:
 *
 */

double backPropagation(Network *network, double expected[]);

/*
 *  Summary:
 *
 *  Params:
 *      network:
 *
 *  Return:
 *
 */

void gradientDescent(Network *network);

// ------ /Network ------

/*
 *  Summary:
 *
 *  Params:
 *      x:
 *
 *  Return:
 *
 */

double sigmoid(double x);

/*
 *  Summary:
 *
 *  Params:
 *      x:
 *
 *  Return:
 *
 */

double sigmoidPrime(double x);

/*
 *  Summary:
 *
 *  Params:
 *      network:
 *
 *  Return:
 *
 */

void printWeights(Network *network);

/*
 *  Summary:
 *
 *  Params:
 *      network:
 *
 *  Return:
 *
 */

double averageErrorRate(Network *network);

#endif