#include <iostream>
#include "personnage.h"
#include "terrain.h"
#include "affichage.h"
#include "combat.h"

using namespace std;

int main(int argc, char** argv)
{

    Affichage display = Affichage();

    Combat fight = Combat();

    Terrain map = Terrain();

    Spell boom = Spell("Boom", 10, 1, 1);
    Spell oof = Spell("Oof", 15, 1, 1);
    Spell heal = Spell("Heal", 0, 2, 2, 2, 0, 10, 0);
    Spell focus = Spell("Focus", 0, 2, 0, 1, 0, 2, 0);

    vector<Spell> spells;
    spells.push_back(boom);
    spells.push_back(oof);
    spells.push_back(heal);
    spells.push_back(focus);

    vector<Personnage> persos;
    persos.push_back(Personnage(60, spells, "Michel", 0,4,9));
    persos.push_back(Personnage(40, spells, "Jean", 0, 2, 2));
    persos.push_back(Personnage(40, spells, "Thierry", 1, 4,6));

    spells.push_back(boom);
    persos.push_back(Personnage(35, spells, "Henry", 1, 14,7));

    int xmouse;
    int ymouse;

    SDL_Event evenements;

    bool end = false;
    bool freeze = false;

    MaptabP mapInt = map.getMapInt();

    //Boucle principale
    while(!end) {
        display.clearRenderer();

        SDL_GetMouseState(&xmouse,&ymouse);

        display.setFrame();

        fight.autoNewTurn(persos);

        display.displayTerrain(&mapInt);
        display.displayCharacters(persos);
        display.displaySpellRange(persos, &mapInt);
        display.displayInfoCard(persos, xmouse, ymouse);
        display.displayMenu(persos);
        
        freeze = display.displayTeam(fight.getTeam());

        fight.decreaseWait();

        SDL_PollEvent(&evenements);
        switch(evenements.type)
        {
            case SDL_QUIT:
                end = 1; break;
            case SDL_KEYDOWN:
                if (!freeze)
                {
                    switch (evenements.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        end = 1; break;
                    case SDLK_UP:
                        fight.move(persos, 0, &mapInt);
                        break;
                    case SDLK_DOWN:
                        fight.move(persos, 1, &mapInt);
                        break;
                    case SDLK_RIGHT:
                        fight.move(persos, 2, &mapInt);
                        break;
                    case SDLK_LEFT:
                        fight.move(persos, 3, &mapInt);
                        break;
                    case SDLK_SPACE:
                        fight.shiftAction(persos, &mapInt);
                        break;
                    case SDLK_RETURN:
                        fight.switchTeams(persos);
                        break;
                    }
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    if (!freeze)
                    {
                        switch (evenements.button.button)
                        {
                        case SDL_BUTTON_LEFT:
                            fight.select(persos, xmouse, ymouse);
                            break;
                        case SDL_BUTTON_RIGHT:
                            break;
                        }
                    }
                }
        }

        display.displayRenderer();
    }

    display.desallouer();

    return 0;
}
