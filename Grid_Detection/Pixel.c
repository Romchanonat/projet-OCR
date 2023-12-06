#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


//Return the wanted pixel in the surface
Uint32 getPixel(SDL_Surface *surface, int x, int y) {
    //int bpp = surface->format->BytesPerPixel;
    //Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    Uint32* pixels = surface->pixels;
    return pixels[y*surface->w+x];
    
    //return  *(uint32_t *)p;
}

//Return the color of the wanted Pixel
SDL_Color getColor(SDL_Surface *surface, int x, int y) {
    SDL_Color color;
    Uint32 pixel = getPixel(surface, x, y);
    SDL_GetRGB(pixel, surface->format, &color.r, &color.g, &color.b);
    return color;
}

//Verifie if the Pixel is black (return 1 if true)
int isBlack(SDL_Surface* surface,int x, int y)
{
	SDL_Color color = getColor(surface,x,y);
	
	return (color.r==0 && color.g==0 && color.b==0);
}

//Put a pixel with the wanted color on the surface
void addPixel(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
	int w= surface->w;
	int h = surface->h;
	if (x<0 || x>=w || y<0 || y>=h)
		return;
    	
    Uint32* pixels = surface->pixels;
    Uint32 color = SDL_MapRGB(surface->format, r, g, b);
    pixels[y*w+x] = color;   
}

void drawLine(SDL_Surface* surface, int x0, int y0, int x1, int y1,Uint32 p)
{
    int w = surface->w;
    int h = surface->h;
    double x = x1 - x0;
    double y = y1 - y0;
    double len = sqrt( x*x + y*y );
    double addx = x / len;
    double addy = y / len;
	x = x0;
    y = y0;
    Uint32* pixels = surface->pixels;
    //pixels[y*w+x] = p;
    //Uint32 p= SDL_MapRGB(surface->format, 255, 0, 255);
    for(int i=0;i<len;i++)
    {
    	if(x>=0 && x<w && y>=0 && y<h)
    		pixels[(int)y*w+(int)x] = p;
        x += addx;
        y += addy;
    }
}
