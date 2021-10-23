#include "Imagery/Segmentation/split.h"

int isBlackLine(Image *image, unsigned int y)
{
    const unsigned int width = image->width;

    for (unsigned int x = 0; x < width; x++)
    {
        if (image->pixels[x][y].r != 0)
        {
            return 0;
        }
    }
    return 1;
}

/*
 *To test the square taken in the image
 */

void displayblock(Image *image, unsigned int xstart, unsigned int ystart,
                  unsigned int xend, unsigned int yend)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    unsigned int y = ystart;
    unsigned int x = xstart;
    if (xend < width && yend < height)
    {
        for (; ystart <= yend; ystart++)
        {
            for (unsigned int xstart = x; xstart <= xend; xstart++)
            {
                if (ystart == y || ystart == yend)
                {
                    image->pixels[xstart][ystart].r = 255;
                    image->pixels[xstart][ystart].g = 0;
                    image->pixels[xstart][ystart].b = 0;
                }
                else
                {
                    image->pixels[x][ystart].r = 255;
                    image->pixels[xend][ystart].r = 255;
                    image->pixels[x][ystart].g = 0;
                    image->pixels[xend][ystart].g = 0;
                    image->pixels[x][ystart].b = 0;
                    image->pixels[xend][ystart].b = 0;
                    break;
                }
            }
        }
    }
    else
    {
        printf("Out of bounds\n");
    }
}

void savesquare(Image *image, unsigned int iall, char *imagename)
{
    char str[200];
    int dozen = iall / 9;
    int unit = iall % 9;

    snprintf(str, sizeof(str), "%s/%d%d.bmp", imagename, dozen, unit);
    saveImage(image, str);
}

void split(Image *image, SDL_Surface *seg81[], int save, char *imagename)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    printf("width: %d, height: %d\n", width, width);
    unsigned int xstart = 0;
    unsigned int ystart = 0;
    Coordinates coorarray[9];
    unsigned int i = 0;

    char directory[200];
    snprintf(directory, sizeof(directory), "mkdir %s", imagename);
    if (!system(directory))
    {
        char delete[200];
        snprintf(delete, sizeof(delete), "rm -rf %s", imagename);
        system(delete);
        system(directory);
    }

    for (unsigned int y = 0; y < height && i < 9; y++)
    {
        if (!isBlackLine(image, y))
        {
            for (unsigned int x = 0; x < width && i < 9; x++)
            {
                if (i != 0)
                {
                    y = ystart;
                }
                if (image->pixels[x][y].r == 255)
                {
                    xstart = x;
                    ystart = y;
                    for (; x < width && image->pixels[x][y].r == 255; x++)
                        ;
                    for (; y < height && image->pixels[xstart][y].r == 255; y++)
                        ;
                    Coordinates coord = {
                        .xstart = xstart, .ystart = ystart, .xend = x, .yend = y
                    };
                    coorarray[i] = coord;
                    updateSurface(image);
                    i++;
                }
            }
        }
    }

    SDL_Rect block;
    unsigned int yinit = coorarray[0].ystart;
    const unsigned int ylen = coorarray[0].yend - coorarray[0].ystart;
    i = 0;
    unsigned int iall = 0;
    for (unsigned int y = coorarray[0].ystart; y < height && i < 9;)
    {
        for (unsigned int x = 0; x < 9; x++, iall++)
        {
            displayblock(image, coorarray[x].xstart, y, coorarray[x].xend,
                         coorarray[x].yend + y - yinit);

            block.x = coorarray[x].xstart;
            block.y = y;
            block.w = coorarray[x].xend - coorarray[x].xstart;
            // printf("w= %d\n",block.w);
            block.h = coorarray[x].yend - coorarray[x].ystart;

            SDL_Surface *surface =
                SDL_CreateRGBSurface(0, block.w, block.h, 24, 0, 0, 0, 0);

            SDL_BlitSurface(image->surface, &block, surface, NULL);

            Image imagebis;
            imagebis.surface = surface;
            imagebis.width = block.w;
            imagebis.height = block.h;

            printf("widthnew: %d, heightnew: %d\n", imagebis.surface->w,
                   imagebis.surface->h);
            newImage(&imagebis);
            

            resize(&imagebis, 28, 28);
            updateSurface(&imagebis);

            seg81[iall] = imagebis.surface;

            if (save)
            {
                savesquare(&imagebis, iall, imagename);
            }

            freeImage(&imagebis);
        }
        i++;
        y += ylen;
        for (; i < 9 && y < height
             && image->pixels[coorarray[0].xstart][y].r == 255;
             y++)
            ;

        for (; i < 9 && y < height
             && image->pixels[coorarray[0].xstart][y].r == 0;
             y++)
            ;
    }
}

void freeList(SDL_Surface *surface[], unsigned int len)
{
    for (unsigned int i = 0; i < len; i++)
    {
        SDL_FreeSurface(surface[i]);
    }
}
