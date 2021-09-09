#include <stdlib.h>
#include <time.h> // To call srand(time(NULL));
#include <math.h>
#include "neural_network_XOR.h"

// ------ Neuron ------

Neuron newNeuron(unsigned int nbWeights)
{
    Neuron neuron = {
        nbWeights,
        NULL,
        0, // Value
        0 // Bias
    };

    // Allocate memory for weights 
    neuron.weights = calloc(nbWeights, sizeof(double));

    return neuron;
}

// Should only be called in initNetwork because of : srand ( time ( NULL));
void initNeuron(Neuron *neuron)
{
    unsigned int nbWeights = neuron->nbWeights;
    for (unsigned int i = 0; i < nbWeights; i++){
        neuron->weights[i] = (double)rand()/RAND_MAX * 2.0 - 1.0;
    }

    neuron->bias = (double)rand()/RAND_MAX * 2.0 - 1.0;
}

void frontPropagationNeuron(Neuron *neuron, Layer *prevLayer)
{
    double sum = 0;
    unsigned int nbWeights = neuron->nbWeights;
    for (unsigned int i = 0; i < nbWeights; i++){
        sum += prevLayer->neurons[i].value * neuron->weights[i];
    }

    neuron->value = Activation(sum + neuron->bias);
}

// ------ /Neuron ------

// ------ Layer ------

Layer newLayer(unsigned int size, unsigned int sizePreviousLayer)
{
    Layer layer = {
        size,
        NULL
    };

    // Allocate memory for neurons
    layer.neurons = calloc(size, sizeof(struct Neuron));

    // Create all the neurons depending on the size of the previous layer
    for (unsigned int i = 0; i < size; i++){
        layer.neurons[i] = newNeuron(sizePreviousLayer);
    }

    return layer;
}
// ------ /Layer ------

// ------ Network ------

Network newNetwork(unsigned int sizeInput, unsigned int sizeHidden, unsigned int nbHiddenLayers, unsigned int sizeOutput)
{
    Network network = {
        nbHiddenLayers + 2, // Add input and output layer
        sizeInput,
        sizeHidden,
        sizeOutput,
        NULL
    };

    // Allocate memory for all layers 
    network.layers = calloc(network.nbLayers, sizeof(struct Layer));;

    // Create the input layer
    network.layers[0] = newLayer(sizeInput, 0);

    // Create all hidden layer with the nbNeurons of the previous one
    for (unsigned int i = 1; i < network.nbLayers - 1; i++){
        network.layers[i] = newLayer(sizeHidden, network.layers[i - 1].nbNeurons);
    }

    // Create the ouput layer
    network.layers[network.nbLayers - 1] = newLayer(sizeOutput, network.layers[network.nbLayers - 2].nbNeurons);

    return network;
}

// Initialize neural network
void initNetwork(Network* network)
{
    srand (time(NULL));
    unsigned int nbLayers = network->nbLayers;
    
    // Initialize it here juste to don't have to create another one and kill memory :c
    unsigned int nbNeurons;
    
    for (unsigned int i = 0; i < nbLayers; i++){
            
        Layer* layer = &network->layers[i];
        nbNeurons = layer->nbNeurons;
        for (unsigned int j = 0; j < nbNeurons; j++){
            initNeuron(&layer->neurons[j]);
        }
    }
}

void frontPropagationNetwork(Network *network)
{
    unsigned int nbLayers = network->nbLayers;
    unsigned int nbNeurons;
    for (unsigned int i = 1; i < nbLayers; i++) {
        
        Layer* prevLayer = &network->layers[i - 1];
        Layer* layer = &network->layers[i];
        nbNeurons = layer->nbNeurons;
        for (unsigned int j = 0; i < nbNeurons; j++){
            frontPropagationNeuron(&layer->neurons[j], &prevLayer);
        }
    }
}

// ------ /Network ------

void backPropagation(Network *network, unsigned int expected[])
{
    unsigned int nbLayers = network->nbLayers;
    for (unsigned int i = nbLayers; i > 0 ; i--)
    {
        Layer *layer = &network->layers[i];
        unsigned int errors[10];
        if (i != nbLayers - 1){
            for (unsigned int j = 0; j < layer->nbNeurons; j++){
                double error = 0.0;
                unsigned int nbNeurons = &network->layers[i + 1].nbNeurons;
                for (unsigned int k = 0; k < nbNeurons; k++){
                    
                }
            }
        }
    }
}

double activation(double x)
{
    return log(1 + exp(x));
}

double backPropFunction(double output)
{
    return output * (1.0 - output);
}