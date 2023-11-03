#ifndef PIXEL_H
#define PIXEL_H

int isBlack(SDL_Surface* surface,int x, int y);

void addPixel(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b);

#endif
