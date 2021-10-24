#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

#include "Imagery/Segmentation/reverse_color.h"
#include "Imagery/Segmentation/sobel.h"
#include "Imagery/Segmentation/square_detection.h"
#include "Imagery/Segmentation/struct.h"
#include "Imagery/Utils/image.h"

void detection(Image *image, Image *drawImage, Image *simplifiedImage,
               Image *squareImage, Image *lastSquare);
LineList houghtransform(Image *image, Image *drawImage);
void drawLineFromDot(unsigned int **matrice, Dot *d1, Dot *d2, double width,
                     double height);
void accToBmp(unsigned int **matrice, unsigned int width, unsigned int height,
              unsigned int max);
int *draw_line(Image *image, int w, int h, Line *line, unsigned int color,
               int thickness);
void matriceToBmp(unsigned int **matrice, unsigned int swidth,
                  unsigned int height);
double degrees_ToRadians(double theta);

#endif
