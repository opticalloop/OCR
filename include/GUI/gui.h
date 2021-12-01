#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GUI/draw.h"
#include "GUI/gui_utils.h"
#include "Imagery/Utils/op.h"
#include "ocr.h"
#include "NeuralNetwork/training.h"

void change_image(Image *_image, char *GtkimageID);

void edit_progress_bar(float progress, char *text);
void edit_terminal(char *string);
void *init_gui();

#endif