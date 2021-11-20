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
 *      Create a new neuron based the number of weight
 *  Params:
 *      nbWeights: number of weights related to this neuron, basically number of
 * neuron of the previous layer
 *
 *  Return:
 *      the Neuron
 */
Neuron newNeuron(unsigned int nbWeights);

/*
 *  Summary:
 *      Randomly initialize weights of the neuron
 *  Params:
 *      *neuron: the neuron to initialize weights
 *
 *  Return:
 *      Directly change the neuron, so void
 */
void initNeuron(Neuron *neuron);

/*
 *  Summary:
 *      Free the neuron in parameter
 *  Params:
 *      *neuron: the neuron to be free
 *
 *  Return:
 *      Free the neuron so void
 */
void freeNeuron(Neuron *neuron);

// ------ /Neuron ------

// ------ Layer ------

/*
 *  Summary:
 *      Initialize a new Layer based on the previous layer
 *  Params:
 *      sizeInput: number of neuron of the layer
 *      sizePreviousLayer: number neuron of the previous layer
 *
 *  Return:
 *      The created layer
 */
Layer newLayer(unsigned int sizeLayer, unsigned int sizePreviousLayer);

/*
 *  Summary:
 *      Free the layer
 *  Params:
 *      layer: the layer to be free
 *
 *  Return:
 *      Free the layer so void
 */
void freeLayer(Layer *layer);
// ------ /Layer ------

// ------ Network ------

/*
 *  Summary:
 *      Create the network
 *  Params:
 *      sizeInput: number of neuron of the first layer
 *      sizeHidden: number of neuron of the hidden layers
 *      nbHiddenLayers: number of hidden layers
 *      sizeOutput: number of neuron of the output layer
 *
 *  Return:
 *      The created network
 */

Network newNetwork(unsigned int sizeInput, unsigned int sizeHidden,
                   unsigned int nbHiddenLayers, unsigned int sizeOutput);

/*
 *  Summary:
 *      Initialize all the weights of the network, call initNeuron
 *  Params:
 *      network: the network to initialize weights
 *
 *  Return:
 *      Directly initialize the network, so void
 */

void initNetwork(Network *network);

/*
 *  Summary:
 *      Perform the front propagation
 *  Params:
 *      network: the network to perform the front propagation
 *      input[]: input (normally same size of the input layer)
 *
 *  Return:
 *      Directly affect the network in parameter, so void
 */

void frontPropagation(Network *network, int input[]);

/*
 *  Summary:
 *      Free the network
 *  Params:
 *      network: the network to free
 *
 *  Return:
 *      Free the network, so void
 */

void freeNetwork(Network *network);

/*
 *  Summary:
 *      Perform the back propagation to check if there is error and inform the
 * network Params: network: the network to be checked expected[]: expected
 * output (normally same size as the last layer)
 *
 *  Return:
 *      The errorRate of this back propagation
 */

double backPropagation(Network *network, double expected[]);

/*
 *  Summary:
 *      Perform the gradient descent to update weights
 *  Params:
 *      network: the network to be updated
 *
 *  Return:
 *      Directly updated the network weights, so void
 */

void gradientDescent(Network *network);

// ------ /Network ------

/*
 *  Summary:
 *      Sigmoid activation function
 *  Params:
 *      x: value
 *
 *  Return:
 *      Result
 */

double sigmoid(double x);

/*
 *  Summary:
 *      Sigmoid prime for the back propagation
 *  Params:
 *      x: value
 *
 *  Return:
 *      Result
 */

double sigmoidPrime(double x);

/*
 *  Summary:
 *      Print weigts of the network
 *  Params:
 *      network: the network to print
 *
 *  Return:
 *      Print, so void
 */

void printWeights(Network *network);

/*
 *  Summary:
 *      Get the average error rate of the network
 *  Params:
 *      network: the network to get the average error
 *
 *  Return:
 *      The average rate of the network
 */

double averageErrorRate(Network *network);

#endif