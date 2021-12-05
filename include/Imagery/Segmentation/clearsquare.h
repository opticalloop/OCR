#ifndef CLEARSQUARE_H
#define CLEARSQUARE_H

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Segmentation/split.h"
#include "Imagery/Segmentation/struct.h"
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

/**
 * @brief
 *
 * @param M
 * @param x
 * @param y
 * @param cur
 * @param image
 * @return int
 */
int __dfs(int **M, int x, int y, int **cur, Image *image);

/**
 * @brief
 *
 * @param M
 * @param image
 */
void clear_matrice(int **M, Image *image);

/**
 * @brief
 *
 * @param image
 */
void clear_imperfections(Image *image);
#endif