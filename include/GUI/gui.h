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
#include "NeuralNetwork/training.h"
#include "ocr.h"

/**
 * @brief
 *
 * @param _image
 * @param GtkimageID
 */
void change_image(Image *_image, char *GtkimageID);

/**
 * @brief
 *
 * @param progress
 * @param text
 */
void edit_progress_bar(float progress, char *text);

/**
 * @brief
 *
 * @param string
 */
void edit_terminal(char *string);

/**
 * @brief
 *
 * @return void*
 */
void *init_gui();

#endif