#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief 
 * 
 * @param cinMatrix 
 * @param coutMatrix 
 * @param sizeMatrix 
 */

void inverse_mat(double cinMatrix[30][30], double coutMatrix[30][30],
                 int sizeMatrix);

/**
 * @brief 
 * 
 * @param mat 
 * @param n 
 */

void free_mat(double **mat, int n);

/**
 * @brief 
 * 
 * @param A 
 * @param B 
 * @param size 
 */

void multiply_mat(double **A, double **B, int size);

/**
 * @brief 
 * 
 * @param M 
 * @param M_inv 
 */
void inverse_3x3_matrix(double **M, double **M_inv);

/**
 * @brief 
 * 
 * @param M 
 * @param v 
 * @param v_out 
 * @param size 
 */
void multiply_mat_vector(double M[][_MAX], double v[_MAX], double v_out[_MAX],
                         int size);

/**
 * @brief 
 * 
 * @param M 
 * @param v 
 * @param v_out 
 * @param size 
 */
void multiply_mat_vector_pt(double **M, double *v, double *v_out, int size);

/**
 * @brief 
 * 
 * @param size 
 * @return double** 
 */
double **alloc_matrix(int size);

#endif