#ifndef SPLIT9_H
#define SPLIT9_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "Imagery/Color_Treatment/blackandwhite.h"
#include "Imagery/Resize/resize.h"
#include "Imagery/Segmentation/clearsquare.h"
#include "Imagery/Segmentation/split.h"
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/op.h"

void split9(Image *image, SDL_Surface *seg81[], int save, char *imagename);

#endif