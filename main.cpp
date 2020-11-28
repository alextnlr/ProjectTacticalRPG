#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "fonctions_fichier.h"
#include "fonctions_SDL.h"
#include "personnage.h"
#include "terrain.h"
#include "spell.h"
#include "affichage.h"
#include "combat.h"

using namespace std;

int main(int argc, char** argv)
{
    /*SDL_Window* fenetre;
    SDL_Event evenements;
    int terminer = 0;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Erreur init SDL: " << SDL_GetError() << endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if(TTF_Init() < 0)
    {
        cout << "Erreur init TTF: " << TTF_GetError() << endl;
        TTF_Quit();
        return EXIT_FAILURE;
    }

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
    }*/

    Affichage display = Affichage();
    Combat fight = Combat();

    Terrain map = Terrain();

    Spell spark = Spell();

    vector<Personnage> allies;
    allies.push_back(Personnage(60, spark, (char*)"Michel"));
    allies.push_back(Personnage(40, spark, (char*)"Jean", 2, 2));

    vector<Personnage> ennemies;
    ennemies.push_back(Personnage(40,spark,(char*)"Thierry",4,6));
    ennemies.push_back(Personnage(35,spark,(char*)"Henry",6,8));

    int xmouse;
    int ymouse;

    SDL_Event evenements;

    bool end = false;

    //Boucle principale
    while(!end) {
        display.clearRenderer();

        SDL_GetMouseState(&xmouse,&ymouse);

        display.setFrame();

        display.displayTerrain(map.getMapInt(), map.getLig(), map.getCol());
        display.displayCharacters(allies, ennemies);
        display.displayInfoCard(allies, ennemies, xmouse, ymouse);

        SDL_PollEvent(&evenements);
        switch(evenements.type)
        {
            case SDL_QUIT:
                end = 1; break;
            case SDL_KEYDOWN:
                switch(evenements.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        end = 1; break;
                    case SDLK_UP:
                        fight.move(allies, display.getPhysicalFrame(), 0, map.getMapInt(), map.getLig(), map.getCol());
                        break;
                    case SDLK_DOWN:
                        fight.move(allies, display.getPhysicalFrame(), 1, map.getMapInt(), map.getLig(), map.getCol());
                        break;
                    case SDLK_RIGHT:
                        fight.move(allies, display.getPhysicalFrame(), 2, map.getMapInt(), map.getLig(), map.getCol());
                        break;
                    case SDLK_LEFT:
                        fight.move(allies, display.getPhysicalFrame(), 3, map.getMapInt(), map.getLig(), map.getCol());
                        break;
                    case SDLK_SPACE:
                        //map1.switchMode();
                        break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    switch(evenements.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            fight.select(allies, xmouse, ymouse);
                            break;
                        case SDL_BUTTON_RIGHT:
                            break;
                    }
                }
        }

        display.displayRenderer();
    }

    //Quitter SDL

    map.desallouer();
    display.desallouer();

    return 0;
}
