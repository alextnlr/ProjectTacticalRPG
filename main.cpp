#include <iostream>
#include <ctime>
#include "personnage.h"
#include "terrain.h"
#include "affichage.h"
#include "combat.h"

using namespace std;

int main(int argc, char** argv)
{

    std::srand(std::time(nullptr));

    Affichage display = Affichage();

    Combat fight = Combat();

    Terrain map = Terrain();

    Spell boom = Spell("Boom", "1d6+2", Line, 1);
    Spell oof = Spell("Oof", "2d8+0", Line, 1);
    Spell heal = Spell("Heal", "0d0+0", Self, 2, Heal, 15);
    Spell focus = Spell("Focus", "0d0+0", Self, 0, Focus, 2);
    Spell shockwave = Spell("Shockwave", "2d10+5", Shock, 2);

    vector<Spell> spells;
    spells.push_back(boom);
    spells.push_back(oof);
    spells.push_back(heal);
    spells.push_back(focus);
    spells.push_back(shockwave);

    vector<Personnage> persos;
    persos.push_back(Personnage(60, 12, spells, "Michel", 0,4,9));
    persos.push_back(Personnage(40, 12, spells, "Jean", 0, 2, 2));
    persos.push_back(Personnage(40, 12, spells, "Thierry", 1, 4,6));
    persos.push_back(Personnage(35, 12, spells, "Henry", 1, 14,7));

    int xmouse;
    int ymouse;

    SDL_Event event;

    bool end = false;
    bool freeze = false;
    bool waitTurn = false;

    MaptabP mapInt = map.getMapInt();

    int roll = 0;

    //Boucle principale
    while(!end) {
        display.clearRenderer();

        SDL_GetMouseState(&xmouse,&ymouse);

        display.setFrame();

        display.displayTerrain(&mapInt);
        display.displayCharacters(persos);
        display.displaySpellRange(persos, &mapInt);
        display.displayInfoCard(persos, xmouse, ymouse);
        display.displayMenu(persos);
        
        waitTurn = display.displayDamages(persos, roll);
        freeze = display.displayRoll(roll);
        if (!waitTurn)
        {
            fight.autoNewTurn(persos);
            freeze = display.displayTeam(fight.getTeam(), roll);
        }

        fight.decreaseWait();

        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                end = 1; break;
            case SDL_KEYDOWN:
                if (!freeze)
                {
                    switch (event.key.keysym.sym)
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
                        roll = fight.shiftAction(persos, &mapInt);
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
                        switch (event.button.button)
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
