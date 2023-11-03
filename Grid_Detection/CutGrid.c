#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

#include "Pixel.h"

//save the Piece with is name and free the surface
void SaveImage(SDL_Surface* surface, char* name)
{
	char str1[100] = "Pieces/";
	char str2[5] = ".bmp";
	strcat(str1, name);
	strcat(str1,str2);
	SDL_SaveBMP(surface, str1);
	SDL_FreeSurface(surface);
	free(name);
}

//create the name of the Piece : [ij].bmp
char* getName(int i, int j)
{
	char* name = malloc(sizeof(char)*3);
	int num = i*10+j;
	sprintf(name, "%d", num);
	return name;
}

//cut the grid in 9x9 Piece 
void CutGrid(SDL_Surface* surface)
{
	int w = surface-> w;
	int h = surface-> h;
	
	int limit_w = w/9;
	int limit_h = h/9;
	
	Uint32* pixels = surface->pixels;
	
	for (int i = 0;i<9;i++)
	{
		for (int j = 0; j<9;j++)
		{
			SDL_Surface* n_surface = 
				SDL_CreateRGBSurface (0, limit_w, limit_h, 32, 0, 0, 0, 0);
			if(n_surface==NULL)
				errx(EXIT_FAILURE, "Unable to create a new surface");
				
			Uint32* n_pixels = n_surface->pixels;
			 
			for (int x=0;x<limit_w;x++)
			{
				int Nx = limit_w*i + x;
				
				for(int y = 0; y<limit_h;y++)
				{
					int Ny = limit_h*j + y;
					
					n_pixels[x*limit_w+y] = pixels[Nx*w+Ny];
				}
			}
			SaveImage(n_surface, getName(i,j));
		}
	}
}
