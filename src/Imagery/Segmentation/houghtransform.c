#include "Imagery/Segmentation/houghtransform.h"

void detection(Image *image)
{
    // Initialize variables of the image
    const unsigned int width = image->width, height = image->height;
    
    // Call major fonction
    unsigned int **copyImage = houghtransform(image);
    matriceToBmp(copyImage, width, height);

    // Free memory
    freeMatrice(copyImage, height);
}

unsigned int **houghtransform(Image *image)
{
    // Save the image dimensions
    const double width = image->width, height = image->height;
    // Calculate the diagonal of the image
    const double diagonal = sqrt(width * width + height * height);

    // Initialize the constant values for theta and rho
    const double maxTheta = 180.0, minTheta = 0.0;
    const double maxRho = diagonal, minRho = -diagonal;
    const double nbRho = 2*diagonal, nbTheta = nbRho;

    // Create an array of theta
    double step = (maxTheta - minTheta)/nbTheta;
    double *arrThetas = calloc(nbTheta + 1, sizeof(double));
    int index = 0;
    for(double val = minTheta; val <= maxTheta && index < nbTheta;
        val+=step, index++)
    {
        arrThetas[index] = val;
    }

    // Create a save of cos and sin value for each theta, to optimize 
    // performance.
    double *saveCos = calloc(nbTheta + 1, sizeof(double));
    double *saveSin = calloc(nbTheta + 1, sizeof(double));
    for(int theta = 0; theta < nbTheta; theta++){
        // Convert each value of theta's array into radians
        arrThetas[theta] = degrees_ToRadians(arrThetas[theta]);

        // Save each cos(theta) and sin(theta) into their respective arrays
        saveCos[theta] = cos(arrThetas[theta]);
        saveSin[theta] = sin(arrThetas[theta]);
    }

    unsigned int **accumulator = initMatrice(nbTheta+1, nbRho+1);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (image->pixels[x][y].r)
            {
                for(int theta = 0; theta <= nbTheta; theta++){
                    double rho = x * saveCos[theta] + y * saveSin[theta];
                    int croppedRho = rho + diagonal;
                    accumulator[croppedRho][theta]++;
                }
            }
        }
    }

    // Initialize the matrice that while the copy of the image with the edges
    unsigned int **copyImage = initMatrice(width,height);
    for (int theta = minTheta; theta < maxTheta; theta++)
    {
        for (int rho = minRho; rho < maxRho; rho++)
        {
            if (accumulator[rho][theta] >= 220)
            {
                int c = saveCos[theta], s = saveSin[theta];
                Dot center, first, second;
                // Calculate center point
                center.X = (c * rho) + width / 2;
                center.Y = (s * rho) + height / 2;

                //Calculate one point of the edge
                first.X = (int) (center.X + 1000 * (-s));
                first.Y = (int) (center.Y + 1000 * c);
                //Calculate a second point of the edge
                second.X = (int) (center.X - 1000 * (-s));
                second.Y = (int) (center.Y - 1000 * c);

                // Draw Lines on the copyImage matrice
                drawLineFromDot(copyImage, &first, &second, width, height);
            }
        }
    }

    // Free cos and sin arrays
    free(saveCos);
    free(saveSin);
    freeMatrice(accumulator, nbTheta+1);

    // Return my accumaltor
    return copyImage;
}

void drawLineFromDot(unsigned int **matrice, Dot *d1, Dot *d2, double width, double height){
    // Calculate the a and b values for the equation y = a * x + b
    float a = (float)(d2->Y - d1->Y) / (float)(d2->X - d1->X);
    float b = (float)d2->Y - (a * (float)d2->X);
    const int hi = height;

    // Going trough the matrice and calculate the points on the line
    for (double x = 0; x < width; x+=1.0)
    {
        int y = a * x + b;
        int xi = x;
        if (y >= 0 && y < hi){
            matrice[y][xi]++;
        }
    }
    
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

double degrees_ToRad(double degrees){
    return degrees * M_PI /180.0;
}