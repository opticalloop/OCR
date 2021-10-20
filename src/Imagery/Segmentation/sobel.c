#include <math.h>

#include "Imagery/Segmentation/trace.h"

double Convolution(SDL_Surface *image, int kernel[3][3], int row, int col)
{
    double sum = 0;

    Uint32 pixel;
    Uint8 r, g, b;
    double px_value;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            pixel = get_pixel(image, j + col, i + row);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            if (r == 0 && g == 0 && b == 0)
                px_value = 1;
            else
                px_value = 0;

            sum += px_value * kernel[i][j];
        }
    }

    return sum;
}

void SobelEdgeDetection(SDL_Surface *in, SDL_Surface *out, double threshold)
{
    double gx, gy;

    double g_px;
    // double theta;
    Uint32 pixel;
    // Uint8 r, g, b;

    int kernel_x[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };

    int kernel_y[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

    for (int i = 1; i < in->h - 2; i++)
    {
        for (int j = 1; j < in->w - 2; j++)
        {
            gx = Convolution(in, kernel_x, i, j);
            gy = Convolution(in, kernel_y, i, j);

            g_px = sqrt(gx * gx + gy * gy);

            if ((g_px > threshold))
            {
                pixel = SDL_MapRGB(in->format, 255, 255, 255);
                put_pixel(out, j, i, pixel);
            }
            else
            {
                pixel = SDL_MapRGB(in->format, 0, 0, 0);
                put_pixel(out, j, i, pixel);
            }
        }
    }
}