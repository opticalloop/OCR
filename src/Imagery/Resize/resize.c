#include "Imagery/Rotations_Resize/resize.h"

static double bilinearly_interpolate(unsigned int top, unsigned int bottom,
                                     unsigned int left, unsigned int right,
                                     double horizontal_position,
                                     double vertical_position, Pixel **pixels)
{
    // Determine the values of the corners.
    double top_left = pixels[left][top].r;
    double top_right = pixels[right][top].r;
    double bottom_left = pixels[left][bottom].r;
    double bottom_right = pixels[right][bottom].r;

    // Figure out "how far" the output pixel being considered is
    // between *_left and *_right.
    double horizontal_progress = horizontal_position - (double)left;
    double vertical_progress = vertical_position - (double)top;

    // Combine top_left and top_right into one large, horizontal
    // block.
    double top_block = top_left + horizontal_progress * (top_right - top_left);

    // Combine bottom_left and bottom_right into one large, horizontal
    // block.
    double bottom_block =
        bottom_left + horizontal_progress * (bottom_right - bottom_left);

    // Combine the top_block and bottom_block using vertical
    // interpolation and return as the resulting pixel.
    return top_block + vertical_progress * (bottom_block - top_block);
}

Image resize(Image *image, unsigned int newwidth, unsigned int newheight)
{
    unsigned int width = image->width;
    unsigned int height = image->height;

    double xscale = newwidth / (double)width;
    double yscale = newheight / (double)height;

    Image newimage;
    newimage.width = newwidth;
    newimage.height = newheight;
    newimage.surface = NULL;
    newimage.path = "";

    newImage(&newimage);

    for (unsigned int x = 0; x < newwidth; x++)
    {
        for (unsigned int y = 0; y < newheight; y++)
        {
            double oldx = x / xscale;
            double oldy = y / yscale;

            unsigned int top = floor(oldy);
            unsigned int bottom = top + 1;
            unsigned int left = floor(oldx);
            unsigned int right = left + 1;

            if (top < height && left < width && bottom < height
                && right < width)
            {
                double pixel = bilinearly_interpolate(
                    top, bottom, left, right, oldx, oldy, image->pixels);

                updatePixelToSameValue(&(newimage.pixels[x][y]), pixel);
            }
        }
    }

    freeImage(image);
    return newimage;
}