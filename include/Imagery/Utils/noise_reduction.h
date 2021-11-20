#ifndef OCR_NOISE_REDUCTION_H
#define OCR_NOISE_REDUCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Imagery/Utils/array_sort.h"
#include "Imagery/Utils/image.h"
#include "verbose.h"

enum Filter
{
    Median,
    Binomial
};

/*
 * Summary:
 *    Apply all filters
 *
 * Params :
 *      *image : the image
 *
 * Return:
 *      void
 */
void Preprocessing(Image *image, char pathToSave[], int verbose, int save, int gui);

void applyFilter(Pixel **mask, Image *image, unsigned int(*f)(Pixel *pixel), enum Filter filter,
                unsigned int width, unsigned int height);

int isWhiteImage(Image *image);

void invert(Image *image);

void NegativePictureIfNormal(Image *image);

void ApplyMaskToImage(Image *image, Pixel **mask, unsigned int w,
                             unsigned int h);

/*
 * Summary:
 *
 * Params :
 *      *matrix : neighbours pixels
 *      *binomialFilter : Binomial filter
 *
 * Return :
 *      average value of the pixel, based on Average Formula with
 * BinominalFilter
 */

unsigned int AverageFilter(Pixel *matrix);

/*
 * Summary:
 *
 * Params :
 *  *matrix : neighbours pixels
 *
 * Return :
 *  Median value of the pixel, based on Median Formula
 */
unsigned int MedianFilter(Pixel *matrix);


/*
 * Summary:
 *
 * Params :
 *      *histogram : the image histogram
 *
 * Return :
 *      return Threshold value based on the histogram
 */
double Thresholding(unsigned int *histogram);

/*
 * Summary:
 *      If the picture contain more white pixels than black, he reverse the
 * picture Params : *image : the image
 *
 * Return:
 *      void
 */
void NegativePictureIfNormal(Image *image);

void adaptativeThreshold(Image *image, const double t);

void dilate(Image *image);

void erode(Image *image);

float max_color(Image *image);

void image_normalize_brightness(Image *image);

void image_levels(Image *mat, size_t n);

float noiseLevel(Image *image);

unsigned int clamp(unsigned int value, unsigned int min,
                          unsigned int max);

#endif
