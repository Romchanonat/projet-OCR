#ifndef HELP_H
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

#endif
