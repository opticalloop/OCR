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

/**
 * @brief
 *
 * @param image
 * @param pathToSave
 * @param verbose
 * @param save
 * @param gui
 */
void Preprocessing(Image *image, char pathToSave[], int verbose, int save,
                   int gui);

/**
 * @brief
 *
 * @param mask
 * @param image
 * @param f
 * @param filter
 * @param width
 * @param height
 */
void applyFilter(Pixel **mask, Image *image, unsigned int (*f)(Pixel *pixel),
                 enum Filter filter, unsigned int width, unsigned int height);

/**
 * @brief
 *
 * @param image
 * @return int
 */
int isWhiteImage(Image *image);

/**
 * @brief
 *
 * @param image
 */
void invert(Image *image);

/**
 * @brief
 *
 * @param image
 */
void NegativePictureIfNormal(Image *image);

/**
 * @brief
 *
 * @param image
 * @param mask
 * @param w
 * @param h
 */
void ApplyMaskToImage(Image *image, Pixel **mask, unsigned int w,
                      unsigned int h);

/**
 * @brief
 *
 * @param matrix
 * @return unsigned int
 */
unsigned int AverageFilter(Pixel *matrix);

/**
 * @brief
 *
 * @param matrix
 * @return unsigned int
 */
unsigned int MedianFilter(Pixel *matrix);

/**
 * @brief
 *
 * @param histogram
 * @return double
 */
double Thresholding(unsigned int *histogram);

/**
 * @brief
 *
 * @param image
 */
void NegativePictureIfNormal(Image *image);

/**
 * @brief
 *
 * @param image
 * @param t
 */
void adaptativeThreshold(Image *image, const double t);

/**
 * @brief
 *
 * @param image
 */
void dilate(Image *image);

/**
 * @brief
 *
 * @param image
 */
void erode(Image *image);

/**
 * @brief
 *
 * @param image
 * @return float
 */
float max_color(Image *image);

/**
 * @brief
 *
 * @param image
 */
void image_normalize_brightness(Image *image);

/**
 * @brief
 *
 * @param mat
 * @param n
 */
void image_levels(Image *mat, size_t n);

/**
 * @brief
 *
 * @param image
 * @return float
 */
float noiseLevel(Image *image);

/**
 * @brief
 *
 * @param value
 * @param min
 * @param max
 * @return unsigned int
 */
unsigned int clamp(unsigned int value, unsigned int min, unsigned int max);

#endif
