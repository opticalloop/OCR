#ifndef GUI_UTILS_H
#define GUI_UTILS_H

#include <SDL/SDL.h>
#include <math.h>
#include "Imagery/Utils/pixel_operations.h"

double bilinearly_interpolateSurface(unsigned int top,
                                     unsigned int left,
                                     double horizontal_position,
                                     double vertical_position, double top_left,
                                     double top_right, double bottom_left,
                                     double bottom_right);


void rotateSurface(SDL_Surface *surface, double angleDegree);

void selectionFilter(SDL_Surface *surface, SDL_Rect *selection);

#endif