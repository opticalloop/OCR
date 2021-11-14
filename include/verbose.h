#include <stdlib.h>
#include <string.h>

#include "Imagery/Utils/image.h"

void saveVerbose(int verbose, Image *image, char *output_folder,
                 char *output_path, int save, int free);

void printVerbose(int verbose, char *message);