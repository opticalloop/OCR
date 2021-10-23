#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

#include "Imagery/Segmentation/reverse_color.h"
#include "Imagery/Segmentation/sobel.h"
#include "Imagery/Segmentation/struct.h"
#include "Imagery/Utils/image.h"

void detection(Image *image, Image *drawImage);
void houghtransform(Image *image, Image *drawImage);
void drawLineFromDot(unsigned int **matrice, Dot *d1, Dot *d2, double width,
                     double height);
void accToBmp(unsigned int **matrice, unsigned int width, unsigned int height,
              unsigned int max);
void draw_line(Image *image, int w, int h, int x0, int y0, int x1, int y1);
void matriceToBmp(unsigned int **matrice, unsigned int swidth,
                  unsigned int height);
double degrees_ToRadians(double theta);

#endif
