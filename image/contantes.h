#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

//#define DEBUG  // Uncomment to debug
#define INFO  // Uncomment to print info

// Couleurs
#define ROUGE 	SDL_MapRGB(Image->format,255,0,0)
#define VERT  	SDL_MapRGB(Image->format,0,255,0)
#define BLEU  	SDL_MapRGB(Image->format,0,0,255)
#define NOIR  	SDL_MapRGB(Image->format,0,0,0)
#define BLANC 	SDL_MapRGB(Image->format,255,255,255)
#define JAUNE 	SDL_MapRGB(Image->format,255,255,0)
#define CYAN  	SDL_MapRGB(Image->format,0,255,255)
#define MAGENTA SDL_MapRGB(Image->format,255,0,255)
#define ORANGE 	SDL_MapRGB(Image->format,255,127,0)
/*
Uint32 couleur(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
   return r << 24 | g << 16 | b << 8 | a;
}
*/

#define DELAY_KEYREPEAT 10
#define INTERVAL 				100
#define LARGEUR_SEUIL 	50

// struct verrou{
//   unsigned char binarisation:1;
//   unsigned char quantification:1;
//   unsigned char lux:1;
//   unsigned char chargementOK:1;
//   unsigned char seuil_haschanged:1;
// };

struct verrou{
	unsigned char binarisation;
	unsigned char quantification:1;
	unsigned char lux:1;
	unsigned char chargementOK:1;
	unsigned char seuil_haschanged:1;
};

struct Filter{
	unsigned char row;
	unsigned char colomn;
	unsigned char **Tab;
};

#endif // CONSTANTES_H_INCLUDED
