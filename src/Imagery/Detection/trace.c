#include "Imagery/Segmentation/trace.h"

// void initTraceImage(unsigned int **trace, size_t width, size_t height)
// {
//     trace = malloc(sizeof(unsigned int *) * height);
//     for (size_t y = 0; y < height; y++)
//     {
//         trace[y] = malloc(sizeof(unsigned int) * width);
//     }
//     for (size_t y = 0; y < height; y++)
//     {
//         for (size_t x = 0; x < width; x++)
//         {
//             trace[y][x] = 0;
//         }
//     }
// }

// void freeTraceImage(unsigned int **trace, size_t height)
// {
//     for (size_t y = 0; y < height; y++)
//     {
//         free(trace[y]);
//     }
//     free(trace);
// }

// void drawLine(unsigned int **trace, float a, float b, Dot *d1, Dot *d2)
// { // style be like y = ax+b
//     unsigned int firstValue = d1->X;
//     unsigned int secondValue = d2->X;
//     if (d1->X > d2->X)
//     {
//         firstValue = d2->X;
//         secondValue = d1->X;
//     }
//     for (unsigned int x = firstValue; x < secondValue; x++)
//     {
//         int y = a * x + b;
//         trace[x][y] += 1;
//     }
// }

// void caculateLine(unsigned int **trace, Dot *d1, Dot *d2)
// {
//     // This function calculate a and b as : y = ax+b
//     float a = (float)(d2->Y - d1->Y) / (float)(d2->X - d1->X);
//     float b = (float)d2->Y - (a * (float)d2->X);
//     printf("y= %fx + %f\n", a, b);
//     drawLine(trace, a, b, d1, d2);
// }

// void printImage(unsigned int **trace, unsigned int width, unsigned int
// height)
// {
//     printf("{\n");
//     for (unsigned int y = 0; y < height; y++)
//     {
//         printf("{");
//         unsigned int x = 0;
//         for (; x < width - 1; x++)
//         {
//             printf("%u,", trace[y][x]);
//         }
//         printf("%u}\n", trace[y][x]);
//     }
//     printf("}\n");
// }

// void testCalc(unsigned int **trace, unsigned int width, unsigned int height,
//               int nbTest)
// {
//     for (int i = 0; i < nbTest; i++)
//     {
//         Dot d1, d2;

//         initDot(&d1, rand() % width, rand() % height);
//         initDot(&d2, rand() % width, rand() % height);
//         printf("Test %i : A(%u,%u) B(%u,%u) -> ", i, d1.X, d1.Y, d2.X, d2.Y);
//         caculateLine(trace, &d1, &d2);
//     }
// }

// void matriceToImage(unsigned int **trace, unsigned int width,
//                     unsigned int height)
// {
//     Image image;
//     image.width = width;
//     image.height = height;
//     image.path = ""; // To create an RGB surface
//     image.averageColor = 0;
//     image.surface = NULL;
//     image.pixels = NULL;
//     newImage(&image);
//     for (size_t y = 0; y < width; y++)
//     {
//         for (size_t x = 0; x < height; x++)
//         {
//             image.pixels[x][y].r = trace[y][x] ? 255 : 0;
//             image.pixels[x][y].g = 0;
//             image.pixels[x][y].b = 0;
//         }
//     }
//     saveImage(&image, "../drawingLine.bmp");
//     freeImage(&image);
// }

// // int main2(int argc, char *argv[])
// // {
// //     if (argc > 2)
// //     {
// //         errx(1, "Usage : ./trace nbTest");
// //     }
// //     unsigned int nb = 10;
// //     if (argc == 2)
// //     {
// //         nb = strtoul(argv[1], NULL, 10);
// //     }
// //     unsigned int height = 266, width = 266;
// //     unsigned int len = height * width;
// //     unsigned int trace[len];
// //     initTraceImage(trace, len);
// //     testCalc(trace, width, nb);
// //     matriceToImage(trace, width, height);
// //     return 0;
// // }
