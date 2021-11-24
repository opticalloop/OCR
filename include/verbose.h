#ifndef VERBOSE_H
#define VERBOSE_H

#include <stdlib.h>
#include <string.h>

#include "GUI/gui.h"
#include "Imagery/Utils/image.h"

void saveVerbose(int verbose, Image *image, char *output_folder,
                 char *output_path, int save, int free);

void printVerbose(int verbose, char *message);

void changeImageGUI(Image *image, int gui, double value, char *message,
                    int free);

#endif