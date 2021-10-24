#ifndef SQUARE_DETECTION_H
#define SQUARE_DETECTION_H

#include <math.h>

#include "Imagery/Segmentation/houghtransform.h"
#include "Imagery/Segmentation/struct.h"

LineList simplifyLines(LineList *linelist);

Square getSquare(LineList *lineList, Line *line, int index, int width,
                 int height);

Dot getIntersection(Line *line1, Line *line2, int width, int height);

SquareList findSquare(LineList *lineList, int width, int height, Image *image);

SquareList getAllSquare(LineList *lineList, int width, int height,
                        Image *image);

int isSquare(Square *square);

int getSquarePerimeter(Square *square);

double getLineLength(Line *line);

int getFactor(Square *square);

Square sortSquares(SquareList *squareList);

void drawSquare(Square *square, Image *image, int width, int height,
                int thickness);

#endif