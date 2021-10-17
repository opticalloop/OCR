#include "Imagery/Rotations/deskew.h"

#include <math.h>
#include <stdio.h>

void autoRotate(Image *image, Dot *UL, Dot *UR, Dot *DL, Dot *DR, int verbose)
{
    double xDistance = DL->x - DR->x;
    double yDistance = DR->y - DL->y;
    double rad = atan(yDistance / xDistance);
    double deg = rad * 180 / M_PI;
    
    if (verbose)
    {
        printf("Rad : %f\n", rad);
        printf("Degree : %f\n", deg);
    }
}

void resize(Image *image, Dot UL, Dot UR, Dot DL, Dot DR)
{

}