#include "GUI/gui_utils.h"

static inline double get_triangle_area(double x1, double y1, double x2,
                                       double y2, double x3, double y3)
{
    return fabs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

static inline double get_quadrilateral_area(double x1, double y1, double x2,
                                            double y2, double x3, double y3,
                                            double x4, double y4)
{
    return fabs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)
                 + x4 * (y2 - y4) + x3 * (y4 - y2) + x4 * (y3 - y4))
                / 2.0);
}

static inline isAlphaNumeric(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9');
}

void selectionFilter(Image *image, Square *square)
{
    const int width = image->width;
    const int height = image->height;

    // Compute square
    square->top.xEnd = square->right.xStart;
    square->top.yEnd = square->right.yStart;
    square->right.xEnd = square->bottom.xStart;
    square->right.yEnd = square->bottom.yStart;
    square->bottom.xEnd = square->left.xStart;
    square->bottom.yEnd = square->left.yStart;
    square->left.xEnd = square->top.xStart;
    square->left.yEnd = square->top.yStart;

    double quadrilateralsArea = get_quadrilateral_area(
        square->top.xStart, square->top.yStart, square->right.xStart,
        square->right.yStart, square->bottom.xStart, square->bottom.yStart,
        square->left.xStart, square->left.yStart);

    // Draw the square
    drawSquare(square, image, width, height, 2);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (get_triangle_area(i, j, square->top.xStart, square->top.yStart,
                                  square->right.xStart, square->right.yStart)
                    + get_triangle_area(
                        i, j, square->right.xStart, square->right.yStart,
                        square->bottom.xStart, square->bottom.yStart)
                    + get_triangle_area(
                        i, j, square->bottom.xStart, square->bottom.yStart,
                        square->left.xStart, square->left.yStart)
                    + get_triangle_area(i, j, square->left.xStart,
                                        square->left.yStart, square->top.xStart,
                                        square->top.yStart)
                > quadrilateralsArea)
            {
                image->pixels[i][j].r = image->pixels[i][j].r / 2;
                image->pixels[i][j].g = image->pixels[i][j].g / 2;
                image->pixels[i][j].b = image->pixels[i][j].b / 2;
            }
        }
    }
}