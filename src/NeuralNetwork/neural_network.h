#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

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
Neuron newNeuron(unsigned int nbWeights);
void initNeuron(Neuron *neuron);
void freeNeuron(Neuron *neuron);
// ------ /Neuron ------

// ------ Layer ------
Layer newLayer(unsigned int sizeLayer, unsigned int sizePreviousLayer);
void freeLayer(Layer *layer);
// ------ /Layer ------

// ------ Network ------
Network newNetwork(unsigned int sizeInput, unsigned int sizeHidden,
                   unsigned int nbHiddenLayers, unsigned int sizeOutput);
void initNetwork(Network *network);
void frontPropagation(Network *network, double input[]);
void freeNetwork(Network *network);
// ------ /Network ------

void backPropagation(Network *network, double expected[]);

void gradientDescent(Network *network);

double sigmoid(double x);

double sigmoidPrime(double x);

void printWeights(Network *network);

#endif