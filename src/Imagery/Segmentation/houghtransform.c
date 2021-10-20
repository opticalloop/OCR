#include "houghtransform.h"

void houghtransform(Image *image, double delta){
    Graph *accumulator;
    initGraph(accumulator, image);
    for(size_t y = 0; y < image->height; y++){
        for(size_t x = 0; x < image->width; x++){
            if (image->pixels[x][y].r && image->pixels[x][y].g && image->pixels[x][y].b){
                for(double teta = 0.0; teta <= 180.0; teta += delta){
                    long rho = (long)(x*cos(teta)) + (long)(y*sin(teta));
                    accumulator[rho][teta] += 1;
                }
            }
        }
    }
    printGraph(accumulator);
}