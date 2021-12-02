#ifndef DRAW_H
#define DRAW_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief
 *
 * @param image
 * @param width
 * @param height
 * @return GdkPixbuf*
 */
GdkPixbuf *resize_image(GdkPixbuf *image, int width, int height);

#endif