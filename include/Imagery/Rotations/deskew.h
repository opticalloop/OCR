#ifndef DESKEW_H
#define DESKEW_H

#include "Imagery/Utils/image.h"

typedef struct Dot
{
    unsigned int x;
    unsigned int y;
} Dot;

/*
 * Auto rotate the image with 4 edges, all dot are pointers because we need to
 * calcule their news cord to after resize the image 
 * Params : 
 *  *image : the image to modify 
 *  *UL : Upper left 
 *  *UR : Upper right 
 *  *DL : Down left 
 *  *DR : Down right
 *  verbose : print details
 * return :
 *  directly change the image, so void
 */
void autoRotate(Image *image, Dot *UL, Dot *UR, Dot *DL, Dot *DR, int verbose);

/*
 * Resize the image to be full screen with 4 edges
 * Params :
 *  UL : Upper left
 *  UR : Upper right
 *  DL : Down left
 *  DR : Down right
 * return :
 *  directly change the image, so void
 */

void resize(Image *image, Dot UL, Dot UR, Dot DL, Dot DR);

#endif
