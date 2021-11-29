#ifndef DRAW_H
#define DRAW_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GdkPixbuf *resize_image(GdkPixbuf *image, int width, int height);

#endif