// #include "Imagery/Segmentation/houghtransform.h"

// void detection(Image *image)
// {
//     const unsigned int width = image->width;
//     const unsigned int height = image->height;
//     Graph *accumulator = houghtransform(image, 10.0);
//     // printGraph(accumulator);
//     unsigned int **imageDrawn = lineSimplication(accumulator, image);
//     matriceToBmp(imageDrawn, width, height);
//     // printMatrice(imageDrawn, image->height, image->width);
//     freeGraph(accumulator, accumulator->generalY);
//     freeMatrice(imageDrawn, height);
//     free(accumulator);
// }

// Graph *houghtransform(Image *image)
// {
//     // Initialisation of accumaltor graph
//     printf("Starting to detect edges :\n-------------------------\n");
//     Graph *accumulator = malloc(sizeof(Graph));
//     accumulator->negativeRho = NULL;
//     accumulator->positiveRho = NULL;
//     initGraph(accumulator, image);

//     for (size_t y = 0; y < image->height; y++)
//     {
//         for (size_t x = 0; x < image->width; x++)
//         {
//             if (image->pixels[x][y].r)
//             {
//                 for (double theta = 0.0; theta < 180.0; theta += delta)
//                 {
//                     long rho = (long)(x * cos(theta)) + (long)(y *
//                     sin(theta)); addGraph(accumulator, rho, theta);
//                 }
//             }
//         }
//     }
//     return accumulator;
// }

// unsigned int **lineSimplication(Graph *accumulator, Image *image)
// {
//     printf("Drawing lines :\n-------------------------\n");
//     const unsigned int accY = accumulator->generalY;
//     const unsigned int width = image->width;
//     const unsigned int height = image->height;

//     searchGraph(accumulator, -1413, 0);
//     unsigned int **imageDrawn = initMatrice(width, height);
//     searchGraph(accumulator, -1413, 0);
//     const unsigned int average = graphAverage(accumulator);
//     printf("average : %u\n", average);
//     int limAcc = accY;
//     for (long y = -limAcc + 1; y < limAcc; y++)
//     {
//         for (size_t x = 0; x < 180; x++)
//         {
//             if (searchGraph(accumulator, y, x) > average)
//             {
//                 printf("draw with >>> theta = %zu and rho = %li\n", x, y);
//                 drawLineRho(image, imageDrawn, y, x, 1.0);
//             }
//         }
//     }
//     return imageDrawn;
// }

// void drawLineRho(Image *image, unsigned int **toDraw, double rho,
//                  unsigned int theta, double delta)
// {
//     const unsigned int width = image->width;
//     const unsigned int height = image->height;
//     for (double x = 0; x < width; x += delta)
//     {
//         for (double y = 0; y < height; y += delta)
//         {
//             int tmpRho = x * cos(theta) + y * sin(theta);
//             if ((double)(tmpRho) == rho && image->pixels[(unsigned
//             int)x][(unsigned int)y].r)
//             {
//                 toDraw[(unsigned int)y][(unsigned int)x]++;
//             }
//         }
//     }
// }

// void matriceToBmp(unsigned int **matrice, unsigned int width,
//                   unsigned int height)
// {
//     Image image;
//     image.width = width;
//     image.height = height;
//     image.path = ""; // To create an RGB surface
//     image.averageColor = 0;
//     image.surface = NULL;
//     image.pixels = NULL;
//     newImage(&image);
//     for (size_t y = 0; y < height; y++)
//     {
//         for (size_t x = 0; x < width; x++)
//         {
//             image.pixels[x][y].r = matrice[y][x] ? 255 : 0;
//             image.pixels[x][y].g = 0;
//             image.pixels[x][y].b = 0;
//         }
//     }
//     saveImage(&image, "drawingLine.bmp");
//     freeImage(&image);
// }
