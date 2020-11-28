#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "fonctions_SDL.h"

using namespace std;

SDL_Texture* charger_image(const char* nomFichier, SDL_Renderer* renderer) {

    SDL_Surface* image = SDL_LoadBMP(nomFichier);
    if(!image)
    {
        cout << "Erreur de chargement de " << nomFichier << " : " <<SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }
    SDL_Texture* monImage = SDL_CreateTextureFromSurface(renderer,image);  //La texture monImage contient maintenant l'image importée

    return monImage;

}

SDL_Texture* charger_image_transparente(const char* nomFichier, SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b) {

    SDL_Surface* image = SDL_LoadBMP(nomFichier);
    if(!image)
    {
        cout << "Erreur de chargement de " << nomFichier <<" : " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    Uint32 pixel = SDL_MapRGB(image->format, r, g, b);
    SDL_SetColorKey(image, SDL_TRUE, pixel);
    SDL_Texture* monImage = SDL_CreateTextureFromSurface(renderer,image);

    return monImage;

}

SDL_Texture* charger_texte(const char* message, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color) {

    SDL_Surface* text = TTF_RenderText_Solid(font, message, color);

    if(!text)
    {
        cout << "Erreur de chargement de " << message <<" : " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    SDL_Texture* monTexte = SDL_CreateTextureFromSurface(renderer,text);

    return monTexte;
}
