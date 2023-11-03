#include <SDL2/SDL.h>
#include <math.h>
#include <err.h>
#include <stdio.h>

#include "Pixel.h"
#include "HoughTransform.h"

#define THRESHOLD 200

long* Matrix_Creation(SDL_Surface* surface,int rho)
{
    int w = surface->w;
    int h = surface->h;

    int theta = 180;
    

    long* mat = calloc(theta*rho,sizeof(long*));
    if(mat==NULL)
        errx(-1,"Allocate error for mat");

    for (int x=0; x<w; x++)
    {
        for (int y = 0; y<h; y++)
        {
			//à réfléchir si besoin de test pour autre couleur
			if(isBlack(surface,x,y)==1)
				continue;
			
			for(int i_theta = 0; i_theta<180;i_theta++)
			{
				long i_rho = x*cos(i_theta)+y*sin(i_theta);
				if(i_rho>0 && i_rho<rho)
					mat[i_theta*rho+i_rho]++;			
			}
        }
    }
    return mat;
}


long Clean(long* mat, int rho)
{
	long nb_line = 0;
	for (int i = 0; i<180*rho; i++)
	{
		if (mat[i]<THRESHOLD)
			mat[i]=0;
		else
			nb_line++;
	}
	return nb_line;
}

tuple_RhoTheta* RecupLineList(long* mat, int rho, long nb_line)
{
	int index = 0;
	
	tuple_RhoTheta* list = malloc(sizeof(tuple_RhoTheta)*nb_line);
	if (list==NULL)
		errx(-1,"Allocate error for list");
		
	for(int i_theta = 0; i_theta<180;i_theta++)
	{
		for (int j_rho = 0; j_rho<rho;j_rho++)
		{
			if(mat[i_theta*rho+j_rho] ==0)
				continue;
			list[index].rho = j_rho;
			list[index].theta = i_theta;
			index++;
		}
	}
	return list;
}

void ReDrawGrid(SDL_Surface* surface,tuple_RhoTheta* list, long nb_line)
{
	for (int i = 0;i<nb_line;i++)
	{
		double rho = list[i].rho;
		double theta = list[i].theta;
        
        for(int x=0;x<surface->w;x++)
        {
        	int y = (rho - x*cos(theta))/sin(theta);
        	addPixel(surface,x,y,255,0,255);
        }
	}	
}


void HoughTransform(SDL_Surface* surface)
{
	int w = surface->w;
	int h = surface->h;
	
	int rho = sqrt(w*w+h*h);
	
	//create the parameter space matrix;
	long* mat = Matrix_Creation(surface,rho);
    printf("Pass Matrix_Creation\n");
    
    //clean and keep the higher value in the parameter space
    long nb_line=Clean(mat, rho);
    printf("Pass Clean : nb_line = %ld \n",nb_line);
    
    //create a list of tuple(Rho/Theta) for the lines to draw
    tuple_RhoTheta* lines = RecupLineList(mat,rho,nb_line);
    free(mat);
    printf("Pass RecupLineList\n");
    
    //redraw the grid with the new lines' value in the tuple(Rho/Theta)
    ReDrawGrid(surface,lines,nb_line);
    free(lines);
    printf("Pass Grid redraw\n");
    
    //save the image with the new grid
    SDL_SaveBMP(surface, "HoughTransform.bmp");
}
     
      
	


			
