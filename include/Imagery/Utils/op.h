#ifndef OP_H
#define OP_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

/*
 *  Summary:
 *
 *  Params:
 *
 *  Return:
 *      void
 */
void init_sdl();

/*
 *  Summary:
 *
 *  Params:
 *      *path:
 *
 *  Return:
 *
 */
SDL_Surface *load_image(char *path);

/*
 *  Summary:
 *
 *  Params:
 *      *img:
 *
 *  Return:
 *
 */
SDL_Surface *display_image(SDL_Surface *img);

/*
 *  Summary:
 *
 *  Params:
 *      *img:
 *
 *  Return:
 *      void
 */
void wait_for_keypressed();

/*
 *  Summary:
 *
 *  Params:
 *      *img:
 *
 *  Return:
 *      void
 */
void SDL_FreeSurface(SDL_Surface *surface);

#endif
