#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "fonctions_fichier.h"
#include "fonctions_SDL.h"
#include "personnage.h"
#include "terrain.h"

using namespace std;

int main(int argc, char** argv)
{
    SDL_Window* fenetre;
    SDL_Event evenements;
    int terminer = 0;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Erreur init SDL: " << SDL_GetError() << endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    TTF_Init();

    //Creation de fenetre
    int width = 22*64;
    int height = 10*64;
    fenetre = SDL_CreateWindow("MageBattle",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_RESIZABLE);
    if(fenetre == NULL) {
        cout << "Erreur creation fenetre: " << SDL_GetError() << endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    //Mise en place du renderer
    SDL_Renderer* ecran;
    ecran = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);

    Uint8 r = 128, g = 160, b = 128;
    SDL_Texture* mageTexture = charger_image_transparente("MageM.bmp",ecran, r, g, b);

    //Texte ttf
    TTF_Font* fontText = TTF_OpenFont("prstartk.ttf",16);
    if(!fontText) {
        cout << "Erreur du chargement de la font" << endl;
    }

    vector<Personnage> allies;
    allies.push_back(Personnage(60, 5, "Michel", mageTexture));
    allies.push_back(Personnage(40, 10, "Jean", 2, 2, mageTexture));

    Terrain map1(ecran, allies);

    map1.ajouterEnnemi(40,8,"Thierry",4,6);
    map1.ajouterEnnemi(35,4,"Henry",6,8);

    int xmouse;
    int ymouse;

    //Boucle principale
    while(!terminer) {
        SDL_RenderClear(ecran);

        SDL_GetMouseState(&xmouse,&ymouse);

        map1.setFrame();

        map1.afficherTerrain(ecran);
        map1.afficherPersos(ecran);
        map1.afficherDegats(ecran,fontText);
        map1.afficherAgro(ecran);
        map1.afficherInfos(ecran,fontText,xmouse,ymouse);

        map1.switchMvtWait();

        SDL_PollEvent(&evenements);
        switch(evenements.type)
        {
            case SDL_QUIT:
                terminer = 1; break;
            case SDL_KEYDOWN:
                switch(evenements.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        terminer = 1; break;
                    case SDLK_UP:
                        map1.deplacerSelect(0);
                        break;
                    case SDLK_DOWN:
                        map1.deplacerSelect(1);
                        break;
                    case SDLK_RIGHT:
                        map1.deplacerSelect(2);
                        break;
                    case SDLK_LEFT:
                        map1.deplacerSelect(3);
                        break;
                    case SDLK_SPACE:
                        map1.switchMode();
                        break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    switch(evenements.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            map1.select(xmouse,ymouse);
                            break;
                        case SDL_BUTTON_RIGHT:
                            map1.deselect();
                            break;
                    }
                }
        }

        SDL_RenderPresent(ecran);
        SDL_Delay(1000./60.);
    }

    //Quitter SDL
    map1.desallouer();
    for(int i = allies.size()-1 ; i >= 0; i--)
    {
        allies[i].desallouer();
    }
    TTF_CloseFont(fontText);
    TTF_Quit();
    SDL_DestroyTexture(mageTexture);
    SDL_DestroyRenderer(ecran);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();

    return 0;
}
