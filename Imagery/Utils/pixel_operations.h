#ifndef PIXEL_OPERATIONS_H
#define PIXEL_OPERATIONS_H
                    
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
                    
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface* screen, SDL_Surface* image);
                    
#endif
