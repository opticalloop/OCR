#include "Imagery/Detection/houghtransform.h"

#define THRESHOLD 0.4

Image detection(Image *image, Image *drawImage, int verbose, int save,
                char *output_folder, double four_angles[4], int gui)
{
    const unsigned int w = image->width;
    const unsigned int h = image->height;

    // Surface without sobel filter
    Image tempImage = copyImage(drawImage, 0);

    double maxTheta;
    // Call major fonction
    MyList list = houghtransform(image, drawImage, verbose, save, output_folder,
                                 &maxTheta);

    saveVerbose(verbose, drawImage, output_folder, "2.3_Hough_all_lines", save,
                0);
    changeImageGUI(drawImage, gui, 0.45, "Hough all lines", 1);
    printVerbose(verbose, 0, "    📈 2.3 Simplyfing lines\n", "terminal_text");

    // LINES SIMPLIFICATION
    MyList *resultingList = simplifyLines(&list);

    if (verbose)
        printf("    📈 2.3.1 %zu edges\n", resultingList->length);

    if (save || gui)
    {
        // Draw simplifieds lines
        Image _simplifiedImage = copyImage(&tempImage, 0);

        Pixel color = { .r = 255, .g = 0, .b = 0 };
        Node *node = resultingList->head;
        for (; node != NULL; node = node->next)
        {
            Line *line = (Line *)node->value;
            draw_line(&_simplifiedImage, w, h, line, &color, 2, 1);
        }

        saveVerbose(verbose, &_simplifiedImage, output_folder,
                    "2.4_Hough_simplified_lines", save, 0);
        changeImageGUI(&_simplifiedImage, gui, 0.5, "Hough simplified lines",
                       1);
    }

    // AUTO ROTATE
    double angle = maxTheta * 180.0 / M_PI;
    int angleRounded = (int)angle % 90; // ROTATE
    if (verbose)
        printf("    📐 2.4 Angle found : %d degrees (%f rad)\n", angleRounded,
               maxTheta);
    if ((angleRounded >= 85 && angleRounded <= 95)
        || (angleRounded >= 0 && angleRounded <= 5))

    {
        printVerbose(verbose, 0, "    📐 2.4.1 Do not need to rotate image\n",
                     "terminal_text");
        four_angles[0] = 0;
    }
    else
    {
        printVerbose(verbose, 0, "    📐 2.4.1 Rotating image\n",
                     "terminal_text");
        four_angles[0] = angleRounded;
        rotateAll(&tempImage, resultingList, angleRounded);
    }

    // Draw auto rotated image
    if (save || gui)
    {
        // Draw simplifieds lines
        Image __simplifiedImage = copyImage(&tempImage, 0);

        const unsigned int len = resultingList->length;
        Pixel color = { .r = 255, .g = 0, .b = 0 };
        for (unsigned int i = 0; i < len; i++)
        {
            Line *line = (Line *)get_value(resultingList, i);
            draw_line(&__simplifiedImage, w, h, line, &color, 2, 1);
        }

        saveVerbose(verbose, &__simplifiedImage, output_folder,
                    "2.5_Autorotated", save, 0);
        changeImageGUI(&__simplifiedImage, gui, 0.55, "Hough autorotated lines",
                       1);
    }

    // FINDING SQUARES

    printVerbose(verbose, 0, "    📦 2.5 Finding all squares\n",
                 "terminal_text");

    // FIND ALL SQUARES
    MyList squares;
    if (save || gui)
    {
        Image _squareImage = copyImage(&tempImage, 0);

        _squareImage.path = image->path;

        squares = findSquare(resultingList, w, h, &_squareImage, save);
        saveVerbose(verbose, &_squareImage, output_folder,
                    "2.6_Hough_squares_only", save, 0);
        changeImageGUI(&_squareImage, gui, 0.6, "Hough squares only", 1);
    }
    else
    {
        squares = findSquare(resultingList, w, h, image, save || gui);
    }

    if (verbose)
    {
        printf("    🔼 2.5.1 %zu squares\n", squares.length);
    }

    // SORTING SQUARES
    printVerbose(verbose, 0, "    📉 2.6 Finding the predominating square\n",
                 "terminal_text");

    // FIND THE PREDOMINANT SQUARE

    Square lastSquare = sortSquares(&squares, image);

    if (save || gui)
    {
        Image _lastSquareImg = copyImage(&tempImage, 0);

        drawSquare(&lastSquare, &_lastSquareImg, w, h, 2);
        saveVerbose(verbose, &_lastSquareImg, output_folder,
                    "2.7_Hough_last_square", save, 0);
        changeImageGUI(&_lastSquareImg, gui, 0.65, "Hough last square", 1);
    }
    // Free MyLists
    free_list(&squares);
    free_list(resultingList);

    // Correc perspective and crop
    Image img =
        correctPerspective(&tempImage, &lastSquare, verbose, output_folder);

    // Save square to surface
    freeImage(&tempImage, 0);

    return img;
}

MyList houghtransform(Image *image, Image *drawImage, int verbose, int draw,
                      char *output_folder, double *max_Theta)
{
    (void)output_folder;

    // Save the image dimensions
    const double width = drawImage->width, height = drawImage->height;
    // Calculate the diagonal of the image
    const double diagonal = sqrt(width * width + height * height);

    // Initialize the constant values for theta and rho
    const double maxTheta = 180.0, minTheta = 0.0;
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

    printVerbose(verbose, 0, "    🎲 2.2.1 Computing cos and sin array\n",
                 "terminal_text");
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

    printVerbose(verbose, 0, "    📥 2.2.2 Filling accumulator\n",
                 "terminal_text");
    unsigned int **accumulator = initMatrice(nbTheta + 1, nbRho + 1);

    // We intialize the accumulator with all the value
    // In the same time, we search for the max value in the accumulator

    unsigned int max = 0;
    double rho;
    int croppedRho;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (image->pixels[x][y].r == 255)
            {
                for (int theta = 0; theta <= nbTheta; theta++)
                {
                    rho = x * saveCos[theta] + y * saveSin[theta];
                    croppedRho = rho + diagonal;
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

    // 5000 * 5000, don't draw it !a
    // if (draw)
    //     accToBmp(accumulator, nbTheta + 1, nbRho + 1, max, verbose,
    //              output_folder);

    // Finding edges
    // Computing threshold
    int lineThreshold = max * THRESHOLD;
    if (verbose)
    {
        printf("    🔼 2.2.3 Hough max value : %u\n", max);
        printf("    🎨 2.2.4 Hough Threshold : %d\n", lineThreshold);
    }

    // Create line return line array
    MyList allLines = { NULL, NULL, 0 };

    double tempMaxTheta = 0.0;
    unsigned int histogram[181] = { 0 };
    unsigned int rounded_angle;

    int prev = accumulator[0][0];
    int prev_theta = 0, prev_rho = 0;
    int boolIsIncreasing = 1;

    printVerbose(verbose, 0, "    📜 2.2.5 Drawing on image\n", "terminal_text");
    Pixel pixel = { .r = 40, .g = 40, .b = 200 };

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
                    rounded_angle = (unsigned int)radian_To_Degree(t);
                    histogram[rounded_angle]++;
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

                Line line;
                line.xStart = d1.X;
                line.xEnd = d2.X;
                line.yStart = d1.Y;
                line.yEnd = d2.Y;
                line.theta = t;

                // Draw Lines on the copyImage matrice
                if (draw)
                    draw_line(drawImage, width, height, &line, &pixel, 1, draw);

                // Add line on our return list
                void *p = Line_tovptr(line);
                append(&allLines, p);
            }
        }
    }

    // Free cos and sin arrays
    free(arrThetas);
    free(arrRhos);
    freeMatrice(accumulator, nbTheta + 1);

    if (verbose)
        printf("    📜 2.2.6 %zu edges\n", allLines.length);

    MyList *list = &allLines;

    // Find best angle
    unsigned int angle = 0;
    for (unsigned int i = 0; i < 181; i++)
    {
        if (histogram[i] > histogram[angle])
            angle = i;
    }

    *max_Theta = degrees_ToRadians(angle);
    return *list;
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

void draw_line(Image *image, int w, int h, Line *line, Pixel *color,
               int thickness, int draw)
{
    // printf("Drawing line\n");
    int x0 = line->xStart;
    int y0 = line->yStart;

    int x1 = line->xEnd;
    int y1 = line->yEnd;

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while (1)
    {
        if (0 <= x0 && x0 < w && 0 <= y0 && y0 < h)
        {
            if (draw)
            {
                image->pixels[x0][y0].r = color->r;
                image->pixels[x0][y0].g = color->g;
                image->pixels[x0][y0].b = color->b;

                if (thickness == 2)
                {
                    if (0 <= (x0 + 1) && (x0 + 1) < w && 0 <= (y0 + 1)
                        && (y0 + 1) < h)
                    {
                        image->pixels[x0 + 1][y0 + 1].r = color->r;
                        image->pixels[x0 + 1][y0 + 1].g = color->g;
                        image->pixels[x0 + 1][y0 + 1].b = color->b;
                    }
                    if (0 <= (x0 - 1) && (x0 - 1) < w && 0 <= (y0 - 1)
                        && (y0 - 1) < h)
                    {
                        image->pixels[x0 - 1][y0 - 1].r = color->r;
                        image->pixels[x0 - 1][y0 - 1].g = color->g;
                        image->pixels[x0 - 1][y0 - 1].b = color->b;
                    }
                }
            }
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
}

void accToBmp(unsigned int **matrice, unsigned int width, unsigned int height,
              unsigned int max, int verbose, char *output_folder)
{
    Image image = newImage(NULL, 0, width, height);
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            unsigned int grayValue =
                ((double)matrice[y][x] / (double)max) * 255.0;
            updatePixelToSameValue(&(image.pixels[x][y]), grayValue);
        }
    }
    saveVerbose(verbose, &image, output_folder, "2.2_Hough_accumulator", 1, 1);
}

unsigned int findTheta(MyList *lineList)
{
    unsigned int histogram[181] = { 0 };

    int value;
    Node *node = lineList->head;
    for (; node != NULL; node = node->next)
    {
        Line *l = (Line *)node->value;
        value = (int)radian_To_Degree(l->theta);
        value++;
        printf("Value : %u\n", value);

        if (value - 1 >= 0 && value - 1 <= 180)
            histogram[value - 1]++;

        if (value >= 0 && value <= 180)
            histogram[value]++;

        if (value + 1 >= 0 && value + 1 <= 180)
            histogram[value + 1]++;
    }
    unsigned int angle = 0;
    for (unsigned int i = 0; i < 181; i++)
    {
        if (histogram[i] > histogram[angle])
            angle = i;
    }

    return angle;
}

void rotateAll(Image *image, MyList *lineList, double angleDegree)
{
    rotate(image, angleDegree);

    double angle = angleDegree * M_PI / 180.0;
    angle += 5.08;
    const double middleX = ((double)image->width / 2.0);
    const double middleY = ((double)image->height / 2.0);

    double newX;
    double newY;

    Node *node = lineList->head;
    for (; node != NULL; node = node->next)
    {
        Line *l = (Line *)node->value;
        // Calculate new position start
        newX = ((double)(cos(angle) * ((double)l->xStart - middleX)
                         - sin(angle) * ((double)l->yStart - middleY))
                + middleX);

        newY = ((double)(cos(angle) * ((double)l->yStart - middleY)
                         + sin(angle) * ((double)l->xStart - middleX))
                + middleY);

        l->xStart = (int)newX;
        l->yStart = (int)newY;

        // Calculate new position end
        newX = ((double)(cos(angle) * ((double)l->xEnd - middleX)
                         - sin(angle) * ((double)l->yEnd - middleY))
                + middleX);

        newY = ((double)(cos(angle) * ((double)l->yEnd - middleY)
                         + sin(angle) * ((double)l->xEnd - middleX))
                + middleY);

        l->xEnd = (int)newX;
        l->yEnd = (int)newY;
    }
}

double degrees_ToRadians(double degrees)
{
    return degrees * M_PI / 180.0;
}

double radian_To_Degree(double radian)
{
    return radian * 180.0 / M_PI;
}