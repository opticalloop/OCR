#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

#include "Imagery/Detection/reverse_color.h"
#include "Imagery/Detection/sobel.h"
#include "Imagery/Detection/square_detection.h"
#include "Imagery/Detection/struct.h"
#include "Imagery/Detection/linked_list.h"
#include "Imagery/Rotations_Resize/rotations.h"
#include "Imagery/Utils/image.h"
#include "verbose.h"

/**
 * @brief
 *
 * @param image
 * @param drawImage
 * @param verbose
 * @param save
 * @param output_folder
 * @param four_angles
 * @param gui
 * @return Image
 */

Image detection(Image *image, Image *drawImage, int verbose, int save,
                char *output_folder, double four_angles[4], int gui);

/**
 * @brief
 *
 * @param image
 * @param drawImage
 * @param verbose
 * @param draw
 * @param output_folder
 * @param maxTheta
 * @return MyList
 */

MyList houghtransform(Image *image, Image *drawImage, int verbose, int draw,
                        char *output_folder, double *max_Theta);

/**
 * @brief
 *
 * @param matrice
 * @param d1
 * @param d2
 * @param width
 * @param height
 */

void drawLineFromDot(unsigned int **matrice, Dot *d1, Dot *d2, double width,
                     double height);

/**
 * @brief
 *
 * @param matrice
 * @param width
 * @param height
 * @param max
 * @param verbose
 * @param output_folder
 */
void accToBmp(unsigned int **matrice, unsigned int width, unsigned int height,
              unsigned int max, int verbose, char *output_folder);

/**
 * @brief
 *
 * @param image
 * @param w
 * @param h
 * @param line
 * @param color
 * @param thickness
 * @param draw
 */
void draw_line(Image *image, int w, int h, Line *line, Pixel *color,
               int thickness, int draw);

/**
 * @brief
 *
 * @param lineList
 * @return unsigned int
 */
unsigned int findTheta(MyList *lineList);

/**
 * @brief
 *
 * @param image
 * @param lineList
 * @param angleDegree
 */
void rotateAll(Image *image, MyList *lineList, double angleDegree);

/**
 * @brief
 *
 * @param theta
 * @return double
 */
double degrees_ToRadians(double theta);

/**
 * @brief
 *
 * @param radian
 * @return double
 */
double radian_To_Degree(double radian);

#endif