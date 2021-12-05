#include "GUI/draw.h"

// function that resize an image
// input:
//   image: image to resize
//   width: new width
//   height: new height
// output:
//   resized image
GdkPixbuf *resize_image(GdkPixbuf *image, int width, int height)
{
    GdkPixbuf *resized_image =
        gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
    gdk_pixbuf_scale(image, resized_image, 0, 0, width, height, 0, 0,
                     (double)width / gdk_pixbuf_get_width(image),
                     (double)height / gdk_pixbuf_get_height(image),
                     GDK_INTERP_BILINEAR);
    return resized_image;
}
