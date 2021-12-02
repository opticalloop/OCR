#include "Imagery/Detection/matrix.h"

//---------------------------------------------------
//	calculate minor of matrix OR build new matrix : k-had = minor
void Minor(double minorMatrix[9][9], int colMatrix, int sizeMatrix,
           double newMinorMatrix[9][9])
{
    int col, row, row2 = 0, col2 = 0;
    for (row = 1; row < sizeMatrix; row++)
    {
        for (col = 0; col < sizeMatrix; col++)
        {
            if (col == colMatrix)
            {
                continue;
            }
            newMinorMatrix[row2][col2] = minorMatrix[row][col];
            col2++;
            if (col2 == (sizeMatrix - 1))
            {
                row2++;
                col2 = 0;
            }
        }
    }
    return;
} // end function

//---------------------------------------------------
//	calculate determinte of matrix
double Determinte(double minorMatrix[9][9], int sizeMatrix)
{
    int col;
    double sum = 0, newMinorMatrix[9][9];
    if (sizeMatrix == 1)
    {
        return minorMatrix[0][0];
    }
    else if (sizeMatrix == 2)
    {
        return (minorMatrix[0][0] * minorMatrix[1][1]
                - minorMatrix[0][1] * minorMatrix[1][0]);
    }
    else
    {
        for (col = 0; col < sizeMatrix; col++)
        {
            Minor(minorMatrix, col, sizeMatrix, newMinorMatrix); // function
            sum += (double)(minorMatrix[0][col] * pow(-1, col)
                            * Determinte(newMinorMatrix,
                                         (sizeMatrix - 1))); // function
        }
    }
    return sum;
} // end function

//---------------------------------------------------
//	calculate transpose of matrix
void Transpose(double cofactorMatrix[9][9], double sizeMatrix,
               double determinte, double coutMatrix[9][9],
               double transposeMatrix[9][9])
{
    int row, col;
    for (row = 0; row < sizeMatrix; row++)
    {
        for (col = 0; col < sizeMatrix; col++)
        {
            transposeMatrix[row][col] = cofactorMatrix[col][row];
            coutMatrix[row][col] =
                cofactorMatrix[col][row] / determinte; // adjoint method
        }
    }
    return;
} // end function

//---------------------------------------------------
//	calculate cofactor of matrix
void Cofactor(double cinMatrix[9][9], double sizeMatrix, double determinte,
              double coutMatrix[9][9], double transposeMatrix[9][9])
{
    double minorMatrix[9][9], cofactorMatrix[9][9];
    int col3, row3, row2, col2, row, col;
    for (row3 = 0; row3 < sizeMatrix; row3++)
    {
        for (col3 = 0; col3 < sizeMatrix; col3++)
        {
            row2 = 0;
            col2 = 0;
            for (row = 0; row < sizeMatrix; row++)
            {
                for (col = 0; col < sizeMatrix; col++)
                {
                    if (row != row3 && col != col3)
                    {
                        minorMatrix[row2][col2] = cinMatrix[row][col];
                        if (col2 < (sizeMatrix - 2))
                        {
                            col2++;
                        }
                        else
                        {
                            col2 = 0;
                            row2++;
                        }
                    }
                }
            }
            cofactorMatrix[row3][col3] = pow(-1, (row3 + col3))
                * Determinte(minorMatrix, (sizeMatrix - 1));
        }
    }
    Transpose(cofactorMatrix, sizeMatrix, determinte, coutMatrix,
              transposeMatrix); // function
    return;
} // end function

//---------------------------------------------------
//	calculate inverse of matrix
void Inverse(double cinMatrix[9][9], int sizeMatrix, double determinte,
             double coutMatrix[9][9], double transposeMatrix[9][9])
{
    if (determinte == 0)
    {
        printf("\n Inverse of entered matrix is not possible \n");
    }
    else if (sizeMatrix == 1)
    {
        coutMatrix[0][0] = 1;
    }
    else
    {
        Cofactor(cinMatrix, sizeMatrix, determinte, coutMatrix,
                 transposeMatrix); // function
    }
    return;
} // end function

void inverse_mat(double cinMatrix[9][9], double coutMatrix[9][9],
                 int sizeMatrix)
{
    double determinte, transposeMatrix[9][9];

    determinte = (double)Determinte(cinMatrix, sizeMatrix);

    Inverse(cinMatrix, sizeMatrix, determinte, coutMatrix, transposeMatrix);
}

void free_mat(double **mat, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(mat[i]);
    }
    free(mat);
}

/**
 * @brief In place multiplication of transform matrix A by B
 */
void multiply_mat(double **A, double **B, int size)
{
    double **C = calloc(size, sizeof(double *));
    for (int i = 0; i < size; i++)
        C[i] = calloc(size, sizeof(double));

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            for (int k = 0; k < size; k++)
                C[i][j] += A[i][k] * B[k][j];

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            A[i][j] = C[i][j];

    for (int i = 0; i < size; i++)
        free(C[i]);
    free(C);
}

/**
 * @brief Inverts a 3x3 matrix
 */
void inverse_3x3_matrix(double **M, double **M_inv)
{
    double MM = M[0][0] * M[1][1] * M[2][2] + M[0][1] * M[1][2] * M[2][0]
        + M[0][2] * M[2][1] * M[1][0] - M[0][2] * M[1][1] * M[2][0]
        - M[0][1] * M[1][0] * M[2][2] - M[0][0] * M[2][1] * M[1][2];

    double AM[3][3] = { { M[1][1] * M[2][2] - M[1][2] * M[2][1],
                          M[0][2] * M[2][1] - M[0][1] * M[2][2],
                          M[0][1] * M[1][2] - M[0][2] * M[1][1] },
                        { M[1][2] * M[2][0] - M[1][0] * M[2][2],
                          M[0][0] * M[2][2] - M[0][2] * M[2][0],
                          M[0][2] * M[1][0] - M[0][0] * M[1][2] },
                        { M[1][0] * M[2][1] - M[1][1] * M[2][0],
                          M[0][1] * M[2][0] - M[0][0] * M[2][1],
                          M[0][0] * M[1][1] - M[0][1] * M[1][0] } };

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            M_inv[i][j] = AM[i][j] / MM;
}

/**
 * @brief Multiply a size*size matrix by a column vector
 */
void multiply_mat_vector(double M[][_MAX], double v[_MAX], double v_out[_MAX],
                         int size)
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            v_out[i] += M[i][j] * v[j];
}

void multiply_mat_vector_pt(double **M, double *v, double *v_out, int size)
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            v_out[i] += M[i][j] * v[j];
}

double **alloc_matrix(int size)
{
    double **mat = calloc(size, sizeof(double *));
    if (mat == NULL)
    {
        printf("Error allocating memory for matrix\n");
        exit(1);
    }
    for (int i = 0; i < size; i++)
    {
        mat[i] = calloc(size, sizeof(double));
        if (mat[i] == NULL)
        {
            printf("Error allocating memory for matrix\n");
            exit(1);
        }
    }
    return mat;
}