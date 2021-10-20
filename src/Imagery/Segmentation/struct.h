#ifndef STRUCT_H
#define STRUCT_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../Utils/image.h"

typedef struct Dot
{
    unsigned int X;
    unsigned int Y;
} Dot;

typedef struct Line
{
    Dot start;
    Dot end;
} Line;

typedef struct Graph{
    unsigned int **positiveRho;
    unsigned int positiveY; 
    unsigned int **negativeRho;
    unsigned int negativeY;
} Graph;

void initGraph(Graph *graph, Image *image);
void initMatrice(unsigned int **matrice, unsigned int x, unsigned y);
void freeMatrice(unsigned int **matrice, size_t height);
void initDot(Dot *dot, unsigned int x, unsigned int y);
void initLine(Line *line, Dot *start, Dot *end);
double lineLength(Line *line);
void printGraph(Graph *graph);
void printMatrice(unsigned int **matrice, unsigned int height, unsigned int width);
void searchGraph(Graph *graph, long rho, double teta);
void addGraph(Graph *graph, long rho, double teta);

#endif