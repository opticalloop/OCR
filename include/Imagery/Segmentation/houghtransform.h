#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

#include "Imagery/Segmentation/reverse_color.h"
#include "Imagery/Segmentation/struct.h"

void detection(Image *image);
Graph *houghtransform(Image *image, double delta);
unsigned int **lineSimplication(Graph *accumulator, Image *image);
void drawLineRho(Image *image, unsigned int **toDraw, double rho,
              unsigned int theta, double delta);
void matriceToBmp(unsigned int **matrice, unsigned int width,
                  unsigned int height);

#endif
