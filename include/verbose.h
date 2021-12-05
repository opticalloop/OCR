#ifndef VERBOSE_H
#define VERBOSE_H

#include <stdlib.h>
#include <string.h>

#include "GUI/gui.h"
#include "Imagery/Utils/image.h"

/**
 * @brief
 *
 * @param verbose
 * @param image
 * @param output_folder
 * @param output_path
 * @param save
 * @param free
 */
void saveVerbose(int verbose, Image *image, char *output_folder,
                 char *output_path, int save, int free);

/**
 * @brief
 *
 * @param verbose
 * @param gui
 * @param message
 */
void printVerbose(int verbose, int gui, char *message, char *terminal);

/**
 * @brief
 *
 * @param image
 * @param gui
 * @param value
 * @param message
 * @param free
 */
void changeImageGUI(Image *image, int gui, double value, char *message,
                    int free);

#endif