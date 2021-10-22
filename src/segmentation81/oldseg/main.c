#include "split81.h"

int main(void)
{
    Image image;
    image.path = "image_01.jpeg";
    newImage(&image);
    SDL_Surface *seg81[81];
    blackandwhite(&image);
    // displayblock(&image,10,10,110,110);
    split(image, seg81,1);
    updateSurface(&image);
    displayImage(&image);
    saveImage(&image, "out.bmp");
    freeImage(&image);
    freeList(seg81,81);
}
