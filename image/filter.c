#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rotozoom.h>
#include <stdio.h>

// Fonction pour faire pivoter une texture SDL_Texture
SDL_Texture* rotateTexture(SDL_Renderer* renderer, SDL_Texture* texture, double angle) {
    // Créer une surface à partir de la texture
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);

    // Copier la texture sur la surface
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, surface->pixels, surface->pitch);

    // Rotation de la surface
    SDL_Surface* rotatedSurface = rotozoomSurface(surface, angle, 1.0, 0);
    if (rotatedSurface == NULL) {
        printf("La rotation de l'image a échoué : %s\n", SDL_GetError());
        return NULL;
    }

    // Créer une texture à partir de la surface rotatée
    SDL_Texture* rotatedTexture = SDL_CreateTextureFromSurface(renderer, rotatedSurface);

    // Libérer les surfaces intermédiaires
    SDL_FreeSurface(surface);
    SDL_FreeSurface(rotatedSurface);

    return rotatedTexture;
}

int main(int argc, char ** argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL n'a pas pu être initialisé : %s\n", SDL_GetError());
        return 1;
    }

    // Création d'une fenêtre SDL
    SDL_Window* window = SDL_CreateWindow("Rotation d'image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("La fenêtre SDL n'a pas pu être créée : %s\n", SDL_GetError());
        return 1;
    }

    // Création d'un rendu pour la fenêtre
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Le rendu SDL n'a pas pu être créé : %s\n", SDL_GetError());
        return 1;
    }

    // Chargement d'une image avec SDL_Image
    SDL_Surface* imageSurface = IMG_Load("chemin/vers/votre/image.jpg");
    if (imageSurface == NULL) {
        printf("L'image n'a pas pu être chargée : %s\n", IMG_GetError());
        return 1;
    }

    // Création d'une texture à partir de l'image
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if (imageTexture == NULL) {
        printf("La texture SDL n'a pas pu être créée : %s\n", SDL_GetError());
        return 1;
    }

    // Libération de la surface de l'image, car nous n'en avons plus besoin
    SDL_FreeSurface(imageSurface);

    // Angle de rotation en degrés
    double angle = 45.0;

    // Boucle principale
    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Effacer le rendu
        SDL_RenderClear(renderer);

        // Afficher l'image avec rotation
        SDL_Texture* rotatedTexture = rotateTexture(renderer, imageTexture, angle);
        if (rotatedTexture != NULL) {
            SDL_RenderCopy(renderer, rotatedTexture, NULL, NULL);
            SDL_DestroyTexture(rotatedTexture);
        }

        // Mettre à jour le rendu
        SDL_RenderPresent(renderer);
    }

    // Libérer la texture et le rendu
    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(renderer);

    // Fermer la fenêtre SDL
    SDL_DestroyWindow(window);

    // Quitter SDL_Image et SDL
    IMG_Quit();
    SDL_Quit();

    return 0;
}
