#include "Imagery/Utils/image.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>
static void FillMatrix(Pixel **pixels, unsigned int x, unsigned int y,
                       unsigned int w, unsigned int h)
{
    Pixel *matrix = pixels[x][y].matrix;

    for (int i = 0; i < 9; ++i)
    {
        matrix[i] = InstantiatePixelZero();
    }
    matrix[4] = pixels[x][y];

    if (x > 0)
    {
        if (y > 0)
            matrix[0] = pixels[x - 1][y - 1];
        matrix[3] = pixels[x - 1][y];
        if (y < h - 1)
            matrix[6] = pixels[x - 1][y + 1];
    }
    if (x < w - 1)
    {
        if (y > 0)
            matrix[2] = pixels[x + 1][y - 1];
        matrix[5] = pixels[x + 1][y];
        if (y < h - 1)
            matrix[8] = pixels[x + 1][y + 1];
    }
    if (y > 0)
    {
        if (x > 0)
            matrix[0] = pixels[x - 1][y - 1];
        matrix[1] = pixels[x][y - 1];
        if (x < w - 1)
            matrix[2] = pixels[x + 1][y - 1];
    }
    if (y != h - 1)
    {
        if (x > 0)
            matrix[6] = pixels[x - 1][y + 1];
        matrix[7] = pixels[x][y + 1];
        if (x < w - 1)
            matrix[8] = pixels[x + 1][y + 1];
    }
}

void newImage(Image *image)
{
    SDL_Surface *surface = !strcmp(image->path, "")
        ? SDL_CreateRGBSurface(0, 266, 266, 24, 0, 0, 0, 0)
        : load_image(image->path);

    const unsigned int width = surface->w;
    const unsigned int height = surface->h;

    image->width = width;
    image->height = height;

    image->pixels = malloc((width + 1) * sizeof(Pixel *));

    if (image->pixels == NULL)
    {
        errx(EXIT_FAILURE,
             "Error while allocating pixels pointers for the image");
    }

    unsigned int x;
    for (x = 0; x < width; x++)
    {
        image->pixels[x] = malloc((height + 1) * sizeof(Pixel));
        if (image->pixels[x] == NULL)
        {
            errx(EXIT_FAILURE,
                 "Error while allocating pixels pointers for the image");
        }
    }
    // Make sure we don't have the '\0'
    image->pixels[x] = NULL;

    SDL_Color rgb;
    Uint32 pixel;
    double averageColor = 0;

    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            // Get pixel from surface
            pixel = get_pixel(surface, x, y);

            // Get RGB values from pixel
            SDL_GetRGB(pixel, surface->format, &rgb.r, &rgb.g, &rgb.b);

            image->pixels[x][y].r = rgb.r;
            image->pixels[x][y].g = rgb.g;
            image->pixels[x][y].b = rgb.b;

            image->pixels[x][y].matrix = NULL;
            image->pixels[x][y].matrix = malloc(sizeof(Pixel) * (9 + 1));
            if (image->pixels[x][y].matrix == NULL)
            {
                errx(EXIT_FAILURE,
                     "Error while allocating pixels pointers for the image "
                     "(matrix creation)");
            }

            averageColor += ((rgb.r + rgb.g + rgb.b) / 3);
        }
    }
    averageColor /= (width * height);
    image->averageColor = averageColor;
    image->surface = surface;

    // fill the neighbours matrix
    for (unsigned int i = 0; i < width; ++i)
    {
        for (unsigned int j = 0; j < height; ++j)
        {
            FillMatrix(image->pixels, i, j, width, height);
        }
    }
}
Pixel **copyPixelsArray(Image *image)
{
    int w = image->width;
    int h = image->height;
    Pixel **mask = malloc((w + 1) * sizeof(Pixel *));
    if (mask == NULL)
    {
        errx(EXIT_FAILURE,
             "Error while allocating pixels pointers for the image "
             "(copy Pixels Array) 1");
    }
    for (unsigned int i = 0; i < w; i++)
    {
        mask[i] = (Pixel *)malloc((h + 1) * sizeof(Pixel));

        if (mask[i] == NULL)
        {
            errx(EXIT_FAILURE,
                 "Error while allocating pixels pointers for the image "
                 "(copy Pixels Array) 2");
        }

        for (unsigned int j = 0; j < h; j++)
        { // MEDIAN FILTER
            mask[i][j].r = image->pixels[i][j].r;
            mask[i][j].g = image->pixels[i][j].g;
            mask[i][j].b = image->pixels[i][j].b;
            mask[i][j].matrix = NULL;
            mask[i][j].matrix = malloc(sizeof(Pixel) * (9 + 1));

            if (mask[i][j].matrix == NULL)
            {
                errx(EXIT_FAILURE,
                     "Error while allocating pixels pointers for the image "
                     "(matrix)");
            }
        }
    }
    // fill the neighbours matrix
    for (unsigned int i = 0; i < w; ++i)
    {
        for (unsigned int j = 0; j < h; ++j)
        {
            FillMatrix(mask, i, j, w, h);
        }
    }
    return mask;
}
void freeMatrixArray(Pixel **mask, int w, int h)
{
    for (int i = 0; i < w; ++i)
    {
        for (int j = 0; j < h; ++j)
        {
            free(mask[i][j].matrix);
        }
        free(mask[i]);
    }
    free(mask);
}

void displayImage(Image *image)
{
    // Init SDL (malloc inside so need to free at the end)
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(EXIT_FAILURE, "Could not initialize SDL: %s.\n", SDL_GetError());
    updateSurface(image);
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

    const unsigned int width = image->width;
    const unsigned int height = image->height;

    // For each pixel in the source image
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            // Get pixel from image
            Pixel _pixel = image->pixels[x][y];

            // Get pixel value for SDL
            Uint32 pixel =
                SDL_MapRGB(pixel_format, _pixel.r, _pixel.g, _pixel.b);

            // Put pixel in img
            put_pixel(image->surface, x, y, pixel);
        }
    }
}

void saveImage(Image *image, char *path)
{
    // Init SDL (malloc inside so need to free at the end)
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(EXIT_FAILURE, "Could not initialize SDL: %s.\n", SDL_GetError());

    // Update SDL_Surface inside Image struct
    updateSurface(image);

    if (SDL_SaveBMP(image->surface, path) != 0)
    {
        errx(EXIT_FAILURE, "Error while saving file");
    }

    SDL_Quit();
}

void updatePixelToSameValue(Pixel *pixel, unsigned int value)
{
    pixel->r = value;
    pixel->g = value;
    pixel->b = value;
}

Pixel InstantiatePixelZero()
{
    Pixel pixel;
    pixel.b = 0;
    pixel.r = 0;
    pixel.g = 0;

    return pixel;
}

void freeImage(Image *image)
{
    unsigned int width = image->width;
    unsigned int height = image->height;

    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            free(image->pixels[x][y].matrix);
        }
        free(image->pixels[x]);
    }

    free(image->pixels);
    SDL_FreeSurface(image->surface);
}