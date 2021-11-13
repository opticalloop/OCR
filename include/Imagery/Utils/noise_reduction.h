//
// Created by dov on 11/09/2021.
//

#ifndef OCR_NOISE_REDUCTION_H
#define OCR_NOISE_REDUCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Imagery/Utils/array_sort.h"
#include "Imagery/Utils/image.h"
#include "verbose.h"

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
void Preprocessing(Image *image, char pathToSave[], int verbose, int save);

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

unsigned int AverageFilter(Pixel *matrix, float *binomialFilter);

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
 *      *pixel : the image
 *
 * Return :
 *      median value of the pixel, based on Median Formula
 */
unsigned int ConstrastFilter(Pixel *pixel, unsigned int *histogram, int max);

/*
 * Summary:
 *      fill the histogram array based on image
 *
 * Params :
 *      *histogram : the image
 *      **pixels : image matrix
 *      w : width of the image
 *      h : height of the image
 *
 * Return:
 *      void
 */
void GetHistogram(unsigned int *histogram, Pixel **pixels, unsigned int w,
                  unsigned h);

/*
 * Summary:
 *      Apply Otsu Filter on a matrix
 * Params :
 *      **pixels : image matrix
 *      w : width of the image
 *      h : height of the image
 *      *histogram : the image histogram
 *
 * Return:
 *
 */
void OtsuFilter(Pixel **pixels, unsigned int w, unsigned int h,
                unsigned int *histogram, int verbose);

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
#endif // OCR_NOISE_REDUCTION_H
