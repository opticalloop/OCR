#ifndef SPLIT_H
#define SPLIT_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Rotations_Resize/resize.h"
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/op.h"

typedef struct
{
    unsigned int xstart;
    unsigned int ystart;
    unsigned int xend;
    unsigned int yend;
} Coordinates;

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
 *      Put a red square on the image
 *
 *  Params:
 *      *image: image where we want to draw
 *      xstart: coordinate of x top right of the block
 *      ystart: coordinate of y top right of the block
 *      xend: coordiate of x bottom left of the block
 *      yend: coordiate of y bottom left of the block
 *
 *  Return:
 *      Nothing it's directly done to the image
 */
void displayblock(Image *image, unsigned int xstart, unsigned int ystart,
                  unsigned int xend, unsigned int yend);

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
void savesquare(Image *image, unsigned int iall, char *imagename);

/*
Not sure we're going to use it
 */
void split(Image *image, SDL_Surface *seg81[], int save, char *imagename);

/*
 *  Summary:
 *      Free a list of SDL_Surface*
 *  Params:
 *      *surface: list to free
 *      len: len of the list
 *
 *  Return:
 *      Nothing
 */

void freeList(SDL_Surface *surface[], unsigned int len);

#endif