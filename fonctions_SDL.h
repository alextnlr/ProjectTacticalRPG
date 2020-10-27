#ifndef FONCTIONS_SDL_H_INCLUDED
#define FONCTIONS_SDL_H_INCLUDED

SDL_Texture* charger_image(const char* nomFichier, SDL_Renderer* renderer);
SDL_Texture* charger_image_transparente(const char* nomFichier, SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b);

SDL_Rect hitbox_update(SDL_Rect rec);

SDL_Texture* charger_texte(const char* message, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color);

#endif // FONCTIONS_SDL_H_INCLUDED
