#include <math.h>
#include <err.h>
#include <float.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "help.h"
#include <limits.h>


Image copy_image(Image* base)
{
    Image result;

    result.height = base->height;
    result.width = base->width;
    result.average_color = base->average_color;
    
    result.surface = SDL_ConvertSurface(base->surface, base->surface->format,
            SDL_SWSURFACE);
    
    result.pixels = malloc((result.width + 1) * sizeof(Pixel *));

    if (result.pixels == NULL)
        errx(1, "Error when allocating memory in clone_image.");

    for (int x = 0; x < result.width; x++)
    {    
        result.pixels[x] = malloc((result.height + 1) * sizeof(Pixel));

        for (int y = 0; y < result.height; y++)
        {
            Pixel pBase = base->pixels[x][y];
            Pixel Final = {pBase.r, pBase.g, pBase.b};
            result.pixels[x][y] = Final;
        }
    }

    return result;
}


Image rotation(Image* image, double degrees)
{
    Image result = copy_image(image);

    double radian = degrees * (3.14159265359 / 180);

    int width = result.width;
    int height = result.height;

    double center_w = width/2;
    double center_h = height/2;

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            double ri;
            double rj;

            ri = (i - center_w) * cos(radian) - (j - center_h) * sin(radian) +
                center_w;
            rj = (i - center_w) * sin(radian) + (j - center_h) * cos(radian) +
                center_h;

            if (0 <= ri && ri < width && 0 <= rj && rj < height)
            {
                Pixel base = image->pixels[(int)ri][(int)rj];
                Pixel Final = {base.r, base.g, base.b};
                result.pixels[i][j] = Final;
            }
            else
            {
                Pixel black = {0,0,0};
                result.pixels[i][j] = black;
            }


        }
    }

    return result;
}


int main(int argc, char** argv)
{
   if (argc != 2)
     errx(EXIT_FAILURE, "Usage: image-file");

   SDL_Surface* surface;

   // Telechargement d'une image
   surface = IMG_Load(argv[1]);
   if (!surface)
       errx(3, "can't load %s: %s", argv[1], IMG_GetError());

   // Création d'une image

   int height = surface->h;
   int width = surface->w;
   Image *tempo = malloc(sizeof(Image));
   
   tempo->height = height;
   tempo->width = width;
   tempo->pixels = malloc(sizeof(Pixel *) * width + 1);
   
   if (tempo->pixels == NULL)
      errx(1, "Error when allocating memory in create_image.");
   
   for (int x = 0; x < width; x++)
     tempo->pixels[x] = malloc(sizeof(Pixel) * height + 1);
  
   tempo->surface = surface;
   Image i = rotation(tempo, (double) 90);

   // Sauvegarde d'une image
   int success = IMG_SavePNG(i.surface, argv[1]);
   if (success != 0)
       errx(1, "could not save the image to '%s': %s.\n", argv[1], SDL_GetError());

   // Libération des ressources
    SDL_FreeSurface(surface);
    IMG_Quit();
    SDL_Quit();


   return 0;
}
