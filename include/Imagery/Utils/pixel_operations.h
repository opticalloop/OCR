#ifndef PIXEL_OPERATIONS_H
#define PIXEL_OPERATIONS_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>

/**
 * @brief Get the pixel object
 *
 * @param surface
 * @param x
 * @param y
 * @return Uint32
 */
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

/**
 * @brief
 *
 * @param surface
 * @param x
 * @param y
 * @param pixel
 */
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

/**
 * @brief
 *
 * @param screen
 * @param image
 */
void update_surface(SDL_Surface *screen, SDL_Surface *image);

#endif
