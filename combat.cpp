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
    m_wait = false;
    m_select = -1;
}

void Combat::switchMvtWait(bool physical_frame, int frameNum)
{
    if(physical_frame && frameNum%2) {
        m_mouvement = false;
        m_wait = false;
    }
}

int Combat::getSelect() const
{
    return m_select;
}
