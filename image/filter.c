//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_rotozoom.h>
//#include <stdio.h>

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <math.h>
#include "constantes.h"
#include "filter.h"

SDL_Surface* inv_img(SDL_Surface *Image){
	SDL_Surface *inv_Img=NULL;
	int i,j;
	inv_Img = SDL_CreateRGBSurface(SDL_HWSURFACE, Image->w, Image->h, 32, 0, 0, 0, 0);

	for(i=0;i<Image->w;i++){
		for(j=0;j<Image->h;j++){
			putpixel(inv_Img, i, j, SDL_MapRGB(inv_Img->format, 255, 255, 255) - getpixel(Image, i, j));
		}
	}
	return inv_Img;
}


SDL_Surface* ng_img(SDL_Surface *Image){
	SDL_Surface *ng_Img=NULL;
	SDL_Color c;
	int i,j,gris;
	ng_Img=SDL_CreateRGBSurface(SDL_HWSURFACE, Image->w, Image->h, 32, 0, 0, 0, 0);

	for(i=0;i<Image->w;i++){
		for(j=0;j<Image->h;j++){
			SDL_GetRGB(getpixel(Image,i,j), Image->format, &c.r, &c.g, &c.b); // Récupère la couleur du pixel et la met dans la structure SDL_Color
			//gris=(c.r+c.g+c.b)/3; // Gris approximé grossièrement
			gris=0.2125*c.r+0.7154*c.g+0.0721*c.b; // Gris naturel proche de l'oeil humain
			putpixel(ng_Img, i, j, SDL_MapRGB(Image->format, gris, gris, gris));
		}
	}

	#ifdef INFO
	printf("Rouge:%d Vert:%d Bleu:%d Gris:%d\n", c.r, c.g, c.b, gris);
	#endif

	return ng_Img;
}

SDL_Surface* bin_img(SDL_Surface *Image,int seuil, Uint32 couleur_down, Uint32 couleur_up){
	SDL_Surface *bin_Img=NULL;
	int i,j;
	SDL_Rect pos0;
	pos0.x=0;
	pos0.y=0;
	bin_Img=SDL_CreateRGBSurface(SDL_HWSURFACE, Image->w, Image->h, 32, 0, 0, 0, 0);
	SDL_BlitSurface(Image, NULL,bin_Img,&pos0);
	bin_Img=ng_img(bin_Img); // Récupère l'image en niveau de gris pour seuillage

	for(i=0;i<Image->w;i++){
		for(j=0;j<Image->h;j++){
			if(getpixel(bin_Img,i,j)<SDL_MapRGB(Image->format,seuil,seuil,seuil))
					putpixel(bin_Img,i,j,couleur_down);
			else
					putpixel(bin_Img,i,j,couleur_up);
		}
	}
	return bin_Img;
}

SDL_Surface* quant_img(SDL_Surface *Image,int seuil,int nb_bits,Uint32 **couleurs){ // problème format couleur (lenna != cb)
	SDL_Surface *quant_Img=NULL;
	int i,j,k,nb_couleurs=1;
	for(i=0;i<nb_bits;i++){
		nb_couleurs*= 2;
	}

	#ifdef INFO
	for(i=0;i<nb_couleurs;i++){
		printf("\n====== Couleurs fonction %d: %8ls",i,couleurs[i]);
	}
	printf("Il y a %d couleurs.\n",nb_couleurs);
	#endif

	quant_Img=SDL_CreateRGBSurface(SDL_HWSURFACE, Image->w, Image->h, 32, 0, 0, 0, 0);
	quant_Img=ng_img(Image); // Récupère l'image en niveau de gris pour seuillage
	for(i=0;i<Image->w;i++){
		for(j=0;j<Image->h;j++){
			for(k=1;k<nb_couleurs+1;k++){
				if(getpixel(quant_Img,i,j)<SDL_MapRGB(quant_Img->format,k*255/nb_couleurs,k*255/nb_couleurs,k*255/nb_couleurs) &&
					getpixel(quant_Img,i,j)>=SDL_MapRGB(quant_Img->format,(k-1)*255/nb_couleurs,(k-1)*255/nb_couleurs,(k-1)*255/nb_couleurs))
				{
					putpixel(quant_Img,i,j,*couleurs[k-1]);
					break;
				}
			}
		}
	}
	return quant_Img;
}

SDL_Surface* lux_img(SDL_Surface *Image, int seuil){
	SDL_Surface *lux_Img=NULL;
	int i,j;
	double n=(double)seuil/128; // n = [ 0 ; 2 ]

	#ifdef INFO
	printf("SEUIL LUX: %d N: %f\n",seuil,n);
	#endif

	SDL_Color color;
	lux_Img=SDL_CreateRGBSurface(SDL_HWSURFACE, Image->w, Image->h, 32, 0, 0, 0, 0);
	for(i=0;i<Image->w; i++){
		for(j=0;j<Image->h;j++){
// Méthode 1
			SDL_GetRGB(getpixel(Image,i,j), Image->format, &color.r, &color.g, &color.b);
			//SDL_GetRGB(Image->pixels+j * Image->w + i, Image->format, &color.r, &color.g, &color.b);
			color.r = f_lux(color.r, n);
			color.g = f_lux(color.g, n);
			color.b = f_lux(color.b, n);
			putpixel(lux_Img,i,j, SDL_MapRGB(Image->format, color.r, color.g, color.b));
// Méthode 2

		}
	}
	return lux_Img;
}

Uint8 f_lux(Uint8 c, double n){
	return (Uint8) (255 * pow((double) c / 255, n));
}

SDL_Surface* detourage(SDL_Surface *Image){
	SDL_Surface *det_Img=NULL;
	SDL_Rect pos0;
	pos0.x=0;
	pos0.y=0;
	int i,j,TAILLE=3;

	det_Img=SDL_CreateRGBSurface(SDL_HWSURFACE, Image->w, Image->h, 32, 0, 0, 0, 0);
	SDL_BlitSurface(Image,NULL,det_Img,&pos0); // Copie l'image de base pour ne pas la détériorer
	Uint32 *p = det_Img->pixels;


	int w = Image->w, h = Image->h;
	for(i=0;i<h; i++){
		for(j=0;j<w;j++){
			p[i*w + j] = 255 - SDL_abs(p[i*w +j] - calc_moy(det_Img,i,j,TAILLE));
		}
	}
	return det_Img;
}

SDL_Surface* filtre_moyenneur(SDL_Surface *Image){
	SDL_Surface *moy_Img=NULL;
	int i,j,TAILLE=3;
	SDL_Rect pos0;
	pos0.x=0;
	pos0.y=0;
//    struct Filter filtre;
//    char filtre_tab[3][3];
//    for(i=0;i<3;i++){
//        for(j=0;j<3;j++){
//            filtre_tab[i][j]=1;
//        }
//    }
//    filtre.colomn=TAILLE;
//    filtre.row=TAILLE;
//    filtre.Tab = malloc(sizeof(unsigned char)*(2+TAILLE*TAILLE));
//    filtre.Tab = filtre_tab;
//    moy_Img=imfilter(Image,&filtre);

	moy_Img=SDL_CreateRGBSurface(SDL_HWSURFACE, Image->w, Image->h, 32, 0, 0, 0, 0);

	SDL_BlitSurface(Image,NULL,moy_Img,&pos0); // Copie l'image de base pour ne pas la détériorer
	Uint32 *p = moy_Img->pixels;

	int w = Image->w, h = Image->h;
	for(i=0;i<h; i++){
		for(j=0;j<w;j++){
			putpixel(moy_Img,j,i, calc_moy(moy_Img,i,j,TAILLE));
			//p[i*w + j] = calc_moy(moy_Img,i,j,TAILLE);
			#ifdef DEBUG
				if(i == h/2 && j == w/2)
				//SDL_SaveBMP(moy_Img,"mid.bmp");
			#endif
		}
	}
	return moy_Img;
}

static Uint32 calc_moy(SDL_Surface *surface, int i, int j, int n) // https://zestedesavoir.com/tutoriels/1014/utiliser-la-sdl-en-langage-c/tp-effets-sur-des-images/#1-noirs-et-negatifs
{
	const int initial_h = SDL_max(i - n, 0);
	const int initial_w = SDL_max(j - n, 0);
	const int final_h = SDL_min(i + n, surface->h - 1);
	const int final_w = SDL_min(j + n, surface->w - 1);
	int nb_pixel = ((final_h - initial_h) * (final_w - initial_w));
	const Uint32 *p = surface->pixels;
	Uint32 sum_r = 0, sum_g = 0, sum_b = 0;
	SDL_Color color;

	for(i=initial_h;i<final_h;i++)
		for(j=initial_w;j<final_w;j++){
			SDL_GetRGB(p[i * surface->w + j], surface->format, &color.r, &color.g, &color.b);
			sum_r += color.r;
			sum_g += color.g;
			sum_b += color.b;
		}

	#ifdef INFO
	if(nb_pixel==0){
		printf("\ni:%d\tj:%d\nnb_pixel=%d",i,j,nb_pixel);
		//nb_pixel=1;
	}
	#endif

	return SDL_MapRGB(surface->format, sum_r / nb_pixel, sum_g / nb_pixel, sum_b / nb_pixel);
}

SDL_Surface* imfilter(SDL_Surface *Image,struct Filter* filtre){
	int i,j;
	SDL_Surface *imgFiltree=NULL;
	SDL_Color color;
	imgFiltree=SDL_CreateRGBSurface(SDL_HWSURFACE, Image->w, Image->h, 32, 0, 0, 0, 0);
	for(i=0;i<filtre->row;i++){
		for(i=0;i<filtre->colomn;j++){
			//Appliquer le filtre à l'image = multiplier le noyau à chaque pixel (avec un masque de la taille du filtre)
			//SDL_GetRGB(getpixel(Image,i,j), Image->format, &color.r, &color.g, &color.b);
			//putpixel(imgFiltree,i,j, SDL_MapRGB(Image->format, color.r*filtre.Tab[i][j], color.g*filtre.Tab[i][j], color.b*filtre.Tab[i][j]));
		}
	}
}

Uint32 getpixel(SDL_Surface *surface, int x, int y){
	int bpp = surface->format->BytesPerPixel; // Octet(s) par pixel
	// *p = l'adresse du début des données de pixels + y * le nombre d'octet(s) sur une ligne de pixel + x * le nombre d'octet(s) par pixel
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			return *p;
			break;
		case 2:
			return *(Uint16 *)p;
			break;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else // Little Endian
				return p[0] | p[1] << 8 | p[2] << 16;
			break;
		case 4:
			return *(Uint32 *)p;
			break;
		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			*p = pixel;
			break;
		case 2:
			*(Uint16 *)p = pixel;
			break;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
					p[0] = (pixel >> 16) & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = pixel & 0xff;
			} else {
					p[0] = pixel & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = (pixel >> 16) & 0xff;
			}
			break;
		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}
