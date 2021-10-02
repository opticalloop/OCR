
#include "noise_reduction.h"
#include <stdio.h>
#include <string.h>

static void printArray(unsigned int *array, unsigned int n) {
    printf("{ ");
    for (unsigned int i = 0; i < n - 1; ++i) {
        printf("%u, ", array[i]);
    }
    printf("%u }\n", array[n - 1]);
}


static float *BinomialFilter() {
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

//static void applyFilter( Pixel **mask, void (* filter,Pixel ** dest){
//    for (unsigned int i = 0; i < w; i++)
//        for (unsigned int j = 0; j < h; j++)
//            updatePixelToSameValue(&(dest[i][j]), filter);

//}
void Preprocessing(Image *image) {
    unsigned int w = image->width;
    unsigned int h = image->height;
    printf("--------------------------\n");
    printf("Applying preprocessing filters\n");

    printf("Calculating Histogram\n");

    float *binomialFilter = BinomialFilter();


    Pixel **mask = copyPixelsArray(image);

    printf("Applying Median Filter\n");
    for (unsigned int i = 0; i < w; i++)
        for (unsigned int j = 0; j < h; j++)
            updatePixelToSameValue(&(mask[i][j]), MedianFilter(image->pixels[i][j].matrix));

    printf("Applying Average Filter\n");
    for (unsigned int i = 0; i < w; i++)
        for (unsigned int j = 0; j < h; j++)
            updatePixelToSameValue(&(image->pixels[i][j]), AverageFilter(mask[i][j].matrix, binomialFilter));

    unsigned int *histogram = GetHistogram(image->pixels, w, h);
    printArray(histogram, 255);
    printf("Applying constrast Filter\n");
    for (unsigned int i = 0; i < w; i++)
        for (unsigned int j = 0; j < h; j++)
            updatePixelToSameValue(&(image->pixels[i][j]), ConstrastFilter(image->pixels[i][j], histogram));

//    printf("Applying Otsu Filter\n");
//    OtsuFilter(image->pixels, w, h, histogram);



//    for (unsigned int i = 0; i < w; i++)
//        for (unsigned int j = 0; j < h; j++)
//            updatePixelToSameValue(&(image->pixels[i][j]), mask[i][j].b);



    free(binomialFilter);
    free(histogram);
    freeMatrixArray(mask, w, h);

}


unsigned int AverageFilter(Pixel *matrix, float *binomialFilter) {
    float result = 0;
    for (unsigned int i = 0; i < 9; ++i) result += matrix[i].b * binomialFilter[i];
    return (unsigned int) result;
}


unsigned int MedianFilter(Pixel *matrix) {
    Pixel *matrix2 = malloc(sizeof(Pixel) * (9 + 1));
    for (int i = 0; i < 9; ++i) matrix2[i] = matrix[i];

    array_sort(matrix2, 9);

    Pixel result = matrix2[4];
    free(matrix2);
    return result.b;
}

static unsigned int clamp(unsigned int value, unsigned int min, unsigned int max) {
    return value < min ? min : value > max ? max : value;
}

unsigned int ConstrastFilter(Pixel pixel, unsigned int *histogram) {
    unsigned int fact = histogram[pixel.b];
    float factor = (259. * (fact + 255)) / (255 * (259 - fact)) *1.1;
//    printf("%f, ",factor * (pixel.b - 128) + 128);
    return clamp(factor * (pixel.b - 128) + 128, 0, 255);
}

unsigned int *GetHistogram(Pixel **pixels, unsigned int w, unsigned h) {
    unsigned int *histogram = calloc((255 + 1), sizeof(unsigned int));
    for (int i = 0; i < w; ++i)
        for (int j = 0; j < h; ++j) {
            histogram[pixels[i][j].b]++;
        }
    return histogram;
}

static double Thresholding(unsigned int *histogram) {
    int bins_num = 256;

// Calculate the bin_edges
    long double bin_edges[256];
    bin_edges[0] = 0.0;
    long double increment = 0.99609375;
    for (int i = 1; i < 256; i++)
        bin_edges[i] = bin_edges[i - 1] + increment;

// Calculate bin_mids
    long double bin_mids[256];
    for (int i = 0; i < 255; i++) {
//        printf("%Lf ",(bin_edges[i] + bin_edges[i + 1]));
        bin_mids[i] = (bin_edges[i] + bin_edges[i + 1]) / 2;
    }

// Iterate over all thresholds (indices) and get the probabilities weight1, weight2
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
                ((weight1[i] * weight2[i] * (mean1[i] - mean2[i + 1])) / dnum) * (mean1[i] - mean2[i + 1]);

// Maximize interclass variance
    long double maxi = 0;
    int getmax = 0;
    for (int i = 0; i < 255; i++) {
        if (maxi < Inter_class_variance[i]) {
            maxi = Inter_class_variance[i];
            getmax = i;
        }
    }
    return bin_mids[getmax];
}

void OtsuFilter(Pixel **pixels, unsigned int w, unsigned int h, unsigned int *histogram) {
    double threshold = Thresholding(histogram);
    printf("\tthreshold value : %f\n", threshold);
    for (unsigned int i = 0; i < w; i++)
        for (unsigned int j = 0; j < h; j++) {
//            printf("%d ",pixels[i][j].b);
            updatePixelToSameValue(&(pixels[i][j]), pixels[i][j].b >= threshold ? 255 : 0);
        }

}