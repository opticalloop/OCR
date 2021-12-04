
#include "Imagery/Utils/noise_reduction.h"

#include <math.h>

#define NOISE_THRESHOLD 0.5

void Preprocessing(Image *image, char pathToSave[], int verbose, int save,
                   int gui)
{
    const unsigned int w = image->width;
    const unsigned int h = image->height;

    printVerbose(verbose, 0, "    🎨 1 Preprocessing image\n");

    // CONTRAST
    printVerbose(verbose, 0, "    📸 1.1 Applying constrast Filter\n");
    image_levels(image, 10);
    invert(image);
    image_normalize_brightness(image);
    saveVerbose(verbose, image, pathToSave, "1.1_Contrast_filter", save, 0);

    changeImageGUI(image, gui, 0.1, "Contrast filter", 0);

    // MEDIAN
    printVerbose(verbose, 0, "    🎥 1.2 Applying Median Filter\n");
    Pixel **mask = copyPixelsArray(image, 1);
    updateNeigbourgs(image);
    applyFilter(mask, image, MedianFilter, Median, w, h);
    ApplyMaskToImage(image, mask, w, h);
    updateNeigbourgs(image);
    saveVerbose(verbose, image, pathToSave, "1.2_Median_filter", save, 0);
    changeImageGUI(image, gui, 0.15, "Median filter", 0);

    // AVERAGE
    printVerbose(verbose, 0, "    🎬 1.3 Applying Average Filter\n");
    applyFilter(mask, image, AverageFilter, Binomial, w, h);
    saveVerbose(verbose, image, pathToSave, "1.3_Average_filter", save, 0);
    changeImageGUI(image, gui, 0.2, "Average filter", 0);

    // ADAPTATIVE THRESHOLD
    printVerbose(verbose, 0,
                 "    💻 1.4 Applying Adaptative Threshold Filter\n");
    float noise = noiseLevel(image);
    if (verbose)
        printf("    👍 1.4.1 Noise level : %f\n", noise);
    adaptativeThreshold(image, noise > 300 ? 0.5 : 0.15);
    saveVerbose(verbose, image, pathToSave, "1.4_Adaptative_threshold", save,
                0);
    changeImageGUI(image, gui, 0.25, "Adaptative threshold", 0);

    // DILATE
    printVerbose(verbose, 0, "    🧱 1.5 Smoothing image\n");
    dilate(image);
    saveVerbose(verbose, image, pathToSave, "1.5_Smooth_filter", save, 0);
    changeImageGUI(image, gui, 0.3, "Smoothed image", 0);

    // INTERTING
    printVerbose(verbose, 0, "    ❓ 1.6 Inverting image\n");
    NegativePictureIfNormal(image);
    saveVerbose(verbose, image, pathToSave, "1.6_Inverted_filter", save, 0);
    changeImageGUI(image, gui, 0.35, "Inverted image", 0);

    freeMatrixArray(mask, w, h);
}

void applyFilter(Pixel **mask, Image *image, unsigned int (*f)(Pixel *pixel),
                 enum Filter filter, unsigned int width, unsigned int height)
{
    for (unsigned int i = 0; i < width; i++)
    {
        for (unsigned int j = 0; j < height; j++)
        {
            if (filter == Median)
            {
                updatePixelToSameValue(&(mask[i][j]),
                                       (*f)(image->pixels[i][j].matrix));
            }
            else
            {
                updatePixelToSameValue(&(mask[i][j]), (*f)(mask[i][j].matrix));
            }
        }
    }
}

int isWhiteImage(Image *image)
{
    unsigned int w = image->width;
    unsigned int h = image->height;
    int whitePixel = 0;
    int blackPixel = 0;

    for (unsigned int i = 0; i < w; i++)
    {
        for (unsigned int j = 0; j < h; j++)
        {
            if (image->pixels[i][j].b == 0)
                blackPixel++;
            else
                whitePixel++;
        }
    }
    return whitePixel > blackPixel;
}

void invert(Image *image)
{
    const unsigned int w = image->width;
    const unsigned int h = image->height;
    for (unsigned int i = 0; i < w; i++)
    {
        for (unsigned int j = 0; j < h; j++)
        {
            updatePixelToSameValue(&(image->pixels[i][j]),
                                   255 - image->pixels[i][j].b);
        }
    }
}

void NegativePictureIfNormal(Image *image)
{
    if (isWhiteImage(image))
    {
        invert(image);
    }
}

void ApplyMaskToImage(Image *image, Pixel **mask, unsigned int w,
                      unsigned int h)
{
    for (unsigned int i = 0; i < w; i++)
    {
        for (unsigned int j = 0; j < h; j++)
        {
            updatePixelToSameValue(&(image->pixels[i][j]), mask[i][j].b);
        }
    }
}

unsigned int AverageFilter(Pixel *matrix)
{
    float filter[9] = { 1 / 16,        (1 / 16.) * 2, (1 / 16.) * 1,
                        (1 / 16.) * 2, (1 / 16.) * 4, (1 / 16.) * 2,
                        (1 / 16.) * 1, (1 / 16.) * 2, (1 / 16.) * 1 };
    float result = 0;
    for (int i = 0; i < 9; ++i)
        result += matrix[i].b * filter[i];
    return (unsigned int)result;
}

unsigned int MedianFilter(Pixel *matrix)
{
    Pixel matrix2[9];
    for (int i = 0; i < 9; ++i)
        matrix2[i] = matrix[i];

    array_sort(matrix2, 9);
    return matrix2[4].b;
}

void adaptativeThreshold(Image *image, const double t)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    const int s2 = fmax(width, height) / 16;
    unsigned long *integral_image =
        calloc(width * height, sizeof(unsigned long));
    long sum = 0;
    unsigned int count = 0;
    int x1, y1, x2, y2;

    for (unsigned int y = 0; y < height; y++)
    {
        sum += image->pixels[0][y].r;
        integral_image[y] = sum;
    }

    for (unsigned int i = 1; i < width; i++)
    {
        sum = 0;
        for (int j = 0; j < height; j++)
        {
            sum += image->pixels[i][j].r;
            integral_image[i * height + j] =
                integral_image[(i - 1) * height + j] + sum;
        }
    }
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            x1 = fmax(i - s2, 1);
            x2 = fmin(i + s2, width - 1);
            y1 = fmax(j - s2, 1);
            y2 = fmin(j + s2, height - 1);
            count = (x2 - x1) * (y2 - y1);
            sum = integral_image[x2 * height + y2]
                - integral_image[x2 * height + (y1 - 1)]
                - integral_image[(x1 - 1) * height + y2]
                + integral_image[(x1 - 1) * height + (y1 - 1)];

            // printf("Previous : %u\n", image->pixels[i][j].r);
            if (image->pixels[i][j].r * count < sum * (1.0 - t))
            {
                updatePixelToSameValue(&(image->pixels[i][j]), 0);
            }
            else
            {
                updatePixelToSameValue(&(image->pixels[i][j]), 255);
            }
            // printf("After : %u\n", image->pixels[i][j].r);
        }
    }
    // Free
    free(integral_image);
}

void dilate(Image *image)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    // Create two dimensional array of pixels
    Pixel **_pixels = copyPixelsArray(image, 0);

    for (unsigned int i = 1; i < width - 1; i++)
    {
        for (unsigned int j = 1; j < height - 1; j++)
        {
            // If white and black around
            if (_pixels[i][j].r == 255)
            {
                // Check if white pixel around
                if (_pixels[i + 1][j - 1].r != 0 && _pixels[i + 1][j].r != 0
                    && _pixels[i + 1][j + 1].r != 0 && _pixels[i][j - 1].r != 0
                    && _pixels[i][j + 1].r != 0 && _pixels[i - 1][j - 1].r != 0
                    && _pixels[i - 1][j].r != 0 && _pixels[i - 1][j + 1].r != 0)
                {
                    updatePixelToSameValue(&(image->pixels[i][j]), 255);
                    continue;
                }
            }
            updatePixelToSameValue(&(image->pixels[i][j]), 0);
        }
    }

    // Free
    for (unsigned int i = 0; i < width; i++)
    {
        free(_pixels[i]);
    }
    free(_pixels);
}

float max_color(Image *image)
{
    float m = 0;
    for (size_t x = 0; x < image->width; x++)
    {
        for (size_t y = 0; y < image->height; y++)
        {
            if (image->pixels[x][y].r > m)
                m = image->pixels[x][y].r;
        }
    }

    return m;
}

void image_normalize_brightness(Image *image)
{
    float m = max_color(image);
    for (size_t x = 0; x < image->width; x++)
    {
        for (size_t y = 0; y < image->height; y++)
        {
            updatePixelToSameValue(&(image->pixels[x][y]),
                                   255 - image->pixels[x][y].r * (255.f / m));
        }
    }
}

void image_levels(Image *mat, size_t n)
{
    for (size_t x = 0; x < mat->width; x++)
    {
        for (size_t y = 0; y < mat->height; y++)
        {
            float v = mat->pixels[x][y].r;
            for (size_t i = 0; i < n; i++)
            {
                if (v >= i * (255.f / n) && v <= (i + 1) * (255.f / n))
                {
                    updatePixelToSameValue(&(mat->pixels[x][y]),
                                           (i + 1) * (255.f / n));
                }
            }
        }
    }
}

float noiseLevel(Image *image)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    float count = 0.0;
    double medium = 0.0;
    for (unsigned int i = 0; i < width; i++)
    {
        for (unsigned int j = 0; j < height; j++)
        {
            medium = 0.0;
            for (unsigned int k = 0; k < 9; k++)
            {
                medium += image->pixels[i][j].matrix[k].r;
            }
            medium /= 9;

            if (abs(1 - (image->pixels[i][j].r / medium)) > NOISE_THRESHOLD)
                count++;
        }
    }

    return count;
}

inline unsigned int clamp(unsigned int value, unsigned int min, unsigned int max)
{
    return value < min ? min : value > max ? max : value;
}
