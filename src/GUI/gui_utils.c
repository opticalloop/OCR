#include "GUI/gui_utils.h"

static int is_in_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
{
    int d1, d2, d3;
    int sign;

    d1 = (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
    d2 = (x3 - x2) * (y - y2) - (y3 - y2) * (x - x2);
    d3 = (x1 - x3) * (y - y3) - (y1 - y3) * (x - x3);

    sign = (d1 < 0) ? -1 : 1;

    return (d1 == 0) ? 0 : sign * d1 / abs(d1);
}

void selectionFilter(Image *image, Square *square)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    // Get center of the qudrilateral
    int x = (square->top.xStart + square->right.xStart + square->bottom.xStart + square->left.xStart) / 4;
    int y = (square->top.yStart + square->right.yStart + square->bottom.yStart + square->left.yStart) / 4;

    // Compute square
    square->top.xEnd = square->right.xStart;
    square->top.yEnd = square->right.yStart;
    square->right.xEnd = square->bottom.xStart;
    square->right.yEnd = square->bottom.yStart;
    square->bottom.xEnd = square->left.xStart;
    square->bottom.yEnd = square->left.yStart;
    square->left.xEnd = square->top.xStart;
    square->left.yEnd = square->top.yStart;

    // Draw the square
    drawSquare(square, image, width, height, 2);

    for (unsigned int i = 0; i < width; i++)
    {
        for (unsigned int j = 0; j < height; j++)
        {
            if (is_in_triangle(x, y, square->top.xStart, square->top.yStart, square->right.xStart, square->right.yStart, i, j))
            {
                continue;
            }

            if (is_in_triangle(x, y, square->right.xStart, square->right.yStart, square->bottom.xStart, square->bottom.yStart, i, j))
            {
                continue;
            }

            if (is_in_triangle(x, y, square->bottom.xStart, square->bottom.yStart, square->left.xStart, square->left.yStart, i, j))
            {
                continue;
            }

            if (is_in_triangle(x, y, square->left.xStart, square->left.yStart, square->top.xStart, square->top.yStart, i, j))
            {
                continue;
            }
            printf("Not in square\n");
            image->pixels[i][j].r = image->pixels[i][j].r / 2;
            image->pixels[i][j].g = image->pixels[i][j].g / 2;
            image->pixels[i][j].b = image->pixels[i][j].b / 2;
        }
    }
}