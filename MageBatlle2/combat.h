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
    void move(vector<Personnage> &allies, bool physicalFrame, int dir, int** map, int lig, int col);
    void select(vector<Personnage> &allies, int xmouse, int ymouse);
private:
    bool m_mouvement;
};

#endif // COMBAT_H_INCLUDED
