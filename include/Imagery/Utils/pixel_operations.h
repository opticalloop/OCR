#ifndef PIXEL_OPERATIONS_H
#define PIXEL_OPERATIONS_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>

/*
 *  Summary:
 *
 *  Params:
 *      *surface:
 *      x:
 *      y:
 *
 *  Return:
 *
 */
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

/*
 *  Summary:
 *
 *  Params:
 *      *surface:
 *      x:
 *      y:
 *
 *  Return:
 *      void
 *
 */
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

/*
 *  Summary:
 *
 *  Params:
 *      *screen:
 *      *image:
 *
 *  Return:
 *      void
 */
void update_surface(SDL_Surface *screen, SDL_Surface *image);

#endif
