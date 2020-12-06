#include <iostream>
#include <ctime>
#include "Character.h"
#include "terrain.h"
#include "Display.h"
#include "Fight.h"

using namespace std;

int main(int argc, char** argv)
{

    std::srand(std::time(nullptr));

    Display display = Display();

    Fight fight = Fight();

    Terrain map = Terrain();

    Spell boom = Spell("Boom", "1d6+2", Line, 1, ShieldDown, 2);
    Spell oof = Spell("Oof", "2d8+0", Line, 1);
    Spell heal = Spell("Heal", "0d0+0", Self, 2, Heal, 15);
    Spell focus = Spell("Focus", "0d0+0", Self, 0, Focus, 2);
    Spell shield = Spell("Shield", "0d0+0", Self, 1, ShieldUp, 2);
    Spell shockwave = Spell("Shockwave", "2d10+5", Shock, 2);

    vector<Spell> spells;
    spells.push_back(boom);
    spells.push_back(oof);
    spells.push_back(heal);
    spells.push_back(focus);
    spells.push_back(shield);
    spells.push_back(shockwave);

    vector<Character> persos;
    persos.push_back(Character(60, 12, spells, "Michel", 0,4,9));
    persos.push_back(Character(40, 12, spells, "Jean", 0, 2, 2));
    persos.push_back(Character(40, 12, spells, "Melissa", 1, 4,6));
    persos.push_back(Character(35, 12, spells, "Jeanne", 1, 14,7));

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
        display.displaySpellRange(persos, &mapInt);
        display.displayCharacters(persos);
        
        if (!freeze)
        {
            display.displayInfoCard(persos, xmouse, ymouse);
            display.displayMenu(persos);
        }
        
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
                        if (!fight.cancel(persos))
                        {
                            end = true;
                        }
                        break;
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
                        if (!roll)
                        {
                            roll = fight.shiftAction(persos, &mapInt);
                        }
                        
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
