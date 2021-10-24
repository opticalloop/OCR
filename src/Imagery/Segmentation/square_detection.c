#include "Imagery/Segmentation/square_detection.h"

LineList simplifyLines(Line *allLines, int len)
{
    printf("Total number of lines : %d\n", len);
    if (len <= 0)
    {
        printf("Got no line\n");
        return;
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
                    if (abs(referenceLine.xStart - currentLine.xStart) < 21
                        && abs(referenceLine.xEnd - currentLine.xEnd) < 21
                        && abs(referenceLine.yStart - currentLine.yStart) < 21
                        && abs(referenceLine.yEnd - currentLine.yEnd) < 21)
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
    printf("Got nb resulting lines : %d\n", lastLinesCount);
    Line *resultingLines = malloc(lastLinesCount * sizeof(Line) + 1);
    int index = 0;
    for (int j = 0; j < lastLinesCount; j++)
    {
        if (allLines[j].xStart != -1)
        {
            printf("j : %d\n", j);
            resultingLines[j] = allLines[index];
            index++;
        }
    }

    free(allLines);
    LineList lines;
    lines.lines = resultingLines;
    lines.len = lastLinesCount;
    return lines;
}