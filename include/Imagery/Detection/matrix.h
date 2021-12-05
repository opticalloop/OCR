#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define _MAX 9
#define N 9

/**
 * @brief
 *
 * @param cinMatrix
 * @param coutMatrix
 * @param sizeMatrix
 */

void inverseMat(double cinMatrix[9][9], double coutMatrix[9][9],
                 int sizeMatrix);

/**
 * @brief
 *
 * @param mat
 * @param n
 */

void freeMat(double **mat, int n);

/**
 * @brief
 *
 * @param A
 * @param B
 * @param size
 */

void multiplyMat(double **A, double **B, int size);

/**
 * @brief
 *
 * @param M
 * @param M_inv
 */
void inverse3x3Mat(double **M, double **M_inv);

void multiplyMatStat(double M[][_MAX], double v[_MAX], double v_out[_MAX],
                         int size);

/**
 * @brief
 *
 * @param M
 * @param v
 * @param v_out
 * @param size
 */
void multiplyMatBis(double **M, double *v, double *v_out, int size);

/**
 * @brief
 *
 * @param size
 * @return double**
 */
double **allocMat(int size);

#endif