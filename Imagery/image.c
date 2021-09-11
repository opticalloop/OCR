#include "image.h"

#include "op/op.h"
#include "Pixel_operations/pixel_operations.h"

void newImage(char* path, Image* image)
{
    SDL_Surface *surface = load_image(path);

    unsigned int width = surface->w;
    unsigned int height = surface->h;

    image->width = width;
    image->height = height;

    printf("width : %u and height : %u\n", width, height);
        
    image->pixels = (Pixel **) malloc(width * sizeof(Pixel *));

    for (unsigned int x = 0; x < width; x++){
        image->pixels[x] = (Pixel *) calloc(height, sizeof(Pixel));  
    }

    SDL_Color rgb;
    Uint32 pixel;
    double averageColor = 0;

    for (unsigned int x = 0; x < width; x++){
        for (unsigned int y = 0; y < height; y++){

            if (x == 1920){
                printf("y : %u\n", y);
            }
            
            pixel = get_pixel(surface, x, y);
            SDL_GetRGB(pixel, surface->format, &rgb.r, &rgb.g, &rgb.b);
            
            image->pixels[x][y].r = rgb.r;
            image->pixels[x][y].g = rgb.g;
            image->pixels[x][y].b = rgb.b;
            
            averageColor += rgb.r + rgb.g + rgb.b;
        }

    }
    averageColor /= (width * height);
}

void freeImage(Image* image)
{
    unsigned int width = image->width;
    unsigned int height = image->height;

    for (unsigned int x = 0; x < width; x++){
        for (unsigned int y = 0; y < height; y++){
            
        }
    }
}

int main(void)
{
    Image *image = {
        0,0,0.0, NULL
    };
    
    newImage("my_image.jpg", &image);

    printf("Average Color : %f", image->averageColor);
    
    return 0;
}