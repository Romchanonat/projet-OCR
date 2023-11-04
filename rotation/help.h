/*#ifndef HELP_H
#define HELP_H

typedef struct{
    Uint8 r, g, b;
} Pixel;


typedef struct{
    int height;
    int width;
    SDL_Surface *surface;
    Pixel **pixels;
    double average_color;
}Image;

Image *create(int height, int width);

#endif*/

// help.h
#ifndef HELP_H
#define HELP_H

#include "SDL2/SDL.h"

typedef struct
{
    Uint8 r, g, b;
} Pixel;

typedef struct
{
    int width;
    int height;
    SDL_Surface *surface;
    Pixel **pixels;
} Image;

Image *create(int height, int width);

#endif // HELP_H

