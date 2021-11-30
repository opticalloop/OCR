#ifndef PERSPECTIVE_CORRECTION_H
#define PERSPECTIVE_CORRECTION_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Imagery/Detection/inverse_matrix.h"
#include "Imagery/Detection/struct.h"
#include "Imagery/Utils/image.h"
#include "verbose.h"

Image correct_perspective(Image *image, Square *selected_square,
                          int verbose_mode, char *verbose_path);

#endif