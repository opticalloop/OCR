#ifndef STRUCT_H
#define STRUCT_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Utils/image.h"

typedef struct Line
{
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
    double theta;
} Line;

typedef struct LineList
{
    Line *lines;
    int len;
    double maxTheta;

} LineList;

typedef struct Dot
{
    int X;
    int Y;
} Dot;

typedef struct Square
{
    Line top;
    Line bottom;
    Line left;
    Line right;
} Square;

typedef struct SquareList
{
    Square *squares;
    int len;
} SquareList;

int **initMatriceInt( int x, int y);

void freeMatrice(int **matrice, size_t height);

#endif