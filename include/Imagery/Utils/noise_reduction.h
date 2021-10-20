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

/*
 * Summary
 * Params :
 *  *image : the image


 *  Apply all filters
 */
void Preprocessing(Image *image);
/*
 * Summary
 * Params :
 *  *matrix : neighbours pixels
 *  *binomialFilter : Binomial filter

 * return :
 *  average value of the pixel, based on Average Formula with BinominalFilter
 */
unsigned int AverageFilter(Pixel *matrix, float *binomialFilter);
/*
 * Summary
 * Params :
 *  *matrix : neighbours pixels

 * return :
 *  median value of the pixel, based on Median Formula
 */
unsigned int MedianFilter(Pixel *matrix);

/*
 * Summary
 * Params :
 *  *pixel : the image

 * return :
 *  median value of the pixel, based on Median Formula
 */
unsigned int ConstrastFilter(Pixel pixel, unsigned int *histogram, int max);

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
                  unsigned h);

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
                unsigned int *histogram);

#endif // OCR_NOISE_REDUCTION_H
