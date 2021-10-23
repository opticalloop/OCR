#ifndef SPLIT_H
#define SPLIT_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Resize/resize.h"
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/op.h"

typedef struct
{
    unsigned int xstart;
    unsigned int ystart;
    unsigned int xend;
    unsigned int yend;
} coordonnes;

/*
 *  Summary:
 *      Verify if the y line is balck or not
 *
 *  Params:
 *      *image: Image where we want to check
 *      y: the line to check
 *
 *  Return:
 *      1 if the line is black 0 if not
 */
int isBlackLine(Image *image, unsigned int y);

/*
 *  Summary:
 *
 *  Params:
 *      *image:
 *      xstart:
 *      ystart:
 *      xend:
 *      yend:
 *
 *  Return:
 *
 */
void displayblock(Image *image, unsigned int xstart, unsigned int ystart,
                  unsigned int xend, unsigned int yend);

/*
 *  Summary:
 *
 *  Params:
 *      *image:
 *
 *  Return:
 *
 */
void equalsplit(Image *image);

/*
 *  Summary:
 *
 *  Params:
 *      imagename:
 *
 *  Return:
 *
 */
void savesquare(Image *image, unsigned int iall, char *imagename);

/*
 *  Summary:
 *
 *  Params:
 *      image:
 *      seg81:
 *      save:
 *      imagename:
 *
 *  Return:
 *
 */
void split(Image *image, SDL_Surface *seg81[], int save, char *imagename);

/*
 *  Summary:
 *
 *  Params:
 *      *surface:
 *      len:
 *
 *  Return:
 *
 */

void freeList(SDL_Surface *surface[], unsigned int len);

#endif