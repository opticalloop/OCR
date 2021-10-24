#include "Imagery/Segmentation/houghtransform.h"

#define THRESHOLD 0.3

void detection(Image *image, Image *drawImage, Image *simplifiedImage)
{
    // Initialize variables of the image
    // const unsigned int width = image->width, height = image->height;
    printf("_________________________________\n");
    // Call major fonction
    // SobelEdgeDetection(image);
    // saveImage(image,"out.bmp");
    LineList list = houghtransform(image, drawImage);
    printf("Max theta : %f\n", list.maxTheta);
    double angle = list.maxTheta * 180.0 / M_PI;
    printf("Angle in degree : %f\n", angle);

    LineList resultingList = simplifyLines(&list);
    angle = resultingList.maxTheta * 180.0 / M_PI;
    printf("Angle in degree after simplification: %f\n", angle);

    const unsigned int w = simplifiedImage->width;
    const unsigned int h = simplifiedImage->height;

    printf("Resulting line numbers : %d\n", resultingList.len);
    for (unsigned int i = 0; i < resultingList.len; i++)
    {
        Line line = resultingList.lines[i];
        int *c = draw_line(simplifiedImage, w, h, line.xStart, line.yStart,
                           line.xEnd, line.yEnd);
        free(c);
    }
    free(resultingList.lines);

    // printf(">- End of detection\n");
    // printMatrice(copyImage, height, width);
    // matriceToBmp(copyImage, width, height);

    // Free memory
    // freeMatrice(copyImage, height);
}

LineList houghtransform(Image *image, Image *drawImage)
{
    printf("Initializing values\n");
    // Save the image dimensions
    const double width = image->width, height = image->height;
    // Calculate the diagonal of the image
    const double diagonal = sqrt(width * width + height * height);

    // Initialize the constant values for theta and rho
    const double maxTheta = 90.0, minTheta = -90.0;
    const double maxRho = diagonal, minRho = -diagonal;
    const double nbRho = 2 * diagonal, nbTheta = nbRho;

    // Create an array of rhos
    double rhoStep = (maxRho - minRho) / nbRho;
    double *arrRhos = calloc(nbRho + 1, sizeof(double));
    int index = 0;
    for (double val = minRho; val <= maxRho && index < nbTheta;
         val += rhoStep, index++)
    {
        arrRhos[index] = val;
    }

    // Create an array of theta
    double step = (maxTheta - minTheta) / nbTheta;
    double *arrThetas = calloc(nbTheta + 1, sizeof(double));
    index = 0;
    for (double val = minTheta; val <= maxTheta && index < nbTheta;
         val += step, index++)
    {
        arrThetas[index] = val;
    }

    printf(">Create cos and sin save\n");
    // Create a save of cos and sin value for each theta, to optimize
    // performance.
    double *saveCos = calloc(nbTheta + 1, sizeof(double));
    double *saveSin = calloc(nbTheta + 1, sizeof(double));
    for (int theta = 0; theta < nbTheta; theta++)
    {
        // Convert each value of theta's array into radians
        arrThetas[theta] = degrees_ToRadians(arrThetas[theta]);

        // Save each cos(theta) and sin(theta) into their respective arrays
        saveCos[theta] = cos(arrThetas[theta]);
        saveSin[theta] = sin(arrThetas[theta]);
    }

    printf(">>Creating the accumulator\n");
    unsigned int **accumulator = initMatrice(nbTheta + 1, nbRho + 1);

    // We intialize the accumulator with all the value
    // In the same time, we search for the max value in the accumulator
    unsigned int max = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (image->pixels[x][y].r == 255)
            {
                for (int theta = 0; theta <= nbTheta; theta++)
                {
                    double rho = x * saveCos[theta] + y * saveSin[theta];
                    int croppedRho = rho + diagonal;
                    accumulator[croppedRho][theta]++;
                    if (accumulator[croppedRho][theta] > max)
                    {
                        max = accumulator[croppedRho][theta];
                    }
                }
            }
        }
    }

    free(saveCos);
    free(saveSin);

    // accToBmp(accumulator, nbTheta + 1, nbRho + 1, max);

    // Finding edges
    // Computing threshold
    int lineThreshold = max * THRESHOLD;
    printf("Threshold : %d\n", lineThreshold);

    // Create line return line array
    Line *allLines = malloc(sizeof(Line));

    int nbEdges = 0;
    double tempMaxTheta = 0.0;

    int prev = accumulator[0][0];
    int prev_theta = 0, prev_rho = 0;
    int boolIsIncreasing = 1;
    printf(">>>Drawing on the drawImage\n");

    for (int theta = 0; theta <= nbTheta; theta++)
    {
        for (int rho = 0; rho <= nbRho; rho++)
        {
            int val = accumulator[rho][theta];

            if (val >= prev)
            {
                prev = val;
                prev_rho = rho;
                prev_theta = theta;
                boolIsIncreasing = 1;
                continue;
            }
            else if (val < prev && boolIsIncreasing)
            {
                boolIsIncreasing = 0;
            }
            else if (val < prev)
            {
                prev = val;
                prev_rho = rho;
                prev_theta = theta;
                continue;
            }

            if (val >= lineThreshold)
            {
                double r = arrRhos[prev_rho], t = arrThetas[prev_theta];

                if (t > tempMaxTheta)
                {
                    tempMaxTheta = t;
                }

                double c = cos(t), s = sin(t);
                Dot d0, d1, d2;
                // Calculate d0 point
                d0.X = (int)(c * r);
                d0.Y = (int)(s * r);
                // Calculate one point of the edge
                d1.X = d0.X + (int)(diagonal * (-s));
                d1.Y = d0.Y + (int)(diagonal * c);

                // Calculate the other point of the edge
                d2.X = d0.X - (int)(diagonal * (-s));
                d2.Y = d0.Y - (int)(diagonal * c);

                // Draw Lines on the copyImage matrice
                int *coordinates =
                    draw_line(drawImage, width, height, d1.X, d1.Y, d2.X, d2.Y);

                printf("Got xStart : %d, yStart : %d, xEnd : %d, yEnd : %d\n",
                       coordinates[0], coordinates[1], coordinates[2],
                       coordinates[3]);

                // Add line on our return list
                allLines = realloc(allLines, sizeof(Line) * (nbEdges + 1));
                Line line;
                line.xStart = coordinates[0];
                line.yStart = coordinates[1];
                line.xEnd = coordinates[2];
                line.yEnd = coordinates[3];
                line.theta = t;

                allLines[nbEdges] = line;
                free(coordinates);

                nbEdges++;
            }
        }
    }

    // Free cos and sin arrays
    free(arrThetas);
    free(arrRhos);
    freeMatrice(accumulator, nbTheta + 1);
    LineList list;
    list.lines = allLines;
    list.len = nbEdges;
    list.maxTheta = tempMaxTheta;
    return list;
}

void drawLineFromDot(unsigned int **matrice, Dot *d1, Dot *d2, double width,
                     double height)
{
    const int hi = height;
    if (d1->X == d2->X)
    {
        int wi = width;
        if (d1->X < wi && d1->X >= 0)
        {
            // Column => x = constant
            for (int y = 0; y < hi; y++)
            {
                matrice[y][d1->X]++;
            }
        }
    }
    else
    {
        // Calculate the a and b values for the equation y = a * x + b
        double a =
            ((double)d2->Y - (double)d1->Y) / ((double)d2->X - (double)d1->X);
        double b = (double)d2->Y - (a * (double)d2->X);

        // Going trough the matrice and calculate the points on the line
        for (double x = 0; x < width; x += 0.1)
        {
            double y = a * x + b;
            int xi = x, yi = y;
            if (yi >= 0 && yi < hi)
            {
                matrice[yi][xi]++;
            }
        }
    }
}

// Return the two extreme points of the lignes
int *draw_line(Image *image, int w, int h, int x0, int y0, int x1, int y1)
{
    int *coordinates = malloc(sizeof(int) * 4 + 1);
    memset(coordinates, -1, sizeof(int) * 4);

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    // int started = 0;

    while (1)
    {
        if (0 <= x0 && x0 < w && 0 <= y0 && y0 < h)
        {
            // White pixel only
            // if ((image->pixels[x0][y0].r == 255 && image->pixels[x0][y0].g ==
            // 255
            //    && image->pixels[x0][y0].b == 255)/* || started*/)
            //{
            // started = 1;
            image->pixels[x0][y0].r = 255;
            image->pixels[x0][y0].g = 0;
            image->pixels[x0][y0].b = 0;

            // Get start point
            if (coordinates[0] == -1 && coordinates[1] == -1)
            {
                coordinates[0] = x0;
                coordinates[1] = y0;
            }
            else
            {
                coordinates[2] = x0;
                coordinates[3] = y0;
            }
            // }
        }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
    return coordinates;
}

void accToBmp(unsigned int **matrice, unsigned int width, unsigned int height,
              unsigned int max)
{
    Image image;
    image.width = width;
    image.height = height;
    image.path = ""; // To create an RGB surface
    image.averageColor = 0;
    image.surface = NULL;
    image.pixels = NULL;
    newImage(&image);
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            unsigned int grayValue =
                ((double)matrice[y][x] / (double)max) * 255.0;
            updatePixelToSameValue(&(image.pixels[x][y]), grayValue);
        }
    }
    saveImage(&image, "acc.bmp");
    freeImage(&image);
}

void matriceToBmp(unsigned int **matrice, unsigned int width,
                  unsigned int height)
{
    Image image;
    image.width = width;
    image.height = height;
    image.path = ""; // To create an RGB surface
    image.averageColor = 0;
    image.surface = NULL;
    image.pixels = NULL;
    newImage(&image);
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            image.pixels[x][y].r = matrice[y][x] ? 255 : 0;
            image.pixels[x][y].g = 0;
            image.pixels[x][y].b = 0;
        }
    }
    saveImage(&image, "drawingLine.bmp");

    freeImage(&image);
}

double degrees_ToRadians(double degrees)
{
    return degrees * M_PI / 180.0;
}