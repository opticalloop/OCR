#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GUI/draw.h"
#include "GUI/gui_utils.h"
#include "Imagery/Detection/distortion_correction.h"
#include "Imagery/Rotations_Resize/resize.h"
#include "Imagery/Utils/image.h"
#include "Imagery/Utils/op.h"
#include "NeuralNetwork/training.h"
#include "Sudoku_Solver/Sudoku_Solvable/sudoku_solvable.h"
#include "Sudoku_Solver/sudoku_solver.h"
#include "ocr.h"

#pragma region "Image_management"

GdkPixbuf *image_to_pixbuf(Image *image);

void set_selected_image(GdkPixbuf *pixbuf, char *GtkimageID);

void change_image(Image *_image, char *GtkimageID);

#pragma endregion "Image_management"

#pragma region "File_management"

char *get_filename_ext(const char *filename);

void on_file_set(GtkFileChooserButton *file_chooser, gpointer data);

#pragma endregion "File_management"

#pragma region "GUI_interaction"

void set_leftPannel_status(gboolean status);

void set_buttons_options_status(gboolean status);

void edit_progress_bar(float progress, char *text);

void show_page(GtkWidget *widget, gpointer data);

void change_panel(GtkWidget *widget, gpointer data);

void cancel_edit_option(GtkWidget *widget, gpointer data);

#pragma endregion "GUI_interaction"

#pragma region "Process"

void run_process(GtkButton *button);

void stop_processing();

#pragma endregion "Process"

#pragma region "Rotate"

void edit_rotation(GtkWidget *widget, gpointer data);

void on_rotation_finished(GtkWidget *widget, gpointer data);

void rotate_img(GtkWidget *widget, gpointer data);

#pragma endregion "Rotate"

#pragma region "Resize"

void edit_resize(GtkWidget *widget, gpointer data);
void on_resize_finished(GtkWidget *widget, gpointer data);
#pragma endregion "Resize"

#pragma region "Neural Network"

void start_nn(GtkWidget *widget, gpointer data);

void reset_nn();

void cancel_nn(GtkWidget *widget, gpointer data);

#pragma endregion "Neural Network"

#pragma region "Terminal"

void reset_terminal(char *terminal_id);

void edit_terminal(char *terminal_id, char *string);

#pragma endregion "Terminal"

void show_result(unsigned int **grid, int dimension, Image *res);

void *init_gui();

void open_website();

void quit();

#endif