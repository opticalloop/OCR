#ifndef PERSPECTIVE_CORRECTION_H
#define PERSPECTIVE_CORRECTION_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Imagery/Detection/matrix.h"
#include "Imagery/Detection/square_detection.h"
#include "Imagery/Detection/struct.h"
#include "Imagery/Utils/image.h"
#include "verbose.h"

/**
 * @brief
 *
 * @param vect_A
 * @param vect_B
 * @param cross_P
 */

void crossProduct(double vect_A[], double vect_B[], double cross_P[]);

/**
 * @brief
 *
 * @param source
 * @param dest
 * @param transformation_matrix
 * @param transformation_matrix_inv
 */

void perspectiveMatrix(int src[4][2], double dst[4][2],
                       double **transformation_matrix,
                       double **transformation_matrix_inv);

/**
 * @brief
 *
 * @param image
 * @param selected_square
 * @param verbose_mode
 * @param verbose_path
 * @return Image
 */

Image correctPerspective(Image *image, Square *selected_square,
                         int verbose_mode, char *verbose_path);

#endif