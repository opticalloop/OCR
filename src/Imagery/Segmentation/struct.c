#include "Imagery/Segmentation/struct.h"

void initGraph(Graph *graph, Image *image)
{
    unsigned int z = image->width;
    initMatGraph(graph->negativeRho, 180, -z);
    graph->negativeY = z;
    if (image->width < image->height)
    {
        z = image->height;
    }
    graph->positiveY = z;
    initMatrice(graph->positiveRho, 180, z);
}

void freeGraph(Graph *graph)
{
    freeMatrice(graph->positiveRho, graph->positiveY);
    freeMatrice(graph->negativeRho, graph->negativeY);
}

void initMatGraph(Graph *graph, unsigned int x, unsigned y)
{
    if (y < 0)
        graph->negativeRho = malloc(sizeof(unsigned int *) * x);
    else
        graph->positiveRho = malloc(sizeof(unsigned int *) * x);
    
    if (matrice == NULL){
        errx(1, "Memory error");
    }

    for (size_t j = 0; j < y; j++)
    {
        matrice[j] = malloc(sizeof(unsigned int) * y);
        if (matrice[j] == NULL){
            errx(1, "Memory error");
    }
    for (size_t j = 0; j < x; j++)
    {
        for (size_t i = 0; i < y; i++)
        {
            matrice[j][i] = 0;
        }
    }
}

unsigned int** initMatrice(unsigned int x, unsigned y)
{
    unsigned int **matrice;
    matrice = malloc(sizeof(unsigned int *) * x);
    if (matrice == NULL){
        errx(1, "Memory error");
    }
    for (size_t j = 0; j < y; j++)
    {
        matrice[j] = malloc(sizeof(unsigned int) * y);
        if (matrice[j] == NULL){
            errx(1, "Memory error");
    }
    }
    for (size_t j = 0; j < x; j++)
    {
        for (size_t i = 0; i < y; i++)
        {
            matrice[j][i] = 0;
        }
    }
    return matrice
}

void freeMatrice(unsigned int **matrice, size_t height)
{
    for (size_t y = 0; y < height; y++)
    {
        free(matrice[y]);
    }
    free(matrice);
}

void initDot(Dot *dot, unsigned int x, unsigned int y)
{
    dot->X = x;
    dot->Y = y;
}

void initLine(Line *line, Dot *start, Dot *end)
{
    line->start = *start;
    line->end = *end;
}

double lineLength(Line *line)
{
    return (float)(line->end.Y - line->start.Y)
        / (float)(line->end.X - line->start.X);
}

void printGraph(Graph *graph)
{
    for (long j = graph->positiveY; j >= 0; j--)
    {
        printf("[");
        size_t i = 0;
        for (; i < 179; i++)
        {
            printf("%u,", graph->positiveRho[j][i]);
        }
        printf("%u]", graph->positiveRho[j][i]);
    }
    for (size_t j = 1; j < graph->negativeY; j++)
    {
        printf("[");
        size_t i = 0;
        for (; i < 179; i++)
        {
            printf("%u,", graph->negativeRho[j][i]);
        }
        printf("%u]", graph->negativeRho[j][i]);
    }
}
void printMatrice(unsigned int **matrice, unsigned int height,
                  unsigned int width)
{
    for (long j = height; j >= 0; j--)
    {
        printf("[");
        size_t i = 0;
        for (; i < width - 1; i++)
        {
            printf("%u,", matrice[j][i]);
        }
        printf("%u]", matrice[j][i]);
    }
}

unsigned int searchGraph(Graph *graph, long rho, size_t theta)
{
    if (rho < 0)
    {
        return graph->negativeRho[-rho][theta];
    }
    return graph->positiveRho[rho][theta];
}

void addGraph(Graph *graph, long rho, size_t theta)
{   
    printf("Pos Rho Lim : %u -- Neg Rho Lim : %u -- Rho : %li -- Theta : %zu\n", graph->positiveY, graph->negativeY,rho, theta);
    if (rho < 0)
    {
        graph->negativeRho[-rho][theta]++;
    }
    else
    {
        graph->positiveRho[rho][theta]++;
    }
}

unsigned int graphAverage(Graph *graph)
{
    unsigned int total = 0, nbVal = 0;
    for (long j = graph->positiveY; j >= 0; j--)
    {
        for (size_t i = 0; i < 180; i++)
        {
            if (graph->positiveRho[j][i])
            {
                nbVal++;
                total += graph->positiveRho[j][i];
            }
        }
    }
    for (size_t j = 1; j < graph->negativeY; j++)
    {
        for (size_t i = 0; i < 180; i++)
        {
            if (graph->negativeRho[j][i])
            {
                nbVal++;
                total += graph->negativeRho[j][i];
            }
        }
    }
    return total / nbVal;
}