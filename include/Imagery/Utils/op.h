#ifndef OP_H
#define OP_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

/**
 * @brief The Op class
 *
 * This class is used to create a SDL_Surface from a file.
 */
void init_sdl();

/**
 * @brief
 *
 * @param path
 * @return SDL_Surface*
 */
SDL_Surface *load_image(char *path);

/**
 * @brief
 *
 * @param img
 * @return SDL_Surface*
 */
SDL_Surface *display_image(SDL_Surface *img);

/**
 * @brief
 *
 */
void wait_for_keypressed();

/**
 * @brief
 *
 * @param surface
 */
void SDL_FreeSurface(SDL_Surface *surface);

#endif
