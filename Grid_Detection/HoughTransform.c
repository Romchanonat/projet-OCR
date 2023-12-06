#include <SDL2/SDL.h>
#include <math.h>
#include <err.h>
#include <stdio.h>

#include "Pixel.h"
#include "HoughTransform.h"

#define THRESHOLD 200

void freeMat(long** mat)
{
	for(int i = 0; i < 180; ++i) 
    {
        free(mat[i]);
    }
    free(mat);
}


long** Matrix_Creation(SDL_Surface* surface,int rho)
{
    int w = surface->w;
    int h = surface->h;

    int theta = 180;
    
    long** mat = calloc(theta,sizeof(long*));
    for (int i = 0;i<theta;i++)
    	mat[i]=calloc(rho,sizeof(long));
    if(mat==NULL)
        errx(-1,"Allocate error for mat");
        
    Uint32* pixels = surface->pixels;
    SDL_Color color;

    for (int y=0; y<h; y++)
    {
        for (int x = 0; x<w; x++)
        {
			Uint32 p = pixels[y*w+x];
			SDL_GetRGB(p, surface->format, &color.r, &color.g, &color.b);
			if(color.r==0 && color.g==0 && color.b==0)
				continue;
			
			for(int i_theta = 0; i_theta<180;i_theta++)
			{
				long i_rho = x*cos(i_theta)+y*sin(i_theta);
				if(i_rho>=0 && i_rho<rho)
					mat[i_theta][i_rho]++;
			}
        }
    }
    return mat;
}

long Clean(long** mat, int rho)
{
	long nb_line = 0;
	for (int i = 0; i<180; i++)
	{
		for (int j = 0; j<rho; j++)
		{
			if (mat[i][j]<THRESHOLD)
				mat[i][j]=0;
			else
				nb_line++;
		}
	}
	return nb_line;
}

tuple_RhoTheta* RecupLineList(long** mat, int rho, long nb_line)
{
	int index = 0;
	
	tuple_RhoTheta* list = malloc(sizeof(tuple_RhoTheta)*nb_line);
	if (list==NULL)
		errx(-1,"Allocate error for list");
		
	for(int i_theta = 0; i_theta<180;i_theta++)
	{
		for (int j_rho = 0; j_rho<rho;j_rho++)
		{
			if(mat[i_theta][j_rho] ==0)
				continue;
			list[index].rho = j_rho;
			list[index].theta = i_theta;
			index++;
		}
	}
	return list;
}

int Similar(double a,double b)
{
	return a>=b-100 && a<=b+100;
}

int* genMat(long nb_line)
{
	int* mat = calloc(nb_line,sizeof(int));
	if(mat==NULL)
		errx(EXIT_FAILURE,"Allocate error for mat");
		
	return mat;
}

tuple_RhoTheta* Defatter(tuple_RhoTheta* list, long* nb_line)
{
	long nb_line_2 = *nb_line;
	int index=0;
	
	tuple_RhoTheta* list_2 = malloc(sizeof(tuple_RhoTheta)*(*nb_line));
	if (list==NULL)
		errx(-1,"Allocate error for list");
		
	int* mat= genMat(*nb_line);
	
	for (int i = 0;i<*nb_line;i++)
	{
		if(mat[i])
			continue;
			
		double x0= list[i].rho*cos(list[i].theta);
		double y0 = list[i].rho*sin(list[i].theta);
		
		list_2[index].rho=list[i].rho;
		list_2[index].theta=list[i].theta;
		
		if(x0<10 && y0<10)
        {
        	nb_line_2--;
        	continue;
        }
		
		for(int j=i+1;j<*nb_line;j++)
		{
			if(mat[j])
				continue;
			double x1= list[j].rho*cos(list[j].theta);
			double y1 = list[j].rho*sin(list[j].theta);
			
			if(Similar(x0,x1) && Similar(y0,y1))
			{
				nb_line_2--;
				mat[j]=1;
			}
		}
		index++;
	}
	
	free(mat);
	free(list);
	
	list_2 = realloc(list_2,nb_line_2*sizeof(tuple_RhoTheta));
	*nb_line = nb_line_2;
	
	return list_2;
}

void ReDrawGrid(SDL_Surface* surface,tuple_RhoTheta* list, long nb_line)
{
	int h = surface->h;
	int w = surface->w;
	Uint32* pixels = surface->pixels;
	Uint32 p= SDL_MapRGB(surface->format, 255, 0, 255);
	for (int i = 0;i<nb_line;i++)
	{
		double rho = list[i].rho;
		double theta = list[i].theta;
        	
		for(int x=0;x<w;x++)
		{
		    int y = (rho - x*cos(theta))/sin(theta);
		    
		    if(y>=0 && y<h)
				pixels[y*w+x] = p;
        }
     	for(int y=0;y<h;y++)
		{
			int x=(rho - y*sin(theta))/cos(theta);
			
		    if(x>=0 && x<w)
				pixels[y*w+x] = p;
        }
	}	
}


void HoughTransform(SDL_Surface* surface)
{
	int w = surface->w;
	int h = surface->h;
	
	int rho = sqrt(w*w+h*h);
	
	//create the parameter space matrix;
	long** mat = Matrix_Creation(surface,rho);
    printf("Pass Matrix_Creation\n");
    
    //clean and keep the higher value in the parameter space
    long nb_line=Clean(mat, rho);
    printf("Pass Clean : nb_line = %ld \n",nb_line);
    
    //create a list of tuple(Rho/Theta) for the lines to draw
    tuple_RhoTheta* lines = RecupLineList(mat,rho,nb_line);
    freeMat(mat);
    
    printf("Pass RecupLineList\n");
    
    lines= Defatter(lines, &nb_line);
    printf("Pass Defatter : nb_line = %ld\n",nb_line);
    
    //redraw the grid with the new lines' value in the tuple(Rho/Theta)
    ReDrawGrid(surface,lines,nb_line);
    
    /*#########################
    	Here to recup "lines" before free it, to use it
    	
    	"lines" is a list of Tuple Rho/Theta
    	
    	the struct "tuple_RhoTheta" is HoughTransform.h if need to see how it work
       ##############################*/
    free(lines);
    printf("Pass Grid redraw\n");
    
    //save the image with the new grid
    SDL_SaveBMP(surface, "HoughTransform.bmp");
}
     
      
	


			
