#include "Imagery/Segmentation/struct.h"

#include <assert.h>

// static void init(Graph *graph, unsigned int x, int y)
// {
//     if (y < 0)
//     {
//         y = -y;
//         graph->negativeRho = calloc(y, sizeof(unsigned int *));
//         if (graph->negativeRho == NULL)
//         {
//             errx(1, "Memory error");
//         }
//         for (size_t j = 0; (int)j < y; j++)
//         {
//             graph->negativeRho[j] = calloc(x, sizeof(unsigned int));
//             if (graph->negativeRho[j] == NULL)
//             {
//                 errx(1, "Memory error");
//             }
//         }
//     }
//     else
//     {
//         graph->positiveRho = calloc(y, sizeof(unsigned int *));
//         if (graph->positiveRho == NULL)
//         {
//             errx(1, "Memory error");
//         }
//         for (size_t j = 0; (int)j < y; j++)
//         {
//             graph->positiveRho[j] = calloc(x, sizeof(unsigned int));
//             if (graph->positiveRho[j] == NULL)
//             {
//                 errx(1, "Memory error");
//             }
//         }
//     }
// }

// void initGraph(Graph *graph, Image *image)
// {
//     const unsigned int width = image->width;
//     const unsigned int height = image->height;
//     unsigned int z = sqrt(pow(width, 2) + pow(height, 2));
//     init(graph, 180, -z);
//     init(graph, 180, z);
//     graph->generalY = z;
// }

// void freeGraph(Graph *graph, unsigned int generalY)
// {
//     for (size_t y = 0; y < generalY; y++)
//     {
//         free(graph->positiveRho[y]);
//         free(graph->negativeRho[y]);
//     }
//     free(graph->positiveRho);
//     free(graph->negativeRho);
// }

unsigned int **initMatrice(unsigned int x, unsigned int y)
{
    unsigned int **matrice = NULL;
    matrice = calloc(y + 1, sizeof(unsigned int *));
    if (matrice == NULL)
    {
        errx(1, "Memory error");
    }
    for (size_t j = 0; j < y; j++)
    {
        matrice[j] = calloc(x + 1, sizeof(unsigned int));
        if (matrice[j] == NULL)
        {
            errx(1, "Memory error");
        }
    }
    return matrice;
}

void freeMatrice(unsigned int **matrice, size_t height)
{
    for (size_t y = 0; y < height; y++)
    {
        free(matrice[y]);
    }
    free(matrice);
}

// void initDot(Dot *dot, unsigned int x, unsigned int y)
// {
//     dot->X = x;
//     dot->Y = y;
// }

// void initLine(Line *line, Dot *start, Dot *end)
// {
//     line->start = *start;
//     line->end = *end;
// }

// double lineLength(Line *line)
// {
//     return (float)(line->end.Y - line->start.Y)
//         / (float)(line->end.X - line->start.X);
// }

// void printGraph(Graph *graph)
// {
//     const unsigned int accY = graph->generalY;
//     for (long j = accY - 1; j >= 0; j--)
//     {
//         printf("[");
//         size_t i = 0;
//         for (; i < 179; i++)
//         {
//             printf("%u,", graph->positiveRho[j][i]);
//         }
//         printf("%u]\n", graph->positiveRho[j][i]);
//     }
//     for (size_t j = 1; j < accY; j++)
//     {
//         printf("[");
//         size_t i = 0;
//         for (; i < 179; i++)
//         {
//             printf("%u,", graph->negativeRho[j][i]);
//         }
//         printf("%u]\n", graph->negativeRho[j][i]);
//     }
// }

// void printMatrice(unsigned int **matrice, unsigned int height,
//                   unsigned int width)
// {
//     for (long j = height - 1; j >= 0; j--)
//     {
//         printf("[");
//         size_t i = 0;
//         for (; i < width - 1; i++)
//         {
//             printf("%u,", matrice[j][i]);
//         }
//         printf("%u]\n", matrice[j][i]);
//     }
// }

// unsigned int searchGraph(Graph *graph, long rho, size_t theta)
// {
//     if (rho < 0)
//     {
//         rho = -rho;
//         // assert(graph->negativeRho);
//         // printf("%p\n", (void*)graph->negativeRho);
//         // assert(graph->negativeRho[rho]);
//         unsigned int negative = graph->negativeRho[rho][theta];
//         // printf("searchgraph -- rho  = %li, theta = %zu, val : %u\n", rho,
//         // theta,
//         //    negative);
//         return negative;
//     }
//     unsigned int positive = graph->positiveRho[rho][theta];
//     // printf("searchgraph -- rho  = %li, theta = %zu, val : %u\n", rho,
//     theta,
//     //    positive);
//     return positive;
// }

// void addGraph(Graph *graph, long rho, size_t theta)
// {
//     if (rho < 0)
//     {
//         rho = -rho;
//         graph->negativeRho[rho][theta]++;
//     }
//     else
//     {
//         graph->positiveRho[rho][theta]++;
//     }
// }

// unsigned int graphAverage(Graph *graph)
// {
//     unsigned long total_sum = 0;
//     unsigned int nb = 0;
//     const unsigned int accY = graph->generalY;
//     for (long j = accY - 1; j >= 0; j--)
//     {
//         for (size_t i = 0; i < 180; i++)
//         {
//             unsigned int val = graph->positiveRho[j][i];
//             total_sum += val;
//             nb++;
//         }
//     }
//     for (size_t j = 1; j < accY; j++)
//     {
//         for (size_t i = 0; i < 180; i++)
//         {
//             unsigned int val = graph->negativeRho[j][i];
//             total_sum += val;
//             nb++;
//         }
//     }
//     printf("total_sum : %lu; nb : %u\n", total_sum, nb);
//     if (nb)
//     {
//         return total_sum / nb;
//     }
//     return 0;
// }