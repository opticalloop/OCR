#ifndef SPLIT9_H
#define SPLIT9_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Rotations_Resize/resize.h"
#include "Imagery/Segmentation/clearsquare.h"
#include "Imagery/Segmentation/split.h"
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/op.h"

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
void split9(Image *image, Image seg81[81], int save, char *imagename);

#endif