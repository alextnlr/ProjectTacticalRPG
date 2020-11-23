#ifndef COMBAT_H_INCLUDED
#define COMBAT_H_INCLUDED

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "personnage.h"

using namespace std;

class Combat
{
public:
    Combat();
    void switchMvtWait(bool physical_frame, int frameNum);
    int getSelect() const;
private:
    int m_select;
    bool m_wait;
    bool m_mouvement;
};

#endif // COMBAT_H_INCLUDED
