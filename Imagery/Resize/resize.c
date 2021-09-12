#include "resize.h" 

void resizeImage(Image *image, unsigned int newWidth, unsigned int newHeight)
{
    unsigned int width = image->width;
    unsigned int height = image->height;

    for (unsigned int x = 0; x < width; x++){
        for (unsigned int y = 0; y = height; y++){
            pixel = get_pixel(image, x, y);
        }
    }
}
