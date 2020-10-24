#include <SDL2/SDL.h>
#include <iostream>

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

SDL_Rect hitbox_update(SDL_Rect rec) {

    SDL_Rect hit;
    hit.x = rec.x+rec.w/5;
    hit.y = rec.y+rec.h/5;
    hit.w = rec.w-rec.w*2/5;
    hit.h = rec.h-rec.h*2/5;

    return hit;
}
