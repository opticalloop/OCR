#include <stdlib.h>
#include <time.h> // To call srand(time(NULL));
#include <math.h>
#include <stdio.h>
#include "neural_network.h"

// ------ Neuron ------

Neuron newNeuron(unsigned int nbWeights)
{
    Neuron neuron = {
        nbWeights,
        NULL,
        0, // Value
        0, // Bias
        0, // Delta
    };

    // Allocate memory for weights 
    if (nbWeights != 0){
        neuron.weights = malloc((nbWeights + 1) * sizeof(double));
    }
    else{
        neuron.weights = malloc(0 * sizeof(double));
    }  
    return neuron;
}

// Should only be called in initNetwork because of : srand ( time ( NULL));
void initNeuron(Neuron *neuron)
{
    unsigned int nbWeights = neuron->nbWeights;
    for (unsigned int i = 0; i < nbWeights; i++){
        neuron->weights[i] = (double)rand()/RAND_MAX * 2.0 - 1.0;
        printf("Weight initialized : %f\n", neuron->weights[i]);
    }

    neuron->bias = (double)rand()/RAND_MAX * 2.0 - 1.0;
}

void freeNeuron(Neuron *neuron)
{
    free(neuron->weights);
}

// ------ /Neuron ------

// ------ Layer ------

Layer newLayer(unsigned int size, unsigned int sizePreviousLayer)
{
    Layer layer = { 
        size,
        NULL
    };

    // Allocate memory for neurons, calloc already put the + 1 for the \0
    layer.neurons = malloc((size + 1) * sizeof(struct Neuron));

    // Create all the neurons depending on the size of the previous layer
    for (unsigned int i = 0; i < size; i++){
        layer.neurons[i] = newNeuron(sizePreviousLayer);
    }

    return layer;
}

void freeLayer(Layer *layer)
{
    for (unsigned int i = 0; i < layer->nbNeurons; i++){
        Neuron *neuron = &(layer->neurons[i]);
        freeNeuron(neuron);
    }
    free(layer->neurons);
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
    network.layers = malloc((network.nbLayers + 1) * sizeof(struct Layer));;
    
    printf("Creating all layers\n");

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
    unsigned int nbNeurons;
    
    for (unsigned int i = 0; i < nbLayers; i++){
        
        printf("Initing all neurons for layer %u\n", i);

        Layer* layer = &(network->layers[i]);
        nbNeurons = layer->nbNeurons;
        for (unsigned int j = 0; j < nbNeurons; j++){
            initNeuron(&(layer->neurons[j]));
        }
    }
}

void frontPropagationNetwork(Network *network, double input[])
{
    // First layer
    Layer *layer = &(network->layers[0]);
    for (unsigned int i = 0; i < layer->nbNeurons; i++){
        layer->neurons[i].value = input[i];
    }

    // Hiddens layer
    unsigned int nbLayers = network->nbLayers;
    unsigned int nbNeurons;
    // For each layer
    for (unsigned int i = 1; i < nbLayers; i++) {
        Layer prevLayer = network->layers[i - 1];
        layer = &(network->layers[i]);
        nbNeurons = layer->nbNeurons;

        // For each neuron of the actual layer
        for (unsigned int j = 0; j < nbNeurons; j++){
            Neuron* neuron = &(layer->neurons[j]);
            double sum = 0.0;

            // Calcul new neuron value based on his weights and the value of previous layer
            for (unsigned int k = 0; k <= prevLayer.nbNeurons; k++){
                sum += neuron->weights[k] * prevLayer.neurons[k].value;
            }
            //sum += neuron->bias;
            layer->neurons[j].value = sigmoid(sum);
            //printf("Layer : %d, value of update : %f\n", i, layer->neurons[j].value);
        }
    }
}

void freeNetwork(Network *network)
{
    for (unsigned int i = 0; i < network->nbLayers; i++){
        Layer *layer = &(network->layers[i]);
        freeLayer(layer);
    }
    free(network->layers);
}

// ------ /Network ------

void backPropagation(Network *network, double expected[])
{
    double errorTemp = 0.0;

    unsigned int nbLayers = network->nbLayers;

    // Output layer
    Layer *outputLayer = &(network->layers[nbLayers - 1]);

    // NbNeurons of lastlayer and expected are equals
    for (unsigned int i = 0; i < outputLayer->nbNeurons; i++){
        Neuron *neuron = &(outputLayer->neurons[i]);
        errorTemp = expected[i] - neuron->value;
        neuron->delta = errorTemp * sigmoidPrime(neuron->value);
        //printf("Error : %f\n", errorTemp);
    }

    // For all layer except the input
    for (unsigned int i = nbLayers - 1; i >= 2; i--){
        Layer layer = network->layers[i];
        Layer *previousLayer = &(network->layers[i - 1]); // Modify weights of this layer
        // For each neurons
        for (unsigned int j = 0; j < previousLayer->nbNeurons; j++){
            errorTemp = 0.0;
            Neuron *neuron = &(previousLayer->neurons[j]);
            // Calculate error rate based on all neuron in the next layer and all weights of the actual neuron
            for (unsigned int k = 0; k < layer.nbNeurons; k++){
                errorTemp += layer.neurons[k].delta * layer.neurons[k].weights[j];
            }
            neuron->delta = errorTemp * sigmoidPrime(neuron->value);
            //printf("Delta : %f\n", neuron->delta);
        }
    }
}

void gradientDescent(Network *network)
{
    // Gradient descent
    for (unsigned int i = network->nbLayers - 1; i >= 1; i--){
        Layer *layer = &(network->layers[i]);
        Layer *previousLayer = &(network->layers[i - 1]);
        // For each neurons in the layer
        for (unsigned int j = 0; j < layer->nbNeurons; j++){
            // For each neurons on the layer
            Neuron *neuron = &(layer->neurons[j]);
            for (unsigned int k = 0; k < previousLayer->nbNeurons; k++){
                // For each weights on the neuron of the previous layer
                neuron->weights[k] += neuron->delta * previousLayer->neurons[k].value;
            }
        }
    }
}

double sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

double sigmoidPrime(double x)
{
    return x * (1 - x);
}