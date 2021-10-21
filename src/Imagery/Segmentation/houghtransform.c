#include "Imagery/Segmentation/houghtransform.h"

void detection(Image *image)
{
    Graph *accumulator = houghtransform(image, 0.5);
    printGraph(accumulator);
    // unsigned int m = graphMediane(accumulator);
    // unsigned int **imageDrawn;
    // imageDrawn = lineSimplication(accumulator, image, m);
    freeGraph(accumulator);
    // freeMatrice(imageDrawn, image->height);
}

Graph* houghtransform(Image *image, double delta)
{
    // Initialisation of accumaltor graph
    Graph acc; 
    acc.negativeRho = NULL;
    acc.positiveRho = NULL;
    Graph *accumulator = &acc;
    initGraph(accumulator, image);
    //printGraph(accumulator);

    for (size_t y = 0; y < image->height; y++)
    {
        for (size_t x = 0; x < image->width; x++)
        {
            if (image->pixels[x][y].r)
            {
                for (double theta = 0.0; theta <= 180.0; theta += delta)
                {
                    long rho = (long)(x * cos(theta)) + (long)(y * sin(theta));
                    addGraph(accumulator, rho, theta);
                }
            }
        }
    }
    return accumulator;
}

// unsigned int** lineSimplication(Graph *accumulator, Image *image,
//                                 unsigned int treshold)
// {
//     // unsigned int **imageDrawn = initMatrice(image->width, image->height);
    
//     // return imageDrawn;
// }