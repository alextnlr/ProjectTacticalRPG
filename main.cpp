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

    vector<Character> persos = fight.createCharacter(map.getMapInt());

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
