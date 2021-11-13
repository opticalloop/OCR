#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

#include "Imagery/Detection/reverse_color.h"
#include "Imagery/Detection/sobel.h"
#include "Imagery/Detection/square_detection.h"
#include "Imagery/Detection/struct.h"
#include "Imagery/Utils/image.h"


SDL_Surface *detection(Image *image, Image *drawImage, int verbose, int save);

LineList houghtransform(Image *image, Image *drawImage, int verbose, int draw);

void drawLineFromDot(unsigned int **matrice, Dot *d1, Dot *d2, double width,
                     double height);
void accToBmp(unsigned int **matrice, unsigned int width, unsigned int height,
              unsigned int max);


void draw_line(Image *image, int w, int h, Line *line, Pixel *color,

               int thickness, int draw);
void matriceToBmp(unsigned int **matrice, unsigned int swidth,
                  unsigned int height);
double degrees_ToRadians(double theta);

#endif