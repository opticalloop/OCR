#include "Imagery/Detection/distortion_correction.h"

/**
 * @brief Cross product of two vectors array
 *
 * @param vect_A
 * @param vect_B
 * @param cross_P
 */
void cross_product(double vect_A[], double vect_B[], double cross_P[])
{
    cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
    cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2];
    cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
}

void compute_perspective_matrix(int source[][2], double dest[][2],
                                double **transformation_matrix,
                                double **transformation_matrix_inv)
{
    double P[][9] = {
        { -source[0][0], -source[0][1], -1, 0, 0, 0, source[0][0] * dest[0][0],
          source[0][1] * dest[0][0], dest[0][0] },
        { 0, 0, 0, -source[0][0], -source[0][1], -1, source[0][0] * dest[0][1],
          source[0][1] * dest[0][1], dest[0][1] },
        { -source[1][0], -source[1][1], -1, 0, 0, 0, source[1][0] * dest[1][0],
          source[1][1] * dest[1][0], dest[1][0] },
        { 0, 0, 0, -source[1][0], -source[1][1], -1, source[1][0] * dest[1][1],
          source[1][1] * dest[1][1], dest[1][1] },
        { -source[2][0], -source[2][1], -1, 0, 0, 0, source[2][0] * dest[2][0],
          source[2][1] * dest[2][0], dest[2][0] },
        { 0, 0, 0, -source[2][0], -source[2][1], -1, source[2][0] * dest[2][1],
          source[2][1] * dest[2][1], dest[2][1] },
        { -source[3][0], -source[3][1], -1, 0, 0, 0, source[3][0] * dest[3][0],
          source[3][1] * dest[3][0], dest[3][0] },
        { 0, 0, 0, -source[3][0], -source[3][1], -1, source[3][0] * dest[3][1],
          source[3][1] * dest[3][1], dest[3][1] },
        { 0, 0, 0, 0, 0, 0, 0, 0, 1 }
    };

    double R[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };

    double P_inv[9][9] = { 0 };

    // Invert matrix P
    inverse_mat(P, P_inv, 9);

    // Compute H = P_inv * R
    double *H = calloc(9, sizeof(double));
    multiply_mat_vector(P_inv, R, H, 9);

    // Convert H to 3x3 matrix
    int v = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++, v++)
        {
            transformation_matrix[i][j] = H[v];
        }
    }

    inverse_3x3_matrix(transformation_matrix, transformation_matrix_inv);

    free(H);
}

Image correct_perspective(Image *image, Square *selected_square,
                          int verbose_mode, char *verbose_path)
{
    if (verbose_mode)
        printf("    🗺️ Correcting perspective and cropping...\n");

    int source[4][2] = {
        { selected_square->top.xStart, selected_square->top.yStart },
        { selected_square->right.xStart, selected_square->right.yStart },
        { selected_square->bottom.xStart, selected_square->bottom.yStart },
        { selected_square->left.xStart, selected_square->left.yStart }
    };

    int edge_1_length = sqrt(pow(source[0][0] - source[1][0], 2)
                             + pow(source[0][1] - source[1][1], 2));
    int edge_2_length = sqrt(pow(source[1][0] - source[2][0], 2)
                             + pow(source[1][1] - source[2][1], 2));
    int edge_3_length = sqrt(pow(source[2][0] - source[3][0], 2)
                             + pow(source[2][1] - source[3][1], 2));
    int edge_4_length = sqrt(pow(source[3][0] - source[0][0], 2)
                             + pow(source[3][1] - source[0][1], 2));

    double max_edge_length = fmax(fmax(edge_1_length, edge_2_length),
                                  fmax(edge_3_length, edge_4_length));

    double destination[4][2] = { { 0, 0 },
                                 { max_edge_length, 0 },
                                 { max_edge_length, max_edge_length },
                                 { 0, max_edge_length } };

    double **transformation_matrix = alloc_matrix(3);

    double **transformation_matrix_inv =  alloc_matrix(3);

    compute_perspective_matrix(source, destination, transformation_matrix,
                               transformation_matrix_inv);

    Image corrected_image = newImage(NULL, 0, max_edge_length, max_edge_length);

    for (int i = 0; i < corrected_image.height; i++)
    {
        for (int j = 0; j < corrected_image.width; j++)
        {
            double ut = i;
            double vt = j;
            double wt = 1;

            double old_coordinates[3] = { ut, vt, wt };
            double new_coordinates[3] = { 0, 0, 0 };

            multiply_mat_vector_pt(transformation_matrix_inv, old_coordinates,
                                   new_coordinates, 3);

            int x = (int)(new_coordinates[0] / new_coordinates[2]);
            int y = (int)(new_coordinates[1] / new_coordinates[2]);

            if (x >= 0 && y >= 0 && x < image->width && y < image->height)
            {
                corrected_image.pixels[i][j] = image->pixels[x][y];
            }
            else
            {
                updatePixelToSameValue(&(corrected_image.pixels[i][j]), 0);
            }
        }
    }

    saveVerbose(verbose_mode, &corrected_image, verbose_path,
                "2.8-perspective-corrected.png", 1, 0);

    free_mat(transformation_matrix, 3);
    free_mat(transformation_matrix_inv, 3);

    return corrected_image;
}