#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "GUI/draw.h"
#include "ocr.h"



void change_image(char * filename);
void * init_gui();

#endif