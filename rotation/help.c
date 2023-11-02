#include <err.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "SDL2/SDL.h"

Image *create(int height, int width)
{
    Image *image = malloc(sizeof(Image));

    image->height = height;
    image->width = width;
    image->pixels = malloc(sizeof(Pixel *) * width + 1);

    if (image->pixels == NULL)
        errx(1, "Error when allocating memory in create_image.");

    for (int x = 0; x < width; x++)
        image->pixels[x] = malloc(sizeof(Pixel) * height + 1);

    image->surface = SDL_CreateRGBSurfaceWithFormat(
        0, width, height, 32, SDL_PIXELFORMAT_RGBA32);

    if (image->surface == NULL)
    {
        const char *errm = SDL_GetError();
        errx(-1, errm);
    }

    return image;
}
