#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GUI/draw.h"
#include "GUI/gui_utils.h"
#include "ocr.h"

void change_image(SDL_Surface *surface, char *GtkimageID);

void edit_progress_bar(float progress, char *text);

void *init_gui();

#endif