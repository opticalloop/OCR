
#include "Imagery/Utils/noise_reduction.h"

static void printArray(unsigned int *array, unsigned int n)
{
    printf("{ ");
    for (unsigned int i = 0; i < n - 1; ++i)
    {
        printf("%u, ", array[i]);
    }
    printf("%u }\n", array[n - 1]);
}
static void printArrayPixel(Pixel *array, unsigned int n)
{
    printf("{ ");
    for (unsigned int i = 0; i < n - 1; ++i)
    {
        printf("%d, ", array[i].b);
    }
    printf("%d }\n", array[n - 1].b);
}
/*
 * return :
 *  binomial fitler
 */
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
static int IsBlackAndWhite(Image *image, unsigned int *histogram,
                           unsigned int max)
{
    unsigned int w = image->width;
    unsigned int h = image->height;
    for (unsigned int i = 0; i < w; i++)
    {
        for (unsigned int j = 0; j < h; j++)
        {
            unsigned int value = image->pixels[i][j].b;
            if (value > 10 && value < 245
                && (float)(histogram[value] / max) > 0.05)
                return 0;
        }
    }
    return 1;
}
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
/*
 * Summary
 * Params :
 *  *image : the image

 *  If the picture contain more white pixels than black, he reverse the picture
 */
static void NegativePictureIfNormal(Image *image)
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

/*
 * Summary
 * Params :
 *  *image : the image


 *  Apply all filters
 */
void Preprocessing(Image *image)
{
    unsigned int w = image->width;
    unsigned int h = image->height;
    printf("--------------------------\n");
    printf("Applying preprocessing filters\n");

    printf("Calculating Histogram\n");

    float *binomialFilter = BinomialFilter();

    unsigned int *histogram = calloc(256 + 1, sizeof(unsigned int));
    GetHistogram(histogram, image->pixels, w, h);
    printArray(histogram, 256);
    int max = image->height * image->width;

    printf("Applying constrast Filter\n");
    for (unsigned int i = 0; i < w; i++)
        for (unsigned int j = 0; j < h; j++)
            updatePixelToSameValue(
                &(image->pixels[i][j]),
                ConstrastFilter(image->pixels[i][j], histogram, max));
    saveImage(image, "test.bmp");
    // displayImage(image);

    Pixel **mask = copyPixelsArray(image);

    printf("Applying Median Filter\n");
    for (unsigned int i = 0; i < w; i++)
    {
        for (unsigned int j = 0; j < h; j++)
        {
            updatePixelToSameValue(&(mask[i][j]),
                                   MedianFilter(image->pixels[i][j].matrix));
        }
    }

    printf("Applying Average Filter\n");
    for (unsigned int i = 0; i < w; i++)
        for (unsigned int j = 0; j < h; j++)
            updatePixelToSameValue(
                &(image->pixels[i][j]),
                AverageFilter(mask[i][j].matrix, binomialFilter));
    GetHistogram(histogram, image->pixels, w, h);

    printf("Applying Otsu Filter\n");
    OtsuFilter(image->pixels, w, h, histogram);
    NegativePictureIfNormal(image);

    //    for (unsigned int i = 0; i < w; i++)
    //        for (unsigned int j = 0; j < h; j++)
    //            updatePixelToSameValue(&(image->pixels[i][j]), mask[i][j].b);

    free(binomialFilter);
    free(histogram);
    freeMatrixArray(mask, w, h);
}
/*
 * Summary
 * Params :
 *  *matrix : neighbours pixels
 *  *binomialFilter : Binomial filter

 * return :
 *  average value of the pixel, based on Average Formula with BinominalFilter
 */
unsigned int AverageFilter(Pixel *matrix, float *binomialFilter)
{
    float result = 0;
    for (int i = 0; i < 9; ++i)
        result += matrix[i].b * binomialFilter[i];
    return (unsigned int)result;
}
/*
 * Summary
 * Params :
 *  *matrix : neighbours pixels

 * return :
 *  median value of the pixel, based on Median Formula
 */
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
/*
 * Summary
 * Params :
 *  *pixel : the image

 * return :
 *  median value of the pixel, based on Median Formula
 */
unsigned int ConstrastFilter(Pixel pixel, unsigned int *histogram, int max)
{
    unsigned int fact = histogram[pixel.b] / max;
    float factor = (259. * (fact + 255)) / (255 * (259 - fact));
    return clamp(factor * (pixel.b - 128) + 128, 0, 255);
}
/*
 * Summary
 * Params :
 *  *histogram : the image
 *  **pixels : image matrix
 *  w : width of the image
 *  h : height of the image
 * fill the histogram array based on image
 */
void GetHistogram(unsigned int *histogram, Pixel **pixels, unsigned int w,
                  unsigned h)
{
    for (unsigned int i = 0; i < w; ++i)
        for (unsigned int j = 0; j < h; ++j)
            histogram[pixels[i][j].b]++;
}

/*
 * Summary
 * Params :
 *  *histogram : the image histogram

 * return :
 *  return Threshold value based on the histogram
 */
static double Thresholding(unsigned int *histogram)
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

/*
 * Summary
 * Params :
 *  **pixels : image matrix
 *  w : width of the image
 *  h : height of the image
 *  *histogram : the image histogram
 * Apply Otsu Filter on a matrix
 */
void OtsuFilter(Pixel **pixels, unsigned int w, unsigned int h,
                unsigned int *histogram)
{
    double threshold = Thresholding(histogram);

    printf("\tthreshold value : %f\n", threshold);
    for (unsigned int i = 0; i < w; i++)
    {
        for (unsigned int j = 0; j < h; j++)
        {
            // printf("%d ",pixels[i][j].b);
            updatePixelToSameValue(&(pixels[i][j]),
                                   pixels[i][j].b >= threshold ? 255 : 0);
        }
    }
}