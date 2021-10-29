#ifndef SQUARE_DETECTION_H
#define SQUARE_DETECTION_H

#include <math.h>

#include "Imagery/Detection/houghtransform.h"
#include "Imagery/Detection/struct.h"

LineList simplifyLines(LineList *linelist);

Square getSquare(LineList *lineList, Line *line, int index);

Dot getIntersection(Line *line1, Line *line2, int width, int height);

SquareList findSquare(LineList *lineList, int width, int height, Image *image,
                      int draw);

SquareList getAllSquare(LineList *lineList, int width, int height,
                        Image *image);

int isSquare(Square *square, unsigned int width, unsigned int height);

double getLineLength(Line *line);

Square sortSquares(SquareList *squareList);

void drawSquare(Square *square, Image *image, int width, int height,
                int thickness);

Dot getBetterCorner(Square *square);

#endif