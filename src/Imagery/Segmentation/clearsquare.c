#include "Imagery/Segmentation/clearsquare.h"

int isLinePartialBlack(Image *image, unsigned int y)
{
    blackandwhite(image);
    unsigned int nbblack = 0;

    for (unsigned int x = 0; x < 28; x++)
    {
        if (image->pixels[x][y].r == 0)
            nbblack++;
    }

    if (nbblack == 0)
        return 1;
    return nbblack > 20 ? 1 : 0;
}

int isColumnPartialBlack(Image *image, unsigned int x)
{
    blackandwhite(image);
    unsigned int nbblack = 0;
    // displayImage(image);
    for (unsigned int y = 0; y < 28; y++)
    {
        if (image->pixels[x][y].r < 128)
            nbblack++;
    }

    if (nbblack == 0)
        return 1;
    return nbblack > 20 ? 1 : 0;
}

void clearLine(Image *image, unsigned int y)
{
    const unsigned int width = image->width;

    for (unsigned int x = 0; x < width; x++)
    {
        updatePixelToSameValue(&(image->pixels[x][y]), 255);
    }
}

void clearColumn(Image *image, unsigned int x)
{
    const unsigned int height = image->height;

    for (unsigned int y = 0; y < height; y++)
    {
        updatePixelToSameValue(&(image->pixels[x][y]), 255);
    }
}

void clearsquare(Image *image)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    int cor[56];

    for (unsigned int i = 0; i < 56; i++)
    {
        cor[i] = -1;
    }

    unsigned int iall = 0;
    for (unsigned int x = 0; x < width; x++, iall++)
    {
        if (isColumnPartialBlack(image, x))
        {
            cor[iall] = x;
        }
    }
    for (unsigned int y = 0; y < height; y++, iall++)
    {
        if (isLinePartialBlack(image, y))
        {
            cor[iall] = y;
        }
    }

    iall = 0;

    for (unsigned int i = 0; i < 56; i++, iall++)
    {
        if (cor[iall] != -1 && i < 28)
        {
            clearColumn(image, cor[iall]);
        }

        else if (cor[iall] != -1 && i >= 28)
        {
            clearLine(image, cor[iall]);
        }
    }
}

unsigned int __dfs(M,x,y,cur){
    M[x][y] =  1;
    cur[x][y] = 1;
    unsigned int lengraph = 0;
    
    if(x-1>=0 && y-1>=0 && M[x-1][y-1]==0)
        lengraph = 1 + __dfs(M,x-1,y-1);
    if(x>=0 && y-1>=0 && M[x][y-1]==0)
        lengraph = 1 + __dfs(M,x,y-1);
    if(x+1>28 && y-1>=0 && M[x+1][y-1]==0)
        lengraph = 1 + __dfs(M,x+1,y-1);
    if(x-1>=0 && y>=0 && M[x-1][y]==0)
        lengraph = 1 + __dfs(M,x-1,y);
    if(x+1>28 && y>=0 && M[x+1][y]==0)
        lengraph = 1 + __dfs(M,x+1,y);
    if(x-1>=0 && y+1>=0 && M[x-1][y+1]==0)
        lengraph = 1 + __dfs(M,x-1,y+1);
    if(x>=0 && y+1>=0 && M[x][y+1]==0)
        lengraph = 1 + __dfs(M,x,y+1);
    if(x+1>28 && y+1>=0 && M[x+1][y+1]==0)
        lengraph = 1 + __dfs(M,x+1,y+1);

    return lengraph
}

void clear_matrice(int[][] M, Image *image){
    for (int i = 0; i < 28; i++)
    {
        for (int j = 0; j < 28; j++)
        {
            if(M[i][j]==1){
                image[x][y].r = 255;
                image[x][y].g = 255;
                image[x][y].b = 255;
            }

void clear_imperfections(Image *image)
{
    int M[28][28];
    unsigned int maxlen = 0;
    for(unsigned int x = 0; x < 28; x++)
    {
        for(unsigned int y = 0; y < 28; y++)
        {
            M[x][y] = 0;
        }
    }

    // the matrix who will keep the current graph connexe 
    int cur[28][28];
    //result matrix
    int res[28][28];

    for(unsigned int x = 0; x < 28; x++)
    {
        for(unsigned int y = 0; y < 28; y++)
        {
            //initialise the matrix
            for(unsigned int i = 0; i < 28; i++)
            {
                for(unsigned int j = 0; j < 28; j++)
                {
                    cur[i][j] = 0;
                }
            }

            if(M[x][y] == 0)
            {
                unsigned int len = __dfs(M,x,y,res);
                if ( len > maxlen){
                    maxlen = len;
                    //copy the current graph in the result matrix
                    for(unsigned int i = 0; i < 28; i++)
                    {
                        for(unsigned int j = 0; j < 28; j++)
                        {
                            res[i][j] = cur[i][j];
                        }
                    }
                }
            }
        }
    }
    clear_matrice(res,image);
}

