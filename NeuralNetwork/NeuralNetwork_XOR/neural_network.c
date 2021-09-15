#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double sigmoid(double x) {return 1 / (1 + exp(-x)); }
double sigmoidPrime(double x) {return x * (1-x); }

#define nbInputs 2
#define nbHiddenNodes 5
#define nbOutputs 1

double hiddenWeights[nbInputs][nbHiddenNodes];
double outputWeights[nbHiddenNodes][nbOutputs];

double hiddenNode[nbHiddenNodes];
double outputNode[nbOutputs];

int nbTraining = 100000;
double trainingInputs[4][2] = { {0.0f,0.0f},{1.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f} };
double trainingOutputs[4][1] = { {0.0f},{1.0f},{1.0f},{0.0f} };

double randWeight() { 
    return (((double)rand())/((double)RAND_MAX)*2)-1; 
}

int initWeight() { 
    for(int i=0; i<nbInputs; i++){
        for(int j=0; j<nbHiddenNodes; j++){
            hiddenWeights[i][j] = randWeight();
        }
    }
    for(int i=0; i<nbHiddenNodes; i++){
        for(int j=0; j<nbOutputs; j++){
            outputWeights[i][j] = randWeight();
        }
    }
    return 0;
}

int Training(){
    initWeight();
    for (int i = 0; i < nbTraining; i++){
        for(int j=0; j<nbHiddenNodes; j++){
            float temp = 0;
            for(int k=0; k<nbInputs; k++){
                temp += trainingInputs[i%4][k]*hiddenWeights[k][j];
            }
            hiddenNode[j] = sigmoid(temp);
        }
        for(int j=0; j<nbOutputs; j++){
            float temp = 0;
            for(int k=0; k<nbHiddenNodes; k++){
                temp += hiddenNode[k]*outputWeights[k][j];
            }
            outputNode[j] = sigmoid(temp);
        }
        double deltaOutput[nbOutputs];
        for (int j=0; j<nbOutputs; j++) {
            double dError = (trainingOutputs[i%4][j]-outputNode[j]);
            deltaOutput[j] = dError*sigmoidPrime(outputNode[j]);
            printf("%f but needed %f\n", outputNode[j], trainingOutputs[i%4][j]);
        }
        double deltaHidden[nbHiddenNodes];
        for (int j=0; j<nbHiddenNodes; j++) {
            double dError = 0.0f;
            for(int k=0; k<nbOutputs; k++) {
                dError+=deltaOutput[k]*outputWeights[j][k];
            }
            deltaHidden[j] = dError*sigmoidPrime(hiddenNode[j]);
        }
        for (int j=0; j<nbOutputs; j++) {
            for (int k=0; k<nbHiddenNodes; k++) {
                outputWeights[k][j]+=hiddenNode[k]*deltaOutput[j];
            }
        }
        for (int j=0; j<nbHiddenNodes; j++) {
            for(int k=0; k<nbInputs; k++) {
                hiddenWeights[k][j] += trainingInputs[i%4][k]*deltaHidden[j];
            }
        }
    }
    return 0;
}

int main(void){
    Training();
    return 0;
}