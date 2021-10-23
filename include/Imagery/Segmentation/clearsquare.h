#ifndef CLEARSQUARE_H
#define CLEARSQUARE_H

#include "Imagery/Utils/image.h"

/*
 *  Summary:
 *      Check if there is more than 50% black on the line
 *  Params:
 *      *image: The image to check
 *      y:  The line to check
 *
 *  Return:
 *      1 if there is more than 50% black on the line
 */
int isLinePartialBlack(Image *image, unsigned int y);

/*
 *  Summary:
 *      Check if there is more than 50% black on the column
 *  Params:
 *      *image: The image to check
 *      x: The colum to check
 *
 *  Return:
 *      1 if there is more than 50% of black 1 otherwise
 */
int isColumnPartialBlack(Image *image, unsigned int x);

/*
 *  Summary:
 *      Make the Line all white
 *  Params:
 *      *image: The image to clear
 *      y: the line to clear
 *
 *  Return:
 *      Nothing clear is applied to the image directly
 */
void clearLine(Image *image, unsigned int y);

/*
 *  Summary:
 *      Make the Column all white
 *  Params:
 *      *image: The image to clear 
 *      x: the column to clear 
 *
 *  Return:
 *      Nothing clear is applied to the image directly
 */
void clearColumn(Image *image, unsigned int x);

/*
 *  Summary:
 *      remove all the line from an image when the is more than 50% of black
 *  Params:
 *      *image: The image to clear
 *
 *  Return:
 *      Nothing clear is applied to the image directly
 */
void clearsquare(Image *image);

#endif