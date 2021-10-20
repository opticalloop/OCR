#include "struct.h"

void initGraph(Graph *graph, Image *image){
    unsigned int z =  image->width;
    initMatrice(graph->negativeRho, 180, -z);
    graph->negativeY = z; 
    if (image->width < image->height){
        z = image->height;
    }
    graph->positiveY = z;
    initMatrice(graph->positiveRho, 180, z);
}

void freeGraph(Graph *graph){
    freeMatrice(graph->positiveRho, graph->positiveY);
    freeMatrice(graph->negativeRho, graph->negativeY);
}

void initMatrice(unsigned int **matrice, unsigned int x, unsigned y){
    matrice = malloc(sizeof(unsigned int*)* x);
    for (size_t j = 0; j < y; j++){
        matrice[j] = malloc(sizeof(unsigned int)*y);
    }
    for (size_t j = 0; j < x; j++){
        for (size_t i = 0; i < y; i++){
            matrice[j][i] = 0;
        }
    }
}

void freeMatrice(unsigned int **matrice, size_t height)
{
    for (size_t y = 0; y < height; y++){
        free(matrice[y]);
    }
    free(matrice);
}

void initDot(Dot *dot, unsigned int x, unsigned int y){
    dot->X = x;
    dot->Y = y;
}

void initLine(Line *line, Dot *start, Dot *end){
    line->start = *start;
    line->end = *end;
}

double lineLength(Line *line)
{
    return (float)(line->end.Y - line->start.Y) / (float)(line->end.X - line->start.X);
}

void printGraph(Graph *graph){
    for(long j = graph->positiveY-1; j >= 0; j--){
        printf("[");
        size_t i = 0;
        for(; i < 179; i++){
            printf("%u,", graph->positiveRho[j][i]);
        }
        printf("%u]", graph->positiveRho[j][i]);
    }
    for(size_t j = 1; j < graph->negativeY; j++ ){
        printf("[");
        size_t i = 0;
        for(; i < 179; i++){
            printf("%u,", graph->positiveRho[j][i]);
        }
        printf("%u]", graph->positiveRho[j][i]);
    }
}
void printMatrice(unsigned int **matrice, unsigned int height, unsigned int width){
    for(long j = height; j >= 0; j--){
        printf("[");
        size_t i = 0;
        for(; i < width - 1; i++){
            printf("%u,", matrice[j][i]);
        }
        printf("%u]", matrice[j][i]);
    }
}

unsigned int searchGraph(Graph *graph, long rho, double teta){
    if (rho < 0){
        return graph->negativeRho[abs(rho)][teta];
    }
    return graph->positiveRho[rho][teta];
}

void addGraph(Graph *graph, long rho, double teta){
    if (rho < 0){
        graph->negativeRho[-rho][teta]++;
    }else{
        graph->positiveRho[rho][teta]++;
    }
}