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
        0, // Value
        0, // Bias
        0, // Delta
        0, // Output
    };

    // Allocate memory for weights 
    neuron.weights = calloc(nbWeights, sizeof(double));

    // \0
    //neuron.weights[neuron.nbWeights - 1] = '\0';

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

    neuron->output = activation(sum + neuron->bias);
}

void freeNeuron(Neuron *neuron)
{
    for (unsigned int i = 0; i < neuron->nbWeights; i++){
        double *weight = &neuron->weights[i]; 
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
    layer.neurons = calloc(size, sizeof(struct Neuron));
    
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
        Neuron *neuron = &layer->neurons[i];
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
    network.layers = calloc(network.nbLayers, sizeof(struct Layer));;
    
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

void frontPropagationNetwork(Network *network)
{
    unsigned int nbLayers = network->nbLayers;
    unsigned int nbNeurons;
    for (unsigned int i = 1; i < nbLayers; i++) {
        
        Layer* prevLayer = &network->layers[i - 1];
        Layer* layer = &network->layers[i];
        nbNeurons = layer->nbNeurons;
        for (unsigned int j = 0; j < nbNeurons; j++){
            printf("nbNeurons : %d and j : %d\n", nbNeurons, j);
            Neuron* neuron = &layer->neurons[j];
            frontPropagationNeuron(neuron, prevLayer);
        }
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

void backPropagation(Network *network, double expected[])
{
    unsigned int nbLayers = network->nbLayers;
    unsigned int errorsIndex = 0;
    printf("Back propagation\n");
    for (unsigned int i = nbLayers - 1; i > 0 ; i--)
    {
        printf("nbLayers : %d and i : %d\n", nbLayers, i);
        Layer *layer = &network->layers[i];
        double *errors = calloc(1, sizeof(double) * 1);
        unsigned int nbNeurons = layer->nbNeurons;

        if (i != nbLayers - 1){
            Layer *layer = &network->layers[i + 1];
            for (unsigned int j = 0; j < nbNeurons; j++){
                
                double error = 0.0;
                Neuron *neuron = &(layer->neurons[j]);
                printf("Is neuron null : %d\n", neuron == NULL);
                unsigned int nbWeights = neuron->nbWeights;
                
                for (unsigned int k = 0; k < nbWeights; k++){
                    printf("nbNeurons : %d and k : %d\n", nbNeurons, k);
                    error += neuron->weights[k] * neuron->delta;
                }

                double *reallocPointer = realloc(errors, sizeof(double) * (errorsIndex + 1));
                if (reallocPointer == NULL){
                    printf("Can't expend errors pointers");
                }
                else{
                    errors = reallocPointer;
                    errors[errorsIndex] = error;
                    errorsIndex++;
                }
                free(reallocPointer);
            }
        }
        else{
            for (unsigned int j = 0; j < nbNeurons; j++){
                errors[errorsIndex] = expected[j] - layer->neurons[j].output;
                errorsIndex++;
            }
        }
        for (unsigned int j = 0; j < nbNeurons; j++){
            layer->neurons[j].delta = errors[j] * backPropFunction(layer->neurons[j].output);
        }
        free(errors);
    }
}

void updateWeights(Network *network, double inputs[], float learningRate)
{
    for (unsigned int i = 0; i < network->nbLayers; i++){
        Layer *layer = &network->layers[i];
        unsigned int nbNeurons = layer->nbNeurons;
        for (unsigned int j = 0; j < nbNeurons; j++){
            for (unsigned int k = 0; k < 2; k++){
                layer->neurons[j].weights[k] += learningRate * layer->neurons[j].delta * inputs[k];
            }
            layer->neurons[j].weights[layer->neurons[j].nbWeights - 1] += learningRate * layer->neurons[j].delta;
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