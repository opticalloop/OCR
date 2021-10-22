#ifndef TRACE_H
#define TRACE_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "reverse_color.h"
#include "struct.h"

void initTraceImage(unsigned int **trace, size_t width, size_t height);
void freeTraceImage(unsigned int **trace, size_t height);
void drawLine(unsigned int **trace, float a, float b, Dot *d1, Dot *d2);
void caculateLine(unsigned int **trace, Dot *d1, Dot *d2);
void printImage(unsigned int **trace, unsigned int width, unsigned int height);
void testCalc(unsigned int **trace, unsigned int width, unsigned int height,
              int nbTest);
void matriceToImage(unsigned int **trace, unsigned int width,
                    unsigned int height);

#endif