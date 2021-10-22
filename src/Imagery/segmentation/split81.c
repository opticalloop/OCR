#include"Imagery/segmentation/split81.h"

int isBlackLine(Image image, unsigned int y)
{
    const unsigned int width = image.width;

    for (unsigned int x = 0; x < width; x++)
    {
        if (image.pixels[x][y].r != 0)
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

    // printf("Appel du displayblock à x: %d et y: %d\n",xstart,ystart);
    unsigned int y = ystart;
    unsigned int x = xstart;
    if (xend < width && yend < height)
    {
        for (; ystart <= yend; ystart++)
        {
            // printf("mon ystart:%d et mon xstart:\n",ystart);
            for (unsigned int xstart = x; xstart <= xend; xstart++)
            {
                if (ystart == y || ystart == yend)
                {
                    // printf("Je fais toute la ligne, x=%d
                    // y=%d\n",xstart,ystart);
                    image->pixels[xstart][ystart].r = 255;
                    image->pixels[xstart][ystart].g = 0;
                    image->pixels[xstart][ystart].b = 0;
                }
                else
                {
                    // printf("Je fais juste le début et la fin\n");
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

typedef struct
{
    unsigned int xstart;
    unsigned int ystart;
    unsigned int xend;
    unsigned int yend;
} coordonnes;

void split(Image image, SDL_Surface *seg81[], int save, char *imagename)
{
    const unsigned int width = image.width;
    const unsigned int height = image.height;

    unsigned int xstart = 0;
    unsigned int ystart = 0;
    coordonnes coorarray[9];
    unsigned int i = 0;

    char directory[200];
    snprintf(directory,sizeof(directory), "mkdir %s",imagename);
    printf("%s",directory);
    system(directory);

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
                if (image.pixels[x][y].r == 255)
                {
                    xstart = x;
                    ystart = y;
                    for (; x < width && image.pixels[x][y].r == 255; x++)
                        ;
                    for (; y < height && image.pixels[xstart][y].r == 255; y++)
                        ;
                    coordonnes coord = {.xstart = xstart,
                        .ystart = ystart,
                        .xend = x,
                        .yend = y};
                    coorarray[i] = coord;
                    updateSurface(&image);
                    // SDL_BlitSurface(image.surface, &block,seg81[i], NULL);
                    i++;
                }
            }
        }
    }
    SDL_Rect block;
    unsigned int yinit = coorarray[0].ystart;
    const unsigned int ytaille = coorarray[0].yend - coorarray[0].ystart;
    i = 0;
    unsigned int iall = 0;
    for (unsigned int y = coorarray[0].ystart; y < height && i < 9;)
    {
        for (unsigned int x = 0; x < 9; x++,iall++)
        {
            //printf("xstart:%d, ystart:%d, xend:%d, yend:%d\n",
            //    coorarray[x].xstart, coorarray[x].ystart, coorarray[x].xend,
            //    coorarray[x].yend);

            displayblock(&image, coorarray[x].xstart, y, coorarray[x].xend,
                coorarray[x].yend + y - yinit);

            block.x = coorarray[x].xstart;
            block.y = y;
            block.w = coorarray[x].xend - coorarray[x].xstart;
            printf("w= %d\n",block.w);
            block.h = coorarray[x].yend - coorarray[x].ystart;
            
            SDL_Surface *surface = 
                SDL_CreateRGBSurface(0,block.w,block.h,24,0,0,0,0);
            //printf("iall=%d\n",iall);
            SDL_BlitSurface(image.surface, &block, surface, NULL);
            SDL_Surface *test = display_image(surface);
            SDL_FreeSurface(test);
            seg81[iall] = surface;
            if(save){
                char str[200];
                int dizaine = iall/9;
                int unite = iall % 9;

                snprintf(str,sizeof(str),"%s/%d%d.bmp",imagename,dizaine,unite);

                printf("%s\n",str);
                if(SDL_SaveBMP(surface,str) != 0){
                    printf("SDL_SaveBMP failed: %s\n",SDL_GetError());
                }
            }
        }
        i++;
        y += ytaille;
        for (; i < 9 && y < height
               && image.pixels[coorarray[0].xstart][y].r == 255;y++);

        for (; i < 9 && y < height
               && image.pixels[coorarray[0].xstart][y].r == 0;y++);
    }
}

void freeList(SDL_Surface *surface[], unsigned int len){
    for(unsigned int i = 0; i < len; i++){
        SDL_FreeSurface(surface[i]);
    }
}
