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

    Affichage display = Affichage();

    Combat fight = Combat();

    Terrain map = Terrain();

    Spell spark = Spell();
    Spell boom = Spell((char*)"boom", 10, 1, 1);

    vector<Personnage> allies;
    allies.push_back(Personnage(60, spark, (char*)"Michel", 0,4,9));
    allies.push_back(Personnage(40, boom, (char*)"Jean", 0, 2, 2));

    vector<Personnage> ennemies;
    ennemies.push_back(Personnage(40, spark,(char*)"Thierry", 1, 4,6));
    ennemies.push_back(Personnage(35, spark,(char*)"Henry", 1, 14,7));

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
        display.displaySpellRange(allies, map.getMapInt(), map.getLig(), map.getCol());
        display.displayInfoCard(allies, ennemies, xmouse, ymouse);

        fight.decreaseWait();

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
                        fight.move(allies, ennemies, display.getPhysicalFrame(), 0, map.getMapInt(), map.getLig(), map.getCol());
                        break;
                    case SDLK_DOWN:
                        fight.move(allies, ennemies, display.getPhysicalFrame(), 1, map.getMapInt(), map.getLig(), map.getCol());
                        break;
                    case SDLK_RIGHT:
                        fight.move(allies, ennemies, display.getPhysicalFrame(), 2, map.getMapInt(), map.getLig(), map.getCol());
                        break;
                    case SDLK_LEFT:
                        fight.move(allies, ennemies, display.getPhysicalFrame(), 3, map.getMapInt(), map.getLig(), map.getCol());
                        break;
                    case SDLK_SPACE:
                        fight.shiftAction(allies, ennemies, map.getMapInt(), map.getLig(), map.getCol());
                        break;
                    case SDLK_RETURN:
                        fight.switchTeams(allies, ennemies);
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
