
#include "Imagery/Utils/noise_reduction.h"
#include <math.h>

// static void printArray(unsigned int *array, unsigned int n)
// {
//     printf("{ ");
//     for (unsigned int i = 0; i < n - 1; ++i)
//     {
//         printf("%u, ", array[i]);
//     }
//     printf("%u }\n", array[n - 1]);
// }
// static void printArrayPixel(Pixel *array, unsigned int n)
// {
//     printf("{ ");
//     for (unsigned int i = 0; i < n - 1; ++i)
//     {
//         printf("%d, ", array[i].b);
//     }
//     printf("%d }\n", array[n - 1].b);
// }

static float *BinomialFilter()
{
    float *filter = malloc(sizeof(float) * (9 + 1));
    filter[0] = 1 / 16.;
    filter[1] = (1 / 16.) * 2;
    filter[2] = (1 / 16.) * 1;
    filter[3] = (1 / 16.) * 2;
    filter[4] = (1 / 16.) * 4;
    filter[5] = (1 / 16.) * 2;
    filter[6] = (1 / 16.) * 1;
    filter[7] = (1 / 16.) * 2;
    filter[8] = (1 / 16.) * 1;
    return filter;
}
/*
 * Summary
 * Params :
 *  *image : the image
 *  *histogram : histogram of the image
 *  *max : number of pixel in the picture

 * return :
 *  check if the picture look like a B&W  (under 5% of grayscale)
 */
// static int IsBlackAndWhite(Image *image, unsigned int *histogram,
//                            unsigned int max)
// {
//     unsigned int w = image->width;
//     unsigned int h = image->height;
//     for (unsigned int i = 0; i < w; i++)
//     {
//         for (unsigned int j = 0; j < h; j++)
//         {
//             unsigned int value = image->pixels[i][j].b;
//             if (value > 10 && value < 245
//                 && (float)(histogram[value] / max) > 0.05)
//                 return 0;
//         }
//     }
//     return 1;
// }
/*
 * Summary
 * Params :
 *  *image : the image

 * return :
 *  check if the picture got a majority of white pixels.
 */
static int isWhiteImage(Image *image)
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

void NegativePictureIfNormal(Image *image)
{
    unsigned int w = image->width;
    unsigned int h = image->height;
    if (isWhiteImage(image))
    {
        for (unsigned int i = 0; i < w; i++)
        {
            for (unsigned int j = 0; j < h; j++)
            {
                updatePixelToSameValue(&(image->pixels[i][j]),
                                       255 - image->pixels[i][j].b);
            }
        }
    }
}
static void SaveTmpPic(Image *image, char pathToSave[], char name[])
{
    char str[200];
    if (!strcmp(pathToSave, ""))
    {
        pathToSave = "Image";
    }
    snprintf(str, sizeof(str), "%s/%s.bmp", pathToSave, name);
    printf("<--     📂 Saved picture : %s\n", str);
    saveImage(image, str);
}
static void ApplyMaskToImage(Image *image, Pixel **mask, unsigned int w,
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
// static void saveMatrix(Pixel **pixels, char path[], unsigned int w,
//                        unsigned int h)
// {
//     FILE *fp;
//     fp = fopen(path, "w+");
//     for (unsigned int i = 0; i < w; i++)
//     {
//         for (unsigned int j = 0; j < h; j++)
//         {
//             char str[20];
//             sprintf(str, "%u ", pixels[i][j].b);
//             fputs(str, fp);
//         }
//         fputs("\n", fp);
//     }

//     fclose(fp);
// }
// static void saveArray(unsigned int *t, char path[], unsigned int n)
// {
//     FILE *fp;
//     fp = fopen(path, "w+");

//     for (unsigned int j = 0; j < n; j++)
//     {
//         char str[20];
//         sprintf(str, "%u", t[j]);
//         fputs(str, fp);
//         fputs("\n", fp);
//     }

//     fclose(fp);
// }
void Preprocessing(Image *image, char pathToSave[], int verbose, int save)
{
    const unsigned int w = image->width;
    const unsigned int h = image->height;

    printVerbose(verbose, "    🎨 1 Preprocessing image\n");
    printVerbose(verbose, "    📊 1.1 Getting image histogram\n");

    float *binomialFilter = BinomialFilter();

    unsigned int *histogram = calloc(256 + 1, sizeof(unsigned int));
    GetHistogram(histogram, image->pixels, w, h);

    int max = image->height * image->width;

    printVerbose(verbose, "    📸 1.2 Applying constrast Filter\n");

    // for (unsigned int i = 0; i < w; i++)
    // {
    //     for (unsigned int j = 0; j < h; j++)
    //     {
    //         updatePixelToSameValue(
    //             &(image->pixels[i][j]),
    //             ConstrastFilter(&(image->pixels[i][j]), histogram, max));
    //     }
    // }

    Pixel **mask = copyPixelsArray(image);
    updateNeigbourgs(image);

    // saveVerbose(verbose, image, pathToSave, "1.1_Contrast_filter", save, 0);
    printVerbose(verbose, "    🎥 1.3 Applying Median Filter\n");

    for (unsigned int i = 0; i < w; i++)
    {
        for (unsigned int j = 0; j < h; j++)
        {
            updatePixelToSameValue(&(mask[i][j]),
                                   MedianFilter(image->pixels[i][j].matrix));
        }
    }
    ApplyMaskToImage(image, mask, w, h);

    updateNeigbourgs(image);

    saveVerbose(verbose, image, pathToSave, "1.2_Median_filter", save, 0);
    printVerbose(verbose, "    🎬 1.4 Applying Average Filter\n");

    for (unsigned int i = 0; i < w; i++)
        for (unsigned int j = 0; j < h; j++)
            updatePixelToSameValue(
                &(image->pixels[i][j]),
                AverageFilter(mask[i][j].matrix, binomialFilter));

    saveVerbose(verbose, image, pathToSave, "1.3_Average_filter", save, 0);
    // NegativePictureIfNormal(image);
    
    // printVerbose(verbose, "    💻 1.5 Applying histogram equal\n");
    // histogram_equil(image);
    // saveVerbose(verbose, image, pathToSave, "1.4_Histogram_equal", save, 0);

    // printVerbose(verbose, "    💻 1.6 Applying histogram spreading\n");
    // histogram_spreading(image);
    // saveVerbose(verbose, image, pathToSave, "1.5_Histogram_spreading", save, 0);
    
    // double sum = 0.0;
    // for (unsigned int i = 0; i < w; i++)
    // {
    //     for (unsigned int j = 0; j < h; j++)
    //     {
    //         if (image->pixels[i][j].r < 200)
    //             sum++;
    //     }
    // }
    // sum /= (w * h);
    // printf("%f\n", sum);

    printVerbose(verbose, "    💻 1.5 Applying Adaptative Threshold Filter\n");
    adaptativeThreshold2(image, 0.15);
    saveVerbose(verbose, image, pathToSave, "1.4_Adaptative_threshold", save, 0);
    
    // printVerbose(verbose, "    💻 1.6 Applying Dilate Filter\n");
    // dilate(image);
    // saveVerbose(verbose, image, pathToSave, "1.5_Dilate_filter", save, 0);

    // printVerbose(verbose, "    💻 1.7 Applying Erode Filter\n");
    // erode(image);
    // saveVerbose(verbose, image, pathToSave, "1.6_Erode_filter", save, 0);

    // printVerbose(verbose, "    💻 1.5 Applying Adaptative Otsu Filter\n");
    // OtsuFilter(image->pixels, w, h, histogram, verbose);
    // saveVerbose(verbose, image, pathToSave, "1.4_Otsu_filter", save, 0);

    printVerbose(verbose, "    ❓ 1.6 Inverting image\n");
    NegativePictureIfNormal(image);
    saveVerbose(verbose, image, pathToSave, "1.5_Inverted_filter", save, 0);

    free(histogram);
    free(binomialFilter);
    freeMatrixArray(mask, w, h);
}

unsigned int AverageFilter(Pixel *matrix, float *binomialFilter)
{
    float result = 0;
    for (int i = 0; i < 9; ++i)
        result += matrix[i].b * binomialFilter[i];
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

/*
 * Summary
 * Params :
 *  value : value
 *  min : min
 *  max : max

 * return :
 *  clamp value between min and max
 */
static unsigned int clamp(unsigned int value, unsigned int min,
                          unsigned int max)
{
    return value < min ? min : value > max ? max : value;
}

unsigned int ConstrastFilter(Pixel *pixel, unsigned int *histogram, int max)
{
    float fact = histogram[pixel->b] / ((float)max);
    float factor = (259. * (fact + 255)) / (255 * (259 - fact));
    unsigned int v = clamp(factor * (pixel->b - 128) + 128, 0, 255);

    return v;
}

void GetHistogram(unsigned int *histogram, Pixel **pixels, unsigned int w,
                  unsigned h)
{
    for (unsigned int i = 0; i < w; ++i)
        for (unsigned int j = 0; j < h; ++j)
            histogram[pixels[i][j].b]++;
}

double Thresholding(unsigned int *histogram)
{
    // Calculate the bin_edges
    long double bin_edges[256];
    bin_edges[0] = 0.0;
    long double increment = 0.99609375;
    for (int i = 1; i < 256; i++)
        bin_edges[i] = bin_edges[i - 1] + increment;

    // Calculate bin_mids
    long double bin_mids[256];
    for (int i = 0; i < 255; i++)
    {
        // printf("%Lf ",(bin_edges[i] + bin_edges[i + 1]));
        bin_mids[i] = (bin_edges[i] + bin_edges[i + 1]) / 2;
    }

    // Iterate over all thresholds (indices) and get the probabilities weight1,
    // weight2
    long double weight1[256];
    weight1[0] = histogram[0];
    for (int i = 1; i < 256; i++)
        weight1[i] = histogram[i] + weight1[i - 1];

    int total_sum = 0;
    for (int i = 0; i < 256; i++)
        total_sum = total_sum + histogram[i];
    long double weight2[256];
    weight2[0] = total_sum;
    for (int i = 1; i < 256; i++)
        weight2[i] = weight2[i - 1] - histogram[i - 1];

    // Calculate the class means: mean1 and mean2
    long double histogram_bin_mids[256];
    for (int i = 0; i < 256; i++)
        histogram_bin_mids[i] = histogram[i] * bin_mids[i];

    long double cumsum_mean1[256];
    cumsum_mean1[0] = histogram_bin_mids[0];
    for (int i = 1; i < 256; i++)
        cumsum_mean1[i] = cumsum_mean1[i - 1] + histogram_bin_mids[i];

    long double cumsum_mean2[256];
    cumsum_mean2[0] = histogram_bin_mids[255];
    for (int i = 1, j = 254; i < 256 && j >= 0; i++, j--)
        cumsum_mean2[i] = cumsum_mean2[i - 1] + histogram_bin_mids[j];

    long double mean1[256];
    for (int i = 0; i < 256; i++)
        mean1[i] = cumsum_mean1[i] / weight1[i];

    long double mean2[256];
    for (int i = 0, j = 255; i < 256 && j >= 0; i++, j--)
        mean2[j] = cumsum_mean2[i] / weight2[j];

    // Calculate Inter_class_variance
    long double Inter_class_variance[255];
    long double dnum = 10000000000;
    for (int i = 0; i < 255; i++)
        Inter_class_variance[i] =
            ((weight1[i] * weight2[i] * (mean1[i] - mean2[i + 1])) / dnum)
            * (mean1[i] - mean2[i + 1]);

    // Maximize interclass variance
    long double maxi = 0;
    int getmax = 0;
    for (int i = 0; i < 255; i++)
    {
        if (maxi < Inter_class_variance[i])
        {
            maxi = Inter_class_variance[i];
            getmax = i;
        }
    }
    return bin_mids[getmax];
}

void OtsuFilter(Pixel **pixels, unsigned int w, unsigned int h,
                unsigned int *histogram, int verbose)
{
    double threshold = Thresholding(histogram);

    if (verbose)
        printf("    📊 1.5.1 Threshold value : %f\n", threshold);
    for (unsigned int i = 0; i < w; i++)
    {
        for (unsigned int j = 0; j < h; j++)
        {
            updatePixelToSameValue(&(pixels[i][j]),
                                   pixels[i][j].b >= threshold ? 255 : 0);
        }
    }
}

void adaptativeThreshold2(Image *image, const double t)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    const int s2 = fmax(width, height) / 16;
    unsigned long* integral_image = calloc(width * height, sizeof(unsigned long));
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
            integral_image[i * height + j] = integral_image[(i - 1) * height + j] + sum;
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
            sum = integral_image[x2 * height + y2] - integral_image[x2 * height + (y1 - 1)] - 
                  integral_image[(x1 - 1) * height + y2] + integral_image[(x1 - 1) * height + (y1 - 1)];
            
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
    Pixel **_pixels = malloc(sizeof(Pixel *) * (width + 1));
    if (_pixels == NULL)
    {
        errx(EXIT_FAILURE, "Error while allocating memory");
    }

    unsigned int x = 0;
    for (; x < width; x++)
    {
        _pixels[x] = malloc(sizeof(Pixel) * (height + 1));
        if (_pixels[x] == NULL)
        {
            errx(EXIT_FAILURE, "Error while allocating memory");
        }
    }
    // '\0'
    _pixels[x] = NULL;

    // Copy of all pixel
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            // Consider that the image is in grayscale
            updatePixelToSameValue(&(_pixels[x][y]), image->pixels[x][y].r);
            updatePixelToSameValue(&(image->pixels[x][y]), 0);
        }
    }

    for (unsigned int i = 1; i < width - 1; i++)
    {
        for (unsigned int j = 1; j < height - 1; j++)
        {
            // If white and black around
            if (_pixels[i][j].r == 255)
            {
                // Check if white pixel around
                if (_pixels[i + 1][j].r != 0 && _pixels[i][j - 1].r != 0 &&
                    _pixels[i][j + 1].r != 0 && _pixels[i - 1][j].r != 0)
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

void erode(Image *image)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    // Create two dimensional array of pixels
    Pixel **_pixels = malloc(sizeof(Pixel *) * (width + 1));
    if (_pixels == NULL)
    {
        errx(EXIT_FAILURE, "Error while allocating memory");
    }

    unsigned int x = 0;
    for (; x < width; x++)
    {
        _pixels[x] = malloc(sizeof(Pixel) * (height + 1));
        if (_pixels[x] == NULL)
        {
            errx(EXIT_FAILURE, "Error while allocating memory");
        }
    }
    // '\0'
    _pixels[x] = NULL;

    // Copy of all pixel
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            // Consider that the image is in grayscale
            updatePixelToSameValue(&(_pixels[x][y]), image->pixels[x][y].r);
            updatePixelToSameValue(&(image->pixels[x][y]), 0);
        }
    }

    for (unsigned int i = 1; i < width - 1; i++)
    {
        for (unsigned int j = 1; j < height - 1; j++)
        {
            // Black pixel
            if (_pixels[i][j].r == 0)
            {
                // Check if white pixel around
                if (_pixels[i + 1][j].r != 255 && _pixels[i][j - 1].r != 255 &&
                    _pixels[i][j + 1].r != 255 && _pixels[i - 1][j].r != 255)
                {
                    updatePixelToSameValue(&(image->pixels[i][j]), 0);
                    continue;
                }
            }
            updatePixelToSameValue(&(image->pixels[i][j]), 255);
        }
    }
    
    // Free
    for (unsigned int i = 0; i < width; i++)
    {
        free(_pixels[i]);
    }
    free(_pixels);
}

unsigned int cumulative_histogram_rec(unsigned int *hist, int i, double div){
    if(i == 0){
        hist[i] = (hist[i] * 255.)/ div;
        return hist[i];
    }
    else{
        hist[i] = (hist[i] * 255.) / div;
        hist[i] += cumulative_histogram_rec(hist,i - 1,div);
        return hist[i];
    }
}

unsigned int *cumulative_histogram(Image *image){
    const unsigned int w = image->width;
    const unsigned int h = image->height;
    double div = (double)(w * h);
    unsigned int *new_hist = calloc(256, sizeof(unsigned int));
    GetHistogram(new_hist, image->pixels, w, h);
    cumulative_histogram_rec(new_hist, 255, div);
    return new_hist;
}

void histogram_equil(Image *image)
{
    const unsigned int w = image->width;
    const unsigned int h = image->height;
    unsigned int *hist = cumulative_histogram(image);
    for (int i = 0; i < w; i++)
    {   
        for (int j = 0; j < h; j++)
        {
            updatePixelToSameValue(&(image->pixels[i][j]), hist[image->pixels[i][j].r]);
        }
    }
    free(hist); 
}

void histogram_spreading(Image *image)
{
    const unsigned int w = image->width;
    const unsigned int h = image->height;
    unsigned int *new_hist = calloc(256, sizeof(unsigned int));
    GetHistogram(new_hist, image->pixels, w, h);

    unsigned int max = 0;
    unsigned int min = 0;
    for (int i = 1; i < 256; i++)
    {
        if (new_hist[i] > new_hist[max])
            max = i;
        else if (new_hist[i] < new_hist[min])
            min = i;
    }
    printf("Max : %d, Min : %d\n", max, min);
    double val;
    for (int i = 0; i < w; i++)
    {   
        for (int j = 0; j < h; j++)
        {
            val = (double) (((image->pixels[i][j].r - min) / (max - min)) * 255.0);
            // printf("Val : %f\n", val);
            updatePixelToSameValue(&(image->pixels[i][j]), 
            val);
        }
    }
    free(new_hist);
}