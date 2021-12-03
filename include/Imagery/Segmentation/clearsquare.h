#ifndef CLEARSQUARE_H
#define CLEARSQUARE_H

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Segmentation/split.h"
#include "Imagery/Utils/image.h"

/**
 * @brief
 *
 * @param image
 * @param y
 * @return int
 */
int isLinePartialBlack(Image *image, unsigned int y);

/**
 * @brief
 *
 * @param image
 * @param x
 * @return int
 */
int isColumnPartialBlack(Image *image, unsigned int x);

/**
 * @brief
 *
 * @param image
 * @param y
 */
void clearLine(Image *image, unsigned int y);

/**
 * @brief
 *
 * @param image
 * @param x
 */
void clearColumn(Image *image, unsigned int x);

/**
 * @brief
 *
 * @param image
 */
void clearsquare(Image *image);

#endif