#ifndef RESIZE_H
#define RESIZE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "../Utils/image.h"

void resizeImage(Image *image, unsigned int newWidth, unsigned int newHeight);

#endif