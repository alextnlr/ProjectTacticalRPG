#include <iostream>
#include <ctime>
#include "Character.h"
#include "Terrain.h"
#include "Display.h"
#include "Fight.h"

using namespace std;

//The main is commented a little too much to avoid annoying comments on the other more importants parts
int main(int argc, char** argv)
{
    //Initialisation of the random seed
    std::srand(std::time(nullptr));

    //Creation of all the classes
    Display display = Display();
    Fight fight = Fight();
    Terrain map = Terrain();

    //We stock the map inside a local variable, even if it's contrary to the principles of OOP it's really easier for now
    MaptabP mapInt = map.getMapInt();

    //Creation of Random Characters
    vector<Character> characters = fight.createCharacter(&mapInt);
    
    //Creation of all the local variables
    int xmouse;
    int ymouse;

    SDL_Event event;

    bool end = false; //Checking if the game is ending
    bool freeze = false; //Checking if the user can interact
    bool waitTurn = false; //Checking if all the animations are completed before going on an other turn

    int roll = 0; //Current roll, by default 0 to not display anything
    int victory = -1; //Checking if one team is victorious

    //Game Loop
    while(!end) {
        display.clearRenderer(); //Always cleaning the screen on every frames

        SDL_GetMouseState(&xmouse,&ymouse); //Stocking the position of the mouse every frames

        display.setFrame(); //Checking if this frame is a physical frame

        victory = fight.checkForVictory(characters); //Checking if the game is over

        if (victory < 0)
        {
            //Drawing on screen Terrain, Character and spell range (if active)
            display.displayTerrain(&mapInt);
            display.displaySpellRange(characters, &mapInt);
            display.displayCharacters(characters);

            //Drawing info card and menu but only if there's no major animation going on
            if (!freeze)
            {
                display.displayInfoCard(characters, xmouse, ymouse);
                display.displayMenu(characters, &mapInt);
            }

            //Checking if there is animation going on
            waitTurn = display.displayDamages(characters, roll);
            freeze = display.displayRoll(roll);

            //Checking if the temp playing has nothing left to do 
            if (!waitTurn)
            {
                fight.autoNewTurn(characters);
                freeze = display.displayTeam(fight.getTeam(), roll);
            }
        }
        else
        {
            //Displaying the end card
            display.displayEndMenu(victory);
        }

        fight.decreaseWait();

        //Checking for inputs
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT: //Window cross
                end = 1; break;
            case SDL_KEYDOWN:
                if (!freeze)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE: //Cancel or quit
                        if (!fight.cancel(characters))
                        {
                            end = true;
                        }
                        break;
                    case SDLK_UP:
                        fight.move(characters, 0, &mapInt);
                        break;
                    case SDLK_DOWN:
                        fight.move(characters, 1, &mapInt);
                        break;
                    case SDLK_RIGHT:
                        fight.move(characters, 2, &mapInt);
                        break;
                    case SDLK_LEFT:
                        fight.move(characters, 3, &mapInt);
                        break;
                    case SDLK_SPACE:
                        if (!roll) //if to avoid cancelling the roll animation
                        {
                            roll = fight.shiftAction(characters, &mapInt);
                        }
                        break;
                    case SDLK_RETURN: //Manually switch turn
                        fight.switchTeams(characters);
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
                            fight.select(characters, xmouse, ymouse); //Select with the mouse (may change)
                            break;
                        case SDL_BUTTON_RIGHT:
                            break;
                        }
                    }
                }
        }

        display.displayRenderer(); //Update the window
    }

    display.desallouer(); //Yeeting the SDL thingies

    return 0;
}
