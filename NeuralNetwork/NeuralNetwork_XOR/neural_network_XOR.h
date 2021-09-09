#ifndef NEURAL_NETWORK_XOR
#define NEURAL_NETWORK_XOR

// ------ Neuron ------
struct Neuron
{
    unsigned int nbWeights;
    double* weights;
    double value;
    double bias;
};
typedef struct Neuron Neuron;
Neuron newNeuron(unsigned int nbWeights);
void initNeuron(Neuron *neuron);
void frontPropagationNeuron(Neuron *neuron, Layer *prevLayer);
// ------ /Neuron ------

// ------ Layer ------
struct Layer
{
    unsigned int nbNeurons;
    Neuron* neurons;
};
typedef struct Layer Layer;
Layer newLayer(unsigned int sizeLayer, unsigned int sizePreviousLayer);
// ------ /Layer ------

// ------ Network ------
struct Network
{
    unsigned int nbLayers;
    unsigned int sizeInput;
    unsigned int sizeHidden;
    unsigned int sizeOutput;
    Layer* layers;
};
typedef struct Network Network;
Network newNetwork(unsigned int sizeInput, unsigned int sizeHidden, unsigned int nbHiddenLayers, unsigned int sizeOutput);
void initNetwork(Network* network);
void frontPropagationNetwork(Network *network);
// ------ /Network ------

void backPropagation(Network *network, unsigned int expected[]);

double activation(double x);

double backPropFunction(double output);


#endif