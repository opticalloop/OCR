#include <stdlib.h>
#include <time.h> // To call srand(time(NULL));
#include <math.h>
#include <stdio.h>
#include "neural_network_XOR.h"

// ------ Neuron ------

Neuron newNeuron(unsigned int nbWeights)
{
    Neuron neuron = {
        nbWeights,
        NULL,
        NULL,
        0, // Value
        0, // Bias
        0, // Delta
        0, // Output
    };

    // Allocate memory for weights 
    if (nbWeights != 0){
        neuron.weights = malloc((nbWeights + 1) * sizeof(double));
        neuron.dw = malloc((nbWeights + 1) * sizeof(double));
    }
    else{
        neuron.weights = malloc(0 * sizeof(double));
        neuron.dw = malloc(0 * sizeof(double));
    }  
    return neuron;
}

// Should only be called in initNetwork because of : srand ( time ( NULL));
void initNeuron(Neuron *neuron)
{
    unsigned int nbWeights = neuron->nbWeights;
    for (unsigned int i = 0; i < nbWeights; i++){
        neuron->weights[i] = (double)rand()/RAND_MAX * 2.0 - 1.0;
        neuron->dw[i] = 0.0; 
    }

    neuron->bias = (double)rand()/RAND_MAX * 2.0 - 1.0;
}

void freeNeuron(Neuron *neuron)
{
    for (unsigned int i = 0; i < neuron->nbWeights; i++){
        double *weight = &(neuron->weights[i]); 
        free(weight);
    }
    free(neuron);
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
    
    unsigned int i = 0;

    // Create all the neurons depending on the size of the previous layer
    for (i = 0; i < size; i++){
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
    free(layer);
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
    
    // Initialize it here juste to don't have to create another one and kill memory :c
    unsigned int nbNeurons;
    
    for (unsigned int i = 0; i < nbLayers; i++){
        
        printf("Initing all neurons for layer %u\n", i);

        Layer* layer = &network->layers[i];
        nbNeurons = layer->nbNeurons;
        for (unsigned int j = 0; j < nbNeurons; j++){
            initNeuron(&layer->neurons[j]);
        }
    }
}

void frontPropagationNetwork(Network *network, double input[], double expected[])
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

            // Propage result from the previous layer
            for (unsigned int k = 0; k <= prevLayer.nbNeurons; k++){
                if (k == 0){
                    sum += neuron->weights[k];
                }
                else{
                    sum += neuron->weights[k] * prevLayer.neurons[k - 1].value;
                }
            }
            layer->neurons[j].value = sigmoid(sum);
        }
    }

    // Last layer delta
    layer = &(network->layers[network->nbLayers - 1]); 
    for (unsigned int i = 0; i < layer->nbNeurons; i++){
        layer->neurons[i].delta = layer->neurons[i].value - expected[i];
    }
}

void freeNetwork(Network *network)
{
    for (unsigned int i = 0; i < network->nbLayers; i++){
        Layer *layer = &(network->layers[i]);
        freeLayer(layer);
    }
    free(network);
}

// ------ /Network ------

void backPropagation(Network *network)
{
    unsigned int nbLayers = network->nbLayers;
    // Last hidden layer to start back propaging
    Layer *lastLayer = &(network->layers[nbLayers - 1]);
    
    for (unsigned int i = 0; i < lastLayer->nbNeurons; i++){
        //printf("i : %u, nbNeurons : %u\n", i, lastLayer->nbNeurons);
        Neuron *neuron = &(lastLayer->neurons[i]);
        neuron->delta = neuron->value * (1 - neuron->value);
        
        for (unsigned int j = 0; j < neuron->nbWeights; j++){
            if (j == 0){
                neuron->dw[j] = 1;
            }
            else{
                neuron->dw[j] = network->layers[nbLayers - 2].neurons[j - 1].value;
            }
        }
        
    }

    // Propage delta for other hidden layers
    for (unsigned int i = nbLayers - 2; i > 0; i--){
        Layer *layer = &(network->layers[i]);
        Layer *previousLayer = &(network->layers[i - 1]);
        Layer *nextLayer = &(network->layers[i + 1]);

        for (unsigned int j = 0; j < layer->nbNeurons; j++){
            Neuron *neuron = &(layer->neurons[j]);
            neuron->delta = neuron->value * (1 - neuron->value);
            neuron->errorRate = 0;

            // Find error rate
            for (unsigned int k = 0; k < nextLayer->nbNeurons; k++){
                Neuron neighbourNeuron = nextLayer->neurons[k];
                neuron->errorRate += neighbourNeuron.errorRate 
                                   * neighbourNeuron.delta 
                                   * neighbourNeuron.weights[j + 1];
            }

            for (unsigned int k = 0; k < neuron->nbWeights; k++){
                if (k == 0){
                    neuron->dw[k] = 1;
                }
                else{
                    neuron->dw[k] = previousLayer->neurons[k - 1].value;
                }
            }
        }
    }
}

void updateWeights(Network *network, double inputs[], float learningRate)
{
    for (unsigned int i = 0; i < network->nbLayers; i++){
        Layer *layer = &(network->layers[i]);
        unsigned int nbNeurons = layer->nbNeurons;
        for (unsigned int j = 0; j < nbNeurons; j++){
            Neuron *neuron = &(layer->neurons[j]);
            for (unsigned int k = 0; k < neuron->nbWeights; k++){
                //printf("Layer : %u, neuron : %u, weight : %u, nbWeight : %u\n", i, j, k, neuron->nbWeights);
                neuron->weights[k] += learningRate * neuron->delta * inputs[k];
            }
            
                if (neuron->nbWeights != 0){
                neuron->weights[neuron->nbWeights - 1] += learningRate *neuron->delta;
            }
        }
    }
}

double sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

double activation(double x)
{
    return log(1 + exp(x));
}

double backPropFunction(double output)
{
    return output * (1.0 - output);
}