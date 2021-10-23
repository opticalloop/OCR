#include "Imagery/Segmentation/houghtransform.h"

void detection(Image *image)
{
    // Initialize variables of the image
    const unsigned int width = image->width, height = image->height;
    printf("_________________________________\n");
    // Call major fonction
    unsigned int **copyImage = houghtransform(image);
    printf(">- End of detection\n");
    // printMatrice(copyImage, height, width);
    matriceToBmp(copyImage, width, height);

    // Free memory
    freeMatrice(copyImage, height);
}

unsigned int **houghtransform(Image *image)
{
    printf("Initializing values\n");
    // Save the image dimensions
    const double width = image->width, height = image->height;
    // Calculate the diagonal of the image
    const double diagonal = sqrt(width * width + height * height);

    // Initialize the constant values for theta and rho
    const double maxTheta = 180.0, minTheta = 0.0;
    //const double maxRho = diagonal, minRho = -diagonal;
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

    printf(">Create cos and sin save\n");
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

    printf(">>Creating the accumulator\n");
    unsigned int **accumulator = initMatrice(nbTheta+1, nbRho+1);

    // We intialize the accumulator with all the value
    // In the same time, we search for the max value in the accumulator
    unsigned int max = 0;
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
                    if (accumulator[croppedRho][theta]> max){
                        max = accumulator[croppedRho][theta];
                    }
                }
            }
        }
    }
    
    // Putting all the value of the accumulator between 0 and 255
    for (int theta = 0; theta <= nbTheta; theta++)
    {
        for (int rho = 0; rho <= nbRho; rho++)
        {
            double newValue = ((double)accumulator[rho][theta]/(double)max) * 255.0;
            accumulator[rho][theta] = (unsigned int)newValue;
        }
    }

    accToBmp(accumulator, nbTheta+1, nbRho+1);
    printf(">>>Creating the copyImage\n");
    // Initialize the matrice that while the copy of the image with the edges
    unsigned int **copyImage = initMatrice(width,height);
    for (int theta = 0; theta <= nbTheta; theta++)
    {
        for (int rho = 0; rho <= nbRho; rho++)
        {
            //printf("rho : %i, theta : %i\n", rho, theta);
            if (accumulator[rho][theta] >= 220)
            {
                printf("DrawLine\n");
                double c = saveCos[theta], s = saveSin[theta];
                Dot center, first, second;
                // Calculate center point
                center.X = (int)(c * (double)rho) + width / 2;
                center.Y = (int)(s * (double)rho) + height / 2;
                printf("cos(theta): %f, sin(theta): %f, X0: %i, Y0: %i\n", c, s, center.X, center.Y);
                //Calculate one point of the edge
                first.X = center.X + (int)(1000.0 * (-s));
                first.Y = center.Y + (int)(1000.0 * c);
                printf("First -- X: %i, Y: %i\n",first.X,first.Y);

                //Calculate a second point of the edge
                second.X = center.X - (int)(1000.0 * (-s));
                second.Y = center.Y - (int)(1000.0 * c);
                printf("Second -- X: %i, Y: %i\n",second.X,second.Y);

                // Draw Lines on the copyImage matrice
                drawLineFromDot(copyImage, &first, &second, width, height);
            }
        }
    }
    // printMatrice(copyImage, height, width);
    // matriceToBmp(copyImage, width, height);

    // Free cos and sin arrays
    free(saveCos);
    free(saveSin);
    free(arrThetas);
    freeMatrice(accumulator, nbTheta+1);

    // Return my accumaltor
    return copyImage;
}

void drawLineFromDot(unsigned int **matrice, Dot *d1, Dot *d2, double width, double height){
    const int hi = height;
    if (d1->X == d2->X){
        int wi = width;
        if(d1->X < wi && d1->X >=0){
            // Column => x = constant
            for (int y = 0; y < hi; y++){
                matrice[y][d1->X]++;
            }
        }
    }else {
        // Calculate the a and b values for the equation y = a * x + b
        double a = ((double)d2->Y - (double)d1->Y) / ((double)d2->X - (double)d1->X);
        double b = (double)d2->Y - (a * (double)d2->X);
        printf("y = %fx + %f\n", a, b);
        // Going trough the matrice and calculate the points on the line
        for (double x = 0; x < width; x+=0.1)
        {
            double y = a * x + b;
            int xi = x, yi = y;
            if (yi >= 0 && yi < hi){
                matrice[yi][xi]++;
            }
        }
    }
}

void accToBmp(unsigned int **matrice, unsigned int width,
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
            unsigned int grayValue = matrice[y][x];
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

double degrees_ToRadians(double degrees){
    return degrees * M_PI /180.0;
}