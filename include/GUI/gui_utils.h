#ifndef GUI_UTILS_H
#define GUI_UTILS_H

#include <SDL/SDL.h>
#include <math.h>

#include "Imagery/Utils/image.h"
#include "Imagery/Utils/pixel_operations.h"

void selectionFilter(Image *image, SDL_Rect *selection);

#endif