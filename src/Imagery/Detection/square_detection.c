#include "Imagery/Detection/square_detection.h"

#define MIN_EQUAL 30

#define SQUARE_FACTOR 50

LineList simplifyLines(LineList *linelist)
{
    const unsigned int len = linelist->len;
    Line *allLines = linelist->lines;
    if (len <= 0)
    {
        errx(EXIT_FAILURE, "Got no line\n");
    }
    int lastLinesCount = 0;

    // If xstart = -1, we don't want the line
    Line currentLine;
    Line referenceLine;
    for (unsigned int i = 0; i < len; i++)
    {
        if (allLines[i].xStart != -1)
        {
            referenceLine = allLines[i];
            for (unsigned int j = 0; j < len; j++)
            {
                if (i == j)
                {
                    continue;
                }

                if (allLines[j].xStart != -1)
                {
                    currentLine = allLines[j];
                    // Line are approximately equals
                    if (abs(referenceLine.xStart - currentLine.xStart)
                            < MIN_EQUAL
                        && abs(referenceLine.xEnd - currentLine.xEnd)
                            < MIN_EQUAL
                        && abs(referenceLine.yStart - currentLine.yStart)
                            < MIN_EQUAL
                        && abs(referenceLine.yEnd - currentLine.yEnd)
                            < MIN_EQUAL)
                    {
                        allLines[i].xStart =
                            (referenceLine.xStart + currentLine.xStart) / 2;
                        allLines[i].xEnd =
                            (referenceLine.xEnd + currentLine.xEnd) / 2;
                        allLines[i].yStart =
                            (referenceLine.yStart + currentLine.yStart) / 2;
                        allLines[i].yEnd =
                            (referenceLine.yEnd + currentLine.yEnd) / 2;
                        allLines[j].xStart = -1;
                        lastLinesCount++;
                    }
                }
            }
        }
    }
    Line *resultingLines = malloc(lastLinesCount * sizeof(Line) + 1);
    int index = 0;
    for (unsigned int j = 0; j < len; j++)
    {
        if (allLines[j].xStart != -1)
        {
            resultingLines[index] = allLines[j];
            index++;
        }
    }

    LineList lines;
    lines.lines = resultingLines;
    lines.len = index;
    lines.maxTheta = linelist->maxTheta;
    free(linelist->lines);
    return lines;
}

Square getSquare(LineList *lineList, Line *line, int index)
{
    const unsigned int len = lineList->len;
    Square square;
    Line actual = *line;
    // Degree
    double actualAngle = actual.theta * 180.0 / M_PI;
    int i = 0;
    for (unsigned int j = index + 1; j < len; j++)
    {
        Line temp = lineList->lines[j];
        // Degree
        double tempAngle = temp.theta * 180.0 / M_PI;
        double angleDiff = actualAngle - tempAngle;
        angleDiff *= angleDiff < 0 ? -1 : 1;

        if (angleDiff >= 80 && angleDiff <= 100)
        {
            switch (i)
            {
            case 0:
                square.top = actual;
                break;
            case 1:
                square.bottom = actual;
                break;
            case 2:
                square.left = actual;
                break;
            case 3:
                square.right = actual;
                return square;
                break;
            }
            actual = temp;
            i++;
        }
    }

    square.bottom.xStart = -1;
    return square;
}

Dot getIntersection(Line *line1, Line *line2, int width, int height)
{
    Dot dot;
    if ((line1->xEnd - line1->xStart) != 0
        && (line2->xEnd - line2->xStart) != 0)
    {
        double directCoeff1 = ((double)line1->yEnd - (double)line1->yStart)
            / ((double)line1->xEnd - (double)line1->xStart);
        double directCoeff2 = ((double)line2->yEnd - (double)line2->yStart)
            / ((double)line2->xEnd - (double)line2->xStart);

        double ordOrigin1 = (double)line1->yStart
            - (double)directCoeff1 * (double)line1->xStart;
        double ordOrigin2 = (double)line2->yStart
            - (double)directCoeff2 * (double)line2->xStart;

        if (((int)directCoeff1 - (int)directCoeff2) != 0)
        {
            int x = (ordOrigin1 - ordOrigin2) / (directCoeff2 - directCoeff1);
            int y = directCoeff1 * (ordOrigin2 - ordOrigin1)
                    / (directCoeff1 - directCoeff2)
                + ordOrigin1;

            // In degree
            double angleDiff = abs(radian_To_Degree(line1->theta)
                                   - radian_To_Degree(line2->theta));

            if (x >= 0 && x < width && y >= 0 && y < height)
            {
                dot.X = x;
                dot.Y = y;
                return dot;
            }
        }
    }
    dot.X = -1;
    dot.Y = -1;
    return dot;
}

SquareList findSquare(LineList *lineList, int width, int height, Image *image,
                      int draw)
{
    SquareList squareList;
    squareList.squares = malloc(sizeof(Square));
    const unsigned int len = lineList->len;
    unsigned int nbSquares = 0;

    // FIRST COLUMN
    for (unsigned int h = 0; h < len; h++)
    {
        for (unsigned int i = 0; i < len; i++)
        {
            if (i == h)
                continue;
            // Get all line that actualLine have a intersection point with
            Dot dot1 = getIntersection(&(lineList->lines[h]),
                                       &(lineList->lines[i]), width, height);

            if (dot1.X != -1)
            {
                // ALL INTERSECTED EDGES
                for (unsigned int j = 0; j < len; j++)
                {
                    if (i == j)
                        continue;
                    Dot dot2 =
                        getIntersection(&(lineList->lines[i]),
                                        &(lineList->lines[j]), width, height);

                    if (dot2.X != -1)
                    {
                        // ALL INTERSECTED EDGES
                        for (unsigned int k = 0; k < len; k++)
                        {
                            if (k == j)
                                continue;
                            Dot dot3 = getIntersection(&(lineList->lines[j]),
                                                       &(lineList->lines[k]),
                                                       width, height);

                            if (dot3.X != -1)
                            {
                                if (k == h)
                                    continue;
                                // DOES K have intersection with h
                                Dot dot4 = getIntersection(
                                    &(lineList->lines[k]),
                                    &(lineList->lines[h]), width, height);

                                if (dot4.X != -1)
                                {
                                    Square square;
                                    Line firstLine = { .xStart = dot1.X,
                                                       .yStart = dot1.Y,
                                                       .xEnd = dot2.X,
                                                       .yEnd = dot2.Y };
                                    square.top = firstLine;

                                    Line secondLine = { .xStart = dot2.X,
                                                        .yStart = dot2.Y,
                                                        .xEnd = dot3.X,
                                                        .yEnd = dot3.Y };
                                    square.right = secondLine;

                                    Line thirdLine = { .xStart = dot3.X,
                                                       .yStart = dot3.Y,
                                                       .xEnd = dot4.X,
                                                       .yEnd = dot4.Y };
                                    square.bottom = thirdLine;

                                    Line fourthLine = { .xStart = dot4.X,
                                                        .yStart = dot4.Y,
                                                        .xEnd = dot1.X,
                                                        .yEnd = dot1.Y };
                                    square.left = fourthLine;

                                    // Not a square
                                    if (!isSquare(&square, width, height))
                                    {
                                        continue;
                                    }

                                    compute_Square(&square);

                                    squareList.squares = realloc(
                                        squareList.squares,
                                        (nbSquares + 1) * sizeof(Square));
                                    squareList.squares[nbSquares] = square;

                                    if (draw)
                                    {
                                        drawSquare(&square, image, width,
                                                   height, 2);
                                    }
                                    nbSquares++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    squareList.len = nbSquares;
    return squareList;
}

int isSquare(Square *square, unsigned int width, unsigned int height)
{
    unsigned int lenLeft = getLineLength(&(square->left));

    unsigned int lenright = getLineLength(&(square->right));

    unsigned int lentop = getLineLength(&(square->top));

    unsigned int lenbottom = getLineLength(&(square->bottom));

    // if (lenLeft <= width / 9 || lenright <= width / 9 || lentop <= width / 9
    //     || lenbottom <= width / 9)
    //     return 0;
    unsigned int max = lenLeft;
    if (lenright > max)
        max = lenright;
    if (lenbottom > max)
        max = lenbottom;
    if (lentop > max)
        max = lentop;

    unsigned int min = lenLeft;
    if (lenright < min)
        min = lenright;
    if (lenbottom < min)
        min = lenbottom;
    if (lentop < min)
        min = lentop;

    unsigned int val = max - min;

    if (val > SQUARE_FACTOR)
        return 0;

    return 1;
}

double getLineLength(Line *line)
{
    return sqrt((line->xEnd - line->xStart) * (line->xEnd - line->xStart)
                + ((line->yEnd - line->yStart) * (line->yEnd - line->yStart)));
}

Square sortSquares(SquareList *squareList, Image *image)
{
    const unsigned int len = squareList->len;
    Square temp = squareList->squares[0];
    int tempFactor = getPerimeter(&temp);
    for (unsigned int i = 1; i < len; i++)
    {
        Square square = squareList->squares[i];
        int factor = getPerimeter(&square);
        if (factor > tempFactor) // && canBeSudokuGrid(&square, image))
        {
            tempFactor = factor;
            temp = square;
        }
    }
    return temp;
}

int canBeSudokuGrid(Square *square, Image *image)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    const unsigned int leftLength = getLineLength(&(square->left));
    const unsigned int topLength = getLineLength(&(square->top));

    Dot topLeftCorner = getBetterCorner(square);
    unsigned int x = topLeftCorner.X;
    unsigned int y = topLeftCorner.Y;

    // Check all lines
    unsigned int i = 0;
    for (; x < topLeftCorner.X + leftLength && x < height; x++)
    {
        for (i = 0; i < topLength && i < width; i++)
        {
            // White pixel
            if (image->pixels[x][i + topLeftCorner.X].r == 255)
            {
                break;
            }
        }
        if (i != topLength && i != width)
        {
            return 0;
        }
    }

    // Check all colums
    unsigned int j = 0;
    for (; y < topLeftCorner.Y + topLength && y < width; y++)
    {
        for (j = 0; j < leftLength && j < height; j++)
        {
            // White pixel
            if (image->pixels[j + topLeftCorner.Y][y].r == 255)
            {
                break;
            }
        }
        if (j != leftLength && j != height)
        {
            return 0;
        }
    }
    return 1;
}

void drawSquare(Square *square, Image *image, int width, int height,
                int thickness)
{
    Pixel color = { .r = 255, .g = 0, .b = 255 };
    draw_line(image, width, height, &(square->left), &color, thickness, 1);
    draw_line(image, width, height, &(square->bottom), &color, thickness, 1);
    draw_line(image, width, height, &(square->top), &color, thickness, 1);
    draw_line(image, width, height, &(square->right), &color, thickness, 1);
}

Dot getBetterCorner(Square *square)
{
    // Get the nearest corner from 0 / 0
    Dot dot = { .X = square->top.xStart, .Y = square->top.yStart };
    if (square->bottom.xStart + square->bottom.yStart < dot.Y + dot.X)
    {
        dot.X = square->bottom.xStart;
        dot.Y = square->bottom.yStart;
    }
    if (square->right.xStart + square->right.yStart < dot.Y + dot.X)
    {
        dot.X = square->right.xStart;
        dot.Y = square->right.yStart;
    }
    if (square->left.xStart + square->left.yStart < dot.Y + dot.X)
    {
        dot.X = square->left.xStart;
        dot.Y = square->left.yStart;
    }
    return dot;
}

double getPerimeter(Square *square)
{
    // Get perimeter of the square
    return getLineLength(&(square->left)) + getLineLength(&(square->right))
        + getLineLength(&(square->top)) + getLineLength(&(square->bottom));
}

// Reorder the 4 points of the square
void compute_Square(Square *square)
{
    Dot dot1 = { .X = square->top.xStart, .Y = square->top.yStart };
    Dot dot2 = { .X = square->right.xStart, .Y = square->right.yStart };
    Dot dot3 = { .X = square->bottom.xStart, .Y = square->bottom.yStart };
    Dot dot4 = { .X = square->left.xStart, .Y = square->left.yStart };

    // Compute addition
    int cord1 = dot1.X + dot1.Y;
    int cord2 = dot2.X + dot2.Y;
    int cord3 = dot3.X + dot3.Y;
    int cord4 = dot4.X + dot4.Y;

    int mark[4] = { 0, 0, 0, 0 };
    int index = 0;

    // Compare them and get all dot
    int min = cord1;
    Dot topLeft = dot1;
    if (cord2 < min)
    {
        min = cord2;
        topLeft = dot2;
        index = 1;
    }
    if (cord3 < min)
    {
        min = cord3;
        topLeft = dot3;
        index = 2;
    }
    if (cord4 < min)
    {
        min = cord4;
        topLeft = dot4;
        index = 3;
    }
    mark[index] = 1;

    // Compute max
    int max = cord1;
    Dot bottomRight = dot1;
    index = 0;
    if (cord2 > max)
    {
        max = cord2;
        bottomRight = dot2;
        index = 1;
    }
    if (cord3 > max)
    {
        max = cord3;
        bottomRight = dot3;
        index = 2;
    }
    if (cord4 > max)
    {
        max = cord4;
        bottomRight = dot4;
        index = 3;
    }
    mark[index] = 1;

    Dot topRight = dot1;
    Dot bottomLeft = dot1;
    if (mark[0] == 0 && mark[1] == 0)
    {
        if (dot1.X < dot2.X)
        {
            bottomLeft = dot1;
            topRight = dot2;
        }
        else
        {
            bottomLeft = dot2;
            topRight = dot1;
        }
    }

    if (mark[0] == 0 && mark[2] == 0)
    {
        if (dot1.X < dot3.X)
        {
            bottomLeft = dot1;
            topRight = dot3;
        }
        else
        {
            bottomLeft = dot3;
            topRight = dot1;
        }
    }

    if (mark[0] == 0 && mark[3] == 0)
    {
        if (dot1.X < dot4.X)
        {
            bottomLeft = dot1;
            topRight = dot4;
        }
        else
        {
            bottomLeft = dot4;
            topRight = dot1;
        }
    }

    if (mark[1] == 0 && mark[2] == 0)
    {
        if (dot2.X < dot3.X)
        {
            bottomLeft = dot2;
            topRight = dot3;
        }
        else
        {
            bottomLeft = dot3;
            topRight = dot2;
        }
    }

    if (mark[1] == 0 && mark[3] == 0)
    {
        if (dot2.X < dot4.X)
        {
            bottomLeft = dot2;
            topRight = dot4;
        }
        else
        {
            bottomLeft = dot4;
            topRight = dot2;
        }
    }

    if (mark[2] == 0 && mark[3] == 0)
    {
        if (dot3.X < dot4.X)
        {
            bottomLeft = dot3;
            topRight = dot4;
        }
        else
        {
            bottomLeft = dot4;
            topRight = dot3;
        }
    }

    // Set each start and end point of the line in the square
    square->top.xStart = topLeft.X;
    square->top.yStart = topLeft.Y;
    square->top.xEnd = topRight.X;
    square->top.yEnd = topRight.Y;

    // printf("Top: (%d, %d)\n", square->top.xStart, square->top.yStart);

    square->right.xStart = topRight.X;
    square->right.yStart = topRight.Y;
    square->right.xEnd = bottomRight.X;
    square->right.yEnd = bottomRight.Y;

    // printf("Right: (%d, %d)\n", square->right.xStart, square->right.yStart);

    square->bottom.xStart = bottomRight.X;
    square->bottom.yStart = bottomRight.Y;
    square->bottom.xEnd = bottomLeft.X;
    square->bottom.yEnd = bottomLeft.Y;

    // printf("Bottom: (%d, %d)\n", square->bottom.xStart,
    // square->bottom.yStart);

    square->left.xStart = bottomLeft.X;
    square->left.yStart = bottomLeft.Y;
    square->left.xEnd = topLeft.X;
    square->left.yEnd = topLeft.Y;

    // printf("Left: (%d, %d)\n\n", square->left.xStart, square->left.yStart);
}