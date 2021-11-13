#include "verbose.h"

void saveVerbose(int verbose, Image *image, char *output_folder,
                 char *output_path, int save, int free)
{
    if (save)
    {
        if (verbose)
        {
            printf("<-- 💾 Saved picture to %s/%s.bmp\n", output_folder,
                   output_path);
        }
        char output[200];
        snprintf(output, sizeof(output), "%s/%s.bmp", output_folder,
                 output_path);
        saveImage(image, output);
        if (free)
        {
            freeImage(image, 0);
        }
    }
}

void printVerbose(int verbose, char *message)
{
    if (verbose)
    {
        printf("%s", message);
    }
}
