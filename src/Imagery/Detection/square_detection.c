#include "Imagery/Detection/square_detection.h"

#define MIN_EQUAL 51
#define MIN_THETA 31

LineList simplifyLines(LineList *linelist)
{
    const unsigned int len = linelist->len;
    Line *allLines = linelist->lines;
    if (len <= 0)
    {
        errx(1, "Got no line\n");
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
            for (unsigned int j = i + 1; j < len; j++)
            {
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
                            < MIN_EQUAL
                        && abs((int)referenceLine.theta
                               - (int)currentLine.theta)
                            < MIN_THETA)
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

            if (x >= 0 && x < width && y >= 0 && y < height)
            {
                dot.X = x;
                dot.Y = y;
                return dot;
            }
            dot.X = -1;
            dot.Y = -1;
            return dot;
        }
        dot.X = -1;
        dot.Y = -1;
        return dot;
    }
    dot.X = -1;
    dot.Y = -1;
    return dot;
}

// Stop at 4
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

                                    int l1 = getLineLength(&firstLine);
                                    int l2 = getLineLength(&secondLine);
                                    int l3 = getLineLength(&thirdLine);
                                    int l4 = getLineLength(&fourthLine);

                                    if (l1 <= width / 9 || l2 <= width / 9
                                        || l3 <= width / 9 || l4 <= width / 9
                                        || abs(l1 - l2) >= 200
                                        || abs(l2 - l3) >= 200
                                        || abs(l3 - l4) >= 200
                                        || abs(l4 - l1) >= 200)
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

int isSquare(Square *square)
{
    int lenLeft = getLineLength(&(square->left));
    
    int lenright = getLineLength(&(square->left));
    
    int lentop = getLineLength(&(square->top));

    int lenbottom = getLineLength(&(square->bottom));

    if (abs(lenLeft - lentop) > 300)
        return 0;
    if (abs(lentop - lenright) > 300)
        return 0;
    if (abs(lenright - lenbottom) > 300)
        return 0;
    if (abs(lenbottom - lentop) > 300)
        return 0;
    if (abs(lenbottom - lenLeft) > 300)
        return 0;
    if (abs(lenLeft - lenright) > 300)
        return 0;
    printf("L1 : %d, L2 : %d, L3 : %d, L4 : %d\n", lenLeft, lenright, lentop,
           lenbottom);
    return 1;
}

int getSquarePerimeter(Square *square)
{
    int lenLeft = getLineLength(&(square->left));
    
    int lenright =getLineLength(&(square->left));
    
    int lentop = getLineLength(&(square->top));

    int lenbottom = getLineLength(&(square->bottom));

    return lenLeft + lenright + lentop + lenbottom;
}

double getLineLength(Line *line)
{
    return sqrt((line->xEnd - line->xStart) * (line->xEnd - line->xStart)
                + ((line->yEnd - line->yStart) * (line->yEnd - line->yStart)));
}

int getFactor(Square *square)
{
    int l1 = getLineLength(&(square->top));

    int l2 = getLineLength(&(square->bottom));

    int l3 = getLineLength(&(square->left));

    int l4 = getLineLength(&(square->left));

    int biggestLine = l1;
    if (l2 > biggestLine)
    {
        biggestLine = l2;
    }
    if (l3 > biggestLine)
    {
        biggestLine = l3;
    }
    if (l4 > biggestLine)
    {
        biggestLine = l4;
    }

    int shortestLine = l1;
    if (l2 < shortestLine)
    {
        shortestLine = l2;
    }
    if (l3 < shortestLine)
    {
        shortestLine = l3;
    }
    if (l4 < shortestLine)
    {
        shortestLine = l4;
    }
    return biggestLine - shortestLine;
}

Square sortSquares(SquareList *squareList)
{
    const unsigned int len = squareList->len;
    Square temp = squareList->squares[0];
    int tempFactor = getFactor(&temp);
    for (unsigned int i = 1; i < len; i++)
    {
        Square square = squareList->squares[i];
        int factor = getFactor(&square);
        if (factor > tempFactor)
        {
            tempFactor = factor;
            temp = square;
        }
    }
    return temp;
}

void drawSquare(Square *square, Image *image, int width, int height,
                int thickness)
{
    draw_line(image, width, height, &(square->left), 50, thickness, 1);
    draw_line(image, width, height, &(square->bottom), 50, thickness, 1);
    draw_line(image, width, height, &(square->top), 50, thickness, 1);
    draw_line(image, width, height, &(square->right), 50, thickness, 1);
    
}