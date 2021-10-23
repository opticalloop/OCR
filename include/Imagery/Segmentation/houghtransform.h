#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

#include "Imagery/Segmentation/reverse_color.h"
#include "Imagery/Segmentation/struct.h"

void detection(Image *image);
unsigned int **houghtransform(Image *image);
void drawLineFromDot(unsigned int **matrice, Dot *d1, Dot *d2, double width, double height);
void matriceToBmp(unsigned int **matrice, unsigned int width,
                  unsigned int height);
double degrees_ToRadians(double theta);

#endif
