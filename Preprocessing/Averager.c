#include <err.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../Imagery/pixel_operations.h"


int main() {
    SDL_Surface *image_surface;
    SDL_Surface *screen_surface;


    image_surface * = load_image("my_image.jpg");


    return 0;
}

Uint32 LocalSum(SDL_Surface *img, int x, int y) {
    Uint32 result;
    Uint32 w = img->w - 1;
    Uint32 h = img->h - 1;

    Uint32 i = (x > 0) ? x - 1 : 0;
    Uint32 j = (y > 0) ? y - 1 : 0;

    Uint32 maxI = (x < h) ? x + 1 : h;
    Uint32 maxY = (y < w) ? y + 1 : w;
    for (; i <= maxI; ++i) {
        for (; j <= maxY; ++j) {
            result += get_pixel(img, i, j);
        }
    }

    return result;
}

SDL_Surface *Averager(SDL_Surface *img) {
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE | SDL_ANYFORMAT);
    if (screen == NULL) {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
             img->w, img->h, SDL_GetError());
    }
    int w = img->w, h = img->h;
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            put_pixel(screen, i, j, LocalSum(img, i, j));
        }
    }


    return screen;
}