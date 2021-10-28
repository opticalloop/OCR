#include "Imagery/Detection/sobel.h"

double Convolution(Image *image, double kernel[3][3], int row, int col)
{
    double sum = 0;
    unsigned int r, g, b;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int x = i + row;
            int y = j + col;
            if (x >= 0 && y >= 0 && x < image->width && y < image->height)
            {
                r = image->pixels[x][y].r;
                sum += r * kernel[i][j];
            }
        }
    }

    return sum;
}

void SobelEdgeDetection(Image *image)
{
    double gx, gy;
    double g_px;

    double kernel_x[3][3] = { { -1.0, 0.0, 1.0 },
                              { -2.0, 0.0, 2.0 },
                              { -1.0, 0.0, 1.0 } };

    double kernel_y[3][3] = { { -1.0, -2.0, -1.0 },
                              { 0.0, 0.0, 0.0 },
                              { 1.0, 2.0, 1.0 } };

    const unsigned int height = image->height;
    const unsigned int width = image->width;
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            gx = Convolution(image, kernel_x, j, i);
            gy = Convolution(image, kernel_y, j, i);
            g_px = sqrt(gx * gx + gy * gy);

            updatePixelToSameValue(&(image->pixels[j][i]), (unsigned int)g_px);
        }
    }
}