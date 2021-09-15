#include "blackandwhite.h"

void blackandwhite(Image* image)
{
    int width = image->width;
    int height = image->height;

    //For each pixel of the image
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height;y++){

            Pixel pixel = image->pixels[x][y];

            if((pixel.r+ pixel.b + pixel.g)/3 >= 127){
                image->pixels[x][y].r = 255;
                image->pixels[x][y].b = 255;
                image->pixels[x][y].g = 255;
            }
            else{
                 image->pixels[x][y].r = 0;
                 image->pixels[x][y].b = 0;
                 image->pixels[x][y].g = 0;
            }
        }
    }
}
