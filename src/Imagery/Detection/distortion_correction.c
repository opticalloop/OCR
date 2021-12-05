#include "Imagery/Detection/distortion_correction.h"

void crossProduct(double vect_A[], double vect_B[], double cross_P[])
{
    cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
    cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2];
    cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
}

void perspectiveMatrix(int src[4][2], double dst[4][2],
                       double **transformation_matrix,
                       double **transformation_matrix_inv)
{
    double P[][9] = {
        { -src[0][0], -src[0][1], -1, 0, 0, 0, src[0][0] * dst[0][0],
          src[0][1] * dst[0][0], dst[0][0] },
        { 0, 0, 0, -src[0][0], -src[0][1], -1, src[0][0] * dst[0][1],
          src[0][1] * dst[0][1], dst[0][1] },
        { -src[1][0], -src[1][1], -1, 0, 0, 0, src[1][0] * dst[1][0],
          src[1][1] * dst[1][0], dst[1][0] },
        { 0, 0, 0, -src[1][0], -src[1][1], -1, src[1][0] * dst[1][1],
          src[1][1] * dst[1][1], dst[1][1] },
        { -src[2][0], -src[2][1], -1, 0, 0, 0, src[2][0] * dst[2][0],
          src[2][1] * dst[2][0], dst[2][0] },
        { 0, 0, 0, -src[2][0], -src[2][1], -1, src[2][0] * dst[2][1],
          src[2][1] * dst[2][1], dst[2][1] },
        { -src[3][0], -src[3][1], -1, 0, 0, 0, src[3][0] * dst[3][0],
          src[3][1] * dst[3][0], dst[3][0] },
        { 0, 0, 0, -src[3][0], -src[3][1], -1, src[3][0] * dst[3][1],
          src[3][1] * dst[3][1], dst[3][1] },
        { 0, 0, 0, 0, 0, 0, 0, 0, 1 }
    };

    double R[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };

    double P_inv[9][9] = { 0 };

    // Invert matrix P
    inverseMat(P, P_inv, 9);

    // Compute H = P_inv * R
    double *H = calloc(9, sizeof(double));
    multiplyMatStat(P_inv, R, H, 9);

    // Convert H to 3x3 matrix
    int v = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++, v++)
        {
            transformation_matrix[i][j] = H[v];
        }
    }

    inverse3x3Mat(transformation_matrix, transformation_matrix_inv);

    free(H);
}

Image correctPerspective(Image *image, Square *square, int verbose,
                         char *output_folder)
{
    if (verbose)
        printf("    🗺️ 2.7 Correcting perspective and cropping...\n");

    int src[4][2] = { { square->top.xStart, square->top.yStart },
                      { square->right.xStart, square->right.yStart },
                      { square->bottom.xStart, square->bottom.yStart },
                      { square->left.xStart, square->left.yStart } };

    int top = getLineLength(&(square->top));
    int right = getLineLength(&(square->right));
    int bottom = getLineLength(&(square->bottom));
    int left = getLineLength(&(square->left));

    double max_edge_length = fmax(fmax(top, right), fmax(bottom, left));

    double dst[4][2] = { { 0, 0 },
                         { max_edge_length, 0 },
                         { max_edge_length, max_edge_length },
                         { 0, max_edge_length } };

    double **transformationMat = allocMat(3);

    double **transformationMatInv = allocMat(3);

    perspectiveMatrix(src, dst, transformationMat, transformationMatInv);

    Image corrected_image = newImage(NULL, 0, max_edge_length, max_edge_length);

    for (unsigned int i = 0; i < corrected_image.height; i++)
    {
        for (unsigned int j = 0; j < corrected_image.width; j++)
        {
            double ut = i;
            double vt = j;
            double wt = 1;

            double old_coordinates[3] = { ut, vt, wt };
            double new_coordinates[3] = { 0, 0, 0 };

            multiplyMatBis(transformationMatInv, old_coordinates,
                           new_coordinates, 3);

            int x = (int)(new_coordinates[0] / new_coordinates[2]);
            int y = (int)(new_coordinates[1] / new_coordinates[2]);

            if (x >= 0 && y >= 0 && x < (int)image->width
                && y < (int)image->height)
            {
                corrected_image.pixels[i][j] = image->pixels[x][y];
            }
            else
            {
                updatePixelToSameValue(&(corrected_image.pixels[i][j]), 0);
            }
        }
    }

    saveVerbose(verbose, &corrected_image, output_folder,
                "2.8-Perspective-corrected", 1, 0);

    freeMat(transformationMat, 3);
    freeMat(transformationMatInv, 3);

    return corrected_image;
}