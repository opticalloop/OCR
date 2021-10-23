#ifndef STRUCT_H
#define STRUCT_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Utils/image.h"

typedef struct
{
    int X;
    int Y;
} Dot;

typedef struct
{
    Dot start;
    Dot end;
} Line;

typedef struct
{
    unsigned int **positiveRho;
    unsigned int **negativeRho;
    unsigned int generalY;
} Graph;

void initGraph(Graph *graph, Image *image);
void freeGraph(Graph *graph, unsigned int generalY);
unsigned int **initMatrice(unsigned int x, unsigned int y);
void freeMatrice(unsigned int **matrice, size_t height);
void initDot(Dot *dot, unsigned int x, unsigned int y);
void initLine(Line *line, Dot *start, Dot *end);
double lineLength(Line *line);
void printGraph(Graph *graph);
void printMatrice(unsigned int **matrice, unsigned int height,
                  unsigned int width);
unsigned int searchGraph(Graph *graph, long rho, size_t theta);
void addGraph(Graph *graph, long rho, size_t theta);
unsigned int graphAverage(Graph *graph);

#endif