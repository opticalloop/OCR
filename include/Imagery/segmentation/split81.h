#ifndef SPLIT81_H
#define SPLIT81_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/op.h"

int isBlackLine(Image image, unsigned int y);
void displayblock(Image *image, unsigned int xstart, unsigned int ystart,
    unsigned int xend, unsigned int yend);
void equalsplit(Image *image);
void split(Image image, SDL_Surface *[], int save, char *imagename);
void freeList(SDL_Surface *surface[], unsigned int len);

#endif