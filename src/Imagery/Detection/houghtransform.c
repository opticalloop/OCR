#include "Imagery/Detection/houghtransform.h"

#define THRESHOLD 0.5

SDL_Surface *detection(Image *image, Image *drawImage, int verbose, int save)
{
    // Directly free
    if (!save)
    {
        freeImage(drawImage);
    }

    // Initialize variables of the image
    if (verbose)
        printf("\n    🔍 2. Hough Transform\n");

    // Call major fonction
    LineList list = houghtransform(image, drawImage, verbose, save);

    if (save)
    {
        if (verbose)
            printf("<-- 💾 Saved picture : %s\n", "2.1_Hough_all_lines.bmp");
        saveImage(drawImage, "2.1_Hough_all_lines.bmp");
        freeImage(drawImage);
    }
    if (verbose)
        printf("    📈 2.3 Simplyfing lines\n");

    // LINES SIMPLIFICATION

    LineList resultingList = simplifyLines(&list);
    if (verbose)
        printf("    📈 2.3.1 %d edges\n", resultingList.len);
    double angle = resultingList.maxTheta * 180.0 / M_PI;

    if (save)
    {
        // Draw simplifieds lines
        Image _simplifiedImage;
        _simplifiedImage.path = image->path;
        _simplifiedImage.surface = NULL;
        Image *simplifiedImage = &_simplifiedImage;
        newImage(simplifiedImage);
        const unsigned int w = simplifiedImage->width;
        const unsigned int h = simplifiedImage->height;

        const unsigned int len = resultingList.len;
        Pixel color = { .r = 255, .g = 0, .b = 0 };
        for (unsigned int i = 0; i < len; i++)
        {
            Line line = resultingList.lines[i];
            draw_line(simplifiedImage, w, h, &line, &color, 2, 1);
        }
        if (verbose)
            printf("<-- 💾 Saved picture : %s\n",
                   "2.2_Hough_simplified_lines.bmp");
        saveImage(simplifiedImage, "2.2_Hough_simplified_lines.bmp");
        freeImage(simplifiedImage);
    }

    // AUTO ROTATE

    int angleRounded = (int)angle % 90;
    if (angleRounded >= 88 && angleRounded <= 91)
    {
        if (verbose)
            printf("    📐 2.4 Do not need to rotate image\n");
    }
    else
    {
        // ROTATE
        if (verbose)
            printf("    📐 2.4 Angle found : %d\n", angleRounded);
    }
    // FINDING SQUARES

    if (verbose)
        printf("    📦 2.5 Finding all squares\n");

    Image _squareImage;
    _squareImage.path = image->path;
    _squareImage.surface = NULL;
    Image *squareImage = &_squareImage;
    if (save)
    {
        newImage(squareImage);
    }
    const unsigned int w = image->width;
    const unsigned int h = image->height;
    SquareList squares = findSquare(&resultingList, w, h, squareImage, save);
    if (save)
    {
        if (verbose)
        {
            printf("    🔼 2.5.1 %d squares\n", squares.len);

            printf("<-- 💾 Saved picture : 2.3_Hough_squares_only.bmp\n");
        }
        saveImage(squareImage, "2.3_Hough_squares_only.bmp");
        freeImage(squareImage);
    }

    // SORTING SQUARES

    if (verbose)
        printf("    📉 2.6 Finding the predominating square\n");
    Square lastSquare = sortSquares(&squares);

    if (save)
    {
        Image _lastSquareImg;
        _lastSquareImg.path = image->path;
        _lastSquareImg.surface = NULL;
        Image *lastSquareImg = &_lastSquareImg;

        newImage(lastSquareImg);
        drawSquare(&lastSquare, lastSquareImg, w, h, 2);
        if (verbose)
            printf("<-- 💾 Saved picture : 2.4_Hough_last_square.bmp\n");
        saveImage(lastSquareImg, "2.4_Hough_last_square.bmp");
        freeImage(lastSquareImg);
    }

    // GETTING MAX SQUARE

    if (verbose)
        printf("    📋 2.7 Computing cropped image\n");

    // Get square dimension
    int l1 = getLineLength(&(lastSquare.top));
    int l3 = getLineLength(&(lastSquare.right));

    // Croping image and getting final result
    SDL_Surface *surface = SDL_CreateRGBSurface(0, l1, l3, 24, 0, 0, 0, 0);
    SDL_Rect rect;
    Dot dot = getBetterCorner(&lastSquare);
    rect.x = dot.X;
    rect.y = dot.Y;
    rect.w = l1;
    rect.h = l3;

    // Save square to surface
    SDL_BlitSurface(image->surface, &rect, surface, NULL);

    // Free squares
    free(squares.squares);

    free(resultingList.lines);

    return surface;
}

LineList houghtransform(Image *image, Image *drawImage, int verbose, int draw)
{
    // Save the image dimensions
    const double width = drawImage->width, height = drawImage->height;
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

    if (verbose)
        printf("    🎲 2.1 Computing cos and sin array\n");
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

    if (verbose)
        printf("    📥 2.2 Filling accumulator\n");
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

    // 5000 * 5000, don't draw it !a
    // accToBmp(accumulator, nbTheta + 1, nbRho + 1, max);

    // Finding edges
    // Computing threshold
    int lineThreshold = max * THRESHOLD;
    if (verbose)
    {
        printf("    🔼 2.2.1 Hough max value : %u\n", max);
        printf("    🎨 2.2.2 Hough Threshold : %d\n", lineThreshold);
    }

    // Create line return line array
    Line *allLines = malloc(sizeof(Line));

    int nbEdges = 0;
    double tempMaxTheta = 0.0;

    int prev = accumulator[0][0];
    int prev_theta = 0, prev_rho = 0;
    int boolIsIncreasing = 1;

    if (verbose)
        printf("    📜 2.2.3 Drawing on image\n");

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
                allLines = realloc(allLines, sizeof(Line) * (nbEdges + 1));

                allLines[nbEdges] = line;

                nbEdges++;
            }
        }
    }

    // Free cos and sin arrays
    free(arrThetas);
    free(arrRhos);
    freeMatrice(accumulator, nbTheta + 1);

    if (verbose)
        printf("    📜 2.2.4 %d edges\n", nbEdges);

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
    saveImage(&image, "2.0_Hough_accumulator.bmp");

    freeImage(&image);
}

double degrees_ToRadians(double degrees)
{
    return degrees * M_PI / 180.0;
}