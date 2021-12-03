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

Image newImage(SDL_Surface *surface, int matrix, unsigned int width,
               unsigned height)
{
    Image image;

    image.width = width;
    image.height = height;

    image.pixels = calloc(width, sizeof(Pixel *));

    if (image.pixels == NULL)
    {
        errx(EXIT_FAILURE,
             "Error while allocating pixels pointers for the image");
    }

    unsigned int x;
    for (x = 0; x < width; x++)
    {
        image.pixels[x] = calloc(height, sizeof(Pixel));
        if (image.pixels[x] == NULL)
        {
            errx(EXIT_FAILURE,
                 "Error while allocating pixels pointers for the image");
        }
    }

    SDL_Color rgb;
    Uint32 pixel;
    double averageColor = 0;

    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            if (surface == NULL)
            {
                updatePixelToSameValue(&image.pixels[x][y], 255);
                averageColor += 255;
            }
            else
            {
                // Get pixel from surface
                pixel = get_pixel(surface, x, y);

                // Get RGB values from pixel
                SDL_GetRGB(pixel, surface->format, &rgb.r, &rgb.g, &rgb.b);

                image.pixels[x][y].r = rgb.r;
                image.pixels[x][y].g = rgb.g;
                image.pixels[x][y].b = rgb.b;
                averageColor += ((rgb.r + rgb.g + rgb.b) / 3);
            }

            if (matrix)
            {
                image.pixels[x][y].matrix = malloc(sizeof(Pixel) * (9 + 1));
                if (image.pixels[x][y].matrix == NULL)
                {
                    errx(EXIT_FAILURE,
                         "Error while allocating pixels pointers for the image "
                         "(matrix creation)");
                }
            }
        }
    }
    averageColor /= (width * height);
    image.averageColor = averageColor;

    if (matrix)
    {
        // fill the neighbours matrix
        for (unsigned int i = 0; i < width; ++i)
        {
            for (unsigned int j = 0; j < height; ++j)
            {
                FillMatrix(image.pixels, i, j, width, height);
            }
        }
    }
    return image;
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

SDL_Surface *createSurface(Image *image)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    // Create rgb surface from image
    SDL_Surface *surface =
        SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    // For each pixel in the source image
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            // Get pixel from image
            Pixel _pixel = image->pixels[x][y];

            // Get pixel value for SDL
            Uint32 pixel =
                SDL_MapRGB(surface->format, _pixel.r, _pixel.g, _pixel.b);

            // Put pixel in img
            put_pixel(surface, x, y, pixel);
        }
    }
    return surface;
}

void saveImage(Image *image, char *path)
{
    // Update SDL_Surface inside Image struct
    SDL_Surface *surface = createSurface(image);

    if (SDL_SaveBMP(surface, path) != 0)
    {
        errx(EXIT_FAILURE, "Error while saving file");
    }
    SDL_FreeSurface(surface);
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

Pixel **copyPixelsArray(Image *image, int matrix)
{
    const unsigned int w = image->width;
    const unsigned int h = image->height;
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
            if (matrix)
            {
                mask[i][j].matrix = malloc(sizeof(Pixel) * (9 + 1));

                if (mask[i][j].matrix == NULL)
                {
                    errx(EXIT_FAILURE,
                         "Error while allocating pixels pointers for the image "
                         "(matrix)");
                }
            }
        }
    }
    if (matrix)
    {
        // fill the neighbours matrix
        for (unsigned int i = 0; i < w; ++i)
        {
            for (unsigned int j = 0; j < h; ++j)
            {
                FillMatrix(mask, i, j, w, h);
            }
        }
    }
    return mask;
}

Image copyImage(Image *image, int matrix)
{
    Image res;
    res.pixels = copyPixelsArray(image, matrix);
    res.width = image->width;
    res.height = image->height;
    res.averageColor = image->averageColor;
    res.path = image->path;

    return res;
}

void freeImage(Image *image, int matrix)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    for (unsigned int x = 0; x < width; x++)
    {
        if (matrix)
        {
            for (unsigned int y = 0; y < height; y++)
            {
                free(image->pixels[x][y].matrix);
            }
        }
        free(image->pixels[x]);
    }

    free(image->pixels);
}

void updateNeigbourgs(Image *image)
{
    unsigned int h = image->height;
    unsigned int w = image->width;
    for (unsigned int i = 0; i < w; ++i)
    {
        for (unsigned int j = 0; j < h; ++j)
        {
            FillMatrix(image->pixels, i, j, w, h);
        }
    }
}

Image cropImage(Image *image, SDL_Rect *rect)
{
    Image res;
    res.width = rect->w;
    res.height = rect->h;

    // Allocate pixels form dimension of rect
    res.pixels = malloc((rect->w + 1) * sizeof(Pixel *));
    if (res.pixels == NULL)
    {
        errx(EXIT_FAILURE,
             "Error while allocating pixels pointers for the image "
             "(crop) 1");
    }
    for (unsigned int i = 0; i < rect->w; i++)
    {
        res.pixels[i] = (Pixel *)malloc((rect->h + 1) * sizeof(Pixel));

        if (res.pixels[i] == NULL)
        {
            errx(EXIT_FAILURE,
                 "Error while allocating pixels pointers for the image "
                 "(crop) 2");
        }
    }

    // Copy pixels from image to res
    for (unsigned int x = rect->x; x < rect->x + rect->w; x++)
    {
        for (unsigned int y = rect->y; y < rect->y + rect->h; y++)
        {
            res.pixels[x - rect->x][y - rect->y].r = image->pixels[x][y].r;
            res.pixels[x - rect->x][y - rect->y].g = image->pixels[x][y].g;
            res.pixels[x - rect->x][y - rect->y].b = image->pixels[x][y].b;
        }
    }
    return res;
}

void pasteOnImage(Image *src, Image *dest, SDL_Rect *rect)
{
    for (unsigned int x = rect->x; x < rect->x + rect->w; x++)
    {
        for (unsigned int y = rect->y; y < rect->y + rect->h; y++)
        {
            if (x - rect->x < src->width && y - rect->y < src->height)
            {
                dest->pixels[x][y].r = src->pixels[x - rect->x][y - rect->y].r;
                dest->pixels[x][y].g = src->pixels[x - rect->x][y - rect->y].g;
                dest->pixels[x][y].b = src->pixels[x - rect->x][y - rect->y].b;
            }
        }
    }
}