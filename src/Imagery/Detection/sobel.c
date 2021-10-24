#include "Imagery/Detection/sobel.h"

double Convolution(Image *image, double kernel[3][3], int row, int col)
{
    double sum = 0;
    double px_value;
    unsigned int r, g, b;

    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            int x = j + col, y = i + row;
            r = image->pixels[x][y].r;
            g = image->pixels[x][y].g;
            b = image->pixels[x][y].b;

            if (r == 0 && g == 0 && b == 0)
                px_value = 1;
            else
                px_value = 0;

            sum += px_value * kernel[i + 1][j + 1];
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

    double kernel_y[3][3] = { { 1.0, 2.0, 1.0 },
                              { 0.0, 0.0, 0.0 },
                              { -1.0, -2.0, -1.0 } };

    const int height = image->height, width = image->width;
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            gx = Convolution(image, kernel_x, i, j);
            gy = Convolution(image, kernel_y, i, j);

            g_px = sqrt(gx * gx + gy * gy);

            updatePixelToSameValue(&(image->pixels[j][i]), g_px);
        }
    }
}