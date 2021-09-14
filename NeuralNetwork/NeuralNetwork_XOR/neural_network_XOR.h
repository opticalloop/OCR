#ifndef NEURAL_NETWORK_XOR
#define NEURAL_NETWORK_XOR

typedef struct Neuron
{
    unsigned int nbWeights;
    double* weights;
    double* dw;
    double value;
    double bias;

    double delta;
    double errorRate;
} Neuron;

typedef struct Layer
{
    unsigned int nbNeurons;
    Neuron* neurons;
}Layer;

typedef struct Network
{
    unsigned int nbLayers;
    unsigned int sizeInput;
    unsigned int sizeHidden;
    unsigned int sizeOutput;
    Layer* layers;
}Network;

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
Network newNetwork(unsigned int sizeInput, unsigned int sizeHidden, unsigned int nbHiddenLayers, unsigned int sizeOutput);
void initNetwork(Network* network);
void frontPropagationNetwork(Network *network, double input[], double expected[]);
void freeNetwork(Network *network);
// ------ /Network ------

void backPropagation(Network *network);

void updateWeights(Network *network, double inputs[], float learningRate);

double sigmoid(double x);

double activation(double x);

double backPropFunction(double output);


#endif