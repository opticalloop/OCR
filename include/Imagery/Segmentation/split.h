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

/**
 * @brief
 *
 * @param image
 * @param iall
 * @param imagename
 * @param hexa
 */
void savesquare(Image *image, unsigned int iall, char *imagename, int hexa);

/**
 * @brief
 *
 * @param image
 * @param seg
 * @param save
 * @param imagename
 * @param hexa
 */
void split(Image *image, Image seg[], int save, char *imagename, int hexa);

#endif