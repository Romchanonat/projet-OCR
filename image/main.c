#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL/SDL_image.h>
#include "constantes.h"

void pause(SDL_Surface *Image,SDL_Surface *screen);
void init(struct verrou v); // A définir pour alléger la lecture du code

struct verrou v;

int main(int argc, char *argv[]){
	v.chargementOK=0;
	//     freopen("CON", "w", stdout); // Redirection de la sortie (autorisation en écriture) sur console
	//     freopen("CON", "r", stdin);  // Redirection de l'entrée (autorisation en lecture) sur console
	//     freopen("CON", "w", stderr); // Redirection de la sortie d'erreur (autorisation en écriture) sur console
	SDL_Surface *screen=NULL, *Image=NULL;
	SDL_Rect posImage;
	posImage.x=0;
	posImage.y=0;

	char nom_fichier[50];
	SDL_Init(SDL_INIT_VIDEO);

	if(argc > 1){
		if(Image = IMG_Load(argv[1]))
			v.chargementOK=1;
	}

	while(!v.chargementOK){
		printf("Vous voulez chargez quelle image ? (ne pas depasser 50 caracteres)\nNom: ");
		scanf("%s",&nom_fichier);
		Image= IMG_Load(nom_fichier);
		if(Image)
			v.chargementOK=1;
		else
			printf("\nErreur de chargement, verifiez le nom de l'image.\n");
	}

	screen=SDL_SetVideoMode(2*Image->w+LARGEUR_SEUIL, Image->h, 32, SDL_HWSURFACE);
	SDL_WM_SetCaption("Traitement d'image", NULL);

	SDL_BlitSurface(Image, NULL, screen, &posImage);
	pause(Image,screen);

	SDL_EnableKeyRepeat(0,0);
	SDL_FreeSurface(Image);
	SDL_Quit();

	return EXIT_SUCCESS;
}

/* Usage:
-1: Demande de changement de couleur 1 (couleur_down)
-2: Demande de changement de couleur 2 (couleur_up)

-b: binarisation (seuillage paramètrable)
-i: inversion (négatif)
-n: transformation en niveaux de gris (noir et blanc)
-q: quantification (avec nb de bits paramètrable)
*/
void pause(SDL_Surface *Image,SDL_Surface *screen){
	int continuer = 1,seuil=0,nb_bits=3,nb_couleurs=1,i=0;
	SDL_Event event;
	SDL_Surface *Image2=NULL,*S_seuil=NULL,*S_quant[64];
	SDL_Rect posImg2, posSeuil, pos0;
	Uint8 *keystate = NULL;
	Uint32 *couleur_up,*couleur_down;
	Uint32 couleurs[64],couleur; // 64 couleurs => 6 bits

	// Affectations
	S_seuil=SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_SEUIL, Image->h, 32, 0, 0, 0, 0);
	Image2=SDL_CreateRGBSurface(SDL_HWSURFACE, Image->w, Image->h, 32, 0, 0, 0, 0);
	pos0.x = 0;
	pos0.y = 0;
	posImg2.x=Image->w;
	posImg2.y=0;
	posSeuil.x=2*Image->w;
	posSeuil.y=0;

	couleur_up	=BLANC;
	couleur_down=NOIR;

	couleurs[0]=NOIR;
	couleurs[1]=BLEU;
	couleurs[2]=CYAN;
	couleurs[3]=VERT;
	couleurs[4]=JAUNE;
	couleurs[5]=ORANGE;
	couleurs[6]=ROUGE;
	couleurs[7]=BLANC;

	for(i=0;i<nb_bits;i++){
		nb_couleurs*=2;
	}

	printf("%d %d %d",couleurs[3],SDL_MapRGB(screen->format,255,0,0),Image->format);
	v.binarisation=0; v.quantification=0; v.seuil_haschanged=0; v.lux=0;

	SDL_EnableKeyRepeat(DELAY_KEYREPEAT,INTERVAL);

	while(continuer){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_QUIT:
				continuer = 0;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_d:
						Image2=detourage(Image);
						break;
					case SDLK_i:
						Image2=inv_img(Image);
						break;
					case SDLK_l:
						seuil=0;
						if(v.lux)
							v.lux=0;
						else
							v.lux=1;
						Image2=lux_img(Image,seuil);
						break;
					case SDLK_m:
						Image2=filtre_moyenneur(Image);
						break;
					case SDLK_n:
						Image2=ng_img(Image);
						break;
					case SDLK_b: // Reset du seuil de binarisation
						seuil=128;
						if(v.binarisation){
							v.binarisation=0;
							SDL_BlitSurface(Image,NULL,Image2,&pos0);
						}
						else{
							v.binarisation=1;
							Image2=bin_img(Image,seuil,couleur_down,couleur_up);
						}
						break;
					case SDLK_q:
						if(v.quantification)
							v.quantification=0;
						else
							v.quantification=1;
						for(i=0;i<4;i++){
							printf("\n====== Couleurs %d: %2x",i,couleurs[i]);
						}
						Image2=quant_img(Image,seuil,nb_bits,couleurs);
						break;
					case SDLK_s:
						SDL_SaveBMP(Image2,"save.bmp");
						printf("\n\nImage sauvegardee\n\n");
						break;
					case SDLK_DOWN:
						if(v.binarisation){
							if(seuil > 0){
								seuil--;
								v.seuil_haschanged=1;
							} else if(seuil == 0){
								seuil=255;
								v.seuil_haschanged=1;
							}
							Image2=bin_img(Image,seuil,couleur_down,couleur_up);
						}
						else if(v.lux){
							if(seuil > 0){
								seuil--;
								v.seuil_haschanged=1;
							} else if(seuil==0){
								seuil=255;
								v.seuil_haschanged=1;
							}
							Image2=lux_img(Image,seuil);
						}
						break;
					case SDLK_UP:
						if(v.binarisation){
							if(seuil < 255){
								seuil++;
								v.seuil_haschanged=1;
							} else if(seuil==255){
								seuil=0;
								v.seuil_haschanged=1;
							}
								Image2=bin_img(Image,seuil,couleur_down,couleur_up);
						}
						else if(v.lux){
							if(seuil < 255){
								seuil++;
								v.seuil_haschanged=1;
							} else if(seuil==255){
								seuil=0;
								v.seuil_haschanged=1;
							}
								Image2=lux_img(Image,seuil);
						}
						break;
				}
				break;
			case SDL_KEYUP:
				if(event.key.keysym.sym==SDLK_b){

				}
				break;
		}
		
		//        keystate = SDL_GetKeyState(NULL);
		//        if(keystate[SDLK_b] && keystate[SDLK_UP] || keystate[SDLK_b] && keystate[SDLK_DOWN]){
		//            SDL_BlitSurface(Image2, NULL, screen, &posImg2);
		//            SDL_Flip(screen);
		//        }
		
		if(v.seuil_haschanged)
			printf("Seuil:%d %2x\n",seuil,seuil);
		v.seuil_haschanged=0;
		if(v.binarisation||v.lux){
			SDL_FillRect(S_seuil, NULL, SDL_MapRGB(S_seuil->format, seuil, seuil, seuil));
			SDL_BlitSurface(S_seuil,NULL,screen,&posSeuil);
		}
		else if(v.quantification){
			for(i=0;i<nb_couleurs;i++){ // Palette de couleur
				S_quant[i]=SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_SEUIL, Image->h/nb_couleurs, 32, 0, 0, 0, 0);
				SDL_FillRect(S_quant[i], NULL, couleurs[i]);
				SDL_BlitSurface(S_quant[i], NULL, screen, &posSeuil);
				posSeuil.y+=(Image->h)/nb_couleurs;
			}
			posSeuil.y=0;
		}
		SDL_BlitSurface(Image2, NULL, screen, &posImg2);
		SDL_Flip(screen);
	}
}
