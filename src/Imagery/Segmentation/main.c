#include "houghtransform.h"

int main()
{
    Image image;
    image.path = "../image_05.jpeg"; // To create an RGB surface
    image.averageColor = 0;
    image.surface = NULL;
    image.pixels = NULL;
    newImage(&image);
    reverse_color(&image);
    houghtransform(&image, 2.0);
    // saveImage(&image, "test.bmp");
    freeImage(&image);
    return 0;
}