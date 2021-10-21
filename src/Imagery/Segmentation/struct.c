#include "Imagery/Segmentation/struct.h"

static void init(Graph *graph, unsigned int x, int y)
{
    if (y < 0)
    {
        y = -y;
        graph->negativeRho = malloc(sizeof(unsigned int *) * (y+1));
        if (graph->negativeRho == NULL){
        errx(1, "Memory error");
        }
        for (size_t j = 0; j < y; j++)
        {
            graph->negativeRho[j] = calloc(x+1, sizeof(unsigned int));
            if (graph->negativeRho[j] == NULL){
                errx(1, "Memory error");
            }
        }
    }
    else
    {
        graph->positiveRho = malloc(sizeof(unsigned int *) * (y+1));
        if (graph->positiveRho == NULL){
        errx(1, "Memory error");
        }
        for (size_t j = 0; j < y; j++)
        {
            graph->positiveRho[j] = calloc(x+1, sizeof(unsigned int));
            if (graph->positiveRho[j] == NULL){
                errx(1, "Memory error");
            }
        }   
    }
}


void initGraph(Graph *graph, Image *image)
{
    unsigned int z = sqrt(pow(image->width, 2)+pow(image->height,2));
    init(graph, 180, -z);
    //initMatrice(graph->negativeRho, 180, z);
    graph->negativeY = z;
    graph->positiveY = z;
    init(graph, 180, z);
    //initMatrice(graph->positiveRho, 180, z);
}

void freeGraph(Graph *graph)
{
    for (size_t y = 0; y < graph->positiveY; y++)
    {   
        //printf("Y : %zu, Max : %u\n", y, graph->positiveY);
        free(graph->positiveRho[y]);
    }
    free(graph->positiveRho);

    for (size_t y = 0; y < graph->negativeY; y++)
    {
        free(graph->negativeRho[y]);
    }
    free(graph->negativeRho);
}

void initMatrice(unsigned int **matrice, unsigned int x, unsigned int y)
{
    matrice = malloc(sizeof(unsigned int *) * (y+1));
    if (matrice == NULL){
        errx(1, "Memory error");
    }
    for (size_t j = 0; j < y; j++)
    {
        matrice[j] = calloc(x+1, sizeof(unsigned int));
        if (matrice[j] == NULL){
            errx(1, "Memory error");
        }
    }
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
    for (long j = graph->positiveY-1; j >= 0; j--)
    {
        printf("[");
        size_t i = 0;
        for (; i < 179; i++)
        {
            //printf("i : %zu, j : %lu\n", i, j);
            printf("%u,", graph->positiveRho[j][i]);
        }
        printf("%u]\n", graph->positiveRho[j][i]);
    }
    for (size_t j = 1; j < graph->negativeY; j++)
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
    //printf("Pos Rho Lim : %u -- Neg Rho Lim : %u -- Rho : %li -- Theta : %zu\n", graph->positiveY, graph->negativeY,rho, theta);
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
    unsigned long total = 0, nbVal = 0;
    for (long j = graph->positiveY-1; j >= 0; j--)
    {
        for (size_t i = 0; i < 180; i++)
        {
            // if (graph->positiveRho[j][i])
            // {
            //     nbVal++;
            //     total += graph->positiveRho[j][i];
            // }
            nbVal++;
            total += graph->positiveRho[j][i];
        }
    }
    for (size_t j = 1; j < graph->negativeY; j++)
    {
        for (size_t i = 0; i < 180; i++)
        {
            // if (graph->negativeRho[j][i])
            // {
            //     nbVal++;
            //     total += graph->negativeRho[j][i];
            // }
            nbVal++;
            total += graph->positiveRho[j][i];
        }
    }
    printf("Total : %lu -- Nb Val : %lu\n", total, nbVal);
    return (unsigned int)(total / nbVal);
}