#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

SDL_Surface* inv_img(SDL_Surface *Image); // Inversion des couleurs de l'image
SDL_Surface* ng_img(SDL_Surface *Image); // Conversion de l'image en niveau de gris
// bin_img binarise une image selon un seuil en 2 couleurs
SDL_Surface* bin_img(SDL_Surface *Image,int seuil, Uint32 couleur1, Uint32 couleur2);
SDL_Surface* quant_img(SDL_Surface *Image,int seuil,int nb_bits,Uint32 **couleurs);
SDL_Surface* lux_img(SDL_Surface *Image, int seuil);
Uint8 f_lux(Uint8 c, double n);
SDL_Surface* detourage(SDL_Surface *Image);
SDL_Surface* filtre_moyenneur(SDL_Surface *Image);
static Uint32 calc_moy(SDL_Surface *Image, int i, int j, int n);
SDL_Surface* imfilter(SDL_Surface *Image,struct Filter* filtre);
Uint32 getpixel(SDL_Surface *surface, int x, int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

#endif // FILTER_H_INCLUDED
