#ifndef GUI_UTILS_H
#define GUI_UTILS_H

#include <SDL/SDL.h>
#include <math.h>

#include "Imagery/Utils/pixel_operations.h"

double bilinearly_interpolateSurface(unsigned int top, unsigned int left,
                                     double horizontal_position,
                                     double vertical_position, double top_left,
                                     double top_right, double bottom_left,
                                     double bottom_right);

typedef struct thread_data
{
    unsigned int x;
    double angle;
    unsigned int width;
    unsigned int height;
    SDL_Surface *surface;
    SDL_Surface *tempSurface;

} thread_data;

void *thread_rotateSurface(void *data);

void selectionFilter(SDL_Surface *surface, SDL_Rect *selection);

void cropSurface(SDL_Surface *surface, SDL_Rect *selection);

#endif