#ifndef NEURAL_NETWORK_XOR
#define NEURAL_NETWORK_XOR

struct Neuron
{
    unsigned int nbWeights;
    double* weights;
    double value;
    double bias;

    double delta;
    double output;
};

typedef struct Neuron Neuron;

struct Layer
{
    unsigned int nbNeurons;
    Neuron* neurons;
};
typedef struct Layer Layer;

struct Network
{
    unsigned int nbLayers;
    unsigned int sizeInput;
    unsigned int sizeHidden;
    unsigned int sizeOutput;
    Layer* layers;
};
typedef struct Network Network;

// ------ Neuron ------
Neuron newNeuron(unsigned int nbWeights);
void initNeuron(Neuron *neuron);
void frontPropagationNeuron(Neuron *neuron, Layer *prevLayer);
// ------ /Neuron ------

// ------ Layer ------
Layer newLayer(unsigned int sizeLayer, unsigned int sizePreviousLayer);
// ------ /Layer ------

// ------ Network ------
Network newNetwork(unsigned int sizeInput, unsigned int sizeHidden, unsigned int nbHiddenLayers, unsigned int sizeOutput);
void initNetwork(Network* network);
void frontPropagationNetwork(Network *network);
// ------ /Network ------

void backPropagation(Network *network, double expected[]);

void updateWeights(Network *network, double inputs[], float learningRate);

double activation(double x);

double backPropFunction(double output);


#endif