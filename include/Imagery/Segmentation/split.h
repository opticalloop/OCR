#ifndef SPLIT_H
#define SPLIT_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Rotations_Resize/resize.h"
#include "Imagery/Segmentation/clearsquare.h"
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/op.h"

/*
 *  Summary:
 *      save the block taken from the big image
 *
 *  Params:
 *      *image: the square taken from the big image
 *      iall: the position of the square in the grid to know how to name the
 * file *imagename: name of the big image
 *
 *  Return:
 *      Nothing it save directly in a folder
 *
 */
void savesquare(Image *image, unsigned int iall, char *imagename, int hexa);

/*
 *  Summary:
 *      Split the image in 81 blocks
 *
 *  Params:
 *      *image: The image split
 *
 *  Return:
 *      Nothing save is done directly and array is modify directly in the
 * function
 */
void split(Image *image, Image seg[], int save, char *imagename, int hexa);

#endif