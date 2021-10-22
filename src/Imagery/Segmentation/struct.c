#include "Imagery/Segmentation/struct.h"

static void init(Graph *graph, unsigned int x, int y)
{
    if (y < 0)
    {
        y = -y;
        graph->negativeRho = malloc(sizeof(unsigned int *) * (y + 1));
        if (graph->negativeRho == NULL)
        {
            errx(1, "Memory error");
        }
        for (size_t j = 0; j < y; j++)
        {
            graph->negativeRho[j] = calloc(x + 1, sizeof(unsigned int));
            if (graph->negativeRho[j] == NULL)
            {
                errx(1, "Memory error");
            }
        }
    }
    else
    {
        graph->positiveRho = malloc(sizeof(unsigned int *) * (y + 1));
        if (graph->positiveRho == NULL)
        {
            errx(1, "Memory error");
        }
        for (size_t j = 0; j < y; j++)
        {
            graph->positiveRho[j] = calloc(x + 1, sizeof(unsigned int));
            if (graph->positiveRho[j] == NULL)
            {
                errx(1, "Memory error");
            }
        }
    }
}

void initGraph(Graph *graph, Image *image)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;
    unsigned int z = sqrt(pow(width, 2) + pow(height, 2));
    init(graph, 180, -z);
    init(graph, 180, z);
    graph->generalY = z;
}

void freeGraph(Graph *graph, unsigned int generalY)
{
    for (size_t y = 0; y < generalY; y++)
    {
        free(graph->positiveRho[y]);
    }
    free(graph->positiveRho);

    for (size_t y = 0; y < generalY; y++)
    {
        free(graph->negativeRho[y]);
    }
    free(graph->negativeRho);
}

unsigned int** initMatrice(unsigned int x, unsigned int y)
{
    unsigned int **matrice;
    matrice = malloc(sizeof(unsigned int *) * (y + 1));
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
    const unsigned int accY = graph->generalY;
    for (long j = accY - 1; j >= 0; j--)
    {
        printf("[");
        size_t i = 0;
        for (; i < 179; i++)
        {
            // printf("i : %zu, j : %lu\n", i, j);
            printf("%u,", graph->positiveRho[j][i]);
        }
        printf("%u]\n", graph->positiveRho[j][i]);
    }
    for (size_t j = 1; j < accY; j++)
    {
        printf("[");
        size_t i = 0;
        for (; i < 179; i++)
        {
            printf("%u,", graph->negativeRho[j][i]);
        }
        printf("%u]\n", graph->negativeRho[j][i]);
    }
}
void printMatrice(unsigned int **matrice, unsigned int height,
                  unsigned int width)
{
    for (long j = height-1; j >= 0; j--)
    {
        printf("[");
        size_t i = 0;
        for (; i < width - 1; i++)
        {
            printf("%u,", matrice[j][i]);
        }
        printf("%u]\n", matrice[j][i]);
    }
}

unsigned int searchGraph(Graph *graph, long rho, size_t theta)
{   
    if (rho < 0)
    {   
        rho = -rho;
        unsigned int negative = graph->negativeRho[rho][theta];
        printf("searchgraph -- rho  = %li, theta = %zu, val : %u\n", rho, theta, negative);
        return negative;
    }
    unsigned int positive = graph->positiveRho[rho][theta];
    printf("searchgraph -- rho  = %li, theta = %zu, val : %u\n", rho, theta, positive);
    return positive;
}

void addGraph(Graph *graph, long rho, size_t theta)
{
    if (rho < 0)
    {
        rho = -rho;
        graph->negativeRho[rho][theta]++;
    }
    else
    {
        graph->positiveRho[rho][theta]++;
    }
}