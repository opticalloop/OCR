#include "image.h"

#include "op.h"
#include "pixel_operations.h"

void newImage(Image* image)
{
    SDL_Surface *surface = load_image(image->path);

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

            // Calculer grayscale et black and white

            image->pixels[x][y].r = rgb.r;
            image->pixels[x][y].g = rgb.g;
            image->pixels[x][y].b = rgb.b;

            averageColor += ((rgb.r + rgb.g + rgb.b) / 3);
        }

    }
    averageColor /= (width * height);
    image->averageColor = averageColor;
}

void displayImage(Image *image)
{
    // Init SDL (malloc inside so need to free at the end)
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());

    // Load img to get the core stats like widht and height
    SDL_Surface *surface = load_image(image->path);

    // Get pixel format for the given image
    SDL_PixelFormat *pixel_format = surface->format;


    unsigned int width = image->width;
    unsigned int height = image-> height;
    
    // For each pixel in the source image
    for (unsigned int x = 0; x < width; x++){
        for (unsigned int y = 0; y < height; y++){
            
            // Get pixel from image
            Pixel _pixel = image->pixels[x][y];

            // Get pixel value for SDL
            Uint32 pixel = SDL_MapRGB(pixel_format, _pixel.r, _pixel.g, _pixel.b);
            
            // Put pixel in img
            put_pixel(surface, x, y, pixel);
        }
    }

    // Display img on screen
    SDL_Surface *screen_surface = display_image(surface);

    wait_for_keypressed();

    // Free memory took by SDL
    SDL_Quit();
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
    _image.path = "my_image.jpg";
    Image *image = &_image;

    newImage("my_image.jpg", image);

    printf("Average Color : %f\n", image->averageColor);
    
    displayImage(image);

    freeImage(image);

    printf("Image is free\n");
    
    return 0;
}