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

            // Get pixel from surface
            pixel = get_pixel(surface, x, y);

            // Get RGB values from pixel
            SDL_GetRGB(pixel, surface->format, &rgb.r, &rgb.g, &rgb.b);

            image->pixels[x][y].r = rgb.r;
            image->pixels[x][y].g = rgb.g;
            image->pixels[x][y].b = rgb.b;

            averageColor += ((rgb.r + rgb.g + rgb.b) / 3);
        }

    }
    averageColor /= (width * height);
    image->averageColor = averageColor;
    image->surface = surface;
}

void displayImage(Image *image)
{
    // Init SDL (malloc inside so need to free at the end)
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());

    // Display img on screen
    display_image(image->surface);

    wait_for_keypressed();

    // Free memory took by SDL
    SDL_Quit();
}

void updateSurface(Image *image)
{
    // Get pixel format for the given image
    SDL_PixelFormat *pixel_format = image->surface->format;

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
            put_pixel(image->surface, x, y, pixel);
        }
    }
}

Image copyImage(Image *image)
{
    unsigned int width = image->width;
    unsigned int height = image->height;

    // Copy image
    Image _image;
    _image.width = width;
    _image.height = height;
    _image.averageColor = image->averageColor;
    _image.surface = load_image(image->path);
    _image.path = image->path;

    // Allocate memory
    _image.pixels = malloc((width + 1) * sizeof(Pixel *));

    if (_image.pixels == NULL){
        errx(1, "Error while allocating pixels pointers for the image");
    }
    unsigned int x;
    for (x = 0; x < width; x++){
        _image.pixels[x] = malloc((height + 1) * sizeof(Pixel));
        if (_image.pixels[x]== NULL){
            errx(1, "Error while allocating pixels pointers for the image");
        }
    }
    // Make sure we don't have the '\0'
    _image.pixels[x] = NULL;

    for (unsigned int x = 0; x < width; x++){
        for (unsigned int y = 0; y < height; y++){
            _image.pixels[x][y].r = image->pixels[x][y].r;
            _image.pixels[x][y].g = image->pixels[x][y].g;
            _image.pixels[x][y].b = image->pixels[x][y].b;
        }
    }
    updateSurface(&_image);

    return _image;
}

void saveImage(Image *image, char *path)
{
    // Init SDL (malloc inside so need to free at the end)
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());

    // Update SDL_Surface inside Image struct
    updateSurface(image);

    SDL_SaveBMP(image->surface, path);

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
