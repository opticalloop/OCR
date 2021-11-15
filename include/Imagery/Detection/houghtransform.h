#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

#include "Imagery/Detection/reverse_color.h"
#include "Imagery/Detection/sobel.h"
#include "Imagery/Detection/square_detection.h"
#include "Imagery/Detection/struct.h"
#include "Imagery/Rotations_Resize/rotations.h"
#include "Imagery/Utils/image.h"
#include "verbose.h"

SDL_Surface *detection(Image *image, Image *drawImage, int verbose, int save,
                       char *output_folder);

LineList houghtransform(Image *image, Image *drawImage, int verbose, int draw,
                        char *output_folder);

void drawLineFromDot(unsigned int **matrice, Dot *d1, Dot *d2, double width,
                     double height);
void accToBmp(unsigned int **matrice, unsigned int width, unsigned int height,
              unsigned int max, int verbose, char *output_folder);

void draw_line(Image *image, int w, int h, Line *line, Pixel *color,
               int thickness, int draw);

unsigned int findTheta(LineList *lineList);

void rotateAll(Image *image, LineList *lineList, double angleDegree);

double degrees_ToRadians(double theta);

double radian_To_Degree(double radian);

#endif