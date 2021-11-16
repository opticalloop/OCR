#include "Imagery/Detection/square_detection.h"

#define MIN_EQUAL 51

#define SQUARE_FACTOR 100

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
        int directCoeff1 =
            (line1->yEnd - line1->yStart) / (line1->xEnd - line1->xStart);
        int directCoeff2 =
            (line2->yEnd - line2->yStart) / (line2->xEnd - line2->xStart);

        int ordOrigin1 = line1->yStart - directCoeff1 * line1->xStart;
        int ordOrigin2 = line2->yStart - directCoeff2 * line2->xStart;

        if ((directCoeff2 - directCoeff1) != 0)
        {
            int x = (ordOrigin1 - ordOrigin2) / (directCoeff2 - directCoeff1);
            int y = directCoeff2 * (ordOrigin1 - ordOrigin2)
                    / (directCoeff2 - directCoeff1)
                + ordOrigin2;

            // In degree
            int angleDiff =
                abs(line1->theta * 180.0 / M_PI - line2->theta * 180.0 / M_PI);

            if (x >= 0 && x < width && y >= 0 && y < height && angleDiff > 85
                && angleDiff < 95)
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
                        // Optimize
                        // Line tempLine = { .xStart = dot1.X,
                        //                   .yStart = dot1.Y,
                        //                   .xEnd = dot2.X,
                        //                   .yEnd = dot2.Y };
                        // if (getLineLength(&tempLine) < width / 9)
                        // {
                        //     continue;
                        // }
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
    int tempFactor =
        getLineLength(&(temp.bottom)) * getLineLength(&(temp.right));
    for (unsigned int i = 1; i < len; i++)
    {
        Square square = squareList->squares[i];
        int factor =
            getLineLength(&(square.bottom)) * getLineLength(&(square.right));
        if (factor > tempFactor)// && canBeSudokuGrid(&square, image))
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
    Pixel color = { .r = 0, .g = 255, .b = 0 };
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