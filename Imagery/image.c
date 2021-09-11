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
        
    image->pixels = malloc((width + 1) * sizeof(Pixel *));
    
    if (image->pixels == NULL){
        printf("Error while allocating pixels pointers for the image");
        return;
    }

    unsigned int x;
    for (x = 0; x < width; x++){
        image->pixels[x] = (Pixel *) malloc((height + 1) * sizeof(Pixel));  
        if (image->pixels[x]== NULL){
            printf("Error while allocating pixels pointers for the image");
            return;
        }
    }
    // Make sure we don't have the '\0'
    image->pixels[x] = NULL;

    SDL_Color rgb;
    Uint32 pixel;
    double averageColor = 0;

    for (unsigned int x = 0; x < width; x++){
        for (unsigned int y = 0; y < height; y++){

            pixel = get_pixel(surface, x, y);
            SDL_GetRGB(pixel, surface->format, &rgb.r, &rgb.g, &rgb.b);
            
            image->pixels[x][y].r = rgb.r;
            image->pixels[x][y].g = rgb.g;
            image->pixels[x][y].b = rgb.b;

            averageColor += ((rgb.r + rgb.g + rgb.b) / 3);
        }

    }
    averageColor /= (width * height);
    image->averageColor = averageColor;
}

void freeImage(Image* image)
{
    unsigned int width = image->width;

    for (unsigned int x = 0; x < width; x++){
        free(image->pixels[x]);
    }

    free(image->pixels);
}

int main(void)
{
    Image _image;
    _image.width = 0;
    _image.height = 0;
    _image.averageColor = 0;
    _image.pixels = NULL;
    Image *image = &_image;

    newImage("my_image.jpg", image);

    printf("Average Color : %f", image->averageColor);
    
    freeImage(image);

    return 0;
}