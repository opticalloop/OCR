#include "Imagery/Segmentation/clearsquare.h"

int isLinePartialBlack(Image *image, unsigned int y){
    const unsigned int width = image->width;

    unsigned int nbwhite = 0;
    unsigned int nbblack = 0;

    for(unsigned int x = 0; x < width; x++){
        image->pixels[x][y].r == 255 ? nbwhite++ : nbblack++;
    }
    
    if (nbblack == 0){
        return 1;
    }

    return (double)nbwhite/(double)nbblack >= 1 ? 0 : 1;
}

int isColumnPartialBlack(Image *image, unsigned int x){
    const unsigned int height = image->height;

    unsigned int nbwhite = 0;
    unsigned int nbblack = 0;

    for(unsigned int y = 0; y < height; y++){
        //printf("x : %d - y : %d\n",x,y);
        image->pixels[x][y].r == 255 ? nbwhite++ : nbblack++;
    }
    
    if (nbblack == 0){
        return 1;
    }
    return (double)nbwhite/(double)nbblack >= 1 ? 0 : 1;
}

void clearLine(Image *image, unsigned int y){
    const unsigned int width = image->width;

    for(unsigned int x = 0; x < width; x++){
        updatePixelToSameValue(&(image->pixels[x][y]),255);
    }
}

void clearColumn(Image *image, unsigned int x){
    const unsigned int height = image->height;

    for(unsigned int y = 0; y < height; y++){
        updatePixelToSameValue(&(image->pixels[x][y]),255);
    }
}

void clearsquare(Image *image){
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    //printf("width : %d - height : %d\n", width, height);
    for(unsigned int y = 0; y < height; y++){
        for(unsigned int x = 0; x < width; x++){
            if(isLinePartialBlack(image,y)){
                clearLine(image,y);
            }
            if(isColumnPartialBlack(image,x)){
                clearColumn(image,x);
            }
        }
    }
}