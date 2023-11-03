#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>
#include <stdio.h>

#include "HoughTransform.h"
#include "Pixel.h"
#include "CutGrid.h"

int main (int argc, char** argv)
{
	if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");
        
    SDL_Surface* image = IMG_Load(argv[1]);
    if (image==NULL)
    	errx(EXIT_FAILURE, "Image not load");
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(image,SDL_PIXELFORMAT_RGB888,0);
    SDL_FreeSurface(image);
    
    //do the HoughTransform of the grid
    HoughTransform(surface);
    
    //cut the grid in 9x9 Piece
    CutGrid(surface);
    
	SDL_FreeSurface(surface);
    
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
