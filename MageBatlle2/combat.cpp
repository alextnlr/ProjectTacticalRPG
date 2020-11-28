#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "fonctions_fichier.h"
#include "fonctions_SDL.h"
#include "personnage.h"
#include "combat.h"

using namespace std;

Combat::Combat()
{
    m_mouvement = false;
}

void Combat::switchMvtWait(bool physical_frame, int frameNum)
{
    if(physical_frame && frameNum%2) {
        m_mouvement = false;
    }
}

void Combat::move(vector<Personnage> &allies, bool physicalFrame, int dir, int** map, int lig, int col)
{
    for (unsigned i = 0; i < allies.size(); i++)
    {
        if (!allies[i].getWait() && allies[i].getState()>=0 && physicalFrame)
        {
            allies[i].walk(dir, map, lig, col);
        }
    }
}

void Combat::select(vector<Personnage> &allies, int xmouse, int ymouse)
{
    for (unsigned i = 0; i < allies.size(); i++)
    {
        allies[i].setState(-1);
        if (xmouse / 64 == allies[i].getCoord().x / 64 && ymouse / 64 == allies[i].getCoord().y / 64)
        {
            allies[i].setState(0);
        }
    }
}

